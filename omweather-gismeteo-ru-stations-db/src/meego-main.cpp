/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-gismeteo-ru-stations-db
 *
 * Copyright (C) 2009-2012 Vlad Vasilyeu
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
#include "meego-main.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#define DAY 1
#define NIGHT 2
#define MORNING 3
#define EVENING 4
#define buff_size 2048
#ifdef QT
    static QHash<QString, QString> *hash_for_icons;
    static QHash<QString, QString> *hash_for_translate;
    QHash<QString, QString> *hash_icons_gismeteo_table_create(void);
#endif

static xmlHashTablePtr hash_for_icons;
static xmlHashTablePtr hash_for_descriptions;
int        location_timezone = 0;
/*******************************************************************************/
struct tm
get_data_from_russia_data(char *temp_string){
    char buffer[256];
    char temp_buffer[256];
    struct tm   tmp_tm = {0};
    char *temp_point;
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
    return tmp_tm;
}
/*******************************************************************************/
#ifdef GLIB
    char*
    choose_hour_weather_icon(GHashTable *hash_for_icons, gchar *image) {
        char *result;
        char *source;
        char *tmp_result = NULL;

        if(!image)
            return g_strdup("49");
        source = g_strdup_printf("%s", image);
        tmp_result = (gchar *)hash_gismeteo_table_find(hash_for_icons, source, FALSE);
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
#ifdef QT 
    QString
    choose_hour_weather_icon(QHash<QString, QString> *hash_for_icons, char *image){
        return hash_gismeteo_icon_table_find(hash_for_icons, image);
    }
#endif
   /*******************************************************************************/
struct tm
get_date_for_current_weather(char *temp_string){
    char buffer[512];
    char temp_buffer[256];
    char temp_buffer2[buff_size];
    struct tm   tmp_tm = {0};
    char *temp_point;
    char *temp_char;
    int j;
    int flag = 0;
    int step = 0;

    memset(buffer, 0, sizeof(buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memset(temp_buffer2, 0, sizeof(temp_buffer2));

    snprintf(temp_buffer2,  sizeof(temp_buffer2)-1, "%s", temp_string);
    for (j = 0 ; (j<(strlen(temp_buffer2)) && j < buff_size); j++ ){
        if ((flag == 0) && ((temp_buffer2[j] == 9)  ||
                            (temp_buffer2[j] == ' ')  ||
                            (temp_buffer2[j] == '\n') || 
                            (temp_buffer2[j] == '\r'))){
            step++;
            continue;
        }else{
            flag = 1;
            temp_buffer2[j-step] = temp_buffer2[j] ;
        }
    }

    temp_point = strchr(temp_buffer2,' ');
    if (!temp_point)
        return tmp_tm;
    snprintf(buffer, strlen(temp_buffer2) - strlen(temp_point+1),"%s", temp_buffer2);
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
get_date_for_hour_weather(char *temp_string){
    char buffer[256];
    char temp_buffer[256];
    char buff[256];
    struct tm   tmp_tm = {0};
    char *temp_point;
    memset(buffer, 0, sizeof(buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memset(buff, 0, sizeof(buff));
    temp_point = strchr(temp_string,'-');
    if (!temp_point)
        return tmp_tm;
    snprintf(buffer, strlen(temp_string) - strlen(temp_point+1),"%s", temp_string);
    snprintf(temp_buffer, strlen(temp_point) - strlen(strchr(temp_point + 1,'-')),"%s", temp_point+1);
    temp_point = strchr(temp_string,'-');
    temp_point = strchr(temp_point,'-');
    snprintf(buff, strlen(temp_point) - strlen(strchr(temp_point + 1,' ')),"%s", temp_point+1);
    strcat(buff, " ");
    strcat(buff, buffer);
    strcat(buff, " ");
    temp_point = strchr(temp_string,' ');
    strcat(buff, temp_point+1);
    fprintf(stderr, "Buffer %s\n", buff); 
    strptime(buff, "%m-%d %Y %H:%M", &tmp_tm);
    fprintf(stderr, "\ntmp_tm hour %d\n", (&tmp_tm)->tm_hour); 
    return tmp_tm;
}
/*******************************************************************************/
int
gismeteoru_parse_and_write_xml_data(const char *station_id, htmlDocPtr doc, const char *result_file){
    char       buff[256],
                buffer[buff_size],
                current_temperature[20],
                current_icon[10],
                current_title[1024],
                current_pressure[15],
                current_humidity[15],
                current_wind_direction[15],
                current_wind_speed[15];
    char       temp_buffer[buff_size];
#ifdef GLIB
    GSList      *forecast = NULL;
    GSList      *tmp = NULL;
    GHashTable  *day = NULL;
    GHashTable *hash_for_translate;
    GHashTable *hash_for_icons;
#endif
    int    flag;
    int    night_flag;
    int    size;
    int    i, j;
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
    char       *temp_char;
    int        pressure; 
    int        speed;

#ifdef GLIB
    char       *image = NULL;
#else
    char        image[buff_size];
#endif
    double      time_diff = 0;
    time_t      loc_time;
    time_t      utc_time;
    int         timezone_flag = false;
    int         sunrise_flag = false;
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
    int         localtimezone;
    struct tm time_tm1;
    struct tm time_tm2;


    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;

    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 
    fprintf(stderr,"Local Time Zone %i\n", localtimezone);

    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", station_id);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");

#ifdef GLIB
   hash_for_translate = hash_description_gismeteo_table_create();
   hash_for_icons = hash_icons_gismeteo_table_create();
#endif
#ifdef QT
    hash_for_translate = hash_description_gismeteo_table_create();
    hash_for_icons = hash_icons_gismeteo_table_create();
#endif

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
      fprintf(stderr, "Current date and time %s\n", (char*)xpathObj->nodesetval->nodeTab[0]->content);
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
                 if ((char)buffer[j] == -30 || (uint)buffer[j] == 226 ||  buffer[j] == '-' || (buffer[j]>='0' && buffer[j]<='9')) {
                     if ((char)buffer[j] == -30 || (uint)buffer[j] == 226)
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
           temp_char = strrchr((char *)xpathObj->nodesetval->nodeTab[0]->children->content, '/');
           temp_char ++;
#ifdef GLIB
           image = g_strdup(temp_char);
#else
           snprintf(image, buff_size-1,"%s", temp_char);
#endif
           i = 0;
           memset(temp_buffer, 0, sizeof(temp_buffer));
           while((image[i] != ')') && (i < strlen(image))){
             sprintf(temp_buffer,"%s%c",temp_buffer, image[i]);
             i++;
            }
        }
#ifdef GLIB
        if (image)
            g_free(image);
#endif
       
#ifdef GLIB
       snprintf(current_icon, sizeof(current_icon)-1,"%s", choose_hour_weather_icon(hash_for_icons, temp_buffer));
#else
       snprintf(current_icon, sizeof(current_icon)-1,"%s",
             (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));

#endif
#ifdef QT 
        snprintf(current_icon, sizeof(current_icon)-1,"%s", choose_hour_weather_icon(hash_for_icons, temp_buffer).toStdString().c_str());
#endif
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  memset(current_title, 0, sizeof(current_title));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div//dd/table/tr/td/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
#ifdef GLIB
    snprintf(current_title, sizeof(current_title)-1,"%s", hash_gismeteo_table_find(hash_for_translate, (char *)xpathObj->nodesetval->nodeTab[0]->content, FALSE));
#endif
#ifdef QT
   snprintf(current_title, sizeof(current_title)-1,"%s", (char*)hash_gismeteo_description_table_find(hash_for_translate, (char *)xpathObj->nodesetval->nodeTab[0]->content).toStdString().c_str()); 
#endif
    snprintf(current_title, sizeof(current_title)-1,"%s", (char*)xmlHashLookup(hash_for_descriptions, (const xmlChar*)xpathObj->nodesetval->nodeTab[0]->content));

  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);
  
  memset(current_pressure, 0, sizeof(current_pressure));
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div//div[@class='wicon barp']/dd[@class='value m_press torr']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      if (strlen((char *)xpathObj->nodesetval->nodeTab[0]->content) > 0){
          pressure = atoi((char *)xpathObj->nodesetval->nodeTab[0]->content);
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
      speed = atoi ((char *)xpathObj->nodesetval->nodeTab[0]->content);
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
  xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div//table/tbody/tr/th[@title]/text()", xpathCtx);
  xpathObj3 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div//table/tbody//*/td[@class='temp']/span[@class='value m_temp c']/text()", xpathCtx);
 
//  xpathObj4 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c1']/div/img/@src", xpathCtx);
  xpathObj4 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*[@class='clicon']/img/@src", xpathCtx);
 // xpathObj5 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c2']/span/text()", xpathCtx);
  xpathObj5 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*[@class='cltext']/text()", xpathCtx);
 // xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c4']/text()", xpathCtx);
  //xpathObj6 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*/text()", xpathCtx);
  xpathObj6 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div//table/tbody//*/td/span[@class='value m_press torr']/text()", xpathCtx);
//  xpathObj7 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*/dl[@class='wind']/dd/text()", xpathCtx);
  xpathObj7 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div//table/tbody//*/dl[@class='wind']/dd/span[@class='value m_wind ms']/text()", xpathCtx);
  //xpathObj8 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div//table/tbody/tr/th/following-sibling::*/dl/dt/text()", xpathCtx);
  xpathObj8 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div//table/tbody//*/dt[@class]/text()", xpathCtx);
//  xpathObj9 = xmlXPathEvalExpression("/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/following-sibling::*[@class='c6']/text()", xpathCtx);
  xpathObj9 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div//div/table/tbody/tr/td[6]/text()", xpathCtx);
  xpathObj10 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div//div/table/tbody/tr/td[7]/span[@class='value m_temp c']/text()", xpathCtx);
  /* fprintf(stderr, "Result (%d nodes):\n", size); */
  for(i = 0; i < size; ++i) {
#ifdef GLIB
      day = NULL;
#endif

      /* Take UTC time: */
      if (!nodes->nodeTab[i]->children->content)
          continue;
      temp_char = strstr((char *)nodes->nodeTab[i]->children->content, "UTC: ");
      if (temp_char && strlen(temp_char) >6)
              temp_char = temp_char +5;

      tmp_tm = get_date_for_hour_weather(temp_char);
      utc_time = mktime(&tmp_tm) + localtimezone*3600;
      //utc_time = mktime(&tmp_tm);
      /* fprintf(stderr," UTC Temp char %s %li %li\n", temp_char, utc_time, mktime(&tmp_tm)); */
      if(!timezone_flag){
          utc_time = mktime(&tmp_tm) + localtimezone*3600;
          temp_char = strstr((char *)nodes->nodeTab[i]->children->content, "Local: ");
          if (temp_char && strlen(temp_char) > 8)
              temp_char = temp_char + 7;
           tmp_tm_loc = get_date_for_hour_weather(temp_char);
           loc_time = mktime(&tmp_tm_loc);
           /* fprintf(stderr," Local Temp char %s %li\n", temp_char, loc_time); */
           time_diff = difftime(loc_time, utc_time);
//           if(time_diff)
           timezone_flag = true;
           location_timezone = (int)time_diff/3600;
           /* fprintf(stderr, "\nTimezone %i\n", location_timezone); */
           fprintf(file_out,"  <timezone>%i</timezone>\n", location_timezone);
      }

      fprintf(file_out,"    <period start=\"%li\"", utc_time);
      fprintf(file_out," end=\"%li\">\n", utc_time + 6*3600); 


#if 0
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
#endif
 
         /* added temperature */
         if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) &&
             xpathObj3->nodesetval->nodeTab[i] && xpathObj3->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "temperature %s\n", xpathObj3->nodesetval->nodeTab[i]->content); */
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj3->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if ((char)buffer[j] == -30 || (uint)buffer[j] == 226 || buffer[j] == '-' || buffer[j] == '&' || (buffer[j]>='0' && buffer[j]<='9')){
                     if ((char)buffer[j] == -30 || buffer[j] == '&' || (uint)buffer[j] == 226){
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     }
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }
			 fprintf(file_out,"     <temperature>%s</temperature>\n", temp_buffer); 
         }
         /* added icon */
         if (xpathObj4 && !xmlXPathNodeSetIsEmpty(xpathObj4->nodesetval) &&
             xpathObj4->nodesetval->nodeTab[i] && xpathObj4->nodesetval->nodeTab[i]->children->content){
            temp_char = strrchr((char*)xpathObj4->nodesetval->nodeTab[i]->children->content, '/');
            temp_char ++;
            /* fprintf (stderr, "icon %s %s \n", xpathObj4->nodesetval->nodeTab[i]->children->content, choose_hour_weather_icon(hash_for_icons, temp_char)); */
#ifdef GLIB
            fprintf(file_out,"     <icon>%s</icon>\n",  choose_hour_weather_icon(hash_for_icons, temp_char));
#endif
#ifdef QT 
        fprintf(file_out,"     <icon>%s</icon>\n",  choose_hour_weather_icon(hash_for_icons, temp_char).toStdString().c_str());
#endif

        fprintf(file_out,"     <icon>%s</icon>\n", (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_char));

         }
         /* added text */
         if (xpathObj5 && !xmlXPathNodeSetIsEmpty(xpathObj5->nodesetval) &&
             xpathObj5->nodesetval->nodeTab[i] && xpathObj5->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "description %s\n", xpathObj5->nodesetval->nodeTab[i]->content); */
#ifdef GLIB
             fprintf(file_out,"     <description>%s</description>\n", hash_gismeteo_table_find(hash_for_translate, (char *)xpathObj5->nodesetval->nodeTab[i]->content, FALSE));
#endif
#ifdef QT
            fprintf(file_out,"     <description>%s</description>\n", (char*)hash_gismeteo_description_table_find(hash_for_translate, (char *)xpathObj5->nodesetval->nodeTab[i]->content).toStdString().c_str()); 
#endif
            fprintf(file_out,"     <description>%s</description>\n", (char*)xmlHashLookup(hash_for_descriptions, (const xmlChar*)xpathObj5->nodesetval->nodeTab[i]->content)
);
         }
         /* added pressure */
         if (xpathObj6 && !xmlXPathNodeSetIsEmpty(xpathObj6->nodesetval) &&
             xpathObj6->nodesetval->nodeNr >= (i*5+2) &&
             xpathObj6->nodesetval->nodeTab[i*5+2] && xpathObj6->nodesetval->nodeTab[i*5+2]->content){
             pressure = atoi((char*)xpathObj6->nodesetval->nodeTab[i*5+2]->content);
             pressure = pressure * 1.333224;
			fprintf(file_out,"     <pressure>%i</pressure>\n", pressure);
         }
         /* added wind speed */
         if (xpathObj7 && !xmlXPathNodeSetIsEmpty(xpathObj7->nodesetval) &&
             xpathObj7->nodesetval->nodeTab[i] && xpathObj7->nodesetval->nodeTab[i]->content){
            /* Normalize speed to km/h from m/s */
            /* fprintf(stderr, "Wind  speed    \n"); */ 
            speed = atoi ((char *)xpathObj7->nodesetval->nodeTab[i]->content);
			fprintf(file_out,"     <wind_speed>%1.f</wind_speed>\n",  (double)(speed));
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
			 fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", buffer); 
         }
         /* added humidity */
         if (xpathObj9 && !xmlXPathNodeSetIsEmpty(xpathObj9->nodesetval) &&
             xpathObj9->nodesetval->nodeTab[i] && xpathObj9->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "description %s\n", xpathObj5->nodesetval->nodeTab[i]->content); */
			fprintf(file_out,"     <humidity>%s</humidity>\n", (char *)xpathObj9->nodesetval->nodeTab[i]->content);
         }
	 /* added feels like */
        if (xpathObj10 && !xmlXPathNodeSetIsEmpty(xpathObj10->nodesetval) &&
             xpathObj10->nodesetval->nodeTab[i] && xpathObj10->nodesetval->nodeTab[i]->content){
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj10->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if ((char)buffer[j] == -30 || (uint)buffer[j] == 226 ||  buffer[j] == '-' || buffer[j] == '&' || (buffer[j]>='0' && buffer[j]<='9')){
                     if ((char)buffer[j] == -30 || buffer[j] == '&' || (uint)buffer[j] == 226){
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     }
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }
		     fprintf(file_out,"     <flike>%s</flike>\n", temp_buffer); 
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
  if (xpathObj9)
    xmlXPathFreeObject(xpathObj9);
  if (xpathObj10)
    xmlXPathFreeObject(xpathObj10);

  /* fill current data */
  utc_time = mktime(&current_tm);
  if (utc_time != -1){
      utc_time = utc_time - location_timezone*3600;
      fprintf(file_out,"    <period start=\"%li\"", utc_time);
      fprintf(file_out," end=\"%li\" current=\"true\">\n", utc_time + 4*3600); 

      fprintf(file_out,"     <temperature>%s</temperature>\n", current_temperature); 
      fprintf(file_out,"     <icon>%s</icon>\n",  current_icon);
      fprintf(file_out,"     <description>%s</description>\n", current_title);
      fprintf(file_out,"     <pressure>%s</pressure>\n", current_pressure);
      fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", current_wind_direction);
      fprintf(file_out,"     <humidity>%s</humidity>\n", current_humidity);
      fprintf(file_out,"     <wind_speed>%s</wind_speed>\n", current_wind_speed);
      fprintf(file_out,"    </period>\n");
  }
// Sun rise  /html/body/div/*//div/div/div/div/div[2]/ul[@class='sun']/li[1]/text() 
//
///html/body/div/*//div/div/div/div/div[2]/ul/@title
  xpathObj = xmlXPathEvalExpression((const xmlChar*)" /html/body/div/*//div[@id='astronomy']/div//span/text()", xpathCtx);
  
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
             xpathObj->nodesetval->nodeTab[0] && xpathObj->nodesetval->nodeTab[0]->content){
        strptime((char *)xpathObj->nodesetval->nodeTab[0]->content, "%d.%m.%Y", &current_tm);
        snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj->nodesetval->nodeTab[0]->content);
        current_tm.tm_min = 0;
        current_tm.tm_hour = 0;
        utc_time = mktime(&current_tm) + localtimezone*3600;
        fprintf(file_out,"    <period start=\"%li\"", utc_time);
        fprintf(file_out," end=\"%li\">\n", utc_time + 24*3600); 

        sunrise_flag = true;
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/*//div[@id='astronomy']//ul[@class='sun']/li[1]/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      setlocale(LC_TIME, "POSIX");
      snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s %s", xpathObj->nodesetval->nodeTab[0]->content, buffer);
      utc_time = mktime(&current_tm) + localtimezone*3600;
      strptime(temp_buffer, " %H:%M %d.%m.Y", &current_tm);
      setlocale(LC_TIME, "");
      utc_time = mktime(&current_tm) + localtimezone*3600;
      fprintf(file_out,"    <sunrise>%li</sunrise>\n", utc_time );
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/*//div[@id='astronomy']//ul[@class='sun']/li[2]/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
        setlocale(LC_TIME, "POSIX");
        snprintf(temp_buffer, sizeof(temp_buffer)-1,"%s %s", xpathObj->nodesetval->nodeTab[0]->content, buffer);
        strptime(temp_buffer, " %H:%M %d.%m.Y", &current_tm);
        setlocale(LC_TIME, "");
        utc_time = mktime(&current_tm) + localtimezone*3600;
        fprintf(file_out,"    <sunset>%li</sunset>\n", utc_time);
  }
  if (xpathObj)
      xmlXPathFreeObject(xpathObj);

  if (sunrise_flag)
      fprintf(file_out,"    </period>\n");

  /* Clean */
