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
#ifndef _weather_utils_h
#define _weather_utils_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
float convert_wind_units(int to, float value);
float mb2inch(float pressure);
void set_font(GtkWidget *widget, const gchar *description, const gint size);
void set_background_color(GtkWidget *widget, GdkColor *bgc);
void set_font_color(GtkWidget *widget, guint16 red, guint16 green, guint16 blue);
float c2f(float temp);
void swap_temperature(int *hi, int *low);
GtkWidget* lookup_widget(GtkWidget* widget, const gchar* widget_name);
#define GLADE_HOOKUP_OBJECT(component,widget,name) \
    g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
    g_object_set_data (G_OBJECT (component), name, widget)
GtkWidget* create_button_with_image(const char *path, const char *image_name,
				    int image_size, gboolean with_border);
GtkWidget* create_tree_view(GtkListStore* list);
GtkWidget* create_scrolled_window_with_text(const char* text,
					    GtkJustification justification);
/*******************************************************************************/
#endif
