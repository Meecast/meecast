/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-weather-com-stations-db
 *
 * Copyright (C) 2006-2019 Vlad Vasilyeu
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
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
#include "main.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <locale.h>
#include <iostream> 
#include "json/json.h"
/*******************************************************************************/
int
get_station_weather_data(const gchar *station_id_with_path, GHashTable *data,
                                                    gboolean get_detail_data){
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int    days_number = -1;
    char   buffer[1024],
           buffer2[1024],
           *delimiter = NULL;
    struct stat file_info;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!station_id_with_path || !data)
        return -1;
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s.new", station_id_with_path);
    /* check file accessibility */
    if(!access(buffer, R_OK))
        if ((lstat(buffer, &file_info) == 0) && (file_info.st_size > 0)){ 
            /* check that the file containe valid data */
            doc = xmlReadFile(buffer, NULL, 0);
            if(doc){
                root_node = xmlDocGetRootElement(doc);
                if(root_node->type == XML_ELEMENT_NODE &&
                        strstr((char*)root_node->name, "err")){
                    xmlFreeDoc(doc);
                    xmlCleanupParser();
                }else{
                    /* prepare station id */
                    *buffer2 = 0;
                    delimiter = strrchr(buffer, '/');
                    if(delimiter){
                        delimiter++; /* delete '/' */
                        snprintf(buffer2, sizeof(buffer2) - 1, "%s", delimiter);
                        delimiter = strrchr(buffer2, '.');
                        if (delimiter)
                            *delimiter = 0;
                        delimiter = strrchr(buffer2, '.');
                        if(!delimiter){
                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                        }else{
                            *delimiter = 0;
                            days_number = parse_xml_data(buffer2, root_node, data);
                            rename(buffer, station_id_with_path);
                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                            return days_number;
                        }
                    }
               }
            }else
                doc = NULL;
        }
    /* check file accessibility */
    if(!access(station_id_with_path, R_OK)){
        /* check that the file containe valid data */
        doc = xmlReadFile(station_id_with_path, NULL, 0);
        if(!doc)
            return -1;
        *buffer = 0;
        snprintf(buffer, sizeof(buffer) - 1, "%s", station_id_with_path);
        root_node = xmlDocGetRootElement(doc);
        if(root_node->type == XML_ELEMENT_NODE &&
                strstr((char*)root_node->name, "err")){
            xmlFreeDoc(doc);
            xmlCleanupParser();
            return -2;
        }
        else{
            /* prepare station id */
            *buffer = 0;
            delimiter = strrchr(buffer, '/');
            if(delimiter){
                delimiter++; /* delete '/' */
                snprintf(buffer2, sizeof(buffer2) - 1, "%s", delimiter);
                delimiter = strrchr(buffer2, '.');
                if(!delimiter){
                    xmlFreeDoc(doc);
                    xmlCleanupParser();
                    return -1;
                }
                *delimiter = 0;
                days_number = parse_xml_data(buffer, root_node, data);
            }
            xmlFreeDoc(doc);
            xmlCleanupParser();
            return days_number;
        }
    }
    else
        return -1;/* file isn't accessability */
}
/*******************************************************************************/
int
parse_xml_data(const gchar *station_id, xmlNode *root_node, GHashTable *data){
    xmlNode     *cur_node = NULL,
                *child_node = NULL,
                *child_node2 = NULL,
                *child_node3 = NULL,
                *child_node4 = NULL;
    xmlChar     *temp_xml_string = NULL;
    xmlChar     *part_of_day = NULL;
    int         store2day = 0,
                count_day = 0;
    gchar       id_station[10],
                buff[256];
    struct tm   tmp_tm = {0,0,0,0,0,0,0,0,0,0,0}; 
    GSList      *forecast = NULL;
    GHashTable  *location = NULL,
                *current = NULL,
                *day = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    for(cur_node = root_node->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            /* get weather station data */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "loc" ) ){
                temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"id");
                snprintf(id_station, sizeof(id_station) - 1,
                            "%s", temp_xml_string);
                xmlFree(temp_xml_string);
                /* If station in xml not station in config file exit */
                if( strcmp(id_station, station_id) )
                    return -1;
                location = g_hash_table_new(g_str_hash, g_str_equal);
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    if( child_node->type == XML_ELEMENT_NODE ){
                        /* station name */
                        if( !xmlStrcmp(child_node->name, (const xmlChar *)"dnam") ){
                            temp_xml_string = xmlNodeGetContent(child_node);
                            g_hash_table_insert(location, (gpointer)"station_name",
                                                (gpointer)g_strdup((char*)temp_xml_string));
                            xmlFree(temp_xml_string);
                            continue;
                        }
                        /* station lattitude */
                        if( !xmlStrcmp(child_node->name, (const xmlChar *)"lat") ){
                            temp_xml_string = xmlNodeGetContent(child_node);
                            g_hash_table_insert(location, (gpointer)"station_latitude",
                                                g_strdup((char*)temp_xml_string));
                            xmlFree(temp_xml_string);
                            continue;
                        }
                        /* station longitude */
                        if( !xmlStrcmp(child_node->name, (const xmlChar *)"lon") ){
                            temp_xml_string = xmlNodeGetContent(child_node);
                            g_hash_table_insert(location, (gpointer)"station_longitude",
                                                g_strdup((char*)temp_xml_string));
                            xmlFree(temp_xml_string);
                            continue;
                        }
                        /* station time zone */
                        if( !xmlStrcmp(child_node->name, (const xmlChar *)"zone") ){
                            temp_xml_string = xmlNodeGetContent(child_node);
                            g_hash_table_insert(location, (gpointer)"station_time_zone",
                                                g_strdup((char*)temp_xml_string));
                            xmlFree(temp_xml_string);
                            continue;
                        }
                    }
                }
                g_hash_table_insert(data, (gpointer)"location", (gpointer)location);
            }
            /* Fill current day */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "cc" ) ){
                current = g_hash_table_new(g_str_hash, g_str_equal);
                for(child_node = cur_node->children; child_node != NULL; child_node = child_node->next){
                    /* last update */
                    if( child_node->type == XML_ELEMENT_NODE  &&
                            ( !xmlStrcmp(child_node->name, (const xmlChar *)"lsup") ) ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(current, (gpointer)"last_update", g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    /* temperature */
                    if( child_node->type == XML_ELEMENT_NODE  &&
                            ( !xmlStrcmp(child_node->name, (const xmlChar *)"tmp") ) ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(current, (gpointer)"day_hi_temperature",
                                            g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    /* feel like */
                    if( child_node->type == XML_ELEMENT_NODE  &&
                            ( !xmlStrcmp(child_node->name, (const xmlChar *)"flik") ) ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(current, (gpointer)"feel_like",
                                            g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    /* title */
                    if(!xmlStrcmp(child_node->name, (const xmlChar *)"t") ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(current, (gpointer)"title", g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    /* icon */
                    if(!xmlStrcmp(child_node->name, (const xmlChar *)"icon") ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(current, (gpointer)"icon",
                                            g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    /* pressure data */
                    if(!xmlStrcmp(child_node->name, (const xmlChar *)"bar") ){
                        for(child_node2 = child_node->children; child_node2 != NULL; child_node2 = child_node2->next){
                            if( child_node2->type == XML_ELEMENT_NODE ){
                                /* pressure */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"r") ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(current, (gpointer)"pressure",
                                                        g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                }
                                /* direction */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"d") ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(current, (gpointer)"pressure_direction",
                                                        g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                }
                            }
                        }
                        continue;
                    }
                    /* humidity */
                    if(!xmlStrcmp(child_node->name, (const xmlChar *)"hmid") ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(current, (gpointer)"humidity", g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    /* visible */
                    if(!xmlStrcmp(child_node->name, (const xmlChar *)"vis") ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(current, (gpointer)"visible", g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    /* wind data */
                    if(!xmlStrcmp(child_node->name, (const xmlChar *)"wind") ){
                        for(child_node2 = child_node->children; child_node2 != NULL; child_node2 = child_node2->next){
                            if( child_node2->type == XML_ELEMENT_NODE ){
                                /* speed */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"s") ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(current, (gpointer)"wind_speed", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                }
                                /* gust */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"gust") ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(current, (gpointer)"wind_gust", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                }
                                /* direction */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t") ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(current, (gpointer)"wind_direction", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
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
                                    g_hash_table_insert(current, "moon_icon", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                }
*/
                                /* phase */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t") ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(current, (gpointer)"moon_phase", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                }
                            }
                        }
                        continue;
                    }
                }/* end for */
                g_hash_table_insert(data, (gpointer)"current", (gpointer)current);
            }
            /* Fill other days */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "dayf" ) ){
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    if(child_node->type == XML_ELEMENT_NODE  &&
                            ( !xmlStrcmp(child_node->name, (const xmlChar *)"day") ) ){
                        day = g_hash_table_new(g_str_hash, g_str_equal);
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
                        g_hash_table_insert(day, (gpointer)"day_name", g_strdup(buff));
                        xmlFree(temp_xml_string);
                        /* get 24h date */
                        temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"dt");
                        g_hash_table_insert(day, (gpointer)"day_date", g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                            if( child_node2->type == XML_ELEMENT_NODE){
                                /* 24h hi temperature */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"hi")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(day, (gpointer)"day_hi_temperature", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                                }
                                /* 24h low temperature */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"low")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(day, (gpointer)"day_low_temperature", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                                }
                                /* 24h sunrise */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"sunr")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(day, (gpointer)"day_sunrise", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                                }
                                /* 24h sunset */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"suns")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(day, (gpointer)"day_sunset", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
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
                                                    g_hash_table_insert(day, (gpointer)"night_humidity", g_strdup((char*)temp_xml_string));
                                                else
                                                    g_hash_table_insert(day, (gpointer)"day_humidity", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                                continue;
                                            }
                                            /* ppcp */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar *)"ppcp") ){
                                                temp_xml_string = xmlNodeGetContent(child_node3);
                                                if(!store2day)
                                                    g_hash_table_insert(day, (gpointer)"night_ppcp", g_strdup((char*)temp_xml_string));
                                                else
                                                    g_hash_table_insert(day, (gpointer)"day_ppcp", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                                continue;
                                            }
                                            /* title */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar *)"t") ){
                                                temp_xml_string = xmlNodeGetContent(child_node3);
                                                if(!store2day)
                                                    g_hash_table_insert(day, (gpointer)"night_title", g_strdup((char*)temp_xml_string));
                                                else
                                                    g_hash_table_insert(day, (gpointer)"day_title", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                                continue;
                                            }
                                            /* icon */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar *)"icon") ){
                                                temp_xml_string = xmlNodeGetContent(child_node3);
                                                if(!store2day)
                                                    g_hash_table_insert(day, (gpointer)"night_icon", g_strdup((char*)temp_xml_string));
                                                else
                                                    g_hash_table_insert(day, (gpointer)"day_icon", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
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
                                                                g_hash_table_insert(day, (gpointer)"night_wind_speed", g_strdup((char*)temp_xml_string));
                                                            else
                                                                g_hash_table_insert(day, (gpointer)"day_wind_speed", g_strdup((char*)temp_xml_string));
                                                            xmlFree(temp_xml_string);
                                                            continue;
                                                        }
                                                        /* title */
                                                        if(!xmlStrcmp(child_node4->name, (const xmlChar *)"t") ){
                                                            temp_xml_string = xmlNodeGetContent(child_node4);
                                                            if(!store2day)
                                                                g_hash_table_insert(day, (gpointer)"night_wind_title",
                                                                                    g_strdup((char*)temp_xml_string));
                                                            else
                                                                g_hash_table_insert(day, (gpointer)"day_wind_title",
                                                                                    g_strdup((char*)temp_xml_string));
                                                            xmlFree(temp_xml_string);
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
                            forecast = g_slist_append(forecast, (gpointer)day);
                            day = NULL;
                            count_day++;
                        }
                    }
                }
                g_hash_table_insert(data, (gpointer)"forecast", (gpointer)forecast);
            }
        }
    }
    return count_day;
}

