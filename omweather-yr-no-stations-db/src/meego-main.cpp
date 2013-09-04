/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-weather-com-stations-db - MeeCast
 *
 * Copyright (C) 2006-2013 Vlad Vasilyeu
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
#include "meego-main.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <locale.h>

static xmlHashTablePtr hash_for_icons;
static xmlHashTablePtr hash_for_translate;
#ifdef GLIB
    static GHashTable *data = NULL;
#endif 
/*******************************************************************************/
#ifdef QT
    static QHash<QString, QString> *hash_for_icons;
    static QHash<QString, QString> *hash_for_translate;
    QHash<QString, QString> *hash_icons_yrno_table_create(void);
#endif
/*******************************************************************************/
#ifdef GLIB
    int
    source_init(void){
        data = g_hash_table_new(g_str_hash, g_str_equal);
        if(!data)
            return FALSE;
        return TRUE;
    }
#endif
/*******************************************************************************/
#ifdef GLIB
    void
    source_destroy(void){
        GHashTable  *hashtable1 = NULL;
        void *hashtable = NULL;
        void *tmp = NULL;
        /* free station location data */
        hashtable = g_hash_table_lookup(data, "location");
        if(hashtable){
            g_hash_table_foreach((GHashTable *)hashtable, free_fields, NULL);
            g_hash_table_remove_all((GHashTable *)hashtable);
            g_hash_table_unref((GHashTable *)hashtable);
        }
        /* free station current data */
        hashtable = g_hash_table_lookup(data, "current");
        if(hashtable){
            g_hash_table_foreach((GHashTable *)hashtable, free_fields, NULL);
            g_hash_table_remove_all((GHashTable *)hashtable);
            g_hash_table_unref((GHashTable *)hashtable);
        }
        /* free station days data */
        tmp = g_hash_table_lookup(data, "forecast");
        while((GSList *)tmp){
            hashtable1 = (GHashTable *)(((GSList *)tmp)->data);
            g_hash_table_foreach((GHashTable *)hashtable1, free_fields, NULL);
            g_hash_table_remove_all((GHashTable *)hashtable1);
            g_hash_table_unref((GHashTable *)hashtable1);
            tmp = g_slist_next(tmp);
        }
        tmp = g_hash_table_lookup(data, "forecast");
        if(tmp)
            g_slist_free((GSList *)tmp);
        if(data){
            g_hash_table_remove_all(data);
            g_hash_table_destroy(data);
        }
    }
/*******************************************************************************/
    void
    free_fields(gpointer key, gpointer val, gpointer user_data){
    #ifdef DEBUGFUNCTIONCALL
        START_FUNCTION;
    #endif
        if(val){
            g_free(val);
            val = NULL;
        }
    }
#endif
/*******************************************************************************/

