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
#include "weather-home_common.h"
/*******************************************************************************/
#define OMWEATHER_RESPONSE_ABOUT 5000
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
int create_icon_set_list(GtkWidget *store); /* scan /usr/share/omweather/icons */
					     /* and add names to the list */
void create_about_dialog(void);	/* create help dialog */
GtkWidget* create_scrolled_window_with_text(const char* text,
					    GtkJustification justification);
int get_active_item_index(GtkTreeModel *list, int time, const gchar *text,
					    gboolean use_index_as_result);
void update_iterval_changed_handler(GtkComboBox *widget, gpointer user_data);
void transparency_button_toggled_handler(GtkToggleButton *togglebutton,
                                            gpointer user_data);
gboolean check_station_code(const gchar *station_code);
void up_key_handler(GtkButton *button, gpointer list);
void down_key_handler(GtkButton *button, gpointer list);
void add_station_to_user_list(gchar *weather_station_id,gchar *weather_station_name, gboolean is_gps);
GtkWidget* create_tree_view(GtkListStore* list);
/*******************************************************************************/
extern void remove_periodic_event(void);
extern void weather_initialize_dbus(void);
extern time_t next_update(void);
extern void print_list(char *buff, size_t buff_size);
extern void new_config_save(AppletConfig *config);
extern void set_background_color(GtkWidget *widget, GdkColor *bgc);
extern void add_periodic_event(time_t last_update);
extern void remove_periodic_event(void);
extern void popup_window_destroy(void);
extern gboolean change_station_select(GtkWidget *widget, gpointer user_data);
extern gboolean switch_timer_handler(gpointer data);
extern void help_activated_handler(GtkWidget *window, gchar *help_id);
/*******************************************************************************/
#endif
