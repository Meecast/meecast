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
#ifndef _weather_home_download_h
#define _weather_home_download_h 1

#include "weather-home_common.h"
#include <curl/multi.h>
#include <osso-ic.h>

GString *url,*full_filename;
CURL *curl_handle = NULL;
CURL *curl_multi = NULL;
CURL *weather_curl_init(void);
gboolean download_html(void);
GtkWidget *update_window;     

struct HtmlFile {
  char *filename;
  FILE *stream;
};

struct HtmlFile html_file;
gint num_transfers = 0, num_msgs = 0;
GSList *tmplist = NULL;
struct weather_station *ws;


#endif

