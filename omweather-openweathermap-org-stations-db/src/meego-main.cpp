/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-openweathermap-org-source - MeeCast
 *
 * Copyright (C) 2013-2018 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "meego-main.h"

static xmlHashTablePtr hash_for_icons;
#define buff_size 2048

int station_timezone = 0;
/*******************************************************************************/
int
parse_and_write_hours_data(htmlDocPtr doc, const char *result_file){

    FILE        *file_out;
    xmlNode     *cur_node = NULL,
                *cur_node0 = NULL,
                *child_node = NULL;
    xmlNode     *root_node = NULL;
    char        buffer[buff_size];
    char        temp_buffer[buff_size];
    time_t      utc_time_start;
    time_t      utc_time_end;
    int         count_day = 0;
    int         temp_hi, temp_low;
    char        short_text[1024];
    char        icon[256],
                precipitation[128],
                wind_direction[10],
                wind_speed[10],
                pressure[10],
                humidity[10];

    time_t      current_time;
    int         localtimezone = 0;
    struct      tm time_tm1;
    struct      tm time_tm2;
    xmlChar     *day_prop;
    xmlChar     *temp_prop;

    if(!doc)
        return -1;

    file_out = fopen(result_file, "aw");
    if (!file_out)
        return -1;

    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 

    root_node = xmlDocGetRootElement(doc);

    for(cur_node0 = root_node->children; cur_node0; cur_node0 = cur_node0->next){
        if( cur_node0->type == XML_ELEMENT_NODE ){
            if (!xmlStrcmp(cur_node0->name, (const xmlChar *) "forecast" ) ){
               for(cur_node = cur_node0->children; cur_node; cur_node = cur_node->next){
                    if( cur_node->type == XML_ELEMENT_NODE ){
                        /* get weather data */
                        if(!xmlStrcmp(cur_node->name, (const xmlChar *) "time")){
                            if((day_prop = xmlGetProp(cur_node, (const xmlChar*)"from")) != NULL) {
                                struct tm tmp_tm = {0};
                                memset(temp_buffer, 0, sizeof(buffer));
                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s", day_prop);
                                xmlFree(day_prop);
                                strptime(temp_buffer, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
 
                                utc_time_start = mktime(&tmp_tm) + localtimezone*3600;
                                utc_time_end = mktime(&tmp_tm) + 3*3600 + localtimezone*3600;
 
                                /* clear variables */
                                temp_hi = INT_MAX; temp_low = INT_MAX; 
                                memset(short_text, 0, sizeof(short_text));
                                memset(icon, 0, sizeof(icon));
                                memset(wind_direction, 0, sizeof(wind_direction));
                                memset(wind_speed, 0, sizeof(wind_speed));
                                memset(precipitation, 0, sizeof(precipitation));
                                memset(pressure, 0, sizeof(pressure));
                                memset(humidity, 0, sizeof(humidity));

                                for (child_node = cur_node->children; child_node; child_node = child_node->next){
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "temperature")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                                                temp_low = atoi((char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                       }
                                    }

                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "precipitation")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                                                snprintf(precipitation, sizeof(precipitation)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }

                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "windDirection")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"code")) != NULL){
                                                snprintf(wind_direction, sizeof(wind_direction)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }

                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "windSpeed")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"mps")) != NULL){
                                                snprintf(wind_speed, sizeof(wind_speed)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }

                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "pressure")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                                                snprintf(pressure, sizeof(pressure)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "humidity")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL ){
                                                snprintf(humidity, sizeof(humidity)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }
                                    /* symbol number="801" name="few clouds" var="02d" */
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "symbol")){
                                            memset(temp_buffer, 0, sizeof(buffer));
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"number")) != NULL){
                                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s", (char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"var")) != NULL){
                                                strcat(temp_buffer, (char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                            if (xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer))
                                                snprintf(icon, sizeof(icon)-1, "%s",
                                                      (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"name")) != NULL){
                                                snprintf(short_text, sizeof(short_text)-1, "%s", (char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                                short_text[0] = toupper(short_text[0]);
                                            }
                                        }
                                    }
                                }
                                fprintf(file_out,"    <period start=\"%li\"", utc_time_start);
                                fprintf(file_out," end=\"%li\" hour=\"true\">\n", utc_time_end); 

                                if (strlen (short_text)>0)
                                   fprintf(file_out, "     <description>%s</description>\n", short_text);
                                if (strlen(icon)>0)
                                    fprintf(file_out,"     <icon>%s</icon>\n", icon);
                                if (temp_low != INT_MAX)
                                    fprintf(file_out,"     <temperature>%i</temperature>\n", temp_low);				                
                                if (strlen (precipitation)>0)
                                    fprintf(file_out,"     <precipitation>%s</precipitation>\n", precipitation);
                                if (strlen (wind_direction)>0)
                                    fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", wind_direction);
                                if (strlen (wind_speed)>0)
                                    fprintf(file_out,"     <wind_speed>%s</wind_speed>\n", wind_speed);
                                if (strlen (pressure)>0)
                                    fprintf(file_out,"     <pressure>%s</pressure>\n", pressure);
                                if (strlen (humidity)>0)
                                    fprintf(file_out,"     <humidity>%s</humidity>\n", humidity);

                                fprintf(file_out,"    </period>\n");
                                count_day++;
                            }
                        }
                    }
                }
            }          
        }
    }
    fclose(file_out);
    return count_day;
}

