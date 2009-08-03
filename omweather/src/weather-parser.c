/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include "weather-common.h"
#include "weather-parser.h"
#include "weather-hash.h"
#include "weather-data.h"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
enum { RP5RU_DAY_BEGIN = 7, RP5RU_NIGHT_BEGIN = 15 };
/*******************************************************************************/
gint 
parse_weather_file_data(const gchar *station_id, gchar *station_source,
					WeatherStationData *wsd,
					    gboolean selected_detail_weather){
    gchar		buffer[2048],
			newname[2048];
    const gchar		*format = ".xml",
			*hour_format = "_hour.xml";
    weather_com_parser	*parser = NULL;
    gint 		(*handler)(const gchar *station_id,
				    weather_com_parser *parser,
				    WeatherStationData *wsd);
    gint		source = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* check storage is aviable */
    if(!wsd || !station_id || !station_source)
	return -1;
    if(!strcmp(station_source, "weather.com"))
	source = 0;
    if(!strcmp(station_source, "rp5.ru"))
	source = 1;
/* init parser */
    if(selected_detail_weather)
	handler = weather_sources[source].parser_hour;
    else
	handler = weather_sources[source].parser;
    if(!handler)
        return -1;
/* Used new file */
    if(selected_detail_weather)
	sprintf(buffer, "%s/%s%s.new", app->config->cache_dir_name, station_id,
			hour_format);
    else
	sprintf(buffer, "%s/%s%s.new", app->config->cache_dir_name, station_id,
			format);
    if(!access(buffer, R_OK)){  /* Not Access to cache weather xml file */
	parser = weather_parser_new_from_file(buffer,
				weather_sources[source].encoding); 
	if(!(parser->error)){
	    if(selected_detail_weather)
		sprintf(newname, "%s/%s%s", app->config->cache_dir_name,
			station_id, hour_format);
	    else
		sprintf(newname, "%s/%s%s", app->config->cache_dir_name,
			station_id, format);
	    rename(buffer, newname);
	}
    }
/* Used old xml file */
    if(( !parser && (access(buffer, R_OK) != 0)) || (parser && parser->error)){
	if(parser){
	    free(parser);
	    parser = NULL;
	}
	if(selected_detail_weather)
	    sprintf(buffer, "%s/%s%s", app->config->cache_dir_name,
			station_id, hour_format);
	else
	    sprintf(buffer, "%s/%s%s", app->config->cache_dir_name,
			station_id, format);
/* Not Access to cache weather xml file or not valid XML file */
	if(!access(buffer, R_OK)){ 
	    parser = weather_parser_new_from_file(buffer,
				weather_sources[source].encoding);
	    if(parser->error){
		free(parser);
		parser = NULL;
		return -1; 
	    }
	}
	else{
	    if(parser){
		free(parser);
		parser = NULL;
	    }
	    return -1;
	}
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return (handler(station_id, parser, &(app->wsd)));
}
/*******************************************************************************/
weather_com_parser*
weather_parser_new_from_file(const gchar *filename,
							const gchar *encoding){
    weather_com_parser	*parser = NULL;
    xmlNode     *cur_node = NULL;
    xmlChar     *temp_xml_string1,
                *temp_xml_string2;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    parser = (weather_com_parser *) malloc(sizeof(weather_com_parser));
    if(!parser)
	return NULL;
    parser->doc = NULL;
    parser->doc = xmlReadFile( filename, encoding, 0 );
    if(!(parser->doc)){
        parser->error = TRUE;
        parser->weather_com_root = NULL;
    }
    else{
        parser->error = FALSE;
	    parser->weather_com_root = xmlDocGetRootElement(parser->doc);
        /* Check internal error in server in xml file */
        cur_node = parser->weather_com_root->children;
        cur_node = cur_node->next;
        if( cur_node->type == XML_ELEMENT_NODE ){
            temp_xml_string1 = xmlGetProp(cur_node, (const xmlChar*)"type");
            temp_xml_string2 = xmlNodeGetContent(cur_node);
            if(temp_xml_string1 && temp_xml_string2 &&
                !strcmp((char*)temp_xml_string1, "0") &&
                !strcmp((char*)temp_xml_string2, "An unknown error has occurred.")){
                    parser->error = TRUE;
                    parser->weather_com_root = NULL;
            }
            if (temp_xml_string1)
                xmlFree(temp_xml_string1);
            if (temp_xml_string2)
                xmlFree(temp_xml_string2);
        }   
    }
    
#ifdef DEBUGFUNCTIONCALL
      END_FUNCTION;
#endif
    return parser;
}
/*******************************************************************************/
gint parse_weather_com_xml(const gchar *station_id, weather_com_parser *parser,
							WeatherStationData *wsd){
    xmlNode	*cur_node = NULL,
		*child_node = NULL,
		*child_node2 = NULL,
		*child_node3 = NULL,
		*child_node4 = NULL; 
    xmlChar	*temp_xml_string;
    xmlChar	*part_of_day = NULL;
    gint	store2day = 0,
		count_day = 0;
    gchar	id_station[10],
		buff[256];
    struct tm	tmp_tm = {0};
    Item	*itm;
    GSList	*day = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    for(cur_node = parser->weather_com_root->children; cur_node; cur_node = cur_node->next){
	if( cur_node->type == XML_ELEMENT_NODE ){
    	    /* Check error */
	    if(!xmlStrcmp(cur_node->name, (const xmlChar *) "err" ) ){
	        xmlFreeDoc(parser->doc);
		xmlCleanupParser();
		free(parser);
		return -2;
	    }
	    /* get weather station data */
	    if(!xmlStrcmp(cur_node->name, (const xmlChar *) "loc" ) ){
		temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"id");
		snprintf(id_station,
			    sizeof(id_station) - 1,
			    "%s", temp_xml_string);
		fprintf(stderr, "\n<<<<<<<<<<<<<<<<<<ID_STATION %s\n", temp_xml_string);
		xmlFree(temp_xml_string);
		/* If station in xml not station in config file exit */ 
		if( strcmp(id_station, station_id) ){
		    free(parser);
		    return -1;
		}
		for(child_node = cur_node->children; child_node; child_node = child_node->next){
		    if( child_node->type == XML_ELEMENT_NODE ){
		/* station name */
			if( !xmlStrcmp(child_node->name, (const xmlChar *)"dnam") ){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    itm = create_item("station_name", (char*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&(wsd->location), itm);
			    continue;
			}
		/* station lattitude */
			if( !xmlStrcmp(child_node->name, (const xmlChar *)"lat") ){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    itm = create_item("station_latitude", (char*)temp_xml_string);
                    	    xmlFree(temp_xml_string);
			    add_item2object(&(wsd->location), itm);
			    continue;
        		}
		/* station longitude */
			if( !xmlStrcmp(child_node->name, (const xmlChar *)"lon") ){
                    	    temp_xml_string = xmlNodeGetContent(child_node);
			    itm = create_item("station_longitude", (char*)temp_xml_string);
                    	    xmlFree(temp_xml_string);
			    add_item2object(&(wsd->location), itm);
			    continue;
        		}
		/* station time zone */
			if( !xmlStrcmp(child_node->name, (const xmlChar *)"zone") ){
                    	    temp_xml_string = xmlNodeGetContent(child_node);
			    itm = create_item("station_time_zone", (char*)temp_xml_string);
                    	    xmlFree(temp_xml_string);
			    add_item2object(&(wsd->location), itm);
			    continue;
        		}
		    }
		}
	    }
	/* Fill current day */
	    if(!xmlStrcmp(cur_node->name, (const xmlChar *) "cc" ) ){
		for(child_node = cur_node->children; child_node != NULL; child_node = child_node->next){
		    /* last update */
		    if( child_node->type == XML_ELEMENT_NODE  &&
			( !xmlStrcmp(child_node->name, (const xmlChar *)"lsup") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("last_update", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wsd->current), itm);
			continue;
		    }
		    /* temperature */
		    if( child_node->type == XML_ELEMENT_NODE  &&
        		    ( !xmlStrcmp(child_node->name, (const xmlChar *)"tmp") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("day_hi_temperature", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wsd->current), itm);
			continue;
		    }
		    /* feel like */
		    if( child_node->type == XML_ELEMENT_NODE  &&
            		    ( !xmlStrcmp(child_node->name, (const xmlChar *)"flik") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("feel_like", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wsd->current), itm);
			continue;
		    }
		    /* title */
		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"t") ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("title", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wsd->current), itm);
			continue;
		    }
		    /* icon */
    		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"icon") ){
			temp_xml_string = xmlNodeGetContent(child_node);
    			itm = create_item("icon", (char*)temp_xml_string);
			if(!strcmp((char*)temp_xml_string, "-"))
			    wsd->current_data_is_invalid = TRUE;
			else
			    wsd->current_data_is_invalid = FALSE;
			xmlFree(temp_xml_string);
			add_item2object(&(wsd->current), itm);
			continue;
        	    }
		    /* pressure data */
    		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"bar") ){
			for(child_node2 = child_node->children; child_node2 != NULL; child_node2 = child_node2->next){
    			    if( child_node2->type == XML_ELEMENT_NODE ){
				/* pressure */
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"r") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("pressure", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wsd->current), itm);
				}
				/* direction */
    				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"d") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("pressure_direction", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wsd->current), itm);
				}
			    }
        		}
			continue;
        	    }
		    /* humidity */
		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"hmid") ){
    			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("humidity", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wsd->current), itm);
			continue;
		    }
		    /* visible */
    		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"vis") ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("visible", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wsd->current), itm);
			continue;
		    }
		    /* wind data */
		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"wind") ){
			for(child_node2 = child_node->children; child_node2 != NULL; child_node2 = child_node2->next){
    			    if( child_node2->type == XML_ELEMENT_NODE ){
				/* speed */
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"s") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("wind_speed", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wsd->current), itm);
				}
				/* gust */
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"gust") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("wind_gust", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wsd->current), itm);
				}
				/* direction */
    				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("wind_direction", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wsd->current), itm);
				}
			    }
        		}
			continue;
        	    }
		    /* Moon data */
		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"moon") ){
			for(child_node2 = child_node->children; child_node2 != NULL; child_node2 = child_node2->next){
    			    if( child_node2->type == XML_ELEMENT_NODE ){
				/* icon */
/* not used now
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"icon") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("moon_icon", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wsd->current), itm);
				}
*/
				/* phase */
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("moon_phase", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wsd->current), itm);
				}
			    }
        		}
			continue;
        	    }
		}
	    }
	    /* Fill other days */
	    if(!xmlStrcmp(cur_node->name, (const xmlChar *) "dayf" ) ){
		for(child_node = cur_node->children; child_node; child_node = child_node->next){
		    if( child_node->type == XML_ELEMENT_NODE  &&
			    ( !xmlStrcmp(child_node->name, (const xmlChar *)"day") ) ){
			/* get 24h name */
			temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"t");
			/* prepare locale value for day name */
			memset(buff, 0, sizeof(buff));
			memcpy(buff, temp_xml_string, (strlen((char*)temp_xml_string) > sizeof(buff)) ?
						      (sizeof(buff) - 1) : 
						      (strlen((char*)temp_xml_string)));
			strptime(buff, "%A", &tmp_tm);
			memset(buff, 0, sizeof(buff));
			strftime(buff, sizeof(buff) - 1, "%a", &tmp_tm);
			itm = create_item("day_name", buff);
			xmlFree(temp_xml_string);
			add_item2object(&day, itm);
			/* get 24h date */
			temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"dt");
			itm = create_item("day_date", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&day, itm);
			
			for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
			    if( child_node2->type == XML_ELEMENT_NODE){
				/* 24h hi temperature */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"hi")){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("day_hi_temperature", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&day, itm);
				    continue;
				}
				/* 24h low temperature */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"low")){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("day_low_temperature", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&day, itm);
				    continue;
				}
				/* 24h sunrise */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"sunr")){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("day_sunrise", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&day, itm);
				    continue;
				}
				/* 24h sunset */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"suns")){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("day_sunset", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&day, itm);
				    continue;
				}
				/* 24h part */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"part")){
				    part_of_day = xmlGetProp(child_node2, (const xmlChar*)"p");
				    if( !xmlStrcmp(part_of_day, (const xmlChar *)"d") )
					store2day = 1;
				    else
					store2day = 0;
				    xmlFree(part_of_day);
				    for(child_node3 = child_node2->children; child_node3; child_node3 = child_node3->next){
					if( child_node3->type == XML_ELEMENT_NODE){
					    /* humidity */
					    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"hmid") ){
						temp_xml_string = xmlNodeGetContent(child_node3);
						if(!store2day)
						    itm = create_item("night_humidity", (char*)temp_xml_string);
						else
						    itm = create_item("day_humidity", (char*)temp_xml_string);
						xmlFree(temp_xml_string);
						add_item2object(&day, itm);
						continue;
					    }
					    /* ppcp */
					    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"ppcp") ){
						temp_xml_string = xmlNodeGetContent(child_node3);
						if(!store2day)
						    itm = create_item("night_ppcp", (char*)temp_xml_string);
						else
						    itm = create_item("day_ppcp", (char*)temp_xml_string);
						xmlFree(temp_xml_string);
						add_item2object(&day, itm);
						continue;
					    }
					    /* title */
					    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"t") ){
						temp_xml_string = xmlNodeGetContent(child_node3);
						if(!store2day)
						    itm = create_item("night_title", (char*)temp_xml_string);
						else
						    itm = create_item("day_title", (char*)temp_xml_string);
						xmlFree(temp_xml_string);
						add_item2object(&day, itm);
						continue;
					    }
					    /* icon */
					    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"icon") ){
						temp_xml_string = xmlNodeGetContent(child_node3);
						if(!store2day)
						    itm = create_item("night_icon", (char*)temp_xml_string);
						else
						    itm = create_item("day_icon", (char*)temp_xml_string);
						xmlFree(temp_xml_string);
						add_item2object(&day, itm);
						continue;
					    }
					    /* wind data */
					    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"wind") ){
						for(child_node4 = child_node3->children; child_node4; child_node4 = child_node4->next){
						    if( child_node4->type == XML_ELEMENT_NODE){
							/* speed */
							if(!xmlStrcmp(child_node4->name, (const xmlChar *)"s") ){
							    temp_xml_string = xmlNodeGetContent(child_node4);
							    if(!store2day)
								itm = create_item("night_wind_speed", (char*)temp_xml_string);
							    else
							    	itm = create_item("day_wind_speed", (char*)temp_xml_string);
							    xmlFree(temp_xml_string);
							    add_item2object(&day, itm);
							    continue;
							}
							/* title */
							if(!xmlStrcmp(child_node4->name, (const xmlChar *)"t") ){
							    temp_xml_string = xmlNodeGetContent(child_node4);
							    if(!store2day)
								itm = create_item("night_wind_title",
									    (char*)temp_xml_string);
							    else
								itm = create_item("day_wind_title",
									    (char*)temp_xml_string);
							    xmlFree(temp_xml_string);
							    add_item2object(&day, itm);
							    continue;
							}
						    }
						}
					    }
					}
				    }
				}
			    }
			}
			/* add day to the days list */
			if(day){
			    add_item2object(&(wsd->days), day);
			    day = NULL;
			    count_day++;
			}
		    }
		}
	    }
	}
    }
    xmlFreeDoc(parser->doc);
    xmlCleanupParser();
    free(parser);
    return count_day;
}
/*******************************************************************************/
gint parse_weather_com_xml_hour(const gchar *station_id,
					weather_com_parser *parser,
							WeatherStationData *wsd){
    xmlNode     *cur_node = NULL,
                *child_node = NULL,
                *child_node2 = NULL,
                *child_node3 = NULL;
    xmlChar     *temp_xml_string;
    gint        count_hour = 0;
    gchar       buff[256];
    struct tm   tmp_tm = {0};
    Item        *itm;
    GSList      *hour_weather = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    for(cur_node = parser->weather_com_root->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            /* Check error */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "err" ) ){
                xmlFreeDoc(parser->doc);
                xmlCleanupParser();
                free(parser);
                return -2;
            }
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "hbhf" ) ){
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                  /* last update */
                  if( child_node->type == XML_ELEMENT_NODE  &&
			( !xmlStrcmp(child_node->name, (const xmlChar *)"lsup") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("last_update", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(hour_weather), itm);
			continue;
		  }
                  if( child_node->type == XML_ELEMENT_NODE  &&
                  (!xmlStrcmp(child_node->name, (const xmlChar *)"hour") )){
                        /*Get an hour*/
                        temp_xml_string = xmlGetProp(child_node, (const xmlChar *)"c");
                        /* prepare locale value for hour */
                        memset(buff, 0, sizeof(buff));
                        memcpy(buff, temp_xml_string, (strlen((char*)temp_xml_string)                                                                    > sizeof(buff)) ?
                                                                 (sizeof(buff) - 1) :
                                                   (strlen((char*)temp_xml_string)));
                        strptime(buff, "%H", &tmp_tm);
                        memset(buff, 0, sizeof(buff));
                        strftime(buff, sizeof(buff) - 1, "%H", &tmp_tm);
                        itm = create_item("hours", buff);
                        xmlFree(temp_xml_string);
                        add_item2object(&hour_weather, itm);
                        for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                            if( child_node2->type == XML_ELEMENT_NODE){
                                /*hour temperature*/
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"tmp"))    {
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    itm = create_item("hour_temperature", (char*)temp_xml_string);
                                    xmlFree(temp_xml_string);
                                    add_item2object(&hour_weather, itm);
                                    continue;
                               }
                               /*feels like for hour*/
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"flik")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    itm = create_item("hour_feels_like", (char*)temp_xml_string);
                                    xmlFree(temp_xml_string);
                                    add_item2object(&hour_weather, itm);
                                    continue;
                               }
                               /*icon for hour*/
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"icon")    ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    itm = create_item("hour_icon", (char*)temp_xml_string);
                                    if(!strcmp((char*)temp_xml_string, "-"))
                                        wsd->hours_data_is_invalid = TRUE;
                                    else
                                        wsd->hours_data_is_invalid = FALSE;
                                    xmlFree(temp_xml_string);
                                    add_item2object(&hour_weather, itm);
                                    continue;
                               }
                               /*title for hour*/
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    itm = create_item("hour_title", (char*)temp_xml_string);
                                    xmlFree(temp_xml_string);
                                    add_item2object(&hour_weather, itm);
                                    continue;
                               }
                               /*wind data for hour*/
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"wind")){
                                    for(child_node3 = child_node2->children; child_node3 != NULL; child_node3 = child_node3->next){
                                    if( child_node3->type == XML_ELEMENT_NODE ){
                                        /* speed */
                                        if(!xmlStrcmp(child_node3->name, (const xmlChar *)"s")){
                                            temp_xml_string = xmlNodeGetContent(child_node3);
                                            itm = create_item("hour_wind_speed", (char*)temp_xml_string);
                                            xmlFree(temp_xml_string);
                                            add_item2object(&hour_weather, itm);
                                     }
                                     /* gust */
                                     if(!xmlStrcmp(child_node3->name, (const xmlChar *)"gust") ){
                                        temp_xml_string = xmlNodeGetContent(child_node3);
                                        itm = create_item("hour_wind_gust", (char*)temp_xml_string);
                                        xmlFree(temp_xml_string);
                                        add_item2object(&hour_weather, itm);
                                     }
                                     /* direction */
                                     if(!xmlStrcmp(child_node3->name, (const xmlChar *)    "t")){
                                        temp_xml_string = xmlNodeGetContent(child_node3);
                                        itm = create_item("hour_wind_direction", (char*)temp_xml_string);
                                        xmlFree(temp_xml_string);
                                        add_item2object(&hour_weather, itm);
                                     }
                                }
                             }
                         }
                         /* hour humidity */
                         if(!xmlStrcmp(child_node2->name, (const xmlChar *)"hmid") ){
                             temp_xml_string = xmlNodeGetContent(child_node2);
                             itm = create_item("hour_humidity", (char*)temp_xml_string);
                             xmlFree(temp_xml_string);
                             add_item2object(&hour_weather, itm);
                             continue;
                         }
                    }
                 }
                 if(hour_weather){
                    add_item2object(&(wsd->hours_weather), hour_weather);
                    hour_weather = NULL;
                    count_hour++;
                 }
             }
         }
    }
 }
}
    xmlFreeDoc(parser->doc);
    xmlCleanupParser();
    free(parser);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif    
    return count_hour;
}
/*******************************************************************************/
