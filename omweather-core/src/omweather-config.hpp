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
#ifndef _omweather_config_hpp
#define _omweather_config_hpp 1
/*******************************************************************************/
#include "omweather-common.hpp"
#include "omweather-stations.hpp"
/*******************************************************************************/
class Config{
    std::string         home_dir;
    void                prepare_save();
    void                prepare_read();
    protected:
    xmlDoc              *document;
    xmlNode             *root_node;
    void                parse_children(xmlNode *node);
    void                save_to_file();
    StationsList        user_stations_list;
    std::string         cache_directory;
    std::string         icons_set_base;
    std::string         current_icons_set;
    std::string         font;
    std::string         last_source;
    std::string         last_country;
    std::string         current_station_name;
    std::string         current_station_code;
    std::string         current_station_source;
    std::string         version;
    int                 update_time;
    int                 switch_time;
    IconsSize           icons_size;
    IconsPreset         icons_preset;
    TextPosition        text_position;
    int                 current_settings_page;
    int                 days_number;
    int                 previos_days_number;
    DistanceUnits       distance_units;
    WindUnits           wind_units;
    ClickType           click_type;
    TemperatureUnits    temperature_units;
    PressureUnits       pressure_units;
    SettingsMode        settings_mode;
    ViewMode            view_mode;
    int                 valid_time;
    bool                transparency;
    bool                separate_data;
    bool                swap_temperature;
    bool                show_station_name;
    bool                show_arrows;
    bool                auto_download;
    bool                show_wind;
    bool                show_wind_gust;
    bool                show_detail_weather;
    Color               font_color;
    Color               background_color;
    public:
            Config();
            virtual ~Config();
            virtual bool read();
            virtual void save();
            virtual Param param(const std::string) const;
};
/*******************************************************************************/
class OS2008Config : public Config{
    void parse_children(xmlNode *node);
    protected:
    bool            use_sensor;
    SensorPosition  position;
    int             sensor_update_time;
    int             alpha_component;
    int             corner_radius;
    public:
            OS2008Config();
            bool read();
            virtual void save();
            Param param(const std::string) const;
};
/*******************************************************************************/
class OS2009Config : public OS2008Config{
    void parse_children(xmlNode *node);
    bool        update_gsm;
    bool        update_wlan;
    public:
            OS2009Config();
            bool read();
            void save();
            Param param(const std::string) const;
};
/*******************************************************************************/
#endif
