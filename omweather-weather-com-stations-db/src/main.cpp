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
parse_xml_detail_data(const gchar *station_id, xmlNode *root_node, GHashTable *data){
    xmlNode     *cur_node = NULL,
                *child_node = NULL,
                *child_node2 = NULL,
                *child_node3 = NULL;
    GHashTable  *detail = NULL,
                *hours_data = NULL;
    xmlChar     *temp_xml_string;
    int         count_hour = 0;
    gchar       buff[256];
    struct tm   tmp_tm = {0,0,0,0,0,0,0,0,0,0,0}; 
    GSList      *hour_weather = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    for(cur_node = root_node->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            if( !xmlStrcmp(cur_node->name, (const xmlChar *) "hbhf") ){
                hours_data = g_hash_table_new(g_str_hash, g_str_equal);
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    /* last update */
                    if( child_node->type == XML_ELEMENT_NODE  &&
                            ( !xmlStrcmp(child_node->name, (const xmlChar *)"lsup") ) ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(hours_data, (gpointer)"last_update", g_strdup((char*)temp_xml_string));
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    if( child_node->type == XML_ELEMENT_NODE  &&
                            (!xmlStrcmp(child_node->name, (const xmlChar *)"hour") )){
                        detail = g_hash_table_new(g_str_hash, g_str_equal);
                        /* Get an hour */
                        temp_xml_string = xmlGetProp(child_node, (const xmlChar *)"c");
                        /* prepare locale value for hour */
                        memset(buff, 0, sizeof(buff));
                        memcpy(buff, temp_xml_string, (strlen((char*)temp_xml_string)                                                                    > sizeof(buff)) ?
                                (sizeof(buff) - 1) :
                                (strlen((char*)temp_xml_string)));
                        strptime(buff, "%H", &tmp_tm);
                        memset(buff, 0, sizeof(buff));
                        strftime(buff, sizeof(buff) - 1, "%H", &tmp_tm);
                        g_hash_table_insert(detail, (gpointer)"hours", g_strdup(buff));
                        xmlFree(temp_xml_string);
                        for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                            if( child_node2->type == XML_ELEMENT_NODE){
                                /* hour temperature */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"tmp"))    {
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, (gpointer)"hour_temperature", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                                }
                                /* feels like for hour */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"flik")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, (gpointer)"hour_feels_like", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                                }
                                /* icon for hour */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"icon")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, (gpointer)"hour_icon", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                                }
                                /* title for hour */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, (gpointer)"hour_title", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                                }
                                /* wind data for hour */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"wind")){
                                    for(child_node3 = child_node2->children; child_node3 != NULL; child_node3 = child_node3->next){
                                        if( child_node3->type == XML_ELEMENT_NODE ){
                                            /* speed */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar *)"s")){
                                                temp_xml_string = xmlNodeGetContent(child_node3);
                                                g_hash_table_insert(detail, (gpointer)"hour_wind_speed", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                            }
                                            /* gust */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar *)"gust") ){
                                                temp_xml_string = xmlNodeGetContent(child_node3);
                                                g_hash_table_insert(detail, (gpointer)"hour_wind_gust", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                            }
                                            /* direction */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar *)"t")){
                                                temp_xml_string = xmlNodeGetContent(child_node3);
                                                g_hash_table_insert(detail, (gpointer)"hour_wind_direction", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                            }
                                        }
                                    }
                                }
                                /* hour humidity */
                                if(!xmlStrcmp(child_node2->name, (const xmlChar *)"hmid") ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, (gpointer)"hour_humidity", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                                }
                            }
                        }
                        if(detail){
                            hour_weather = g_slist_append(hour_weather, (gpointer)detail);
                            detail = NULL;
                            count_hour++;
                        }
                    }
                }
            }
        }
    }
    g_hash_table_insert(hours_data, (gpointer)"hours_data", (gpointer)hour_weather);
    g_hash_table_insert(data, (gpointer)"detail", (gpointer)hours_data);
    return count_hour;
}
/*******************************************************************************/
int
parse_and_write_detail_xml_data(const gchar *station_id, xmlNode *root_node, const gchar *result_file){
    xmlNode     *cur_node = NULL,
                *child_node = NULL,
                *child_node2 = NULL,
                *child_node3 = NULL;
    xmlChar     *temp_xml_string = NULL;
    int         count_hour = 0;
    struct tm   last_update_time = {0,0,0,0,0,0,0,0,0,0,0}; 
    FILE        *file_out;
    int         timezone_my = 0;
    int         hour = 0;
    time_t      t_start = 0, t_end = 0,
                current_day = 0;
    int         first_midnight = 1;
    int         first_item = 1;
    int         offset = 0;
    file_out = fopen(result_file, "a");
    if (!file_out)
        return -1;

    for(cur_node = root_node->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            if( !xmlStrcmp(cur_node->name, (const xmlChar *) "loc") ){
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    if( child_node->type == XML_ELEMENT_NODE ){
                        /* station time zone */
                        if( !xmlStrcmp(child_node->name, (const xmlChar *)"zone") ){
                            temp_xml_string = xmlNodeGetContent(child_node);
                            //    snprintf(timezone, sizeof(timezone) - 1, "%s", (char*)temp_xml_string);
                            timezone_my = atoi((char*)temp_xml_string);
                            xmlFree(temp_xml_string);
                            continue;
                        }
                    }
                }

            }
            if( !xmlStrcmp(cur_node->name, (const xmlChar *) "hbhf") ){
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    /* last update */
                    if( child_node->type == XML_ELEMENT_NODE  &&
                            ( !xmlStrcmp(child_node->name, (const xmlChar *)"lsup") ) ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        /* g_hash_table_insert(hours_data, "last_update", g_strdup((char*)temp_xml_string)); */
                        setlocale(LC_TIME, "POSIX");
                        strptime((char *)temp_xml_string, "%D", &last_update_time);
                        last_update_time.tm_sec = 0; last_update_time.tm_min = 0;
                        last_update_time.tm_hour = 0;
                        last_update_time.tm_isdst = 0;

                        current_day = mktime(&last_update_time);
                        setlocale(LC_TIME, "");
                        xmlFree(temp_xml_string);
                        continue;
                    }
                    if( child_node->type == XML_ELEMENT_NODE  &&
                            (!xmlStrcmp(child_node->name, (const xmlChar *)"hour") )){
                        if (xmlGetProp(child_node, (const xmlChar *)"c")){

                            /* Get an hour */
                            temp_xml_string = xmlGetProp(child_node, (const xmlChar *)"c");
                            hour = atoi((char*)temp_xml_string);
                            if (!first_midnight && hour == 0)
                                offset = 3600*24;
                            if (first_midnight && hour == 0)
                                first_midnight = 0;
                            if (!first_item && hour == 0) 
                                offset = 3600*24;
                            t_start = current_day + 3600*hour + offset; 
                            if (first_item == 1)
                                t_start = t_start - 3600;
                            //t_start = current_day + 3600*hour + offset - timezone*3600; 
                            xmlFree(temp_xml_string);
                            fprintf(file_out,"    <period start=\"%li\"", t_start);
                            /* 1 hour for weather.com */
                            if (first_item == 1)
                                fprintf(file_out," end=\"%li\" hour=\"true\">\n", t_start + 2*3600); 
                            else
                                fprintf(file_out," end=\"%li\" hour=\"true\">\n", t_start + 1*3600); 
                            first_item = 0;
                            for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                                if( child_node2->type == XML_ELEMENT_NODE){
                                    /* hour temperature */
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"tmp"))    {
                                        temp_xml_string = xmlNodeGetContent(child_node2);
                                        fprintf(file_out,"     <temperature>%s</temperature>\n",
                                                         (char*)temp_xml_string); 
                                        xmlFree(temp_xml_string);
                                        continue;
                                    }
                                    /* feels like for hour */
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"flik")){
                                        temp_xml_string = xmlNodeGetContent(child_node2);
                                        fprintf(file_out,"     <flike>%s</flike>\n",
                                                         (char*)temp_xml_string); 
                                        xmlFree(temp_xml_string);
                                        continue;
                                    }
                                    /* icon for hour */
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"icon")){
                                        temp_xml_string = xmlNodeGetContent(child_node2);
                                        fprintf(file_out,"     <icon>%s</icon>\n", 
                                                         (char*)temp_xml_string);
                                        xmlFree(temp_xml_string);
                                        continue;
                                    }
                                    /* title for hour */
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t")){
                                        temp_xml_string = xmlNodeGetContent(child_node2);
                                        fprintf(file_out,"     <description>%s</description>\n",
                                                        (char*)temp_xml_string);
                                        xmlFree(temp_xml_string);
                                        continue;
                                   }
                                   /* dewpoint for hour */
                                   if(!xmlStrcmp(child_node2->name, (const xmlChar *)"dewp")){
                                        temp_xml_string = xmlNodeGetContent(child_node2);
                                        fprintf(file_out,"     <dewpoint>%s</dewpoint>\n",
                                                        (char*)temp_xml_string);
                                        xmlFree(temp_xml_string);
                                        continue;
                                   }

                                   /* wind data for hour */
                                   if(!xmlStrcmp(child_node2->name, (const xmlChar *)"wind")){
                                        for(child_node3 = child_node2->children; child_node3 != NULL; child_node3 = child_node3->next){
                                            if( child_node3->type == XML_ELEMENT_NODE ){
                                                /* speed */
                                                if(!xmlStrcmp(child_node3->name, (const xmlChar *)"s")){
                                                    temp_xml_string = xmlNodeGetContent(child_node3);
                                                    fprintf(file_out,"     <wind_speed>%1.f</wind_speed>\n", 
                                                         (double)(atoi((char*)temp_xml_string)) * 1000/3600);
                                                    xmlFree(temp_xml_string);
                                                }
                                                /* gust */
                                                if(!xmlStrcmp(child_node3->name, (const xmlChar *)"gust") ){
                                                    temp_xml_string = xmlNodeGetContent(child_node3);
                                                    fprintf(file_out,"     <wind_gust>%s</wind_gust>\n", (char*)temp_xml_string);
                                                    xmlFree(temp_xml_string);
                                                }
                                                /* direction */
                                                if(!xmlStrcmp(child_node3->name, (const xmlChar *)"t")){
                                                    temp_xml_string = xmlNodeGetContent(child_node3);
                                                    fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", (char*)temp_xml_string);
                                                    xmlFree(temp_xml_string);
                                                }
                                            }
                                        }
                                    }
                                    /* hour humidity */
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"hmid") ){
                                        temp_xml_string = xmlNodeGetContent(child_node2);
                                        fprintf(file_out,"     <humidity>%s</humidity>\n",
                                                (char*)temp_xml_string);
                                        xmlFree(temp_xml_string);
                                        continue;
                                    }
                                }
                            }
                            fprintf(file_out,"    </period>\n");
                            count_hour++;
                        }
                    }
                }
            }
        }
    }
    fclose(file_out);
    return count_hour;

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

    #define MAX_BUFF_SIZE 512000
    char buffer[MAX_BUFF_SIZE];
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    Json::Value val;
    Json::Value node;
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


