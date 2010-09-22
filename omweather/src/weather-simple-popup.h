/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * Copyright (C) 2008 Andrew Olmsted
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _weather_simple_popup_h
#define _weather_simple_popup_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
void weather_simple_window_popup(GtkWidget *widget, gpointer user_data);
gchar* get_next_station_name(const gchar *current_station_name, GtkListStore *user_stations_list);
GtkWidget* create_mainbox_for_forecast_window(GtkWidget* window, gpointer user_data);
GtkWidget* create_top_buttons_box(GtkWidget* window, gpointer user_data);
GtkWidget* create_weather_collapsed_view(GtkWidget *vbox, gint day_number);
GtkWidget* create_weather_expanded_view(GtkWidget *vbox, gint day_number);
GtkWidget* create_weather_for_two_hours_collapsed_view(GtkWidget *vbox, gint day_number);
void show_expanded_day_button_handler(GtkWidget *button, gpointer user_data);
void show_detailes_of_current_day_button_handler(GtkWidget *button, gpointer user_data);
void show_collapsed_day_button_handler(GtkWidget *button, gpointer user_data);
void weather_simple_window_redraw(GtkWidget *window);
gboolean jump_panarea(gpointer user_data);
void donation_button_handler (void);
void click_url(GtkButton *button);
#if defined OS2009
HildonAppMenu* create_view_menu(void);
#endif
void popup_simple_window_expose(GtkWidget *widget, GdkEventExpose *event);

/*******************************************************************************/
#endif
