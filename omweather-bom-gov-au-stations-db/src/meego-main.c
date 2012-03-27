/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-bom-gov-au-stations-db
 *
 * Copyright (C) 2012 Vlad Vasilyeu
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
static GHashTable *data = NULL;
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
    tmp_result = hash_bomgovau_table_find(hash_for_icons, source, FALSE);
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
gint
parse_and_write_detail_data(const gchar *station_id, htmlDocPtr doc, const gchar *result_file){
    gchar       buff[256],
                buffer[buff_size],
                current_temperature[20],
                current_icon[10],
                current_title[1024],
                current_pressure[15],
                current_humidity[15],
                current_wind_direction[15],
                current_wind_speed[15];
    gchar       temp_buffer[buff_size];
    GSList      *forecast = NULL;
    GSList      *tmp = NULL;
    GHashTable  *day = NULL;
    gboolean    flag;
    gboolean    night_flag;
    gint        size;
    gint        i, j;
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
    gchar       *temp_char2;
    gint        pressure; 
    gint        speed;
    gint        ppcp;

    gchar       *image = NULL;
    double      time_diff = 0;
    time_t      loc_time;
    time_t      utc_time;
    gint        location_timezone = 0;
    gboolean timezone_flag = FALSE;
    gboolean sunrise_flag = FALSE;
    struct tm   tmp_tm_loc = {0};
    struct tm   tmp_tm = {0};
    struct tm   current_tm = {0};
    struct tm   tm_l = {0};
    struct tm   tmp_tm2 = {0};
    struct tm   *tm;
    time_t      t_start = 0, t_end = 0,
                t_sunrise = 0, t_sunset = 0,
                current_time = 0;
    FILE        *file_out;
    int index = 1;

    file_out = fopen(result_file, "a");
    if (!file_out)
        return -1;

    hash_for_icons = hash_icons_bomgovau_table_create();
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
    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/table//tr[1]/th[@colspan='4']/text()", xpathCtx);
  
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", " /html/body/div/div/table//tr[1]/th[@colspan='4']/text()");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
    }
    temp_char = strchr(xpathObj->nodesetval->nodeTab[0]->content, ' ');
    if (temp_char == NULL || strlen(temp_char)<2)
        return -1;
    temp_char = temp_char + 1;
    temp_char = strchr(temp_char, ' ');
    if (temp_char != NULL){
        for (j=0; j<strlen(temp_char)-1; j++){
            if (temp_char[j] == ' ' || temp_char[j] == '\n')
                continue; 
            else{
                temp_char = temp_char + j;
                break;
            }
        }
    }


    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/table//tr[3]/td[@class='in']/text()", xpathCtx);
    if (xpathObj && xpathObj->nodesetval->nodeTab[0]->content){
        snprintf(buffer, sizeof(buffer)-1,"%s %s", temp_char, xpathObj->nodesetval->nodeTab[0]->content);
        current_time = time(NULL);
        tm = localtime(&current_time);

        setlocale(LC_TIME, "POSIX");
        strptime((const char*)buffer, "%d/%m %H:%M", &tmp_tm);
        setlocale(LC_TIME, "");
        /* set begin of day in localtime */
        tmp_tm.tm_year = tm->tm_year;

        t_start = mktime(&tmp_tm);
        fprintf(file_out,"    <period start=\"%li\"", (t_start +1));
        /* set end of current time in localtime */
        t_end = t_start + 3600*4 - 1;
        fprintf(file_out," end=\"%li\" current=\"true\" >\n", t_end);

    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/table//tr[3]/td[2]/span/text()", xpathCtx);
    /* added temperature */
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[0] && xpathObj->nodesetval->nodeTab[0]->content){
        /* fprintf (stderr, "temperature %s\n", xpathObj->nodesetval->nodeTab[0]->content); */
        snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj->nodesetval->nodeTab[0]->content);
        memset(temp_buffer, 0, sizeof(temp_buffer));
        for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
            if (buffer[j] == '&')
               break;
            if ((uint)buffer[j] == 226 ||  buffer[j] == '-' || 
                (buffer[j]>='0' && buffer[j]<='9')){
                if ((uint)buffer[j] == 226)
                   sprintf(temp_buffer,"%s-",temp_buffer);
                else
                   sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
            }
        }
        /* fprintf(stderr, "     <temperature>%s</temperature>\n", temp_buffer); */
        fprintf(file_out,"     <temperature>%s</temperature>\n", temp_buffer); 
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/table//tr[3]/td[3]/img/@alt", xpathCtx);
    /* added wind direction */
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[0] && 
        xpathObj->nodesetval->nodeTab[0]->children->content){
       /* fprintf(stderr, "Wind  direction  .%s.  \n", xpathObj->nodesetval->nodeTab[0]->children->content); */
       if (strlen(xpathObj->nodesetval->nodeTab[0]->children->content)>0)
            fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",  xpathObj->nodesetval->nodeTab[0]->children->content);
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/table//tr[3]/td[3]/text()", xpathCtx);
    /* added wind speed */
     if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
         xpathObj->nodesetval->nodeTab[0] && xpathObj->nodesetval->nodeTab[0]->content){
        /* fprintf(stderr, "Wind  direction  %s  \n", xpathObj4->nodesetval->nodeTab[i]->children->content);  */
        fprintf(file_out,"     <wind_speed>%s</wind_speed>\n",  
                               xpathObj->nodesetval->nodeTab[0]->content);
     }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/table//tr[3]/td[4]/img/@src", xpathCtx);
    /* added icon */
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[0] && 
        xpathObj->nodesetval->nodeTab[0]->children->content){
       temp_char = strrchr((char*)xpathObj->nodesetval->nodeTab[0]->children->content, '/');
       temp_char ++;
       /*  fprintf (stderr, "icon %s %s \n", xpathObj6->nodesetval->nodeTab[i]->children->content, choose_hour_weather_icon(hash_for_icons, temp_char)); */ 
       fprintf(file_out,"     <icon>%s</icon>\n",  
                              choose_hour_weather_icon(hash_for_icons, temp_char));
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);
    
    index = 1;
    snprintf(buffer, sizeof(buffer)-1,"/html/body/div/div/table//tr[4]/td/text()[%i]", index);
    xpathObj = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    /* added text */
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[0] && xpathObj->nodesetval->nodeTab[0]->content){
        if (strstr(xpathObj->nodesetval->nodeTab[0]->content, "Feels Like:") == NULL){
            /* fprintf (stderr, "description %s\n", xpathObj7->nodesetval->nodeTab[i]->content); */

            snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj->nodesetval->nodeTab[0]->content);
            memset(temp_buffer, 0, sizeof(temp_buffer));
            for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
               if (buffer[j] == 13 || buffer[j] == 10)
                    continue;
               sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
            }
            index ++;
            fprintf(file_out,"     <description>%s</description>\n", temp_buffer);
        }
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);
    snprintf(buffer, sizeof(buffer)-1,"/html/body/div/div/table//tr[4]/td/text()[%i]", index);
    xpathObj = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);

    temp_char = strstr(xpathObj->nodesetval->nodeTab[0]->content, "Feels Like:");
    if (temp_char != NULL){
        temp_char = strchr(temp_char, ':');
        temp_char = temp_char + 2;
        snprintf(buffer, sizeof(buffer)-1,"%s", temp_char);
        memset(temp_buffer, 0, sizeof(temp_buffer));
        for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
            if (buffer[j] == '&')
               break;
            if ((uint)buffer[j] == 226 ||  buffer[j] == '-' || 
                (buffer[j]>='0' && buffer[j]<='9')){
                if ((uint)buffer[j] == 226)
                   sprintf(temp_buffer,"%s-",temp_buffer);
                else
                   sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
            }
        }
        /* fprintf(stderr, "     <temperature>%s</temperature>\n", temp_buffer); */
        fprintf(file_out,"     <flike>%s</flike>\n", temp_buffer); 
    }
    fprintf(file_out,"    </period>\n");



    /* To DO sunrise and sunset */

    xpathObj = xmlXPathEvalExpression("/html/body/div/div/table//tr[th]/*/text()", xpathCtx);
    /* Day weather forecast */
    nodes   = xpathObj->nodesetval;
    size = (nodes) ? nodes->nodeNr : 0; 
    
    for(i = 1; i < (size-1) ; ++i) {
       fprintf(stderr,"ssssssssssssss %s\n", xpathObj->nodesetval->nodeTab[i]->content);
       
       snprintf(buffer, sizeof(buffer)-1,"/html/body/div/div/table//tr[th][%i]/following-sibling::*", i);
       xpathObj2 = xmlXPathEvalExpression(buffer, xpathCtx);
       nodes   = xpathObj2->nodesetval;
       fprintf(stderr,"Nodes %i\n", nodes->nodeNr);
       for (j = 0; j <(nodes->nodeNr); ++j){
            xpathObj3 = xpathObj2->nodesetval->nodeTab[j]->children; 
         //   fprintf(stderr, "ssssssssssssssss %s\n", xpathObj3->nodesetval->nodeTab[0]->content);
       }
#if 0
        current_time = time(NULL);
        tm = localtime(&current_time);

        setlocale(LC_TIME, "POSIX");
        strptime((const char*)buffer, "%A %d/%m %H:%M", &tmp_tm);
        setlocale(LC_TIME, "");
        /* set begin of day in localtime */
        tmp_tm.tm_year = tm->tm_year;

        t_start = mktime(&tmp_tm);
        fprintf(file_out,"    <period start=\"%li\"", (t_start +1));
        /* set end of current time in localtime */
        t_end = t_start + 3600*4 - 1;
        fprintf(file_out," end=\"%li\" current=\"true\" >\n", t_end);
#endif
    }
    fclose(file_out);
    return 1; 
    /* Day weather forecast */
    nodes   = xpathObj->nodesetval;
    size = (nodes) ? nodes->nodeNr : 0;
    if (size > 10)
        size = 10;
    /* fprintf(stderr, "SIZE!!!!!!!!!!!!!!: %i\n", size); */
    xpathObj2 = xmlXPathEvalExpression("/html/body/div/div/table//tr/td[@class='in']//span[1]/text()", xpathCtx);
    xpathObj3 = xmlXPathEvalExpression("/html/body/div/div/table//tr/td[@class='in']//span[2]/text()", xpathCtx);
    xpathObj4 = xmlXPathEvalExpression("/html/body/div/div/table//tr/td[2]/img/@alt", xpathCtx);
    xpathObj5 = xmlXPathEvalExpression("/html/body/div/div/table//tr/td[2]/text()", xpathCtx);
    xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/table//tr/td[3]/img/@src", xpathCtx);
    xpathObj7 = xmlXPathEvalExpression("/html/body/div/div/table//tr/td[@class='in2']/text()[1]", xpathCtx);
    xpathObj8 = xmlXPathEvalExpression("/html/body/div/div/table//tr/td[@class='in2']/text()[2]", xpathCtx);
  
    /* fprintf(stderr, "Result (%d nodes):\n", size); */
    for(i = 0; i < size; ++i) {
         day = NULL;

         /* Take time: */
         if (!nodes->nodeTab[i]->children->content)
             continue;
         temp_char = strstr(nodes->nodeTab[i]->children->content, " ");
         int j = 0;
         if (temp_char != NULL){
             for (j=0; j<strlen(temp_char)-1; j++){
                 if (temp_char[j] == ' ' || temp_char[j] == '\n')
                     continue; 
                 else{
                     temp_char = temp_char + j;
                     break;
                 }
             }
         }
         current_time = time(NULL);
         tm = localtime(&current_time);

         setlocale(LC_TIME, "POSIX");
         strptime((const char*)temp_char, "%b %d", &tmp_tm);
         setlocale(LC_TIME, "");
         /* set begin of day in localtime */
         tmp_tm.tm_year = tm->tm_year;
         tmp_tm.tm_hour = 0; tmp_tm.tm_min = 0; tmp_tm.tm_sec = 0;

         t_start = mktime(&tmp_tm);
         fprintf(file_out,"    <period start=\"%li\"", (t_start +1));
         /* set end of day in localtime */
         t_end = t_start + 3600*24 - 1;
         fprintf(file_out," end=\"%li\">\n", t_end);
     
           
         /* added hi temperature */
         if (xpathObj2 && !xmlXPathNodeSetIsEmpty(xpathObj2->nodesetval) &&
             xpathObj2->nodesetval->nodeTab[i] && xpathObj2->nodesetval->nodeTab[i]->content){
             fprintf (stderr, "temperature %s\n", xpathObj2->nodesetval->nodeTab[i]->content); 
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj2->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if (buffer[j] == '&')
                    break;
                 if ((uint)buffer[j] == 226 ||  buffer[j] == '-' || 
                     (buffer[j]>='0' && buffer[j]<='9')){
                     if ((uint)buffer[j] == 226)
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }
             /* fprintf(stderr, "     <temperature>%s</temperature>\n", temp_buffer); */
             fprintf(file_out,"     <temperature_hi>%s</temperature_hi>\n", temp_buffer); 
         }
         /* added lo temperature */
         if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) &&
             xpathObj3->nodesetval->nodeTab[i] && xpathObj3->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "temperature %s\n", xpathObj3->nodesetval->nodeTab[i]->content); */
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj3->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if (buffer[j] == '&')
                    break;
                 if ((uint)buffer[j] == 226 ||  buffer[j] == '-' ||
                     (buffer[j]>='0' && buffer[j]<='9')){
                     if ((uint)buffer[j] == 226)
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }
             /* fprintf(stderr, "     <temperature>%s</temperature>\n", temp_buffer); */
             fprintf(file_out,"     <temperature_low>%s</temperature_low>\n", temp_buffer); 
         }
         /* added wind direction */
         if (xpathObj4 && !xmlXPathNodeSetIsEmpty(xpathObj4->nodesetval) &&
             xpathObj4->nodesetval->nodeTab[i] && 
             xpathObj4->nodesetval->nodeTab[i]->children->content){
            /* fprintf(stderr, "Wind  direction  %s  \n", xpathObj4->nodesetval->nodeTab[i]->children->content);  */
            if (xpathObj4->nodesetval->nodeTab[i]->children->content != "")
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",  xpathObj4->nodesetval->nodeTab[i]->children->content);
         }

        /* added wind speed */
         if (xpathObj5 && !xmlXPathNodeSetIsEmpty(xpathObj5->nodesetval) &&
             xpathObj5->nodesetval->nodeTab[i] && xpathObj5->nodesetval->nodeTab[i]->content){
            /* fprintf(stderr, "Wind  direction  %s  \n", xpathObj4->nodesetval->nodeTab[i]->children->content);  */
            fprintf(file_out,"     <wind_speed>%s</wind_speed>\n",  
                                   xpathObj5->nodesetval->nodeTab[i]->content);
         }

         /* added icon */
         if (xpathObj6 && !xmlXPathNodeSetIsEmpty(xpathObj6->nodesetval) &&
             xpathObj6->nodesetval->nodeTab[i] && 
             xpathObj6->nodesetval->nodeTab[i]->children->content){
            temp_char = strrchr((char*)xpathObj6->nodesetval->nodeTab[i]->children->content, '/');
            temp_char ++;
            /*  fprintf (stderr, "icon %s %s \n", xpathObj6->nodesetval->nodeTab[i]->children->content, choose_hour_weather_icon(hash_for_icons, temp_char)); */ 
            fprintf(file_out,"     <icon>%s</icon>\n",  
                                   choose_hour_weather_icon(hash_for_icons, temp_char));
         }
         /* added text */
         if (xpathObj7 && !xmlXPathNodeSetIsEmpty(xpathObj7->nodesetval) &&
             xpathObj7->nodesetval->nodeTab[i] && xpathObj7->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "description %s\n", xpathObj7->nodesetval->nodeTab[i]->content); */
             fprintf(file_out,"     <description>%s</description>\n", 
                                    xpathObj7->nodesetval->nodeTab[i]->content);
         }
         /* added ppcp */
         if (xpathObj8 && !xmlXPathNodeSetIsEmpty(xpathObj8->nodesetval) &&
             xpathObj8->nodesetval->nodeTab[i] && xpathObj8->nodesetval->nodeTab[i]->content){
             snprintf(buffer, sizeof(buffer) - 1, "%s", xpathObj8->nodesetval->nodeTab[i]->content); 
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < strlen(buffer)); j++ ){
                 if (buffer[j] == '%')
                    break;
                 if ( (buffer[j]>='0' && buffer[j]<='9')){
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }
             fprintf(file_out,"     <ppcp>%s</ppcp>\n", temp_buffer);
         }
      fprintf(file_out,"    </period>\n");

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
  
  /* Clean */
  g_hash_table_destroy(hash_for_icons);

  fclose(file_out);

  return size;
}


