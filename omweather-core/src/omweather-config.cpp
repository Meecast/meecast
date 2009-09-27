/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
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
#include "omweather-config.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
/*******************************************************************************/
#define GCONF_KEY_PREFIX                            "/apps/maemo/omweather"
#define GCONF_KEY_HTTP_PROXY_PREFIX                 "/system/http_proxy"
#define GCONF_KEY_PROXY_PREFIX                      "/system/proxy"
#define GCONF_KEY_CLOCK_PREFIX                      "/apps/osso/clock"
#define GCONF_KEY_WEATHER_DIR_NAME                  GCONF_KEY_PREFIX"/weather-dir"
#define GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME      GCONF_KEY_PREFIX"/current-country-name"
#define GCONF_KEY_WEATHER_CURRENT_SOURCE_NAME       GCONF_KEY_PREFIX"/current-source-name"
#define GCONF_KEY_WEATHER_STATE_NAME                GCONF_KEY_PREFIX"/state-name"
#define GCONF_KEY_WEATHER_CURRENT_STATION_NAME      GCONF_KEY_PREFIX"/current-station-name"
#define GCONF_KEY_WEATHER_CURRENT_STATION_ID        GCONF_KEY_PREFIX"/current-station-id"
#define GCONF_KEY_WEATHER_ICONS_SIZE                GCONF_KEY_PREFIX"/icons-size"
#define GCONF_KEY_WEATHER_FONT_COLOR                GCONF_KEY_PREFIX"/font-color"
#define GCONF_KEY_WEATHER_FONT                      GCONF_KEY_PREFIX"/font"
#define GCONF_KEY_WEATHER_BACKGROUND_COLOR          GCONF_KEY_PREFIX"/background-color"
#define GCONF_KEY_ENABLE_TRANSPARENCY               GCONF_KEY_PREFIX"/transparency"
#define GCONF_KEY_ALPHA_COMPONENT                   GCONF_KEY_PREFIX"/alpha-component"
#define GCONF_KEY_CORNER_RADIUS                     GCONF_KEY_PREFIX"/corner-radius"
#define GCONF_KEY_SEPARATE_DATA                     GCONF_KEY_PREFIX"/separate-data"
#define GCONF_KEY_SWAP_TEMPERATURE                  GCONF_KEY_PREFIX"/swap-temperature"
#define GCONF_KEY_SHOW_WIND                         GCONF_KEY_PREFIX"/show-wind"
#define GCONF_KEY_SHOW_WIND_GUST                    GCONF_KEY_PREFIX"/show-wind-gust"
#define GCONF_KEY_SHOW_STATION_NAME                 GCONF_KEY_PREFIX"/show-station-name"
#define GCONF_KEY_SHOW_ARROWS                       GCONF_KEY_PREFIX"/show-arrows"
#define GCONF_KEY_WEATHER_UPDATE_INTERVAL           GCONF_KEY_PREFIX"/update-interval"
#define GCONF_KEY_WEATHER_TEMPERATURE_UNIT          GCONF_KEY_PREFIX"/temperature-unit"
#define GCONF_KEY_ICONS_LAYOUT                      GCONF_KEY_PREFIX"/icons-layout"
#define GCONF_KEY_TEXT_POSITION                     GCONF_KEY_PREFIX"/text-position"
#define GCONF_KEY_WEATHER_STATIONS_LIST             GCONF_KEY_PREFIX"/station-list"
#define GCONF_KEY_WEATHER_ALERTS_LIST               GCONF_KEY_PREFIX"/alerts-list"
#define GCONF_KEY_WEATHER_DAYS                      GCONF_KEY_PREFIX"/days"
#define GCONF_KEY_WEATHER_DISTANCE_UNITS            GCONF_KEY_PREFIX"/distance-units"
#define GCONF_KEY_WEATHER_PRESSURE_UNITS            GCONF_KEY_PREFIX"/pressure-units"
#define GCONF_KEY_WEATHER_WIND_UNITS                GCONF_KEY_PREFIX"/wind-units"
#define GCONF_KEY_WEATHER_ICON_SET                  GCONF_KEY_PREFIX"/icon-set"
#define GCONF_KEY_CURRENT_STATION_SOURCE            GCONF_KEY_PREFIX"/current-station-source"
#define GCONF_KEY_WEATHER_VALID_DATA_TIME           GCONF_KEY_PREFIX"/valid-time"
#define GCONF_KEY_WEATHER_PROGRAM_VERSION           GCONF_KEY_PREFIX"/version"
#define GCONF_KEY_WEATHER_SETTING_TAB_NUMBER        GCONF_KEY_PREFIX"/settings-tab-number"
#define GCONF_KEY_USE_SENSOR                        GCONF_KEY_PREFIX"/use-sensor"
#define GCONF_KEY_DISPLAY_SENSOR_AT                 GCONF_KEY_PREFIX"/display-sensor-at"
#define GCONF_KEY_SENSOR_UPDATE_TIME                GCONF_KEY_PREFIX"/sensor-update-time"
#define GCONF_KEY_WEATHER_SWITCH_TIME               GCONF_KEY_PREFIX"/switch-time"
#define GCONF_KEY_PROXY_MODE                        GCONF_KEY_PROXY_PREFIX"/mode"
#define GCONF_KEY_HTTP_PROXY_ON                     GCONF_KEY_HTTP_PROXY_PREFIX"/use_http_proxy"
#define GCONF_KEY_HTTP_PROXY_HOST                   GCONF_KEY_HTTP_PROXY_PREFIX"/host"
#define GCONF_KEY_HTTP_PROXY_PORT                   GCONF_KEY_HTTP_PROXY_PREFIX"/port"
#define GCONF_KEY_CLOCK_REMOTE_LOCATION             GCONF_KEY_CLOCK_PREFIX"/remote-location"
#define GCONF_KEY_CLOCK_HOME_LOCATION               GCONF_KEY_CLOCK_PREFIX"/home-location"
#define GCONF_KEY_DOWNLOADING_AFTER_CONNECTING      GCONF_KEY_PREFIX"/downloading-after-connecting"
#define GCONF_KEY_USE_GPS_STATION                   GCONF_KEY_PREFIX"/gps-station"
#define GCONF_KEY_GPS_STATION_NAME                  GCONF_KEY_PREFIX"/gps-station_name"
#define GCONF_KEY_GPS_STATION_ID                    GCONF_KEY_PREFIX"/gps-station_id"
#define GCONF_KEY_CLICK_FOR_LAUNCH_POPUP            GCONF_KEY_PREFIX"/click-for-launch-popup"
#define GCONF_KEY_SHOW_WEATHER_FOR_TWO_HOURS        GCONF_KEY_PREFIX"/show-weather-for-two-hours"
#define GCONF_KEY_MODE                              GCONF_KEY_PREFIX"/mode"
#define GCONF_KEY_UPDATE_GSM                        GCONF_KEY_PREFIX"/update-gsm"
#define GCONF_KEY_UPDATE_WLAN                       GCONF_KEY_PREFIX"/update-WLAN"
#define GCONF_KEY_VIEW_MODE                         GCONF_KEY_PREFIX"/view_mode"
/*******************************************************************************/
Config::Config(){
    home_dir.clear();
    version.clear();
    cache_directory.clear();
    icons_set_base.clear();
    current_icons_set.clear();
    font.clear();
    last_source.clear();
    last_country.clear();
    current_station_name.clear();
    current_station_code.clear();
    current_station_source.clear();
    iap_http_proxy_host.clear();
    iap_http_proxy_port = 0;
    update_time = 60;
    switch_time = 0;
    current_settings_page = 0;
    days_number = 5;
    previos_days_to_show = days_to_show;
    valid_time = 2 * 3600;
    transparency = true;
    separate = false;
    swap_hi_low_temperature = true;
    show_station_name = true;
    show_arrows = true;
    auto_download = true;
    show_wind = false;
    show_wind_gust = false;
    show_detail_weather = true;
    font_color.pixel = 0;
    font_color.red = 0xFF00U;
    font_color.green = 0xFF00U;
    font_color.blue = 0x0000U;
    background_color.pixel = 0;
    background_color.red = 0x0000U;
    background_color.green = 0x0000U;
    background_color.blue = 0x0000U;
}
/*******************************************************************************/
Config::~Config(){
}
/*******************************************************************************/
bool Config::read(){
    std::string filename;

    home_dir = getenv("HOME");
    if(!home_dir){
        filename = "/tmp/omweather.xml";
    else
        filename = home_dir + "/" + "omweather.xml";

    ifstream file(filename);
    if(file.is_open()){
        file.close();
        xmlDoc document = xmlReadFile(filename, NULL, 0);
        if(document){
            xmlNode root_node = xmlDocGetRootElement(document);
            xmlNode current_node = root_node->children;
            parse_children(current_node);
            xmlFreeDoc(document);
        }
    }
}
/*******************************************************************************/
void Config::parse_children(xmlNode *node){
    xmlChar     *value = NULL;
    int         t;

    while(node){
        if(node->type == XML_ELEMENT_NODE){
            /* version */
            if(!xmlStrcmp(node->name, (const xmlChar*)"version")){
                value = xmlNodeGetContent(node);
                version = (char*)value;
                xmlFree(value);
            }
            /* current-station-source */
            if(!xmlStrcmp(node->name, (const xmlChar*)"current-station-source")){
                value = xmlNodeGetContent(node);
                current_station_source = (char*)value;
                xmlFree(value);
            }
            /* current-station-name */
            if(!xmlStrcmp(node->name, (const xmlChar*)"current-station-name")){
                value = xmlNodeGetContent(node);
                current_station_name = (char*)value;
                xmlFree(value);
            }
            /* current-station-code */
            if(!xmlStrcmp(node->name, (const xmlChar*)"current-station-code")){
                value = xmlNodeGetContent(node);
                current_station_code = (char*)value;
                xmlFree(value);
            }
            /* current-icons-set */
            if(!xmlStrcmp(node->name, (const xmlChar*)"current-icons-set")){
                value = xmlNodeGetContent(node);
                current_icons_set = (char*)value;
                xmlFree(value);
            }
            /* last-source */
            if(!xmlStrcmp(node->name, (const xmlChar*)"last-source")){
                value = xmlNodeGetContent(node);
                last_source = (char*)value;
                xmlFree(value);
            }
            /* last-country */
            if(!xmlStrcmp(node->name, (const xmlChar*)"last-country")){
                value = xmlNodeGetContent(node);
                last_country = (char*)value;
                xmlFree(value);
            }
            /* cache-directory */
            if(!xmlStrcmp(node->name, (const xmlChar*)"cache-directory")){
                value = xmlNodeGetContent(node);
                cache_directory = (char*)value;
                xmlFree(value);
                if(cache_directory.is_empty())
                    cache_directory = home_dir + "/apps/omweather";
            }
            /* icons-preset */
            if(!xmlStrcmp(node->name, (const xmlChar*)"icons-preset")){
                value = xmlNodeGetContent(node);
                icons_preset.current((char*)value);
                xmlFree(value);
            }
            /* icons-size */
            if(!xmlStrcmp(node->name, (const xmlChar*)"icons-size")){
                value = xmlNodeGetContent(node);
                icons_size.current((char*)value);
                xmlFree(value);
            }
            /* view-mode */
            if(!xmlStrcmp(node->name, (const xmlChar*)"view-mode")){
                value = xmlNodeGetContent(node);
                view_mode.current((char*)value);
                xmlFree(value);
            }
            /* settings-mode */
            if(!xmlStrcmp(node->name, (const xmlChar*)"settings-mode")){
                value = xmlNodeGetContent(node);
                settings_mode.current((char*)value);
                xmlFree(value);
            }
            /* show-arrows */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-arrows")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "false"))
                    show_arrows = false;
                else
                    show_arrows = true; /* default value */
                xmlFree(value);
            }
            /* show-station-name */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-station-name")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "false"))
                    show_station_name = false;
                else
                    show_station_name = true; /* default value */
                xmlFree(value);
            }
            /* show-wind */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-wind")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "true"))
                    show_wind = true;
                else
                    show_wind = false; /* default value */
                xmlFree(value);
            }
            /* show-wind-gust */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-wind-gust")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "true"))
                    show_wind_gust = true;
                else
                    show_wind_gust = false; /* default value */
                xmlFree(value);
            }
            /* show-detail-weather */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-detail-weather")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "false"))
                    show_detail_weather = false;
                else
                    show_detail_weather = true; /* default value */
                xmlFree(value);
            }
            /* text-position */
            if(!xmlStrcmp(node->name, (const xmlChar*)"text-position")){
                value = xmlNodeGetContent(node);
                text_position.current((char*)value);
                xmlFree(value);
            }
            /* swap-temperature */
            if(!xmlStrcmp(node->name, (const xmlChar*)"swap-temperature")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "false"))
                    swap_temperature = false;
                else
                    swap_temperature = true; /* default value */
                xmlFree(value);
            }
            /* font-color */
            if(!xmlStrcmp(node->name, (const xmlChar*)"font-color")){
                value = xmlNodeGetContent(node);
                font_color.set(value);
                xmlFree(value);
            }
            /* background-color */
            if(!xmlStrcmp(node->name, (const xmlChar*)"background-color")){
                value = xmlNodeGetContent(node);
                background_color.set(value);
                xmlFree(value);
            }
            /* separate-data */
            if(!xmlStrcmp(node->name, (const xmlChar*)"separate-data")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "true"))
                    separate_data = true;
                else
                    separate_data = false; /* default value */
                xmlFree(value);
            }
            /* transparency */
            if(!xmlStrcmp(node->name, (const xmlChar*)"transparency")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "false"))
                    transparency = false;
                else
                    transparency = true; /* default value */
                xmlFree(value);
            }
            /* wind-units */
            if(!xmlStrcmp(node->name, (const xmlChar*)"wind-units")){
                value = xmlNodeGetContent(node);
                wind_units.current((char*)value);
                xmlFree(value);
            }
            /* pressure-units */
            if(!xmlStrcmp(node->name, (const xmlChar*)"pressure-units")){
                value = xmlNodeGetContent(node);
                pressure_units.current((char*)value);
                xmlFree(value);
            }
            /* distance-units */
            if(!xmlStrcmp(node->name, (const xmlChar*)"distance-units")){
                value = xmlNodeGetContent(node);
                distance_units.current((char*)value);
                xmlFree(value);
            }
            /* valid-time */
            if(!xmlStrcmp(node->name, (const xmlChar*)"valid-time")){
                value = xmlNodeGetContent(node);
                t = atoi((char*)value);
                if(t == 1 || t == 2 || t == 4 || t == 8)
                    valid_time = t * 3600;
                xmlFree(value);
            }
            /* switch-time */
            if(!xmlStrcmp(node->name, (const xmlChar*)"switch-time")){
                value = xmlNodeGetContent(node);
                t = atoi((char*)value);
                if(t == 0 || t == 10 || t == 20 ||
                        t == 30 || t == 40 || t == 50 || t == 60)
                    switch_time = t;
                xmlFree(value);
            }
            /* update-time */
            if(!xmlStrcmp(node->name, (const xmlChar*)"update-time")){
                value = xmlNodeGetContent(node);
                t = atoi((char*)value);
                if(t >= 0 && t <= 24 * 60)       /* No more than 24 hours */
                    update_time = t;
                xmlFree(value);
            }
            /* auto-download */
            if(!xmlStrcmp(node->name, (const xmlChar*)"auto-download")){
                value = xmlNodeGetContent(node);
                if(!strcmp(value, "false"))
                    auto_download = false;
                else
                    auto_download = true; /* default value */
                xmlFree(value);
            }
            /* font */
            if(!xmlStrcmp(node->name, (const xmlChar*)"font")){
                value = xmlNodeGetContent(node);
                font = (char*)value;
                xmlFree(value);
            }
            /* click-type */
            if(!xmlStrcmp(node->name, (const xmlChar*)"click-type")){
                value = xmlNodeGetContent(node);
                click_type.current((char*)value);
                xmlFree(value);
            }
            /* days-number */
            if(!xmlStrcmp(node->name, (const xmlChar*)"days-number")){
                value = xmlNodeGetContent(node);
                t = atoi((char*)value);
                if(t > 0 && t <= Max_count_weather_day)
                    days_number = t;
                xmlFree(value);
            }
        }
        node = node->next;
    }
}
/*******************************************************************************/
bool Config::read1(){
    char        *tmp = NULL,
                *home_dir = NULL;
    int         t, fd;
    GdkColor    tmp_color;
    GConfValue  *value = NULL;
    GError      *gerror = NULL;
    GSList      *stlist = NULL;

    if(!gconf_client){
//        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return false;
    }
    /* Get Weather Cache Directory.  Default is "~/apps/omweather". */
    tmp = gconf_client_get_string(gconf_client,
                                  GCONF_KEY_WEATHER_DIR_NAME, NULL);
    if(!tmp){
        home_dir = getenv("HOME");
        if(!home_dir){
//            fprintf(stderr, _("Can not get path to the HOME directory. Quitting.\n"));
            return false;
        }
        cache_directory = home_dir;
        cache_directory.append("/apps/omweather");
    }
    else{
        cache_directory = tmp;
        g_free(tmp);
    }
    /* create Cache directory */
    if(mkdir(cache_directory.c_str(), S_IRWXU) == -1 && errno != EEXIST){
//        fprintf(stderr, _("Could not create weather cache directory.\n"));
        return false;
    }
    /* Get MODE  Default SIMPLE */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_MODE, NULL);
    if(t >= SIMPLE_MODE && t <= EXTENDED_MODE)
        mode = t;
    /* Get Weather source name. */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_WEATHER_CURRENT_SOURCE_NAME,
                                    NULL);
    if(tmp){
        if(strlen(tmp) > 0)
            current_source = tmp;
        g_free(tmp);
    }
    /* Get Weather country name. */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME,
                                    NULL);
    if(tmp){
        if(strlen(tmp) > 0)
            current_country = tmp;
        g_free(tmp);
    }
    /* Get current station id */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_WEATHER_CURRENT_STATION_ID,
                                    NULL);
    if(tmp){
        if(strlen(tmp) > 0)
            current_station_id = tmp;
        g_free(tmp);
    }
    /* Get Weather current station name. */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
                                    NULL);
    if(tmp){
        if(strlen(tmp) > 0)
            current_station_name = tmp;
        g_free(tmp);
    }
    /* Get weather current station source */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_CURRENT_STATION_SOURCE,
                                    NULL);
    if(tmp){
        if(strlen(tmp) > 0)
            current_station_source = tmp;
        g_free(tmp);
    }
    /* Get user Weather Stations list */
    stlist = gconf_client_get_list(gconf_client,
                                    GCONF_KEY_WEATHER_STATIONS_LIST,
                                    GCONF_VALUE_STRING, NULL);
    if(stlist){
        user_stations_list = StationsList(stlist);
        g_slist_foreach(stlist, (GFunc)g_free, NULL);
        g_slist_free(stlist);
    }
    /* Get icon set name */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_WEATHER_ICON_SET, NULL);
    if(tmp){
        if(strlen(tmp) > 0){
            std::string str(icons_set_base);
            str.append(icon_set);
            if((fd = open(str.c_str(), O_RDONLY)) == -1)
                icon_set = "Glance";
            else{
                close(fd);
                icon_set = tmp;
            }
            icons_set_base.clear();
            icons_set_base = ICONS_PATH;
            icons_set_base.append("/");
            icons_set_base.append(icon_set);
        }
        g_free(tmp);
    }
    /* Get Weather Icon Size  */
    t = gconf_client_get_int(gconf_client,
                                        GCONF_KEY_WEATHER_ICONS_SIZE, NULL);
    if(t >= TINY && t <= GIANT)
        icons_size = t;
    /* Get setting tab number  */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_WEATHER_SETTING_TAB_NUMBER, NULL);
    if(t >= 0 && t <= MAX_SETTINGS_PAGE_NUMBER)
        current_settings_page = t;
    /* Get Weather periodic update time. */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_WEATHER_UPDATE_INTERVAL, NULL);
    if(t >= 0 && t <= 24 * 60)       /* No more than 24 hours */
        update_interval = t;
