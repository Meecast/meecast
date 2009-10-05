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
#define GCONF_KEY_HTTP_PROXY_PREFIX                 "/system/http_proxy"
#define GCONF_KEY_PROXY_PREFIX                      "/system/proxy"
/*******************************************************************************/
Config::Config(){
    document = NULL;
    root_node = NULL;
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
    if(document)
        xmlFreeDoc(document);
}
/*******************************************************************************/
void Config::prepare_read(){
    std::string filename;

    home_dir = getenv("HOME");
    if(home_dir.empty())
        filename = "/tmp/omweather.xml";
    else
        filename = home_dir + "/" + "omweather.xml";

    std::ifstream file(filename.c_str());
    if(file.is_open())
        file.close();
    document = xmlReadFile(filename.c_str(), NULL, 0);
    if(document)
        root_node = xmlDocGetRootElement(document);
}
/*******************************************************************************/
bool Config::read(){
    if(!document)
        prepare_read();
    if(!root_node)
        return false;
    parse_children(root_node->children);
    return true;
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
void Config::prepare_save(){
    if(document)
        xmlFreeDoc(document);
    document = xmlNewDoc(BAD_CAST "1.0");
    xmlNewGlobalNs(document, BAD_CAST "https://garage.maemo.org/projects/omweather/", BAD_CAST "");
    root_node = xmlNewNode(NULL, BAD_CAST "omweather");
}
/*******************************************************************************/
void Config::save_to_file(){
    if(!document)
        return;
    std::string filename = home_dir + "/" + "omweather.xml";
    xmlSaveFormatFileEnc(filename.c_str(), document, "UTF-8", 1);
}
/*******************************************************************************/
void Config::save(){
    if(!document)
        prepare_save();
    if(!root_node)
        return;
    char buffer[128];
    xmlNewChild(root_node, NULL, BAD_CAST "version", BAD_CAST version.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "current-station-source", BAD_CAST current_station_source.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "current-station-name", BAD_CAST current_station_name.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "current-station-code", BAD_CAST current_station_code.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "current-icons-set", BAD_CAST current_icons_set.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "last-source", BAD_CAST last_source.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "last-country", BAD_CAST last_country.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "cache-directory", BAD_CAST cache_directory.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "icons-preset", BAD_CAST icons_preset.toString().c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "icons-size", BAD_CAST icons_size.toString().c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "view-mode", BAD_CAST view_mode.toString().c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "settings-mode", BAD_CAST settings_mode.toString().c_str());
    if(show_arrows)
        xmlNewChild(root_node, NULL, BAD_CAST "show-arrows", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "show-arrows", BAD_CAST "false");
    if(show_station_name)
        xmlNewChild(root_node, NULL, BAD_CAST "show-station-name", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "show-station-name", BAD_CAST "false");
    if(show_wind)
        xmlNewChild(root_node, NULL, BAD_CAST "show-wind", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "show-wind", BAD_CAST "false");
    if(show_wind_gust)
        xmlNewChild(root_node, NULL, BAD_CAST "show-wind-gust", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "show-wind-gust", BAD_CAST "false");
    if(show_detail_weather)
        xmlNewChild(root_node, NULL, BAD_CAST "show-detail-weather", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "show-detail-weather", BAD_CAST "false");
    xmlNewChild(root_node, NULL, BAD_CAST "text-position", BAD_CAST text_position.toString().c_str());
    if(swap_temperature)
        xmlNewChild(root_node, NULL, BAD_CAST "swap-temperature", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "swap-temperature", BAD_CAST "false");
    xmlNewChild(root_node, NULL, BAD_CAST "font-color", BAD_CAST font_color.get());
    xmlNewChild(root_node, NULL, BAD_CAST "background-color", BAD_CAST background_color.get());
    if(separate_data)
        xmlNewChild(root_node, NULL, BAD_CAST "separate-_data", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "separate-data", BAD_CAST "false");
    if(transparency)
        xmlNewChild(root_node, NULL, BAD_CAST "transparency", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "transparency", BAD_CAST "false");
    xmlNewChild(root_node, NULL, BAD_CAST "wind-units", BAD_CAST wind_units.toString().c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "pressure-units", BAD_CAST pressure_units.toString().c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "distance-units", BAD_CAST distance_units.toString().c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "temperature-units", BAD_CAST temperature_units.toString().c_str());
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%d", valid_time);
    xmlNewChild(root_node, NULL, BAD_CAST "valid-time", BAD_CAST buffer);
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%d", switch_time);
    xmlNewChild(root_node, NULL, BAD_CAST "switch-time", BAD_CAST buffer);
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%d", update_time);
    xmlNewChild(root_node, NULL, BAD_CAST "update-time", BAD_CAST buffer);
    if(auto_download)
        xmlNewChild(root_node, NULL, BAD_CAST "auto-download", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "auto-download", BAD_CAST "false");
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%d", current_settings_page);
    xmlNewChild(root_node, NULL, BAD_CAST "settings-tab", BAD_CAST buffer);
    xmlNewChild(root_node, NULL, BAD_CAST "font", BAD_CAST font.c_str());
    xmlNewChild(root_node, NULL, BAD_CAST "click-type", BAD_CAST click_type.toString().c_str());
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%d", days_number);
    xmlNewChild(root_node, NULL, BAD_CAST "days-number", BAD_CAST buffer);
    save_to_file();
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
    position.current(STATION);
    sensor_update_time = 0;
    alpha_component = 0;
    corner_radius = 10;
}
/*******************************************************************************/
bool OS2008Config::read(){
    Config::read();
    if(!root_node)
        return false;
    parse_children(root_node->children);
    return true;
}
/*******************************************************************************/
void OS2008Config::parse_children(xmlNode *node){
    xmlNode     *child_node = NULL;
    xmlChar     *val = NULL;
    int         t;

    while(node){
        if(node->type == XML_ELEMENT_NODE){
            /* use-sensor */
            if(!xmlStrcmp(node->name, (const xmlChar*)"use-sensor")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "true")){
                    use_sensor = true;
                    xmlFree(val);
                    val = xmlGetProp(node, (const xmlChar*)"position");
                    position.current((char*)val);
                    xmlFree(val);
                    val = xmlGetProp(node, (const xmlChar*)"time");
                    sensor_update_time = atoi((char*)val);
                    xmlFree(val);
                }
                else{
                    use_sensor = false;
                    position.current(STATION);
                    sensor_update_time = 0;
                }
            }
            /* alpha-component */
            if(!xmlStrcmp(node->name, (const xmlChar*)"alpha-component")){
                val = xmlNodeGetContent(node);
                alpha_component = atoi((char*)val);
                xmlFree(val);
            }
            /* corner-radius */
            if(!xmlStrcmp(node->name, (const xmlChar*)"corner-radius")){
                val = xmlNodeGetContent(node);
                corner_radius = atoi((char*)val);
                xmlFree(val);
            }
        }
        node = node->next;
    }
}
/*******************************************************************************/
void OS2008Config::save(){
    Config::save();
    if(!root_node)
        return;
    char buffer[128];
    if(use_sensor)
        xmlNewChild(root_node, NULL, BAD_CAST "use-sensor", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "use-sensor", BAD_CAST "false");
    xmlNewProp(root_node, BAD_CAST "position", BAD_CAST position.toString().c_str());
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%d", sensor_update_time);
    xmlNewProp(root_node, BAD_CAST "time", BAD_CAST buffer);
    save_to_file();
}
/*******************************************************************************/
Param OS2008Config::param(const std::string param_name) const{
    Param       p;
    /* use_sensor */
    if(param_name == "use_sensor"){
        p.bool_param = use_sensor;
        goto exit;
    }
    /* position */
    if(param_name == "position"){
        p.int_param = position.toInt();
        goto exit;
    }
    /* sensor_update_time */
    if(param_name == "sensor_update_time"){
        p.int_param = sensor_update_time;
        goto exit;
    }
    /* alpha_component */
    if(param_name == "alpha_component"){
        p.int_param = alpha_component;
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
    OS2008Config::read();
    if(!root_node)
        return false;
    parse_children(root_node->children);
    return true;
}
/*******************************************************************************/
void OS2009Config::parse_children(xmlNode *node){
    xmlNode     *child_node = NULL;
    xmlChar     *val = NULL;
    int         t;

    while(node){
            /* update-wlan */
            if(!xmlStrcmp(node->name, (const xmlChar*)"update-wlan")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "true"))
                    update_wlan = true;
                else
                    update_wlan = false;
                xmlFree(val);
            }
            /* update-gsm */
            if(!xmlStrcmp(node->name, (const xmlChar*)"update-gsm")){
                val = xmlNodeGetContent(node);
                if(!strcmp((const char*)val, "true"))
                    update_gsm = true;
                else
                    update_gsm = false;
                xmlFree(val);
            }
        node = node->next;
    }
}
/*******************************************************************************/
void OS2009Config::save(){
    OS2008Config::save();
    if(!root_node)
        return;
    char buffer[128];
    if(update_wlan)
        xmlNewChild(root_node, NULL, BAD_CAST "update-wlan", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "update-wlan", BAD_CAST "false");
    if(update_gsm)
        xmlNewChild(root_node, NULL, BAD_CAST "update-gsm", BAD_CAST "true");
    else
        xmlNewChild(root_node, NULL, BAD_CAST "update-gsm", BAD_CAST "false");
    save_to_file();
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