int
parse_and_write_days_xml_data(htmlDocPtr doc, const char *result_file){

    FILE        *file_out;
    xmlNode     *cur_node = NULL,
                *cur_node0 = NULL,
                *child_node = NULL;
    xmlNode     *root_node = NULL;
    char        buffer[buff_size];
    char        temp_buffer[buff_size];
    time_t      utc_time_start;
    time_t      utc_time_end;
    int         count_day = 0;
    int         temp_hi, temp_low;
    char        short_text[1024];
    char        icon[256],
                ppcp[128],
                wind_direction[10],
                wind_speed[10],
                pressure[10],
                humidity[10];

    time_t      current_time;
    int         localtimezone = 0;
    struct      tm time_tm1;
    struct      tm time_tm2;
    xmlChar     *day_prop;
    xmlChar     *temp_prop;

    if(!doc)
        return -1;

    file_out = fopen(result_file, "aw");
    if (!file_out)
        return -1;

    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 


    root_node = xmlDocGetRootElement(doc);

    for(cur_node0 = root_node->children; cur_node0; cur_node0 = cur_node0->next){
        if( cur_node0->type == XML_ELEMENT_NODE ){
            if (!xmlStrcmp(cur_node0->name, (const xmlChar *) "forecast" ) ){
               for(cur_node = cur_node0->children; cur_node; cur_node = cur_node->next){
                    if( cur_node->type == XML_ELEMENT_NODE ){
                        /* get weather data */
                        if(!xmlStrcmp(cur_node->name, (const xmlChar *) "time")){
                            if((day_prop = xmlGetProp(cur_node, (const xmlChar*)"day")) != NULL) {
                                struct tm tmp_tm = {0};
                                memset(temp_buffer, 0, sizeof(buffer));
                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s", day_prop);
                                xmlFree(day_prop);
                                strptime(temp_buffer, "%Y-%m-%d", &tmp_tm);
                                utc_time_start = mktime(&tmp_tm)  + localtimezone*3600 - station_timezone*3600;
                                utc_time_end = mktime(&tmp_tm) + 24*3600  + localtimezone*3600 - station_timezone*3600;
                                /* clear variables */
                                temp_hi = INT_MAX; temp_low = INT_MAX; 
                                memset(short_text, 0, sizeof(short_text));
                                memset(icon, 0, sizeof(icon));
                                memset(wind_direction, 0, sizeof(wind_direction));
                                memset(wind_speed, 0, sizeof(wind_speed));
                                memset(ppcp, 0, sizeof(ppcp));
                                memset(pressure, 0, sizeof(pressure));
                                memset(humidity, 0, sizeof(humidity));

                                for (child_node = cur_node->children; child_node; child_node = child_node->next){
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "temperature")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"min")) != NULL){
                                                temp_low = atoi((char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"max")) != NULL){
                                                temp_hi = atoi((char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }

                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "precipitation")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                                                snprintf(ppcp, sizeof(ppcp)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }

                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "windDirection")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"code")) != NULL){
                                                snprintf(wind_direction, sizeof(wind_direction)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }

                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "windSpeed")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"mps")) != NULL){
                                                snprintf(wind_speed, sizeof(wind_speed)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }

                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "pressure")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                                                snprintf(pressure, sizeof(pressure)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "humidity")){
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                                                snprintf(humidity, sizeof(humidity)-1,"%s",(char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                        }
                                    }
                                    /* symbol number="801" name="few clouds" var="02d" */
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "symbol")){
                                            memset(temp_buffer, 0, sizeof(buffer));
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"number")) != NULL){
                                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s", (char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"var")) != NULL){
                                                strcat(temp_buffer, (char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                            }
                                            if (xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer))
                                                snprintf(icon, sizeof(icon)-1, "%s",
                                                      (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));
                                            if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"name")) != NULL){
                                                snprintf(short_text, sizeof(short_text)-1, "%s", (char *)temp_prop);
                                                xmlFree(temp_prop);
                                                temp_prop = NULL;
                                                short_text[0] = toupper(short_text[0]);
                                            }
                                        }
                                    }
                                }
                                fprintf(file_out,"    <period start=\"%li\"", utc_time_start);
                                fprintf(file_out," end=\"%li\">\n", utc_time_end); 

                                if (strlen (short_text)>0)
                                   fprintf(file_out, "     <description>%s</description>\n", short_text);
                                if (strlen(icon)>0)
                                    fprintf(file_out,"     <icon>%s</icon>\n", icon);
                                if (temp_hi != INT_MAX)
                                    fprintf(file_out,"     <temperature_hi>%i</temperature_hi>\n", temp_hi);				                
                                if (temp_low != INT_MAX)
                                    fprintf(file_out,"     <temperature_low>%i</temperature_low>\n", temp_low);
                                if (strlen (ppcp)>0)
                                    fprintf(file_out,"     <ppcp>%s</ppcp>\n", ppcp);

                                if (strlen (wind_direction)>0)
                                    fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", wind_direction);
                                if (strlen (wind_speed)>0)
                                    fprintf(file_out,"     <wind_speed>%s</wind_speed>\n", wind_speed);
                                
                                if (strlen (pressure)>0)
                                    fprintf(file_out,"     <pressure>%s</pressure>\n", pressure);

                                if (strlen (humidity)>0)
                                    fprintf(file_out,"     <humidity>%s</humidity>\n", humidity);

                                fprintf(file_out,"    </period>\n");
                                count_day++;
                            }
                        }
                    }
                }
            }          
        }
    }
    fclose(file_out);
    return count_day;
}
/*******************************************************************************/
int
parse_and_write_timezone_data(htmlDocPtr doc, const char *result_file){

    FILE        *file_out;
    xmlNode     *child_node = NULL,
                *child_node2 = NULL;
    xmlNode     *root_node = NULL;
    char        buffer[buff_size];
    struct tm   tmp_tm = {0};
    time_t      utc_time_start;
    time_t      utc_time_end;
    time_t      utc_time_sunrise = -1;
    time_t      utc_time_sunset = -1;
    int         count_day = 0;
    int         temp;
    char        short_text[1024];
    char        icon[256],
                ppcp[128],
                wind_direction[10],
                wind_speed[10],
                pressure[10],
                humidity[10];

    time_t      current_time;
    struct      tm time_tm1 = {0};
    struct      tm time_tm2 = {0};
    xmlChar     *temp_prop;

    if(!doc)
        return -1;

    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;

    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"Unknown\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n");
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");

    root_node = xmlDocGetRootElement(doc);

  
    for(child_node = root_node->children; child_node; child_node = child_node->next){
       /* fprintf(stderr," first name %s\n", child_node->name); */
       if (child_node->type == XML_ELEMENT_NODE ){
           if(!xmlStrcmp(child_node->name, (const xmlChar *) "timezone")){
               for (child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                   if (child_node2->type == XML_ELEMENT_NODE ){
                       /* fprintf(stderr,"second name %s", child_node2->name); */
                        if(!xmlStrcmp(child_node2->name, (const xmlChar *) "rawOffset")){
                            xmlChar     *temp_xml_string = NULL;
                            temp_xml_string = xmlNodeGetContent(child_node2);
                            if (temp_xml_string != NULL){
                                station_timezone = atoi((char *)temp_xml_string);
                                xmlFree(temp_xml_string);
                            }
                        }
                    }
               }   
           }
       }
    }
    fprintf(file_out,"    <timezone>%i</timezone>\n", station_timezone);
    fclose(file_out);
    return 0;
}

