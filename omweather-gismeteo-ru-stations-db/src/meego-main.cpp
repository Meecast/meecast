/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-gismeteo-ru-stations-db MeeCast
 *
 * Copyright (C) 2009-2025 Vlad Vasilyeu
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
#include <iostream> 
#include "json/json.h"
#include <string.h>
#include <math.h> 
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
    /* Add Month */
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
int
get_month(char *temp_string){
    std::size_t found;
    const char* months[12]
        = { "январь", "февраль", "март", "апрель", "май", "июнь", "июль","август", "сентябрь", "октябрь", "ноябрь", "декабрь" };

    for (int i = 0; i < 12; i++){
        if (sizeof(temp_string) > sizeof(months[i]))
            continue;
        //fprintf(stderr, "compare %s %s %i %i\n", months[i], temp_string, sizeof(temp_string), strncmp(months[i], temp_string,  strlen(temp_string)));
        if (strncmp(months[i], temp_string,  strlen(temp_string)) == 0)
            return i + 1;
    }
    /* default negative result -1 */
    return -1;
}
/*******************************************************************************/
struct tm
get_date_for_current_weather(char *temp_string){
    char buffer[512];
    char temp_buffer[256];
    char temp_buffer2[256];
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
    /* fprintf(stderr, "Buffer %s\n", buff); */
    strptime(buff, "%m-%d %Y %H:%M", &tmp_tm);
    /* fprintf(stderr, "\ntmp_tm hour %d\n", (&tmp_tm)->tm_hour); */
    return tmp_tm;
}
/*******************************************************************************/
int
gismeteoru_parse_and_write_xml_data(const char *station_id, htmlDocPtr doc, const char *result_file){
    char        buffer[buff_size],
                current_temperature[20],
                current_icon[10],
                current_title[1024],
                current_pressure[15],
                current_humidity[15],
                current_wind_direction[15],
                current_wind_speed[15];
    char        temp_buffer[buff_size];
    char        temp_buffer2[buff_size];
    int         month = -1;
    int         day_of_month = -1;
    int         year = -1;
#ifdef GLIB
    GSList      *forecast = NULL;
    GSList      *tmp = NULL;
    GHashTable  *day = NULL;
    GHashTable *hash_for_translate;
    GHashTable *hash_for_icons;
#endif
    int    size;
    int    i, j;
    xmlXPathContextPtr xpathCtx; 
    xmlXPathObjectPtr xpathObj = NULL; 
    xmlXPathObjectPtr xpathObj_day = NULL; 
    xmlXPathObjectPtr xpathObj_date = NULL; 
    xmlXPathObjectPtr xpathObjTemp = NULL; 
    xmlXPathObjectPtr xpathObjIcons = NULL; 
    xmlXPathObjectPtr xpathObjDescription = NULL; 
    xmlXPathObjectPtr xpathObjMaxPressure = NULL; 
    xmlXPathObjectPtr xpathObjWindSpeed = NULL; 
    xmlXPathObjectPtr xpathObjWindGust = NULL; 
    xmlXPathObjectPtr xpathObjWindDirection = NULL; 
    xmlXPathObjectPtr xpathObjHumidity = NULL; 
    xmlXPathObjectPtr xpathObjUv = NULL; 
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
    time_t      current_time = 0;
    FILE        *file_out;
    int         localtimezone;
    struct tm time_tm1;
    struct tm time_tm2;


    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    Json::Value val;
    Json::Value nullval ;
    std::string wind_directions[17] = {"N","NNE","NE","ENE","E","ESE","SE","SSE","S","SSW","SW","WSW","W","WNW","NW","NNW","N"};

    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;

    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 
    /* fprintf(stderr,"Local Time Zone %i\n", localtimezone); */

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

    /* Current weather */
    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/head/script/text()", xpathCtx);
    nodes   = xpathObj->nodesetval;
    size = (nodes) ? nodes->nodeNr : 0;

    fprintf(stderr,"Count of scripts %i\n", size);
    for(i = 0; i < size; ++i) {
        if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) && xpathObj->nodesetval->nodeTab[i]->content){
            temp_char = ((char *)xpathObj->nodesetval->nodeTab[i]->content);
            std::string buffer_0(temp_char);
            size_t index_0 = buffer_0.find("window.M.state = ", 0);
            if (index_0  != std::string::npos){
                std::string buffer_keys_and_values = buffer_0.substr(index_0 + 17, buffer_0.length());
                //std::cerr<<buffer_keys_and_values<<std::endl;
                bool parsingSuccessful = reader.parse(buffer_keys_and_values, root, false);
                if (parsingSuccessful){
                    val = root["city"];
                    auto timezone_json = val["timeZone"].asInt();
                    std::cerr<<timezone_json<<std::endl;
                    fprintf(file_out,"    <timezone>%i</timezone>\n", timezone_json/60);
                    auto utc = val["dates"]["utc"].asString();
                    /* "2024-12-07T15:51:53.282Z" */
                    tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
                    tmp_tm.tm_isdst = time_tm2.tm_isdst;
                    setlocale(LC_TIME, "POSIX");
                    strptime((const char*)utc.c_str(), "%Y-%m-%dT%H:%M:%S", &tmp_tm);
                    utc_time = mktime(&tmp_tm); 
                    setlocale(LC_TIME, "");
                    fprintf(file_out,"    <period start=\"%li\" current=\"true\"", utc_time + 3600*localtimezone);
       
                    fprintf(file_out," end=\"%li\">\n", utc_time + 3*3600 + 3600*localtimezone); 
                    std::cerr<<utc<<std::endl;

                    val = root["weather"]["cw"];
                    auto description = val["description"][0].asString();
                    fprintf(file_out,"     <description>%s</description>\n", description.c_str());
                    auto humidity = val["humidity"][0].asInt();
                    fprintf(file_out,"     <humidity>%i</humidity>\n", humidity);
                    auto pressure = val["pressure"][0].asInt();
                    fprintf(file_out,"     <pressure>%0.f</pressure>\n", pressure*1.333224);
                    auto precipitation = val["precipitation"][0].asDouble();
                    fprintf(file_out,"     <precipitation>%.1f</precipitation>\n", precipitation);

                    auto radiation = val["radiation"][0].asInt();
                    fprintf(file_out,"     <uv_index>%i</uv_index>\n", radiation);

                    auto temperatureAir = val["temperatureAir"][0].asInt();
                    fprintf(file_out,"     <temperature>%i</temperature>\n", temperatureAir);
                    auto temperatureHeatIndex = val["temperatureHeatIndex"][0].asInt();
                    fprintf(file_out,"     <flike>%i</flike>\n", temperatureHeatIndex);
                    auto windDirection = val["windDirection"][0].asInt();
                    int wind_index = (int)round(windDirection/22.5) + 1;
                    //std::cout<<"Wind_index "<<wind_index<<std::endl;
                    if (wind_index > 16){
                        wind_index = 16;
                    }
                    //std::cout<<"Wind_direction "<<wind_directions[wind_index].c_str()<<std::endl;
                    fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",wind_directions[wind_index].c_str());

                    auto windSpeed = val["windSpeed"][0].asInt();
                    fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", windSpeed);
                    auto windGust = val["windGust"][0].asInt();
                    fprintf(file_out,"     <wind_gust>%i</wind_gust>\n", windGust);
                    auto iconWeather = val["iconWeather"][0].asString();
                    if (xmlHashLookup(hash_for_icons, (const xmlChar*)iconWeather.c_str())){
                       snprintf(current_icon, sizeof(current_icon)-1,"%s",
                        (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)iconWeather.c_str()));
                        fprintf(file_out,"     <icon>%s</icon>\n",  current_icon);
                    }else{
                        fprintf(file_out,"     <icon>49</icon>\n");
                        printf("Current icon name: %s not found\n", iconWeather.c_str());
                    }
                    fprintf(file_out, "    </period>\n");
                }else{
                    std::cerr<<"Problem in parsingSuccessful";
                }
                break; /* Exit from cycle */
            }else{
                std::cerr<<"Error in index_0"<<std::endl;
                continue;
            }
        }
    }

    /* Day weather forecast */
    /* Evaluate xpath expression */
    xpathObj_day = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/div[1]/section[2]/div[1]/div/div/*//div[@class='day']", xpathCtx);
    xpathObj_date = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/div[1]/section[2]/div[1]/div/div/*//div[@class='date']", xpathCtx);
    if(xpathObj_day == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", "/html/body/main/div[1]/section[2]/div[1]/div/div/*//div[@class='day']");
        xmlXPathFreeContext(xpathCtx); 
        return(-1);
    }

    nodes   = xpathObj_day->nodesetval;
    size = (nodes) ? nodes->nodeNr : 0;
    fprintf(stderr, "SIZE!!!!!!!!!!!!!!: %i\n", size); 
    xpathObjTemp = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row-chart widget-row-chart-temperature-air row-with-caption']/div/div[@class='values']/div[@class='value']", xpathCtx);

    xpathObjIcons = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/div[1]/section[2]/div[1]/div/div/div[2]/div/div[@class='weather-icon-group']", xpathCtx);
    xpathObjDescription = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/div[1]/section[2]/div[1]/div/div/div[2]/div[@data-tooltip]", xpathCtx);
    xpathObjMaxPressure = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row-chart widget-row-chart-pressure row-with-caption'  ]/div/div[@class='values']/div[@class='value']/div[@class='maxt']/pressure-value", xpathCtx);
    xpathObjWindSpeed = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-wind row-wind-speed row-with-caption'  ]/div//speed-value", xpathCtx);
    //xpathObjWindGust = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-wind row-wind-gust row-with-caption'  ]/div//speed-value", xpathCtx);
    xpathObjWindGust = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-wind row-wind-gust row-with-caption'  ]/div[@class='row-item']", xpathCtx);
    xpathObjWindDirection = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-wind row-wind-direction row-with-caption'  ]/div//div[@class='direction']/text()", xpathCtx);
    xpathObjHumidity = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-humidity row-with-caption']/div/text()", xpathCtx);
    xpathObjUv = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-radiation row-with-caption']/div/text()", xpathCtx);
    fprintf(stderr, "Result (%d nodes):\n", size); 
    for(i = 0; i < size; ++i) {
#ifdef GLIB
      day = NULL;
#endif
        day_of_month = -1;
        if (!nodes->nodeTab[i]->children->content)
            continue;
        if (xpathObj_date && !xmlXPathNodeSetIsEmpty(xpathObj_date->nodesetval) &&
               xpathObj_date->nodesetval->nodeTab[i] && xpathObj_date->nodesetval->nodeTab[i]->children){
            day_of_month = atoi((char *)xpathObj_date->nodesetval->nodeTab[i]->children->content);
            /* If this is the first day of the row then, look for the month */
            if (i == 0){
                memset(temp_buffer, 0, sizeof(temp_buffer));
                /* Find first separate - Space */
                char *temp_point = strchr((char *)xpathObj_date->nodesetval->nodeTab[i]->children->content,' ');
                if (temp_point){
                    snprintf(temp_buffer, strlen(temp_point),"%s", temp_point + 1);
                }
                month = get_month(temp_buffer);
                /* Current year */
                time_t seconds=time(NULL);
                struct tm* current_time=localtime(&seconds);
                year = current_time->tm_year + 1900;
            }else{
                /* Check transition to the next month and year */
                if (day_of_month == 1){
                    if (month == 12){
                        month = 1;
                        year ++;
                    }else{
                        month++;
                    }
                }
            }
        }
        if (month == -1)
            continue;
        /* Set time of period */
        setlocale(LC_TIME, "POSIX");
        memset(temp_buffer, 0, sizeof(temp_buffer));
        snprintf(temp_buffer, sizeof(temp_buffer)-1,"%i  %i %i", day_of_month, month, year);
        tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
        strptime(temp_buffer, "%d %m %Y", &tmp_tm);
        setlocale(LC_TIME, "");
        //utc_time = mktime(&tmp_tm) + localtimezone*3600 + 1;
        utc_time = mktime(&tmp_tm);
        fprintf(file_out,"    <period start=\"%li\"", utc_time);
        fprintf(file_out," end=\"%li\">\n", utc_time + 24*3600); 

        memset(buffer, 0, sizeof(buffer));
        /* added temperature */
        bool flag_mint = false;
        if (xpathObjTemp && !xmlXPathNodeSetIsEmpty(xpathObjTemp->nodesetval) &&
            xpathObjTemp->nodesetval->nodeTab[i] && xpathObjTemp->nodesetval->nodeTab[i]->children){
            for (xmlNodePtr iter_node = xpathObjTemp->nodesetval->nodeTab[i]->children; iter_node; iter_node = iter_node->next){
               if (iter_node->type == XML_ELEMENT_NODE){
                    xmlChar *temp_xml = (xmlChar *)xmlGetProp(iter_node, (xmlChar*) "class");
                    if (strncmp((char *)temp_xml, "maxt",  strlen((char *)temp_xml)) == 0){
                        if (strncmp( (char*)iter_node->children->name,"temperature-value", strlen((char *)iter_node->children->name)) == 0){
                           xmlChar *temp_xml1 = xmlGetProp(iter_node->children, (xmlChar*) "value");
                           if (temp_xml1){
                               snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml1);
                               fprintf(file_out,"     <temperature_hi>%s</temperature_hi>\n", buffer); 
                           }
                           xmlFree(temp_xml1);
                        }
                    }
                    if (strncmp((char *)temp_xml, "mint",  strlen((char *)temp_xml)) == 0){
                        if (strncmp( (char*)iter_node->children->name,"temperature-value", strlen((char *)iter_node->children->name)) == 0){
                           xmlChar *temp_xml1 = xmlGetProp(iter_node->children, (xmlChar*) "value");
                           if (temp_xml1){
                               flag_mint = true;
                               snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml1);
                               fprintf(file_out,"     <temperature_low>%s</temperature_low>\n", buffer);
                           }
                           xmlFree(temp_xml1);
                        }
                    }
                    xmlFree(temp_xml);
                }
            }
        }
        if (!flag_mint && strlen(buffer)>0){
            fprintf(file_out,"     <temperature_low>%s</temperature_low>\n", buffer);
        }
        /* added icon */
        memset(temp_buffer, 0, sizeof(temp_buffer));
        xmlNodePtr _node = xpathObjIcons->nodesetval->nodeTab[i];
        for (xmlNodePtr iter_node = _node->children; iter_node; iter_node = iter_node->next){
           if (iter_node->type == XML_ELEMENT_NODE){
                //printf("node type: Element, name: %s\n", iter_node->name);
                if (strncmp((char *)iter_node->name, "svg",  strlen((char *)iter_node->name)) == 0){
                    for (xmlNodePtr iter_node2 = iter_node->children; iter_node2; iter_node2 = iter_node2->next){
                        if (strncmp((char *)iter_node2->name, "use",  strlen((char *)iter_node2->name)) == 0){
                           //printf("node2 type: Element, name: %s\n", iter_node2->name);
                           xmlChar *namehref = (xmlChar *)xmlGetProp(iter_node2, (xmlChar*) "href");
                           //printf("href name: %s\n", namehref);
                           if (strlen(temp_buffer) == 0) {
                               strcat(temp_buffer, (char *)namehref + 1);
                           }else{
                               char additional[] = "_";
                               strcat(temp_buffer, additional);
                               strcat(temp_buffer, (char *)namehref + 1);
                           }
                           xmlFree(namehref);
                        }
                    }
                }
            }
        }
        fprintf(stderr, "href name: %s\n", temp_buffer);
        if (xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer)){
           snprintf(current_icon, sizeof(current_icon)-1,"%s",
            (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));
            fprintf(file_out,"     <icon>%s</icon>\n",  current_icon);
        }else{
            fprintf(file_out,"     <icon>49</icon>\n");
            fprintf(stderr, "href name: %s not found\n", temp_buffer);
        }

        xmlNode *iter_node = NULL;

        /* added text */
        if (xpathObjDescription && !xmlXPathNodeSetIsEmpty(xpathObjDescription->nodesetval) &&
            xpathObjDescription->nodesetval->nodeTab[i]){
            xmlChar *temp_xml_string = NULL;
            temp_xml_string = (xmlChar*)xmlGetProp(xpathObjDescription->nodesetval->nodeTab[i], (xmlChar*) "data-tooltip");
            if (xmlHashLookup(hash_for_descriptions, (const xmlChar*)temp_xml_string)){

               fprintf(file_out,"     <description>%s</description>\n", (char*)xmlHashLookup(hash_for_descriptions, (const xmlChar*)temp_xml_string));
            }else{
                fprintf (stderr, "description %s not found\n", (char *)temp_xml_string);
                fprintf(file_out,"     <description>%s</description>\n", (const xmlChar*)temp_xml_string);
            }
            xmlFree(temp_xml_string);
        }

        /* added pressure */
        memset(buffer, 0, sizeof(buffer));
        if (xpathObjMaxPressure && !xmlXPathNodeSetIsEmpty(xpathObjMaxPressure->nodesetval) &&
            xpathObjMaxPressure->nodesetval->nodeTab[i]){
            xmlChar *temp_xml_string = NULL;
            temp_xml_string = (xmlChar*)xmlGetProp(xpathObjMaxPressure->nodesetval->nodeTab[i], (xmlChar*) "value");
            snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml_string);
            ///fprintf (stderr, "pressure %s %0.f\n",  xmlGetProp(xpathObjMaxPressure->nodesetval->nodeTab[i], (xmlChar*) "value"), atoi(buffer) * 1.33 );
            fprintf(file_out,"     <pressure>%0.f</pressure>\n", (atoi(buffer)*1.333224)); 
            //fprintf(file_out,"     <pressure>%i</pressure>\n", atoi(buffer) * 1.333224); 
            xmlFree(temp_xml_string);
        }
    /*
    if (xpathObjMaxPressure && !xmlXPathNodeSetIsEmpty(xpathObjMaxPressure->nodesetval) &&
        xpathObjMaxPressure->nodesetval->nodeNr >= (i*5+2) &&
        xpathObjMaxPressure->nodesetval->nodeTab[i*5+2] && xpathObjMaxPressure->nodesetval->nodeTab[i*5+2]->content){
        pressure = atoi((char*)xpathObjMaxPressure->nodesetval->nodeTab[i*5+2]->content);
        pressure = pressure * 1.333224;
	   fprintf(file_out,"     <pressure>%i</pressure>\n", pressure);
    }
    */
        /* added wind speed */
        memset(buffer, 0, sizeof(buffer));
        if (xpathObjWindSpeed && !xmlXPathNodeSetIsEmpty(xpathObjWindSpeed->nodesetval) &&
            xpathObjWindSpeed->nodesetval->nodeTab[i]){
           xmlChar *temp_xml_string = NULL;
           temp_xml_string = (xmlChar*)xmlGetProp(xpathObjWindSpeed->nodesetval->nodeTab[i], (xmlChar*) "value");
           /* Normalize speed to km/h from m/s */
           /* fprintf(stderr, "Wind  speed    \n"); */ 
           snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml_string);
           speed = atoi ((char *)buffer);
           fprintf(file_out,"     <wind_speed>%1.f</wind_speed>\n",  (double)(speed));
           xmlFree(temp_xml_string);
        }
        /* added wind gust */
        memset(buffer, 0, sizeof(buffer));
        if (xpathObjWindGust && !xmlXPathNodeSetIsEmpty(xpathObjWindGust->nodesetval) &&
            xpathObjWindGust->nodesetval->nodeTab[i] && xpathObjWindGust->nodesetval->nodeTab[i]->children){
            /* Normalize speed to km/h from m/s */
            xmlChar *temp_xml_string = NULL;
            temp_xml_string = xmlGetProp(xpathObjWindGust->nodesetval->nodeTab[i]->children, (xmlChar*) "class");
            if (temp_xml_string && (strncmp((char *)temp_xml_string, "item-bg",  strlen((char *)temp_xml_string)) == 0)){
               for (xmlNodePtr iter_node = xpathObjWindGust->nodesetval->nodeTab[i]->children; iter_node; iter_node = iter_node->next){
                  if (iter_node->type == XML_ELEMENT_NODE){
                    if ((strncmp((char *)iter_node->name, "speed-value",  strlen((char *)iter_node->name)) == 0)){
                        xmlChar *temp_xml_string1 = (xmlChar*)xmlGetProp(iter_node, (xmlChar*) "value");
                        snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml_string1);
                        speed = atoi ((char *)buffer);
                        fprintf(file_out,"     <wind_gust>%1.f</wind_gust>\n",  (double)(speed));
                        xmlFree(temp_xml_string1);
                     }
                  }
               }
           }
           xmlFree(temp_xml_string);
        }

        /* added wind direction */
        memset(buffer, 0, sizeof(buffer));
        if (xpathObjWindDirection && !xmlXPathNodeSetIsEmpty(xpathObjWindDirection->nodesetval) &&
            xpathObjWindDirection->nodesetval->nodeTab[i] && xpathObjWindDirection->nodesetval->nodeTab[i]->content){
            /* fprintf (stderr, "Wind direction: %s\n", xpathObjWindDirection->nodesetval->nodeTab[i]->content);  */
            snprintf(buffer, sizeof(buffer)-1,"%s", xpathObjWindDirection->nodesetval->nodeTab[i]->content);
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
        if (xpathObjHumidity && !xmlXPathNodeSetIsEmpty(xpathObjHumidity->nodesetval) &&
            xpathObjHumidity->nodesetval->nodeTab[i] && xpathObjHumidity->nodesetval->nodeTab[i]->content){
           fprintf(file_out,"     <humidity>%s</humidity>\n", (char *)xpathObjHumidity->nodesetval->nodeTab[i]->content);
        }
        /* added uv-index */
        if (xpathObjUv && !xmlXPathNodeSetIsEmpty(xpathObjUv->nodesetval) &&
            xpathObjUv->nodesetval->nodeTab[i] && xpathObjUv->nodesetval->nodeTab[i]->content){
           fprintf(file_out,"     <uv_index>%s</uv_index>\n", (char *)xpathObjUv->nodesetval->nodeTab[i]->content);
        }
        fprintf(file_out,"    </period>\n");
    }	
    /* Cleanup */
    if (xpathObj_day)
      xmlXPathFreeObject(xpathObj_day);
    if (xpathObj)
      xmlXPathFreeObject(xpathObj);
    if (xpathObj_date)
      xmlXPathFreeObject(xpathObj_date);
    if (xpathObjTemp)
      xmlXPathFreeObject(xpathObjTemp);
    if (xpathObjIcons)
      xmlXPathFreeObject(xpathObjIcons);
    if (xpathObjDescription)
      xmlXPathFreeObject(xpathObjDescription);
    if (xpathObjMaxPressure)
      xmlXPathFreeObject(xpathObjMaxPressure);
    if (xpathObjWindSpeed)
      xmlXPathFreeObject(xpathObjWindSpeed);
    if (xpathObjWindDirection)
      xmlXPathFreeObject(xpathObjWindDirection);
    if (xpathObjWindGust)
      xmlXPathFreeObject(xpathObjWindGust);
    if (xpathObjHumidity)
      xmlXPathFreeObject(xpathObjHumidity);
    if (xpathObjUv)
      xmlXPathFreeObject(xpathObjUv);

  /* Clean */