#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    //    file_out = fopen ("myfile.txt","w"); 
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
    if(xpathObj == NULL || xpathObj->nodesetval == NULL ||  xpathObj->nodesetval->nodeTab == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", "/html/body/script[contains(text(),'window.__data')]/text()");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
    }

    //fprintf(stderr, "length %s\n", xpathObj->nodesetval->nodeTab[0]->content  + 14);
    
    snprintf(buffer, xmlStrlen(xpathObj->nodesetval->nodeTab[0]->content) - 14 - 12, "%s", xpathObj->nodesetval->nodeTab[0]->content + 14 + 12);

    // normalization for json
    char buffer_additional[MAX_BUFF_SIZE];

    size_t d = strlen(buffer);
    size_t j = 0;
    for (size_t i=0; i<d; i++)
        if (i<d-1){
            if ((buffer[i]=='\\')&&(buffer[i+1]=='\\'&&(buffer[i+2]=='"'))){
                buffer_additional[j++] = '"';
                i++;
                i++;
                continue;
            }
            if ((buffer[i]=='\\')&&(buffer[i+1]=='"')){
                buffer_additional[j++] = '"';
                i++;
            }else{
                buffer_additional[j++] = buffer[i];
            }
        }else{
            buffer_additional[j++] = buffer[i];
        }
    buffer_additional[j] = '\0';
    //fprintf(stderr, "%s\n", buffer_additional);
    bool parsingSuccessful = reader.parse(buffer_additional, root, false);

    if (!parsingSuccessful)
        return -1;

    if (!root["dal"]["getSunV3CurrentObservationsUrlConfig"].empty() &&
        root["dal"]["getSunV3CurrentObservationsUrlConfig"][root["dal"]["getSunV3CurrentObservationsUrlConfig"].getMemberNames()[0]]["data"].isObject()){

        val = root["dal"]["getSunV3CurrentObservationsUrlConfig"][root["dal"]["getSunV3CurrentObservationsUrlConfig"].getMemberNames()[0]]["data"];
        std::string current_time_string;
        time_t utc_time = 0;
        current_time_string = val.get("observationTime","").asString();

            utc_time = val.get("validTimeUtc", "").asInt() ; 
            fprintf(file_out,"    <period start=\"%li\"", utc_time - 3600);
            fprintf(file_out," end=\"%li\"  current=\"true\">\n", utc_time + 3*3600); 

            fprintf(file_out,"      <temperature>%i</temperature>\n", val.get("temperature","").asInt());
            fprintf(file_out,"      <flike>%i</flike>\n", val.get("temperatureFeelsLike",-100).asInt());
            fprintf(file_out,"      <icon>%s</icon>\n", val.get("iconCode","").asString().c_str());
            fprintf(file_out,"      <description>%s</description>\n", val.get("wxPhraseLong","").asCString());
            fprintf(file_out,"      <wind_direction>%s</wind_direction>\n", val.get("windDirectionCardinal","").asCString());
            fprintf(file_out,"      <wind_speed>%1.f</wind_speed>\n", (double)(val.get("windSpeed","").asInt()) * 1000/3600); 
            fprintf(file_out,"      <humidity>%i</humidity>\n", val.get("relativeHumidity","").asInt());
            fprintf(file_out,"      <pressure>%1.f</pressure>\n", val.get("pressureAltimeter","").asFloat());
            fprintf(file_out,"      <pressure_direction>%s</pressure_direction>\n", val.get("pressureTendencyTrend","").asCString());
            fprintf(file_out,"      <dewpoint>%i</dewpoint>\n", val.get("temperatureDewPoint","").asInt());
            fprintf(file_out,"      <wind_gust>%1.f</wind_gust>\n", (double)(val.get("windGust","0").asInt()) * 1000/3600);
            fprintf(file_out,"      <uv_index>%i</uv_index>\n", val.get("uvIndex","").asInt() );
            fprintf(file_out,"      <visible>%i</visible>\n",(int)(val.get("visibility","").asDouble()*1000) );
            fprintf(file_out,"    </period>\n");
    }

    if (!root["dal"]["getSunV3DailyForecastWithHeadersUrlConfig"].empty() &&
        root["dal"]["getSunV3DailyForecastWithHeadersUrlConfig"][root["dal"]["getSunV3DailyForecastWithHeadersUrlConfig"].getMemberNames()[0]]["data"]["validTimeUtc"].isArray()){
       
        val = root["dal"]["getSunV3DailyForecastWithHeadersUrlConfig"][root["dal"]["getSunV3DailyForecastWithHeadersUrlConfig"].getMemberNames()[0]]["data"]["validTimeUtc"];
        node = root["dal"]["getSunV3DailyForecastWithHeadersUrlConfig"][root["dal"]["getSunV3DailyForecastWithHeadersUrlConfig"].getMemberNames()[0]]["data"];
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

            utc_time = node["validTimeUtc"][i].asInt();
            sunrise_time = node["sunriseTimeUtc"][i].asInt();
            sunset_time = node["sunsetTimeUtc"][i].asInt();
            temp_hi_string = node["temperatureMax"][i].asString();
            temp_low_string = node["temperatureMin"][i].asString();
            icon_day_string = node["daypart"][0]["iconCode"][i*2].asString();
            icon_night_string = node["daypart"][0]["iconCode"][i*2 + 1].asString();
            desc_day_string = node["daypart"][0]["wxPhraseLong"][i*2].asString();
            desc_night_string = node["daypart"][0]["wxPhraseLong"][i*2 + 1].asString();
            wind_direct_day_string = node["daypart"][0]["windDirectionCardinal"][i*2].asString();
            wind_direct_night_string = node["daypart"][0]["windDirectionCardinal"][i*2 + 1].asString();
            wind_speed_day_string = node["daypart"][0]["windSpeed"][i*2].asString();
            wind_speed_night_string = node["daypart"][0]["windSpeed"][i*2 + 1].asString();
            humidity_day_string = node["daypart"][0]["relativeHumidity"][i*2].asString();
            humidity_night_string = node["daypart"][0]["relativeHumidity"][i*2 + 1].asString();
            ppcp_day_string = node["daypart"][0]["precipChance"][i*2].asString();
            ppcp_night_string = node["daypart"][0]["precipChance"][i*2 + 1].asString();;
            uv_index_day_string = node["daypart"][0]["uvIndex"][i*2].asString();
            uv_index_night_string = node["daypart"][0]["uvIndex"][i*2 + 1].asString();
            
            setlocale(LC_TIME, "");
            /* get timezone */
            if (!check_timezone){
                char buffer_zone[4];

                utc_time_string = node["validTimeLocal"][i].asString();
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

    for (uint j = 0; j < 2; j++){
        if (!root["dal"]["getSunV3HourlyForecastUrlConfig"].empty() &&
            root["dal"]["getSunV3HourlyForecastUrlConfig"][root["dal"]["getSunV3HourlyForecastUrlConfig"].getMemberNames()[j]]["data"]["validTimeUtc"].isArray()){
            val = root["dal"]["getSunV3HourlyForecastUrlConfig"][root["dal"]["getSunV3HourlyForecastUrlConfig"].getMemberNames()[j]]["data"]["validTimeUtc"];
            node = root["dal"]["getSunV3HourlyForecastUrlConfig"][root["dal"]["getSunV3HourlyForecastUrlConfig"].getMemberNames()[j]]["data"];
            for (uint i = 0; i < val.size(); i++){
                time_t utc_time = node["validTimeUtc"][i].asInt();
                std::string desc_string  = node["wxPhraseLong"][i].asString();
                std::string icon_string =node["iconCode"][i].asString();
                int temp_string = node["temperature"][i].asInt();
                int wind_speed = node["windSpeed"][i].asInt();
                std::string wind_direct_string = node["windDirectionCardinal"][i].asCString();
                int humidity = node["relativeHumidity"][i].asInt();
                int ppcp = node["precipChance"][i].asInt();
                int uv = node["uvIndex"][i].asInt();
                int feel = node["temperatureFeelsLike"][i].asInt();
                if (desc_string != "" && icon_string != ""){
                    fprintf(file_out,"    <period start=\"%li\"", utc_time);
                    fprintf(file_out," end=\"%li\" hour=\"true\">\n", utc_time + 60*60); 

                    fprintf(file_out,"      <temperature>%i</temperature>\n", temp_string);
                    fprintf(file_out,"      <icon>%s</icon>\n", icon_string.c_str());
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
convert_station_weather_com_data(const char *station_id_with_path, const char *result_file,
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
main_weather_com(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "weathercom <input_file> <output_file> <input_detail_fail>\n");
        return -1;
    }
    if (argc == 3) 
    	result = convert_station_weather_com_data(argv[1], argv[2], "");
    if (argc == 4)
    	result = convert_station_weather_com_data(argv[1], argv[2], argv[3]);
    fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