#ifdef GLIB
  g_hash_table_destroy(hash_for_translate);
  g_hash_table_destroy(hash_for_icons);
#endif
  if (xpathCtx)
    xmlXPathFreeContext(xpathCtx); 

  fclose(file_out);

  return size/4;
}
/*******************************************************************************/
void
gismeteoru_parse_and_write_detail_data(const char *station_id, htmlDocPtr doc, const char *result_file){
#ifdef GLIB
    GHashTable  *hash_for_translate;
    GHashTable  *hash_for_icons;
    GHashTable  *detail = NULL; 
    GHashTable  *hours_data = NULL;
    GSList      *hour_weather = NULL;
#endif
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
    int        i = 0, j = 0, k = 0;
    char       buffer[buff_size];
    char       buff[buff_size];
    char       temp_buffer[buff_size];
    char       tmp[buff_size];
    char       *temp_char;
    int        speed;
    int        pressure; 
    int        temperature; 
    int        count_of_hours = 0;
    struct tm   tmp_tm = {0};
    struct tm   tmp_tm_utc = {0};
    struct tm   tmp_tm_loc = {0};
    double      time_diff = 0;
    time_t      loc_time;
    time_t      utc_time;
    int break_flag = false; 
    int timezone_flag = false;
    int feels_like_flag = false;
    char       *image = NULL;

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
    int size;
    time_t      t_start = 0, t_end = 0;
    FILE        *file_out;
    struct tm time_tm1;
    struct tm time_tm2;
    int localtimezone;



   /* Create xpath evaluation context */
   xpathCtx = xmlXPathNewContext(doc);
   if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
         return;
   }
   file_out = fopen(result_file, "a");
    if (!file_out)
        return ;

   /* Register namespaces from list (if any) */
   xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"html",
                                (const xmlChar*)"http://www.w3.org/1999/xhtml");
