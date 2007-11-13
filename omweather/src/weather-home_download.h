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
/*******************************************************************************/
#ifndef _weather_home_download_h
#define _weather_home_download_h 1
/*******************************************************************************/
#include "weather-home_common.h"
#include <curl/multi.h>
#include <osso-ic.h>
#include <osso-ic-dbus.h>
#include <dbus/dbus-glib.h>
/*******************************************************************************/
CURL* weather_curl_init(CURL *curl_handle);
gboolean download_html(gpointer data);
void weather_initialize_dbus(void);
gboolean check_connected(void);
void clean_download(void);
void pre_update_weather(void);
/*******************************************************************************/
struct HtmlFile {
  char *filename;
  FILE *stream;
};
/*******************************************************************************/
extern void config_update_proxy(void);
extern void add_current_time_event(void);
extern void popup_window_destroy(void);
/*******************************************************************************/
#endif
