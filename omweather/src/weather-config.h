/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
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
#define GCONF_KEY_PREFIX		"/apps/maemo/omweather"
#define GCONF_KEY_HTTP_PROXY_PREFIX	"/system/http_proxy"
#define GCONF_KEY_CLOCK_PREFIX		"/apps/osso/clock"
#define GCONF_KEY_WEATHER_DIR_NAME		GCONF_KEY_PREFIX"/weather-dir"
#define GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME	GCONF_KEY_PREFIX"/current-country-name"
#define GCONF_KEY_WEATHER_STATE_NAME		GCONF_KEY_PREFIX"/state-name"
#define GCONF_KEY_WEATHER_CURRENT_STATION_NAME	GCONF_KEY_PREFIX"/current-station-name"
#define GCONF_KEY_WEATHER_CURRENT_STATION_ID	GCONF_KEY_PREFIX"/current-station-id"
#define GCONF_KEY_WEATHER_ICONS_SIZE		GCONF_KEY_PREFIX"/icons-size"
#define GCONF_KEY_WEATHER_FONT_COLOR		GCONF_KEY_PREFIX"/font-color"
#define GCONF_KEY_WEATHER_FONT			GCONF_KEY_PREFIX"/font"
#define GCONF_KEY_WEATHER_THEME_OVERRIDE	GCONF_KEY_PREFIX"/theme-override"
#define GCONF_KEY_WEATHER_BACKGROUND_COLOR	GCONF_KEY_PREFIX"/background-color"
#define GCONF_KEY_ENABLE_TRANSPARENCY		GCONF_KEY_PREFIX"/transparency"
#define GCONF_KEY_ALPHA_COMPONENT		GCONF_KEY_PREFIX"/alpha-component"
#define GCONF_KEY_CORNER_RADIUS			GCONF_KEY_PREFIX"/corner-radius"
#define GCONF_KEY_SEPARATE_DATA			GCONF_KEY_PREFIX"/separate-data"
#define GCONF_KEY_SWAP_TEMPERATURE		GCONF_KEY_PREFIX"/swap-temperature"
#define GCONF_KEY_SHOW_WIND			GCONF_KEY_PREFIX"/show-wind"
#define GCONF_KEY_SHOW_STATION_NAME		GCONF_KEY_PREFIX"/show-station-name"
#define GCONF_KEY_SHOW_ARROWS			GCONF_KEY_PREFIX"/show-arrows"
#define GCONF_KEY_WEATHER_UPDATE_INTERVAL	GCONF_KEY_PREFIX"/update-interval"
#define GCONF_KEY_WEATHER_TEMPERATURE_UNIT	GCONF_KEY_PREFIX"/temperature-unit"
#define GCONF_KEY_ICONS_LAYOUT			GCONF_KEY_PREFIX"/icons-layout"   
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
#if defined(OS2008) || defined(DEBUGTEMP)
#define GCONF_KEY_USE_SENSOR			GCONF_KEY_PREFIX"/use-sensor"
#define GCONF_KEY_DISPLAY_SENSOR_AT		GCONF_KEY_PREFIX"/display-sensor-at"
#define GCONF_KEY_SENSOR_UPDATE_TIME		GCONF_KEY_PREFIX"/sensor-update-time"
#endif
#define GCONF_KEY_WEATHER_SWITCH_TIME		GCONF_KEY_PREFIX"/switch-time"
#define GCONF_KEY_HTTP_PROXY_ON			GCONF_KEY_HTTP_PROXY_PREFIX"/use_http_proxy"
#define GCONF_KEY_HTTP_PROXY_HOST		GCONF_KEY_HTTP_PROXY_PREFIX"/host"
#define GCONF_KEY_HTTP_PROXY_PORT		GCONF_KEY_HTTP_PROXY_PREFIX"/port"
#define GCONF_KEY_CLOCK_REMOTE_LOCATION		GCONF_KEY_CLOCK_PREFIX"/remote-location"
#define GCONF_KEY_CLOCK_HOME_LOCATION		GCONF_KEY_CLOCK_PREFIX"/home-location"
#define GCONF_KEY_DOWNLOADING_AFTER_CONNECTING  GCONF_KEY_PREFIX"/downloading-after-connecting"
#define GCONF_KEY_USE_GPS_STATION		GCONF_KEY_PREFIX"/gps-station"
#define GCONF_KEY_GPS_STATION_NAME		GCONF_KEY_PREFIX"/gps-station_name"
#define GCONF_KEY_GPS_STATION_ID		GCONF_KEY_PREFIX"/gps-station_id"
#define GCONF_KEY_SHOW_WEATHER_FOR_TWO_HOURS    GCONF_KEY_PREFIX"/show-weather-for-two-hours"

/*******************************************************************************/
extern	gchar	path_large_icon[_POSIX_PATH_MAX];
/*******************************************************************************/
gboolean config_set_weather_dir_name(gchar *new_weather_dir_name);
void config_update_proxy(void);
int read_config(AppletConfig*);
void config_save(AppletConfig*);
GtkListStore* create_time_update_list(void);
void fill_user_stations_list_from_clock(GtkListStore** list);
void fill_user_stations_list(GSList *source_list, GtkListStore** list);
GSList* create_stations_string_list(void);
/*******************************************************************************/
extern void remove_periodic_event(void);
extern void add_periodic_event(time_t last_update);
extern void add_gps_event(guint interval);
extern gboolean switch_timer_handler(gpointer data);
extern void update_weather(gboolean show_update_window);
#if defined(OS2008) || defined(DEBUGTEMP)
extern void read_sensor(gint need_redraw);
#endif
/*******************************************************************************/
#endif
