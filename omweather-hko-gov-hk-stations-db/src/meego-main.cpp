/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-hko-gov-hk-stations-db - MeeCast
 *
 * Copyright (C) 2012 - 2013 Vlad Vasilyeu
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
#include <ctype.h> 
/*******************************************************************************/
#define buff_size 2048
#if GLIB
    static GHashTable *data = NULL;
#endif
#ifdef QT
    static QHash<QString, QString> *hash_for_icons;
    QHash<QString, QString> *hash_hko_table_create(void);
#endif
static xmlHashTablePtr hash_for_icons;
/*******************************************************************************/
#ifdef GLIB
gchar*
choose_hour_weather_icon(GHashTable *hash_for_icons, char *image)
{
    char *result;
    char *source;
    char *tmp_result = NULL;

    if(!image)
        return g_strdup("49");
    source = g_strdup_printf("%s", image);
    tmp_result = (char *)hash_hko_table_find(hash_for_icons, source, FALSE);
    result = g_strdup(tmp_result);
    g_free(source);
    return result;
}
#endif
#ifdef QT 
    QString
    choose_hour_weather_icon(QHash<QString, QString> *hash_for_icons, char *image){
        return hash_hko_table_find(hash_for_icons, image);
    }
#endif

void
parse_forecast_weather(const char *detail_path_data, const char *result_file){

    FILE    *file_out;
    FILE    *file_in;
    FILE    *file_in2;
    char buffer [4096];
    char buffer2 [4096];
    char temp_buffer [1024];
    char *comma = NULL;
    char *comma2 = NULL;
    struct tm   tmp_tm = {0};
    time_t      t_start = 0, t_end = 0;
    int   temperature, humidity1, humidity2, icon;
    double wind_speed;
    int   year=0;
    int   number_of_day = 0;
    fpos_t pos;
#ifdef GLIB
    GHashTable *hash_for_icons;
#endif
    file_out = fopen(result_file, "a");
    if (!file_out)
        return;
    file_in = fopen(detail_path_data, "r");
    if (!file_in)
        return;

    file_in2 = fopen(detail_path_data, "r");
    while(fgets(buffer, sizeof(buffer), file_in2)){
        if (strstr(buffer,"Weather Cartoons for 9-day weather forecast"))
            break;
    }
    hash_for_icons = hash_icons_hko_table_create();
 
    while(fgets(buffer, sizeof(buffer), file_in)){
        if (strstr(buffer,"Bulletin updated"))
            if ((comma = strstr(buffer, "at ")) != NULL){
                comma = comma + 3;
                setlocale(LC_TIME, "POSIX");
                /* 3:02 HKT 28/Apr/2012 */
                strptime((const char*)comma, "%H:%M HKT %d/%b/%Y", &tmp_tm);
                setlocale(LC_TIME, "");
                year = tmp_tm.tm_year + 1900;
            }
        if (strstr(buffer,"Date/Month"))
            if ((comma = strstr(buffer, "h ")) != NULL){
                if (number_of_day != 0)
                    fprintf(file_out,"    </period>\n");
                comma = comma + 2;
                snprintf(temp_buffer, sizeof(temp_buffer) - 1, "00:00 %i %s", year, comma);
                strptime((const char*)temp_buffer, "%H:%M %Y %d/%m", &tmp_tm);
                t_start = timegm(&tmp_tm) - 8*3600;
                fprintf(file_out,"    <period start=\"%li\"", (t_start));
                /* set end of current time in localtime */
                t_end = t_start + 3600*24 - 1;
                fprintf(file_out," end=\"%li\">\n", t_end);
                number_of_day ++; 
                if ((fgets(buffer2, sizeof(buffer2), file_in2) != NULL) && ((comma = strstr(buffer2, "no. ")) != NULL)){
                    comma = comma + 3;
                    icon = atoi (comma);
                    snprintf(temp_buffer, sizeof(temp_buffer) - 1, "%i", icon);
#ifdef GLIB
                    fprintf(file_out,"     <icon>%s</icon>\n", choose_hour_weather_icon(hash_for_icons, temp_buffer));
#endif
#ifdef QT 
                    fprintf(file_out, "     <icon>%s</icon>\n", choose_hour_weather_icon(hash_for_icons, temp_buffer).toStdString().c_str());
#endif
                   if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer)){
                        fprintf(file_out,"     <icon>%s</icon>\n",  
                         (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));
                   }else 
                        fprintf(file_out,"     <icon>49</icon>\n");  

                }
            }
        if (strstr(buffer,"Wind"))
            if ((comma = strstr(buffer, ": ")) != NULL){
                comma = comma + 2;
                comma2 = strstr(comma, "force");               
                snprintf(temp_buffer, comma2 - comma, "%s", comma);
#ifdef GLIB
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", choose_hour_weather_icon(hash_for_icons, temp_buffer)); 
#endif
#ifdef QT 
                    fprintf(file_out, "     <wind_direction>%s</wind_direction>\n", choose_hour_weather_icon(hash_for_icons, temp_buffer).toStdString().c_str());
#endif
                if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer)){

                    fprintf(file_out, "     <wind_direction>%s</wind_direction>\n", (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));
                }
                comma = strstr(comma2, " ");               
                comma++;
                switch (atoi(comma)){
                    case 0: wind_speed = 0.278; break;
                    case 1: wind_speed = 4 * 0.278; break; 
                    case 2: wind_speed = 9 * 0.278; break; 
                    case 3: wind_speed = 15 * 0.278; break; 
                    case 4: wind_speed = 25 * 0.278; break; 
                    case 5: wind_speed = 35 * 0.278; break; 
                    case 6: wind_speed = 45 * 0.278; break; 
                    case 7: wind_speed = 57 * 0.278; break; 
                    case 8: wind_speed = 70 * 0.278; break; 
                    case 9: wind_speed = 81 * 0.278; break; 
                    case 10: wind_speed = 95 * 0.278; break; 
                    case 11: wind_speed = 110 * 0.278; break; 
                    case 12: wind_speed = 120 * 0.278; break;
                }
                fprintf(file_out, "     <wind_speed>%f</wind_speed>\n", wind_speed); 
            }
        if (strstr(buffer,"Weather"))
            if ((comma = strstr(buffer, ": ")) != NULL){
                comma = comma + 2;
                snprintf(temp_buffer, sizeof(temp_buffer) - 1, "%s", comma);
                fgetpos (file_in, &pos);
                if (fgets(buffer, sizeof(buffer), file_in)){
                    if (strstr(buffer, "Temp Range:")){
                        temp_buffer[strlen(temp_buffer)-1] = 0;
                    }else{
                        temp_buffer[strlen(temp_buffer)-1] = ' ';
                        strcat(temp_buffer, buffer);
                        temp_buffer[strlen(temp_buffer)-1] = 0;
                    }
                }
                fprintf(file_out, "     <description>%s</description>\n", temp_buffer); 
            }
        if (strstr(buffer,"Temp Range"))
            if ((comma = strstr(buffer, ": ")) != NULL){
                comma = comma + 2;
                temperature = atoi (comma);
                fprintf(file_out, "     <temperature_low>%i</temperature_low>\n", temperature);
                comma2 = strstr(buffer, "- ");
                comma2 = comma2 + 2;
                temperature = atoi (comma2);
                fprintf(file_out, "     <temperature_hi>%i</temperature_hi>\n", temperature);
            }
        if (strstr(buffer,"R.H. Range"))
            if ((comma = strstr(buffer, ": ")) != NULL){
                comma = comma + 2;
                humidity1 = atoi (comma);
                comma2 = strstr(buffer, "- ");
                comma2 = comma2 + 2;
                humidity2 = atoi (comma2);
                fprintf(file_out, "     <humidity>%i</humidity>\n", (humidity2 - humidity1)/2 + humidity1);
            }

    }
    fprintf(file_out,"    </period>\n");
    xmlHashFree(hash_for_icons, NULL);
    fclose(file_out);
    fclose(file_in);
    fclose(file_in2);
}

