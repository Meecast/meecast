/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-bom-gov-au-stations-db
 *
 * Copyright (C) 2012-2023 Vlad Vasilyeu
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
/*******************************************************************************/
#define buff_size 2048
#ifdef GLIB
    static GHashTable *data = NULL;
#endif
#ifdef QT
    static QHash<QString, QString> *hash_for_icons;
    static QHash<QString, QString> *hash_for_stations;
#endif
static xmlHashTablePtr hash_for_icons;
static xmlHashTablePtr hash_for_stations;
int au_timezone = 0;
char current_icon[256];
volatile int current_icon_buffer_size = sizeof(current_icon);
char current_title[1024];
/*******************************************************************************/
#ifdef QT
QString
choose_hour_weather_icon( QHash<QString, QString> *hash_for_icons, char *image){
    return  hash_bomgovau_table_find(hash_for_icons, image, FALSE);
}

#endif
#ifdef GLIB
char*
choose_hour_weather_icon(GHashTable *hash_for_icons, gchar *image)
{
    char *result;
    char *source;
    char *tmp_result = NULL;

    source = g_strdup_printf("%s", image);
    tmp_result = (char *)hash_bomgovau_table_find(hash_for_icons, source, FALSE);
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
#endif
/*******************************************************************************/
int
parse_and_write_bom_gov_au_detail_data(const char *station_name, htmlDocPtr doc, const char *result_file){
#ifdef GLIB
    GSList      *forecast = NULL;
    GSList      *tmp = NULL;
    GHashTable  *day = NULL;
    GHashTable *hash_for_stations;
    GHashTable *hash_for_icons;
#endif
    int        size;
    int        i;
    xmlXPathContextPtr xpathCtx; 
    xmlXPathObjectPtr xpathObj = NULL; 
    xmlXPathObjectPtr xpathObj2 = NULL; 
    xmlNodeSetPtr nodes;
    struct tm   tmp_tm = {0};
    struct tm   *tm;
    time_t      t_start = 0, t_end = 0,
                current_time = 0;
    FILE        *file_out;

    file_out = fopen(result_file, "a");
    if (!file_out)
        return -1;

//#ifdef GLIB
//#endif
    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
         return(-1);
    }
    /* Register namespaces from list (if any) */
    xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"html",
                                (const xmlChar*)"http://www.w3.org/1999/xhtml");
    /* Current weather forecast */
    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div[2]/div/div/table/tbody/tr/th/a/text()", xpathCtx);
  
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", " /html/body/div/div/table//tr[1]/th[@colspan='4']/text()");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
    }
    /* Day weather forecast */
    nodes   = xpathObj->nodesetval;
    size = (nodes) ? nodes->nodeNr : 0; 
    
    for(i = 1; i < (size-1) ; ++i) {
       #ifdef GLIB
       if (!strcmp((char*)xpathObj->nodesetval->nodeTab[i]->content, 
                   (char *)hash_bomgovau_table_find(hash_for_stations, (void *)station_name, FALSE))){
       #endif
       #ifdef QT
       if (!strcmp((char*)xpathObj->nodesetval->nodeTab[i]->content, 
                   (char*)hash_bomgovau_table_find(hash_for_stations, station_name, FALSE).toStdString().c_str())){
       #endif

       if ((char*)xmlHashLookup(hash_for_stations, (const xmlChar *)station_name) &&
            !strcmp((char*)xpathObj->nodesetval->nodeTab[i]->content, 
                   (char *)xmlHashLookup(hash_for_stations, (const xmlChar *)station_name))){

            xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/table/tbody/tr/td[contains (@headers, '-datetime')]/text()", xpathCtx);
            current_time = time(NULL);
            tm = localtime(&current_time);
            setlocale(LC_TIME, "POSIX");
            strptime((const char*)xpathObj2->nodesetval->nodeTab[i]->content, "%d/%I:%M %p", &tmp_tm);
            setlocale(LC_TIME, "");
            /* set begin of day in localtime */
            tmp_tm.tm_year = tm->tm_year;
            tmp_tm.tm_mon = tm->tm_mon;  
            t_start = mktime(&tmp_tm)-2*3600;// - au_timezone*3600;
            t_end = t_start + 4*3600;
            fprintf(file_out,"    <period start=\"%li\" current=\"true\"", t_start);
            fprintf(file_out," end=\"%li\">\n", t_end); 
            xmlFree(xpathObj2);
            xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/table/tbody/tr/td[contains (@headers, '-tmp')]/text()", xpathCtx);
            fprintf(file_out,"     <temperature>%s</temperature>\n", (const char*)xpathObj2->nodesetval->nodeTab[i]->content);				                
            xmlFree(xpathObj2);
            xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/table/tbody/tr/td[contains (@headers, '-relhum')]/text()", xpathCtx);
            fprintf(file_out,"     <humidity>%s</humidity>\n", (const char*)xpathObj2->nodesetval->nodeTab[i]->content);				                
            xmlFree(xpathObj2);
            xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/table/tbody/tr/td[contains (@headers, '-wind-dir')]/text()", xpathCtx);
            fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",  xpathObj2->nodesetval->nodeTab[i]->content);
            xmlFree(xpathObj2);
            xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/table/tbody/tr/td[contains (@headers, '-wind-spd-kmh')]/text()", xpathCtx);
            fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", ((atoi((const char*)xpathObj2->nodesetval->nodeTab[i]->content) * 1000)/3600));
            xmlFree(xpathObj2);
            xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/table/tbody/tr/td[contains (@headers, '-wind-gust-kmh')]/text()", xpathCtx);
            fprintf(file_out,"     <wind_gust>%i</wind_gust>\n", ((atoi((const char*)xpathObj2->nodesetval->nodeTab[i]->content)*1000)/3600));
            xmlFree(xpathObj2);
            xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/table/tbody/tr/td[contains (@headers, '-pressure')]/text()", xpathCtx);
            if(xmlXPathNodeSetIsEmpty(xpathObj2->nodesetval)){
                xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/table/tbody/tr/td[contains (@headers, '-press')]/text()", xpathCtx);
            }
            if(!xmlXPathNodeSetIsEmpty(xpathObj2->nodesetval)){
                fprintf(file_out,"     <pressure>%s</pressure>\n", xpathObj2->nodesetval->nodeTab[i]->content);
            }
            fprintf(file_out, "     <description>%s</description>\n", current_title);
            fprintf(file_out, "     <icon>%s</icon>\n", current_icon);
            fprintf(file_out,"    </period>\n");
            xmlFree(xpathObj2);
            break;
       }
    }

    fclose(file_out);
    return 1; 
}


/*******************************************************************************/
int
parse_and_write_xml_data(const char *station_id, const char *station_name, htmlDocPtr doc, const char *result_file){
    xmlNode     *cur_node = NULL,
                *cur_node0 = NULL,
                *child_node = NULL,
                *child_node2 = NULL;
    xmlChar     *temp_xml_string = NULL;
    int         count_day = 0;
    char        id_station[1024],
                short_text[1024],
                ppcp[128],
                buffer[1024];
    int         i;
    int         temp_hi, temp_low;
    char        icon[256];
    int         check_timezone = false;
    char        temp_buffer[buff_size];
    struct tm   tmp_tm = {0};
#ifdef GLIB
    GSList      *forecast = NULL;
    GHashTable  *location = NULL,
                *current = NULL,
                *day = NULL;
#endif
    xmlNode *root_node = NULL;
    FILE        *file_out;
    time_t      utc_time_start;
    time_t      utc_time_end;
#ifdef GLIB
    GHashTable *hash_for_icons;
#endif
    int index = INT_MAX;
    if(!doc)
        return -1;

    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;
    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", station_id);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");

    memset(current_icon, 0, sizeof(current_icon));
    sprintf(current_icon, "%s", "48"); 
    memset(current_title, 0, sizeof(current_title));
    root_node = xmlDocGetRootElement(doc);

    for(cur_node0 = root_node->children; cur_node0; cur_node0 = cur_node0->next){
        if( cur_node0->type == XML_ELEMENT_NODE ){
            if (!xmlStrcmp(cur_node0->name, (const xmlChar *) "forecast" ) ){
                for(cur_node = cur_node0->children; cur_node; cur_node = cur_node->next){
                    if( cur_node->type == XML_ELEMENT_NODE ){
                        /* get weather data */
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
                                                if (strlen(temp_buffer) > 6){
                                                    /* create string for timezone */
                                                    for (i=strlen(temp_buffer) - 6; i < strlen(temp_buffer) -3; i++){
                                                        buffer[i - strlen(temp_buffer) + 6] = temp_buffer[i]; 
                                                    }
                                                    fprintf(file_out,"  <timezone>%s</timezone>\n", buffer);
                                                    au_timezone = atoi(buffer);
                                                }else{
                                                    /* Set default timezone +10 for Australia */
                                                    fprintf(file_out,"  <timezone>+10</timezone>\n");
                                                    au_timezone = 10;
                                                }
                                                check_timezone = true;
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
                                                /* fprintf(stderr,"Index %i\n", index); */
                                            }

                                            for (child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){

                                                if (child_node2->type == XML_ELEMENT_NODE ){
                                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *) "element")){                                               
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "air_temperature_minimum" ))
                                                            temp_low = atoi((char *)xmlNodeGetContent(child_node2));
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "air_temperature_maximum" ))
                                                            temp_hi = atoi((char *)xmlNodeGetContent(child_node2));
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "forecast_icon_code" )){
#ifdef GLIB                                                         
                                                            snprintf(icon, sizeof(icon) - 1, "%s", choose_hour_weather_icon(hash_for_icons, (char *)xmlNodeGetContent(child_node2))); 
#endif
#ifdef QT
                                                            snprintf(icon, sizeof(icon) - 1, "%s", (char*)choose_hour_weather_icon(hash_for_icons, (char *)xmlNodeGetContent(child_node2)).toStdString().c_str()); 
#endif
                                                           if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)xmlNodeGetContent(child_node2))){
                                                                snprintf(icon, sizeof(icon) - 1, "%s", (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)xmlNodeGetContent(child_node2))); 
                                                           }else 
                                                                snprintf(icon, sizeof(icon) - 1, "49");
                                                        }

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
                                                snprintf(current_icon, current_icon_buffer_size, "%s", icon);
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
                    }
                }
            }          
        }
    }
