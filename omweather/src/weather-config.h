/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * Copyright (C) 2008      Andrew Olmsted
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
#ifndef _weather_config_h
#define _weather_config_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
#define GCONF_KEY_PREFIX                        "/apps/maemo/omweather"
#define GCONF_KEY_HTTP_PROXY_PREFIX             "/system/http_proxy"
#define GCONF_KEY_PROXY_PREFIX                  "/system/proxy"
#define GCONF_KEY_CLOCK_PREFIX                  "/apps/osso/clock"
#define GCONF_KEY_WEATHER_DIR_NAME              GCONF_KEY_PREFIX"/weather-dir"
#define GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME	GCONF_KEY_PREFIX"/current-country-name"
#define GCONF_KEY_WEATHER_CURRENT_SOURCE_NAME	GCONF_KEY_PREFIX"/current-source-name"
#define GCONF_KEY_WEATHER_STATE_NAME		GCONF_KEY_PREFIX"/state-name"
#define GCONF_KEY_WEATHER_CURRENT_STATION_NAME	GCONF_KEY_PREFIX"/current-station-name"
#define GCONF_KEY_WEATHER_CURRENT_STATION_ID	GCONF_KEY_PREFIX"/current-station-id"
#define GCONF_KEY_WEATHER_ICONS_SIZE		GCONF_KEY_PREFIX"/icons-size"
#define GCONF_KEY_WEATHER_FONT_COLOR		GCONF_KEY_PREFIX"/font-color"
#define GCONF_KEY_WEATHER_FONT			GCONF_KEY_PREFIX"/font"
#define GCONF_KEY_WEATHER_BACKGROUND_COLOR	GCONF_KEY_PREFIX"/background-color"
#define GCONF_KEY_ENABLE_TRANSPARENCY		GCONF_KEY_PREFIX"/transparency"
#define GCONF_KEY_ALPHA_COMPONENT		GCONF_KEY_PREFIX"/alpha-component"
#define GCONF_KEY_CORNER_RADIUS			GCONF_KEY_PREFIX"/corner-radius"
#define GCONF_KEY_SEPARATE_DATA			GCONF_KEY_PREFIX"/separate-data"
#define GCONF_KEY_SWAP_TEMPERATURE		GCONF_KEY_PREFIX"/swap-temperature"
#define GCONF_KEY_SHOW_WIND			    GCONF_KEY_PREFIX"/show-wind"
#define GCONF_KEY_SHOW_WIND_GUST        GCONF_KEY_PREFIX"/show-wind-gust"
#define GCONF_KEY_SHOW_STATION_NAME		GCONF_KEY_PREFIX"/show-station-name"
#define GCONF_KEY_SHOW_ARROWS			GCONF_KEY_PREFIX"/show-arrows"
#define GCONF_KEY_WEATHER_UPDATE_INTERVAL	GCONF_KEY_PREFIX"/update-interval"
#define GCONF_KEY_WEATHER_TEMPERATURE_UNIT	GCONF_KEY_PREFIX"/temperature-unit"
#define GCONF_KEY_ICONS_LAYOUT			GCONF_KEY_PREFIX"/icons-layout"   
#define GCONF_KEY_TEXT_POSITION			GCONF_KEY_PREFIX"/text-position"
#define GCONF_KEY_WEATHER_STATIONS_LIST		GCONF_KEY_PREFIX"/station-list"
#define GCONF_KEY_WEATHER_ALERTS_LIST		GCONF_KEY_PREFIX"/alerts-list"
#define GCONF_KEY_WEATHER_DAYS			GCONF_KEY_PREFIX"/days"
#define GCONF_KEY_WEATHER_DISTANCE_UNITS	GCONF_KEY_PREFIX"/distance-units"
#define GCONF_KEY_WEATHER_PRESSURE_UNITS	GCONF_KEY_PREFIX"/pressure-units"
#define GCONF_KEY_WEATHER_WIND_UNITS		GCONF_KEY_PREFIX"/wind-units"
#define GCONF_KEY_WEATHER_ICON_SET		GCONF_KEY_PREFIX"/icon-set"
#define GCONF_KEY_CURRENT_STATION_SOURCE	GCONF_KEY_PREFIX"/current-station-source"
#define GCONF_KEY_WEATHER_VALID_DATA_TIME	GCONF_KEY_PREFIX"/valid-time"
#define GCONF_KEY_WEATHER_PROGRAM_VERSION	GCONF_KEY_PREFIX"/version"
#define GCONF_KEY_WEATHER_SETTING_TAB_NUMBER	GCONF_KEY_PREFIX"/settings-tab-number"
#if defined(OS2008) || defined(DEBUGTEMP) || defined(OS2009)
#define GCONF_KEY_USE_SENSOR            GCONF_KEY_PREFIX"/use-sensor"
#define GCONF_KEY_DISPLAY_SENSOR_AT     GCONF_KEY_PREFIX"/display-sensor-at"
#define GCONF_KEY_SENSOR_UPDATE_TIME    GCONF_KEY_PREFIX"/sensor-update-time"
#endif
#define GCONF_KEY_WEATHER_SWITCH_TIME   GCONF_KEY_PREFIX"/switch-time"
#define GCONF_KEY_PROXY_MODE            GCONF_KEY_PROXY_PREFIX"/mode"
#define GCONF_KEY_HTTP_PROXY_ON         GCONF_KEY_HTTP_PROXY_PREFIX"/use_http_proxy"
#define GCONF_KEY_HTTP_PROXY_HOST       GCONF_KEY_HTTP_PROXY_PREFIX"/host"
#define GCONF_KEY_HTTP_PROXY_PORT       GCONF_KEY_HTTP_PROXY_PREFIX"/port"
#define GCONF_KEY_CLOCK_REMOTE_LOCATION		GCONF_KEY_CLOCK_PREFIX"/remote-location"
#define GCONF_KEY_CLOCK_HOME_LOCATION		GCONF_KEY_CLOCK_PREFIX"/home-location"
#define GCONF_KEY_DOWNLOADING_AFTER_CONNECTING  GCONF_KEY_PREFIX"/downloading-after-connecting"
#define GCONF_KEY_USE_GPS_STATION		GCONF_KEY_PREFIX"/gps-station"
#define GCONF_KEY_GPS_STATION_NAME		GCONF_KEY_PREFIX"/gps-station_name"
#define GCONF_KEY_GPS_STATION_ID		GCONF_KEY_PREFIX"/gps-station_id"
#define GCONF_KEY_CLICK_FOR_LAUNCH_POPUP	GCONF_KEY_PREFIX"/click-for-launch-popup"
#define GCONF_KEY_SHOW_WEATHER_FOR_TWO_HOURS    GCONF_KEY_PREFIX"/show-weather-for-two-hours"
#define GCONF_KEY_MODE                          GCONF_KEY_PREFIX"/mode"
#define GCONF_KEY_UPDATE_GSM GCONF_KEY_PREFIX"/update-gsm"
#define GCONF_KEY_UPDATE_WLAN GCONF_KEY_PREFIX"/update-WLAN"
#define GCONF_KEY_VIEW_MODE"/view_mode"
//#define GCONF_KEY_STATION_COUNTRY_BUTTON_PRESSED"/station_country_button_pressed"
//#define GCONF_KEY_STATION_SOURCE_BUTTON_PRESSED"/station_source_button_pressed"
//#define GCONF_KEY_STATION_REGION_BUTTON_PRESSED"/station_region_button_pressed"
//#define GCONF_KEY_STATION_NAME_BUTTON_PRESSED"/station_name_button_pressed;"
/*******************************************************************************/
gboolean config_set_weather_dir_name(gchar *new_weather_dir_name);
void config_update_proxy(void);
void check_current_station_id(void);
int read_config(AppletConfig*);
void config_save(AppletConfig*);
GtkListStore* create_time_update_list(void);
void fill_user_stations_list_from_clock(GtkListStore** list);
void fill_user_stations_list(GSList *source_list, GtkListStore** list);
GSList* create_stations_string_list(void);
/*******************************************************************************/
#endif