/*******************************************************************************/
void
parse_current_weather(const char *detail_path_data, const char *result_file){

    FILE    *file_out;
    FILE    *file_in;
    char buffer [4096];
    char temp_buffer [1024];
    char *comma = NULL;
    struct tm   tmp_tm = {0};
    time_t      t_start = 0, t_end = 0;
    int   temperature, humidity, icon;
#ifdef GLIB
    GHashTable *hash_for_icons;
#endif
    file_out = fopen(result_file, "a");
    if (!file_out)
        return;
    file_in = fopen(detail_path_data, "r");
    if (!file_in)
        return;

    hash_for_icons = hash_icons_hko_table_create();
    while(fgets(buffer, sizeof(buffer), file_in)){
        if (strstr(buffer,"Bulletin updated"))
            if ((comma = strstr(buffer, "at ")) != NULL){
                comma = comma + 3;
                setlocale(LC_TIME, "POSIX");
                /* 3:02 HKT 28/Apr/2012 */
                strptime((const char*)comma, "%H:%M HKT %d/%b/%Y", &tmp_tm);
                setlocale(LC_TIME, "");

                t_start = timegm(&tmp_tm) - 8*3600;
                fprintf(file_out,"    <period start=\"%li\"", (t_start + 1 - 2*3600));
                /* set end of current time in localtime */
                t_end = t_start + 3600*4 - 1;
                fprintf(file_out," end=\"%li\" current=\"true\" >\n", t_end);
            }
        if (strstr(buffer,"Air Temperature"))
            if ((comma = strstr(buffer, ": ")) != NULL){
                comma = comma + 2;
                temperature = atoi (comma);
                fprintf(file_out,"     <temperature>%i</temperature>\n", temperature); 
            }
        if (strstr(buffer,"Relative Humidity"))
            if ((comma = strstr(buffer, ": ")) != NULL){
                comma = comma + 2;
                humidity = atoi (comma);
                fprintf(file_out,"     <humidity>%i</humidity>\n", humidity);				                
            }
        if (strstr(buffer,"Weather Cartoon")){
            if ((comma = strstr(buffer, "No. ")) != NULL){
                comma = comma + 3;
                icon = atoi (comma);
                snprintf(temp_buffer, sizeof(temp_buffer) - 1, "%i", icon);
#ifdef GLIB
                    fprintf(file_out,"     <icon>%s</icon>\n", choose_hour_weather_icon(hash_for_icons, temp_buffer));
#endif
#ifdef QT 
                    fprintf(file_out, "     <icon>%s</icon>\n", choose_hour_weather_icon(hash_for_icons, temp_buffer).toStdString().c_str());
#endif
               if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer)){
                    fprintf(file_out,"     <icon>%s</icon>\n",  
                     (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));
               }else 
                    fprintf(file_out,"     <icon>49</icon>\n");  


            }
            if ((comma = strstr(buffer, " - ")) != NULL){
                comma = comma + 3;
                snprintf(temp_buffer, sizeof(temp_buffer) - 1, "%s", comma);
                if (strlen(temp_buffer) > 3)
                    temp_buffer[strlen(temp_buffer) - 2] = 0; 
                int l=0;
                while (temp_buffer[l]){
                    temp_buffer[l] = tolower(temp_buffer[l]);
                    l++;
                }
                if (temp_buffer[0])
                    temp_buffer[0] = toupper(temp_buffer[0]);
                fprintf(file_out,"     <description>%s</description>\n", temp_buffer);				                
            }else
                if ((comma = strstr(buffer, " : ")) != NULL){
                    comma = comma + 3;
                    snprintf(temp_buffer, sizeof(temp_buffer) - 1, "%s", comma);
                    if (strlen(temp_buffer) > 3)
                        temp_buffer[strlen(temp_buffer) - 1] = 0; 
                    int l=0;
                    while (temp_buffer[l]){
                        temp_buffer[l] = tolower(temp_buffer[l]);
                        l++;
                    }
                    if (temp_buffer[0])
                        temp_buffer[0] = toupper(temp_buffer[0]);
                    fprintf(file_out,"     <description>%s</description>\n", temp_buffer);				                       
                }
        }
        if (strstr(buffer,"UV Index"))
            if ((comma = strstr(buffer, ": ")) != NULL){
                comma = comma + 2;
                humidity = atoi (comma);
                fprintf(file_out,"     <uv_index>%i</uv_index>\n", humidity);
            }

    }
    fprintf(file_out,"    </period>\n");
    xmlHashFree(hash_for_icons, NULL);
    fclose(file_out);
    fclose(file_in);
}
/*******************************************************************************/
int
convert_station_hkogovhk_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data ){
 
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int    days_number = -1;
    char   buffer[1024],
           buffer2[1024],
           *delimiter = NULL;
    FILE   *file_out;

    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;
    /* prepare station id */
    *buffer = 0;
    *buffer2 = 0;
    snprintf(buffer2, sizeof(buffer2) - 1, "%s", station_id_with_path);
    delimiter = strrchr(buffer2, '/');
    if(delimiter){
        delimiter++; /* delete '/' */
        snprintf(buffer, sizeof(buffer) - 1, "%s", delimiter);
    }
    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", buffer);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");
    fprintf(file_out," <timezone>8</timezone>\n");
    fclose(file_out);
   
    parse_current_weather(detail_path_data, result_file);
    parse_forecast_weather(station_id_with_path, result_file);

    file_out = fopen(result_file, "a");
    if (file_out){
        fprintf(file_out,"</station>");
        fclose(file_out);
     }

    return 0;
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
        doc =  htmlReadFile(station_id_with_path, "UTF-8", 0);
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
             //   if(get_detail_data)
             //       days_number = parse_xml_detail_data(buffer, root_node, data);
             //   else
                days_number = parse_and_write_xml_data(buffer, doc, result_file);
                xmlFreeDoc(doc);
                xmlCleanupParser();
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
                            //parse_and_write_detail_data(buffer, doc, result_file);
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
        }
    }
    else
        return -1;/* file isn't accessability */
    return days_number;
}
/*******************************************************************************/
int
main_hkogovhk(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "hkogovhk <input_file> <output_file> <input_detail_data>\n");
        return -1;
    }
    result = convert_station_hkogovhk_data(argv[1], argv[2], argv[3]);
    //fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
