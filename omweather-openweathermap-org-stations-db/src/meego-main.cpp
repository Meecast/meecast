/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-openweathermap-org-source - MeeCast
 *
 * Copyright (C) 2013 Vlad Vasilyeu
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

#define buff_size 2048
/*******************************************************************************/
int
parse_and_write_days_xml_data(htmlDocPtr doc, const char *result_file){

    FILE        *file_out;
    xmlNode     *cur_node = NULL,
                *cur_node0 = NULL,
                *child_node = NULL,
                *child_node2 = NULL,
                *child_node3 = NULL,
                *child_node4 = NULL;
    xmlNode *root_node = NULL;
    char   buffer[buff_size],
           buffer2[buff_size];
    char   temp_buffer[buff_size];
    time_t      utc_time;
    struct tm   tmp_tm = {0};
    time_t      utc_time_start;
    time_t      utc_time_end;
    int         count_day = 0;
    int         temp_hi, temp_low;
    char        id_station[1024],
                short_text[1024];
    char       icon[256],
               ppcp[128];

    if(!doc)
        return -1;

    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;

    root_node = xmlDocGetRootElement(doc);

    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"Unknown\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n");
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");
    for(cur_node0 = root_node->children; cur_node0; cur_node0 = cur_node0->next){
        if( cur_node0->type == XML_ELEMENT_NODE ){
            if (!xmlStrcmp(cur_node0->name, (const xmlChar *) "forecast" ) ){
                for(cur_node = cur_node0->children; cur_node; cur_node = cur_node->next){
                    if( cur_node->type == XML_ELEMENT_NODE ){
                        /* get weather data */
                        if(!xmlStrcmp(cur_node->name, (const xmlChar *) "time")){

                         //       fprintf(stderr, "Element time\n" );
                            if(xmlGetProp(cur_node, (const xmlChar*)"day")) {
                                memset(temp_buffer, 0, sizeof(buffer));
                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s",
                                                    xmlGetProp(cur_node, (const xmlChar*)"day"));
                                strptime(temp_buffer, "%Y-%m-%d", &tmp_tm);
                                fprintf(stderr, "Element %s\n", xmlGetProp(cur_node, (const xmlChar*)"day"));
                                utc_time_start = mktime(&tmp_tm);
                                utc_time_end = mktime(&tmp_tm) + 24*3600;
                                /* clear variables */
                                temp_hi = INT_MAX; temp_low = INT_MAX; 
                                memset(short_text, 0, sizeof(short_text));
                                memset(icon, 0, sizeof(icon));
                                memset(ppcp, 0, sizeof(ppcp));


                                for (child_node = cur_node->children; child_node; child_node = child_node->next){
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "temperature")){
                                            temp_low = atoi((char *)xmlGetProp(child_node, (const xmlChar*)"min"));
                                            temp_hi = atoi((char *)xmlGetProp(child_node, (const xmlChar*)"max"));
                                        }
                                    }
                                }
                                fprintf(file_out,"    <period start=\"%li\"", utc_time_start);
                                fprintf(file_out," end=\"%li\">\n", utc_time_end); 

                                if (temp_hi != INT_MAX)
                                            fprintf(file_out,"     <temperature_hi>%i</temperature_hi>\n", temp_hi);				                
                                if (temp_low != INT_MAX)
                                            fprintf(file_out,"     <temperature_low>%i</temperature_low>\n", temp_low);
 
                                fprintf(file_out,"    </period>\n");
                                count_day++;


                            }

                        }
#if 0
                        if(!xmlStrcmp(cur_node->name, (const xmlChar *) "area")){
                            if(xmlGetProp(cur_node, (const xmlChar*)"type") &&
                               !xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"type"), 
                                                               (const xmlChar *) "location" )){
                                temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"description");
                                snprintf(id_station, sizeof(id_station) - 1,
                                            "%s", temp_xml_string);
                                xmlFree(temp_xml_string);
                                /* If station in xml not station in config file exit */
                                if(strcmp(id_station, station_name))
                                    continue;
                                for (child_node = cur_node->children; child_node; child_node = child_node->next){
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        /* clear variables */
                                        temp_hi = INT_MAX; temp_low = INT_MAX; 
                                        memset(short_text, 0, sizeof(short_text));
                                        memset(icon, 0, sizeof(icon));
                                        memset(ppcp, 0, sizeof(ppcp));
                                        /* station name */
                                        if (!xmlStrcmp(child_node->name, (const xmlChar *)"forecast-period") ){
                                            /* get timezone */
                                            if (!check_timezone  && xmlGetProp(child_node, (const xmlChar*)"start-time-local") != NULL){
                                                memset(buffer, 0, sizeof(buffer));
                                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s",
                                                                      xmlGetProp(child_node, (const xmlChar*)"start-time-local"));
                                                for (i=strlen(temp_buffer) - 6; i < strlen(temp_buffer) -3; i++){
                                                    sprintf(buffer,"%s%c", buffer, temp_buffer[i]);
                                                }
                                                fprintf(file_out,"  <timezone>%s</timezone>\n", buffer);
                                                au_timezone = atoi(buffer);
                                                check_timezone = TRUE;
                                            }
                                            /* get start time for period */
                                            if (xmlGetProp(child_node, (const xmlChar*)"start-time-utc") != NULL){
                                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s",
                                                                      xmlGetProp(child_node, (const xmlChar*)"start-time-utc"));
                                                strptime(temp_buffer, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                                                utc_time_start = mktime(&tmp_tm) + au_timezone*3600;
                                            }
                                            /* get end time for period */
                                            if (xmlGetProp(child_node, (const xmlChar*)"end-time-utc") != NULL){
                                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s",
                                                                      xmlGetProp(child_node, (const xmlChar*)"end-time-utc"));
                                                strptime(temp_buffer, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                                                utc_time_end = mktime(&tmp_tm) + au_timezone*3600;
                                            }
                                            /* get index */
                                            if (xmlGetProp(child_node, (const xmlChar*)"index") != NULL){
                                                index = atoi((char *)xmlGetProp(child_node, (const xmlChar*)"index"));
                                                fprintf(stderr,"Index %i\n", index);
                                            }

                                            for (child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){

                                                if (child_node2->type == XML_ELEMENT_NODE ){
                                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *) "element")){                                               
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "air_temperature_minimum" ))
                                                            temp_low = atoi((char *)xmlNodeGetContent(child_node2));
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "air_temperature_maximum" ))
                                                            temp_hi = atoi((char *)xmlNodeGetContent(child_node2));
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "forecast_icon_code" ))
#ifdef GLIB                                                         
                                                            snprintf(icon, sizeof(icon) - 1, "%s", choose_hour_weather_icon(hash_for_icons, (char *)xmlNodeGetContent(child_node2))); 