/*******************************************************************************/
int
parse_and_write_html_data(const gchar *station_id, htmlDocPtr doc, const gchar *result_file){

    xmlXPathContextPtr xpathCtx; 
    xmlXPathObjectPtr xpathObj = NULL; 
    xmlNodeSetPtr nodes;
    struct tm   current_tm = {0};
    time_t      current_time = 0;
    FILE        *file_out;
    struct tm time_tm1;
    struct tm time_tm2;

    char buffer[128000];
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    Json::Value val;
    Json::Value val2;
    Json::Value valday;
    Json::Value valnight;
    Json::Value nullval ;

    int check_timezone = false;
    int timezone = 0;
    int localtimezone = 0;
    int first_day = false;
    int afternoon = false;
    int dark = false;

    struct tm tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
    time_t local_time = 0;


    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;

    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 

    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", station_id);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");

    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
         return(-1);
    }
    /* Register namespaces from list (if any) */
    xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"html",
                                (const xmlChar*)"http://www.w3.org/1999/xhtml");


   /* Day weather forecast */
   /* Evaluate xpath expression */

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/script[contains(text(),'window.__data')]/text()", xpathCtx);
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", "/html/body/script[contains(text(),'window.__data')]/text()");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
    }

    snprintf(buffer, xmlStrlen(xpathObj->nodesetval->nodeTab[0]->content) - 65 - 14 -1,"%s", xpathObj->nodesetval->nodeTab[0]->content + 14);
    //fprintf(stderr, "%s\n", buffer);
    bool parsingSuccessful = reader.parse(buffer, root, false);

    if (!parsingSuccessful)
        return -1;

    //std::cerr<<root["dal"]["Observation"][root["dal"]["Observation"].getMemberNames()[0]]["data"]["vt1observation"]<<std::endl;

    if (root["dal"]["Observation"][root["dal"]["Observation"].getMemberNames()[0]]["data"]["vt1observation"].isObject()){

        val = root["dal"]["Observation"][root["dal"]["Observation"].getMemberNames()[0]]["data"]["vt1observation"];
        std::string current_time_string;
        time_t utc_time = 0;
        current_time_string = val.get("observationTime","").asString();

        if (current_time_string != "" && current_time_string.length()>22){
            tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
            tmp_tm.tm_isdst = time_tm2.tm_isdst;
            setlocale(LC_TIME, "POSIX");
            strptime((const char*)current_time_string.c_str(), "%Y-%m-%dT%H:%M", &tmp_tm);
            utc_time = mktime(&tmp_tm); 
            setlocale(LC_TIME, "");
            fprintf(file_out,"    <period start=\"%li\"", utc_time - 3600);
            fprintf(file_out," end=\"%li\"  current=\"true\">\n", utc_time + 3*3600); 

            fprintf(file_out,"      <temperature>%i</temperature>\n", val.get("temperature","").asInt());
            fprintf(file_out,"      <flike>%i</flike>\n", val.get("feelsLike","").asInt());
            fprintf(file_out,"      <icon>%i</icon>\n", val.get("icon","").asInt());
            fprintf(file_out,"      <description>%s</description>\n", val.get("phrase","").asCString());
            fprintf(file_out,"      <wind_direction>%s</wind_direction>\n", val.get("windDirCompass","").asCString());
            fprintf(file_out,"      <wind_speed>%1.f</wind_speed>\n", (double)(val.get("windSpeed","").asInt()) * 1000/3600); 
            fprintf(file_out,"      <humidity>%i</humidity>\n", val.get("humidity","").asInt());
            fprintf(file_out,"      <pressure>%1.f</pressure>\n", val.get("altimeter","").asFloat());
            fprintf(file_out,"      <pressure_direction>%s</pressure_direction>\n", val.get("barometerTrend","").asCString());
            fprintf(file_out,"      <dewpoint>%i</dewpoint>\n", val.get("dewPoint","").asInt());
            fprintf(file_out,"      <wind_gust>%1.f</wind_gust>\n", (double)(val.get("gust","0").asInt()) * 1000/3600);
            fprintf(file_out,"      <uv_index>%i</uv_index>\n", val.get("uvIndex","").asInt() );
            fprintf(file_out,"      <visibility>%i</visibility>\n",(int)(val.get("visibility","").asDouble()*1000) );
            fprintf(file_out,"    </period>\n");
        }
    }

    //std::cerr<<root["dal"]["DailyForecast"][root["dal"]["DailyForecast"].getMemberNames()[0]]["data"]["vt1dailyForecast"]<<std::endl;
    if (!root["dal"]["DailyForecast"][root["dal"]["DailyForecast"].getMemberNames()[0]]["data"]["vt1dailyForecast"].isArray())
        return -1;
    val = root["dal"]["DailyForecast"][root["dal"]["DailyForecast"].getMemberNames()[0]]["data"]["vt1dailyForecast"];
    //fprintf(stderr,"Size %i\n", val.size());
    for (uint i = 0; i < val.size(); i++){
        std::string utc_time_string;
        std::string sunrise_time_string;
        std::string sunset_time_string;
        std::string temp_hi_string;
        std::string temp_low_string;
        std::string icon_day_string;
        std::string icon_night_string;
        std::string desc_day_string;
        std::string desc_night_string;
        std::string wind_direct_day_string;
        std::string wind_direct_night_string;
        std::string wind_speed_day_string;
        std::string wind_speed_night_string;
        std::string humidity_day_string;
        std::string humidity_night_string;
        std::string ppcp_day_string;
        std::string ppcp_night_string;
        std::string uv_index_day_string;
        std::string uv_index_night_string;
        time_t utc_time = 0;
        time_t sunrise_time = 0;
        time_t sunset_time = 0;
        time_t offset_time = 0;

        
        utc_time_string = val[i].get("validDate","").asCString();
        //fprintf(stderr,"Time %s\n", utc_time_string.c_str());
        sunrise_time_string = val[i].get("sunrise","").asCString();
        sunset_time_string = val[i].get("sunset","").asCString();
        temp_hi_string = val[i]["day"]["temperature"].asString();
        temp_low_string = val[i]["night"]["temperature"].asString();
        icon_day_string = val[i]["day"]["icon"].asString();
        icon_night_string = val[i]["night"]["icon"].asString();
        desc_day_string = val[i]["day"]["phrase"].asString();
        desc_night_string = val[i]["night"]["phrase"].asString();
        wind_direct_day_string = val[i]["day"]["windDirCompass"].asString();
        wind_direct_night_string = val[i]["night"]["windDirCompass"].asString();
        wind_speed_day_string = val[i]["day"]["windSpeed"].asString();
        wind_speed_night_string = val[i]["night"]["windSpeed"].asString();
        humidity_day_string = val[i]["day"]["humidityPct"].asString();
        humidity_night_string = val[i]["night"]["humidityPct"].asString();
        ppcp_day_string = val[i]["day"]["precipPct"].asString();
        ppcp_night_string = val[i]["night"]["precipPct"].asString();
        uv_index_day_string = val[i]["day"]["uvIndex"].asString();
        uv_index_night_string = val[i]["night"]["uvIndex"].asString();

        if (utc_time_string != "" && utc_time_string.length()>22 &&
            sunrise_time_string != "" && sunrise_time_string.length()>22 &&
            sunset_time_string != "" && sunset_time_string.length()>22
           ){
            tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
            tmp_tm.tm_isdst = time_tm2.tm_isdst;
            setlocale(LC_TIME, "POSIX");
            strptime((const char*)utc_time_string.c_str(), "%Y-%m-%dT", &tmp_tm);
            utc_time = mktime(&tmp_tm); 
            strptime((const char*)sunrise_time_string.c_str(), "%Y-%m-%dT%H:%M", &tmp_tm);
            sunrise_time = mktime(&tmp_tm); 
            strptime((const char*)sunset_time_string.c_str(), "%Y-%m-%dT%H:%M", &tmp_tm);
            sunset_time = mktime(&tmp_tm); 


            setlocale(LC_TIME, "");
            /* get timezone */
            if (!check_timezone){
                char buffer_zone[4];
                memset(buffer_zone, 0, sizeof(buffer_zone));
                snprintf(buffer_zone, sizeof(buffer_zone), "%s", utc_time_string.c_str() + 19);
                timezone = atoi(buffer_zone);
                fprintf(file_out,"  <timezone>%i</timezone>\n", timezone);
                check_timezone = true;
                first_day = true;
                
            }    

            if (desc_night_string != "" && icon_night_string != ""){
                fprintf(file_out,"    <period start=\"%li\"", utc_time);
                fprintf(file_out," end=\"%li\">\n", sunrise_time); 
                if (temp_hi_string != ""){
                    fprintf(file_out,"      <temperature_hi>%s</temperature_hi>\n", temp_hi_string.c_str());
                }else{
                    fprintf(file_out,"      <temperature_hi>%s</temperature_hi>\n", temp_low_string.c_str());
                }
                fprintf(file_out,"      <temperature_low>%s</temperature_low>\n", temp_low_string.c_str());
                fprintf(file_out,"      <icon>%s</icon>\n", icon_night_string.c_str());
                fprintf(file_out,"      <description>%s</description>\n", desc_night_string.c_str());
                fprintf(file_out,"      <wind_direction>%s</wind_direction>\n", wind_direct_night_string.c_str());
                fprintf(file_out,"      <wind_speed>%1.f</wind_speed>\n", (double)(atoi( wind_speed_night_string.c_str())) * 1000/3600);
                fprintf(file_out,"      <humidity>%s</humidity>\n", humidity_night_string.c_str());
                fprintf(file_out,"      <ppcp>%s</ppcp>\n", ppcp_night_string.c_str());
                fprintf(file_out,"      <uv_index>%s</uv_index>\n", uv_index_night_string.c_str());
                fprintf(file_out,"    </period>\n");
            }
            if (desc_day_string != "" && icon_day_string != ""){
                fprintf(file_out,"    <period start=\"%li\"", sunrise_time);
                fprintf(file_out," end=\"%li\">\n", sunset_time); 
                fprintf(file_out,"      <temperature_hi>%s</temperature_hi>\n", temp_hi_string.c_str());
                fprintf(file_out,"      <temperature_low>%s</temperature_low>\n", temp_low_string.c_str());
                fprintf(file_out,"      <icon>%s</icon>\n", icon_day_string.c_str());
                fprintf(file_out,"      <description>%s</description>\n", desc_day_string.c_str());
                fprintf(file_out,"      <wind_direction>%s</wind_direction>\n", wind_direct_day_string.c_str());
                fprintf(file_out,"      <wind_speed>%1.f</wind_speed>\n", (double)(atoi( wind_speed_day_string.c_str())) * 1000/3600);
                fprintf(file_out,"      <humidity>%s</humidity>\n", humidity_day_string.c_str());
                fprintf(file_out,"      <ppcp>%s</ppcp>\n", ppcp_day_string.c_str());
                fprintf(file_out,"      <uv_index>%s</uv_index>\n", uv_index_day_string.c_str());
                fprintf(file_out,"    </period>\n");
            }else{
                if (desc_night_string != "" && icon_night_string != ""){
                    fprintf(file_out,"    <period start=\"%li\"", utc_time);
                    fprintf(file_out," end=\"%li\">\n", utc_time +  24*3600); 
                    if (temp_hi_string != ""){
                        fprintf(file_out,"      <temperature_hi>%s</temperature_hi>\n", temp_hi_string.c_str());
                    }else{
                        fprintf(file_out,"      <temperature_hi>%s</temperature_hi>\n", temp_low_string.c_str());
                    }
                    fprintf(file_out,"      <temperature_low>%s</temperature_low>\n", temp_low_string.c_str());
                    fprintf(file_out,"      <icon>%s</icon>\n", icon_night_string.c_str());
                    fprintf(file_out,"      <description>%s</description>\n", desc_night_string.c_str());
                    fprintf(file_out,"      <wind_direction>%s</wind_direction>\n", wind_direct_night_string.c_str());
                    fprintf(file_out,"      <wind_speed>%1.f</wind_speed>\n", (double)(atoi( wind_speed_night_string.c_str())) * 1000/3600);
                    fprintf(file_out,"      <humidity>%s</humidity>\n", humidity_night_string.c_str());
                    fprintf(file_out,"      <ppcp>%s</ppcp>\n", ppcp_night_string.c_str());
                    fprintf(file_out,"      <uv_index>%s</uv_index>\n", uv_index_night_string.c_str());
                    fprintf(file_out,"    </period>\n");
                }

            }
            if (desc_night_string != "" && icon_night_string != ""){
                fprintf(file_out,"    <period start=\"%li\"", sunset_time);
                fprintf(file_out," end=\"%li\">\n", utc_time +  24*3600); 
                if (temp_hi_string != ""){
                    fprintf(file_out,"      <temperature_hi>%s</temperature_hi>\n", temp_hi_string.c_str());
                }else{
                    fprintf(file_out,"      <temperature_hi>%s</temperature_hi>\n", temp_low_string.c_str());
                }
                fprintf(file_out,"      <temperature_low>%s</temperature_low>\n", temp_low_string.c_str());
                fprintf(file_out,"      <icon>%s</icon>\n", icon_night_string.c_str());
                fprintf(file_out,"      <description>%s</description>\n", desc_night_string.c_str());
                fprintf(file_out,"      <wind_direction>%s</wind_direction>\n", wind_direct_night_string.c_str());
                fprintf(file_out,"      <wind_speed>%1.f</wind_speed>\n", (double)(atoi( wind_speed_night_string.c_str())) * 1000/3600);
                fprintf(file_out,"      <humidity>%s</humidity>\n", humidity_night_string.c_str());
                fprintf(file_out,"      <ppcp>%s</ppcp>\n", ppcp_night_string.c_str());
                fprintf(file_out,"      <uv_index>%s</uv_index>\n", uv_index_night_string.c_str());
                fprintf(file_out,"    </period>\n");
            }
            fprintf(file_out,"    <period start=\"%li\"", utc_time);
            fprintf(file_out," end=\"%li\">\n", utc_time +  24*3600); 

            fprintf(file_out,"      <sunrise>%li</sunrise>\n", sunrise_time);
            fprintf(file_out,"      <sunset>%li</sunset>\n", sunset_time);
            fprintf(file_out,"    </period>\n");
        }
    }

    //std::cerr<<root["dal"]["HourlyForecast"][root["dal"]["HourlyForecast"].getMemberNames()[0]]["data"]["vt1hourlyForecast"]<<std::endl;
    if (root["dal"]["HourlyForecast"].isObject() && root["dal"]["HourlyForecast"][root["dal"]["HourlyForecast"].getMemberNames()[0]]["data"]["vt1hourlyForecast"].isArray()){
        val2 = root["dal"]["HourlyForecast"][root["dal"]["HourlyForecast"].getMemberNames()[0]]["data"]["vt1hourlyForecast"];
        //fprintf(stderr,"Size in val2 %i\n", val2.size());
        for (uint i = 0; i < val2.size(); i++){
            std::string utc_time_string;
            utc_time_string = val2[i].get("processTime","").asCString();

            if (utc_time_string != "" && utc_time_string.length()>22){
                tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
                tmp_tm.tm_isdst = time_tm2.tm_isdst;
                setlocale(LC_TIME, "POSIX");
                strptime((const char*)utc_time_string.c_str(), "%Y-%m-%dT%H:%M", &tmp_tm);
                time_t utc_time = 0;
                utc_time = mktime(&tmp_tm); 
                std::string desc_string = val2[i].get("phrase","").asCString();
                int icon_string = val2[i].get("icon", 0).asInt();
                int temp_string = val2[i].get("temperature", 0).asInt();
                int wind_speed = val2[i].get("windSpeed", 0).asInt();
                int humidity = val2[i].get("rh", 0).asInt();
                int ppcp = val2[i].get("precipPct", 0).asInt();
                int uv = val2[i].get("uvIndex", 0).asInt();
                int feel = val2[i].get("feelsLike", 0).asInt();
                std::string wind_direct_string = val2[i].get("windDirCompass","").asCString();

                if (desc_string != "" && icon_string >0){
                    fprintf(file_out,"    <period start=\"%li\"", utc_time);
                    fprintf(file_out," end=\"%li\" hour=\"true\">\n", utc_time + 60*60); 

                    fprintf(file_out,"      <temperature>%i</temperature>\n", temp_string);
                    fprintf(file_out,"      <icon>%i</icon>\n", icon_string);
                    fprintf(file_out,"      <description>%s</description>\n", desc_string.c_str());
                    fprintf(file_out,"      <wind_direction>%s</wind_direction>\n", wind_direct_string.c_str());
                    fprintf(file_out,"      <wind_speed>%1.f</wind_speed>\n", (double)( wind_speed * 1000/3600));
                    fprintf(file_out,"      <humidity>%i</humidity>\n", humidity);
                    fprintf(file_out,"      <ppcp>%i</ppcp>\n", ppcp);
                    fprintf(file_out,"      <uv_index>%i</uv_index>\n", uv);
                    fprintf(file_out,"      <flike>%i</flike>\n", feel);
                    fprintf(file_out,"    </period>\n");
                }
            } 
        }

    }
 
    fclose(file_out);
    return val.size();
}