int
convert_station_yrno_data(const char *station_id_with_path, const char *result_file, const char *detail){
 
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int     days_number = -1;
    char    buffer[1024],
            buffer2[1024],
            *delimiter = NULL;
    
    if(!station_id_with_path)
        return -1;
/* check for new file, if it exist, than rename it */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s.new", station_id_with_path);
    if(!access(buffer, R_OK))
        rename(buffer, station_id_with_path);
    /* check file accessability */
    if(!access(station_id_with_path, R_OK)){
        /* check that the file containe valid data */
        doc = xmlReadFile(station_id_with_path, NULL, 0);
        if(!doc)
            return -1;
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
            snprintf(buffer2, sizeof(buffer2) - 1, "%s", station_id_with_path);
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
                days_number = parse_and_write_yrno_xml_data(buffer, root_node, result_file);
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
int
parse_and_write_yrno_xml_data(char *station_id, xmlNode *root_node, const char *result_file){
    xmlNode     *cur_node = NULL,
                *child_node = NULL,
                *child_node1 = NULL,
                *child_node2 = NULL;
    xmlChar     *temp_xml_string = NULL;
    int         count_day = 0;
    char        id_station[10],
                buff[256];
    struct tm   tmp_tm = {0};

#ifdef GLIB
    GHashTable  *current = NULL,
                *day = NULL;
#endif
    int         first_day = true;
    int         period;
    int         timezone = 0;
#ifdef GLIB
    GHashTable  *hash_for_translate;
    GHashTable  *hash_for_icons;
#endif
    int speed;
    time_t      utc_time;
    time_t      current_time;
    time_t      end_of_first_day;
    FILE        *file_out;

    int    localtimezone;
    struct tm time_tm1;
    struct tm time_tm2;


#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef GLIB
    hash_for_translate = hash_description_yrno_table_create();
    hash_for_icons = hash_icons_yrno_table_create();
#endif
#ifdef QT
    hash_for_translate = hash_description_yrno_table_create();
    hash_for_icons = hash_icons_yrno_table_create();
#endif

    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;

    hash_for_icons = hash_icons_yrno_table_create();
    hash_for_translate = hash_description_yrno_table_create();

    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 
    /* fprintf(stderr,"Local Time Zone %i\n", localtimezone); */


    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", station_id);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");

    for(cur_node = root_node->children; cur_node; cur_node = cur_node->next){
        if( cur_node->type == XML_ELEMENT_NODE ){
            /* get weather station data */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "location" ) ){
                if ((temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"id")) != NULL){
                    snprintf(id_station, sizeof(id_station) - 1,
                                "%s", temp_xml_string);
                    xmlFree(temp_xml_string);
                    temp_xml_string = NULL;
                }
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                    if( child_node->type == XML_ELEMENT_NODE ){
                        if(!xmlStrcmp(child_node->name, (const xmlChar *) "location" ) ){
                            /* station lattitude */
                                temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"latitude");
                                xmlFree(temp_xml_string);
                            /* station longitude */
                                temp_xml_string =  xmlGetProp(child_node, (const xmlChar*)"longitude");
                                xmlFree(temp_xml_string);
                                continue;
                        }
                        /* station time zone */
                        if( !xmlStrcmp(child_node->name, (const xmlChar *)"timezone") ){
                            memset(buff, 0, sizeof(buff));
                            if ((temp_xml_string = xmlGetProp(child_node, (const xmlChar*)"utcoffsetMinutes")) != NULL){
                                timezone = atoi((char *)temp_xml_string)/60;
                                fprintf(file_out,"  <timezone>%i</timezone>\n", timezone);
                                xmlFree(temp_xml_string);
                                temp_xml_string = NULL;
                            }
                            continue;
                        }
                    }
                }
            }
            /* fill sun set sun rise */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "sun" ) ){
                if (temp_xml_string = xmlGetProp(child_node1, (const xmlChar*)"rise")){
                    setlocale(LC_TIME, "POSIX");
                    strptime((const char*)temp_xml_string, "%Y-%m-%dT", &tmp_tm);
                    setlocale(LC_TIME, "");
                    memset(buff, 0, sizeof(buff));
                    strftime(buff, sizeof(buff) - 1, "%a", &tmp_tm);
                    utc_time = mktime(&tmp_tm) - timezone * 3600 + localtimezone*3600;
                    fprintf(file_out,"    <period start=\"%li\" end=\"%li\"", utc_time, utc_time + 24*3600);
                    setlocale(LC_TIME, "POSIX");
                    strptime((const char*)temp_xml_string, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                    setlocale(LC_TIME, "");
                    memset(buff, 0, sizeof(buff));
                    strftime(buff, sizeof(buff) - 1, "%a", &tmp_tm);
                    utc_time = mktime(&tmp_tm) - timezone * 3600 + localtimezone*3600;
                    end_of_first_day = mktime(&tmp_tm) - timezone * 3600 + 24*3600+1 + localtimezone*3600;
                    fprintf(file_out,"    <sunrise> %li <sunirise>", utc_time);
                    xmlFree(temp_xml_string);
                    temp_xml_string = NULL;
                    if (temp_xml_string = xmlGetProp(child_node1, (const xmlChar*)"set")){
                        setlocale(LC_TIME, "POSIX");
                        strptime((const char*)temp_xml_string, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                        setlocale(LC_TIME, "");
                        memset(buff, 0, sizeof(buff));
                        strftime(buff, sizeof(buff) - 1, "%a", &tmp_tm);
                        utc_time = mktime(&tmp_tm) - timezone * 3600 + localtimezone*3600;
                        fprintf(file_out,"    <sunset> %li <sunset>", utc_time);
                        xmlFree(temp_xml_string);
                        temp_xml_string = NULL;
                    }
                    fprintf(file_out,"    </period>\n");
                }

            }

            /* Fill other days */
            if(!xmlStrcmp(cur_node->name, (const xmlChar *) "forecast" ) ){
                for(child_node = cur_node->children; child_node; child_node = child_node->next){
                   if (!xmlStrcmp(child_node->name, (const xmlChar *) "tabular" )){
                    for(child_node1 = child_node->children; child_node1; child_node1 = child_node1->next){
                        if(child_node1->type == XML_ELEMENT_NODE  &&
                                ( !xmlStrcmp(child_node1->name, (const xmlChar *)"time") ) ){
                            if (temp_xml_string = xmlGetProp(child_node1, (const xmlChar*)"period")){
                                if (!xmlStrcmp(temp_xml_string, (const xmlChar *)"0")){
                                    period = 0;
                                    count_day++;
                                }else if (!xmlStrcmp(temp_xml_string, (const xmlChar *)"1"))
                                         period = 1;
                                     else if (!xmlStrcmp(temp_xml_string, (const xmlChar *)"2"))
                                              period = 2;
                                          else if (!xmlStrcmp(temp_xml_string, (const xmlChar *)"3"))
                                                   period = 3;
                                               else
                                                   period = INT_MAX;
                                xmlFree(temp_xml_string);
                                temp_xml_string = NULL;
                            }

                            /* add day */
                            if (temp_xml_string = xmlGetProp(child_node1, (const xmlChar*)"from")){
                                setlocale(LC_TIME, "POSIX");
                                strptime((const char*)temp_xml_string, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                                setlocale(LC_TIME, "");
                                memset(buff, 0, sizeof(buff));
                                strftime(buff, sizeof(buff) - 1, "%a", &tmp_tm);
                                utc_time = mktime(&tmp_tm) - timezone * 3600 + localtimezone*3600;
                                /* increase past time for first forecast data */ 
                                if (first_day){
                                    first_day = false;
                                    utc_time = utc_time - 12*3600;
                                }
                                fprintf(file_out,"    <period start=\"%li\"", utc_time);
                                xmlFree(temp_xml_string);
                                temp_xml_string = NULL;
                                if (temp_xml_string = xmlGetProp(child_node1, (const xmlChar*)"to")){
                                    setlocale(LC_TIME, "POSIX");
                                    strptime((const char*)temp_xml_string, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                                    setlocale(LC_TIME, "");
                                    memset(buff, 0, sizeof(buff));
                                    strftime(buff, sizeof(buff) - 1, "%a", &tmp_tm);
                                    utc_time = mktime(&tmp_tm) - timezone * 3600 + localtimezone*3600;
                                    fprintf(file_out," end=\"%li\">\n", utc_time); 
                                    xmlFree(temp_xml_string);
                                    temp_xml_string = NULL;
                                }
                            }
                            for(child_node2 = child_node1->children; child_node2; child_node2 = child_node2->next){
                                if( child_node2->type == XML_ELEMENT_NODE){
                                    /* 24h hi temperature */
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"temperature")){
                                        if (temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"value")){
			                                fprintf(file_out,"     <temperature>%s</temperature>\n", (char*)temp_xml_string); 
                                            xmlFree(temp_xml_string);
                                            temp_xml_string = NULL;
                                        }
                                        continue;
                                    }  /* 24h icon */
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"symbol")){
                                        temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"number");
                                        memset(buff, 0, sizeof(buff));
                                        if (period == 0){
                                            snprintf(buff, sizeof(buff)-1, "night%s",(char*)temp_xml_string);
                                            #ifdef GLIB
			                                    fprintf(file_out,"     <icon>%s</icon>\n", (char *)hash_yrno_table_find(hash_for_icons, buff , FALSE)); 
                                            #endif
                                            #ifdef QT
			                                    fprintf(file_out,"     <icon>%s</icon>\n", hash_yrno_icon_table_find(hash_for_icons, buff).toStdString().c_str()); 
                                            #endif
                                            if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)buff)){
                                                fprintf(file_out,"     <icon>%s</icon>\n",  
                                                    (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)buff));
                                            }else 
                                                fprintf(file_out,"     <icon>49</icon>\n");  
                                        }
                                        if (period == 1){
                                            snprintf(buff, sizeof(buff)-1, "night%s",(char*)temp_xml_string);
                                            #ifdef GLIB
			                                    fprintf(file_out,"     <icon>%s</icon>\n", (char*)hash_yrno_table_find(hash_for_icons, buff , FALSE)); 
                                            #endif
                                            #ifdef QT
			                                    fprintf(file_out,"     <icon>%s</icon>\n", hash_yrno_icon_table_find(hash_for_icons, buff).toStdString().c_str()); 
                                            #endif
                                             if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)buff)){
                                                fprintf(file_out,"     <icon>%s</icon>\n",  
                                                    (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)buff));
                                            }else 
                                                fprintf(file_out,"     <icon>49</icon>\n");  

                                        }
                                        if (period == 2){
                                            snprintf(buff, sizeof(buff)-1, "day%s",(char*)temp_xml_string);
                                            #ifdef GLIB
			                                    fprintf(file_out,"     <icon>%s</icon>\n", (char*)hash_yrno_table_find(hash_for_icons, buff , FALSE)); 
                                            #endif
                                            #ifdef QT
			                                    fprintf(file_out,"     <icon>%s</icon>\n", hash_yrno_icon_table_find(hash_for_icons, buff).toStdString().c_str()); 
                                            #endif
                                             if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)buff)){
                                                fprintf(file_out,"     <icon>%s</icon>\n",  
                                                    (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)buff));
                                            }else 
                                                fprintf(file_out,"     <icon>49</icon>\n");  

                                        }
                                        if (period == 3){
                                            snprintf(buff, sizeof(buff)-1, "day%s",(char*)temp_xml_string);
                                            #ifdef GLIB
			                                    fprintf(file_out,"     <icon>%s</icon>\n", (char*)hash_yrno_table_find(hash_for_icons, buff , FALSE)); 
                                            #endif
                                            #ifdef QT
			                                    fprintf(file_out,"     <icon>%s</icon>\n", hash_yrno_icon_table_find(hash_for_icons, buff).toStdString().c_str()); 
                                            #endif
                                             if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)buff)){
                                                fprintf(file_out,"     <icon>%s</icon>\n",  
                                                    (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)buff));
                                            }else 
                                                fprintf(file_out,"     <icon>49</icon>\n");  

                                        }
                                        xmlFree(temp_xml_string);
                                        temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"name");
                                        #ifdef GLIB
			                                fprintf(file_out,"     <description>%s</description>\n",
                                                         hash_yrno_table_find(hash_for_translate, 
                                                         (char*)temp_xml_string, FALSE));
                                        #endif
                                        #ifdef QT
			                                fprintf(file_out,"     <description>%s</description>\n", hash_yrno_description_table_find(hash_for_translate,  (char*)temp_xml_string).toStdString().c_str()); 
                                        #endif
                                        if ((char*)xmlHashLookup(hash_for_translate, (const xmlChar*)temp_xml_string)){
			                                fprintf(file_out,"     <description>%s</description>\n",
                                                    (char*)xmlHashLookup(hash_for_translate, (const xmlChar*)temp_xml_string));
                                        }
                                        xmlFree(temp_xml_string);
                                        temp_xml_string = NULL;
                                        continue;
                                    }
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"pressure") ){
                                        if (temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"value")){
                                            fprintf(file_out,"     <pressure>%s</pressure>\n",
                                                                       (char*)temp_xml_string);
                                            xmlFree(temp_xml_string);
                                            temp_xml_string = NULL;
                                        }
                                        continue;
                                    }
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"precipitation") ){
                                        if (temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"value")){
                                            fprintf(file_out,"     <precipitation>%s</precipitation>\n",
                                                                   (char*)temp_xml_string);
                                            xmlFree(temp_xml_string);
                                            temp_xml_string = NULL;
                                        }
                                        continue;
                                    }
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"windSpeed") ){
                                        if (temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"mps")){
                                            /* Normalize speed to km/h from m/s */
                                            /* fprintf(stderr, "Wind  speed    %s\n", temp_buffer); */
                                            speed = atoi((char*)temp_xml_string);
                                            /* speed = speed * 3600/1000; why??? */
                                            memset(buff, 0, sizeof(buff));
                                            snprintf(buff, sizeof(buff)-1, "%i", speed);
                                            fprintf(file_out,"     <wind_speed>%s</wind_speed>\n",  buff);
                                            xmlFree(temp_xml_string);
                                            temp_xml_string = NULL;
                                        }
                                        continue;
                                    }
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"windDirection") ){
                                        if (temp_xml_string = xmlGetProp(child_node2, (const xmlChar*)"code")){
                                            fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",
                                                                                     (char*)temp_xml_string);
                                            xmlFree(temp_xml_string);
                                            temp_xml_string = NULL;
                                        }
                                        continue;
                                    }



                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"sunr")){
                                        temp_xml_string = xmlNodeGetContent(child_node2);

                                        #ifdef GLIB
                                            g_hash_table_insert(day, (gpointer)"day_sunrise", (gpointer)g_strdup((char *)temp_xml_string));
                                        #endif
                                        xmlFree(temp_xml_string);
                                        continue;
                                    }
                                    /* 24h sunset */
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *)"suns")){
                                        temp_xml_string = xmlNodeGetContent(child_node2);
                                        #ifdef GLIB
                                            g_hash_table_insert(day, (gpointer)"day_sunset", (void *)g_strdup((char*)temp_xml_string));
                                        #endif
                                        xmlFree(temp_xml_string);
                                        continue;
                                    }
                                }
                            }
                            fprintf(file_out,"    </period>\n");
                        }
                     }    
                   }
                }
            }
        }
    }
#ifdef GLIB
    g_hash_table_destroy(hash_for_translate);                                                        
    g_hash_table_destroy(hash_for_icons);
#endif
    fprintf(file_out,"</station>");
    fclose(file_out);

    xmlHashFree(hash_for_icons, NULL);
    xmlHashFree(hash_for_translate, NULL);
    return count_day;
}

/*******************************************************************************/
int
main_yr_no(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "yrno <input_file> <output_file>\n");
        return -1;
    }
    result = convert_station_yrno_data(argv[1], argv[2], argv[3]);
    fprintf(stderr, "\nresult = %d\n", result);
    /* fprintf(stderr, "\nresult = %d\n", result); */
    return result;
}
