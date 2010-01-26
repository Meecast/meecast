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
#ifndef _weather_download_h
#define _weather_download_h 1
/*******************************************************************************/
#include "weather-event.h"
struct download_params{
    char        *proxy_host;
    int         proxy_port;
    char        *url;
    char        *filename;
    int         status;
    gboolean    hour_data;
};
/*******************************************************************************/
struct download_data{
    struct download_params  params;
    pthread_t               tid;
};
/*******************************************************************************/
gchar* get_new_gismeteo_code(gchar *old_code, gchar *source);
gboolean download_html(void *user_data);
void* download_url(void *user_data);
int data_read(void *buffer, size_t size, size_t nmemb, void *stream);
gboolean get_station_url(gchar **url, gchar **filename, gchar **hour_url, gchar **filename_hour, gboolean first);
gboolean wakeup_connection(void);
/*******************************************************************************/
#endif
