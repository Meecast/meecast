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
GHashTable *hash_for_translate;
GHashTable *hash_for_icons;
/*******************************************************************************/
gint
get_station_weather_data(const gchar *station_id_with_path, GHashTable *data,
                                                    gboolean get_detail_data){
    htmlDocPtr doc = NULL;
    xmlNode    *root_node = NULL;
    gint       days_number = -1;
    gchar      buffer[1024],
               *delimiter = NULL;
    FILE       *source_file = NULL, *destination_file = NULL;
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
gchar*
choose_icon(gchar *image1, gchar *image2)
{
    gchar *result;
    gchar *source;

    if (!image1 || !image2)
        return g_strdup("49");
    source = g_strdup_printf("%s %s", image1, image2);
    result = hash_gismeteo_table_find(hash_for_icons, source, FALSE);
    g_free (source);
    if (strlen(result) == 2)
       return g_strdup(result);
    else{
       fprintf(stderr,"Unknown strings %s %s",image1, image2);
       return g_strdup("49");
    }
}
/*******************************************************************************/
void
fill_day (xmlNode *root_node, GHashTable *day){
#define buff_size 2048
    xmlNode     *cur_node = NULL;
    xmlNode     *child_node = NULL;
    xmlNode     *child_node2 = NULL;
    xmlNode     *child_node3 = NULL;
    xmlNode     *child_node4 = NULL;
    xmlChar     *temp_xml_string = NULL;
    xmlChar     *temp_xml_char = NULL;
    gint        i = 0 , j = 0;
    gint        count_of_string = 0;
    gchar   buffer[buff_size];
    gchar   temp_buffer[buff_size];
    gchar   *image1 = NULL;
    gchar   *image2 = NULL;
    gchar   temp_char;
    gint speed;

    if (root_node->next)
        cur_node = root_node->next;
    for(cur_node = cur_node; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            /* Check the end of day */
            temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"style");
            if (!xmlStrcmp(temp_xml_string, (const xmlChar*)"background: url(/media/pic/big/2/sep01.gif) repeat-x 0 0"))
                break;/* Exit from function */

            for(child_node = cur_node->children; child_node; child_node = child_node->next){
               for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                  if (!xmlStrcmp(child_node2->name, (const xmlChar *)"img") ){
                      temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"src");
                      temp_xml_string = (const xmlChar*)strrchr((char*)temp_xml_string, '/');
                      temp_xml_string ++;
                      image1 = g_strdup(temp_xml_string);
                  }
                  for(child_node3 = child_node2->children; child_node3; child_node3 = child_node3->next){
                     for(child_node4 = child_node3->children; child_node4; child_node4 = child_node4->next){
                          if (!xmlStrcmp(child_node4->name, (const xmlChar *)"img") ){
                            temp_xml_string = xmlGetProp(child_node4, (const xmlChar*)"src");
                            temp_xml_string = strrchr((char*)temp_xml_string, '/');
                            temp_xml_string ++;
                            image2 = g_strdup(temp_xml_string);
                        }

                     }
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
            /* remove last space */
            if (buffer[strlen(buffer)-1] == ' ')
                buffer[strlen(buffer)-1] = 0;
            if (strlen(buffer)>1){
                /* check description */
                if (count_of_string == 0){
                    g_hash_table_insert(day, "day_title", g_strdup(hash_gismeteo_table_find(hash_for_translate, buffer, FALSE)));
                }
                /* check temperature string */
                if (count_of_string == 1){
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    for (i = 0 ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == '-' || (buffer[i]>='0' && buffer[i]<='9'))
                            sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                    }
                    g_hash_table_insert(day, "day_hi_temperature", g_strdup(temp_buffer));
                }
                /* check pressure, humidity, wind */
                if (count_of_string == 2){
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    for (i = 0 ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ' ')
                            break;
                    }
                    i++;
                    for (i = i ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if ( (buffer[i]>='0' && buffer[i]<='9'))
                            sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                        else
                            break;
                    }
                    /*TODO need normalization from mm.rt.stolba to bar */
                    g_hash_table_insert(day, "day_pressure", g_strdup(temp_buffer));
                    /* find begin of humidity */
                    for (i = i ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ':'){
                            break;
                        }
                    }
                    i++;
                    i++;
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    /* fill humidity */
                    for (i = i ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if ( (buffer[i]>='0' && buffer[i]<='9'))
                            sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                        else
                            break;
                    }
                    g_hash_table_insert(day, "day_humidity", g_strdup(temp_buffer));
                    /* find begin of wind title*/
                    for (i = i ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ':')
                            break;
                    }
                    i++;
                    i++;
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    /* fill wind title */
                    for (i = i ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ' ')
                            break;
                        else{
                            if (buffer[i] == '-')
                                continue;
                           sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                        }
                    }
                    if (!strcoll(temp_buffer, "З"))
                        sprintf(temp_buffer,"%s","W");
                    if (!strcoll(temp_buffer, "Ю"))
                        sprintf(temp_buffer,"%s","S");
                    if (!strcoll(temp_buffer, "В"))
                        sprintf(temp_buffer,"%s","E");
                    if (!strcoll(temp_buffer, "С"))
                        sprintf(temp_buffer,"%s","N");
                    if (!strcoll(temp_buffer, "ЮЗ"))
                        sprintf(temp_buffer,"%s","SW");
                    if (!strcoll(temp_buffer, "ЮВ"))
                        sprintf(temp_buffer,"%s","SE");
                    if (!strcoll(temp_buffer, "СЗ"))
                        sprintf(temp_buffer,"%s","NW");
                    if (!strcoll(temp_buffer, "СВ"))
                        sprintf(temp_buffer,"%s","NE");
                    g_hash_table_insert(day, "day_wind_title",g_strdup(temp_buffer));

                    /* find begin of wind speed */
                    for (i = i ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ',')
                            break;
                    }
                    i++;
                    i++;
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    /* fill wind speed */
                    for (i = i ; (i<(xmlStrlen(buffer)) && i < buff_size); i++ ){
                        if ( (buffer[i]>='0' && buffer[i]<='9'))
                            sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                        else
                            break;
                    }
                    /* Normalize speed to km/h from m/s */
                    fprintf(stderr, "Wind  speed    %s\n", temp_buffer);
                    speed = atoi (temp_buffer);
                    speed = speed * 3600/1000;
                    sprintf(temp_buffer, "%i", speed);
                    g_hash_table_insert(day, "day_wind_speed",g_strdup(temp_buffer));
                }
                count_of_string ++;
            }
       }
    }
    if (image1 && image2) {
          g_hash_table_insert(day, "day_icon", choose_icon(image1, image2));
    }
    if (image1)
        g_free(image1);
    if (image2)
        g_free(image2);

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
    struct tm   *gmt;
    time_t      first_day, day_in_list;
    GSList      *forecast = NULL;
    GHashTable  *location = NULL,
                *current = NULL,
                *day = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* Fix me free memory */