/*******************************************************************************/
gint
parse_and_write_xml_data(const gchar *station_id, const gchar *station_name, htmlDocPtr doc, const gchar *result_file){
    xmlNode     *cur_node = NULL,
                *cur_node0 = NULL,
                *child_node = NULL,
                *child_node2 = NULL,
                *child_node3 = NULL,
                *child_node4 = NULL;
    xmlChar     *temp_xml_string = NULL;
    xmlChar     *part_of_day = NULL;
    gint        store2day = 0,
                count_day = 0;
    gchar       id_station[1024],
                short_text[1024],
                ppcp[128],
                buffer[1024],
                buff[256];
    int         i;
    int         temp_hi, temp_low;
    gchar       icon[256];
    int         check_timezone = FALSE;
    gchar       temp_buffer[buff_size];
    struct tm   tmp_tm = {0};
    GSList      *forecast = NULL;
    GHashTable  *location = NULL,
                *current = NULL,
                *day = NULL;
    xmlNode *root_node = NULL;
    FILE        *file_out;
    time_t      utc_time_start;
    time_t      utc_time_end;
    GHashTable *hash_for_icons;

    if(!doc)
        return -1;

    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;

    hash_for_icons = hash_icons_bomgovau_table_create();
    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", station_id);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");

    root_node = xmlDocGetRootElement(doc);

    for(cur_node0 = root_node->children; cur_node0; cur_node0 = cur_node0->next){
        if( cur_node0->type == XML_ELEMENT_NODE ){
            if (!xmlStrcmp(cur_node0->name, (const xmlChar *) "forecast" ) ){
                for(cur_node = cur_node0->children; cur_node; cur_node = cur_node->next){
                    if( cur_node->type == XML_ELEMENT_NODE ){
                        fprintf(stderr,"Node %s\n", cur_node->name);
                        /* get weather data */
                        if(!xmlStrcmp(cur_node->name, (const xmlChar *) "area")){
                            if(xmlGetProp(cur_node, (const xmlChar*)"type") &&
                               !xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"type"), 
                                                               (const xmlChar *) "location" )){
                                temp_xml_string = xmlGetProp(cur_node, (const xmlChar*)"description");
                                snprintf(id_station, sizeof(id_station) - 1,
                                            "%s", temp_xml_string);
                                fprintf(stderr,"Station %s\n", id_station);
                                xmlFree(temp_xml_string);
                                /* If station in xml not station in config file exit */
                                if(strcmp(id_station, station_name))
                                    continue;
                                for (child_node = cur_node->children; child_node; child_node = child_node->next){
                                    if (child_node->type == XML_ELEMENT_NODE ){
                                        fprintf(stderr,"Children Node %s\n", child_node->name);
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
                                                check_timezone = TRUE;
                                            }
                                            /* get start time for period */
                                            if (xmlGetProp(child_node, (const xmlChar*)"start-time-utc") != NULL){
                                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s",
                                                                      xmlGetProp(child_node, (const xmlChar*)"start-time-utc"));
                                                strptime(temp_buffer, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                                                utc_time_start = mktime(&tmp_tm);
                                            }
                                            /* get end time for period */
                                            if (xmlGetProp(child_node, (const xmlChar*)"end-time-utc") != NULL){
                                                snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s",
                                                                      xmlGetProp(child_node, (const xmlChar*)"end-time-utc"));
                                                strptime(temp_buffer, "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                                                utc_time_end = mktime(&tmp_tm);
                                            }
                                            for (child_node2 = child_node->children; child_node2; child_node2 = child_node2->next){

                                                if (child_node2->type == XML_ELEMENT_NODE ){
                                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *) "element")){                                               
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "air_temperature_minimum" ))
                                                            temp_low = atoi(xmlNodeGetContent(child_node2));
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "air_temperature_maximum" ))
                                                            temp_hi = atoi(xmlNodeGetContent(child_node2));
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "forecast_icon_code" ))

                                                            snprintf(icon, sizeof(icon) - 1, "%s", choose_hour_weather_icon(hash_for_icons, xmlNodeGetContent(child_node2))); 
                                                        
                                                    }
                                                    if(!xmlStrcmp(child_node2->name, (const xmlChar *) "text")){                           
                                                        if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "precis" )){
                                                            snprintf(short_text, sizeof(short_text)-1,"%s",
                                                                     xmlNodeGetContent(child_node2));
                                                            if (strlen (short_text) >1)
                                                                short_text[strlen(short_text) - 1] = 0;

                                                        }                                                                                                          if(!xmlStrcmp(xmlGetProp(child_node2, (const xmlChar*)"type"), (const xmlChar *) "probability_of_precipitation")){
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
                                        if (strlen(icon)>0)
                                            fprintf(file_out, "     <icon>%s</icon>\n", icon);
                                        if (strlen (short_text)>0)
                                            fprintf(file_out, "     <description>%s</description>\n", short_text);
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

    g_hash_table_destroy(hash_for_icons);
    fclose(file_out);
    return count_day;
 
}

