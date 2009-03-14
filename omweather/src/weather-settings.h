/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
#else
    #include <hildon-widgets/hildon-controlbar.h>
#endif
/*******************************************************************************/
struct lists_struct{
    GtkWidget		*countries;
    GtkListStore	*countries_list;
    GtkWidget		*states;
    GtkListStore	*regions_list;
    GtkWidget		*stations;
    GtkListStore	*stations_list;
    GtkListStore	*sources_list;
    sqlite3		*database;
};
/*******************************************************************************/
extern gboolean		not_event;
extern gchar		path_large_icon[_POSIX_PATH_MAX];
extern WeatherSource	weather_sources[];
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
void add_station_to_user_list(gchar *weather_station_id,gchar *weather_station_name,
				gboolean is_gps, gchar *source);
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
extern void redraw_home_window(gboolean first_start);
extern void remove_periodic_event(void);
extern void weather_initialize_dbus(void);
extern time_t next_update(void);
extern void print_list(char *buff, size_t buff_size);
extern void config_save(AppletConfig *config);
extern void set_background_color(GtkWidget *widget, GdkColor *bgc);
extern void add_periodic_event(time_t last_update);
extern void remove_periodic_event(void);
extern void popup_window_destroy(void);
extern void add_gps_event(guint interval);
extern gboolean change_station_select(GtkWidget *widget, gpointer user_data);
extern gboolean switch_timer_handler(gpointer data);
extern void help_activated_handler(GtkWidget *window, gchar *help_id);
extern void update_weather(gboolean show_update_window);
extern GtkWidget* lookup_widget(GtkWidget *widget, const gchar *widget_name);
extern void weather_window_popup(GtkWidget *widget, GdkEvent *event, gpointer user_data);
extern GtkWidget* create_tree_view(GtkListStore* list);
#if defined(OS2008) || defined(DEBUGTEMP) || defined(OS2009)
extern GtkWidget* create_sensor_page(GtkWidget *config_window);
extern void read_sensor(gint need_redraw);
extern guint get_time_from_index(guint index);
#endif
extern GtkWidget* create_button_with_image(const char *path, const char *image_name,
			int image_size, gboolean with_border, gboolean toggled);
extern GtkWidget* create_scrolled_window_with_text(const char* text,
					    GtkJustification justification);
extern guint get_index_from_time(guint time);
/*******************************************************************************/
#endif
