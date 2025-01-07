/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-foreca-com-stations-db - MeeCast
 *
 * Copyright (C) 2012-2014  Vlad Vasilyeu
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
#include "hash.h"
/*******************************************************************************/
#define buff_size 2048
#ifdef QT
static QHash<QString, QString> *hash_for_icons;
QHash<QString, QString> *hash_icons_forecacom_table_create(void);
#endif
static xmlHashTablePtr hash_for_icons;
/*******************************************************************************/
#ifdef QT
QString
choose_hour_weather_icon(char *image)
{
    QString result;

    if (hash_for_icons->contains(QString(image)))
        return hash_for_icons->value(QString(image));
    else{
        fprintf(stderr,"Unknown strings %s\n", image);
        return QString("49");
    }
}
#endif
/*******************************************************************************/
int
parse_and_write_detail_data(const char *station_id, htmlDocPtr doc, const char *result_file){
    char       buff[256];
    char       buffer[buff_size];
    char       temp_buffer[buff_size];
    char       temp_buffer2[buff_size];
    int        j, k, l;

    xmlXPathContextPtr xpathCtx; 
    xmlXPathObjectPtr xpathObj = NULL; 
    xmlXPathObjectPtr xpathObj2 = NULL; 
    xmlXPathObjectPtr xpathObj3 = NULL; 
    xmlNodeSetPtr nodes;
    char       *temp_char;
    int    flag;
    struct tm   tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
    struct tm   *tm;
    time_t      t_start = 0, t_end = 0,
                current_time = 0;
    FILE        *file_out;
    int index = 1;
    struct tm time_tm1;
    struct tm time_tm2;
    int localtimezone = 0;
    int remotetimezone = 0;
    int hour = 0;
    int day = 0;
    int month = 0;
    char buff_day[256];


    /* fprintf(stderr, "parse_and_write_detail_data()\n"); */
    file_out = fopen(result_file, "a");
    if (!file_out)
        return -1;
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    hash_for_icons = hash_icons_forecacom_table_create();

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
    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div[@id='cc']/div[@class='cctext']/p/text()[3]", xpathCtx);
  
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", " /html/body/div[@id='cc']/div[@class='cctext']/p/text()[3]");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
    }

    if (xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeTab == NULL || xpathObj->nodesetval->nodeTab[0] ==NULL || xpathObj->nodesetval->nodeTab[0]->content == NULL){
        
        xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div[@id='cc']/div[@class='cctext']/p/text()[2]", xpathCtx);

        if (xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeTab == NULL || xpathObj->nodesetval->nodeTab[0] ==NULL || xpathObj->nodesetval->nodeTab[0]->content == NULL){
            xmlXPathFreeContext(xpathCtx); 
            return -1;
        }
    }
    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 
    /* fprintf(stderr,"Local Time Zone %i\n", localtimezone); */

    if (xpathObj && xpathObj->nodesetval->nodeTab[0]->content){
        tm = localtime(&current_time);
        snprintf(buffer, sizeof(buffer)-1,"%i %s", tm->tm_year + 1900, xpathObj->nodesetval->nodeTab[0]->content);
        /* fprintf(stderr, "Time %s", buffer); */
        setlocale(LC_TIME, "POSIX");
        strptime((const char*)buffer, "%Y%n%d/%m %H", &tmp_tm);
        hour = tmp_tm.tm_hour; 
        day = tmp_tm.tm_mday;
        month = tmp_tm.tm_mon;
        if (xpathObj)
            xmlXPathFreeObject(xpathObj);
        xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div[@class='hourlyfc']/*[@class='symbcol']/preceding-sibling::div[@class='timecol']/p/text()", xpathCtx);
        if (xpathObj && xpathObj->nodesetval->nodeTab[0]->content){
            int hour = atoi((const char*)xpathObj->nodesetval->nodeTab[0]->content) -1;
            tmp_tm.tm_hour = hour;
        }
        time_tm1.tm_min = 0;
        tmp_tm.tm_min = 0;
        remotetimezone = (mktime(&tmp_tm) - mktime(&time_tm1))/3600; 
        if (abs(remotetimezone) < 13)
           fprintf(file_out,"  <timezone>%i</timezone>\n", remotetimezone);
        /* fprintf(stderr,"Remote timezone %i\n", remotetimezone); */
        strptime((const char*)buffer, "%Y%n%d/%m %H:%M", &tmp_tm);
        /* fprintf (stderr, "Time %s\n", buffer); */

        setlocale(LC_TIME, "");
        /* set begin of day in localtime */
        //tmp_tm.tm_year = tm->tm_year;

        t_start = mktime(&tmp_tm) - 3600*remotetimezone + 3600*localtimezone;
        fprintf(file_out,"    <period start=\"%li\"", (t_start + 1  - 2*3600));
        /* set end of current time in localtime */
        t_end = t_start + 3600*4 - 1;
        fprintf(file_out," end=\"%li\" current=\"true\" >\n", t_end);

    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div[@id='cc']/div[@class='right']/span/text()", xpathCtx);
    /* added temperature */
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[0] && xpathObj->nodesetval->nodeTab[0]->content){
        /* fprintf (stderr, "temperature %s\n", xpathObj->nodesetval->nodeTab[0]->content); */
        snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj->nodesetval->nodeTab[0]->content);
        memset(temp_buffer, 0, sizeof(temp_buffer));
        memset(temp_buffer2, 0, sizeof(temp_buffer2));
        for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
            if (buffer[j] == '&')
               break;
            if ((uint)buffer[j] == 226 ||  buffer[j] == '-' || 
                     (buffer[j]>='0' && buffer[j]<='9')){
                if ((uint)buffer[j] == 226){
                    if (temp_buffer[0] != 0){
                        sprintf(temp_buffer2,"%s",temp_buffer);
                        sprintf(temp_buffer,"%s-",temp_buffer2);
                    }else{
                        sprintf(temp_buffer,"%s-",temp_buffer);
                    }
                }else{
                    if (temp_buffer[0] != 0){
                        sprintf(temp_buffer2,"%s",temp_buffer);
                        sprintf(temp_buffer,"%s%c",temp_buffer2, buffer[j]);
                    }else{
                        sprintf(temp_buffer,"%c", buffer[j]);
                    }
                }
            }
        }
        /* fprintf(stderr, "     <temperature>%s</temperature>\n", temp_buffer); */
        fprintf(file_out,"     <temperature>%s</temperature>\n", temp_buffer); 
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div[@id='cc']/div[@class='right wind']/div/@class", xpathCtx);
    /*         fprintf(stderr,"sss %s", xpathObj->nodesetval->nodeTab[0]->children->content); */
    /* added wind direction */
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[0] && 
        xpathObj->nodesetval->nodeTab[0]->children->content){
             temp_char = strstr((char*)xpathObj->nodesetval->nodeTab[0]->children->content, " ");
             temp_char++;
             temp_char++;
             memset(buff, 0, sizeof(buff));
             switch (atoi(temp_char)){
                case 0:
                    snprintf(buff, sizeof(buff)-1,"N");
                    break;
                case 45:
                    snprintf(buff, sizeof(buff)-1,"NE");
                    break;
                case 90:
                    snprintf(buff, sizeof(buff)-1,"E");
                    break;
                case 135:
                    snprintf(buff, sizeof(buff)-1,"SE");
                    break;
                case 180:
                    snprintf(buff, sizeof(buff)-1,"S");
                    break;
                case 225:
                    snprintf(buff, sizeof(buff)-1,"SW");
                    break;
                case 270:
                    snprintf(buff, sizeof(buff)-1,"W");
                    break;
                case 315:
                    snprintf(buff, sizeof(buff)-1,"NW");
                    break;
                case 360:
                    snprintf(buff, sizeof(buff)-1,"N");
                    break;
                default:
                    break;
             }
             /* fprintf(stderr, "Wind  direction  .%s.  \n", xpathObj->nodesetval->nodeTab[0]->children->content); */
             if (strlen(buff)>0)
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",  buff);
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div[@id='cc']/div[@class='right wind']/strong/text()", xpathCtx);
    /* added wind speed */
     if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
         xpathObj->nodesetval->nodeTab[0] && xpathObj->nodesetval->nodeTab[0]->content){
        /* fprintf(stderr, "Wind  direction  %s  \n", xpathObj4->nodesetval->nodeTab[i]->children->content);  */
        fprintf(file_out,"     <wind_speed>%i</wind_speed>\n",  
                               atoi((const char*)xpathObj->nodesetval->nodeTab[0]->content));
     }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div[@id='cc']/div[@class='left']/img/@src", xpathCtx);
    /* added icon */
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[0] && 
        xpathObj->nodesetval->nodeTab[0]->children->content){
       temp_char = strrchr((char*)xpathObj->nodesetval->nodeTab[0]->children->content, '/');
       temp_char ++;
       /*  fprintf (stderr, "icon %s %s \n", xpathObj6->nodesetval->nodeTab[i]->children->content, choose_hour_weather_icon(hash_for_icons, temp_char)); */ 
//       fprintf(file_out,"     <icon>%s</icon>\n",  
//                              choose_hour_weather_icon(temp_char).toStdString().c_str());
       if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_char)){
            fprintf(file_out,"     <icon>%s</icon>\n",  
             (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_char));
       }else 
            fprintf(file_out,"     <icon>49</icon>\n");  

    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);
    
    index = 1;