/*******************************************************************************/

convert_station_bomgovau_data(const gchar *station_id_with_path, const gchar *result_file, const gchar *detail_path_data ){
 
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    gint    days_number = -1;
    gchar   buffer[1024],
            buffer2[1024],
            *delimiter = NULL;
    FILE    *file_out;

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
        doc =  xmlReadFile(station_id_with_path, "UTF-8", 0);
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
            delimiter = strrchr(station_id_with_path, '/');
            if(delimiter){
                delimiter++; /* delete '/' */
                snprintf(buffer, sizeof(buffer) - 1, "%s", delimiter);
                delimiter = strrchr(buffer, '_');
                if(!delimiter){
                    xmlFreeDoc(doc);
                    xmlCleanupParser();
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
                fprintf(stderr,"Buf %s buf2 %s\n", buffer, buffer2);
                days_number = parse_and_write_xml_data(buffer, buffer2, doc, result_file);
                xmlFreeDoc(doc);
                xmlCleanupParser();
                if(!access(detail_path_data, R_OK)){
                     doc =  xmlReadFile(detail_path_data, "UTF-8", 0);
                    if(doc){
                        root_node = NULL;
                        root_node = xmlDocGetRootElement(doc);
                        if(!root_node || ( root_node->type == XML_ELEMENT_NODE &&
                                strstr((char*)root_node->name, "err"))){
                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                        }
                        else{
                            parse_and_write_detail_data(buffer, doc, result_file);
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
main(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "bomgovau <input_file> <output_file> <input_detail_data>\n");
        return -1;
    }
    result = convert_station_bomgovau_data(argv[1], argv[2], argv[3]);
    //fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
