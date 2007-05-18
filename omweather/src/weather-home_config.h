/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) Superkaramba's Liquid Weather ++ team
 *	for ALL the artwork (icons)
 *        The maintainer (also the main author I believe)
 *        is Matthew <dmbkiwi@yahoo.com>
 *  http://liquidweather.net/icons.html#iconsets
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
#ifndef _weather_home_config_h
#define _weather_home_config_h 1

#include "weather-home_common.h"

#define GCONF_KEY_PREFIX "/apps/maemo/omweather"
#define GCONF_KEY_WEATHER_DIR_NAME         GCONF_KEY_PREFIX"/weather-dir"
#define GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME     GCONF_KEY_PREFIX"/current-country-name"
#define GCONF_KEY_WEATHER_STATE_NAME		GCONF_KEY_PREFIX"/state-name"
#define GCONF_KEY_WEATHER_CURRENT_STATION_NAME	GCONF_KEY_PREFIX"/current-station-name"
#define GCONF_KEY_WEATHER_CURRENT_STATION_ID	GCONF_KEY_PREFIX"/current-station-id"
#define GCONF_KEY_WEATHER_STATION_IDS		GCONF_KEY_PREFIX"/station-ids"
#define GCONF_KEY_WEATHER_ICONS_SIZE		GCONF_KEY_PREFIX"/icons-size"
#define GCONF_KEY_WEATHER_FONT_COLOR		GCONF_KEY_PREFIX"/font-color"
#define GCONF_KEY_ENABLE_TRANSPARENCY		GCONF_KEY_PREFIX"/transparency"
#define GCONF_KEY_SEPARATE_DATA			GCONF_KEY_PREFIX"/separate-data"
#define GCONF_KEY_WEATHER_UPDATE_INTERVAL	GCONF_KEY_PREFIX"/update-interval"
#define GCONF_KEY_WEATHER_TEMPERATURE_UNIT	GCONF_KEY_PREFIX"/temperature-unit"
#define GCONF_KEY_ICONS_LAYOUT			GCONF_KEY_PREFIX"/icons-layout"   
#define GCONF_KEY_WEATHER_STATIONS_LIST		GCONF_KEY_PREFIX"/station-list"   
#define GCONF_KEY_WEATHER_DAYS			GCONF_KEY_PREFIX"/days"
#define GCONF_KEY_WEATHER_DISTANCE_UNITS	GCONF_KEY_PREFIX"/distance-units"
#define GCONF_KEY_WEATHER_WIND_UNITS		GCONF_KEY_PREFIX"/wind-units"
#define GCONF_KEY_WEATHER_ICON_SET		GCONF_KEY_PREFIX"/icon-set"
#define GCONF_KEY_WEATHER_VALID_DATA_TIME	GCONF_KEY_PREFIX"/valid-time"
#define GCONF_KEY_WEATHER_PROGRAM_VERSION	GCONF_KEY_PREFIX"/version"
#define GCONF_KEY_HTTP_PROXY_PREFIX "/system/http_proxy"
#define GCONF_KEY_CURRENT_CONNECTIVITY  "/system/osso/connectivity/IAP/current"
#define GCONF_KEY_HTTP_PROXY_ON GCONF_KEY_HTTP_PROXY_PREFIX"/use_http_proxy"
#define GCONF_KEY_HTTP_PROXY_HOST GCONF_KEY_HTTP_PROXY_PREFIX"/host"
#define GCONF_KEY_HTTP_PROXY_PORT GCONF_KEY_HTTP_PROXY_PREFIX"/port"
#define GCONF_KEY_CLOCK_PREFIX "/apps/osso/clock"
#define GCONF_KEY_CLOCK_REMOTE_LOCATION GCONF_KEY_CLOCK_PREFIX"/remote-location"
#define GCONF_KEY_CLOCK_HOME_LOCATION GCONF_KEY_CLOCK_PREFIX"/home-location"


extern GSList   *time_update_list;
extern	gchar	path_large_icon[_POSIX_PATH_MAX];

gboolean config_set_weather_dir_name(gchar *new_weather_dir_name);
void config_update_proxy(void);


#endif