fprintf(stderr, "sdddddddddddddd\n");
hash_for_translate = hash_description_gismeteo_table_create();
hash_for_icons = hash_icons_gismeteo_table_create();
/* calculate count of day */
    for(cur_node = root_node->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            /* get weather station data */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "body" ) ){
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    if( child_node->type == XML_ELEMENT_NODE ){
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
                            first_day = mktime(&tmp_tm) - 3600*24;
                            day_in_list = first_day;
                        }
                        if (count_of_div > 2 && 
                            !xmlStrcmp(child_node->name, (const xmlChar *)"div")){
                            temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"style");
                            if (!xmlStrcmp(temp_xml_string, 
                                           (const xmlChar*)"float: left; padding: 0 0 0 4px;"))
                            {
                                for(child_node3 = child_node->children; child_node3 != NULL; child_node3 = child_node3->next)
                                {
                                    if (!xmlStrcmp(child_node3->name, (const xmlChar *)"img")){
                                       temp_xml_string = xmlGetProp(child_node3, (const xmlChar*)"src");
                                       delimiter = strrchr(temp_xml_string, '/');
                                       if(delimiter){
                                            delimiter++; /* delete '/' */
                                            if (delimiter && !strcmp(delimiter,"day.gif"))
                                                count_day++;
                                       }
                                    }

                                }
                            }
                        }
                   }
                }
            }
        }
    }
    fprintf(stderr, "count_of_day %i\n", count_day);
    /* Filling data */
    count_of_div = 0;
    count_of_div_temp = 0;
    count_of_table = 0;
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
                            first_day = mktime(&tmp_tm) -3600*24;
                            day_in_list = first_day+(count_day*3600*24);
                        }
                        if (count_of_div > 2 && 
                            !xmlStrcmp(child_node->name, (const xmlChar *)"div")){
                            temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"style");
                            if (!xmlStrcmp(temp_xml_string, 
                                           (const xmlChar*)"float: left; padding: 0 0 0 4px;"))
                            {
                                for(child_node3 = child_node->children; child_node3 != NULL; child_node3 = child_node3->next){
                                    if (!xmlStrcmp(child_node3->name, (const xmlChar *)"img")){
                                       temp_xml_string = xmlGetProp(child_node3, (const xmlChar*)"src");
                                       delimiter = strrchr(temp_xml_string, '/');
                                       if(delimiter){
                                            delimiter++; /* delete '/' */
                                            if (delimiter && !strcmp(delimiter,"day.gif")){
                                                /* To do Check existing of day */ 
                                                day = g_hash_table_new(g_str_hash, g_str_equal);
                                                if (day){
                                                    memset(buff, 0, sizeof(buff));
                                                    gmt = gmtime(&day_in_list);
                                                    strftime(buff, sizeof(buff) - 1, "%a", gmt);
                                                    g_hash_table_insert(day, "day_name", g_strdup(buff));
                                                    memset(buff, 0, sizeof(buff));
                                                    strftime(buff, sizeof(buff) - 1, "%b %d", gmt);
                                                    g_hash_table_insert(day, "day_date", g_strdup(buff));
                                                    fprintf(stderr,"!!!!!!!!! day name %s\n",buff);
                                                    forecast = g_slist_prepend(forecast,(gpointer)day);
                                                    fill_day(child_node, day);
                                                    day_in_list = day_in_list - 3600*24;
                                                }
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