//    remove_periodic_event();    /* delete event from list */
//    add_periodic_event(time(NULL));     /* add new event */
    /* Get Weather font color. */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_WEATHER_FONT_COLOR, NULL);
    if(tmp){
        if(gdk_color_parse(tmp, &tmp_color))
            font_color = tmp_color;
        g_free(tmp);
    }
    /* Get Weather background color. */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_WEATHER_BACKGROUND_COLOR, NULL);
    if(tmp){
        if(gdk_color_parse(tmp, &tmp_color))
            background_color = tmp_color;
        g_free(tmp);
    }
    /* Get font description. */
    tmp = gconf_client_get_string(gconf_client,
                                    GCONF_KEY_WEATHER_FONT, NULL);
    if(tmp){
        if(strlen(tmp) > 0)
            font = tmp;
        g_free(tmp);
    }
    /* Get Enable Transparency flag. Default is TRUE. */
    value = gconf_client_get(gconf_client,
                                GCONF_KEY_ENABLE_TRANSPARENCY, NULL);
    if(value){
        transparency = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get Enable Weather For Two Hours. Default is TRUE. */
    value = gconf_client_get(gconf_client,
                                GCONF_KEY_SHOW_WEATHER_FOR_TWO_HOURS, NULL);
    if(value){
        show_weather_for_two_hours = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get Split Button State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SEPARATE_DATA, NULL);
    if(value){
        separate = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get auto_downloading_after_connecting. Default is FALSE */
    value = gconf_client_get(gconf_client,
                                GCONF_KEY_DOWNLOADING_AFTER_CONNECTING, NULL);
    if(value){
        downloading_after_connecting = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get Swap Temperature Button State. Default is TRUE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SWAP_TEMPERATURE, NULL);
    if(value){
        swap_hi_low_temperature = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get Show wind Button State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SHOW_WIND, NULL);
    if(value){
        show_wind = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get Show Station Name. Default is TRUE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SHOW_STATION_NAME, NULL);
    if(value){
        show_station_name = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get Show Arrows State. Default is TRUE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SHOW_ARROWS, NULL);
    if(value){
        show_arrows = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get Temperature Unit  Default Celsius */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_WEATHER_TEMPERATURE_UNIT, &gerror);
    if(!gerror){
        if(t >= CELSIUS && t <= FAHRENHEIT)
            temperature_units = t;
        if(gerror)
            g_error_free(gerror);
    }
    /* Get Layout Default Preset ONE */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_ICONS_LAYOUT, NULL);
    if(t >= ONE_ROW && t <= PRESET_NOW_PLUS_SEVEN)
        icons_layout = t;
    /* Get Text Position  Default Right */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_TEXT_POSITION, NULL);
    if(t >= RIGHT && t <= BOTTOM)
        text_position = t;
    /* Get number days to show default 5 */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_WEATHER_DAYS, &gerror);
    if(!gerror){
        if(t > 0 && t <= Max_count_weather_day)
            days_to_show = t;
        if(gerror)
            g_error_free(gerror);

    }
    previos_days_to_show = days_to_show;
    /* Get distance units */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_WEATHER_DISTANCE_UNITS, &gerror);
    if(!gerror){
        if(t >= METERS && t <= SEA_MILES)
            distance_units = t;
        if(gerror)
            g_error_free(gerror);
    }
    /* Get pressure units */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_WEATHER_PRESSURE_UNITS, &gerror);
    if(!gerror){
        if(t >= MB && t <= MM)
            pressure_units = t;
        if(gerror)
            g_error_free(gerror);
    }
    /* Get wind units */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_WEATHER_WIND_UNITS, &gerror);
    if(!gerror){
        if(t >= METERS_S && t <= MILES_H)
            wind_units = t;
        if(gerror)
            g_error_free(gerror);
    }
    /* Get the  type of clicking default SHORT click */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_CLICK_FOR_LAUNCH_POPUP, &gerror);
    if(!gerror){
        if(t >= SHORT_CLICK && t <= LONG_CLICK)
            clicking_type = t;
        if(gerror)
            g_error_free(gerror);
    }
    /* Get switch time */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_WEATHER_SWITCH_TIME, &gerror);
    if(!gerror){
        if(t == 0 || t == 10 || t == 20 ||
                t == 30 || t == 40 || t == 50 || t == 60)
            switch_time = t;
        if(gerror)
            g_error_free(gerror);
    }
