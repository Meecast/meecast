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
#define DAY 1
#define NIGHT 2
#define MORNING 3
#define EVENING 4
/*******************************************************************************/
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
choose_icon(GHashTable *hash_for_icons, gchar *image1, gchar *image2)
{
    gchar *result;
    gchar *source;
    if (!image1 || !image2)
        return g_strdup("49");
    source = g_strdup_printf("%s %s", image1, image2);
    result = hash_gismeteo_table_find(hash_for_icons, source, FALSE);
    g_free (source);
    if (strlen(result) == 2 || strlen(result) == 1)
       return g_strdup(result);
    else{
       fprintf(stderr,"Unknown strings %s %s",image1, image2);
       return g_strdup("49");
    }
}
/*******************************************************************************/
gchar*
choose_hour_weather_icon(GHashTable *hash_for_icons, gchar *image)
{
    gchar *result;
    gchar *source;
    if(!image)
        return g_strdup("49");
    source = g_strdup_printf("%s", image);
    result = hash_gismeteo_table_find(hash_for_icons, source, FALSE);
    g_free (source);
    if (strlen(result) == 2 || strlen(result) == 1)
        return g_strdup(result);
    else{
        fprintf(stderr,"Unknown string %s",image);
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
    gint speed;

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
    gchar *temp_point;
    gchar *temp_char;
    time_t rawtime;
    memset(buffer, 0, sizeof(buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    temp_point = strchr(temp_string,' ');
    snprintf(buffer, strlen(temp_string) - strlen(temp_point+1),"%s", temp_string);
    snprintf(temp_buffer, strlen(temp_point) - strlen(strchr(temp_point + 1,' ')),"%s", temp_point+1);
    if (!strcoll(temp_buffer, "Янв"))
        strcat(buffer," Jun");
    if (!strcoll(temp_buffer, "Фев"))
        strcat(buffer," Feb");
    if (!strcoll(temp_buffer, "Мар"))
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
    /* fprintf(stderr, "\ntmp_tm min %d\n", (&tmp_tm)->tm_min); */
    return tmp_tm;
}
/*******************************************************************************/
struct tm
get_date_for_hour_weather(gchar *temp_string){
    gchar buffer[256];
    gchar temp_buffer[256];
    gchar buff[256];
    struct tm   tmp_tm = {0};
    gchar *temp_point;
    memset(buffer, 0, sizeof(buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memset(buff, 0, sizeof(buff));
    temp_point = strchr(temp_string,'-');
    if (!temp_point)
        return tmp_tm;
    snprintf(buffer, strlen(temp_string) - strlen(temp_point+1),"%s", temp_string);
    snprintf(temp_buffer, strlen(temp_point) - strlen(strchr(temp_point + 1,'-')),"%s", temp_point+1);
    temp_point = strrchr(temp_string,'-');
    snprintf(buff, strlen(temp_point) - strlen(strchr(temp_point + 1,' ')),"%s", temp_point+1);
    if (!strcoll(temp_buffer, "01"))
        strcat(buff," Jun");
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
    /* fprintf(stderr, "\ntmp_tm hour %d\n", (&tmp_tm)->tm_hour); */
    return tmp_tm;
}
/*******************************************************************************/
void
fill_current_data(xmlNode *root_node, GHashTable *current_weather, GHashTable *hash_for_translate, GHashTable *hash_for_icons)
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
     xmlChar     *temp_xml_string = NULL;
     gint        i = 0; 
     gchar       buffer[buff_size];
     gchar       temp_buffer[buff_size];
     gchar       *image = NULL;
     gchar       *temp_char;
     gint        speed;
     struct tm   tmp_tm = {0};

 if (root_node->children)
    cur_node = root_node->children;
 
 cur_node = cur_node->next;
 cur_node = cur_node->children;

 for(cur_node = cur_node; cur_node; cur_node = cur_node->next){
    if(!xmlStrcmp(cur_node->name, (const xmlChar *)"div")){
        for(child_node = cur_node->children; child_node; child_node = child_node->next){
            if(!xmlStrcmp(child_node->name, (const xmlChar *)"div")){
                for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"div")){
                        for(child_node3 = child_node2->children; child_node3; child_node3 = child_node3->next){
                            if (!xmlStrcmp(child_node3->name, (const xmlChar *)"div")) {
                                temp_xml_string = xmlGetProp(child_node3, (const xmlChar*)"class");
                                    if(!xmlStrcmp(temp_xml_string, (const xmlChar *)"M123 clearfix")){
                                        child_node4 = child_node3->children;
                                        child_node4 = child_node4->next;
                                        child_node4 = child_node4->children;
                                        child_node4 = child_node4->next;
                                        child_node4 = child_node4->children;
                                        child_node4 = child_node4->next;
                                        child_node4 = child_node4->children;

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

                                        child_node6 = child_node4->children;
                                        child_node6 = child_node6->next;
                                        child_node6 = child_node6->next;
                                        child_node6 = child_node6->children;
                                        child_node6 = child_node6->next;
                                        /* Pressure */
                                        for(child_node6 = child_node6->children;child_node6;child_node6 = child_node6->next){
                                             xmlFree(temp_xml_string);
                                             temp_xml_string = xmlGetProp(child_node6,(const xmlChar*)"class");
                                             if(!xmlStrcmp(temp_xml_string, (const xmlChar*)"c1")){
                                                xmlFree(temp_xml_string); 
                                                temp_xml_string = xmlNodeGetContent(child_node6);
                                                g_hash_table_insert(current_weather, "pressure", g_strdup((char*)temp_xml_string));
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
                                        setlocale(LC_TIME, "");
                                        g_hash_table_insert(current_weather, "last_update", g_strdup(buffer));
                                        xmlFree(temp_xml_string);
                                    }else
                                        xmlFree(temp_xml_string);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
/*******************************************************************************/
gint
parse_xml_data(const gchar *station_id, xmlNode *root_node, GHashTable *data){
    xmlNode     *cur_node = NULL,
                *child_node = NULL,
                *child_node3 = NULL;
    xmlChar     *temp_xml_string = NULL;
    gint        count_day = 0,
                count_night = 0,
                count_of_div = 0,
                count_of_div_temp = 0,
                count_of_table = 0;
    gchar       buff[256],
                *delimiter = NULL;
    struct tm   tmp_tm = {0};
    struct tm   *gmt = NULL ;
    time_t      first_day, day_in_list, night_in_list;
    GSList      *forecast = NULL;
    GSList      *tmp = NULL;
    GHashTable  *day = NULL;
    gboolean    flag;
    GHashTable *hash_for_translate;
    GHashTable *hash_for_icons;

/* Fix me free memory */
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
                            if (temp_xml_string){
                                fprintf(stderr,"Date %s\n", temp_xml_string);
                                tmp_tm = get_data_from_russia_data((char*)temp_xml_string);
                                first_day = mktime(&tmp_tm) - 3600*24;
                                day_in_list = night_in_list = first_day;
                                xmlFree(temp_xml_string);
                            }
                        }
                        if (count_of_div > 2 && 
                            !xmlStrcmp(child_node->name, (const xmlChar *)"div")){
                            temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"style");
                            if (!xmlStrcmp(temp_xml_string,
                                           (const xmlChar*)"float: left; padding: 0 0 0 4px;")){
                                xmlFree(temp_xml_string);
                                for(child_node3 = child_node->children; child_node3 != NULL; child_node3 = child_node3->next){
                                    if (!xmlStrcmp(child_node3->name, (const xmlChar *)"img")){
                                       temp_xml_string = xmlGetProp(child_node3, (const xmlChar*)"src");
                                       delimiter = strrchr((char*)temp_xml_string, '/');
                                       if(delimiter){
                                            delimiter++; /* delete '/' */
                                            if (delimiter && !strcmp(delimiter,"day.gif"))
                                                count_day++;
                                            if (delimiter && !strcmp(delimiter,"night.gif"))
                                                count_night++;
                                       }
                                       if (temp_xml_string)
                                            xmlFree(temp_xml_string);
                                    }

                                }
                            }else
                                if (temp_xml_string)
                                    xmlFree(temp_xml_string);
                        }
                   }
                }
            }
        }
    }
    /* Filling data */
    count_of_div = 0;
    count_of_div_temp = 0;
    count_of_table = 0;
    for(cur_node = root_node->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
             /*  fprintf(stderr,"cur_node %s\n",cur_node->name); */
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
                            if (temp_xml_string){
                                /* fprintf(stderr,"Date %s\n", temp_xml_string); */
                                tmp_tm = get_data_from_russia_data((char*)temp_xml_string);
                                first_day = mktime(&tmp_tm) -3600*24;
                                day_in_list = first_day+(count_day*3600*24);
                                night_in_list = first_day+(count_night*3600*24);
                                xmlFree(temp_xml_string);
                            }
                        }
                        if (count_of_div > 2 && 
                            !xmlStrcmp(child_node->name, (const xmlChar *)"div")){
                            temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"style");
                            if (!xmlStrcmp(temp_xml_string, 
                                           (const xmlChar*)"float: left; padding: 0 0 0 4px;"))
                            {
                                xmlFree(temp_xml_string);
                                for(child_node3 = child_node->children; child_node3 != NULL; child_node3 = child_node3->next){
                                    if (!xmlStrcmp(child_node3->name, (const xmlChar *)"img")){
                                       temp_xml_string = xmlGetProp(child_node3, (const xmlChar*)"src");
                                       delimiter = strrchr((char*)temp_xml_string, '/');
                                       if(delimiter){
                                            delimiter++; /* delete '/' */
                                            if (delimiter && 
                                                (!strcmp(delimiter,"day.gif")||
                                                (!strcmp(delimiter,"night.gif"))
                                                )){
                                                memset(buff, 0, sizeof(buff));
                                                setlocale(LC_TIME, "POSIX");

                                                if (!strcmp(delimiter,"day.gif"))
                                                    gmt = gmtime(&day_in_list);
                                                if (!strcmp(delimiter,"night.gif"))
                                                    gmt = gmtime(&night_in_list);

                                                memset(buff, 0, sizeof(buff));
                                                strftime(buff, sizeof(buff) - 1, "%b %d", gmt);
                                                setlocale(LC_TIME, "");
                                                /* searching of a day */
                                                tmp = forecast;
                                                flag = FALSE;
                                                while(tmp){
                                                    day = (GHashTable*)tmp->data;
                                                    if (g_hash_table_lookup(day, "day_date") &&
                                                        !strcmp(g_hash_table_lookup(day,"day_date"), buff)){
                                                            flag = TRUE;
                                                            break;
                                                    }
                                                    tmp = g_slist_next(tmp);
                                                }
                                                if (!flag){
                                                    day = g_hash_table_new(g_str_hash, g_str_equal);
                                                    g_hash_table_insert(day, "day_date", g_strdup(buff));
                                                    strftime(buff, sizeof(buff) - 1, "%a", gmt);
                                                    g_hash_table_insert(day, "day_name", g_strdup(buff));
 
                                                    forecast = g_slist_prepend(forecast,(gpointer)day);
                                                }
                                                if (day){
                                                    if (!strcmp(delimiter,"day.gif")){
                                                        fill_day(child_node, day, DAY, hash_for_translate, hash_for_icons);
                                                        day_in_list = day_in_list - 3600*24;
                                                    }
                                                    if (!strcmp(delimiter,"night.gif")){
                                                        fill_day(child_node, day, NIGHT, hash_for_translate, hash_for_icons);
                                                        night_in_list = night_in_list - 3600*24;
                                                    }

                                                }
                                            }
                                       }
                                       if (temp_xml_string)
                                            xmlFree(temp_xml_string);
                                    }
                                }
                            }else
                                if (temp_xml_string)
                                        xmlFree(temp_xml_string);
                        }
                   }
                }
                g_hash_table_insert(data, "forecast", (gpointer)forecast);
            }
        }
    }
    g_hash_table_destroy(hash_for_translate);
    g_hash_table_destroy(hash_for_icons);
    return count_day;
}
/*******************************************************************************/
void
fill_detail_data(xmlNode *root_node, GHashTable *location, GHashTable *hash_for_icons, GHashTable *hash_for_translate, GHashTable *data){
    #define buff_size 2048
    xmlNode     *cur_node = NULL;
    xmlNode     *child_node = NULL;
    xmlNode     *child_node2 = NULL;
    xmlNode     *child_node3 = NULL;
    xmlNode     *child_node4 = NULL;
    xmlNode     *child_node5 = NULL;
    xmlNode     *child_node6 = NULL;
    xmlNode     *child_node7 = NULL;
    xmlNode     *child_node8 = NULL;
    xmlNode     *child_node9 = NULL;
    xmlNode     *child_node10 = NULL;
    xmlNode     *child_node11 = NULL;
    xmlNode     *child_node12 = NULL;
    xmlChar     *temp_xml_string = NULL;
    xmlChar     *temp_xml_string_2 = NULL;
    gint        i = 0, j = 0, k = 0;
    gchar       buffer[buff_size];
    gchar       temp_buffer[buff_size];
    gchar       tmp[buff_size];
    gchar       *temp_char;
    gint        speed;
    gint        count_of_hours = 0;
    gint        location_timezone = 0;
    GHashTable  *detail = NULL; 
    GSList      *hour_weather = NULL;
    GHashTable  *hours_data = NULL;
    struct tm   tmp_tm = {0};
    struct tm   tmp_tm_utc = {0};
    double      time_diff = 0;
    time_t      loc_time;
    time_t      utc_time;
    gboolean break_flag = FALSE; 
    gboolean timezone_flag = FALSE;
    gboolean feels_like_flag = FALSE;

    if (root_node->children)
         cur_node = root_node->children;

    cur_node = cur_node->next;
    cur_node = cur_node->children;
    hours_data = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(hours_data, "last_update", g_strdup(g_hash_table_lookup(g_hash_table_lookup(data, "current"),"last_update")));
    for(cur_node = cur_node; cur_node; cur_node = cur_node->next){
        if(!xmlStrcmp(cur_node->name, (const xmlChar *)"div")){
            for(child_node = cur_node->children; child_node; child_node = child_node->next){
             if(!xmlStrcmp(child_node->name, (const xmlChar *)"div")){
                for(child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){
                 if(!xmlStrcmp(child_node2->name, (const xmlChar *)"div")){
                    for(child_node3 = child_node2->children; child_node3; child_node3 = child_node3->next){
                     if (!xmlStrcmp(child_node3->name, (const xmlChar *)"div")) {
                        temp_xml_string = xmlGetProp(child_node3, (const xmlChar*)"class");
                        if(!xmlStrcmp(temp_xml_string, (const xmlChar *)"M123 clearfix")){
                             for(child_node4 = child_node3->children; child_node4; child_node4 = child_node4->next){
                              if (!xmlStrcmp(child_node4->name, (const xmlChar *)"div")) {
                                for(child_node5 = child_node4->children; child_node5; child_node5 = child_node5->next){
                                 if (!xmlStrcmp(child_node5->name, (const xmlChar *)"div")) {
                                    for(child_node6 = child_node5->children; child_node6; child_node6 = child_node6->next){
                                        xmlFree(temp_xml_string);
                                        temp_xml_string = xmlGetProp(child_node6, (const xmlChar*)"class");
                                        if(!xmlStrcmp(temp_xml_string, (const xmlChar *)"full")){
                                            for(child_node7 = child_node6->children; child_node7; child_node7 = child_node7->next){
                                                if (!xmlStrcmp(child_node7->name, (const xmlChar *)"div")) {
                                                    for(child_node8 = child_node7->children; child_node8;child_node8 = child_node8->next){
                                                        if (!xmlStrcmp(child_node8->name, (const xmlChar *)"div")) {
                                                            xmlFree(temp_xml_string);
                                                            temp_xml_string = xmlGetProp(child_node8, (const xmlChar*)"class");
                                                            if(!xmlStrcmp(temp_xml_string, (const xmlChar *)"pumpersW2")){
                                                                child_node9 = child_node8->children;
                                                                while(xmlStrcmp(child_node9->name,(const xmlChar *)"table")){
                                                                    child_node9 = child_node9->next;
                                                                }
                                                                child_node9 = child_node9->children;
                                                                for(child_node9;child_node9;child_node9=child_node9->next){
                                                                    child_node10 = child_node9->children;
                                                                    child_node10 = child_node10->next;
                                                                    child_node10 = child_node10->next;
                                                                    for(child_node10;child_node10;child_node10=child_node10->next){
                                                                        if (break_flag >1)
                                                                            break;
                                                                        detail = g_hash_table_new(g_str_hash, g_str_equal);
                                                                        fprintf(stderr," New detail %p\n", detail );
                                                                        for(child_node11=child_node10->children;child_node11;
                                                                            child_node11 = child_node11->next){
                                                                           if (temp_xml_string){
                                                                               xmlFree(temp_xml_string);
                                                                               temp_xml_string = NULL;
                                                                           }
                                                                           temp_xml_string = xmlGetProp(child_node11, (const xmlChar*)"class");
                                                                           /* Time */
                                                                           if(!xmlStrcmp(temp_xml_string,(const xmlChar*)"c0")){
                                                                               xmlFree(temp_xml_string);
                                                                               memset(buffer, 0, sizeof(     buffer));
                                                                               temp_xml_string = xmlGetProp(child_node11, (const xmlChar*)"title");
                                                                               for(i = 0;i<strlen((char*)temp_xml_string);i++){
                                                                                   if(temp_xml_string[i] == 'L')
                                                                                       j=i;
                                                                                   if(!timezone_flag){
                                                                                        if(temp_xml_string[i] == 'U')
                                                                                            k=i;
                                                                                   }
                                                                               }
                                                                               j += 7;
                                                                               for(j=j;j < strlen((char*)temp_xml_string);j++)
                                                                                   sprintf(buffer,"%s%c",buffer, temp_xml_string[j]);

                                                                               if (strlen(buffer)==0)
                                                                                   break;
                                                                               sprintf(tmp, "%s", buffer);

                                                                               /* fprintf(stderr, "Time 1 %s 2 %s\n", buffer, temp_xml_string); */
                                                                               tmp_tm = get_date_for_hour_weather(buffer);
                                                                               memset(buffer, 0, sizeof(buffer));
                                                                               setlocale(LC_TIME, "POSIX");
                                                                               strftime(buffer, sizeof(buffer) - 1, "%H", &tmp_tm);
                                                                               g_hash_table_insert(detail, "hours", g_strdup(buffer));
                                                                               strftime(buffer, sizeof(buffer) - 1, "%D %I:%M %p", &tmp_tm);
                                                                               setlocale(LC_TIME, "");

                                                                               if (buffer && 
                                                                                 g_hash_table_lookup(hours_data,"last_update")&&
                                                                                 !strcmp(buffer,g_hash_table_lookup(hours_data,"last_update"))){
                                                                                   feels_like_flag = TRUE;
                                                                               }
                                                                               if (buffer && (!strcmp(buffer,"00")))
                                                                                    break_flag++;
                                                                               /* fprintf(stderr,"HOUR %s\n",buffer); */
                                                                               if(!timezone_flag){
                                                                                    k += 5;
                                                                                    memset(temp_buffer,0, sizeof(temp_buffer));
                                                                                    for(k=k;k < strlen((char*)temp_xml_string)-strlen(tmp)-9;k++)
                                                                                        sprintf(temp_buffer,"%s%c",temp_buffer, temp_xml_string[k]);
                                                                                    /* fprintf(stderr, "\n temp_buffer %s\n", temp_buffer); */
                                                                                    tmp_tm_utc = get_date_for_hour_weather(temp_buffer);
                                                                                    tmp_tm = get_date_for_hour_weather(tmp);
                                                                                    loc_time = mktime(&tmp_tm);
                                                                                    utc_time = mktime(&tmp_tm_utc);
                                                                                    time_diff = difftime(loc_time, utc_time);
                                                                                    if(time_diff)
                                                                                        timezone_flag = TRUE;
                                                                                    location_timezone = (gint)time_diff/3600;
                                                                                    /* fprintf(stderr, "\nTimezone %i\n", location_timezone); */
                                                                                    snprintf(temp_buffer, sizeof(temp_buffer)-1,"%i",location_timezone);

                                                                                    g_hash_table_insert(location, "station_time_zone", g_strdup(temp_buffer));
                                                                               }
                                                                           }
                                                                           /* Icon */
                                                                           if(!xmlStrcmp(temp_xml_string,(const xmlChar*)"c1")){
                                                                               child_node12 = child_node11->children;
                                                                               child_node12 = child_node12->children;
                                                                               xmlFree(temp_xml_string);
                                                                               temp_xml_string = xmlGetProp(child_node12, (const xmlChar*)"src");
                                                                               if (temp_xml_string){
                                                                                   temp_char = strrchr((char*)temp_xml_string, '/');
                                                                                   if (temp_char){
                                                                                       temp_char ++;
                                                                                       g_hash_table_insert(detail, "hour_icon", choose_hour_weather_icon(hash_for_icons, temp_char));
                                                                                   }
                                                                               }
                                                                           }

                                                                           if(!xmlStrcmp(temp_xml_string,(const xmlChar*)"c2")){
                                                                               xmlFree(temp_xml_string);
                                                                               temp_xml_string = xmlNodeGetContent(child_node11);
                                                                               g_hash_table_insert(detail, "hour_title", g_strdup(hash_gismeteo_table_find(hash_for_translate, (char*)temp_xml_string, FALSE)));
                                                                               /* fprintf(stderr, "\nSumma %s\n", temp_xml_string); */
                                                                           }
                                                                           if(!xmlStrcmp(temp_xml_string,(const xmlChar*)"c3")){
                                                                               xmlFree(temp_xml_string);
                                                                               temp_xml_string = xmlNodeGetContent(child_node11);
                                                                               g_hash_table_insert(detail, "hour_temperature", g_strdup((char*)temp_xml_string));
                                                                               /* fprintf(stderr, "\nTemp%s\n", temp_xml_string); */
                                                                           }
                                                                           if(!xmlStrcmp(temp_xml_string,(const xmlChar*)"c4")){
                                                                               xmlFree(temp_xml_string);
                                                                               temp_xml_string = xmlNodeGetContent(child_node11);
                                                                               g_hash_table_insert(detail, "hour_pressure", g_strdup((char*)temp_xml_string));
                                                                               /* fprintf(stderr, "Pressure %s\n", temp_xml_string); */
                                                                           }
                                                                           if(!xmlStrcmp(temp_xml_string,(const xmlChar*)"c5")){
                                                                               xmlFree(temp_xml_string);
                                                                               memset(temp_buffer, 0, sizeof(temp_buffer));
                                                                               memset(buffer, 0, sizeof(buffer));
                                                                               temp_xml_string = xmlNodeGetContent(child_node11);
                                                                               /* Wind speed. m/s in km/h */
                                                                               speed = atoi ((char*)temp_xml_string);
                                                                               speed = speed * 3600/1000;
                                                                               sprintf(temp_buffer, "%i",speed);
                                                                               g_hash_table_insert(detail, "hour_wind_speed", g_strdup((char*)temp_buffer));
                                                                               child_node12 = child_node11->children;
                                                                               child_node12 = child_node12->children;
                                                                               temp_xml_string_2 = xmlGetProp(child_node12, (const xmlChar*)"title");
                                                                               sprintf(buffer,"%s%s",buffer,temp_xml_string_2);
                                                                               xmlFree(temp_xml_string_2);
                                                                               temp_xml_string_2 = NULL;
                                                                               /* fprintf(stderr, "Wind %s\n", buffer); */
                                                                               /* Wind direction*/
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
                                                                               g_hash_table_insert(detail, "hour_wind_direction", g_strdup(buffer));
                                                                              /*  fprintf(stderr, "\nWind %s\n", buffer); */
                                                                           }
                                                                           if(!xmlStrcmp(temp_xml_string,(const xmlChar*)"c6")){
                                                                               xmlFree(temp_xml_string);
                                                                               temp_xml_string = xmlNodeGetContent(child_node11);
                                                                               g_hash_table_insert(detail, "hour_humidity", 
                                                                                                           g_strdup((char*)temp_xml_string));
                                                                               /* fprintf(stderr, "\nHummidity %s \n", temp_xml_string); */
                                                                           }
                                                                           if(!xmlStrcmp(temp_xml_string,(const xmlChar*)"c7")){
                                                                               xmlFree(temp_xml_string);
                                                                               temp_xml_string = xmlNodeGetContent(child_node11);
                                                                               g_hash_table_insert(detail, "hour_feels_like",
                                                                                                           g_strdup((char*)temp_xml_string));
                                                                               if (feels_like_flag){ 
                                                                                    /* fprintf(stderr, "\nComfort %s \n", temp_xml_string); */
                                                                                    feels_like_flag = FALSE;
                                                                                    g_hash_table_insert(g_hash_table_lookup(data, "current"),"feel_like",g_strdup((char*)temp_xml_string));
                                                                               }
                                                                           }
                                                                        }
                                                                        if(detail){
                                                                           if (g_hash_table_lookup(detail, "hours"))
                                                                               hour_weather = g_slist_append(hour_weather,(gpointer)detail);
                                                                               fprintf(stderr," New detail confirmaed  %p\n", detail );
                                                                               count_of_hours++;
                                                                               if ((count_of_hours) >9)
                                                                                    break_flag++;
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
                                 }
                                }
                              }
                           }
                        }else{
                            xmlFree(temp_xml_string); 
                            temp_xml_string = NULL;
                        }
                     }
                    }
                 }
                }
             }
            }
        }
   }

   g_hash_table_insert(hours_data, "hours_data", (gpointer)hour_weather);
   detail = hour_weather->data;
   g_hash_table_insert(data, "detail", (gpointer)hours_data);
}
/*******************************************************************************/
gint
parse_xml_detail_data(const gchar *station_id, xmlNode *root_node, GHashTable *data){

    GHashTable  *current_weather = NULL;
    GHashTable  *hash_for_icons;
    GHashTable  *hash_for_translate;
    GHashTable  *location = NULL;
    gint location_timezone = 0;

    hash_for_translate = hash_description_gismeteo_table_create();
    hash_for_icons = hash_icons_gismeteo_table_create();
    current_weather = g_hash_table_new(g_str_hash, g_str_equal);
    location = g_hash_table_new(g_str_hash, g_str_equal);
    fill_current_data(root_node, current_weather, hash_for_translate, hash_for_icons);
    g_hash_table_insert(data, "location", (gpointer)location);
    g_hash_table_insert(data, "current", (gpointer)current_weather);
    fill_detail_data(root_node, location, hash_for_icons, hash_for_translate, data);
    g_hash_table_destroy(hash_for_translate);
    g_hash_table_destroy(hash_for_icons);

    return -1;
}
/**************************************************************************/
