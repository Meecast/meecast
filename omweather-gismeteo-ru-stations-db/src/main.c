/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-gismeteo-ru-stations-db
 *
 * Copyright (C) 2009-2011 Vlad Vasiliev
 * Copyright (C) 2009 Pavel Fialko
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
#include "main.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#define DAY 1
#define NIGHT 2
#define MORNING 3
#define EVENING 4
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
        if (!root_node){
            xmlFreeDoc(doc);
            return -1;
        }
        /*  htmlDocDump(stdout, doc); */
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
                if(get_detail_data)
                    days_number = parse_xml_detail_data(buffer, doc, data);
                else
                    days_number = parse_xml_data(buffer, doc, data);
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
    time_t t;
    struct tm   * tmp_tm1;
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
        strcat(buffer," Jan");
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
    t = mktime(&tmp_tm);
    tmp_tm1 = localtime(&t);
    tmp_tm = *tmp_tm1;

    return tmp_tm;
}
/*******************************************************************************/
gchar*
choose_icon(GHashTable *hash_for_icons, gchar *image1, gchar *image2)
{
    gchar *result = NULL;
    gchar *tmp_result = NULL;
    gchar *source = NULL;

    if (!image1 || !image2)
        return g_strdup("49");
    source = g_strdup_printf("%s %s", image1, image2);
    tmp_result = hash_gismeteo_table_find(hash_for_icons, source, FALSE);
    if (tmp_result && (strlen(tmp_result) == 2 || strlen(tmp_result) == 1)){
       result = g_strdup(tmp_result);
       g_free(source);
       return result;
    }else{
       fprintf(stderr,"Unknown strings %s %s\n", image1, image2);
       g_free(source);
       return g_strdup("49");
    }
}
/*******************************************************************************/
gchar*
choose_hour_weather_icon(GHashTable *hash_for_icons, gchar *image)
{
    gchar *result;
    gchar *source;
    gchar *tmp_result = NULL;

    if(!image)
        return g_strdup("49");
    source = g_strdup_printf("%s", image);
    tmp_result = hash_gismeteo_table_find(hash_for_icons, source, FALSE);
    if (tmp_result && (strlen(tmp_result) == 2 || strlen(tmp_result) == 1)){
       result = g_strdup(tmp_result);
       g_free(source);
       return result;
    }else{
       fprintf(stderr,"Unknown strings %s\n", image);
       g_free(source);
       return g_strdup("49");
    }
}
/*******************************************************************************/
void
fill_day (xmlNode *root_node, GHashTable *day, gint part_of_day, GHashTable *hash_for_translate, GHashTable *hash_for_icons){
#define buff_size 2048
    xmlNode     *cur_node = NULL;
    xmlNode     *child_node = NULL;
    xmlNode     *child_node2 = NULL;
    xmlNode     *child_node3 = NULL;
    xmlNode     *child_node4 = NULL;
    xmlChar     *temp_xml_string = NULL;
    gint        i = 0 , j = 0;
    gint        count_of_string = 0;
    gchar   buffer[buff_size];
    gchar   temp_buffer[buff_size];
    gchar   *image1 = NULL;
    gchar   *image2 = NULL;
    gchar   *temp_char;
    gint    speed;
    gint    pressure;

    if (root_node->next)
        cur_node = root_node->next;
    for(cur_node = cur_node; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            /* Check the end of day */
            temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"style");
            if (!xmlStrcmp(temp_xml_string, (const xmlChar*)"background: url(/media/pic/big/2/sep01.gif) repeat-x 0 0")){
                xmlFree(temp_xml_string);
                break;/* Exit from function */
            }
            if (temp_xml_string)
                xmlFree(temp_xml_string);
            for(child_node = cur_node->children; child_node; child_node = child_node->next){
               for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                  if (!xmlStrcmp(child_node2->name, (const xmlChar *)"img") ){
                      temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"src");
                      if (temp_xml_string){
                          temp_char = strrchr((char*)temp_xml_string, '/');
                          temp_char ++;
                          image1 = g_strdup(temp_char);
                          xmlFree(temp_xml_string);
                      }
                  }
                  for(child_node3 = child_node2->children; child_node3; child_node3 = child_node3->next){
                     for(child_node4 = child_node3->children; child_node4; child_node4 = child_node4->next){
                          if (!xmlStrcmp(child_node4->name, (const xmlChar *)"img") ){
                            temp_xml_string = xmlGetProp(child_node4, (const xmlChar*)"src");
                            if (temp_xml_string){
                                temp_char = strrchr((char*)temp_xml_string, '/');
                                temp_char ++;
                                image2 = g_strdup(temp_char);
                                xmlFree(temp_xml_string);
                            }
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
                   buffer[strlen(buffer)] = temp_xml_string[i];
            }
            /* remove last space */
            if ((strlen(buffer) > 0) && (buffer[strlen(buffer)-1] == ' '))
                buffer[strlen(buffer)-1] = 0;
            if (strlen(buffer)>1){
                /* check description */
                if (count_of_string == 0){
                    switch (part_of_day){
                        case DAY:    g_hash_table_insert(day, "day_title", g_strdup(hash_gismeteo_table_find(hash_for_translate, buffer, FALSE)));
                            break;
                        case NIGHT:    g_hash_table_insert(day, "night_title", g_strdup(hash_gismeteo_table_find(hash_for_translate, buffer, FALSE)));
                            break;
 
                    }
                }
                /* check temperature string */
                if (count_of_string == 1){
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    for (i = 0 ; (i<(strlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == '-' || (buffer[i]>='0' && buffer[i]<='9'))
                            sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                    }
                    switch (part_of_day){
                        case DAY: g_hash_table_insert(day, "day_hi_temperature", g_strdup(temp_buffer));
                              g_hash_table_insert(day, "day_temperature", g_strdup(temp_buffer));
                              break;
                        case NIGHT: g_hash_table_insert(day, "day_low_temperature", g_strdup(temp_buffer));
                              g_hash_table_insert(day, "night_temperature", g_strdup(temp_buffer));
                              break;
                    }
                }
                /* check pressure, humidity, wind */
                if (count_of_string == 2){
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    for (i = 0 ; (i<(strlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ' ')
                            break;
                    }
                    i++;
                    for (i = i ; (i<(strlen(buffer)) && i < buff_size); i++ ){
                        if ( (buffer[i]>='0' && buffer[i]<='9'))
                            sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                        else
                            break;
                    }
                    /*TODO need normalization from mm.rt.stolba to bar */
                    /* Normalizing from mm.rt.stolba to milibar bar */
                    if (strlen(temp_buffer) > 0){
                        pressure = atoi(temp_buffer);
                        pressure = pressure * 1.333224;
                        snprintf(temp_buffer, sizeof(temp_buffer)-1,"%i", pressure);
                    }
                    switch (part_of_day){
                        case DAY:
                              g_hash_table_insert(day, "day_pressure", g_strdup(temp_buffer));
                              break;
                        case NIGHT: 
                              g_hash_table_insert(day, "night_pressure", g_strdup(temp_buffer));
                              break;
                    }
                    /* find begin of humidity */
                    for (i = i ; (i<(strlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ':'){
                            break;
                        }
                    }
                    i++;
                    i++;
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    /* fill humidity */
                    for (i = i ; (i<(strlen(buffer)) && i < buff_size); i++ ){
                        if ( (buffer[i]>='0' && buffer[i]<='9'))
                            sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                        else
                            break;
                    }
                    switch (part_of_day){
                        case DAY:
                              g_hash_table_insert(day, "day_humidity", g_strdup(temp_buffer));
                              break;
                        case NIGHT: 
                              g_hash_table_insert(day, "night_humidity", g_strdup(temp_buffer));
                              break;
                    }
 
                    /* find begin of wind title*/
                    for (i = i ; (i<(strlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ':')
                            break;
                    }
                    i++;
                    i++;
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    /* fill wind title */
                    for (i = i ; (i<(strlen(buffer)) && i < buff_size); i++ ){
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
                    if (!strcoll(temp_buffer, "безветрие"))
                        sprintf(temp_buffer,"%s","CALM");
                    switch (part_of_day){
                        case DAY:
                              g_hash_table_insert(day, "day_wind_title",g_strdup(temp_buffer));
                              break;
                        case NIGHT: 
                              g_hash_table_insert(day, "night_wind_title",g_strdup(temp_buffer));
                              break;
                    }
 
                    /* find begin of wind speed */
                    for (i = i ; (i<(strlen(buffer)) && i < buff_size); i++ ){
                        if (buffer[i] == ',')
                            break;
                    }
                    i++;
                    i++;
                    memset(temp_buffer, 0, sizeof(temp_buffer));
                    /* fill wind speed */
                    for (i = i ; (i<(strlen(buffer)) && i < buff_size); i++ ){
                        if ( (buffer[i]>='0' && buffer[i]<='9'))
                            sprintf(temp_buffer,"%s%c",temp_buffer, buffer[i]);
                        else
                            break;
                    }
                    /* Normalize speed to km/h from m/s */
                    /* fprintf(stderr, "Wind  speed    %s\n", temp_buffer); */
                    speed = atoi (temp_buffer);
                    speed = speed * 3600/1000;
                    sprintf(temp_buffer, "%i", speed);
                    switch (part_of_day){
                        case DAY:
                              g_hash_table_insert(day, "day_wind_speed",g_strdup(temp_buffer));
                              break;
                        case NIGHT: 
                              g_hash_table_insert(day, "night_wind_speed",g_strdup(temp_buffer));
                              break;
                    }
                }
                count_of_string ++;
            }
            if (temp_xml_string)
                    xmlFree(temp_xml_string);
       }
    }
    if (image1 && image2) {
        switch (part_of_day){
            case DAY:
                    g_hash_table_insert(day, "day_icon", choose_icon(hash_for_icons, image1, image2));
                    break;
            case NIGHT:
                    g_hash_table_insert(day, "night_icon", choose_icon(hash_for_icons, image1, image2));
                    break;
        }
    }
    if (image1)
        g_free(image1);
    if (image2)
        g_free(image2);

}
/*******************************************************************************/
struct tm
get_date_for_current_weather(gchar *temp_string){
    gchar buffer[512];
    gchar temp_buffer[256];
    struct tm   tmp_tm = {0};
    time_t t;
    struct tm   * tmp_tm1;
    gchar *temp_point;
    gchar *temp_char;
    time_t rawtime;
    memset(buffer, 0, sizeof(buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    temp_point = strchr(temp_string,' ');
    if (!temp_point)
        return tmp_tm;
    snprintf(buffer, strlen(temp_string) - strlen(temp_point+1),"%s", temp_string);
    snprintf(temp_buffer, strlen(temp_point) - strlen(strchr(temp_point + 1,' ')),"%s", temp_point+1);
    if (!strcoll(temp_buffer, "Янв"))
        strcat(buffer," Jan");
    if (!strcoll(temp_buffer, "Фев"))
        strcat(buffer," Feb");
    if (!strcoll(temp_buffer, "Мар"))
        strcat(buffer," Mar");
    if (!strcoll(temp_buffer, "Март"))
        strcat(buffer," Mar");
    if (!strcoll(temp_buffer, "Апр"))
        strcat(buffer," Apr");
    if (!strcoll(temp_buffer, "Мая"))
        strcat(buffer," May");
    if (!strcoll(temp_buffer, "Июн"))
        strcat(buffer," Jun");
    if (!strcoll(temp_buffer, "Июл"))
        strcat(buffer," Jul");
    if (!strcoll(temp_buffer, "Авг"))
        strcat(buffer," Aug");
    if (!strcoll(temp_buffer, "Сен"))
        strcat(buffer," Sep");
    if (!strcoll(temp_buffer, "Окт"))
        strcat(buffer," Oct");
    if (!strcoll(temp_buffer, "Ноя"))
        strcat(buffer," Nov");
    if (!strcoll(temp_buffer, "Дек"))
        strcat(buffer," Dec");
    time ( &rawtime );
    temp_char = strrchr((char*)ctime (&rawtime), ' ');
    temp_char ++;
    sprintf(buffer, "%s %s", buffer, temp_char);
    /* fprintf(stderr, "\n%s\n", buffer); */
    temp_point = strchr(temp_point + 1,' ');
    snprintf(temp_buffer, strlen(temp_point)+1, "%s", temp_point+1);
    strcat(buffer, " ");
    strcat(buffer, temp_buffer);
    /* fprintf(stderr, "\n%s\n", buffer); */
    strptime(buffer, "%d %b %Y %T", &tmp_tm);
    t = mktime(&tmp_tm);
    tmp_tm1 = localtime(&t);
    tmp_tm = *tmp_tm1;
    /* fprintf(stderr, "\ntmp_tm min %d\n", (&tmp_tm)->tm_min); */
    return tmp_tm;
}
/*******************************************************************************/
struct tm
get_date_for_hour_weather(gchar *temp_string){
    gchar buffer[256];
    gchar temp_buffer[256];
    gchar buff[256];
    time_t t;
    struct tm   tmp_tm = {0};
    struct tm   * tmp_tm1;
    gchar *temp_point;
    memset(buffer, 0, sizeof(buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memset(buff, 0, sizeof(buff));
    temp_point = strchr(temp_string,'-');
    if (!temp_point)
        return tmp_tm;
    snprintf(buffer, strlen(temp_string) - strlen(temp_point+1),"%s", temp_string);
    snprintf(temp_buffer, strlen(temp_point) - strlen(strchr(temp_point + 1,'-')),"%s", temp_point+1);
    temp_point = strchr(temp_point+1,'-');
    snprintf(buff, strlen(temp_point) - strlen(strchr(temp_point + 1,' ')),"%s", temp_point+1);

    if (!strcoll(temp_buffer, "01"))
        strcat(buff," Jan");
    if (!strcoll(temp_buffer, "02"))
        strcat(buff," Feb");
    if (!strcoll(temp_buffer, "03")) 
        strcat(buff," Mar");
    if (!strcoll(temp_buffer, "04"))
        strcat(buff," Apr"); 
    if (!strcoll(temp_buffer, "05"))
        strcat(buff," May");
    if (!strcoll(temp_buffer, "06"))   
        strcat(buff," Jun");
    if (!strcoll(temp_buffer, "07")) 
        strcat(buff," Jul"); 
    if (!strcoll(temp_buffer, "08"))
        strcat(buff," Aug");
    if (!strcoll(temp_buffer, "09"))
        strcat(buff," Sep");  
    if (!strcoll(temp_buffer, "10"))  
        strcat(buff," Oct");  
    if (!strcoll(temp_buffer, "11"))  
        strcat(buff," Nov"); 
    if (!strcoll(temp_buffer, "12"))
        strcat(buff," Dec");
    strcat(buff, " ");
    strcat(buff, buffer);
    strcat(buff, " ");
    temp_point = strchr(temp_string,' ');
    strcat(buff, temp_point+1);
    strptime(buff, "%d %b %Y %T", &tmp_tm);
    t = mktime(&tmp_tm);
    tmp_tm1 = localtime(&t);
    tmp_tm = *tmp_tm1;
    /* fprintf(stderr, "\ntmp_wday hour %d\n", (&tmp_tm)->tm_wday); */
    return tmp_tm;
}
/*******************************************************************************/
void
fill_current_data(xmlNode *root_node, GHashTable *current_weather, GHashTable *data, GHashTable *hash_for_translate, GHashTable *hash_for_icons)
{
#define buff_size 2048
     xmlNode     *cur_node = NULL;
     xmlNode     *child_node = NULL;
     xmlNode     *child_node2 = NULL;
     xmlNode     *child_node3 = NULL;
     xmlNode     *child_node4 = NULL;
     xmlNode     *child_node5 = NULL;
     xmlNode     *child_node6 = NULL;
     xmlNode     *child_node7 = NULL;
     xmlNode     *child_node9 = NULL;
     xmlNode     *child_node10 = NULL;
     xmlNode     *child_node11 = NULL;
     xmlChar     *temp_xml_string = NULL;
     xmlChar     *temp_xml_string2 = NULL;
     xmlChar     *temp_xml_string3 = NULL;
     gint        i = 0; 
     gchar       buffer[buff_size];
     gchar       temp_buffer[buff_size];
     gchar       current_day[buff_size];
     gchar       *image = NULL;
     gchar       *temp_char;
     gint        speed;
     gint        pressure; 
     struct tm   tmp_tm = {0};
     GHashTable  *tmp_hash = NULL;
     GSList      *tmp_list = NULL;

 if (root_node->children)
    cur_node = root_node->children;

 if (cur_node) 
    cur_node = cur_node->next;
 else
    return;
 if (cur_node) 
    cur_node = cur_node->children;
 else
   return; 
 for(cur_node = cur_node; cur_node; cur_node = cur_node->next){
    if(!xmlStrcmp(cur_node->name, (const xmlChar *)"div")){
        temp_xml_string3 = xmlGetProp(cur_node, (const xmlChar*)"class");
        if(!xmlStrcmp(temp_xml_string3, (const xmlChar *)"M123")){
            for(child_node = cur_node->children; child_node; child_node = child_node->next){
                if(!xmlStrcmp(child_node->name, (const xmlChar *)"div")){
                    for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                        if(!xmlStrcmp(child_node2->name, (const xmlChar *)"div")){
                            temp_xml_string2 = xmlGetProp(child_node2, (const xmlChar*)"class");
                            if(!xmlStrcmp(temp_xml_string2, (const xmlChar *)"container astro inner_floated")){
                                for(child_node3 = child_node2->children; child_node3; child_node3 = child_node3->next){
                                    if (!xmlStrcmp(child_node3->name, (const xmlChar *)"div")) {
                                        temp_xml_string = xmlGetProp(child_node3, (const xmlChar*)"class");
                                            if(!xmlStrcmp(temp_xml_string, (const xmlChar *)"container_content")){
                                                child_node9 = child_node3->children;
                                                while(child_node9 && child_node9->name &&
                                                      xmlStrcmp(child_node9->name, (const xmlChar *)"ul")){
                                                    if (child_node9)
                                                        child_node9 = child_node9->next;
                                                    else
                                                        break;
                                                }

                                                if (child_node9)
                                                    child_node10 = child_node9->children;
                                                if(child_node10)
                                                    child_node10 = child_node10->children;
 
                                                if(child_node10)
                                                    child_node11 = child_node10->next;
                                                if(child_node11)
                                                    child_node11 = child_node11->children;
                                                if(child_node11)
                                                    child_node11 = child_node11->children;
                                                if(child_node11)
                                                    child_node11 = child_node11->next;
                                                if(child_node11)
                                                    child_node11 = child_node11->next;
                                                if(child_node11)
                                                    child_node11 = child_node11->children;

                                                xmlFree(temp_xml_string);
                                                temp_xml_string = xmlNodeGetContent(child_node11);
                                                strptime((char*)temp_xml_string,"%H:%M", &tmp_tm);
                                                setlocale(LC_TIME, "POSIX");
                                                strftime(buffer, sizeof(buffer) - 1, "%I:%M %p", &tmp_tm);
                                                setlocale(LC_TIME, "");
                                                
                                                if (child_node10)
                                                    child_node10 = child_node10->next;
                                                if (child_node10) 
                                                    child_node11 = child_node10->children;
                                                if (child_node11)
                                                        child_node11 = child_node11->next;
                                                if (child_node11)
                                                        child_node11 = child_node11->next;
                                                if (child_node11)
                                                     child_node11 = child_node11->children;
                                                if (child_node11)
                                                     child_node11 = child_node11->next;
                                                if (child_node11)
                                                     child_node11 = child_node11->next;

                                                xmlFree(temp_xml_string);
                                                temp_xml_string = xmlNodeGetContent(child_node11);
                                                if (temp_xml_string){
                                                    strptime((char*)temp_xml_string,"%H:%M", &tmp_tm);
                                                    setlocale(LC_TIME, "POSIX");
                                                    strftime(temp_buffer, sizeof(temp_buffer) - 1, "%I:%M %p", &tmp_tm);
                                                    setlocale(LC_TIME, "");

                                                    tmp_list = g_hash_table_lookup(data, "forecast");
                                                    while(tmp_list ){
                                                        tmp_hash = (GHashTable*)tmp_list->data;
                                                        if (g_hash_table_lookup(tmp_hash,"day_date")&&
                                                            strcmp(current_day,g_hash_table_lookup(tmp_hash,"day_date"))){
                                                            g_hash_table_insert(tmp_hash, "day_sunset_not_realistically", g_strdup(buffer));
                                                            g_hash_table_insert(tmp_hash, "day_sunrise_not_realistically", g_strdup(temp_buffer));
                                                        }else{
                                                            g_hash_table_insert(tmp_hash, "day_sunset", g_strdup(temp_buffer));
                                                            g_hash_table_insert(tmp_hash, "day_sunrise", g_strdup(buffer));
                                                            /*
                                                             fprintf(stderr, "Real Sunrise %s\n", buffer); 
                                                             fprintf(stderr, "Real Sunset %s\n", temp_buffer); 
                                                            */
                                                            
                                                        }

                                                        tmp_list = g_slist_next(tmp_list);
                                                    }
                                                }

                                                if (child_node9)
                                                    child_node9 = child_node9->children;
                                                if (child_node9)
                                                    child_node11 = child_node9->next;
                                                if (child_node11)
                                                    child_node11 = child_node11->next;
                                                if (child_node11)
                                                    child_node11 = child_node11->children;
                                                if (child_node11)
                                                    child_node11 = child_node11->next;

                                                xmlFree(temp_xml_string);
                                                temp_xml_string = NULL;
                                                temp_xml_string = xmlGetProp(child_node11,(const xmlChar*)"class"); 
                                                fprintf(stderr, "\n Фаза %s \n", temp_xml_string);
                                                if (temp_xml_string){
                                                    if (strstr(temp_xml_string,"vax0")){
                                                        if (!strncmp(temp_xml_string,"moon1",5)||!strncmp(temp_xml_string,"moon2",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("New"));
                                                        if (!strncmp(temp_xml_string,"moon2",5)||!strncmp(temp_xml_string,"moon3",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("Waxing Crescent"));
                                                        if (!strncmp(temp_xml_string,"moon4",5)||!strncmp(temp_xml_string,"moon5",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("First Quarter"));
                                                        if (!strncmp(temp_xml_string,"moon6",5)||!strncmp(temp_xml_string,"moon7",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("Waxing Gibbous"));
                                                        if (!strncmp(temp_xml_string,"moon8",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("Full"));
                                                    }
                                                    if (strstr(temp_xml_string,"vax1")){
                                                        if (!strncmp(temp_xml_string,"moon0",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("Waxing Crescent"));
                                                        if (!strncmp(temp_xml_string,"moon1",5)||!strncmp(temp_xml_string,"moon2",5)||
                                                            !strncmp(temp_xml_string,"moon3",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("Waning Crescent"));
                                                        if (!strncmp(temp_xml_string,"moon4",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("Last Quarter"));
                                                        if (!strncmp(temp_xml_string,"moon5",5)||!strncmp(temp_xml_string,"moon6",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("Waning Gibbous"));
                                                        if (!strncmp(temp_xml_string,"moon7",5) || !strncmp(temp_xml_string,"moon8",5))
                                                            g_hash_table_insert(current_weather, "moon_phase", 
                                                                                                g_strdup("Full"));
                                                    }

                                                    xmlFree(temp_xml_string);
                                                }
/*
                                                if(child_node11 && child_node11->name &&
                                                   !xmlStrcmp(child_node11->name, (const xmlChar *)"li")){
                                                   xmlFree(temp_xml_string);
                                                    temp_xml_string = xmlNodeGetContent(child_node11);
                                                    fprintf(stderr, "\n Фаза %s\n", temp_xml_string);
                                                    if (temp_xml_string){
                                                        g_hash_table_insert(current_weather, "moon_phase", g_strdup(hash_gismeteo_table_find(hash_for_translate, temp_xml_string, FALSE)));
                                                       xmlFree(temp_xml_string);
                                                    }
                                                }
*/
                                           }
                                       }
                                  }
                              }
                              if(!xmlStrcmp(temp_xml_string2, (const xmlChar *)"container current_weather4a inner_floated")){
                                                child_node4 = child_node2->children;
                                                child_node4 = child_node4->children;

                                                /* look up table */
                                                while(xmlStrcmp(child_node4->name, (const xmlChar *)"table")){
                                                    child_node4 = child_node4->next;
                                                }

                                                child_node7 = child_node4->next;
                                                /* fprintf(stderr, "\nchild_node7 %s \n",child_node7->name); */

                                                while(xmlStrcmp(child_node4->name, (const xmlChar *)"tr")){
                                                   child_node4 = child_node4->children;
                                                }

                                                xmlFree(temp_xml_string);
                                                temp_xml_string=NULL;
                                                child_node5 = child_node4->children;

                                                for(child_node5 = child_node5->children; child_node5;child_node5 = child_node5->next){
                                                    temp_xml_string = xmlGetProp(child_node5,(const xmlChar*)"class");
                                                    /* Icon */
                                                    if(!xmlStrcmp(temp_xml_string, (const xmlChar*)"png ico_bigWeather5 fll")){
                                                     xmlFree(temp_xml_string);
                                                     temp_xml_string = NULL;
                                                     temp_xml_string = xmlGetProp(child_node5, (const xmlChar*)"style");
                                                     if (temp_xml_string){
                                                      temp_char = strrchr((char*)temp_xml_string, '/');
                                                      temp_char ++;
                                                      image = g_strdup(temp_char);
                                                      i = 0;
                                                      memset(temp_buffer, 0, sizeof(temp_buffer));
                                                      while((image[i] != ')') && (i < strlen(image))){
                                                        sprintf(temp_buffer,"%s%c",temp_buffer, image[i]);
                                                        i++;
                                                      }
                                                      if (image)
                                                          g_free(image);
                                                      /* fprintf(stderr, "\nImage %s\n", temp_buffer); */
                                                      g_hash_table_insert(current_weather, "icon", choose_hour_weather_icon(hash_for_icons, temp_buffer));
                                                     }
                                                     if (temp_xml_string)
                                                        xmlFree(temp_xml_string);
                                                        temp_xml_string = NULL;
                                                    }
                                                    /* Temperature */
                                                    if(temp_xml_string && !xmlStrcmp(temp_xml_string, (const xmlChar*)"deg")){
                                                     xmlFree(temp_xml_string);
                                                     temp_xml_string = xmlNodeGetContent(child_node5);
                                                     g_hash_table_insert(current_weather, "day_hi_temperature", g_strdup((char*)temp_xml_string));
                                                     /* fprintf(stderr, "\n Deg %s\n", temp_xml_string); */
                                                     xmlFree(temp_xml_string);
                                                     temp_xml_string = NULL;
                                                    }
                                                    /* Summa */
                                                    if(temp_xml_string && !xmlStrcmp(temp_xml_string, (const xmlChar*)"summa")){
                                                        xmlFree(temp_xml_string);
                                                        temp_xml_string = xmlNodeGetContent(child_node5);
                                                        g_hash_table_insert(current_weather, "title", g_strdup(hash_gismeteo_table_find(hash_for_translate, temp_xml_string, FALSE)));
                                                        /* fprintf(stderr, "\n i!!!Summa %s\n", temp_xml_string); */
                                                        xmlFree(temp_xml_string);
                                                    }
                                                }
                                                if (child_node4)
                                                    child_node6 = child_node4->children;

                                                if (child_node6)
                                                    child_node6 = child_node6->next;
                                                if (child_node6)
                                                    child_node6 = child_node6->next;
                                                if (child_node6)
                                                    child_node6 = child_node6->children;
                                                if (child_node6)
                                                    child_node6 = child_node6->next;
                                                /* Pressure */
                                                if (child_node6 && child_node6->children)
                                                    for(child_node6 = child_node6->children;child_node6;child_node6 = child_node6->next){
                                                         xmlFree(temp_xml_string);
                                                         temp_xml_string = xmlGetProp(child_node6,(const xmlChar*)"class");
                                                         if(!xmlStrcmp(temp_xml_string, (const xmlChar*)"c1")){
                                                            xmlFree(temp_xml_string); 
                                                            temp_xml_string = NULL;
                                                            temp_xml_string = xmlNodeGetContent(child_node6);
                                                            if (temp_xml_string){
                                                                pressure = atoi(temp_xml_string);
                                                                pressure = pressure * 1.333224;
                                                                snprintf(temp_buffer, sizeof(temp_buffer)-1, "%i", pressure);
                                                             }
 
                                                            g_hash_table_insert(current_weather, "pressure", g_strdup((char*)temp_buffer));
                                                             /* fprintf(stderr, "\n Pressure %s\n", temp_xml_string); */
                                                         }
                                                         /* Wind */
                                                         if(!xmlStrcmp(temp_xml_string, (const xmlChar*)"c2")){
                                                            xmlFree(temp_xml_string);
                                                            temp_xml_string = xmlNodeGetContent(child_node6);
                                                            memset(temp_buffer, 0, sizeof(temp_buffer));
                                                            memset(buffer, 0, sizeof(buffer));
                                                            for(i=0;i<strlen((char*)temp_xml_string);i++){
                                                                if((temp_xml_string[i] >= '0') && (temp_xml_string[i] <= '9'))
                                                                    sprintf(temp_buffer,"%s%c",temp_buffer, temp_xml_string[i]);
                                                               else
                                                                    sprintf(buffer,"%s%c",buffer,temp_xml_string[i]);
                                                          }
                                                            /* Wind direction */
                                                            if (!strcoll(buffer, "З"))
                                                                 sprintf(buffer,"%s","W");
                                                            if (!strcoll(buffer, "Ю"))
                                                                 sprintf(buffer,"%s","S");
                                                            if (!strcoll(buffer, "В"))
                                                                 sprintf(buffer,"%s","E");
                                                            if (!strcoll(buffer, "С"))
                                                                 sprintf(buffer,"%s","N");
                                                            if (!strcoll(buffer, "ЮЗ"))
                                                                 sprintf(buffer,"%s","SW");
                                                            if (!strcoll(buffer, "ЮВ"))
                                                                 sprintf(buffer,"%s","SE");
                                                            if (!strcoll(buffer, "СЗ"))
                                                                 sprintf(buffer,"%s","NW");
                                                            if (!strcoll(buffer, "СВ"))
                                                                 sprintf(buffer,"%s","NE");
                                                            if (!strcoll(buffer, "безветрие"))
                                                                 sprintf(buffer,"%s","CALM");
                                                            if (!strcoll(buffer, "Ш"))
                                                                 sprintf(buffer,"%s","CALM");
 
                                                            g_hash_table_insert(current_weather, "wind_direction", g_strdup(buffer));
                                                            /*Wind speed. m/s in km/h*/
                                                            speed = atoi (temp_buffer);
                                                            speed = speed * 3600/1000;
                                                            sprintf(temp_buffer, "%i", speed);
                                                            g_hash_table_insert(current_weather, "wind_speed", g_strdup(temp_buffer));
                                                         }
                                                         if(!xmlStrcmp(temp_xml_string, (const xmlChar*)"c3")){
                                                             xmlFree(temp_xml_string);
                                                             temp_xml_string = xmlNodeGetContent(child_node6);
                                                             g_hash_table_insert(current_weather, "humidity", g_strdup((char*)temp_xml_string));
                                                             /* fprintf(stderr, "\n Humidity %s\n", temp_xml_string); */

                                                         }
                                                    }
     
                                                xmlFree(temp_xml_string);
                                                temp_xml_string = xmlNodeGetContent(child_node7->children);
                                                tmp_tm = get_date_for_current_weather((char*)temp_xml_string);
                                                memset(buffer, 0, sizeof(buffer));
                                                setlocale(LC_TIME, "POSIX");
                                                strftime(buffer, sizeof(buffer) - 1, "%D %I:%M %p", &tmp_tm);
                                                /* fprintf(stderr, "\n Time current %s\n", buffer); */
                                                /* Current day for sunrise and sunset settings */
                                                strftime(current_day, sizeof(current_day) - 1, "%b %d", &tmp_tm);
                                                /* fprintf(stderr, "\n Current day %s\n", current_day); */
                                                setlocale(LC_TIME, "");
                                                g_hash_table_insert(current_weather, "last_update", g_strdup(buffer));
                                                xmlFree(temp_xml_string);
                              }
                              {
                              }
                              xmlFree(temp_xml_string2);
                          }
                        }
                    }
                }
               xmlFree(temp_xml_string3);
            }else
               xmlFree(temp_xml_string3);
        }
    }
}
/*******************************************************************************/
gint
parse_xml_data(const gchar *station_id, htmlDocPtr doc, GHashTable *data){
    gchar       buff[256],
                buffer[2048];
    struct tm   tmp_tm = {0};
    GSList      *forecast = NULL;
    GSList      *tmp = NULL;
    GHashTable  *day = NULL;
    gboolean    flag;
    gboolean    night_flag;
    gint        size;
    gint        i;
    GHashTable *hash_for_translate;
    GHashTable *hash_for_icons;
    xmlXPathContextPtr xpathCtx; 
    xmlXPathObjectPtr xpathObj = NULL; 
    xmlXPathObjectPtr xpathObj2 = NULL; 
    xmlXPathObjectPtr xpathObj3 = NULL; 
    xmlXPathObjectPtr xpathObj4 = NULL; 
    xmlXPathObjectPtr xpathObj5 = NULL; 
    xmlXPathObjectPtr xpathObj6 = NULL; 
    xmlXPathObjectPtr xpathObj7 = NULL; 
    xmlXPathObjectPtr xpathObj8 = NULL; 
    xmlXPathObjectPtr xpathObj9 = NULL; 
    xmlNodeSetPtr nodes;
    gchar       *temp_char;
    gint        pressure; 
    gint        speed;

   hash_for_translate = hash_description_gismeteo_table_create();
   hash_for_icons = hash_icons_gismeteo_table_create();
   /* Create xpath evaluation context */
   xpathCtx = xmlXPathNewContext(doc);
   if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
         return(-1);
   }
   /* Register namespaces from list (if any) */
   xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"html",
                                (const xmlChar*)"http://www.w3.org/1999/xhtml");

   /* Evaluate xpath expression */
   // xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/@title", xpathCtx);
   xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/@title", xpathCtx);
  

   if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", "//*[@class='c0 day']/div/text()");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
   }

  nodes   = xpathObj->nodesetval;
  size = (nodes) ? nodes->nodeNr : 0;
  /* fprintf(stderr, "SIZE!!!!!!!!!!!!!!: %i\n", size); */
 // xpathObj2 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0' and @title]/text()", xpathCtx);
  xpathObj2 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/th[@title]/text()", xpathCtx);
 // xpathObj3 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c3']/text()", xpathCtx);
  xpathObj3 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/following-sibling::*[@class='temp']/text()", xpathCtx);
//  xpathObj4 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c1']/div/img/@src", xpathCtx);
  xpathObj4 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/following-sibling::*[@class='clicon']/img/@src", xpathCtx);
 // xpathObj5 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c2']/span/text()", xpathCtx);
  xpathObj5 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/following-sibling::*[@class='cltext']/text()", xpathCtx);
 // xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c4']/text()", xpathCtx);
  xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/following-sibling::*/text()", xpathCtx);
//  xpathObj7 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c5']/div/text()", xpathCtx);
  xpathObj7 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/following-sibling::*/dl[@class='wind']/dd/text()", xpathCtx);
//  xpathObj8 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c5']/div/img/@title", xpathCtx);
  xpathObj8 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/following-sibling::*/dl/dt/text()", xpathCtx);
//  xpathObj9 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c6']/text()", xpathCtx);
  xpathObj9 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/following-sibling::*/text()", xpathCtx);
  /* fprintf(stderr, "Result (%d nodes):\n", size); */
  for(i = 0; i < size; ++i) {
      day = NULL;
      /* Take UTC time: */
      if (!nodes->nodeTab[i]->children->content)
          continue;
      temp_char = strstr(nodes->nodeTab[i]->children->content, "UTC: ");
      if (temp_char && strlen(temp_char) >6)
              temp_char = temp_char +5;

      tmp_tm = get_date_for_hour_weather(temp_char);
      memset(buff, 0, sizeof(buff));
      setlocale(LC_TIME, "POSIX");
      strftime(buff, sizeof(buff) - 1, "%b %d", &tmp_tm);
      setlocale(LC_TIME, "");

      tmp = forecast;
      flag = FALSE;
      night_flag = FALSE;
      /* Check Day and Night */
      if (xpathObj2 && !xmlXPathNodeSetIsEmpty(xpathObj2->nodesetval) && xpathObj2->nodesetval->nodeTab[i]->content && (
           !strcmp(xpathObj2->nodesetval->nodeTab[i]->content, "Ночь")||
           !strcmp(xpathObj2->nodesetval->nodeTab[i]->content, "День"))
          ){
            if (!strcmp(xpathObj2->nodesetval->nodeTab[i]->content, "Ночь"))
                night_flag = TRUE;
            /* fprintf(stderr,"Day : %s\n", xpathObj2->nodesetval->nodeTab[i]->content); */
      }else{
          continue;
      }
      /* Look up this day in hash */
      while(tmp){
          day = (GHashTable*)tmp->data;
          if (g_hash_table_lookup(day, "day_date") &&
              !strcmp(g_hash_table_lookup(day,"day_date"), buff)){
                  flag = TRUE;
                  break;
          }
          tmp = g_slist_next(tmp);
      }
      /* added new day to hash */
      if (!flag){
          day = g_hash_table_new(g_str_hash, g_str_equal);
          g_hash_table_insert(day, "day_date", g_strdup(buff));
          memset(buff, 0, sizeof(buff));
          strftime(buff, sizeof(buff) - 1, "%a", &tmp_tm);
          g_hash_table_insert(day, "day_name", g_strdup(buff));
          forecast = g_slist_append(forecast,(gpointer)day);
      }
     
      if (day){
         /* added temperature */
         if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) &&
             xpathObj3->nodesetval->nodeTab[i] && xpathObj3->nodesetval->nodeTab[i]->content){
            /* fprintf (stderr, "temperature %s\n", xpathObj3->nodesetval->nodeTab[i]->content); */
            if (night_flag){
                g_hash_table_insert(day, "day_low_temperature", g_strdup(xpathObj3->nodesetval->nodeTab[i]->content));
                g_hash_table_insert(day, "night_temperature", g_strdup(xpathObj3->nodesetval->nodeTab[i]->content));
            }else{
                g_hash_table_insert(day, "day_hi_temperature", g_strdup(xpathObj3->nodesetval->nodeTab[i]->content));
                g_hash_table_insert(day, "day_temperature", g_strdup(xpathObj3->nodesetval->nodeTab[i]->content));
            }
         }
         /* added icon */
         if (xpathObj4 && !xmlXPathNodeSetIsEmpty(xpathObj4->nodesetval) &&
             xpathObj4->nodesetval->nodeTab[i] && xpathObj4->nodesetval->nodeTab[i]->children->content){
            /* fprintf (stderr, "sdfff %s\n", xpathObj4->nodesetval->nodeTab[i]->children->content); */
            temp_char = strrchr((char*)xpathObj4->nodesetval->nodeTab[i]->children->content, '/');
            temp_char ++;
            if (night_flag)
                g_hash_table_insert(day, "night_icon", choose_hour_weather_icon(hash_for_icons, temp_char));
            else
                g_hash_table_insert(day, "day_icon", choose_hour_weather_icon(hash_for_icons, temp_char));
         }
         /* added text */
         if (xpathObj5 && !xmlXPathNodeSetIsEmpty(xpathObj5->nodesetval) &&
             xpathObj5->nodesetval->nodeTab[i] && xpathObj5->nodesetval->nodeTab[i]->content){
            /* fprintf (stderr, "sdfff %s\n", xpathObj5->nodesetval->nodeTab[i]->content); */
            if (night_flag)
                g_hash_table_insert(day, "night_title", 
                                 g_strdup(hash_gismeteo_table_find(hash_for_translate, xpathObj5->nodesetval->nodeTab[i]->content, FALSE)));
            else
            g_hash_table_insert(day, "day_title", 
                                 g_strdup(hash_gismeteo_table_find(hash_for_translate, xpathObj5->nodesetval->nodeTab[i]->content, FALSE)));
         }
         /* added pressure */
         if (xpathObj6 && !xmlXPathNodeSetIsEmpty(xpathObj6->nodesetval) &&
             xpathObj6->nodesetval->nodeNr >= (i*5+2) &&
             xpathObj6->nodesetval->nodeTab[i*5+2] && xpathObj6->nodesetval->nodeTab[i*5+2]->content){
            pressure = atoi((char*)xpathObj6->nodesetval->nodeTab[i*5+2]->content);
            pressure = pressure * 1.333224;
            snprintf(buffer, sizeof(buffer)-1,"%i", pressure);
            /* fprintf (stderr, "pressure %s\n", xpathObj6->nodesetval->nodeTab[i*5+2]->content); */ 
            if (night_flag)
               g_hash_table_insert(day, "night_pressure", g_strdup(buffer));
            else
                g_hash_table_insert(day, "day_pressure", g_strdup(buffer));
         }
         /* added wind speed */
         if (xpathObj7 && !xmlXPathNodeSetIsEmpty(xpathObj7->nodesetval) &&
             xpathObj7->nodesetval->nodeTab[i] && xpathObj7->nodesetval->nodeTab[i]->content){
            /* Normalize speed to km/h from m/s */
            /* fprintf(stderr, "Wind  speed    \n"); */ 
            speed = atoi (xpathObj7->nodesetval->nodeTab[i]->content);
            speed = speed * 3600/1000;
            sprintf(buffer, "%i", speed);
            if (night_flag)
                g_hash_table_insert(day, "night_wind_speed", g_strdup(buffer));
            else
                g_hash_table_insert(day, "day_wind_speed", g_strdup(buffer));
         }
         /* added wind direction */
         if (xpathObj8 && !xmlXPathNodeSetIsEmpty(xpathObj8->nodesetval) &&
             xpathObj8->nodesetval->nodeTab[i] && xpathObj8->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "Wind direction: %s\n", xpathObj8->nodesetval->nodeTab[i]->content); */ 
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj8->nodesetval->nodeTab[i]->content);
             /* Wind direction */
             if (!strcoll(buffer, "З"))
                  sprintf(buffer,"%s","W");
             if (!strcoll(buffer, "Ю"))
                  sprintf(buffer,"%s","S");
             if (!strcoll(buffer, "В"))
                  sprintf(buffer,"%s","E");
             if (!strcoll(buffer, "С"))
                  sprintf(buffer,"%s","N");
             if (!strcoll(buffer, "ЮЗ"))
                  sprintf(buffer,"%s","SW");
             if (!strcoll(buffer, "ЮВ"))
                  sprintf(buffer,"%s","SE");
             if (!strcoll(buffer, "СЗ"))
                  sprintf(buffer,"%s","NW");
             if (!strcoll(buffer, "СВ"))
                  sprintf(buffer,"%s","NE");
             if (!strcoll(buffer, "безветрие"))
                  sprintf(buffer,"%s","CALM");
             if (!strcoll(buffer, "Ш"))
                  sprintf(buffer,"%s","CALM");

             if (night_flag)
                g_hash_table_insert(day, "night_wind_title", g_strdup(buffer));
             else
                g_hash_table_insert(day, "day_wind_title", g_strdup(buffer));
         }
         /* added humidity */
         if (xpathObj9 && !xmlXPathNodeSetIsEmpty(xpathObj9->nodesetval) &&
             xpathObj9->nodesetval->nodeNr >= (i*5+3) &&
             xpathObj9->nodesetval->nodeTab[i*5+3] && xpathObj9->nodesetval->nodeTab[i*5+3]->content){
            /* fprintf (stderr, "temperature %s\n", xpathObj9->nodesetval->nodeTab[i*5+3]->content); */
            if (night_flag){
                g_hash_table_insert(day, "night_humidity", g_strdup(xpathObj9->nodesetval->nodeTab[i*5+3]->content));
            }else{
                g_hash_table_insert(day, "day_humidity", g_strdup(xpathObj9->nodesetval->nodeTab[i*5+3]->content));
            }
         }
      }
  }	
  /* Cleanup */
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);
  if (xpathObj2)
    xmlXPathFreeObject(xpathObj2);
  if (xpathObj3)
    xmlXPathFreeObject(xpathObj3);
  if (xpathObj4)
    xmlXPathFreeObject(xpathObj4);
  if (xpathObj5)
    xmlXPathFreeObject(xpathObj5);
  if (xpathObj6)
    xmlXPathFreeObject(xpathObj6);
  if (xpathObj7)
    xmlXPathFreeObject(xpathObj7);
  if (xpathObj8)
    xmlXPathFreeObject(xpathObj8);
  if (xpathObj9)
    xmlXPathFreeObject(xpathObj9);
  if (xpathCtx)
    xmlXPathFreeContext(xpathCtx); 

  g_hash_table_insert(data, "forecast", (gpointer)forecast);
  g_hash_table_destroy(hash_for_translate);
  g_hash_table_destroy(hash_for_icons);

  return size/4;
}
/*******************************************************************************/
void
fill_detail_data(htmlDocPtr doc, GHashTable *location, GHashTable *hash_for_icons, GHashTable *hash_for_translate, GHashTable *data){
    #define buff_size 2048
    xmlNode     *cur_node = NULL;
    xmlNode     *child_node = NULL;
    xmlNode     *child_node2 = NULL;
    xmlNode     *child_node4 = NULL;
    xmlNode     *child_node5 = NULL;
    xmlNode     *child_node7 = NULL;
    xmlNode     *child_node8 = NULL;
    xmlNode     *child_node9 = NULL;
    xmlNode     *child_node10 = NULL;
    xmlNode     *child_node11 = NULL;
    xmlNode     *child_node12 = NULL;
    xmlChar     *temp_xml_string = NULL;
    xmlChar     *temp_xml_string2 = NULL;
    xmlChar     *temp_xml_string_2 = NULL;
    xmlChar     *temp_xml_string3 = NULL;
    gint        i = 0, j = 0, k = 0;
    gchar       buffer[buff_size];
    gchar       buff[buff_size];
    gchar       temp_buffer[buff_size];
    gchar       tmp[buff_size];
    gchar       *temp_char;
    gint        speed;
    gint        pressure; 
    gint        count_of_hours = 0;
    gint        location_timezone = 0;
    GHashTable  *detail = NULL; 
    GSList      *hour_weather = NULL;
    GHashTable  *hours_data = NULL;
    struct tm   tmp_tm = {0};
    struct tm   tmp_tm_utc = {0};
    struct tm   tmp_tm_loc = {0};
    double      time_diff = 0;
    time_t      loc_time;
    time_t      utc_time;
    gboolean break_flag = FALSE; 
    gboolean timezone_flag = FALSE;
    gboolean feels_like_flag = FALSE;
    gchar       *image = NULL;

    xmlXPathContextPtr xpathCtx; 
    xmlXPathObjectPtr xpathObj = NULL; 
    xmlXPathObjectPtr xpathObj2 = NULL; 
    xmlXPathObjectPtr xpathObj3 = NULL; 
    xmlXPathObjectPtr xpathObj4 = NULL; 
    xmlXPathObjectPtr xpathObj5 = NULL; 
    xmlXPathObjectPtr xpathObj6 = NULL; 
    xmlXPathObjectPtr xpathObj7 = NULL; 
    xmlXPathObjectPtr xpathObj8 = NULL; 
    xmlXPathObjectPtr xpathObj9 = NULL; 
    xmlNodeSetPtr nodes;
    GHashTable *current_weather = NULL;
    gint size;

   /* Create xpath evaluation context */
   xpathCtx = xmlXPathNewContext(doc);
   if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
         return(-1);
   }
   /* Register namespaces from list (if any) */
   xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"html",
                                (const xmlChar*)"http://www.w3.org/1999/xhtml");

  /* Evaluate xpath expression */
 // xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/@title", xpathCtx);
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div/span[@class='icon date']/text()", xpathCtx);

  hours_data = g_hash_table_new(g_str_hash, g_str_equal);
  current_weather = g_hash_table_lookup(data, "current");
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      strptime(xpathObj->nodesetval->nodeTab[0]->content, "%d.%m.%Y %H:%M:%S", &tmp_tm);
      memset(buff, 0, sizeof(buff));
      setlocale(LC_TIME, "POSIX");
      strftime(buff, sizeof(buff) - 1, "%D %I:%M %p", &tmp_tm);
      setlocale(LC_TIME, "");
      g_hash_table_insert(hours_data, "last_update", g_strdup(buff));
      g_hash_table_insert(current_weather, "last_update", g_strdup(buff));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='temp']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      g_hash_table_insert(current_weather, "day_hi_temperature", g_strdup(xpathObj->nodesetval->nodeTab[0]->content));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/dl/dt[@class='png']/@style", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->children->content){
        if (xpathObj->nodesetval->nodeTab[0]->children->content){
           temp_char = strrchr(xpathObj->nodesetval->nodeTab[0]->children->content, '/');
           temp_char ++;
           image = g_strdup(temp_char);
           i = 0;
           memset(temp_buffer, 0, sizeof(temp_buffer));
           while((image[i] != ')') && (i < strlen(image))){
             sprintf(temp_buffer,"%s%c",temp_buffer, image[i]);
             i++;
            }
        }
        if (image)
            g_free(image);
        /* fprintf(stderr, "\nImage %s\n", temp_buffer); */
        g_hash_table_insert(current_weather, "icon", choose_hour_weather_icon(hash_for_icons, temp_buffer));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/dl/dd/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      g_hash_table_insert(current_weather, "title", g_strdup(hash_gismeteo_table_find(hash_for_translate, xpathObj->nodesetval->nodeTab[0]->content, FALSE)));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='wicon barp']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      if (strlen(xpathObj->nodesetval->nodeTab[0]->content) > 0){
                        pressure = atoi(xpathObj->nodesetval->nodeTab[0]->content);
                        pressure = pressure * 1.333224;
                        snprintf(temp_buffer, sizeof(temp_buffer)-1,"%i", pressure);
      }
      g_hash_table_insert(current_weather, "pressure", g_strdup(temp_buffer));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='wicon hum']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      g_hash_table_insert(current_weather, "humidity", g_strdup(xpathObj->nodesetval->nodeTab[0]->content));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='wicon wind']/dl/dt/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
     snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj->nodesetval->nodeTab[0]->content);
     /* Wind direction */
     if (!strcoll(buffer, "З"))
          sprintf(buffer,"%s","W");
     if (!strcoll(buffer, "Ю"))
          sprintf(buffer,"%s","S");
     if (!strcoll(buffer, "В"))
          sprintf(buffer,"%s","E");
     if (!strcoll(buffer, "С"))
          sprintf(buffer,"%s","N");
     if (!strcoll(buffer, "ЮЗ"))
          sprintf(buffer,"%s","SW");
     if (!strcoll(buffer, "ЮВ"))
          sprintf(buffer,"%s","SE");
     if (!strcoll(buffer, "СЗ"))
          sprintf(buffer,"%s","NW");
     if (!strcoll(buffer, "СВ"))
          sprintf(buffer,"%s","NE");
     if (!strcoll(buffer, "безветрие"))
          sprintf(buffer,"%s","CALM");
     if (!strcoll(buffer, "Ш"))
          sprintf(buffer,"%s","CALM");
     g_hash_table_insert(current_weather, "wind_direction", g_strdup(buffer));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='wicon wind']/dl/dd/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      /* Normalize speed to km/h from m/s */
      /* fprintf(stderr, "Wind  speed    %s\n", temp_buffer); */
      speed = atoi (xpathObj->nodesetval->nodeTab[0]->content);
      speed = speed * 3600/1000;
      sprintf(buffer, "%i", speed);
      g_hash_table_insert(current_weather, "wind_speed", g_strdup(buffer));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/@title", xpathCtx);
  xpathObj2 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*[@class='clicon']/img/@src", xpathCtx);
  xpathObj3 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*[@class='cltext']/text()", xpathCtx);
  xpathObj4 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*[@class='temp']/text()", xpathCtx);
  xpathObj5 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*/text()", xpathCtx);
  xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*/dl[@class='wind']/dd/text()", xpathCtx);
  xpathObj7 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*/dl/dt/text()", xpathCtx);
  xpathObj8 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*/text()", xpathCtx);
  xpathObj9 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*/text()", xpathCtx);
  nodes   = xpathObj->nodesetval;
  size = (nodes) ? nodes->nodeNr : 0;
  if (size > 8)
      size = 8;
  for(i = 0; i < size; ++i) {
      /* Take UTC time: */
      if (!nodes->nodeTab[i]->children->content)
          continue;
      temp_char = strstr(nodes->nodeTab[i]->children->content, "UTC: ");
      if (temp_char && strlen(temp_char) > 6)
              temp_char = temp_char + 5;

      detail = g_hash_table_new(g_str_hash, g_str_equal);
      tmp_tm = get_date_for_hour_weather(temp_char);
      memset(buff, 0, sizeof(buff));
      setlocale(LC_TIME, "POSIX");

      strftime(buff, sizeof(buff) - 1, "%H", &tmp_tm);
      g_hash_table_insert(detail, "hours", g_strdup(buff));
      strftime(buffer, sizeof(buff) - 1, "%D %I:%M %p", &tmp_tm);

      setlocale(LC_TIME, "");

      if(!timezone_flag){
          utc_time = mktime(&tmp_tm);
          temp_char = strstr(nodes->nodeTab[i]->children->content, "Local: ");
          if (temp_char && strlen(temp_char) > 8)
              temp_char = temp_char + 7;
           tmp_tm_loc = get_date_for_hour_weather(temp_char);
           loc_time = mktime(&tmp_tm_loc);
           time_diff = difftime(loc_time, utc_time);
           if(time_diff)
               timezone_flag = TRUE;
           location_timezone = (gint)time_diff/3600;
           /* fprintf(stderr, "\nTimezone %i\n", location_timezone); */
           snprintf(temp_buffer, sizeof(temp_buffer)-1, "%i",location_timezone);
           g_hash_table_insert(location, "station_time_zone", g_strdup(temp_buffer));
      }
 
   /* added icon */
   if (xpathObj2 && !xmlXPathNodeSetIsEmpty(xpathObj2->nodesetval) && xpathObj2->nodesetval->nodeTab[i]->children->content){
      /* fprintf (stderr, "sdfff %s\n", xpathObj4->nodesetval->nodeTab[i]->children->content); */
      temp_char = strrchr((char*)xpathObj2->nodesetval->nodeTab[i]->children->content, '/');
      temp_char ++;
      g_hash_table_insert(detail, "hour_icon", choose_hour_weather_icon(hash_for_icons, temp_char));
   }
   /* added text */
   if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) && xpathObj3->nodesetval->nodeTab[i]->content){
      g_hash_table_insert(detail, "hour_title", 
                           g_strdup(hash_gismeteo_table_find(hash_for_translate, xpathObj3->nodesetval->nodeTab[i]->content, FALSE)));
   }
   /* added temperature */
   if (xpathObj4 && !xmlXPathNodeSetIsEmpty(xpathObj4->nodesetval) && xpathObj4->nodesetval->nodeTab[i]->content){
       /* fprintf (stderr, "temperature %s\n", xpathObj3->nodesetval->nodeTab[i]->content); */
        g_hash_table_insert(detail, "hour_temperature", g_strdup(xpathObj4->nodesetval->nodeTab[i]->content));
   }
   /* added pressure */
   if (xpathObj5 && !xmlXPathNodeSetIsEmpty(xpathObj5->nodesetval) && xpathObj5->nodesetval->nodeTab[i*5+2]->content){
      pressure = atoi((char*)xpathObj5->nodesetval->nodeTab[i*5+2]->content);
      pressure = pressure * 1.333224;
      snprintf(buffer, sizeof(buffer)-1,"%i", pressure);
      /* fprintf (stderr, "pressure %s\n", xpathObj5->nodesetval->nodeTab[i*5+2]->content); */
      g_hash_table_insert(detail, "hour_pressure", g_strdup(buffer));
   }
   /* added wind speed */
   if (xpathObj6 && !xmlXPathNodeSetIsEmpty(xpathObj6->nodesetval) && xpathObj6->nodesetval->nodeTab[i]->content){
      /* Normalize speed to km/h from m/s */
      /* fprintf(stderr, "Wind  speed    %s\n", temp_buffer); */
      speed = atoi (xpathObj6->nodesetval->nodeTab[i]->content);
      speed = speed * 3600/1000;
      sprintf(buffer, "%i", speed);
      g_hash_table_insert(detail, "hour_wind_speed", g_strdup(buffer));
   }
   /* added wind direction */
   if (xpathObj7 && !xmlXPathNodeSetIsEmpty(xpathObj7->nodesetval) && xpathObj7->nodesetval->nodeTab[i]->content){
       /* fprintf (stderr, "Wind direction: %s\n", xpathObj7->nodesetval->nodeTab[i]->content); */
       snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj7->nodesetval->nodeTab[i]->content);
       /* Wind direction */
       if (!strcoll(buffer, "З"))
            sprintf(buffer,"%s","W");
       if (!strcoll(buffer, "Ю"))
            sprintf(buffer,"%s","S");
       if (!strcoll(buffer, "В"))
            sprintf(buffer,"%s","E");
       if (!strcoll(buffer, "С"))
            sprintf(buffer,"%s","N");
       if (!strcoll(buffer, "ЮЗ"))
            sprintf(buffer,"%s","SW");
       if (!strcoll(buffer, "ЮВ"))
            sprintf(buffer,"%s","SE");
       if (!strcoll(buffer, "СЗ"))
            sprintf(buffer,"%s","NW");
       if (!strcoll(buffer, "СВ"))
            sprintf(buffer,"%s","NE");
       if (!strcoll(buffer, "безветрие"))
            sprintf(buffer,"%s","CALM");
       if (!strcoll(buffer, "Ш"))
            sprintf(buffer,"%s","CALM");
 
       g_hash_table_insert(detail, "hour_wind_direction", g_strdup(buffer));
   }
  /* added humidity */
  if (xpathObj8 && !xmlXPathNodeSetIsEmpty(xpathObj8->nodesetval) && xpathObj8->nodesetval->nodeTab[i*5+3]->content){
     /* fprintf (stderr, "temperature %s\n", xpathObj9->nodesetval->nodeTab[i*5+3]->content); */
     g_hash_table_insert(detail, "hour_humidity", g_strdup(xpathObj8->nodesetval->nodeTab[i*5+3]->content));
  }
  /* added feels like */
  if (xpathObj9 && !xmlXPathNodeSetIsEmpty(xpathObj9->nodesetval) && xpathObj9->nodesetval->nodeTab[i*5+4]->content){
     /* fprintf (stderr, "hour_feels_like %s\n", xpathObj9->nodesetval->nodeTab[i*5+4]->content); */
     g_hash_table_insert(detail, "hour_feels_like", g_strdup(xpathObj9->nodesetval->nodeTab[i*5+4]->content));
  }

  hour_weather = g_slist_append(hour_weather,(gpointer)detail);

  }
  g_hash_table_insert(hours_data, "hours_data", (gpointer)hour_weather);
  g_hash_table_insert(data, "detail", (gpointer)hours_data);
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);
  if (xpathObj2)
    xmlXPathFreeObject(xpathObj2);
  if (xpathObj3)
    xmlXPathFreeObject(xpathObj3);
  if (xpathObj4)
    xmlXPathFreeObject(xpathObj4);
  if (xpathObj5)
    xmlXPathFreeObject(xpathObj5);
  if (xpathObj6)
    xmlXPathFreeObject(xpathObj6);
  if (xpathObj7)
    xmlXPathFreeObject(xpathObj7);
  if (xpathObj8)
    xmlXPathFreeObject(xpathObj8);
  if (xpathObj9)
    xmlXPathFreeObject(xpathObj9);
  if (xpathCtx)
    xmlXPathFreeContext(xpathCtx); 

}
/*******************************************************************************/
gint
parse_xml_detail_data(const gchar *station_id, htmlDocPtr doc, GHashTable *data){

    GHashTable  *current_weather = NULL;
    GHashTable  *hash_for_icons;
    GHashTable  *hash_for_translate;
    GHashTable  *location = NULL;
    hash_for_translate = hash_description_gismeteo_table_create();
    hash_for_icons = hash_icons_gismeteo_table_create();
    current_weather = g_hash_table_new(g_str_hash, g_str_equal);
    location = g_hash_table_new(g_str_hash, g_str_equal);
//    fill_current_data(root_node, current_weather, data, hash_for_translate, hash_for_icons);
    g_hash_table_insert(data, "location", (gpointer)location);
    g_hash_table_insert(data, "current", (gpointer)current_weather);
    fill_detail_data(doc, location, hash_for_icons, hash_for_translate, data);
    g_hash_table_destroy(hash_for_translate);
    g_hash_table_destroy(hash_for_icons);

    return -1;
}
/**************************************************************************/