/*
    if(switch_time > 0){
        g_source_remove(app->switch_timer);
        app->switch_timer = g_timeout_add(switch_time * 1000,
                                            (GtkFunction)switch_timer_handler,
                                              app->main_window);
    }
*/
    /* Get valid data time for current weather */
    t = gconf_client_get_int(gconf_client,
                                GCONF_KEY_WEATHER_VALID_DATA_TIME, &gerror);
    if(!gerror){
        if(t == 1 || t == 2 || t == 4 || t == 8)
            data_valid_interval = t * 3600;
        if(gerror)
            g_error_free(gerror);
    }
    /* Get VIEW_MODE Default COLLAPSED */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_VIEW_MODE, NULL);
    if(t >= COLLAPSED_VIEW_MODE && t <= EXPANDED_VIEW_MODE)
        view_mode = t;
//    check_current_station_id();
    return true;
}
/*******************************************************************************/
void Config::save(){
    char        buffer[255];
    if(!gconf_client){
//        fprintf(stderr, _("Failed to initialize GConf. Settings were not saved.\n"));
        return;
    }
    /* Save Weather Cache Directory. */
    gconf_client_set_string(gconf_client,
                            GCONF_KEY_WEATHER_DIR_NAME,
                            cache_directory.c_str(), NULL);
    /* Save Weather source name. */
    if(!current_source.empty())
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_WEATHER_CURRENT_SOURCE_NAME,
                                current_source.c_str(), NULL);
    /* Save Weather country name. */
    if(!current_country.empty())
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME,
                                current_country.c_str(), NULL);
    /* Save Weather station name. */
    if(!current_station_name.empty())
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
                                current_station_name.c_str(), NULL);
    else
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
                                "", NULL);
    /* Save Weather station id. */
    if(!current_station_id.empty())
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_WEATHER_CURRENT_STATION_ID,
                                current_station_id.c_str(), NULL);
    else
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_WEATHER_CURRENT_STATION_ID, "",
                                NULL);
    /* Save Weather Data Source  */
    if(!current_station_source.empty())
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_CURRENT_STATION_SOURCE,
                                current_station_source.c_str(), NULL);
    else
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_CURRENT_STATION_SOURCE, "", NULL);
    /* Save icon set name */
    if(!icon_set.empty())
        gconf_client_set_string(gconf_client, GCONF_KEY_WEATHER_ICON_SET,
                                icon_set.c_str(), NULL);
    /* Save Weather Icon Size  */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_ICONS_SIZE,
                            icons_size, NULL);
    /* Save current setting tab number  */
    gconf_client_set_int(gconf_client,
                            GCONF_KEY_WEATHER_SETTING_TAB_NUMBER,
                            current_settings_page, NULL);
    /* Save Weather Font Color */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "#%02x%02x%02x",
                font_color.red >> 8, font_color.green >> 8,
                font_color.blue >> 8);
    gconf_client_set_string(gconf_client, GCONF_KEY_WEATHER_FONT_COLOR, buffer,
                            NULL);
    /* Save Background Color */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "#%02x%02x%02x",
                background_color.red >> 8, background_color.green >> 8,
                background_color.blue >> 8);
    gconf_client_set_string(gconf_client, GCONF_KEY_WEATHER_BACKGROUND_COLOR,
                            buffer, NULL);
    /* Save Font Description */
    if(!font.empty())
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_WEATHER_FONT, font.c_str(), NULL);
    else
        gconf_client_set_string(gconf_client,
                                GCONF_KEY_WEATHER_FONT, "", NULL);
    /* Save Weather Update setting  */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_UPDATE_INTERVAL,
                            update_interval, NULL);
    /* Save Enable Enable Transparency flag. */
    gconf_client_set_bool(gconf_client, GCONF_KEY_ENABLE_TRANSPARENCY,
                            transparency, NULL);
    /* Save Enable Weather For Two Hours */
    gconf_client_set_bool(gconf_client, GCONF_KEY_SHOW_WEATHER_FOR_TWO_HOURS,
                            show_weather_for_two_hours, NULL);
    /* Save Split Button State */
    gconf_client_set_bool(gconf_client, GCONF_KEY_SEPARATE_DATA, separate, NULL);
    /* Save Downloading after connecting State */
    gconf_client_set_bool(gconf_client, GCONF_KEY_DOWNLOADING_AFTER_CONNECTING,
                            downloading_after_connecting, NULL);
    /* Save Swap Temperature Button State */
    gconf_client_set_bool(gconf_client, GCONF_KEY_SWAP_TEMPERATURE,
                            swap_hi_low_temperature, NULL);
    /* Save Show Wind Button State */
    gconf_client_set_bool(gconf_client, GCONF_KEY_SHOW_WIND, show_wind, NULL);
    /* Save Type of clicking for showing window popup */
    gconf_client_set_int(gconf_client, GCONF_KEY_CLICK_FOR_LAUNCH_POPUP,
                            clicking_type, NULL);
    /* Save Show Station Name */
    gconf_client_set_bool(gconf_client, GCONF_KEY_SHOW_STATION_NAME,
                            show_station_name, NULL);
    /* Save Show Arrows */
    gconf_client_set_bool(gconf_client, GCONF_KEY_SHOW_ARROWS,
                            show_arrows, NULL);
    /* Save Weather Temperature Unit  */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_TEMPERATURE_UNIT,
                            temperature_units, NULL);
    /* Save Days to show. */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_DAYS,
                            days_to_show, NULL);
    /* Save distance units. */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_DISTANCE_UNITS,
                            distance_units, NULL);
    /* Save pressure units. */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_PRESSURE_UNITS,
                            pressure_units, NULL);
    /* Save wind units. */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_WIND_UNITS,
                            wind_units, NULL);
    /* Save switch time */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_SWITCH_TIME,
                            switch_time, NULL);
    /* Save valid data time */
    gconf_client_set_int(gconf_client, GCONF_KEY_WEATHER_VALID_DATA_TIME,
                            data_valid_interval / 3600, NULL);
    /* Save Layout type. */
    gconf_client_set_int(gconf_client, GCONF_KEY_ICONS_LAYOUT,
                            icons_layout, NULL);
    /* Save Mode. */
    gconf_client_set_int(gconf_client, GCONF_KEY_MODE, mode, NULL);
    /* Save position of text. */
    gconf_client_set_int(gconf_client, GCONF_KEY_TEXT_POSITION,
                            text_position, NULL);
    /* Save station list */
    GSList *stlist = user_stations_list.toGSList();
    gconf_client_set_list(gconf_client, GCONF_KEY_WEATHER_STATIONS_LIST,
                            GCONF_VALUE_STRING, stlist, NULL);
    g_slist_foreach(stlist, (GFunc)g_free, NULL);
    g_slist_free(stlist);
    /* Save View mode */
    gconf_client_set_int(gconf_client, GCONF_KEY_VIEW_MODE, view_mode, NULL);
}
/*******************************************************************************/
bool Config::read_proxy_settings(){
    char        *proxy_mode = NULL;
    if(!gconf_client){
//        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return false;
    }
    proxy_mode = gconf_client_get_string(gconf_client,
                                            GCONF_KEY_PROXY_MODE, NULL);
    if(proxy_mode){
        if(strcmp(proxy_mode, "none")){
            /* Get proxy data */
            if(gconf_client_get_bool(gconf_client,
                                        GCONF_KEY_HTTP_PROXY_ON, NULL)){
                /* HTTP Proxy is on. */
                iap_http_proxy_host
                    = gconf_client_get_string(gconf_client,
                                                GCONF_KEY_HTTP_PROXY_HOST,
                                                NULL);
                iap_http_proxy_port
                    = gconf_client_get_int(gconf_client,
                                            GCONF_KEY_HTTP_PROXY_PORT,
                                            NULL);
            }
        }
    }
    g_free(proxy_mode);
    return true;
}
/*******************************************************************************/
Param Config::param(const std::string param_name) const{
    Param       p;
    /* cache directory */
    if(param_name == "cache_directory"){
        p.string_param = const_cast<char*>(cache_directory.c_str());
        goto exit;
    }
    /* icon set */
    if(param_name == "icon_set"){
        p.string_param = const_cast<char*>(icon_set.c_str());
        goto exit;
    }
    /* icon sets base*/
    if(param_name == "icons_set_base"){
        p.string_param = const_cast<char*>(icons_set_base.c_str());
        goto exit;
    }
    /* font */
    if(param_name == "font"){
        p.string_param = const_cast<char*>(font.c_str());
        goto exit;
    }
    /* current source */
    if(param_name == "current_source"){
        p.string_param = const_cast<char*>(current_source.c_str());
        goto exit;
    }
    /* current_country */
    if(param_name == "current_country"){
        p.string_param = const_cast<char*>(current_country.c_str());
        goto exit;
    }
    /* current_station_name */
    if(param_name == "current_station_name"){
        p.string_param = const_cast<char*>(current_station_name.c_str());
        goto exit;
    }
    /* current_station_id */
    if(param_name == "current_station_id"){
        p.string_param = const_cast<char*>(current_station_id.c_str());
        goto exit;
    }
    /* current_station_source */
    if(param_name == "current_station_source"){
        p.string_param = const_cast<char*>(current_station_source.c_str());
        goto exit;
    }
    /* iap_http_proxy_host */
    if(param_name == "proxy_host"){
        p.string_param = const_cast<char*>(iap_http_proxy_host.c_str());
        goto exit;
    }
    /* iap_http_proxy_port */
    if(param_name == "proxy_port"){
        p.int_param = iap_http_proxy_port;
        goto exit;
    }
    /* update_interval */
    if(param_name == "update_interval"){
        p.int_param = update_interval;
        goto exit;
    }
    /* switch_time */
    if(param_name == "switch_time"){
        p.int_param = switch_time;
        goto exit;
    }
    /* icons_size */
    if(param_name == "icons_size"){
        p.int_param = icons_size;
        goto exit;
    }
    /* icons_layout */
    if(param_name == "icons_layout"){
        p.int_param = icons_layout;
        goto exit;
    }
    /* text_position */
    if(param_name == "text_position"){
        p.int_param = text_position;
        goto exit;
    }
    /* current_settings_page */
    if(param_name == "current_settings_page"){
        p.int_param = current_settings_page;
        goto exit;
    }
    /* days_to_show */
    if(param_name == "days_to_show"){
        p.int_param = days_to_show;
        goto exit;
    }
    /* distance_units */
    if(param_name == "distance_units"){
        p.int_param = distance_units;
        goto exit;
    }
    /* wind_units */
    if(param_name == "wind_units"){
        p.int_param = wind_units;
        goto exit;
    }
    /* clicking_type */
    if(param_name == "clicking_type"){
        p.int_param = clicking_type;
        goto exit;
    }
    /* temperature_units */
    if(param_name == "temperature_units"){
        p.int_param = temperature_units;
        goto exit;
    }
    /* pressure_units */
    if(param_name == "pressure_units"){
        p.int_param = pressure_units;
        goto exit;
    }
    /* mode */
    if(param_name == "mode"){
        p.int_param = mode;
        goto exit;
    }
    /* view_mode */
    if(param_name == "view_mode"){
        p.int_param = view_mode;
        goto exit;
    }
    /* data_valid_interval */
    if(param_name == "data_valid_interval"){
        p.int_param = data_valid_interval;
        goto exit;
    }
    /* transparency */
    if(param_name == "transparency"){
        p.bool_param = transparency;
        goto exit;
    }
    /* separate */
    if(param_name == "separate"){
        p.bool_param = separate;
        goto exit;
    }
    /* swap_hi_low_temperature */
    if(param_name == "swap_hi_low_temperature"){
        p.bool_param = swap_hi_low_temperature;
        goto exit;
    }
    /* show_station_name */
    if(param_name == "show_station_name"){
        p.bool_param = show_station_name;
        goto exit;
    }
    /* show_arrows */
    if(param_name == "show_arrows"){
        p.bool_param = show_arrows;
        goto exit;
    }
    /* downloading_after_connecting */
    if(param_name == "downloading_after_connecting"){
        p.bool_param = downloading_after_connecting;
        goto exit;
    }
    /* show_wind */
    if(param_name == "show_wind"){
        p.bool_param = show_wind;
        goto exit;
    }
    /* show_weather_for_two_hours */
    if(param_name == "show_weather_for_two_hours"){
        p.bool_param = show_weather_for_two_hours;
        goto exit;
    }
    /* font_color */
    if(param_name == "font_color"){
        memcpy(&p.color_param, &font_color, sizeof(p.color_param));
        goto exit;
    }
    /* background_color */
    if(param_name == "background_color"){
        memcpy(&p.color_param, &background_color, sizeof(p.color_param));
        goto exit;
    }
    exit:
        return p;
}
/*******************************************************************************/
OS2008Config::OS2008Config() : Config(){
    gps_station = false;
    display_at = STATION_NAME;
    sensor_update_time = 0;
    alpha_comp = 0;
    corner_radius = 10;
}
/*******************************************************************************/
bool OS2008Config::read(){
    GConfValue  *value = NULL;
    GError      *gerror = NULL;
    int         t;

    Config::read();
    show_arrows = false;
    if(!gconf_client){
//        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return false;
    }
    /* Get gps_station. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_USE_GPS_STATION, NULL);
    if(value){
        gps_station = gconf_value_get_bool(value);
        gconf_value_free(value);
//        add_gps_event(1);
    }
    /* Get Use sensor Button State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_USE_SENSOR, NULL);
    if(value){
        use_sensor = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get display sensor at  */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_DISPLAY_SENSOR_AT, NULL);
    if(t >= ICON && t <= STATION_NAME)
        display_at = t;
    /* Get sensor update time  */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_SENSOR_UPDATE_TIME, NULL);
    if(t >= 0 && t <= 30)
        sensor_update_time = t;
    /* start timer for read data from device temperature sensor */
