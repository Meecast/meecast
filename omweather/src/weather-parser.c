/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
enum { RP5RU_DAY_BEGIN = 3, RP5RU_NIGHT_BEGIN = 15 };
/*******************************************************************************/
gint parse_weather_file_data(const gchar *station_id, const gint station_source,
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
    gint		source;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* check storage is aviable */
    if(!wsd || !station_id)
	return -1;
    (station_source < 0) ? (source = app->config->weather_source)
			 : (source = station_source);
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
    if(( !parser && (access(buffer,R_OK) != 0)) || (parser && parser->error)){
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
    return (handler(station_id, parser, &(app->wsd)));
}
/*******************************************************************************/
weather_com_parser *weather_parser_new_from_file(const gchar *filename,
							const gchar *encoding){
    weather_com_parser	*parser = NULL;
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
    }
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
			itm = create_item("title", (char*)hash_table_find(temp_xml_string, FALSE));
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
				    itm = create_item("pressure_direction",
					    (char*)hash_table_find(temp_xml_string, FALSE));
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
				    itm = create_item("wind_direction",
					    (char*)hash_table_find(temp_xml_string, FALSE));
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
                                        wsd->current_data_is_invalid = TRUE;
                                    else
                                        wsd->current_data_is_invalid = FALSE;
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
    return count_hour;
}
/*******************************************************************************/
gint parse_rp5_ru_xml(const gchar *station_id, weather_com_parser *parser,
							WeatherStationData *wsd){
    xmlNode	*current_node = NULL,
		*child_node = NULL;
    xmlChar	*temp_xml_string = NULL;
    Item        *itm;
    gboolean	store2day = TRUE;
    GSList	*day = NULL;
    guint	day_count = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    current_node = parser->weather_com_root->children;
/* check for error */
    if(current_node->type == XML_ELEMENT_NODE ){
	/* Check error */
	if(!xmlStrcmp(current_node->name, (const xmlChar *) "err" )){
	    xmlFreeDoc(parser->doc);
	    xmlCleanupParser();
	    free(parser);
	    return -2;
	}
    }
    current_node = current_node->next->children;
    while(current_node){
	if(current_node->type == XML_ELEMENT_NODE){
	    /* point_name */
	    if(!xmlStrcmp(current_node->name, (const xmlChar *)"point_name")){
		temp_xml_string = xmlNodeGetContent(current_node);
		itm = create_item("station_name", (gchar*)temp_xml_string);
		xmlFree(temp_xml_string);
		add_item2object(&(wsd->location), itm);
	    }
	    /* gmt_add */
	    if(!xmlStrcmp(current_node->name, (const xmlChar *)"gmt_add")){
		temp_xml_string = xmlNodeGetContent(current_node);
		itm = create_item("station_time_zone", (gchar*)temp_xml_string);
		xmlFree(temp_xml_string);
		add_item2object(&(wsd->location), itm);
	    }
	    /* timestep */
	    if(!xmlStrcmp(current_node->name, (const xmlChar *)"timestep")){
		child_node = current_node->children;
		while(child_node){
		    if(child_node->type == XML_ELEMENT_NODE){
			/* datetime */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"datetime")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    itm = create_item("day_date_time", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* G */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"G")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(!strcmp(temp_xml_string, "3"))
				store2day = FALSE;
			    else
				store2day = TRUE;
			    xmlFree(temp_xml_string);
			}
			/* cloud_cover */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"cloud_cover")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_cloud_cover", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_cloud_cover", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* precipitation */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"precipitation")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_precipitation", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_precipitation", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* pressure */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"pressure")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_pressure", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_pressure", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* temperature */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"temperature")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_temperature", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_temperature", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* humidity */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"humidity")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_humidity", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_humidity", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* wind_direction */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"wind_direction")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_wind_direction", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_wind_direction", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* wind_velocity */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"wind_velocity")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_wind_speed", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_wind_speed", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* falls */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"falls")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_falls", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_falls", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
			/* drops */
			if(!xmlStrcmp(child_node->name, (const xmlChar *)"drops")){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    if(store2day)
				itm = create_item("day_drops", (gchar*)temp_xml_string);
			    else
				itm = create_item("night_drops", (gchar*)temp_xml_string);
			    xmlFree(temp_xml_string);
			    add_item2object(&day, itm);
			}
		    }/* if */
		    child_node = child_node->next;
		}/* while */
		if(day){
		    add_item2object(&(wsd->days), day);
		    day = NULL;
		    day_count++;
		}
	    }
	}
	current_node = current_node->next;
    }/* while */
    xmlFreeDoc(parser->doc);
    xmlCleanupParser();
    free(parser);
    return day_count;
}
/*******************************************************************************/
gint parse_underground_com_data(const gchar *station){
    htmlDocPtr	doc;
    gchar	buffer[512];
    htmlNodePtr	current_node = NULL,
		root_node = NULL;
    gint	day_count = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!station)
	return -1;

    snprintf(buffer, sizeof(buffer) - 1, "%s/%s.htm", app->config->cache_dir_name, station);

    if(access(buffer, R_OK)){  /* htm file does not exist or no permissions */
	strcat(buffer, "l");
	if(access(buffer, R_OK))
	    return -1;			/* no one of htm or html was found */
    }
    doc = htmlReadFile(buffer, NULL, HTML_PARSE_NOERROR);
    
    root_node = xmlDocGetRootElement(doc);
    /* error in html document */
    if(!root_node)
	return -1;
    /* search node named 'body' */
    current_node = root_node->children;
    while(current_node && strcmp(current_node->name, "body"))
	current_node = current_node->next;
    /* search node named 'table' */
    current_node = current_node->children;
    while(current_node && strcmp(current_node->name, "table"))
	current_node = current_node->next;
    /* process tags <tr> */
    /* first <tr> contains current weather */
    process_undeground_com_current_weather(current_node->children->children);
    /* second <tr> must be skipped */
    current_node = current_node->next;
    /* third <tr> contains forecast weather */
    process_undeground_com_forecast_weather(current_node->next);

    xmlFreeDoc(doc);
    return day_count;
}
/*******************************************************************************/
void process_undeground_com_current_weather(const htmlNodePtr node){
    htmlNodePtr	current_node = node,
		child_node = NULL,
		child_node1 = NULL,
		child_node2 = NULL,
		child_node3 = NULL;
    enum { Nothing = -1, Temperature = 0, Windchill, Humidity, DewPoint, Wind,
	    Pressure, Conditions, Visibility, UV, Clouds, YesterdayMaximum,
	    YesterdayMinimum, YesterdayHeating, Sunrise, Sunset, MoonRise,
	    MoonSet, MoonPhase, RawMETAR };
    gint	what_process = Nothing;
#ifndef RELEASE
    START_FUNCTION;
#endif    
    /* search tag named 'center' */
    current_node = current_node->children;
    while(current_node && strcmp(current_node->name, "center"))
	current_node = current_node->next;
    /* search tag named 'table' */
    current_node = current_node->children;
    while(current_node && strcmp(current_node->name, "table"))
	current_node = current_node->next;

    for(child_node = current_node->children; child_node; child_node = child_node->next){
	fprintf(stderr, "\nChild name - %s\n", child_node->name);
	for(child_node1 = child_node->children; child_node1; child_node1 = child_node1->next){
	    fprintf(stderr, "\n\tChild1 name - %s\n", child_node1->name);
	    for(child_node2 = child_node1->children; child_node2; child_node2 = child_node2->next){
		fprintf(stderr, "\n\t\tChild2 name - %s\n\t\t\tContent - %s\n", child_node2->name, child_node2->content);
	    	for(child_node3 = child_node2->children; child_node3; child_node3 = child_node3->next)
		    fprintf(stderr, "\n\t\t\tChild3 name - %s\n\t\t\t\tContent - %s\n", child_node3->name, child_node3->content);
		/* find update field */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Updated:")){
		    if(!strcmp(child_node2->next->name, "b") && !strcmp(child_node2->next->children->name, "text")){
			fprintf(stderr, "\n>>>>>>>>>>Last update at: %s\n", child_node2->next->children->content);
		    }
		}
		/* select what process */
		/* temperature */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Temperature"))
		    what_process = Temperature;
		/* windchill */
		if(!strcmp(child_node2->name, "text") && !strcmp((char*)child_node2->content, "Windchill"))
		    what_process = Windchill;
		/* humidity */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Humidity"))
		    what_process = Humidity;
		/* DewPoint */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Dew Point"))
		    what_process = DewPoint;
		/* wind */
		if(!strcmp(child_node2->name, "text") && !strcmp((char*)child_node2->content, "Wind"))
		    what_process = Wind;
		/* pressure  */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Pressure"))
		    what_process = Pressure;
		/* conditions */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Conditions"))
		    what_process = Conditions;
		/* visibility  */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Visibility"))
		    what_process = Visibility;
		/* UV */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "UV"))
		    what_process = UV;
		/* Clouds */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Clouds"))
		    what_process = Clouds;
		/* Yesterday's Maximum */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Yesterday's Maximum"))
		    what_process = YesterdayMaximum;
		/* Yesterday's Minimum */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Yesterday's Minimum"))
		    what_process = YesterdayMinimum;
		/* Yesterday's Heating */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Yesterday's Heating"))
		    what_process = YesterdayHeating;
		/* sunrise */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Sunrise"))
		    what_process = Sunrise;
		/* sunset */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Sunset"))
		    what_process = Sunset;
		/* Moon Rise */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Moon Rise"))
		    what_process = MoonRise;
		/* Moon Set */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Moon Set"))
		    what_process = MoonSet;
		/* Moon Phase */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Moon Phase"))
		    what_process = MoonPhase;
		/* Raw METAR */
		if(!strcmp(child_node2->name, "text") && strstr((char*)child_node2->content, "Raw METAR"))
		    what_process = RawMETAR;
		/* process selected tag */
		if(what_process != Nothing &&
			((!strcmp(child_node2->name, "span") && !strcmp(child_node2->children->name, "b")) ||
			(!strcmp(child_node2->name, "b") && !strcmp(child_node2->children->name, "text")) ) ){
		    switch(what_process){
			default: what_process = Nothing;
			break;
			case Temperature:
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Temperature: %s\n", child_node2->children->children->content);
			    what_process = Nothing;
			break;
			case Windchill:
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Windchill: %s\n", child_node2->children->children->content);
			    what_process = Nothing;
			break;
			case Humidity:
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Humidity: %s\n", child_node2->children->content);
			    what_process = Nothing;
			break;
			case Wind:
			    if(!child_node2->children->children)
				fprintf(stderr, "\n>>>>>>>>>>>>>>Wind direction: %s\n", child_node2->children->content);
			    else{
				fprintf(stderr, "\n>>>>>>>>>>>>>>Wind speed: %s\n", child_node2->children->children->content);
				what_process = Nothing;
			    }
			break;
			case Pressure:
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Pressure: %s\n", child_node2->children->children->content);
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Pressure direction: %s\n", child_node2->next->next->children->content);
			    what_process = Nothing;
			break;
			case Conditions:
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Conditions: %s\n", child_node2->children->content);
			    what_process = Nothing;
			break;
			case Visibility:
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Visibility: %s\n", child_node2->children->content);
			    what_process = Nothing;
			break;
			case Sunrise:
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Sunrise: %s\n", child_node2->children->children->content);
			    what_process = Nothing;
			break;
			case Sunset:
			    fprintf(stderr, "\n>>>>>>>>>>>>>>Sunset: %s\n", child_node2->children->children->content);
			    what_process = Nothing;
			break;
		    }
		}
	    }
	}
    }
}
/*******************************************************************************/
void process_undeground_com_forecast_weather(const htmlNodePtr node){
    htmlNodePtr	current_node = node,
		child_node = NULL;
#ifndef RELEASE
    START_FUNCTION;
#endif    
    fprintf(stderr, "\nName - %s\n", current_node->name);
    /* process tags <td> */
    for(child_node = current_node->children; child_node; child_node = child_node->next)
	    fprintf(stderr, "\nChild name - %s\n", child_node->name);
}
/*******************************************************************************/
