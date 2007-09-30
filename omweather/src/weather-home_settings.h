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
#ifndef _weather_home_settings_h
#define _weather_home_settings_h 1
/*******************************************************************************/
#define OMW_RESPONSE_ADD_CUSTOM_STATION 10000
/*******************************************************************************/
#include "weather-home_common.h"
/*******************************************************************************/
extern gboolean  not_event;
extern	gchar	path_large_icon[_POSIX_PATH_MAX];
/*******************************************************************************/
void weather_window_settings(GtkWidget *widget, GdkEvent *event,
				gpointer user_data);
void fill_station_list_view(GtkWidget *station_list_view,
			    GtkListStore *station_list_store);
void station_list_view_select_handler(GtkTreeView *tree_view,
			    gpointer user_data);
void weather_window_edit_station(GtkWidget *widget,
            			   GdkEvent *event,
                    		   gpointer user_data);
void weather_window_add_station(GtkWidget *widget,
            			GdkEvent *event,
                    		gpointer user_data);
void changed_country(void);
void changed_state(void);
void changed_stations(void);
void weather_window_add_custom_station(void);

GtkWidget	*countrys,
		*states,
		*stations,
		*icon_size,
		*layout_type,
		*update_time,
		*temperature_unit,
		*days_number,
		*custom_station_name,
		*custom_station_code,
		*units,
		*iconset,
		*wunits,
		*valid_time_list;

static GtkListStore *station_list_store;
static GtkWidget *station_list_view;
static GtkWidget *window_add_station;

static char flag_update_station = FALSE; /* Flag update station list */
static gchar *_weather_station_id_temp; /* Temporary value for weather_station_id */
int create_icon_set_list(GtkWidget *store); /* scan /usr/share/weather/icons */
					     /* and add names to the list */
void create_help_dialog(void);	/* create help dialog */
GtkWidget* create_scrolled_window_with_text(const char* text,
					    GtkJustification justification);
int get_active_time_update(GtkTreeModel *list, int time, const gchar *text);
void update_iterval_changed_handler(GtkComboBox *widget, gpointer user_data);
void transparency_button_toggled_handler(GtkToggleButton *togglebutton,
                                            gpointer user_data);
gboolean check_station_code(const gchar *station_code);
/* extern functions */
extern void remove_periodic_event(void);
extern void weather_buttons_init(void);
extern void weather_initialize_dbus(void);
extern time_t next_update(void);
extern void print_list(char *buff, size_t buff_size);
extern void new_config_save(AppletConfig *config);
extern void set_background_color(GtkWidget *widget, GdkColor *bgc);
extern void add_periodic_event(time_t last_update);
extern void remove_periodic_event(void);
extern void popup_window_destroy(void);
/*******************************************************************************/
#endif
