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
#ifndef _weather_parser_h
#define _weather_parser_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
GtkWidget* create_omweather(void);
GtkWidget* create_main_screen(void);
gboolean main_window_button_key_press_cb(GtkWidget *widget, GdkEventKey *event,
					    gpointer user_data);
gboolean main_window_state_event_cb(GtkWidget *widget, GdkEventWindowState *event,
					    gpointer user_data);
gint dbus_callback(const gchar *interface, const gchar *method, GArray *arguments,
			gpointer data, osso_rpc_t *retval);
/*******************************************************************************/
#endif