#ifdef GLIB
    g_hash_table_destroy(hash_for_icons);
#endif
    fclose(file_out);
    return count_day;
 
}

/*******************************************************************************/
int
convert_station_bomgovau_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data ){
 
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int    days_number = -1;
    char   buffer[1024],
            buffer2[1024],
            *delimiter = NULL;
    FILE    *file_out;

    if(!station_id_with_path)
        return -1;

    hash_for_icons = hash_icons_bomgovau_table_create();
    hash_for_stations = hash_stations_bomgovau_table_create();

/* check for new file, if it exist, than rename it */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s.new", station_id_with_path);
    if(!access(buffer, R_OK))
        rename(buffer, station_id_with_path);
    /* check file accessability */
    if(!access(station_id_with_path, R_OK)){
        /* check that the file containe valid data */
        doc =  xmlReadFile(station_id_with_path, "UTF-8", 0);
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
            delimiter = (char*)strrchr(station_id_with_path, '/');
            if(delimiter){
                delimiter++; /* delete '/' */
                snprintf(buffer, sizeof(buffer) - 1, "%s", delimiter);
                delimiter = strrchr(buffer, '_');
                if(!delimiter){
                    xmlFreeDoc(doc);
                    xmlCleanupParser();
                    fprintf(stderr, "Not found delimeter '_' in file name\n");
                    return -1;
                }
                *delimiter = 0;
                delimiter++; /* delete '_' */
                snprintf(buffer2, sizeof(buffer2) - 1, "%s", delimiter);
                delimiter = strrchr(buffer2, '.');
                if(!delimiter){
                    xmlFreeDoc(doc);
                    xmlCleanupParser();
                    return -1;
                }
                
                *delimiter = 0;
                days_number = parse_and_write_xml_data(buffer, buffer2, doc, result_file);
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
                            parse_and_write_bom_gov_au_detail_data(buffer2, doc, result_file);
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
main_bom_gov_au(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "bomgovau <input_file> <output_file> <input_detail_data>\n");
        return -1;
    }
    result = convert_station_bomgovau_data(argv[1], argv[2], argv[3]);
    //fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