#ifdef GLIB
   hash_for_translate = hash_description_gismeteo_table_create();
   hash_for_icons = hash_icons_gismeteo_table_create();
#endif

  /* Evaluate xpath expression */
 // xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div/table/tbody/tr/td[@class='c0']/@title", xpathCtx);
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div/span[@class='icon date']/text()", xpathCtx);

//  hours_data = g_hash_table_new(g_str_hash, g_str_equal);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      strptime((char *)xpathObj->nodesetval->nodeTab[0]->content, "%d.%m.%Y %H:%M:%S", &tmp_tm);
      memset(buff, 0, sizeof(buff));
      setlocale(LC_TIME, "POSIX");
      strftime(buff, sizeof(buff) - 1, "%D %I:%M %p", &tmp_tm);
      setlocale(LC_TIME, "");
//      g_hash_table_insert(hours_data, "last_update", g_strdup(buff));
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='temp']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/dl/dt[@class='png']/@style", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->children->content){
        if (xpathObj->nodesetval->nodeTab[0]->children->content){
           temp_char = strrchr((char *)xpathObj->nodesetval->nodeTab[0]->children->content, '/');
           temp_char ++;
#ifdef GLIB
           image = g_strdup(temp_char);
#else
           snprintf(image, sizeof(image)-1,"%s", temp_char);
#endif
           i = 0;
           memset(temp_buffer, 0, sizeof(temp_buffer));
           while((image[i] != ')') && (i < strlen(image))){
             sprintf(temp_buffer,"%s%c",temp_buffer, image[i]);
             i++;
            }
        }