//    snprintf(buffer, sizeof(buffer)-1,"/html/body/div[@id='cc']/div[@class='cctext']/p/strong/text()", index);
    snprintf(buffer, sizeof(buffer)-1,"/html/body/div[@id='cc']/div[@class='cctext']/p[not(contains(text(), ':'))]/strong/text()");
    xpathObj = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    /* added text */
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[0] && xpathObj->nodesetval->nodeTab[0]->content){
            /* fprintf (stderr, "description %s\n", xpathObj7->nodesetval->nodeTab[i]->content); */
            fprintf(file_out,"     <description>%s</description>\n", xpathObj->nodesetval->nodeTab[0]->content);
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);
/*    
    snprintf(buffer, sizeof(buffer)-1,"/html/body/div[@id='cc']/div[@class='cctext']/p/strong[1]/text()");
    xpathObj = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[1] && xpathObj->nodesetval->nodeTab[1]->content){
        fprintf(file_out,"     <flike>%i</flike>\n", atoi((char*)xpathObj->nodesetval->nodeTab[1]->content)); 
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    snprintf(buffer, sizeof(buffer)-1,"/html/body/div[@id='cc']/div[@class='cctext']/p/strong/text()");
    xpathObj = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[2] && xpathObj->nodesetval->nodeTab[2]->content){
        fprintf(file_out,"     <humidity>%i</humidity>\n", atoi((char*)xpathObj->nodesetval->nodeTab[2]->content)); 
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    snprintf(buffer, sizeof(buffer)-1,"/html/body/div[@id='cc']/div[@class='cctext']/p/strong/text()");
    xpathObj = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[3] && xpathObj->nodesetval->nodeTab[3]->content){
        fprintf(file_out,"     <pressure>%i</pressure>\n", atoi((char*)xpathObj->nodesetval->nodeTab[3]->content)); 
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    snprintf(buffer, sizeof(buffer)-1,"/html/body/div[@id='cc']/div[@class='cctext']/p/strong/text()");
    xpathObj = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&
        xpathObj->nodesetval->nodeTab[4] && xpathObj->nodesetval->nodeTab[4]->content){
        fprintf(file_out,"     <visible>%i</visible>\n", atoi((char*)xpathObj->nodesetval->nodeTab[4]->content)*1000); 
    }
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);
*/

    snprintf(buffer, sizeof(buffer)-1,"/html/body/div[@id='cc']/div[@class='cctext']/p[2]/text()[normalize-space(.)]");
    xpathObj = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    snprintf(buffer, sizeof(buffer)-1,"/html/body/div[@id='cc']/div[@class='cctext']/p/strong/text()");
    xpathObj3 = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) &&  xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval)){
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        //fprintf(stderr, "xpathObj %i xpathObj3 %i\n", xpathObj->nodesetval->nodeNr, xpathObj3->nodesetval->nodeNr); 
        int offset = xpathObj->nodesetval->nodeNr -  xpathObj3->nodesetval->nodeNr + 1;
        if (offset < 0 || offset >1) offset = 0;
        if (xpathObj->nodesetval->nodeNr <= xpathObj3->nodesetval->nodeNr){
            for (int i=1; i <= nodeset->nodeNr; i++) {
                snprintf(buffer, sizeof(buffer)-1,"normalize-space(/html/body/div[@id='cc']/div[@class='cctext']/p[2]/text()[%i])", i);
                xpathObj2 = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
                //fprintf(stderr, "Stringval %s Content %s \n ", (const char*)xpathObj2->stringval, (char*)xpathObj3->nodesetval->nodeTab[i-offset]->content);
                if (xpathObj2){
                    if ((char*)xpathObj2->stringval && !strcmp((const char*)xpathObj2->stringval, "Humidity:")){
                        if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) &&
//                            xpathObj3->nodesetval->nodeTab[i+1] && xpathObj3->nodesetval->nodeTab[i+1]->content){
//                            fprintf(file_out,"     <humidity>%i</humidity>\n", atoi((char*)xpathObj3->nodesetval->nodeTab[i+1]->content)); 
                             xpathObj3->nodesetval->nodeTab[i-offset] && xpathObj3->nodesetval->nodeTab[i-offset]->content){
                             fprintf(file_out,"     <humidity>%i</humidity>\n", atoi((char*)xpathObj3->nodesetval->nodeTab[i-offset]->content)); 
                        }    
                    }
                    if ((char*)xpathObj2->stringval && !strcmp((const char*)xpathObj2->stringval, "Barometer:")){
                        if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) &&
//                            xpathObj3->nodesetval->nodeTab[i+1] && xpathObj3->nodesetval->nodeTab[i+1]->content){
//                            fprintf(file_out,"     <pressure>%i</pressure>\n", atoi((char*)xpathObj3->nodesetval->nodeTab[i+1]->content)); 
                            xpathObj3->nodesetval->nodeTab[i-offset] && xpathObj3->nodesetval->nodeTab[i-offset]->content){
                            fprintf(file_out,"     <pressure>%i</pressure>\n", atoi((char*)xpathObj3->nodesetval->nodeTab[i-offset]->content)); 

                        }    
                    }
                    if ((char*)xpathObj2->stringval && !strcmp((const char*)xpathObj2->stringval, "Visibility:")){
                        if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) &&