#endif
#ifdef QT
                                                            snprintf(icon, sizeof(icon) - 1, "%s", (char*)choose_hour_weather_icon(hash_for_icons, (char *)xmlNodeGetContent(child_node2)).toStdString().c_str()); 
#endif
                                                    }
                                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *) "text")){                           
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "precis" )){
                                                            snprintf(short_text, sizeof(short_text)-1,"%s",
                                                                     xmlNodeGetContent(child_node2));
                                                            if (strlen (short_text) >1)
                                                                short_text[strlen(short_text) - 1] = 0;
                                                        }
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "probability_of_precipitation")){
                                                            snprintf(ppcp, sizeof(ppcp)-1,"%s",
                                                                     xmlNodeGetContent(child_node2));
                                                            if (strlen (ppcp) >1)
                                                                ppcp[strlen(ppcp) - 1] = 0;
                                                        }
                                                    }
                                                }
                                            }
                                            temp_xml_string = xmlNodeGetContent(child_node);
                                            xmlFree(temp_xml_string);
                                        }
                                        fprintf(file_out,"    <period start=\"%li\"", utc_time_start);
                                        fprintf(file_out," end=\"%li\">\n", utc_time_end); 
                                        if (temp_hi != INT_MAX)
                                            fprintf(file_out,"     <temperature_hi>%i</temperature_hi>\n", temp_hi);				                
                                        if (temp_low != INT_MAX)
                                            fprintf(file_out,"     <temperature_low>%i</temperature_low>\n", temp_low);
                                        if (strlen(icon)>0){
                                            fprintf(file_out, "     <icon>%s</icon>\n", icon);
                                            if (index==0)
                                                snprintf(current_icon, sizeof(current_icon)-1,"%s", icon);
                                        }
                                        if (strlen (short_text)>0){
                                            fprintf(file_out, "     <description>%s</description>\n", short_text);
                                            if (index==0)
                                                snprintf(current_title, sizeof(current_title)-1,"%s", short_text);
                                        }
                                        if (strlen (ppcp)>0)
                                            fprintf(file_out, "     <ppcp>%s</ppcp>\n", ppcp);

                                        fprintf(file_out,"    </period>\n");
                                        count_day++;
                                    }
                                }
                            }
                        }
#endif
                    }
                }
            }          
        }
    }


}
int
convert_station_openweathermaporg_data(const char *days_data_path, const char *result_file, const char *hours_data_path, const char *current_data_path ){
 
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int     days_number = -1;
    char    buffer[1024],
            *delimiter = NULL;
    FILE    *file_out;
    
    if(!days_data_path)
        return -1;
    /* check file accessability */
    if(!access(days_data_path, R_OK)){
        /* check that the file containe valid data */
        doc =  xmlReadFile(days_data_path, "UTF-8", 0);
        if(!doc)
            return -1;
        root_node = xmlDocGetRootElement(doc);
        if(root_node->type == XML_ELEMENT_NODE &&
                strstr((char*)root_node->name, "err")){
            xmlFreeDoc(doc);
            xmlCleanupParser();
            return -2;
        }else{
            days_number = parse_and_write_days_xml_data(doc, result_file);
            fprintf(stderr, "First success\n");
            xmlFreeDoc(doc);
            xmlCleanupParser();
            /*
            if(!access(detail_path_data, R_OK)){
                 doc =  htmlReadFile(detail_path_data, "UTF-8", 0);
                if(doc){
                    root_node = NULL;
                    root_node = xmlDocGetRootElement(doc);
                    if(!root_node || ( root_node->type == XML_ELEMENT_NODE &&
                            strstr((char*)root_node->name, "err"))){
                        xmlFreeDoc(doc);
                        xmlCleanupParser();
                    }
                    else{
                        parse_and_write_detail_data(buffer2, doc, result_file);
                        xmlFreeDoc(doc);
                        xmlCleanupParser();
                    }
                }
             }
           */
            if (days_number > 0){
                file_out = fopen(result_file, "a");
                if (file_out){
                    fprintf(file_out,"</station>");
                    fclose(file_out);
                }
            }
        }
    }else
        return -1;/* file isn't accessability */

    return days_number;
}

/*******************************************************************************/
int
main(int argc, char *argv[]){
    int result; 
    if (argc < 4) {
        fprintf(stderr, "openweathermaporg <input_days_file> <output_file> <input_hours_file> <input_current_file>\n");
        return -1;
    }
    result = convert_station_openweathermaporg_data(argv[1], argv[2], argv[3], argv[4]);
    //fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
