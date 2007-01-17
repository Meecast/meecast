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
#include "weather-home_common.h"

extern gchar     *_weather_state_name;
extern gchar     *_weather_station_name;
extern gchar     *_weather_station_id;
extern gchar     *_weather_dir_name;
extern GtkWidget *buttons[];
extern int 	 boxs_offset[];
extern GtkWidget *weather_window_popup;
extern GtkWidget *box;
extern gchar     path_large_icon[];
extern gchar      _weather_temperature_unit;
/* by Pavel */
extern	gint	days_to_show;


gboolean weather_window_popup_show (GtkWidget *widget,GdkEvent *event,gpointer user_data);
void weather_window_preference (GtkWidget *widget,GdkEvent *event,gpointer user_data);
int c2f(int temp);
void set_font_size(GtkWidget *widget, char font_size);

