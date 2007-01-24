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

#define OMW_RESPONSE_ADD_CUSTOM_STATION 10000
#include <gtk/gtk.h>
#include "weather-home_common.h"

extern GtkWidget *box;
extern gchar     *_weather_dir_name;
extern gchar     *_weather_country_name;
extern gchar     *_weather_state_name;
extern gchar     *_weather_station_name;
extern gchar     *_weather_station_id;
extern gchar     *_weather_icon_size;
extern gboolean  _enable_transparency;
extern int       _weather_periodic_update;
extern GdkColor  _weather_font_color;
extern GtkWidget *weather_window_popup;  
extern GSList    *time_update_list;
extern gchar     _weather_temperature_unit;
extern gboolean  not_event;
extern int       _weather_layout;
/* by Pavel */
extern	gint	days_to_show;
extern	gint	distance_units;

void weather_window_preference (GtkWidget *widget,GdkEvent *event,gpointer user_data);
void fill_station_list_view (GtkWidget *station_list_view,GtkListStore *station_list_store);

GtkWidget  *countrys, *states, *stations, *icon_size, *layout_type, *update_time, *temperature_unit, *days_number;
GtkWidget  *custom_station_name, *custom_station_code, *units;

static GSList *stations_list_in_state = NULL; /* Station list of country or state or province.  Use in combobox*/
static GtkListStore *station_list_store;
static GtkWidget *station_list_view;
static GtkWidget *window_add_station;

struct station_and_weather_code {
        gchar *station_name;
        gchar *station_code;
};

gchar *weather_station_id;
int index_state = 0; /* Position state of the list */
int index_station = 0; /* Position station of the list */
static char flag_update_station = FALSE; /* Flag update station list */
static gchar *_weather_station_id_temp; /* Temporary value for weather_station_id */

extern void remove_periodic_event(void);
extern void weather_buttons_init(void);