//                            xpathObj3->nodesetval->nodeTab[i+1] && xpathObj3->nodesetval->nodeTab[i+1]->content){
//                            fprintf(file_out,"     <visible>%i</visible>\n", atoi((char*)xpathObj3->nodesetval->nodeTab[i+1]->content)*1000); 
                             xpathObj3->nodesetval->nodeTab[i-offset] && xpathObj3->nodesetval->nodeTab[i-offset]->content){
                             fprintf(file_out,"     <visible>%i</visible>\n", atoi((char*)xpathObj3->nodesetval->nodeTab[i-offset]->content)*1000); 
                       }
                    }
                    if ((char*)xpathObj2->stringval && !strcmp((const char*)xpathObj2->stringval, "Feels Like:")){
                        if (xpathObj3 && !xmlXPathNodeSetIsEmpty(xpathObj3->nodesetval) &&
//                            xpathObj3->nodesetval->nodeTab[i+1] && xpathObj3->nodesetval->nodeTab[i+1]->content){
//                            fprintf(file_out,"     <flike>%i</flike>\n", atoi((char*)(char*)xpathObj3->nodesetval->nodeTab[i+1]->content)); 
                            xpathObj3->nodesetval->nodeTab[i-offset] && xpathObj3->nodesetval->nodeTab[i-offset]->content){
                            fprintf(file_out,"     <flike>%i</flike>\n", atoi((char*)(char*)xpathObj3->nodesetval->nodeTab[i-offset]->content)); 
                        }    
                    }
                }
                if (xpathObj2)
                    xmlXPathFreeObject(xpathObj2);
            }
            if (xpathObj3)
                xmlXPathFreeObject(xpathObj3);
        }
    } 
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    fprintf(file_out,"    </period>\n");


    //return 1;

    /* To DO sunrise and sunset */

    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div[@class='hourlyfc']/*[@class='symbcol']/preceding-sibling::div[@class='timecol']/p/strong/text()", xpathCtx);
    snprintf(buff_day, sizeof(buff_day)-1,"%s", (const char*)xpathObj->nodesetval->nodeTab[0]->content);
    /* fprintf(stderr, "buff_day %s\n", buff_day); */
    if (xpathObj)
        xmlXPathFreeObject(xpathObj);

    snprintf(buffer, sizeof(buffer)-1,"(/html/body/div[@class='hourlyfc']/*[@class='symbcol']/preceding-sibling::div[@class='timecol']/p/strong/text() | /html/body/div[@class='hourlyfc']/*[@class='symbcol']/preceding-sibling::div[@class='timecol']/p/text()  | /html/body/div[@class='hourlyfc']/*[@class='symbcol']/img/@title | /html/body/div[@class='hourlyfc']/*[@class='symbcol']/img/@src | /html/body/div[@class='hourlyfc']/*[@class='symbcol']/following-sibling::div/span/text() | /html/body/div[@class='hourlyfc']/*[@class='symbcol']/following-sibling::div/div/@class | /html/body/div[@class='hourlyfc']/*[@class='symbcol']/following-sibling::div/strong/text() ) ");
    xpathObj2 = xmlXPathEvalExpression((const xmlChar*)buffer, xpathCtx);
    k = 0; 
       
   nodes   = xpathObj2->nodesetval;
   flag = true;
   for (j = 0; j <(nodes->nodeNr); ++j){
      if (k >6)
           flag = false;
       
      if (flag){
           switch (k){
                case 1: {
                    current_time = time(NULL);
                    tm = localtime(&current_time);
                    snprintf(buffer, sizeof(buffer)-1,"%i %i %s", month + 1, day, xpathObj2->nodesetval->nodeTab[j]->content);
                    /* fprintf(stderr," Buffer %s\n", buffer); */
                    setlocale(LC_TIME, "POSIX");
                    strptime((const char*)buffer, "%m %d %H:%M", &tmp_tm);
                    setlocale(LC_TIME, "");
                    /* set begin of day in localtime */
                    tmp_tm.tm_year = tm->tm_year;

                    t_start = mktime(&tmp_tm) - 3600*remotetimezone + 3600*localtimezone;
                    if (strcmp((const char*)xpathObj2->nodesetval->nodeTab[j-1]->content, buff_day)){
                        t_start = t_start + 24*3600;
                    }
                    fprintf(file_out,"    <period start=\"%li\"", (t_start));
                    /* set end of current time in localtime */
                    t_end = t_start + 3600*3 - 1;
                    fprintf(file_out," end=\"%li\" hour=\"true\">\n", t_end);
                    break;
                    }
                case 2: {
                        if (xpathObj2->nodesetval->nodeTab[j] && xpathObj2->nodesetval->nodeTab[j]->children && xpathObj2->nodesetval->nodeTab[j]->children->content && strlen((char*)xpathObj2->nodesetval->nodeTab[j]->children->content)>0){
                            temp_char = strrchr((char*)xpathObj2->nodesetval->nodeTab[j]->children->content, '/');
                            temp_char ++;
                           // fprintf(file_out,"     <icon>%s</icon>\n",  
                           // choose_hour_weather_icon(temp_char).toStdString().c_str());
                           if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_char)){
                                fprintf(file_out,"     <icon>%s</icon>\n",  
                                 (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_char));
                           }else 
                                fprintf(file_out,"     <icon>49</icon>\n");  
                         } 
                }
                break;
                case 3: {
                    if (strlen((char*)xpathObj2->nodesetval->nodeTab[j]->children->content)>0){
                        fprintf(file_out,"     <description>%s</description>\n", xpathObj2->nodesetval->nodeTab[j]->children->content);
                    }
            }
            break;

            case 4: {
                    /* added temperature */
                    if (xpathObj2->nodesetval->nodeTab[j]->content){
                        snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj2->nodesetval->nodeTab[j]->content);
                        memset(temp_buffer, 0, sizeof(temp_buffer));
                        memset(temp_buffer2, 0, sizeof(temp_buffer2));
                        for (l = 0 ; (l<(strlen(buffer)) && l < buff_size); l++ ){
                            if (buffer[l] == '&')
                               break;
                            if ((uint)buffer[l] == 226 ||  buffer[l] == '-' ||
-                                (buffer[l]>='0' && buffer[l]<='9')){
                                if ((uint)buffer[l] == 226){
                                    if (temp_buffer[0] != 0){
                                        sprintf(temp_buffer2,"%s",temp_buffer);
                                        sprintf(temp_buffer,"%s-",temp_buffer2);
                                    }else{
                                        sprintf(temp_buffer,"%s-",temp_buffer);
                                    }
                                }else{
                                    if (temp_buffer[0] != 0){
                                        sprintf(temp_buffer2,"%s",temp_buffer);
                                        sprintf(temp_buffer,"%s%c",temp_buffer2, buffer[l]);
                                    }else{
                                        sprintf(temp_buffer,"%c", buffer[l]);
                                    }
                                }
                            }
                        }
                        fprintf(file_out,"     <temperature>%s</temperature>\n", temp_buffer); 
                    }
                    break;
                }
            case 5: {
                if (strlen((char*)xpathObj2->nodesetval->nodeTab[j]->children->content)>0){
                    temp_char = strstr((char*)xpathObj2->nodesetval->nodeTab[j]->children->content, " ");
                    temp_char++;
                    temp_char++;
                    memset(buff, 0, sizeof(buff));
                    switch (atoi(temp_char)){
                        case 0:
                            snprintf(buff, sizeof(buff)-1,"N");
                            break;
                        case 45:
                            snprintf(buff, sizeof(buff)-1,"NE");
                            break;
                        case 90:
                            snprintf(buff, sizeof(buff)-1,"E");
                            break;
                        case 135:
                            snprintf(buff, sizeof(buff)-1,"SE");
                            break;
                        case 180:
                            snprintf(buff, sizeof(buff)-1,"S");
                            break;
                        case 225:
                            snprintf(buff, sizeof(buff)-1,"SW");
                            break;
                        case 270:
                            snprintf(buff, sizeof(buff)-1,"W");
                            break;
                        case 315:
                            snprintf(buff, sizeof(buff)-1,"NW");
                            break;
                        case 360:
                            snprintf(buff, sizeof(buff)-1,"N");
                            break;
                        default:
                            break;
                    }
                    if (strlen(buff)>0)
                        fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",  buff);
                 }
                 break;
              }
           case 6: {
                if (strlen((char*)xpathObj2->nodesetval->nodeTab[j]->content)>0){
                    fprintf(file_out,"     <wind_speed>%s</wind_speed>\n",  xpathObj2->nodesetval->nodeTab[j]->content);
                }
                k = -1;
                fprintf(file_out,"    </period>\n");
            }
            break;
           }
        k++;
       }
    }
    fclose(file_out);
    if (xpathObj2)
        xmlXPathFreeObject(xpathObj2);
    xmlHashFree(hash_for_icons, NULL);
    xmlXPathFreeContext(xpathCtx); 
    return 1;
}


