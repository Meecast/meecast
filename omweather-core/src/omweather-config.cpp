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
#include <fstream>
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
    previos_days_number = days_number;
    valid_time = 2 * 3600;
    transparency = true;
    separate_data = false;
    swap_temperature = true;
    show_station_name = true;
    show_arrows = true;
    auto_download = true;
    show_wind = false;
    show_wind_gust = false;
    show_detail_weather = true;
    font_color.r = 0xFF00U;
    font_color.g = 0xFF00U;
    font_color.b = 0x0000U;
    background_color.r = 0x0000U;
    background_color.g = 0x0000U;
    background_color.b = 0x0000U;
}
/*******************************************************************************/
Config::~Config(){
}
/*******************************************************************************/
bool Config::read(){
    std::string filename;

    home_dir = getenv("HOME");
    if(home_dir.empty())
        filename = "/tmp/omweather.xml";
    else
        filename = home_dir + "/" + "omweather.xml";

    std::ifstream file(filename.c_str());
    if(file.is_open()){
        file.close();
        xmlDoc *document = xmlReadFile(filename.c_str(), NULL, 0);
        if(document){
            xmlNode *root_node = xmlDocGetRootElement(document);
            xmlNode *current_node = root_node->children;
            parse_children(current_node);
            xmlFreeDoc(document);
        }
    }
}
/*******************************************************************************/
void Config::parse_children(xmlNode *node){
    xmlNode     *child_node = NULL;
    xmlChar     *val = NULL;
    int         t;

    while(node){
        if(node->type == XML_ELEMENT_NODE){
            /* version */
            if(!xmlStrcmp(node->name, (const xmlChar*)"version")){
                val = xmlNodeGetContent(node);
                version = (char*)val;
                xmlFree(val);
            }
            /* current-station-source */
            if(!xmlStrcmp(node->name, (const xmlChar*)"current-station-source")){
                val = xmlNodeGetContent(node);
                current_station_source = (char*)val;
                xmlFree(val);
            }
            /* current-station-name */
            if(!xmlStrcmp(node->name, (const xmlChar*)"current-station-name")){
                val = xmlNodeGetContent(node);
                current_station_name = (char*)val;
                xmlFree(val);
            }
            /* current-station-code */
            if(!xmlStrcmp(node->name, (const xmlChar*)"current-station-code")){
                val = xmlNodeGetContent(node);
                current_station_code = (char*)val;
                xmlFree(val);
            }
            /* current-icons-set */
            if(!xmlStrcmp(node->name, (const xmlChar*)"current-icons-set")){
                val = xmlNodeGetContent(node);
                current_icons_set = (char*)val;
                xmlFree(val);
            }
            /* last-source */
            if(!xmlStrcmp(node->name, (const xmlChar*)"last-source")){
                val = xmlNodeGetContent(node);
                last_source = (char*)val;
                xmlFree(val);
            }
            /* last-country */
            if(!xmlStrcmp(node->name, (const xmlChar*)"last-country")){
                val = xmlNodeGetContent(node);
                last_country = (char*)val;
                xmlFree(val);
            }
            /* cache-directory */
            if(!xmlStrcmp(node->name, (const xmlChar*)"cache-directory")){
                val = xmlNodeGetContent(node);
                cache_directory = (char*)val;
                xmlFree(val);
                if(cache_directory.empty())
                    cache_directory = home_dir + "/apps/omweather";
            }
            /* icons-preset */
            if(!xmlStrcmp(node->name, (const xmlChar*)"icons-preset")){
                val = xmlNodeGetContent(node);
                icons_preset.current((char*)val);
                xmlFree(val);
            }
            /* icons-size */
            if(!xmlStrcmp(node->name, (const xmlChar*)"icons-size")){
                val = xmlNodeGetContent(node);
                icons_size.current((char*)val);
                xmlFree(val);
            }
            /* view-mode */
            if(!xmlStrcmp(node->name, (const xmlChar*)"view-mode")){
                val = xmlNodeGetContent(node);
                view_mode.current((char*)val);
                xmlFree(val);
            }
            /* settings-mode */
            if(!xmlStrcmp(node->name, (const xmlChar*)"settings-mode")){
                val = xmlNodeGetContent(node);
                settings_mode.current((char*)val);
                xmlFree(val);
            }
            /* show-arrows */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-arrows")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "false"))
                    show_arrows = false;
                else
                    show_arrows = true; /* default value */
                xmlFree(val);
            }
            /* show-station-name */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-station-name")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "false"))
                    show_station_name = false;
                else
                    show_station_name = true; /* default value */
                xmlFree(val);
            }
            /* show-wind */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-wind")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "true"))
                    show_wind = true;
                else
                    show_wind = false; /* default value */
                xmlFree(val);
            }
            /* show-wind-gust */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-wind-gust")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "true"))
                    show_wind_gust = true;
                else
                    show_wind_gust = false; /* default value */
                xmlFree(val);
            }
            /* show-detail-weather */
            if(!xmlStrcmp(node->name, (const xmlChar*)"show-detail-weather")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "false"))
                    show_detail_weather = false;
                else
                    show_detail_weather = true; /* default value */
                xmlFree(val);
            }
            /* text-position */
            if(!xmlStrcmp(node->name, (const xmlChar*)"text-position")){
                val = xmlNodeGetContent(node);
                text_position.current((char*)val);
                xmlFree(val);
            }
            /* swap-temperature */
            if(!xmlStrcmp(node->name, (const xmlChar*)"swap-temperature")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "false"))
                    swap_temperature = false;
                else
                    swap_temperature = true; /* default value */
                xmlFree(val);
            }
            /* font-color */
            if(!xmlStrcmp(node->name, (const xmlChar*)"font-color")){
                val = xmlNodeGetContent(node);
                font_color.set((const char*)val);
                xmlFree(val);
            }
            /* background-color */
            if(!xmlStrcmp(node->name, (const xmlChar*)"background-color")){
                val = xmlNodeGetContent(node);
                background_color.set((const char*)val);
                xmlFree(val);
            }
            /* separate-data */
            if(!xmlStrcmp(node->name, (const xmlChar*)"separate-data")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "true"))
                    separate_data = true;
                else
                    separate_data = false; /* default value */
                xmlFree(val);
            }
            /* transparency */
            if(!xmlStrcmp(node->name, (const xmlChar*)"transparency")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "false"))
                    transparency = false;
                else
                    transparency = true; /* default value */
                xmlFree(val);
            }
            /* wind-units */
            if(!xmlStrcmp(node->name, (const xmlChar*)"wind-units")){
                val = xmlNodeGetContent(node);
                wind_units.current((char*)val);
                xmlFree(val);
            }
            /* pressure-units */
            if(!xmlStrcmp(node->name, (const xmlChar*)"pressure-units")){
                val = xmlNodeGetContent(node);
                pressure_units.current((char*)val);
                xmlFree(val);
            }
            /* distance-units */
            if(!xmlStrcmp(node->name, (const xmlChar*)"distance-units")){
                val = xmlNodeGetContent(node);
                distance_units.current((char*)val);
                xmlFree(val);
            }
            /* valid-time */
            if(!xmlStrcmp(node->name, (const xmlChar*)"valid-time")){
                val = xmlNodeGetContent(node);
                t = atoi((char*)val);
                if(t == 1 || t == 2 || t == 4 || t == 8)
                    valid_time = t * 3600;
                xmlFree(val);
            }
            /* switch-time */
            if(!xmlStrcmp(node->name, (const xmlChar*)"switch-time")){
                val = xmlNodeGetContent(node);
                t = atoi((char*)val);
                if(t == 0 || t == 10 || t == 20 ||
                        t == 30 || t == 40 || t == 50 || t == 60)
                    switch_time = t;
                xmlFree(val);
            }
            /* update-time */
            if(!xmlStrcmp(node->name, (const xmlChar*)"update-time")){
                val = xmlNodeGetContent(node);
                t = atoi((char*)val);
                if(t >= 0 && t <= 24 * 60)       /* No more than 24 hours */
                    update_time = t;
                xmlFree(val);
            }
            /* auto-download */
            if(!xmlStrcmp(node->name, (const xmlChar*)"auto-download")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "false"))
                    auto_download = false;
                else
                    auto_download = true; /* default value */
                xmlFree(val);
            }
            /* font */
            if(!xmlStrcmp(node->name, (const xmlChar*)"font")){
                val = xmlNodeGetContent(node);
                font = (char*)val;
                xmlFree(val);
            }
            /* click-type */
            if(!xmlStrcmp(node->name, (const xmlChar*)"click-type")){
                val = xmlNodeGetContent(node);
                click_type.current((char*)val);
                xmlFree(val);
            }
            /* days-number */
            if(!xmlStrcmp(node->name, (const xmlChar*)"days-number")){
                val = xmlNodeGetContent(node);
                t = atoi((char*)val);
                if(t > 0 && t <= Max_count_weather_day)
                    days_number = t;
                xmlFree(val);
                previos_days_number = days_number;
            }
            /* stations-list */
            if(!xmlStrcmp(node->name, (const xmlChar*)"stations-list")){
                for(child_node = node->children; child_node; child_node = child_node->next)
                    if(child_node->type == XML_ELEMENT_NODE){
                        /* station */
                        if(!xmlStrcmp(child_node->name, (const xmlChar*)"station")){
                            std::string name, code, source;
                            bool is_gps;
                            /* name */
                            val = xmlGetProp(node, (const xmlChar*)"name");
                            name = (char*)val;
                            xmlFree(val);
                            /* code */
                            val = xmlGetProp(node, (const xmlChar*)"code");
                            code = (char*)val;
                            xmlFree(val);
                            /* source */
                            val = xmlGetProp(node, (const xmlChar*)"source");
                            source = (char*)val;
                            xmlFree(val);
                            /* type */
                            val = xmlGetProp(node, (const xmlChar*)"type");
                            if(!strcmp((const char*)val, "gps"))
                                is_gps = true;
                            else
                                is_gps = false;
                            xmlFree(val);
                            user_stations_list.add(Station(name, code, source, is_gps));
                        }
                    }
            }
        }
        node = node->next;
    }
}
/*******************************************************************************/
void Config::save(){
}
/*******************************************************************************/
bool Config::read_proxy_settings(){
/*
    char        *proxy_mode = NULL;
    if(!gconf_client){
//        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return false;
    }
    proxy_mode = gconf_client_get_string(gconf_client,
                                            GCONF_KEY_PROXY_MODE, NULL);
    if(proxy_mode){
        if(strcmp(proxy_mode, "none")){
            if(gconf_client_get_bool(gconf_client,
                                        GCONF_KEY_HTTP_PROXY_ON, NULL)){
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
*/
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
    if(param_name == "last_source"){
        p.string_param = const_cast<char*>(last_source.c_str());
        goto exit;
    }
    /* current_country */
    if(param_name == "last_country"){
        p.string_param = const_cast<char*>(last_country.c_str());
        goto exit;
    }
    /* current_station_name */
    if(param_name == "current_station_name"){
        p.string_param = const_cast<char*>(current_station_name.c_str());
        goto exit;
    }
    /* current_station_id */
    if(param_name == "current_station_code"){
        p.string_param = const_cast<char*>(current_station_code.c_str());
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
    if(param_name == "valid_time"){
        p.int_param = valid_time;
        goto exit;
    }
    /* switch_time */
    if(param_name == "switch_time"){
        p.int_param = switch_time;
        goto exit;
    }
    /* icons_size */
    if(param_name == "icons_size"){
        p.int_param = icons_size.toInt();
        goto exit;
    }
    /* icons_preset */
    if(param_name == "icons_preset"){
        p.int_param = icons_preset.toInt();
        goto exit;
    }
    /* text_position */
    if(param_name == "text_position"){
        p.int_param = text_position.toInt();
        goto exit;
    }
    /* current_settings_page */
    if(param_name == "current_settings_page"){
        p.int_param = current_settings_page;
        goto exit;
    }
    /* days_to_show */
    if(param_name == "days_number"){
        p.int_param = days_number;
        goto exit;
    }
    /* distance_units */
    if(param_name == "distance_units"){
        p.int_param = distance_units.toInt();
        goto exit;
    }
    /* wind_units */
    if(param_name == "wind_units"){
        p.int_param = wind_units.toInt();
        goto exit;
    }
    /* clicking_type */
    if(param_name == "click_type"){
        p.int_param = click_type.toInt();
        goto exit;
    }
    /* temperature_units */
    if(param_name == "temperature_units"){
        p.int_param = temperature_units.toInt();
        goto exit;
    }
    /* pressure_units */
    if(param_name == "pressure_units"){
        p.int_param = pressure_units.toInt();
        goto exit;
    }
    /* mode */
    if(param_name == "settings_mode"){
        p.int_param = settings_mode.toInt();
        goto exit;
    }
    /* view_mode */
    if(param_name == "view_mode"){
        p.int_param = view_mode.toInt();
        goto exit;
    }
    /* data_valid_interval */
    if(param_name == "valid_time"){
        p.int_param = valid_time;
        goto exit;
    }
    /* transparency */
    if(param_name == "transparency"){
        p.bool_param = transparency;
        goto exit;
    }
    /* separate */
    if(param_name == "separate_data"){
        p.bool_param = separate_data;
        goto exit;
    }
    /* swap_temperature */
    if(param_name == "swap_temperature"){
        p.bool_param = swap_temperature;
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
    if(param_name == "auto_download"){
        p.bool_param = auto_download;
        goto exit;
    }
    /* show_wind */
    if(param_name == "show_wind"){
        p.bool_param = show_wind;
        goto exit;
    }
    /* show_weather_for_two_hours */
    if(param_name == "show_detail_weather"){
        p.bool_param = show_detail_weather;
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
    return true;
}
/*******************************************************************************/
void OS2008Config::save(){
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
}
/*******************************************************************************/
void OS2009Config::save(){
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
