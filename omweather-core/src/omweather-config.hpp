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
#include <gconf/gconf-client.h>
/*******************************************************************************/
class Config{
    protected:
    GConfClient         *gconf_client;
    StationsList        user_stations_list;
    std::string         icons_set_base;
    std::string         icon_set;
    std::string         font;
    std::string         current_source;
    std::string         current_country;
    std::string         current_station_name;
    std::string         current_station_id;
    std::string         current_station_source;
    std::string         iap_http_proxy_host;
    std::string         cache_directory;
    int                 iap_http_proxy_port;
    int                 update_interval;
    int                 switch_time;
    int                 icons_size;
    int                 icons_layout;
    int                 text_position;
    int                 current_settings_page;
    int                 days_to_show;
    int                 previos_days_to_show;
    int                 distance_units;
    int                 wind_units;
    int                 clicking_type;
    int                 temperature_units;
    int                 pressure_units;
    int                 mode;
    int                 view_mode;
    int                 data_valid_interval;
    bool                transparency;
    bool                separate;
    bool                swap_hi_low_temperature;
    bool                show_station_name;
    bool                show_arrows;
    bool                downloading_after_connecting;
    bool                show_wind;
/*    bool      show_wind_gust;*/
    bool                show_weather_for_two_hours;
    GdkColor            font_color;
    GdkColor            background_color;
    public:
            Config();
            virtual ~Config();
            virtual bool read();
            virtual void save();
            bool read_proxy_settings();
            virtual Param param(const std::string) const;
};
/*******************************************************************************/
class OS2008Config : public Config{
    protected:
    bool        gps_station;
    bool        use_sensor;
    int         display_at;
    int         sensor_update_time;
    int         alpha_comp;
    int         corner_radius;
    public:
            OS2008Config();
            bool read();
            void save();
            Param param(const std::string) const;
};
/*******************************************************************************/
class OS2009Config : public OS2008Config{
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