/*
    if(use_sensor){
        read_sensor(FALSE);
        if(sensor_update_time)
            app->sensor_timer =
                g_timeout_add(config->sensor_update_time * 1000 * 60,
                              (GtkFunction) read_sensor,
                              GINT_TO_POINTER(1));
        else
            app->sensor_timer = 0;
    }
    else
        app->sensor_timer = 0;
*/
    /* Get value of transparence default 0 */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_ALPHA_COMPONENT, &gerror);
    if(!gerror && t >= 0) {
        alpha_comp = t;
        if(gerror)
            g_error_free(gerror);
    }
    /* Get value of corner radius. Default is 10 */
    t = gconf_client_get_int(gconf_client, GCONF_KEY_CORNER_RADIUS, &gerror);
    if(!gerror && t >= 0) {
        corner_radius = t;
        if(gerror)
            g_error_free(gerror);
    }
}
/*******************************************************************************/
void OS2008Config::save(){
    Config::save();
    if(!gconf_client){
//        fprintf(stderr, _("Failed to initialize GConf. Settings were not saved.\n"));
        return;
    }
    /* Save Use sensor Button State */
    gconf_client_set_bool(gconf_client, GCONF_KEY_USE_SENSOR, use_sensor, NULL);
    /* Save display sensor at  */
    gconf_client_set_int(gconf_client, GCONF_KEY_DISPLAY_SENSOR_AT,
                            display_at, NULL);
    /* Save sensor update time */
    gconf_client_set_int(gconf_client, GCONF_KEY_SENSOR_UPDATE_TIME,
                            sensor_update_time, NULL);
    /* Save Use GPS station */
    gconf_client_set_bool(gconf_client, GCONF_KEY_USE_GPS_STATION,
                            gps_station, NULL);
    /* Save Alpha component */
    gconf_client_set_int(gconf_client, GCONF_KEY_ALPHA_COMPONENT,
                            alpha_comp, NULL);
    /* Save Corner Radius */
    gconf_client_set_int(gconf_client, GCONF_KEY_CORNER_RADIUS,
                            corner_radius, NULL);
}
/*******************************************************************************/
Param OS2008Config::param(const std::string param_name) const{
    Param       p;
    /* gps_station */
    if(param_name == "gps_station"){
        p.bool_param = gps_station;
        goto exit;
    }
    /* use_sensor */
    if(param_name == "use_sensor"){
        p.bool_param = use_sensor;
        goto exit;
    }
    /* display_at */
    if(param_name == "display_at"){
        p.int_param = display_at;
        goto exit;
    }
    /* sensor_update_time */
    if(param_name == "sensor_update_time"){
        p.int_param = sensor_update_time;
        goto exit;
    }
    /* alpha_comp */
    if(param_name == "alpha_comp"){
        p.int_param = alpha_comp;
        goto exit;
    }
    /* corner_radius */
    if(param_name == "corner_radius"){
        p.int_param = corner_radius;
        goto exit;
    }
    return Config::param(param_name);
    exit:
        return p;
}
/*******************************************************************************/
OS2009Config::OS2009Config() : OS2008Config(){
    update_gsm = false;
    update_wlan = true;
}
/*******************************************************************************/
bool OS2009Config::read(){
    GConfValue  *value = NULL;
    GError      *gerror = NULL;
    int         t;

    OS2008Config::read();
    if(!gconf_client){
//        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return false;
    }
    /* Get update GSM */ 
    value = gconf_client_get(gconf_client, GCONF_KEY_UPDATE_GSM, NULL);
    if(value){
        update_gsm = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    /* Get update WLAN */
    value = gconf_client_get(gconf_client, GCONF_KEY_UPDATE_WLAN, NULL);
    if(value){
        update_wlan = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
}
/*******************************************************************************/
void OS2009Config::save(){
    OS2008Config::save();
    if(!gconf_client){
//        fprintf(stderr, _("Failed to initialize GConf. Settings were not saved.\n"));
        return;
    }
    /* Save GSM update setting */
    gconf_client_set_bool(gconf_client, GCONF_KEY_UPDATE_GSM, update_gsm, NULL);
    /* Save WLAN update setting */
    gconf_client_set_bool(gconf_client, GCONF_KEY_UPDATE_WLAN, update_wlan, NULL);
}
/*******************************************************************************/
Param OS2009Config::param(const std::string param_name) const{
    Param       p;
    /* update_gsm */
    if(param_name == "update_gsm"){
        p.bool_param = update_gsm;
        goto exit;
    }
    /* update_wlan */
    if(param_name == "update_wlan"){
        p.bool_param = update_wlan;
        goto exit;
    }
    return OS2008Config::param(param_name);
    exit:
        return p;
}
/*******************************************************************************/
