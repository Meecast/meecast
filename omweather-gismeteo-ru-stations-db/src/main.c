/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-gismeteo-ru-stations-db
 *
 * Copyright (C) 2009-2012 Vlad Vasilyeu
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
#define buff_size 2048
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
        doc =  htmlReadFile(station_id_with_path, "UTF-8", HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
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
struct tm
get_date_for_current_weather(gchar *temp_string){
    gchar buffer[512];
    gchar temp_buffer[256];
    struct tm   tmp_tm = {0};
    gchar *temp_point;
    gchar *temp_char;

    memset(buffer, 0, sizeof(buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    temp_point = strchr(temp_string,' ');
    if (!temp_point)
        return tmp_tm;
    snprintf(buffer, strlen(temp_string) - strlen(temp_point+1),"%s", temp_string);
    snprintf(temp_buffer, strlen(temp_point) - strlen(strchr(temp_point + 1,' ')),"%s", temp_point+1);
    if (!strcoll(temp_buffer, "января"))
        strcat(buffer," Jan");
    if (!strcoll(temp_buffer, "фев"))
        strcat(buffer," Feb");
    if (!strcoll(temp_buffer, "марта"))
        strcat(buffer," Mar");
    if (!strcoll(temp_buffer, "апреля"))
        strcat(buffer," Apr");
    if (!strcoll(temp_buffer, "мая"))
        strcat(buffer," May");
    if (!strcoll(temp_buffer, "июня"))
        strcat(buffer," Jun");
    if (!strcoll(temp_buffer, "июля"))
        strcat(buffer," Jul");
    if (!strcoll(temp_buffer, "августа"))
        strcat(buffer," Aug");
    if (!strcoll(temp_buffer, "сентября"))
        strcat(buffer," Sep");
    if (!strcoll(temp_buffer, "октября"))
        strcat(buffer," Oct");
    if (!strcoll(temp_buffer, "ноября"))
        strcat(buffer," Nov");
    if (!strcoll(temp_buffer, "декабря"))
        strcat(buffer," Dec");
    temp_char = strchr(temp_point, ' ');
    temp_char ++;
    temp_char = strchr(temp_char, ' ');
    sprintf(buffer, "%s%s", buffer, temp_char);
    setlocale(LC_TIME, "POSIX");
    strptime(buffer, "%d %b %Y %R", &tmp_tm);
    setlocale(LC_TIME, "");
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
    strptime(buff, "%d %b %Y %H:%M", &tmp_tm);
    /* fprintf(stderr, "\ntmp_tm hour %d\n", (&tmp_tm)->tm_hour); */
    return tmp_tm;
}
/*******************************************************************************/
gint
parse_xml_data(const gchar *station_id, htmlDocPtr doc, GHashTable *data){

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
    xmlXPathObjectPtr xpathObj10 = NULL; 
    xmlNodeSetPtr nodes;
    gchar       *temp_char;
    gint        pressure; 
    gint        speed;

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


    GHashTable *current_weather = NULL;
    GHashTable *location = NULL;

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

   /* Current data */
   xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div//span[@class='icon date']/text()", xpathCtx);

   if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      current_tm = get_date_for_current_weather((char*)xpathObj->nodesetval->nodeTab[0]->content);
   }
   if (xpathObj)
      xmlXPathFreeObject(xpathObj);

  memset(current_temperature, 0, sizeof(current_temperature));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)" /html/body/div/div/div/div/div//div/dd[@class='value m_temp c']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
        snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj->nodesetval->nodeTab[0]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if ((char)buffer[j] == -30 || buffer[j] == '-' || (buffer[j]>='0' && buffer[j]<='9')) {
                     if ((char)buffer[j] == -30)
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }
        snprintf(current_temperature, sizeof(current_temperature)-1,"%s", temp_buffer);
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  memset(current_icon, 0, sizeof(current_icon));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div//dt[@class='png']/@style", xpathCtx);
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
        snprintf(current_icon, sizeof(current_icon)-1,"%s", choose_hour_weather_icon(hash_for_icons, temp_buffer));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  memset(current_title, 0, sizeof(current_title));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div//dd/table/tr/td/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
    snprintf(current_title, sizeof(current_title)-1,"%s", hash_gismeteo_table_find(hash_for_translate, xpathObj->nodesetval->nodeTab[0]->content, FALSE));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);
  
  memset(current_pressure, 0, sizeof(current_pressure));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div//div[@class='wicon barp']/dd[@class='value m_press torr']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      if (strlen(xpathObj->nodesetval->nodeTab[0]->content) > 0){
          pressure = atoi(xpathObj->nodesetval->nodeTab[0]->content);
          pressure = pressure * 1.333224;
          snprintf(current_pressure, sizeof(current_pressure)-1,"%i", pressure);
      }
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  memset(current_humidity, 0, sizeof(current_humidity));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div//div[@class='wicon hum']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
     snprintf(current_humidity, sizeof(current_humidity)-1,"%s", xpathObj->nodesetval->nodeTab[0]->content);
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  memset(current_wind_direction, 0, sizeof(current_wind_direction));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)" /html/body/div/div/div/div//div[@class='wicon wind']//dt/text()", xpathCtx);
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
 
     snprintf(current_wind_direction, sizeof(current_wind_direction)-1,"%s", buffer);
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  memset(current_wind_speed, 0, sizeof(current_wind_speed));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div//div[@class='wicon wind']//dd[@class='value m_wind ms']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      /* Normalize speed to km/h from m/s */
      /* fprintf(stderr, "Wind  speed    %s\n", temp_buffer); */
      speed = atoi (xpathObj->nodesetval->nodeTab[0]->content);
      //speed = speed * 3600/1000;
      snprintf(current_wind_speed, sizeof(current_wind_speed)-1,"%i", speed);
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

   /* Day weather forecast */
   /* Evaluate xpath expression */
   // xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/@title", xpathCtx);