/*******************************************************************************/
int
convert_station_weather_data(const char *station_id_with_path, const char *result_file,
	                     const char *station_detail_id_with_path){
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int     days_number = -1;
    gchar   buffer[1024],
            buffer2[1024],
            *delimiter = NULL;
    struct stat file_info;
    FILE        *file_out;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!station_id_with_path)
        return -1;
    *buffer = 0;
    /* check file accessability */
    if(!access(station_id_with_path, R_OK)){
        /* check that the file containe valid data */
        doc = htmlReadFile(station_id_with_path, "UTF-8", HTML_PARSE_NOWARNING);
        if(!doc)
            return -1;
        root_node = xmlDocGetRootElement(doc);
        if(root_node && root_node->type == XML_ELEMENT_NODE &&
                strstr((char*)root_node->name, "err")){
            xmlFreeDoc(doc);
            xmlCleanupParser();
            return -2;
        }
        else{
            /* prepare station id */
            *buffer = 0;
            *buffer2 = 0;
            snprintf(buffer2, sizeof(buffer) - 1, "%s", station_id_with_path);
            delimiter = strrchr(buffer2, '/');
            if(delimiter){
                delimiter++; /* delete '/' */
                snprintf(buffer, sizeof(buffer) - 1, "%s", delimiter);
                delimiter = strrchr(buffer, '.');
                if(!delimiter){
                    xmlFreeDoc(doc);
                    xmlCleanupParser();
                    return -1;
                }
                *delimiter = 0;
                days_number = parse_and_write_html_data(buffer, doc, result_file);

                xmlFreeDoc(doc);
                xmlCleanupParser();
		        if (days_number > 0){
                    file_out = fopen(result_file, "a");
    			    if (file_out){
                        fprintf(file_out,"</station>");
                        fclose(file_out);
                    }
		        }
            }
            return days_number;
        }
    }
    else
        return -1;/* file isn't accessability */
}

int
main(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "weathercom <input_file> <output_file> <input_detail_fail>\n");
        return -1;
    }
    if (argc == 3) 
    	result = convert_station_weather_data(argv[1], argv[2], "");
    if (argc == 4)
    	result = convert_station_weather_data(argv[1], argv[2], argv[3]);
    fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
