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
#ifndef _weather_window_popup_h
#define _weather_window_popup_h 1
/*******************************************************************************/
#include "weather-home_common.h"
/*******************************************************************************/
extern	GtkWidget	*buttons[];
extern	int		boxs_offset[];
extern	gchar		path_large_icon[];
/*******************************************************************************/
GtkWidget* create_header_widget(int i);
GtkWidget* create_footer_widget(void);
GtkWidget* create_current_weather_widget(void);
GtkWidget* create_temperature_range_widget(int i);
GtkWidget* create_24_hours_widget(int i, time_t current_time);
GtkWidget* create_sun_time_widget(int i);
gboolean weather_window_popup_show(GtkWidget *widget,GdkEvent *event,gpointer user_data);
void weather_window_settings(GtkWidget *widget,GdkEvent *event,gpointer user_data);
float convert_wind_units(int to, float value);
void pre_update_weather(void);
/* extern functions */
extern gpointer hash_table_find(gpointer key);
extern void weather_initialize_dbus(void);
extern void swap_temperature(int *hi, int *low);
extern int c2f(int temp);
extern void set_font_size(GtkWidget *widget, char font_size);
/*******************************************************************************/
#endif
