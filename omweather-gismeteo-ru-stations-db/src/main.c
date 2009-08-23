/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-gismeteo-ru-stations-db
 *
 * Copyright (C) 2009 Vlad Vasiliev
 * Copyright (C) 2009 Pavel Fialko
 * 	for the code
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "main.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
/*******************************************************************************/
#if 0
gint
get_station_weather_data(const gchar *station_id_with_path, GHashTable *data,
                                                    gboolean get_detail_data){
    gint    days_number = -1;
    gchar   buffer[1024];
    FILE *  source_file, destination_file;
    wint_t wc = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!station_id_with_path || !data)
        return -1;
/* check for new file, if it exist, than rename it */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s.new", station_id_with_path);
    if(!access(buffer, R_OK)){
  //      rename(buffer, station_id_with_path);
      source_file = fopen(buffer,"r");
      destination_file = fopen(station_id_with_path, "w");
      if (source_file && destination_file){
          while((wc = fgetwc(source_file))!=WEOF){
              fputwc(wc, destination_file);
          }
          fclose(source_file);
          fclose(destination_file);
      }else
        return -1;
    }
    /* check file accessability */
    if(!access(station_id_with_path, R_OK)){
        fprintf(stderr,"ddddddddddddddddddddddddddddddddddddddddddd\n");
//        file = fopen(station_id_with_path, "wb");
//        if(!file)
            return -1;          /* failure, can't open file to read */
//        find_date_string(file);
//        fclose(file);
        days_number = -1;
    }
    else
        return -1;/* file isn't accessability */
    return days_number;
}
#endif
/*******************************************************************************/
gint
get_station_weather_data(const gchar *station_id_with_path, GHashTable *data,
                                                    gboolean get_detail_data){
    htmlDocPtr  *doc = NULL;
    xmlNode *root_node = NULL;
    gint    days_number = -1;
    gchar   buffer[1024],
            *delimiter = NULL;
    FILE *source_file = NULL, *destination_file = NULL;
    wint_t wc = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!station_id_with_path || !data)
        return -1;