//   xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div/table/tbody/tr/th/@title", xpathCtx);
   xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div//table/tbody/tr/th/@title", xpathCtx);
   if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", "//*[@class='c0 day']/div/text()");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
   }

  nodes   = xpathObj->nodesetval;
  size = (nodes) ? nodes->nodeNr : 0;
  fprintf(stderr, "SIZE!!!!!!!!!!!!!!: %i\n", size); 
  xpathObj2 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th[@title]/text()", xpathCtx);
  xpathObj3 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody//*/td[@class='temp']/span[@class='value m_temp c']/text()", xpathCtx);
 
//  xpathObj4 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c1']/div/img/@src", xpathCtx);
  xpathObj4 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*[@class='clicon']/img/@src", xpathCtx);
 // xpathObj5 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c2']/span/text()", xpathCtx);
  xpathObj5 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*[@class='cltext']/text()", xpathCtx);
 // xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c4']/text()", xpathCtx);
  //xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*/text()", xpathCtx);
  xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody//*/td/span[@class='value m_press torr']/text()", xpathCtx);
//  xpathObj7 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*/dl[@class='wind']/dd/text()", xpathCtx);
  xpathObj7 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody//*/dl[@class='wind']/dd/span[@class='value m_wind ms']/text()", xpathCtx);
  //xpathObj8 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*/dl/dt/text()", xpathCtx);
  xpathObj8 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody//*/dt[@class]/text()", xpathCtx);
