/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
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
#ifndef _weather_download_h
#define _weather_download_h 1
/*******************************************************************************/
#include "weather-common.h"
#include <curl/multi.h>

#ifdef USE_DBUS
#include <osso-ic.h>
#include <osso-ic-dbus.h>
#include <dbus/dbus.h>
#endif

#include <dbus/dbus-glib.h>
/*******************************************************************************/
struct HtmlFile {
  char *filename;
  FILE *stream;
};
/*******************************************************************************/
CURL* weather_curl_init(CURL *curl_handle);
gboolean download_html(gpointer data);
void weather_initialize_dbus(void);
gboolean check_connected(void);
void clean_download(void);
#ifndef USE_CONIC
void iap_callback(struct iap_event_t *event, void *arg);
#endif
gboolean get_station_url(gchar **url, struct HtmlFile *html_file, gboolean first);
GtkWidget* create_window_update(void);
/*******************************************************************************/
extern WeatherSource	weather_sources[];
/*******************************************************************************/
extern void config_update_proxy(void);
extern void add_current_time_event(void);
extern void popup_window_destroy(void);
extern void update_weather(gboolean show_update_window);
extern void redraw_home_window(gboolean first_start);
/*******************************************************************************/
#endif