/*******************************************************************************/
int
parse_and_write_current_data(htmlDocPtr doc, const char *result_file){

    FILE        *file_out;
    xmlNode     *child_node = NULL,
                *child_node2 = NULL;
    xmlNode *root_node = NULL;
    char   buffer[buff_size];
    char   temp_buffer[buff_size];
    time_t      utc_time;
    struct tm   tmp_tm = {0};
    struct tm   tmp_tm2 = {0};
    time_t      utc_time_start;
    time_t      utc_time_end;
    time_t      utc_time_sunrise = -1;
    time_t      utc_time_sunset = -1;
    int         count_day = 0;
    int         temp;
    char        short_text[1024];
    char        icon[256],
                ppcp[128],
                wind_direction[10],
                wind_speed[10],
                pressure[10],
                humidity[10];

    time_t      current_time;
    int         localtimezone;
    struct      tm time_tm1 = {0};
    struct      tm time_tm2 = {0};
    xmlChar     *temp_prop;

    if(!doc)
        return -1;

    file_out = fopen(result_file, "aw");
    if (!file_out)
        return -1;

    root_node = xmlDocGetRootElement(doc);

    /* clear variables */
    temp = INT_MAX;  
    memset(short_text, 0, sizeof(short_text));
    memset(icon, 0, sizeof(icon));
    memset(wind_direction, 0, sizeof(wind_direction));
    memset(wind_speed, 0, sizeof(wind_speed));
    memset(ppcp, 0, sizeof(ppcp));
    memset(pressure, 0, sizeof(pressure));
    memset(humidity, 0, sizeof(humidity));

    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 
   
    for(child_node = root_node->children; child_node; child_node = child_node->next){
       if (child_node->type == XML_ELEMENT_NODE ){
           if(!xmlStrcmp(child_node->name, (const xmlChar *) "city")){
               for (child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                   if (child_node2->type == XML_ELEMENT_NODE ){
                       if(!xmlStrcmp(child_node2->name, (const xmlChar *) "sun")){
                            if ((temp_prop = xmlGetProp(child_node2, (const xmlChar*)"rise")) != NULL){
                                memset(temp_buffer, 0, sizeof(buffer));
                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s", (char *)temp_prop);
                                xmlFree(temp_prop);
                                temp_prop = NULL;
                                strptime(temp_buffer, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                                utc_time_sunrise = mktime(&tmp_tm) + localtimezone*3600;
                            }
                            if ((temp_prop = xmlGetProp(child_node2, (const xmlChar*)"set")) != NULL){
                                memset(temp_buffer, 0, sizeof(buffer));
                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s",(char *)temp_prop);
                                xmlFree(temp_prop);
                                temp_prop = NULL;
                                strptime(temp_buffer, "%Y-%m-%dT%H:%M:%S", &tmp_tm2);
                                utc_time_sunset = mktime(&tmp_tm2) + localtimezone*3600;
                            }
                       }
                   }
               }   
           }

           if(!xmlStrcmp(child_node->name, (const xmlChar *) "lastupdate")){
               if((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL) {
                   struct tm tmp_tm1 = {0};
                   memset(temp_buffer, 0, sizeof(buffer));
                   snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s", (char *)temp_prop);
                   xmlFree(temp_prop);
                   temp_prop = NULL;
                   strptime(temp_buffer, "%Y-%m-%dT%H:%M:%S", &tmp_tm1);
                   /* fprintf(stderr, "Current time buffer %s\n", temp_buffer); */
                   utc_time_start = mktime(&tmp_tm1) + localtimezone*3600;
                   utc_time_end = mktime(&tmp_tm1) + localtimezone*3600 + 4*3600;
               }
           }

           if(!xmlStrcmp(child_node->name, (const xmlChar *) "temperature")){
                if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                   temp = atoi((char *)temp_prop);
                   xmlFree(temp_prop);
                   temp_prop = NULL;
                }
           }

           if(!xmlStrcmp(child_node->name, (const xmlChar *) "precipitation")){
                if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                   snprintf(ppcp, sizeof(ppcp)-1,"%s", (char *)temp_prop);
                   xmlFree(temp_prop);
                   temp_prop = NULL;
                }
           }

           if(!xmlStrcmp(child_node->name, (const xmlChar *) "wind")){
               for (child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                   if (child_node2->type == XML_ELEMENT_NODE ){
                       if(!xmlStrcmp(child_node2->name, (const xmlChar *) "speed")){
                           if ((temp_prop = xmlGetProp(child_node2, (const xmlChar*)"value")) != NULL){
                                snprintf(wind_speed, sizeof(wind_speed)-1,"%s",(char *)temp_prop);
                                xmlFree(temp_prop);
                                temp_prop = NULL;
                           }
                       }
                       if(!xmlStrcmp(child_node2->name, (const xmlChar *) "direction")){
                           if ((temp_prop = xmlGetProp(child_node2, (const xmlChar*)"code")) != NULL){
                                snprintf(wind_direction, sizeof(wind_direction)-1,"%s", (char *)temp_prop);
                                xmlFree(temp_prop);
                                temp_prop = NULL;
                           } 
                       }
                   }
               }   
           }

           if(!xmlStrcmp(child_node->name, (const xmlChar *) "pressure")){
               if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                    snprintf(pressure, sizeof(pressure)-1,"%s", (char *)temp_prop);
                    xmlFree(temp_prop);
                    temp_prop = NULL;
               }
           }

           if(!xmlStrcmp(child_node->name, (const xmlChar *) "humidity")){
               if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                   snprintf(humidity, sizeof(humidity)-1,"%s", (char *)temp_prop);
                   xmlFree(temp_prop);
                   temp_prop = NULL;
               }
           }

           /* symbol number="801" name="few clouds" var="02d" */
           if(!xmlStrcmp(child_node->name, (const xmlChar *) "weather")){
               memset(temp_buffer, 0, sizeof(buffer));
               if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"number")) != NULL){
                   snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s", (char *)temp_prop);
                   xmlFree(temp_prop);
                   temp_prop = NULL;
               }
               if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"icon")) != NULL){
                   strcat(temp_buffer, (char *)temp_prop);
                   xmlFree(temp_prop);
                   temp_prop = NULL;
               }
               if (xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer))
                   snprintf(icon, sizeof(icon)-1, "%s",
                         (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));
               if ((temp_prop = xmlGetProp(child_node, (const xmlChar*)"value")) != NULL){
                   snprintf(short_text, sizeof(short_text)-1, "%s",(char *)temp_prop);
                   short_text[0] = toupper(short_text[0]);
                   xmlFree(temp_prop);
                   temp_prop = NULL;
               }
           }
       }
    }
    fprintf(file_out,"    <period current=\"true\" start=\"%li\"", utc_time_start);
    fprintf(file_out," end=\"%li\">\n", utc_time_end); 

    if (strlen (short_text)>0)
        fprintf(file_out, "     <description>%s</description>\n", short_text);
    if (strlen(icon)>0)
        fprintf(file_out, "     <icon>%s</icon>\n", icon);
    if (temp != INT_MAX)
        fprintf(file_out, "     <temperature>%i</temperature>\n", temp);				                
    if (strlen (ppcp)>0)
        fprintf(file_out, "     <ppcp>%s</ppcp>\n", ppcp);

    if (strlen (wind_direction)>0)
        fprintf(file_out, "     <wind_direction>%s</wind_direction>\n", wind_direction);

    if (strlen (wind_speed)>0)
        fprintf(file_out, "     <wind_speed>%s</wind_speed>\n", wind_speed);
    
    if (strlen (pressure)>0)
        fprintf(file_out, "     <pressure>%s</pressure>\n", pressure);

    if (strlen (humidity)>0)
        fprintf(file_out, "     <humidity>%s</humidity>\n", humidity);
    fprintf(file_out, "    </period>\n");
    if (utc_time_sunset != -1 && utc_time_sunrise != -1){
        tmp_tm.tm_min = 0;
        tmp_tm.tm_hour = 0;
        utc_time = mktime(&tmp_tm);

        fprintf(file_out,"    <period start=\"%li\"", utc_time);
        fprintf(file_out," end=\"%li\">\n", utc_time + 24*3600); 
        fprintf(file_out,"     <sunset>%li</sunset>\n", utc_time_sunset);
        fprintf(file_out,"     <sunrise>%li</sunrise>\n", utc_time_sunrise);
    }
    fprintf(file_out, "    </period>\n");
    count_day++;
    fclose(file_out);
    return count_day;
}

