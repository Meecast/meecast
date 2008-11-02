/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
#ifndef _weather_sensor_h
#define _weather_sensor_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
GtkWidget* create_sensor_page(GtkWidget *config_window);
void use_sensor_button_toggled_handler(GtkToggleButton *togglebutton,
                                        		    gpointer user_data);
void display_group_change_handler(GtkRadioButton *button, gpointer user_data);
gboolean read_sensor(gboolean need_redraw);
WDB* create_sensor_icon_widget(const int icon_size, gboolean transparency,
				gboolean draw_day_label, GdkColor *color);
guint get_time_from_index(guint index);
guint get_index_from_time(guint time);
/*******************************************************************************/
extern void redraw_home_window(gboolean first_start);
extern WDB* create_weather_day_button(const char *text, const char *icon,
				const gint icon_size, gboolean transparency,
				gboolean draw_day_label, GdkColor *color);
extern float c2f(float temp);
extern void check_buttons_changed_handler(GtkToggleButton *button,
							    gpointer user_data);
extern void combo_boxs_changed_handler(GtkComboBox *combobox, gpointer user_data);
/*******************************************************************************/
#endif