//  xpathObj9 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c6']/text()", xpathCtx);
  xpathObj9 = xmlXPathEvalExpression("/html/body/div/div/div/div/div//div/table/tbody/tr/td[6]/text()", xpathCtx);
  xpathObj10 = xmlXPathEvalExpression("/html/body/div/div/div/div/div//div/table/tbody/tr/td[7]/span[@class='value m_temp c']/text()", xpathCtx);
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
      utc_time = mktime(&tmp_tm);
      /* fprintf(stderr," UTC Temp char %s %li", temp_char, utc_time); */
      if(!timezone_flag){
          location = g_hash_table_new(g_str_hash, g_str_equal);
          utc_time = mktime(&tmp_tm);
          temp_char = strstr(nodes->nodeTab[i]->children->content, "Local: ");
          if (temp_char && strlen(temp_char) > 8)
              temp_char = temp_char + 7;
           tmp_tm_loc = get_date_for_hour_weather(temp_char);
           loc_time = mktime(&tmp_tm_loc);
           /* fprintf(stderr," Local Temp char %s %li\n", temp_char, loc_time); */
           time_diff = difftime(loc_time, utc_time);
//           if(time_diff)
           timezone_flag = TRUE;
           location_timezone = (gint)time_diff/3600;
           /* fprintf(stderr, "\nTimezone %i\n", location_timezone); */
           snprintf(buffer, sizeof(buffer)-1,"%i", location_timezone);
           g_hash_table_insert(location, "station_time_zone", g_strdup(temp_buffer));
           g_hash_table_insert(data, "location", (gpointer)location);
      }


 //    tmp_tm = get_date_for_hour_weather(temp_char);
 //   memset(buff, 0, sizeof(buff));
      setlocale(LC_TIME, "POSIX");
      strftime(buff, sizeof(buff) - 1, "%b %d", &tmp_tm);
      setlocale(LC_TIME, "");

      tmp = forecast;
      flag = FALSE;
      night_flag = FALSE;

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