#ifdef GLIB
  g_hash_table_destroy(hash_for_translate);
  g_hash_table_destroy(hash_for_icons);
#endif
    if (xpathCtx)
        xmlXPathFreeContext(xpathCtx); 

    fclose(file_out);
    return size;
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
    int        i = 0, j = 0, k = 0;
    char       buffer[buff_size];
    char       buff[buff_size];
    char       temp_buffer[buff_size];
    char       temp_buffer2[buff_size];
    char       tmp[buff_size];
    char       *temp_char;
    char       icon[10];
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
    xmlXPathObjectPtr xpathObjTemp = NULL; 
    xmlXPathObjectPtr xpathObjFlike = NULL; 
    xmlXPathObjectPtr xpathObjIcons = NULL; 
    xmlXPathObjectPtr xpathObjDescription = NULL; 
    xmlXPathObjectPtr xpathObjMaxPressure = NULL; 
    xmlXPathObjectPtr xpathObjWindSpeed = NULL; 
    xmlXPathObjectPtr xpathObjWindGust = NULL; 
    xmlXPathObjectPtr xpathObjWindDirection = NULL; 
    xmlXPathObjectPtr xpathObjHumidity = NULL; 
    xmlXPathObjectPtr xpathObjUv = NULL; 
    xmlNodeSetPtr nodes;
    int size;
    time_t      t_start = 0, t_end = 0;
    FILE        *file_out;
    struct tm time_tm1;
    struct tm time_tm2;
    int localtimezone = 0;



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

    fprintf(stderr, "detail\n");
    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/div[1]/section[2]/div[1]/div/div/div[2]/div[@title]", xpathCtx);
    xpathObjIcons = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/div[1]/section[2]/div[1]/*//div[@class='weather-icon-group']", xpathCtx);
    xpathObjDescription = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/div[1]/section[2]/div[1]/div/div/div[3]/div[@data-tooltip]", xpathCtx);
    xpathObjMaxPressure = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/div[1]/section[2]/div[1]/*//div/div/pressure-value", xpathCtx);
    xpathObjTemp = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row-chart widget-row-chart-temperature-air row-with-caption']/div/div[@class='values']/div[@class='value']", xpathCtx);
    xpathObjFlike = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row-chart widget-row-chart-temperature-heat-index row-with-caption']/div/div[@class='values']/div[@class='value']", xpathCtx);

    xpathObjWindSpeed = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-wind row-wind-speed row-with-caption'  ]/div//speed-value", xpathCtx);
    xpathObjWindGust = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-wind row-wind-gust row-with-caption'  ]/div//speed-value", xpathCtx);
    xpathObjWindDirection = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-wind row-wind-direction row-with-caption'  ]/div//div[@class='direction']/text()", xpathCtx);
    xpathObjHumidity = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-humidity row-with-caption']/div/text()", xpathCtx);

    xpathObjUv = xmlXPathEvalExpression((const xmlChar*)"/html/body/main/*//div[@class='widget-row widget-row-radiation row-with-caption']/div/text()", xpathCtx);
    nodes   = xpathObj->nodesetval;
    size = (nodes) ? nodes->nodeNr : 0;
    fprintf(stderr, "size %i\n", size);
    for(i = 0; i < size; ++i) {
        if (xpathObj && !xmlXPathNodeSetIsEmpty(xpathObj->nodesetval)){
            //fprintf(stderr, "sss %s\n", (char *)xmlGetProp(xpathObj->nodesetval->nodeTab[i], (xmlChar*) "title"));
            xmlChar *temp_xml_string = NULL;
            temp_xml_string = xmlGetProp(xpathObj->nodesetval->nodeTab[i], (xmlChar*) "title");
            if (!temp_xml_string){
                continue;
            }
            std::string buffer_0((char *)temp_xml_string);
            xmlFree(temp_xml_string);

            //fprintf(stderr, "buffer %s\n", buffer_0.c_str());
            size_t index_0 = buffer_0.find(", ", 0);
            if (index_0  != std::string::npos){
                std::string buffer_keys_and_values = buffer_0.substr(index_0 + 2, buffer_0.length() - index_0 - 2);
                //fprintf(stderr, "index %i %s\n", index_0, buffer_keys_and_values.c_str());
                strptime(buffer_keys_and_values.c_str(), "%Y-%m-%d %H:%M:%S", &tmp_tm);
                t_start = timegm(&tmp_tm);
                fprintf(file_out,"    <period start=\"%li\"", t_start);

                /* 1 hour */
                fprintf(file_out," end=\"%li\" hour=\"true\">\n", t_start + 4*3600); 
                
                /* added icon */
                memset(temp_buffer, 0, sizeof(temp_buffer));
                xmlNodePtr _node = xpathObjIcons->nodesetval->nodeTab[i];
                for (xmlNodePtr iter_node = _node->children; iter_node; iter_node = iter_node->next){
                   if (iter_node->type == XML_ELEMENT_NODE){
                        //printf("node type: Element, name: %s\n", iter_node->name);
                        if (strncmp((char *)iter_node->name, "svg",  strlen((char *)iter_node->name)) == 0){
                            for (xmlNodePtr iter_node2 = iter_node->children; iter_node2; iter_node2 = iter_node2->next){
                                if (strncmp((char *)iter_node2->name, "use",  strlen((char *)iter_node2->name)) == 0){
                                   //printf("node2 type: Element, name: %s\n", iter_node2->name);
                                   xmlChar *namehref = (xmlChar *)xmlGetProp(iter_node2, (xmlChar*) "href");
                                   //printf("href name: %s\n", namehref);
                                   if (strlen(temp_buffer) == 0) {
                                       strcat(temp_buffer, (char *)namehref + 1);
                                   }else{
                                       char additional[] = "_";
                                       strcat(temp_buffer, additional);
                                       strcat(temp_buffer, (char *)namehref + 1);
                                   }
                                   xmlFree(namehref);
                                }
                            }
                        }
                    }
                }
                fprintf(stderr, "href name: %s\n", temp_buffer);
                if (xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer)){
                   snprintf(icon, sizeof(icon)-1,"%s",
                    (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)temp_buffer));
                    fprintf(file_out,"     <icon>%s</icon>\n",  icon);
                }else{
                    fprintf(file_out,"     <icon>49</icon>\n");
                    fprintf(stderr, "href name: %s not found\n", temp_buffer);
                }
            }
        }
        xmlNode *iter_node = NULL;

        /* added text */
        if (xpathObjDescription && !xmlXPathNodeSetIsEmpty(xpathObjDescription->nodesetval) &&
            xpathObjDescription->nodesetval->nodeTab[i]){
            xmlChar *temp_xml_string = NULL;
            temp_xml_string = xmlGetProp(xpathObjDescription->nodesetval->nodeTab[i], (xmlChar*) "data-tooltip");
            if (temp_xml_string){
                if (xmlHashLookup(hash_for_descriptions, (const xmlChar*)temp_xml_string)){
                   fprintf(file_out,"     <description>%s</description>\n", (char*)xmlHashLookup(hash_for_descriptions, (const xmlChar*)temp_xml_string));
                }else{
                    fprintf (stderr, "description %s not found\n", (char *)temp_xml_string);
                    fprintf(file_out,"     <description>%s</description>\n", (const xmlChar*)temp_xml_string);
                }
                xmlFree(temp_xml_string);
            }
        }
        /* added pressure */
        memset(buffer, 0, sizeof(buffer));
        if (xpathObjMaxPressure && !xmlXPathNodeSetIsEmpty(xpathObjMaxPressure->nodesetval) &&
            xpathObjMaxPressure->nodesetval->nodeTab[i]){
            xmlChar* temp_xml_string = xmlGetProp(xpathObjMaxPressure->nodesetval->nodeTab[i], (xmlChar*) "value");
            snprintf(buffer, sizeof(buffer)-1,"%s", (char *)temp_xml_string);
            ///fprintf (stderr, "pressure %s %0.f\n",  xmlGetProp(xpathObjMaxPressure->nodesetval->nodeTab[i], (xmlChar*) "value"), atoi(buffer) * 1.33 );
            fprintf(file_out,"     <pressure>%0.f</pressure>\n", (atoi(buffer)*1.333224)); 
            //fprintf(file_out,"     <pressure>%i</pressure>\n", atoi(buffer) * 1.333224); 
            xmlFree(temp_xml_string);
        }

        /* added temperature */
        if (xpathObjTemp && !xmlXPathNodeSetIsEmpty(xpathObjTemp->nodesetval) &&
            xpathObjTemp->nodesetval->nodeTab[i] && xpathObjTemp->nodesetval->nodeTab[i]->children){
            iter_node = xpathObjTemp->nodesetval->nodeTab[i];

            if (strncmp( (char*)iter_node->children->name,"temperature-value", strlen((char *)iter_node->children->name)) == 0){
               xmlChar* temp_xml_string = xmlGetProp(iter_node->children, (xmlChar*) "value");
               if (temp_xml_string){
                   snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml_string);
                   fprintf(file_out,"     <temperature>%s</temperature>\n", buffer); 
               }
               xmlFree(temp_xml_string);
            }
        }

        /* added temperature */
        if (xpathObjFlike && !xmlXPathNodeSetIsEmpty(xpathObjFlike->nodesetval) &&
            xpathObjFlike->nodesetval->nodeTab[i] && xpathObjFlike->nodesetval->nodeTab[i]->children){
            iter_node = xpathObjFlike->nodesetval->nodeTab[i];

            if (strncmp( (char*)iter_node->children->name,"temperature-value", strlen((char *)iter_node->children->name)) == 0){
               xmlChar* temp_xml_string = xmlGetProp(iter_node->children, (xmlChar*) "value");
               if (temp_xml_string){
                   snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml_string);
                   fprintf(file_out,"     <flike>%s</flike>\n", buffer); 
               }
               xmlFree(temp_xml_string);
            }
        }

        /* added wind speed */
        memset(buffer, 0, sizeof(buffer));
        if (xpathObjWindSpeed && !xmlXPathNodeSetIsEmpty(xpathObjWindSpeed->nodesetval) &&
            xpathObjWindSpeed->nodesetval->nodeTab[i]){
           /* Normalize speed to km/h from m/s */
           /* fprintf(stderr, "Wind  speed    \n"); */ 
           xmlChar* temp_xml_string = (xmlChar*)xmlGetProp(xpathObjWindGust->nodesetval->nodeTab[i], (xmlChar*) "value");
           snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml_string);
           speed = atoi ((char *)buffer);
           fprintf(file_out,"     <wind_speed>%1.f</wind_speed>\n",  (double)(speed));
           xmlFree(temp_xml_string);
        }
        /* added wind gust */
        memset(buffer, 0, sizeof(buffer));
        if (xpathObjWindGust && !xmlXPathNodeSetIsEmpty(xpathObjWindGust->nodesetval) &&
            xpathObjWindGust->nodesetval->nodeTab[i]){
           /* Normalize speed to km/h from m/s */
           /* fprintf(stderr, "Wind  speed    \n"); */ 
           xmlChar* temp_xml_string = (xmlChar*)xmlGetProp(xpathObjWindGust->nodesetval->nodeTab[i], (xmlChar*) "value");
           snprintf(buffer, sizeof(buffer)-1,"%s", temp_xml_string);
           speed = atoi ((char *)buffer);
           fprintf(file_out,"     <wind_gust>%1.f</wind_gust>\n",  (double)(speed));
           xmlFree(temp_xml_string);
        }
        /* added wind direction */
        memset(buffer, 0, sizeof(buffer));
        if (xpathObjWindDirection && !xmlXPathNodeSetIsEmpty(xpathObjWindDirection->nodesetval) &&
            xpathObjWindDirection->nodesetval->nodeTab[i] && xpathObjWindDirection->nodesetval->nodeTab[i]->content){
            /* fprintf (stderr, "Wind direction: %s\n", xpathObjWindDirection->nodesetval->nodeTab[i]->content);  */
            snprintf(buffer, sizeof(buffer)-1,"%s", xpathObjWindDirection->nodesetval->nodeTab[i]->content);
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
        if (xpathObjHumidity && !xmlXPathNodeSetIsEmpty(xpathObjHumidity->nodesetval) &&
            xpathObjHumidity->nodesetval->nodeTab[i] && xpathObjHumidity->nodesetval->nodeTab[i]->content){
           fprintf(file_out,"     <humidity>%s</humidity>\n", (char *)xpathObjHumidity->nodesetval->nodeTab[i]->content);
        }

        /* added uv-index */
        if (xpathObjUv && !xmlXPathNodeSetIsEmpty(xpathObjUv->nodesetval) &&
            xpathObjUv->nodesetval->nodeTab[i] && xpathObjUv->nodesetval->nodeTab[i]->content){
           fprintf(file_out,"     <uv_index>%s</uv_index>\n", (char *)xpathObjUv->nodesetval->nodeTab[i]->content);
        }

        fprintf(file_out,"    </period>\n");

    }

  if (xpathObj)
    xmlXPathFreeObject(xpathObj);
  if (xpathObj2)
    xmlXPathFreeObject(xpathObj2);
  if (xpathObjTemp)
    xmlXPathFreeObject(xpathObjTemp);
  if (xpathObjIcons)
    xmlXPathFreeObject(xpathObjIcons);
  if (xpathObjDescription)
    xmlXPathFreeObject(xpathObjDescription);
  if (xpathObjMaxPressure)
    xmlXPathFreeObject(xpathObjMaxPressure);
  if (xpathObjWindSpeed)
    xmlXPathFreeObject(xpathObjWindSpeed);
  if (xpathObjWindDirection)
    xmlXPathFreeObject(xpathObjWindDirection);
  if (xpathObjHumidity)
    xmlXPathFreeObject(xpathObjHumidity);
  if (xpathObjUv)
    xmlXPathFreeObject(xpathObjUv);
  if (xpathObjFlike)
    xmlXPathFreeObject(xpathObjFlike);
  if (xpathObjWindGust)
    xmlXPathFreeObject(xpathObjWindGust);
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
                            xmlHashFree(hash_for_icons, NULL);
                            xmlHashFree(hash_for_descriptions, NULL);
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
        if(!doc){
            xmlHashFree(hash_for_icons, NULL);
            xmlHashFree(hash_for_descriptions, NULL);
            return -1;
        }
        root_node = xmlDocGetRootElement(doc);
        if(root_node && root_node->type == XML_ELEMENT_NODE &&
                strstr((char*)root_node->name, "err")){
            xmlFreeDoc(doc);
            xmlCleanupParser();
            xmlHashFree(hash_for_icons, NULL);
            xmlHashFree(hash_for_descriptions, NULL);
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
                    xmlHashFree(hash_for_icons, NULL);
                    xmlHashFree(hash_for_descriptions, NULL);
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
            xmlHashFree(hash_for_icons, NULL);
            xmlHashFree(hash_for_descriptions, NULL);
            return days_number;
        }
    }
    else{
        xmlHashFree(hash_for_icons, NULL);
        xmlHashFree(hash_for_descriptions, NULL);
        return -1;/* file isn't accessability */
    }
}

int
main_gismeteo_ru(int argc, char *argv[]){

//main(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "gismeteoru <input_file> <output_file> <input_detail_data>\n");
        return -1;
    }
    result = convert_station_gismeteo_data(argv[1], argv[2], argv[3]);
    /* fprintf(stderr, "\nresult = %d\n", result); */
    return result;
}