/* check for new file, if it exist, than rename it */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s.new", station_id_with_path);
    if(!access(buffer, R_OK)){
      source_file = fopen(buffer,"r");
      destination_file = fopen(station_id_with_path, "w");
      /* Correct problem with symbol '&' in html file */
      if (source_file && destination_file){
          while((wc = fgetwc(source_file))!=WEOF){
              if (wc == '\n'|| wc == '\r'){
                fputwc(' ',destination_file);
                continue;
              }
              if (wc == '&'){
                fputwc('&',destination_file);
                fputwc('a',destination_file);
                fputwc('m',destination_file);
                fputwc('p',destination_file);
                fputwc(';',destination_file);
              }else
                fputwc(wc, destination_file);
          }
          fclose(source_file);
          fclose(destination_file);
          unlink(buffer);
      }else
        return -1;
    }
    /* check file accessability */
    if(!access(station_id_with_path, R_OK)){
        fprintf(stderr,"sscsc %s\n",station_id_with_path);
        /* check that the file containe valid data */
        doc =  htmlReadFile(station_id_with_path, "UTF-8", 0);
        if(!doc)
            return -1;
        root_node = xmlDocGetRootElement(doc);
        if (!root_node)
            return -1;
//        htmlDocDump(stdout, doc);
        if(root_node->type == XML_ELEMENT_NODE &&
                strstr((char*)root_node->name, "err")){
            xmlFreeDoc(doc);
            xmlCleanupParser();
            return -2;
        }
        else{
            /* prepare station id */
            *buffer = 0;
            delimiter = strrchr(station_id_with_path, '/');
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
                fprintf(stderr,"llllllllllllllllllllllllllll\n");
                if(get_detail_data)
                    days_number = parse_xml_detail_data(buffer, root_node, data);
                else
                    days_number = parse_xml_data(buffer, root_node, data);
            }
            xmlFreeDoc(doc);
            xmlCleanupParser();
        }
    }
    else
        return -1;/* file isn't accessability */
    return days_number;
}
/*******************************************************************************/
struct tm
get_data_from_russia_data(gchar *temp_string){
    gchar buffer[256];
    gchar temp_buffer[256];
    struct tm   tmp_tm = {0};
    gchar *temp_point;
    memset(buffer, 0, sizeof(buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    /* Find first separate - Space */
    temp_point = strchr(temp_string,' ');
    snprintf(buffer, strlen(temp_point) - strlen(strchr(temp_point + 1,' ')),"%s", temp_point + 1);
    /* Add Mounth */
    temp_point = strchr(temp_point + 1,' ');
    snprintf(temp_buffer, strlen(temp_point) - strlen(strchr(temp_point + 1,' ')),"%s", temp_point + 1);
    if (!strcoll(temp_buffer, "Января"))
        strcat(buffer," Jun");
    if (!strcoll(temp_buffer, "Февраля"))
        strcat(buffer," Feb");
    if (!strcoll(temp_buffer, "Марта"))
        strcat(buffer," Mar");
    if (!strcoll(temp_buffer, "Апреля"))
        strcat(buffer," Apr");
    if (!strcoll(temp_buffer, "Мая"))
        strcat(buffer," May");
    if (!strcoll(temp_buffer, "Июня"))
        strcat(buffer," Jun");
    if (!strcoll(temp_buffer, "Июля"))
        strcat(buffer," Jul");
    if (!strcoll(temp_buffer, "Августа"))
        strcat(buffer," Aug");
    if (!strcoll(temp_buffer, "Сентября"))
        strcat(buffer," Sep");
    if (!strcoll(temp_buffer, "Октября"))
        strcat(buffer," Oct");
    if (!strcoll(temp_buffer, "Ноября"))
        strcat(buffer," Nov");
    if (!strcoll(temp_buffer, "Декабря"))
        strcat(buffer," Dec");
    /* Addd Year */
    temp_point = strchr(temp_point + 1,' ');
    snprintf(temp_buffer, 6, "%s", temp_point);
    strcat(buffer, temp_buffer);
    strptime(buffer, "%d %b %Y", &tmp_tm);
    return tmp_tm;
}
/*******************************************************************************/
void
fill_day (xmlNode *root_node, GHashTable *day){
#define buff_size 2048
    xmlNode     *cur_node = NULL;
    xmlNode     *child_node = NULL;
    xmlChar     *temp_xml_string = NULL;
    xmlChar     *temp_xml_char = NULL;
    gint i,j;
    gchar   buffer[buff_size];

    temp_xml_char = xmlCharStrdup("\r");
    for(cur_node = root_node; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            fprintf(stderr,"node %s\n",cur_node->name);
            if (cur_node->children){
                for(child_node = cur_node->children; child_node; child_node = child_node->next)
                {
                   if (!xmlStrcmp(child_node->name, (const xmlChar *)"img") ){
                        temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"src");
                        fprintf(stderr, "hhhhhhhhhhhhhhh %s\n", temp_xml_string); 
                   }
                }
            }
            temp_xml_string = xmlNodeGetContent(cur_node);
            memset(buffer, 0, sizeof(buffer));
            /* remove leading space */
            for (j = 0; (i<(xmlStrlen(temp_xml_string)) && j < buff_size); j++ ){
                if (temp_xml_string[j] != ' ')
                    break;
            }
            /* remove many spaces between words */
            for (i = j ; (i<(xmlStrlen(temp_xml_string)) && i < buff_size); i++ ){
                if (temp_xml_string[i] == ' ' && temp_xml_string[i+1] == ' ')
                    continue;
                else
                   sprintf(buffer,"%s%c",buffer, temp_xml_string[i]);
            }
            fprintf(stderr,"%s\n", buffer);
//            if (xmlStrcmp(xmlNodeGetContent(cur_node),"\r")){
//                fprintf(stderr," aa %s\n", xmlNodeGetContent(cur_node));
//            }
            temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"style");
            if (!xmlStrcmp(temp_xml_string, (const xmlChar*)"background: url(/media/pic/big/2/sep01.gif) repeat-x 0 0")){
                fprintf(stderr,"ddddddddddddd\n");
                break;
            }
        }
    }
}
/*******************************************************************************/
gint
parse_xml_data(const gchar *station_id, xmlNode *root_node, GHashTable *data){
    xmlNode     *cur_node = NULL,
                *child_node = NULL,
                *child_node2 = NULL,
                *child_node3 = NULL,
                *child_node4 = NULL;
    xmlChar     *temp_xml_string = NULL;
    xmlChar     *part_of_day = NULL;
    gint        store2day = 0,
                count_day = 0,
                count_of_div = 0,
                count_of_div_temp = 0,
                count_of_table = 0,
                count_of_day = 0,
                count_of_evening = 0,
                count_of_night = 0,
                count_of_morning = 0;
    gchar       id_station[10],
                buff[256],
                *delimiter = NULL;
    struct tm   tmp_tm = {0};
    time_t      first_day, day_in_list;
    GSList      *forecast = NULL;
    GHashTable  *location = NULL,
                *current = NULL,
                *day = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    for(cur_node = root_node->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
//            fprintf(stderr,"cur_node %s\n",cur_node->name);
            /* get weather station data */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "body" ) ){
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    if( child_node->type == XML_ELEMENT_NODE ){
//                        fprintf(stderr,"child_node->name %s\n",child_node->name);
                        if (!xmlStrcmp(child_node->name, (const xmlChar *)"div") ){
                            count_of_div ++;
                            count_of_div_temp ++;
                        }
                        if (!xmlStrcmp(child_node->name, (const xmlChar *)"table") )
                            count_of_table ++;
                        if (count_of_div == 2 && count_of_table == 0){
                            temp_xml_string = xmlNodeGetContent(child_node);
                            fprintf(stderr,"Date %s\n", temp_xml_string);
                            tmp_tm = get_data_from_russia_data(temp_xml_string);
                            first_day = mktime(&tmp_tm);
                        }
                        if (count_of_div > 2 && 
                            !xmlStrcmp(child_node->name, (const xmlChar *)"div")){
                            temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"style");
                            if (!xmlStrcmp(temp_xml_string, 
                                           (const xmlChar*)"float: left; padding: 0 0 0 4px;"))
                            {
                                fprintf(stderr,"Div content %s\n", temp_xml_string);
                                for(child_node3 = child_node->children; child_node3 != NULL; child_node3 = child_node3->next)
                                {
                                    fprintf(stderr, "jjjjjjjjj %s\n", child_node3->name);
                                    if (!xmlStrcmp(child_node3->name, (const xmlChar *)"img")){
                                       temp_xml_string = xmlGetProp(child_node3, (const xmlChar*)"src");
                                       delimiter = strrchr(temp_xml_string, '/');
                                       if(delimiter){
                                            delimiter++; /* delete '/' */
                                            if (delimiter && !strcmp(delimiter,"day.gif")){
                                                fprintf(stderr, "Test %s\n", delimiter);
                                                /* To do Check existing of day */ 
                                                day = g_hash_table_new(g_str_hash, g_str_equal);
                                                if (day){
                                                    forecast = g_slist_append(forecast,(gpointer)day);
                                                    count_day++;
                                                }
                                                fill_day(child_node, day);
                                                count_of_day ++; 
                                            }
                                       }
                                    }

                                }
                            }
                        }
                   }
                }
                g_hash_table_insert(data, "forecast", (gpointer)forecast);
            }
        }
    }
    return count_day;
}
/*******************************************************************************/
gint
parse_xml_detail_data(const gchar *station_id, xmlNode *root_node, GHashTable *data){
    xmlNode     *cur_node = NULL,
                *child_node = NULL,
                *child_node2 = NULL,
                *child_node3 = NULL;
    GHashTable  *detail = NULL,
                *hours_data = NULL;
    xmlChar     *temp_xml_string;
    gint        count_hour = 0;
    gchar       buff[256];
    struct tm   tmp_tm = {0};
    GSList      *hour_weather = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    return -1;
    for(cur_node = root_node->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            if( !xmlStrcmp(cur_node->name, (const xmlChar *) "hbhf") ){
                hours_data = g_hash_table_new(g_str_hash, g_str_equal);
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    /* last update */
                    if( child_node->type == XML_ELEMENT_NODE  &&
                            ( !xmlStrcmp(child_node->name, (const xmlChar *)"lsup") ) ){
                        temp_xml_string = xmlNodeGetContent(child_node);
                        g_hash_table_insert(hours_data, "last_update", g_strdup((char*)temp_xml_string));
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
                        g_hash_table_insert(detail, "hours", g_strdup(buff));
                        xmlFree(temp_xml_string);
                        for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                            if( child_node2->type == XML_ELEMENT_NODE){
                                /* hour temperature */
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"tmp"))    {
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, "hour_temperature", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                               }
                               /* feels like for hour */
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"flik")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, "hour_feels_like", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                               }
                               /* icon for hour */
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"icon")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, "hour_icon", g_strdup((char*)temp_xml_string));
                                    xmlFree(temp_xml_string);
                                    continue;
                               }
                               /* title for hour */
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"t")){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, "hour_title", g_strdup((char*)temp_xml_string));
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
                                                g_hash_table_insert(detail, "hour_wind_speed", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                            }
                                            /* gust */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar *)"gust") ){
                                                temp_xml_string = xmlNodeGetContent(child_node3);
                                                g_hash_table_insert(detail, "hour_wind_gust", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                            }
                                            /* direction */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar *)"t")){
                                                temp_xml_string = xmlNodeGetContent(child_node3);
                                                g_hash_table_insert(detail, "hour_wind_direction", g_strdup((char*)temp_xml_string));
                                                xmlFree(temp_xml_string);
                                            }
                                        }
                                    }
                               }
                               /* hour humidity */
                               if(!xmlStrcmp(child_node2->name, (const xmlChar *)"hmid") ){
                                    temp_xml_string = xmlNodeGetContent(child_node2);
                                    g_hash_table_insert(detail, "hour_humidity", g_strdup((char*)temp_xml_string));
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
    g_hash_table_insert(hours_data, "hours_data", (gpointer)hour_weather);
    g_hash_table_insert(data, "detail", (gpointer)hours_data);
    return count_hour;
}
/*******************************************************************************/