//#if 0
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
//#endif
 
         /* added temperature */
         if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) &&
             xpathObj3->nodesetval->nodeTab[i] && xpathObj3->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "temperature %s\n", xpathObj3->nodesetval->nodeTab[i]->content); */
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj3->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if ((char)buffer[j] == -30 ||  buffer[j] == '-' || buffer[j] == '&' || (buffer[j]>='0' && buffer[j]<='9')){
                     if ((char)buffer[j] == -30 || buffer[j] == '&'){
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     }
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }

            if (night_flag){
                g_hash_table_insert(day, "day_low_temperature", g_strdup(temp_buffer));
                g_hash_table_insert(day, "night_temperature", g_strdup(temp_buffer));
            }else{
                g_hash_table_insert(day, "day_hi_temperature", g_strdup(temp_buffer));
                g_hash_table_insert(day, "day_temperature", g_strdup(temp_buffer));
            }
         }
         /* added icon */
         if (xpathObj4 && !xmlXPathNodeSetIsEmpty(xpathObj4->nodesetval) &&
             xpathObj4->nodesetval->nodeTab[i] && xpathObj4->nodesetval->nodeTab[i]->children->content){
            temp_char = strrchr((char*)xpathObj4->nodesetval->nodeTab[i]->children->content, '/');
            temp_char ++;
            /* fprintf (stderr, "icon %s %s \n", xpathObj4->nodesetval->nodeTab[i]->children->content, choose_hour_weather_icon(hash_for_icons, temp_char)); */
            if (night_flag)
                g_hash_table_insert(day, "night_icon", choose_hour_weather_icon(hash_for_icons, temp_char));
            else
                g_hash_table_insert(day, "day_icon", choose_hour_weather_icon(hash_for_icons, temp_char));
         }
         /* added text */
         if (xpathObj5 && !xmlXPathNodeSetIsEmpty(xpathObj5->nodesetval) &&
             xpathObj5->nodesetval->nodeTab[i] && xpathObj5->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "description %s\n", xpathObj5->nodesetval->nodeTab[i]->content); */
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
            sprintf(buffer, "%i", speed);
            if (night_flag)
                g_hash_table_insert(day, "night_wind_speed", g_strdup(buffer));
            else
                g_hash_table_insert(day, "day_wind_speed", g_strdup(buffer));
         }
         /* added wind direction */
         if (xpathObj8 && !xmlXPathNodeSetIsEmpty(xpathObj8->nodesetval) &&
             xpathObj8->nodesetval->nodeTab[i] && xpathObj8->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "Wind direction: %s\n", xpathObj8->nodesetval->nodeTab[i]->content);  */
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
             xpathObj9->nodesetval->nodeTab[i] && xpathObj9->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "description %s\n", xpathObj5->nodesetval->nodeTab[i]->content); */
            if (night_flag){
                g_hash_table_insert(day, "night_humidity", g_strdup(xpathObj9->nodesetval->nodeTab[i]->content));
            }else{
                g_hash_table_insert(day, "day_humidity", g_strdup(xpathObj9->nodesetval->nodeTab[i]->content));
            } 
         }
	 /* added feels like */
        if (xpathObj10 && !xmlXPathNodeSetIsEmpty(xpathObj10->nodesetval) &&
             xpathObj10->nodesetval->nodeTab[i] && xpathObj10->nodesetval->nodeTab[i]->content){
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj10->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if ((char)buffer[j] == -30 ||  buffer[j] == '-' || buffer[j] == '&' || (buffer[j]>='0' && buffer[j]<='9')){
                     if ((char)buffer[j] == -30 || buffer[j] == '&'){
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     }
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }
	//	     fprintf(file_out,"     <flike>%s</flike>\n", temp_buffer); 
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
  if (xpathObj10)
    xmlXPathFreeObject(xpathObj10);

  /* fill current data */
  //utc_time = mktime(&current_tm);
//  utc_time = timegm(&current_tm);
//  if (utc_time != -1){
      current_weather = g_hash_table_new(g_str_hash, g_str_equal);
      memset(buff, 0, sizeof(buff));
      setlocale(LC_TIME, "POSIX");
      strftime(buff, sizeof(buff) - 1, "%D %I:%M %p", &current_tm);
      setlocale(LC_TIME, "");
      /* fprintf(stderr,"Last update !!!!!!!!!!!! %s    %s\n", buff, current_icon); */
      g_hash_table_insert(current_weather, "last_update", g_strdup(buff));
      g_hash_table_insert(current_weather, "icon", g_strdup(current_icon));
      g_hash_table_insert(current_weather, "title", g_strdup(current_title));
      g_hash_table_insert(current_weather, "day_hi_temperature", g_strdup(current_temperature));
      g_hash_table_insert(current_weather, "pressure", g_strdup(current_pressure));
      g_hash_table_insert(current_weather, "humidity", g_strdup(current_humidity));
      g_hash_table_insert(current_weather, "wind_speed", g_strdup(current_wind_speed));
      g_hash_table_insert(current_weather, "wind_direction", g_strdup(current_wind_direction));
      g_hash_table_insert(data, "current", (gpointer)current_weather);
//  }
// Sun rise   
#if 0
  xpathObj = xmlXPathEvalExpression((const xmlChar*)" /html/body/div/*//div[@id='astronomy']/div//span/text()", xpathCtx);
  
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
             xpathObj->nodesetval->nodeTab[0] && xpathObj->nodesetval->nodeTab[0]->content){
        strptime(xpathObj->nodesetval->nodeTab[0]->content, "%d.%m.%Y", &current_tm);
        current_tm.tm_min = 0;
        current_tm.tm_hour = 0;
        utc_time = mktime(&current_tm);
        fprintf(file_out,"    <period start=\"%li\"", utc_time);
        fprintf(file_out," end=\"%li\">\n", utc_time + 24*3600); 
        sunrise_flag = TRUE;
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/*//div[@id='astronomy']//ul[@class='sun']/li[1]/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      setlocale(LC_TIME, "POSIX");
      strptime(xpathObj->nodesetval->nodeTab[0]->content, "%H:%M", &current_tm);
      setlocale(LC_TIME, "");
      utc_time = mktime(&current_tm);
      fprintf(file_out,"    <sunrise>%li</sunrise>\n", utc_time);
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/*//div[@id='astronomy']//ul[@class='sun']/li[2]/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
        setlocale(LC_TIME, "POSIX");
        strptime(xpathObj->nodesetval->nodeTab[0]->content, "%H:%M", &current_tm);
        setlocale(LC_TIME, "");
        utc_time = mktime(&current_tm);
        fprintf(file_out,"    <sunset>%li</sunset>\n", utc_time);
  }
  if (xpathObj)
      xmlXPathFreeObject(xpathObj);

  if (sunrise_flag)
      fprintf(file_out,"    </period>\n");
#endif
  g_hash_table_insert(data, "forecast", (gpointer)forecast);
  /* Clean */
  g_hash_table_destroy(hash_for_translate);
  g_hash_table_destroy(hash_for_icons);
  if (xpathCtx)
    xmlXPathFreeContext(xpathCtx); 


  return size/4;


}



