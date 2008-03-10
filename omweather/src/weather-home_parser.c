/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) Superkaramba's Liquid Weather ++ team
 *	for ALL the artwork (icons)
 *        The maintainer (also the main author I believe)
 *        is Matthew <dmbkiwi@yahoo.com>
 *  http://liquidweather.net/icons.html#iconsets
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
#include "weather-home_parser.h"
#include "weather-home_hash.h"
#include "weather-data.h"
/*******************************************************************************/
weather_com_parser *weather_parser_new_from_file(const gchar *filename){
    weather_com_parser *parser;

    parser = (weather_com_parser *) malloc(sizeof(weather_com_parser));
    if(!parser)
	return NULL;
    parser->doc = NULL;
    parser->doc = xmlReadFile( filename, NULL, 0 );
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
int new_parse_weather_com_xml(void){
    weather_com_parser	*parser;
    xmlNode	*cur_node = NULL,
		*child_node = NULL,
		*child_node2 = NULL,
		*child_node3 = NULL,
		*child_node4 = NULL; 
    xmlChar	*temp_xml_string;
    xmlChar	*part_of_day = NULL;
    int		day = 0,
		i = 0,
		count_day = 0,
		year;
    gchar	buffer[2048],
    		id_station[10],
		newname[2048],
		buff[256];
    time_t	current_time;
    struct tm	*tm, tmp_tm;
    Item	*itm;
    
/*Prepare date string */
/* Imortant need will Check New Year in future !!!!!!!!!!! */
    current_time = time(NULL);
    tm = localtime(&current_time);
    year = 1900 + tm->tm_year;
    parser = NULL;
    
    if(!app->config->current_station_id)
	return -1;
    sprintf(buffer, "%s/%s.xml.new", app->config->cache_dir_name,
				    app->config->current_station_id); /* Used new file */
    if(!access(buffer, R_OK)){  /* Not Access to cache weather xml file */
	parser = weather_parser_new_from_file(buffer); 
	    if(!(parser->error)){
		sprintf(newname, "%s/%s.xml", app->config->cache_dir_name,
			    app->config->current_station_id);
		rename(buffer, newname);
	    }
    }
    if(((parser == NULL) && (access(buffer,R_OK) != 0)) || (parser != NULL && parser->error)){ /* Used old xml file */
    	if(parser){
	    free(parser);
	    parser = NULL;
	}    
	sprintf(buffer, "%s/%s.xml", app->config->cache_dir_name,
		app->config->current_station_id);
	/* Not Access to cache weather xml file or not valid XML file */
	if(!access(buffer,R_OK)){ 
	    parser = weather_parser_new_from_file(buffer);
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

    for(cur_node = parser->weather_com_root->children; cur_node != NULL; cur_node = cur_node->next){
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
		if( strcmp(id_station, app->config->current_station_id) ){
		    free(parser);
		    return -1;
		}
		for(child_node = cur_node->children; child_node; child_node = child_node->next){
		    if( child_node->type == XML_ELEMENT_NODE ){
			if( !xmlStrcmp(child_node->name, (const xmlChar *)"dnam") ){
			    for (i = 0; i < Max_count_weather_day; i++){
				temp_xml_string = xmlNodeGetContent(child_node);
				itm = create_item("station_name", (char*)temp_xml_string);
				xmlFree(temp_xml_string);
				add_item2object(&(wcs.day_data[i]), itm);
			    }
			    continue;
			}
			if( !xmlStrcmp(child_node->name, (const xmlChar *)"lat") ){
                	    for(i = 0; i < Max_count_weather_day; i++){
                    		temp_xml_string = xmlNodeGetContent(child_node);
				itm = create_item("station_latitude", (char*)temp_xml_string);
                    		xmlFree(temp_xml_string);
				add_item2object(&(wcs.day_data[i]), itm);
			    }
        		}
			if( !xmlStrcmp(child_node->name, (const xmlChar *)"lon") ){
                	    for(i = 0; i < Max_count_weather_day; i++){
                    		temp_xml_string = xmlNodeGetContent(child_node);
				itm = create_item("station_longitude", (char*)temp_xml_string);
                    		xmlFree(temp_xml_string);
				add_item2object(&(wcs.day_data[i]), itm);
			    }
        		}
			if( !xmlStrcmp(child_node->name, (const xmlChar *)"zone") ){
                	    for(i = 0; i < Max_count_weather_day; i++){
                    		temp_xml_string = xmlNodeGetContent(child_node);
				itm = create_item("station_time_zone", (char*)temp_xml_string);
                    		xmlFree(temp_xml_string);
				add_item2object(&(wcs.day_data[i]), itm);
			    }
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
			add_item2object(&(wcs.current_data), itm);
			continue;
		    }
		    /* temperature */
		    if( child_node->type == XML_ELEMENT_NODE  &&
        		    ( !xmlStrcmp(child_node->name, (const xmlChar *)"tmp") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("temperature", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wcs.current_data), itm);
			continue;
		    }
		    /* feel like */
		    if( child_node->type == XML_ELEMENT_NODE  &&
            		    ( !xmlStrcmp(child_node->name, (const xmlChar *)"flik") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("feel_like", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wcs.current_data), itm);
			continue;
		    }
		    /* title */
		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"t") ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("title", (char*)hash_table_find(temp_xml_string));
			xmlFree(temp_xml_string);
			add_item2object(&(wcs.current_data), itm);
			continue;
		    }
		    /* icon */
    		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"icon") ){
			temp_xml_string = xmlNodeGetContent(child_node);					    
    			itm = create_item("icon", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wcs.current_data), itm);
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
				    add_item2object(&(wcs.current_data), itm);
				}
				/* direction */
    				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"d") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("pressure_direction",
					    (char*)hash_table_find(temp_xml_string));
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.current_data), itm);		    
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
			add_item2object(&(wcs.current_data), itm);
			continue;
		    }
		    /* visible */
    		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"vis") ){
			temp_xml_string = xmlNodeGetContent(child_node);
			itm = create_item("visible", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wcs.current_data), itm);
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
				    add_item2object(&(wcs.current_data), itm);
				}
				/* gust */
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"gust") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("wind_gust", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.current_data), itm);
				}
				/* direction */
    				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("wind_direction",
					    (char*)hash_table_find(temp_xml_string));
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.current_data), itm);
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
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"icon") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("moon_icon", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.current_data), itm);
				}
				/* phase */
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("moon_phase", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.current_data), itm);
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
			memcpy(buff, temp_xml_string, sizeof(buff) - 1);
			strptime(buff, "%A", &tmp_tm);
			memset(buff, 0, sizeof(buff));
			strftime(buff, sizeof(buff) - 1, "%a", &tmp_tm);
			itm = create_item("24h_name", buff);
			xmlFree(temp_xml_string);
			add_item2object(&(wcs.day_data[count_day]), itm);
			/* get 24h date */
			temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"dt");
			itm = create_item("24h_date", (char*)temp_xml_string);
			xmlFree(temp_xml_string);
			add_item2object(&(wcs.day_data[count_day]), itm);
			
			for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
			    if( child_node2->type == XML_ELEMENT_NODE){
				/* 24h hi temperature */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"hi")){
				    temp_xml_string = xmlNodeGetContent(child_node2);
    				    itm = create_item("24h_hi_temperature", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.day_data[count_day]), itm);
				    continue;
				}
				/* 24h low temperature */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"low")){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("24h_low_temperature", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.day_data[count_day]), itm);
				    continue;
				}
				/* 24h sunrise */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"sunr")){
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    itm = create_item("24h_sunrise", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.day_data[count_day]), itm);
				    continue;
				}
				/* 24h sunset */
				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"suns")){
				    temp_xml_string = xmlNodeGetContent(child_node2);
    			    	    itm = create_item("24h_sunset", (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				    add_item2object(&(wcs.day_data[count_day]), itm);
				    continue;
				}
				/* 24h part */
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"part")){
				    part_of_day = xmlGetProp(child_node2, (const xmlChar*)"p");
				    if( !xmlStrcmp(part_of_day, (const xmlChar *)"d") )
					day = 1;
				    else
					day = 0;
				    xmlFree(part_of_day);
				    for(child_node3 = child_node2->children; child_node3; child_node3 = child_node3->next){
					if( child_node3->type == XML_ELEMENT_NODE){
					    /* humidity */
            				    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"hmid") ){
						temp_xml_string = xmlNodeGetContent(child_node3);
						if(!day)
						    itm = create_item("night_humidity", (char*)temp_xml_string);
						else
						    itm = create_item("day_humidity", (char*)temp_xml_string);
						xmlFree(temp_xml_string);
						add_item2object(&(wcs.day_data[count_day]), itm);
						continue;
					    }
					    /* title */
					    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"t") ){
						temp_xml_string = xmlNodeGetContent(child_node3);
						if(!day)
						    itm = create_item("night_title",
							    (char*)hash_table_find(temp_xml_string));
						else
						    itm = create_item("day_title",
                                    			    (char*)hash_table_find(temp_xml_string));													    
						xmlFree(temp_xml_string);
						add_item2object(&(wcs.day_data[count_day]), itm);
						continue;
					    }
					    /* icon */
					    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"icon") ){
						temp_xml_string = xmlNodeGetContent(child_node3);
						if(!day)
						    itm = create_item("night_icon", (char*)temp_xml_string);
						else
						    itm = create_item("day_icon", (char*)temp_xml_string);
						xmlFree(temp_xml_string);
						add_item2object(&(wcs.day_data[count_day]), itm);
						continue;
					    }
					    /* wind data */
					    if(!xmlStrcmp(child_node3->name, (const xmlChar *)"wind") ){
						for(child_node4 = child_node3->children; child_node4; child_node4 = child_node4->next){
	    					    if( child_node4->type == XML_ELEMENT_NODE){
							/* speed */
							if(!xmlStrcmp(child_node4->name, (const xmlChar *)"s") ){
							    temp_xml_string = xmlNodeGetContent(child_node4);
							    if(!day)
								itm = create_item("night_wind_speed", (char*)temp_xml_string);
							    else
							    	itm = create_item("day_wind_speed", (char*)temp_xml_string);
							    xmlFree(temp_xml_string);
							    add_item2object(&(wcs.day_data[count_day]), itm);
							    continue;
		    					}
							/* title */
							if(!xmlStrcmp(child_node4->name, (const xmlChar *)"t") ){
							    temp_xml_string = xmlNodeGetContent(child_node4);
							    if(!day)
								itm = create_item("night_wind_title",
									    (char*)hash_table_find(temp_xml_string));
							    else
    								itm = create_item("day_wind_title",
									    (char*)hash_table_find(temp_xml_string));
							    xmlFree(temp_xml_string);
							    add_item2object(&(wcs.day_data[count_day]), itm);
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
		    count_day++;
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
