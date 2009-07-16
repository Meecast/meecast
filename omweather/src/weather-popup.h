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
#ifndef _weather_popup_h
#define _weather_popup_h 1
/*******************************************************************************/
#include "weather-common.h"
#include "weather-sources.h"
/*******************************************************************************/
GtkWidget* create_sun_time_widget(GHashTable *day);
GtkWidget* create_moon_phase_widget(GHashTable *current);
GtkWidget* create_time_updates_widget(GHashTable *current, gboolean change_color);

gboolean weather_window_popup(GtkWidget *widget, GdkEvent *event, gpointer user_data);

void settings_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data);
void refresh_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data);
void about_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data);
void popup_close_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data);
void maximize_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data);
void destroy_container (GtkWidget *widget, gpointer *data);

GtkWidget* create_day_tab(GHashTable *current, GHashTable *day, gchar **day_name);
GtkWidget* create_pseudo_day_tab(GHashTable *current, GHashTable *day, gchar **day_name);
GtkWidget* create_current_tab(GHashTable *current);
GtkWidget* create_hour_tab(void);
GtkWidget* create_toolbar_box(GtkCallback exit_function, GtkWidget *window, gboolean fullscreen_button);
gboolean make_current_tab(GtkWidget *vbox);
gboolean make_tab(GtkWidget *vbox);
gboolean make_hour_tab(GtkWidget *vbox);
GtkWidget* create_copyright_widget(const gchar *text, const gchar *image);
GtkWidget* create_window_header(const gchar *station_name, GtkWidget *popup_window);
void popup_switch_cb(GtkNotebook * nb, gpointer nb_page, gint page, gpointer data);
#ifdef CLUTTER
void popup_window_expose(GtkWidget *widget, GdkEventExpose *event);
#endif
void destroy_popup_window(void);
/*******************************************************************************/
extern void weather_window_settings(GtkWidget *widget,
				    gpointer user_data);
/*extern void pre_update_weather(void);*/
extern void update_weather(gboolean show_update_window);
extern void set_font(GtkWidget *widget, const gchar *description, const gint size);
extern void set_font_color (GtkWidget *widget, guint16 red, guint16 green, guint16 blue);
extern float c2f(float temp);
extern void swap_temperature(int *hi, int *low);
extern gpointer hash_table_find(gpointer key, gboolean search_short_name);
extern float convert_wind_units(int to, float value);
extern GtkWidget* create_button_with_image(const char *path, const char *image_name,
			int image_size, gboolean with_border, gboolean toggled);
extern void create_about_dialog(void);  /* create help dialog */
extern gboolean change_station_prev(GtkWidget *widget, GdkEvent *event,
                    		    gpointer user_data);
extern gboolean change_station_next(GtkWidget *widget, GdkEvent *event,
                    		    gpointer user_data);
/*******************************************************************************/
#endif
