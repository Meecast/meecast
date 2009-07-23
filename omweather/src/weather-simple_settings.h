/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
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
#ifndef _weather_simple_settings_h
#define _weather_simple_settings_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
void weather_simple_window_settings(gpointer user_data);
GtkWidget* create_and_full_stations_buttons(void);
extern GtkListStore* get_all_information_about_station(gchar *source, gchar *station_code);
extern gint get_country_code(gchar *source, gchar *country_name);
extern gint get_state_code(gchar *source, gchar *country_name);
GtkTreeIter add_station_to_user_list(gchar *weather_station_id,gchar *weather_station_name,
          gboolean is_gps, gchar *source, gint position);

#endif