#ifdef GLIB        
        if (image)
            g_free(image);
#endif
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/dl/dd/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='wicon barp']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      if (strlen((char *)xpathObj->nodesetval->nodeTab[0]->content) > 0){
                        pressure = atoi((char *)xpathObj->nodesetval->nodeTab[0]->content);
                        pressure = pressure * 1.333224;
                        snprintf(temp_buffer, sizeof(temp_buffer)-1,"%i", pressure);
      }
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='wicon hum']/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
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
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);

  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div/div/div/div[@class='wicon wind']/dl/dd/text()", xpathCtx);
  if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[0]->content){
      /* Normalize speed to km/h from m/s */
      /* fprintf(stderr, "Wind  speed    %s\n", temp_buffer); */
      speed = atoi ((char *)xpathObj->nodesetval->nodeTab[0]->content);
      speed = speed * 3600/1000;
      sprintf(buffer, "%i", speed);
  }
  if (xpathObj)
    xmlXPathFreeObject(xpathObj);
 // xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div//div/table/tbody/tr[@class='wrow forecast']/th/@title", xpathCtx);
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div//div[@id='weather-hourly']//div[@class='wsection wdata']/table/tbody//th/@title", xpathCtx);
 // xpathObj2 = xmlXPathEvalExpression("/html/body/div/div/div//div/table/tbody/tr[@class='wrow forecast']/th/following-sibling::*[@class='clicon']/img/@src", xpathCtx);
  xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div//div/table/tbody//td[@class='clicon']/img/@src", xpathCtx);
  xpathObj3 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div//div/table/tbody/tr/th/following-sibling::*[@class='cltext']/text()", xpathCtx);
  xpathObj4 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div//div/table/tbody/tr/td[@class='temp']/span[@class='value m_temp c']/text()", xpathCtx);
  xpathObj5 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div//div/table/tbody/tr/td/span[@class='value m_press torr']/text()", xpathCtx);
  xpathObj6 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div//table/tbody/tr/td/dl[@class='wind']/dt/text()", xpathCtx);
  xpathObj7 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div//div/table/tbody/tr/th/following-sibling::*/dl/dt/text()", xpathCtx);
  xpathObj8 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div//div/table/tbody/tr/td[6]/text()", xpathCtx);
  xpathObj9 = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/div/div//div/table/tbody/tr/td[7]/span[@class='value m_temp c']/text()", xpathCtx);
  nodes   = xpathObj->nodesetval;
  size = (nodes) ? nodes->nodeNr : 0;
  fprintf(stderr,"Size fo hours %i\n", size);
  //if (size > 8)
  //    size = 8;
  for(i = 0; i < size; ++i) {
      /* Take UTC time: */
      if (!nodes->nodeTab[i]->children->content)
          continue;
      /* fprintf(stderr," Time %s\n", nodes->nodeTab[i]->children->content); */
      temp_char = strstr((char *)nodes->nodeTab[i]->children->content, "UTC: ");
      if (temp_char && strlen(temp_char) > 6)
              temp_char = temp_char + 5;

      tmp_tm = get_date_for_hour_weather(temp_char);
      t_start = timegm(&tmp_tm);
      fprintf(file_out,"    <period start=\"%li\"", t_start);
      /* 1 hour for weather.com */
      fprintf(file_out," end=\"%li\" hour=\"true\">\n", t_start + 4*3600); 

      memset(buff, 0, sizeof(buff));
      setlocale(LC_TIME, "POSIX");

      strftime(buff, sizeof(buff) - 1, "%H", &tmp_tm);
      strftime(buffer, sizeof(buff) - 1, "%D %I:%M %p", &tmp_tm);

      setlocale(LC_TIME, "");

      if(!timezone_flag){
          utc_time = mktime(&tmp_tm) + localtimezone*3600;
          temp_char = strstr((char *)nodes->nodeTab[i]->children->content, "Local: ");
          if (temp_char && strlen(temp_char) > 8)
              temp_char = temp_char + 7;
           tmp_tm_loc = get_date_for_hour_weather(temp_char);
           loc_time = mktime(&tmp_tm_loc);
           time_diff = difftime(loc_time, utc_time);
           if(time_diff)
               timezone_flag = true;
           location_timezone = (int)time_diff/3600;
           fprintf(stderr, "\nTimezone %i\n", location_timezone); 
           snprintf(temp_buffer, sizeof(temp_buffer)-1, "%i",location_timezone);
      }
 
   /* added icon */
   if (xpathObj2 && !xmlXPathNodeSetIsEmpty(xpathObj2->nodesetval) && xpathObj2->nodesetval->nodeTab[i]->children->content){
      /* fprintf (stderr, "Icon %s\n", xpathObj2->nodesetval->nodeTab[i]->children->content); */
      temp_char = strrchr((char*)xpathObj2->nodesetval->nodeTab[i]->children->content, '/');
      temp_char ++;
#ifdef GLIB
      fprintf(file_out,"     <icon>%s</icon>\n", choose_hour_weather_icon(hash_for_icons, temp_char));
#endif
      fprintf(file_out,"     <icon>%s</icon>\n", (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_char));
#ifdef QT 
      fprintf(file_out,"     <icon>%s</icon>\n",  choose_hour_weather_icon(hash_for_icons, temp_char).toStdString().c_str());
#endif
   }
   /* added text */
   if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) && i <= xpathObj3->nodesetval->nodeNr && xpathObj3->nodesetval->nodeTab[i]->content){
#ifdef GLIB
      fprintf(file_out,"     <description>%s</description>\n", (char*)hash_gismeteo_table_find(hash_for_translate, (char *)xpathObj3->nodesetval->nodeTab[i]->content, FALSE)); 
#endif
#ifdef QT
      fprintf(file_out,"     <description>%s</description>\n", (char*)hash_gismeteo_description_table_find(hash_for_translate, (char *)xpathObj3->nodesetval->nodeTab[i]->content).toStdString().c_str()); 
#endif
      fprintf(file_out,"     <description>%s</description>\n", (char*)xmlHashLookup(hash_for_descriptions, (const xmlChar*)xpathObj3->nodesetval->nodeTab[i]->content)); 
   }
   /* added temperature */
   if (xpathObj4 && !xmlXPathNodeSetIsEmpty(xpathObj4->nodesetval) && xpathObj4->nodesetval->nodeTab[i]->content){
      
       snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj4->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if ((char)buffer[j] == -30  || (uint)buffer[j] == 226 || buffer[j] == '-' || (buffer[j]>='0' && buffer[j]<='9')){
                     if ((char)buffer[j] == -30 || (uint)buffer[j] == 226)
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }
        fprintf(file_out,"     <temperature>%s</temperature>\n", temp_buffer); 
        /* fprintf (stderr, "temperature %s %s\n", xpathObj4->nodesetval->nodeTab[i]->content, temp_buffer); */
   }
   /* added pressure */
   if (xpathObj5 && !xmlXPathNodeSetIsEmpty(xpathObj5->nodesetval) && xpathObj5->nodesetval->nodeTab[i]->content){
      pressure = atoi((char*)xpathObj5->nodesetval->nodeTab[i]->content);
      pressure = pressure * 1.333224;
      snprintf(buffer, sizeof(buffer)-1,"%i", pressure);
      fprintf(file_out,"     <pressure>%s</pressure>\n", buffer);
      /* fprintf (stderr, "pressure %s\n", xpathObj5->nodesetval->nodeTab[i]->content); */
   }
   /* added wind speed */
   if (xpathObj6 && !xmlXPathNodeSetIsEmpty(xpathObj6->nodesetval) && xpathObj6->nodesetval->nodeTab[i]->content){
      /* Normalize speed to km/h from m/s */
      /* fprintf(stderr, "Wind  speed    %s\n", temp_buffer); */
      speed = atoi ((char *)xpathObj6->nodesetval->nodeTab[i]->content);
      speed = speed * 3600/1000;
      sprintf(buffer, "%i", speed);
      fprintf(file_out,"     <wind_speed>%s</wind_speed>\n", buffer);
   }
   /* added wind direction */
   if (xpathObj7 && !xmlXPathNodeSetIsEmpty(xpathObj7->nodesetval) && xpathObj7->nodesetval->nodeTab[i]->content){
       /*  fprintf (stderr, "Wind direction: %s\n", xpathObj7->nodesetval->nodeTab[i]->content); */
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
 
       fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", buffer);
   }
  /* added humidity */
  if (xpathObj8 && !xmlXPathNodeSetIsEmpty(xpathObj8->nodesetval) && xpathObj8->nodesetval->nodeTab[i]->content){
     fprintf(file_out,"     <humidity>%s</humidity>\n", xpathObj8->nodesetval->nodeTab[i]->content);
  }
  /* added feels like */
  if (xpathObj9 && !xmlXPathNodeSetIsEmpty(xpathObj9->nodesetval) && xpathObj9->nodesetval->nodeTab[i]->content){
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj9->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if ((char)buffer[j] == -30 || (uint)buffer[j] == 226 ||  buffer[j] == '-' || (buffer[j]>='0' && buffer[j]<='9')){
                     if ((char)buffer[j] == -30 || buffer[j] == '&' || (uint)buffer[j] == 226)
                        sprintf(temp_buffer,"%s-",temp_buffer);
                     else
                        sprintf(temp_buffer,"%s%c",temp_buffer, buffer[j]);
                 }
             }

     fprintf(file_out,"     <flike>%s</flike>\n", temp_buffer);
  }

   fprintf(file_out,"    </period>\n");
  }
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

  fclose(file_out);
}
/**************************************************************************/
int
convert_station_gismeteo_data(const char *station_id_with_path, const char *result_file,  const char *detail_path_data){
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int    days_number = -1;
    char   buffer[1024],
           buffer2[1024],
           *delimiter = NULL;
    struct stat file_info;
    FILE   *file_out;

    if(!station_id_with_path)
        return -1;
    *buffer = 0;
    hash_for_icons = hash_icons_gismeteoru_table_create();
    hash_for_descriptions = hash_descriptions_gismeteoru_table_create();
    snprintf(buffer, sizeof(buffer) - 1, "%s.new", station_id_with_path);
    /* check file accessability */
    if(!access(buffer, R_OK))
        if ((lstat(buffer, &file_info) == 0) && (file_info.st_size > 0)){ 
            /* check that the file containe valid data */
            doc =  htmlReadFile(station_id_with_path, "UTF-8", HTML_PARSE_NOWARNING);
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
                            //if(get_detail_data)
                            //    days_number = parse_xml_detail_data(buffer2, root_node, data);
                            //else
                            days_number = gismeteoru_parse_and_write_xml_data(buffer2, doc, result_file);
                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                            rename(buffer, station_id_with_path);
                            return days_number;
                        }
                    }
               }
            }else
                doc = NULL;
        }
    /* check file accessability */
    if(!access(station_id_with_path, R_OK)){
        /* check that the file containe valid data */
        doc =  htmlReadFile(station_id_with_path, "UTF-8", HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
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
            delimiter = strrchr((char *)station_id_with_path, '/');
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
//                if(get_detail_data)
//                    days_number = parse_xml_detail_data(buffer, root_node, data);
//                else
                 days_number = gismeteoru_parse_and_write_xml_data(buffer, doc, result_file);         
                 xmlFreeDoc(doc);
                 xmlCleanupParser();
                 if(!access(detail_path_data, R_OK)){
                     doc =  htmlReadFile(detail_path_data, "UTF-8", HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
                    if(doc){
                        root_node = NULL;
                        root_node = xmlDocGetRootElement(doc);
                        if(!root_node || ( root_node->type == XML_ELEMENT_NODE &&
                                strstr((char*)root_node->name, "err"))){
                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                        }
                        else{
                            gismeteoru_parse_and_write_detail_data(buffer, doc, result_file);
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

                    //parse_and_write_xml_detail_data(buffer, doc, result_file);

            }
            return days_number;
        }
    }
    else
        return -1;/* file isn't accessability */
}

int
main(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "gismeteoru <input_file> <output_file> <input_detail_data>\n");
        return -1;
    }
    result = convert_station_gismeteo_data(argv[1], argv[2], argv[3]);
    fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