void
fill_detail_data(htmlDocPtr doc, GHashTable *hash_for_icons, GHashTable *hash_for_translate, GHashTable *data){
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

    struct tm   current_tm = {0};
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

    fprintf(stderr, "parse_and_write_detail_data\n");
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


  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      current_tm = get_date_for_current_weather((char*)xpathObj->nodesetval->nodeTab[0]->content);
      memset(buff, 0, sizeof(buff));
      setlocale(LC_TIME, "POSIX");
      strftime(buff, sizeof(buff) - 1, "%D %I:%M %p", &current_tm);
      setlocale(LC_TIME, "");
      g_hash_table_insert(hours_data, "last_update", g_strdup(buff));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);


  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div//div/table/tbody/tr[@class='wrow forecast']/th/@title", xpathCtx);
  xpathObj2 = xmlXPathEvalExpression("/html/body/div/div/div//div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*[@class='clicon']/img/@src", xpathCtx);
  xpathObj3 = xmlXPathEvalExpression("/html/body/div/div/div//div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*[@class='cltext']/text()", xpathCtx);
  xpathObj4 = xmlXPathEvalExpression("/html/body/div/div/div/div//div/table/tbody/tr/td/span[@class='value m_temp c']/text()", xpathCtx);
  xpathObj5 = xmlXPathEvalExpression("/html/body/div/div/div/div//div/table/tbody/tr/td/span[@class='value m_press torr']/text()", xpathCtx);
  xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div//div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*/dl[@class='wind']/dd/text()", xpathCtx);
  xpathObj7 = xmlXPathEvalExpression("/html/body/div/div/div//div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*/dl/dt/text()", xpathCtx);
  xpathObj8 = xmlXPathEvalExpression("/html/body/div/div/div/div//div/table/tbody/tr/td[6]/text()", xpathCtx);
  xpathObj9 = xmlXPathEvalExpression("/html/body/div/div/div/div//div/table/tbody/tr/td[7]/span[@class='value m_temp c']/text()", xpathCtx);

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
          strftime(buffer, sizeof(buffer) - 1, "%D %I:%M %p", &tmp_tm);

          setlocale(LC_TIME, "");
          
           
       /* added icon */
       if (xpathObj2 && !xmlXPathNodeSetIsEmpty(xpathObj2->nodesetval) && xpathObj2->nodesetval->nodeTab[i]->children->content){
          /* fprintf (stderr, "sdfff %s\n", xpathObj2->nodesetval->nodeTab[i]->children->content); */
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
        /*    fprintf (stderr, "temperature %s\n", xpathObj4->nodesetval->nodeTab[i]->content); */
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
      if (xpathObj8 && !xmlXPathNodeSetIsEmpty(xpathObj8->nodesetval) && xpathObj8->nodesetval->nodeTab[i]->content){
         /* fprintf (stderr, "temperature %s\n", xpathObj8->nodesetval->nodeTab[i]->content); */
         g_hash_table_insert(detail, "hour_humidity", g_strdup(xpathObj8->nodesetval->nodeTab[i]->content));
      }
      /* added feels like */
      if (xpathObj9 && !xmlXPathNodeSetIsEmpty(xpathObj9->nodesetval) && xpathObj9->nodesetval->nodeTab[i]->content){
         /* fprintf (stderr, "hour_feels_like %s\n", xpathObj9->nodesetval->nodeTab[i*5+4]->content); */
         g_hash_table_insert(detail, "hour_feels_like", g_strdup(xpathObj9->nodesetval->nodeTab[i]->content));
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

    GHashTable  *hash_for_icons;
    GHashTable  *hash_for_translate;
    hash_for_translate = hash_description_gismeteo_table_create();
    hash_for_icons = hash_icons_gismeteo_table_create();
    fill_detail_data(doc, hash_for_icons, hash_for_translate, data);
    g_hash_table_destroy(hash_for_translate);
    g_hash_table_destroy(hash_for_icons);

    return -1;
}
/**************************************************************************/
