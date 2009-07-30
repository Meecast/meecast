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
#ifndef _weather_settings_h
#define _weather_settings_h 1
/*******************************************************************************/
#include "weather-common.h"
#ifdef OS2009
    #include <hildon/hildon-controlbar.h>
#elif OS2008
    #include <hildon/hildon-controlbar.h>
#elif NONMAEMO
    #include <hildon/hildon-controlbar.h>
#else
    #include <hildon-widgets/hildon-controlbar.h>
#endif
/*******************************************************************************/
extern gboolean		not_event;
/*******************************************************************************/
void weather_window_settings(GtkWidget *widget, gpointer user_data);
void station_list_view_select_handler(GtkTreeView *tree_view,
			    gpointer user_data);
gboolean process_locations_tab(GtkWidget *vbox);
gboolean process_visuals_tab(GtkWidget *vbox);
gboolean process_display_tab(GtkWidget *vbox);
gboolean process_units_tab(GtkWidget *vbox);
gboolean process_update_tab(GtkWidget *vbox);
#ifdef OS2008
gboolean process_sensor_tab(GtkWidget *vbox);
#endif
GtkWidget *create_layouts_line(GtkWidget *window, gint icon_size, gint mode);
GtkWidget *create_iconsets_line(GtkWidget *window, gint icon_size, gint mode);
gboolean process_alert_tab(GtkWidget *vbox);
void changed_country_handler(GtkWidget *widget, gpointer user_data);
void changed_state_handler(GtkWidget *widget, gpointer user_data);
void changed_stations_handler(GtkWidget *widget, gpointer user_data);
void changed_sources_handler(GtkWidget *widget, gpointer user_data);
void weather_window_add_custom_station(void);
int get_active_item_index(GtkTreeModel *list, int time, const gchar *text,
					    gboolean use_index_as_result);
void update_iterval_changed_handler(GtkComboBox *widget, gpointer user_data);
void transparency_button_toggled_handler(GtkToggleButton *togglebutton,
                                            gpointer user_data);
gboolean check_station_code(gchar *source, const gchar *station_code);
void up_key_handler(GtkButton *button, gpointer list);
void down_key_handler(GtkButton *button, gpointer list);
void new_station_handler(GtkButton *button, gpointer user_data);
void delete_station_handler(GtkButton *button, gpointer user_data);
GtkTreeIter add_station_to_user_list(gchar *weather_station_id,gchar *weather_station_name,
				gboolean is_gps, gchar *source, gint position);
void delete_station_from_user_list(gchar *station_selected, gpointer user_data);
void highlight_current_station(GtkTreeView *tree_view);
int lookup_and_select_station(gchar *db_path, gchar *station_name, Station *result);
gboolean switch_cb(GtkNotebook * nb, gpointer nb_page, gint page, gpointer data);
/*******************************************************************************/
void apply_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data);
void rename_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data);
void help_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data);
void close_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data);
void back_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data);
void add_button_handler(GtkWidget *button, gpointer user_data);
void chk_download_button_toggled_handler(GtkRadioButton *button,
							    gpointer user_data);
void entry_changed_handler(GtkWidget *entry, gpointer user_data);
void check_buttons_changed_handler(GtkToggleButton *button, gpointer user_data);
void color_buttons_changed_handler(GtkColorButton *button, gpointer user_data);
void combo_boxs_changed_handler(GtkComboBox *combobox, gpointer user_data);
void control_bars_changed_handler(HildonControlbar *control, gpointer user_data);
GtkWidget* create_locations_tab(GtkWidget *window);
GtkWidget* create_visuals_tab(GtkWidget *window);
GtkWidget* create_display_tab(GtkWidget *window);
GtkWidget* create_units_tab(GtkWidget *window);
GtkWidget* create_update_tab(GtkWidget *window);
GtkWidget* create_test_tab(GtkWidget *window);
void font_changed_handler(GtkFontButton *widget, gpointer user_data);
/*******************************************************************************/
#endif