/*******************************************************************************/
int
convert_station_openweathermaporg_data(const char *days_data_path, const char *result_file, const char *current_data_path, const char *hours_data_path ){
 
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int     days_number = -1;
    char    buffer[2048],
            *delimiter = NULL;
    FILE    *file_out;
    
    if(!days_data_path)
        return -1;

    hash_for_icons = hash_icons_openweathermaporg_table_create();
    snprintf(buffer, sizeof(buffer)-1,"%s.timezone", result_file);
    if(!access(buffer, R_OK)){
         doc =  xmlReadFile(buffer, "UTF-8", 0);
        if(doc){
            root_node = NULL;
            root_node = xmlDocGetRootElement(doc);
            if(!root_node || ( root_node->type == XML_ELEMENT_NODE &&
                    strstr((char*)root_node->name, "err"))){
                xmlFreeDoc(doc);
                xmlCleanupParser();
            }else{
                parse_and_write_timezone_data(doc, result_file);
                xmlFreeDoc(doc);
                xmlCleanupParser();
            }
        }
    }

    /* check file accessability */
    if(!access(days_data_path, R_OK)){
        /* check that the file containe valid data */
        doc =  xmlReadFile(days_data_path, "UTF-8", 0);
        if(!doc){
            xmlHashFree(hash_for_icons, NULL);
            return -1;
        }
        root_node = xmlDocGetRootElement(doc);
        if(root_node && root_node->type == XML_ELEMENT_NODE &&
                strstr((char*)root_node->name, "err")){
            xmlFreeDoc(doc);
            xmlCleanupParser();
            xmlHashFree(hash_for_icons, NULL);
            return -2;
        }else{
            days_number = parse_and_write_days_xml_data(doc, result_file);
            xmlFreeDoc(doc);
            xmlCleanupParser();
            if(!access(current_data_path, R_OK)){
                 doc =  xmlReadFile(current_data_path, "UTF-8", 0);
                if(doc){
                    root_node = NULL;
                    root_node = xmlDocGetRootElement(doc);
                    if(!root_node || ( root_node->type == XML_ELEMENT_NODE &&
                            strstr((char*)root_node->name, "err"))){
                        xmlFreeDoc(doc);
                        xmlCleanupParser();
                    }else{
                        parse_and_write_current_data(doc, result_file);
                        xmlFreeDoc(doc);
                        xmlCleanupParser();
                    }
                }
            }
            if(!access(hours_data_path, R_OK)){
                 doc =  xmlReadFile(hours_data_path, "UTF-8", 0);
                if(doc){
                    root_node = NULL;
                    root_node = xmlDocGetRootElement(doc);
                    if(!root_node || ( root_node->type == XML_ELEMENT_NODE &&
                            strstr((char*)root_node->name, "err"))){
                        xmlFreeDoc(doc);
                        xmlCleanupParser();
                    }else{
                        parse_and_write_hours_data(doc, result_file);
                        xmlFreeDoc(doc);
                        xmlCleanupParser();
                    }
                }
            }

            if (days_number > 0){
            
                file_out = fopen(result_file, "a");
                if (file_out){
                    fprintf(file_out,"</station>");
                    fclose(file_out);
                }
            }
        }
    }else{
        xmlHashFree(hash_for_icons, NULL);
        return -1;/* file isn't accessability */
    }

    xmlHashFree(hash_for_icons, NULL);
    return days_number;
}

/*******************************************************************************/
int
main_openweathermap_org(int argc, char *argv[]){
    int result; 
    if (argc < 4) {
        fprintf(stderr, "openweathermaporg <input_days_file> <output_file> <input_hours_file> <input_current_file>\n");
        return -1;
    }
    result = convert_station_openweathermaporg_data(argv[1], argv[2], argv[3], argv[4]);
    //fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
