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
#ifndef _weather_home_h
#define _weather_home_h 1
/*******************************************************************************/
#include "weather-common.h"
#include <locale.h>
#include <libintl.h>
/*******************************************************************************/
gboolean change_station_prev(GtkWidget *widget, GdkEvent *event,
                                gpointer user_data);
gboolean change_station_next(GtkWidget *widget, GdkEvent *event,
                                gpointer user_data);
gboolean change_station_select(GtkWidget *widget, gpointer user_data);
WDB* create_weather_day_button(const char *text, const char *icon,
                const gint type_of_button, gboolean transparency,
                gboolean draw_day_label, GdkColor *color, gint wind_direction, gint  wind_gust);
void fill_weather_day_button_expand(WDB *new_day_button, const char *text, const char *icon,
                const gint icon_size, gboolean transparency, gboolean draw_day_label );
void delete_weather_day_button(WDB **day);
void draw_home_window(gint count_day);
void update_weather(gboolean show_update_window);
void redraw_home_window(gboolean first_start);
void create_panel(GtkWidget* panel, gint layout, gboolean transparency,
                                gchar* st_name);
void weather_window_popup_show(GtkWidget *widget,GdkEvent *event,
                                gpointer user_data);
gboolean download_html(gpointer data);
void free_memory(void);
void menu_init(void);
gboolean switch_timer_handler(gpointer data);
gboolean expose_main_window(GtkWidget *widget, GdkEventExpose *event);
gboolean remitted_update(void);
GtkWidget* create_current_weather_simple_widget(GHashTable *current);
GtkListStore* create_user_stations_list(void);
GtkWidget* create_forecast_weather_simple_widget(GHashTable *day);
int calculate_offset_of_day(int count_day);
void add_change_day_part_event(GHashTable *day, guint year, guint month);
time_t get_day_part_begin_time(GHashTable *day, guint year, const gchar *day_part);
void create_wind_parameters(GHashTable *day, gchar *buffer, gboolean is_day, gint *direction, gfloat *gust);
void view_popup_menu (GtkWidget *treeview, GdkEventButton *event, gpointer userdata);
void create_current_temperature_text(GHashTable *day, gchar *buffer, gboolean valid,
                                const gchar *day_name);
void create_day_temperature_text(GHashTable *day, gchar *buffer, gboolean valid,
                                gboolean for_combination_mode, gint button_number);
void free_main_hash_table(GHashTable *table);
#if defined OS2008 || defined OS2009 || defined APPLICATION || defined NONMAEMO
void omweather_destroy(GtkObject *widget);
#endif

gboolean expose_main_window1(GtkWidget *widget, GdkEventExpose *event);
#ifdef OS2008
GtkWidget*
settings_menu(HildonDesktopHomeItem *home_item, GtkWindow *parent);
#endif
/*******************************************************************************/
#if defined (OS2009) || defined(NONMAEMO) || defined (APPLICATION)
gboolean omweather_init_OS2009(GtkWidget *applet);
void omweather_destroy(GtkObject *widget);
#ifdef CLUTTER
gboolean top_widget_expose(GtkWidget *widget, GdkEventExpose *event);
#endif
#endif
#ifdef CLUTTER
void free_clutter_objects_list(GSList **clutter_objects);
#endif
#if defined(OS2009)
void init_portrait(GtkWidget *main_widget);
#endif
/*******************************************************************************/
typedef struct _HomeAppletHandler HomeAppletHandler;
/*******************************************************************************/
struct _HomeAppletHandler {
    GObject parent;
    gchar *desktoppath;
    gchar *libraryfile;
    GtkEventBox *eventbox;
    gint x;
    gint y;
    gint width;
    gint height;
    gint minwidth;
    gint minheight;
    gint resizable_width;
    gint resizable_height;
};
/*******************************************************************************/
GtkWidget* hildon_home_applet_lib_settings(void *applet_data, GtkWindow *parent);
/*******************************************************************************/
#ifdef OS2009
static void omweather_plugin_class_finalize(OmweatherPluginClass *klass);
static void omweather_plugin_class_init(OmweatherPluginClass *klass);
static void omweather_plugin_visible_notify(GObject *object, GParamSpec *spec,
                                          OmweatherPlugin *applet);
static void omweather_plugin_init (OmweatherPlugin *applet);
#endif 
/*******************************************************************************/
#endif