/*******************************************************************************/
int
parse_and_write_xml_data(const char *station_id, htmlDocPtr doc, const char *result_file){
    char        buff[256],
                buffer[buff_size];
    char        temp_buffer[buff_size];
    char        temp_buffer2[buff_size];
    int         size;
    int         i,j;
    xmlXPathContextPtr xpathCtx; 
    xmlXPathObjectPtr xpathObj = NULL; 
    xmlXPathObjectPtr xpathObj2 = NULL; 
    xmlXPathObjectPtr xpathObj3 = NULL; 
    xmlXPathObjectPtr xpathObj4 = NULL; 
    xmlXPathObjectPtr xpathObj5 = NULL; 
    xmlXPathObjectPtr xpathObj6 = NULL; 
    xmlXPathObjectPtr xpathObj7 = NULL; 
    xmlNodeSetPtr nodes;
    char       *temp_char;
    bool   possible_new_year = false;

    struct tm   tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
    struct tm   *tm;
    time_t      t_start = 0, t_end = 0,
                current_time = 0;
    FILE        *file_out;

    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;
    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", station_id);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    hash_for_icons = hash_icons_forecacom_table_create();
    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
         return(-1);
    }
    /* Register namespaces from list (if any) */
    xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"html",
                                (const xmlChar*)"http://www.w3.org/1999/xhtml");
    /* Day weather forecast */
    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/*/div[(@data-ts)]/@data-ts", xpathCtx);
//    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/div/div/table//tr/th[@colspan='3']", xpathCtx);
  
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", "/html/body/*/div[(@data-ts)]/@data-ts");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
    }

    nodes   = xpathObj->nodesetval;
    size = (nodes) ? nodes->nodeNr : 0;
    if (size > 14)
        size = 14;
    /* fprintf(stderr, "SIZE!!!!!!!!!!!!!!: %i\n", size); */
    xpathObj2 = xmlXPathEvalExpression((const xmlChar*)"/html/body/*/div[(@data-ts)]/div/span[(@class='temp_max temp_warm') or (@class='temp_max temp_cold')]/text()", xpathCtx);
    xpathObj3 = xmlXPathEvalExpression((const xmlChar*)"/html/body/*/div[(@data-ts)]/div/span[(@class='temp_min')]/text()", xpathCtx);
    xpathObj4 = xmlXPathEvalExpression((const xmlChar*)"/html/body/*/div[(@data-ts)]/div/div/@class", xpathCtx);
    xpathObj5 = xmlXPathEvalExpression((const xmlChar*)"/html/body/*/div[(@data-ts)]/div/strong/text()", xpathCtx);
    xpathObj6 = xmlXPathEvalExpression((const xmlChar*)"/html/body/*/div[(@data-ts)]/div/img/@src", xpathCtx);
    xpathObj7 = xmlXPathEvalExpression((const xmlChar*)"/html/body/*/div[(@data-ts)]/div/img/@title", xpathCtx);
  
    /* fprintf(stderr, "Result (%d nodes):\n", size); */
    for(i = 0; i < size; ++i) {

         /* Take time: */
         if (!nodes->nodeTab[i]->children->content){
             fprintf(stderr,"ERRROR");
             continue;
         }
         /* fprintf(stderr," TEXT %s\n", nodes->nodeTab[i]->children->content); */ 
         current_time = time(NULL);
         tm = localtime(&current_time);

         setlocale(LC_TIME, "POSIX");
         strptime((const char*)nodes->nodeTab[i]->children->content, "%Y%m%d", &tmp_tm);
         setlocale(LC_TIME, "");
         /* set begin of day in localtime */
         tmp_tm.tm_year = tm->tm_year;
         tmp_tm.tm_hour = 0; tmp_tm.tm_min = 0; tmp_tm.tm_sec = 0;
         if (!possible_new_year && tmp_tm.tm_mon == 11 && tmp_tm.tm_mday > 15){
            possible_new_year = true;
         }
         if (possible_new_year && tmp_tm.tm_mon == 0 && tmp_tm.tm_mday < 15){
            tmp_tm.tm_year++;
         }

         t_start = mktime(&tmp_tm);
         fprintf(file_out,"    <period start=\"%li\"", t_start);
         /* set end of day in localtime */
         t_end = t_start + 3600*24 - 1;
         fprintf(file_out," end=\"%li\">\n", t_end);
     
         /* added hi temperature */
         if (xpathObj2 && !xmlXPathNodeSetIsEmpty(xpathObj2->nodesetval) &&
             xpathObj2->nodesetval->nodeTab[i] && xpathObj2->nodesetval->nodeTab[i]->content){
             /* fprintf (stderr, "temperature %s\n", xpathObj2->nodesetval->nodeTab[i]->content); */
             snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj2->nodesetval->nodeTab[i]->content);
             memset(temp_buffer, 0, sizeof(temp_buffer));
             memset(temp_buffer2, 0, sizeof(temp_buffer2));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if (buffer[j] == '&')
                    break;
                 if ((uint)buffer[j] == 226 ||  buffer[j] == '-' || 
                     (buffer[j]>='0' && buffer[j]<='9')){

                     if ((uint)buffer[j] == 226){
                        sprintf(temp_buffer2,"%s",temp_buffer);
                        sprintf(temp_buffer,"%s-",temp_buffer2);
                     }else{
                        sprintf(temp_buffer2,"%s",temp_buffer);
                        sprintf(temp_buffer,"%s%c",temp_buffer2, buffer[j]);
                     }
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
             memset(temp_buffer2, 0, sizeof(temp_buffer2));
             for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
                 if (buffer[j] == '&')
                    break;
                 if ((uint)buffer[j] == 226 ||  buffer[j] == '-' ||
                     (buffer[j]>='0' && buffer[j]<='9')){
                     if ((uint)buffer[j] == 226){
                        sprintf(temp_buffer2,"%s",temp_buffer);
                        sprintf(temp_buffer,"%s-",temp_buffer2);
                     }else{
                        sprintf(temp_buffer2,"%s",temp_buffer);
                        sprintf(temp_buffer,"%s%c",temp_buffer2, buffer[j]);
                     }
                 }
             }
             fprintf(stderr, "     <temperature>%s</temperature>\n", temp_buffer);
             fprintf(file_out,"     <temperature_low>%s</temperature_low>\n", temp_buffer); 
         }
         /* added wind direction */
         if (xpathObj4 && !xmlXPathNodeSetIsEmpty(xpathObj4->nodesetval) &&
             xpathObj4->nodesetval->nodeTab[i] && 
             xpathObj4->nodesetval->nodeTab[i]->children->content){
             temp_char = strstr((char*)xpathObj4->nodesetval->nodeTab[i]->children->content, " ");
             temp_char++;
             temp_char++;
             memset(buff, 0, sizeof(buff));
             switch (atoi(temp_char)){
                case 0:
                    snprintf(buff, sizeof(buff)-1,"N");
                    break;
                case 45:
                    snprintf(buff, sizeof(buff)-1,"NE");
                    break;
                case 90:
                    snprintf(buff, sizeof(buff)-1,"E");
                    break;
                case 135:
                    snprintf(buff, sizeof(buff)-1,"SE");
                    break;
                case 180:
                    snprintf(buff, sizeof(buff)-1,"S");
                    break;
                case 225:
                    snprintf(buff, sizeof(buff)-1,"SW");
                    break;
                case 270:
                    snprintf(buff, sizeof(buff)-1,"W");
                    break;
                case 315:
                    snprintf(buff, sizeof(buff)-1,"NW");
                    break;
                case 360:
                    snprintf(buff, sizeof(buff)-1,"N");
                    break;
                default:
                    break;
             }
             /* fprintf(stderr, "Wind  direction %s %s  \n",temp_char, buff); */
             if (strlen(buff)>0)
                 fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", buff);
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
            //fprintf(file_out,"     <icon>%s</icon>\n",  
            //                       choose_hour_weather_icon(temp_char).toStdString().c_str());

             if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_char)){
                fprintf(file_out,"     <icon>%s</icon>\n",  
                     (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_char));
             }else 
                fprintf(file_out,"     <icon>49</icon>\n");  


         }
         /* added text */
         if (xpathObj7 && !xmlXPathNodeSetIsEmpty(xpathObj7->nodesetval) &&
             xpathObj7->nodesetval->nodeTab[i] && xpathObj7->nodesetval->nodeTab[i]->children->content){
            snprintf(buffer, sizeof(buffer)-1,"%s", xpathObj7->nodesetval->nodeTab[i]->children->content);
            memset(temp_buffer, 0, sizeof(temp_buffer));
            memset(temp_buffer2, 0, sizeof(temp_buffer2));
            for (j = 0 ; (j<(strlen(buffer)) && j < buff_size); j++ ){
               if (buffer[j] == 13 || buffer[j] == 10)
                    continue;
               sprintf(temp_buffer2,"%s",temp_buffer);
               sprintf(temp_buffer,"%s%c",temp_buffer2, buffer[j]);

            }
            fprintf(file_out,"     <description>%s</description>\n", temp_buffer);
         }
         fprintf(file_out,"    </period>\n");
  }	
  /* Cleanup */
  fclose(file_out);
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
  if (xpathCtx)
    xmlXPathFreeContext(xpathCtx);

  xmlHashFree(hash_for_icons, NULL);

  return size;
}

/*******************************************************************************/
int
convert_station_forecacom_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data ){
 
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
    int    days_number = -1;
    char   buffer[1024],
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
        doc =  htmlReadFile(station_id_with_path, "UTF-8",  HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
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
                     doc =  htmlReadFile(detail_path_data, "UTF-8",  HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
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
main_foreca_com(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "forecacom <input_file> <output_file> <input_detail_data>\n");
        return -1;
    }
    result = convert_station_forecacom_data(argv[1], argv[2], argv[3]);
    /* fprintf(stderr, "\nresult = %d\n", result); */
    return result;
}
