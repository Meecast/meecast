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
int parse_weather_com_xml(void){
    weather_com_parser	*parser;
    xmlNode	*cur_node	= NULL,
		*child_node	= NULL,
		*child_node2 = NULL,
		*child_node3 = NULL,
		*child_node4 = NULL; 
    xmlChar	*temp_xml_string;
    xmlChar	*part_of_day = NULL;
    int		day = 0,
		count_day = 0,
		i,
		current_month = 1,
		year;
    char	*icon,
		*temp_string_pointer,
		id_station[10],
		date_in_string[255];
    gchar	buffer[2048],
		newname[2048];
    time_t	current_time;
    struct tm	*tm;
/*  fprintf (stderr, "vlad: %s",get_weather_html());
    http://xoap.weather.com/weather/local/BOXX0014?cc=*&prod=xoap&par=1004517364&key=a29796f587f206b2&unit=m&dayf=5
    http://www.weather.com/weather/mpdwcr/tenday?locid=BOXX0014&channel=other&datapoint=htempdp&adprodname=pif_undcl_tenday_long
    http://yolinux.com/TUTORIALS/GnomeLibXml2.html
    $par = "1005217190";
    $key = "2e4490982af206e0";
    http://www.rap.ucar.edu/weather/surface/stations.txt
*/

/*Prepare date string */
/* Imortant need will Check New Year in future !!!!!!!!!!! */
    current_time = time(NULL);
    tm = localtime(&current_time);
    year = 1900 + tm->tm_year;
    parser = NULL;
    
    if(!app->current_station_id)
	return -1;
    sprintf(buffer, "%s/%s.xml.new", app->weather_dir_name,
				    app->current_station_id); /* Used new file */
    if(!access(buffer,R_OK)){  /* Not Access to cache weather xml file */
	parser = weather_parser_new_from_file(buffer); 
	    if(!(parser->error)){
		sprintf(newname, "%s/%s.xml", app->weather_dir_name,
			    app->current_station_id);
		rename(buffer,newname);
	    }
    }
    if(((parser == NULL) &&(access (buffer,R_OK) != 0)) || (parser != NULL && parser->error)){ /* Used old xml file */
    	if (parser) free(parser);
	sprintf(buffer, "%s/%s.xml", app->weather_dir_name,
		app->current_station_id);
	/* Not Access to cache weather xml file or not valid XML file */
	if(!access(buffer,R_OK)){ 
	    parser = weather_parser_new_from_file(buffer);
	    if (parser->error){
		free(parser);
		return -1; 
	    }	
	}
	else{
	    free(parser);
	    return -1;
	}    
    }

    for(cur_node = parser->weather_com_root->children; cur_node != NULL; cur_node = cur_node->next){
	if( cur_node->type == XML_ELEMENT_NODE ){
        /* Check error */
	    if(!xmlStrcmp(cur_node->name, (const xmlChar *) "err" ) ){
		free(parser);
		return -2;
	    }	
        /* Fill all buttons Location data */
    	    if(!xmlStrcmp(cur_node->name, (const xmlChar *) "loc" ) ){
		temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"id");
		sprintf(id_station, "%.8s", temp_xml_string);
		xmlFree(temp_xml_string);
	 /* If station in xml not station in config file exit */ 
		if( strcmp(id_station, app->current_station_id) != 0 ){
		    free(parser);
		    return -1;
		}    
/*	 fprintf(stderr,"Element: %s \n", cur_node->name); */
		for(child_node = cur_node->children; child_node != NULL; child_node = child_node->next){
		    if( child_node->type == XML_ELEMENT_NODE  && 
			    ( !xmlStrcmp(child_node->name, (const xmlChar *)"dnam") ) ){
			for (i = 0; i < Max_count_weather_day; i++){
			    temp_xml_string = xmlNodeGetContent(child_node);
			    snprintf(weather_days[i].location, sizeof(weather_days[i].location) - 1, "%s",temp_xml_string);
			    xmlFree(temp_xml_string);
			}    
		    }
		    if( child_node->type == XML_ELEMENT_NODE  &&
                	    ( !xmlStrcmp(child_node->name, (const xmlChar *)"zone") ) ){
                	for(i = 0; i < Max_count_weather_day; i++){
                    	    temp_xml_string = xmlNodeGetContent(child_node);
                	    weather_days[i].zone = atol((char*)temp_xml_string) * 60 * 60;
                    	    xmlFree(temp_xml_string);
			}
        	    }
		}
	    }
	/* Fill current day */
	    if(!xmlStrcmp(cur_node->name, (const xmlChar *) "cc" ) ){
		for(child_node = cur_node->children; child_node != NULL; child_node = child_node->next){
		    if( child_node->type == XML_ELEMENT_NODE  &&
            		    ( !xmlStrcmp(child_node->name, (const xmlChar *)"lsup") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);    
			sprintf(date_in_string,"%s",temp_xml_string);
			xmlFree(temp_xml_string);
			strptime(date_in_string, "%D %I:%M", tm);
			current_month = tm->tm_mon;
			weather_current_day.date_time = mktime(tm);
			temp_string_pointer = strchr(date_in_string,'M');
	    /* Add 12 hours if  date have PM field */
			if(*(temp_string_pointer - 1) == 'P')
			    weather_current_day.date_time = weather_current_day.date_time + 12 * 3600;
		    }	
		    if( child_node->type == XML_ELEMENT_NODE  &&
        		    ( !xmlStrcmp(child_node->name, (const xmlChar *)"tmp") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);			
			sprintf(weather_current_day.day.temp,"%.3s", temp_xml_string);
			xmlFree(temp_xml_string);
		    }
    		    if( child_node->type == XML_ELEMENT_NODE  &&
            		    ( !xmlStrcmp(child_node->name, (const xmlChar *)"flik") ) ){
			temp_xml_string = xmlNodeGetContent(child_node);			    
    			sprintf(weather_current_day.low_temp,"%.3s", temp_xml_string);
			xmlFree(temp_xml_string);
		    }	
		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"t") ){
			temp_xml_string = xmlNodeGetContent(child_node);			
    			snprintf(weather_current_day.day.title,
				 sizeof(weather_current_day.day.title) - 1,
				 "%s",
				 (char*)hash_table_find(temp_xml_string));
			xmlFree(temp_xml_string);	 
		    }
    		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"icon") ){
			temp_xml_string = xmlNodeGetContent(child_node);					    
    			icon = (char*) temp_xml_string;
    			weather_current_day.day.icon=atoi(icon);
			xmlFree(temp_xml_string);	 
        	    }
    		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"bar") ){
			for(child_node2 = child_node->children; child_node2 != NULL; child_node2 = child_node2->next){
    			    if( child_node2->type == XML_ELEMENT_NODE ){
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"r") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);			
        			    weather_current_day.day.pressure = atof((char*)temp_xml_string);
				    xmlFree(temp_xml_string);	 				    
				}    
    				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"d") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);			
        			    snprintf(weather_current_day.day.pressure_str,
					     sizeof(weather_current_day.day.pressure_str) - 1,
					     "%s",
					     (char*)hash_table_find(temp_xml_string));
				    xmlFree(temp_xml_string);	 				    
				}
			    }
        		}
        	    }
		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"hmid") ){
    			temp_xml_string = xmlNodeGetContent(child_node);					    
    			sprintf(weather_current_day.day.hmid,"%.3s",temp_xml_string);
			xmlFree(temp_xml_string);	 				    
		    }	
    		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"vis") ){
			temp_xml_string = xmlNodeGetContent(child_node);					    
			snprintf(weather_current_day.day.vis,
				sizeof(weather_current_day.day.vis) - 1,
				"%s",
				(char*)temp_xml_string);
			xmlFree(temp_xml_string);
		    }	
		    if(!xmlStrcmp(child_node->name, (const xmlChar *)"wind") ){
			for(child_node2 = child_node->children; child_node2 != NULL; child_node2 = child_node2->next){
    			    if( child_node2->type == XML_ELEMENT_NODE ){
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"s") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);			
        			    snprintf(weather_current_day.day.wind_speed,
					    sizeof(weather_current_day.day.wind_speed) - 1,
					    "%s", 
        				    (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				}
        			if(!xmlStrcmp(child_node2->name, (const xmlChar *)"gust") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);							
				    snprintf(weather_current_day.day.wind_gust,
					    sizeof(weather_current_day.day.wind_gust) - 1,
					    "%s", 
        				    (char*)temp_xml_string);
				    xmlFree(temp_xml_string);
				}
    				if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t") ){
				    temp_xml_string = xmlNodeGetContent(child_node2);							
        			    snprintf(weather_current_day.day.wind_title,
					     sizeof(weather_current_day.day.wind_title) - 1,
					     "%s",
					     (char*)hash_table_find(temp_xml_string));
				    xmlFree(temp_xml_string);
				}
			    }
        		}
        	    }         
		}
	    }
	    if(!xmlStrcmp(cur_node->name, (const xmlChar *) "dayf" ) ){
    		for(child_node = cur_node->children; child_node != NULL; child_node = child_node->next){
    		    if( child_node->type == XML_ELEMENT_NODE  &&
			    ( !xmlStrcmp(child_node->name, (const xmlChar *)"day") ) ){
			if( count_day < Max_count_weather_day ){ /* Check limit day */
			    count_day++; 
			    temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"t");
			    memset(buffer, 0, sizeof(buffer));
			    memcpy(buffer, temp_xml_string, sizeof(buffer) - 2);
			    strptime(buffer, "%A", tm);
                    	    memset(weather_days[count_day-1].dayshname, 0,
				    sizeof(weather_days[count_day-1].dayshname));
			    strftime(weather_days[count_day-1].dayshname,
				    sizeof(weather_days[count_day-1].dayshname) - 1,
				    "%a", tm);
			    xmlFree(temp_xml_string);
			    temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"t");
			    memset(weather_days[count_day-1].dayfuname, 0,
				    sizeof(weather_days[count_day-1].dayfuname));
			    snprintf(weather_days[count_day-1].dayfuname,
				     sizeof(weather_days[count_day-1].dayfuname) - 1, "%s",
				     temp_xml_string);
			    xmlFree(temp_xml_string);				     

			    temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"dt");
			    snprintf(weather_days[count_day-1].date,
        			     sizeof(weather_days[count_day-1].date) - 1,
				     "%s",
				     (char*)temp_xml_string);
			    xmlFree(temp_xml_string);				     
			    sprintf(date_in_string,"%s %i 00:00:00",weather_days[count_day-1].date,year);
			    strptime(date_in_string, "%b %d %Y %T", tm);
	      /* Check New Year */
			    if((current_month == 11) && (tm->tm_mon == 0)){
	    			sprintf(date_in_string,"%s %i 00:00:00",weather_days[count_day-1].date,year+1);
	    			strptime(date_in_string, "%b %d %Y %T", tm);
			    } 
			    weather_days[count_day-1].date_time = mktime(tm);
			    for(child_node2 = child_node->children; child_node2 != NULL; child_node2 = child_node2->next){
				if( child_node2->type == XML_ELEMENT_NODE){  
        			    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"part")){
					part_of_day = xmlGetProp(child_node2, (const xmlChar*)"p");
					if( !xmlStrcmp(part_of_day, (const xmlChar *)"d") )
					    day = 1;
					else
					    day = 0;
					xmlFree(part_of_day);
					for(child_node3 = child_node2->children; child_node3 != NULL; child_node3 = child_node3->next){
					    if( child_node3->type == XML_ELEMENT_NODE){  
            					if(!xmlStrcmp(child_node3->name, (const xmlChar *)"hmid") ){
						    if(!day){
						        temp_xml_string = xmlNodeGetContent(child_node3);
							sprintf(weather_days[count_day-1].night.hmid,"%.3s",temp_xml_string);
							xmlFree(temp_xml_string);
						    }	
						    else{
						        temp_xml_string = xmlNodeGetContent(child_node3);							
							sprintf(weather_days[count_day-1].day.hmid,"%.3s",temp_xml_string);
							xmlFree(temp_xml_string);
						    }
						    
						}
						if(!xmlStrcmp(child_node3->name, (const xmlChar *)"t") ){
						    if(!day){
							temp_xml_string = xmlNodeGetContent(child_node3);							
							snprintf(weather_days[count_day-1].night.title,
								 sizeof(weather_days[count_day-1].night.title) - 1,
								 "%s",
								 (char*)hash_table_find(temp_xml_string));
							xmlFree(temp_xml_string);
						    }								 
						    else{
						    	temp_xml_string = xmlNodeGetContent(child_node3);													    
							snprintf(weather_days[count_day-1].day.title,
								 sizeof(weather_days[count_day-1].day.title) - 1,
								"%s",
								 (char*)hash_table_find(temp_xml_string));
							xmlFree(temp_xml_string);								 
						    }
						}
						if(!xmlStrcmp(child_node3->name, (const xmlChar *)"icon") ){
						    temp_xml_string = xmlNodeGetContent(child_node3);	
						    icon = (char*)temp_xml_string;	
						    if(!day)
							weather_days[count_day-1].night.icon=atoi(icon);
						    else
		    					weather_days[count_day-1].day.icon=atoi(icon);          
						    xmlFree(temp_xml_string);								 
						}
						if(!xmlStrcmp(child_node3->name, (const xmlChar *)"wind") ){
						    for(child_node4 = child_node3->children; child_node4 != NULL; child_node4 = child_node4->next){
	    						if( child_node4->type == XML_ELEMENT_NODE){
							    if(!xmlStrcmp(child_node4->name, (const xmlChar *)"s") ){
								if(!day){
								    temp_xml_string = xmlNodeGetContent(child_node4);	
								    snprintf(weather_days[count_day-1].night.wind_speed,
									    sizeof(weather_days[count_day-1].night.wind_speed) - 1,
									    "%s",
									    (char*)temp_xml_string);
								    xmlFree(temp_xml_string);	
								}
								else{
								    temp_xml_string = xmlNodeGetContent(child_node4);	
								    snprintf(weather_days[count_day-1].day.wind_speed,
									    sizeof(weather_days[count_day-1].day.wind_speed) - 1,
									    (char*)temp_xml_string);
								    xmlFree(temp_xml_string);
								}
		    					    }
							    if(!xmlStrcmp(child_node4->name, (const xmlChar *)"t") ){
								temp_xml_string = xmlNodeGetContent(child_node4);
								if(!day)
								    snprintf(weather_days[count_day-1].night.wind_title,
									     sizeof(weather_days[count_day-1].night.wind_title) - 1,
									     "%s",
									     (char*)hash_table_find(temp_xml_string));
								else
								    sprintf(weather_days[count_day-1].day.wind_title,"%.10s",
									    (char*)hash_table_find(temp_xml_string));
    								xmlFree(temp_xml_string);
    								
							    }
    							}
						    } 
						}
					    }
					} 
	    			    }   	
				    temp_xml_string = xmlNodeGetContent(child_node2);
				    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"hi"))
					sprintf(weather_days[count_day-1].hi_temp,"%.3s", temp_xml_string);
				    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"low"))
					sprintf(weather_days[count_day-1].low_temp,"%.3s", temp_xml_string);
				    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"sunr")){ 
					sprintf(date_in_string,"%s %i %s",weather_days[count_day-1].date,year,temp_xml_string);
					strptime(date_in_string, "%b %d %Y %I:%M %p", tm);
					weather_days[count_day-1].day.begin_time = mktime(tm);
				    
		 /* Check New Year */
					if((current_month == 11) && (tm->tm_mon == 0)){
					    sprintf(date_in_string,"%s %i %s",weather_days[count_day-1].date,year+1,temp_xml_string);
					    strptime(date_in_string, "%b %d %Y %I:%M %p", tm);
	    				    weather_days[count_day-1].day.begin_time = mktime(tm);	        
					}
				    }     
				    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"suns")){ 
					sprintf(date_in_string,"%s %i %s",weather_days[count_day-1].date,year,temp_xml_string);
					strptime(date_in_string, "%b %d %Y %I:%M %p", tm);
					weather_days[count_day-1].night.begin_time = mktime(tm);
                  /* Check New Year */
	    				if((current_month == 11) && (tm->tm_mon == 0)){
					    sprintf(date_in_string,"%s %i %s",weather_days[count_day-1].date,year+1,temp_xml_string);
					    strptime(date_in_string, "%b %d %Y %I:%M %p", tm);
					    weather_days[count_day-1].night.begin_time = mktime(tm);
					}
				    }
				    xmlFree(temp_xml_string);    
				}
			    } 
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
