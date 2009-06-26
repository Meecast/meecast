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
                const gint icon_size, gboolean transparency,
                gboolean draw_day_label, GdkColor *color);
void fill_weather_day_button_expand(WDB *new_day_button, const char *text, const char *icon,
                const gint icon_size, gboolean transparency,
                gboolean draw_day_label, GdkColor *color)
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
GtkWidget* create_current_weather_simple_widget(GSList *current);
GtkListStore* create_user_stations_list(void);
GtkWidget* create_forecast_weather_simple_widget(GSList *day);
int calculate_offset_of_day(int count_day);
void add_change_day_part_event(GSList *day, guint year, guint month);
time_t get_day_part_begin_time(GSList *day, guint year, const gchar *day_part);
void add_wind_text(GSList *day, gchar *buffer, gboolean is_day);
void view_popup_menu (GtkWidget *treeview, GdkEventButton *event, gpointer userdata);
void create_current_temperature_text(GSList *day, gchar *buffer, gboolean valid,
                                const gchar *day_name);
void create_day_temperature_text(GSList *day, gchar *buffer, gboolean valid,
                                gboolean for_combination_mode);
#if defined OS2008 || defined OS2009 || defined APPLICATION || defined NONMAEMO
void omweather_destroy(GtkObject *widget);
#endif

gboolean expose_main_window1(GtkWidget *widget, GdkEventExpose *event);
#ifdef OS2008
GtkWidget*
settings_menu(HildonDesktopHomeItem *home_item, GtkWindow *parent);
#endif
/*******************************************************************************/
extern void swap_temperature(int *hi, int *low);
extern float c2f(float temp);
extern void set_font(GtkWidget *widget, const gchar *description, const gint size);
extern void set_font_color(GtkWidget *widget, guint16 red, guint16 green, guint16 blue);
extern void set_background_color(GtkWidget *widget, GdkColor *bgc);
extern void timer(int interval);
extern void create_timer_with_interval(guint interval);
extern void weather_initialize_dbus(void);
extern void add_periodic_event(time_t last_update);
extern time_t calculate_diff_time(int timezone);
extern void remove_daytime_event(void);
extern void clean_download(void);
extern int read_config(AppletConfig*);
extern void weather_window_settings(GtkWidget *widget, gpointer user_data);
extern void config_save(AppletConfig*);
extern GtkListStore* create_items_list(const char *path, const char *filename,
				long start, long end, long *items_number);
extern GtkListStore* create_time_update_list(void);
extern gboolean show_popup_window_handler(GtkWidget *widget, GdkEvent *event,
				    gpointer user_data);
extern gint parse_weather_file_data(const gchar *station_id, gchar *station_source,
					WeatherStationData *wsd,
						gboolean selected_detail_weather);
extern gint parse_weather_com_xml(const gchar *station_id, weather_com_parser *parser,
								WeatherStationData *wsd);
extern gint parse_weather_com_xml_hour(const gchar *station_id, weather_com_parser *parser,
								WeatherStationData *wsd);
extern gint parse_rp5_ru_xml(const gchar *station_id, weather_com_parser *parser, WeatherStationData *wsd);
void free_hashtable_with_source(GHashTable* hashtable);
extern time_t last_update_time(GSList *object);
extern float convert_wind_units(int to, float value);
#if defined (ENABLE_GPS)
extern void initial_gps_control(void);
extern void initial_gps_connect(void);
extern void deinitial_gps_connect(void);
extern void deinitial_gps_control(void);
extern gboolean expose_parent(GtkWidget *widget, GdkEventExpose *event);
#endif
extern void weather_window_popup(GtkWidget *widget, GdkEvent *event, gpointer user_data);
extern float mb2inch(float pressure);
#if defined(OS2008) || defined(DEBUGTEMP) || defined(OS2009)
extern void read_sensor(gint need_redraw);
extern WDB* create_sensor_icon_widget(const int icon_size, gboolean transparency,
				gboolean draw_day_label, GdkColor *color);
#endif
#if defined (OS2009) || defined(NONMAEMO) || defined (APPLICATION)
gboolean omweather_init_OS2009(GtkWidget *applet);
void omweather_destroy(GtkObject *widget);
#ifdef CLUTTER
gboolean top_widget_expose(GtkWidget *widget, GdkEventExpose *event);
#endif
extern GtkWidget* create_time_updates_widget(GSList *current, gboolean change_color);
#endif
extern GtkWidget *create_toolbar_box(gpointer exit_function, GtkWidget *window, gboolean fullscreen_button);
extern void free_list_time_event(void);
extern void event_add(time_t time_value, short int type_event);
extern void destroy_popup_window(void);
extern float mb2mm(float pressure);
extern GtkWidget *create_icon_widget(GdkPixbuf *icon_buffer, const char *icon_path, int icon_size, GSList **objects_list);
#ifdef USE_DBUS
extern void weather_deinitialize_dbus(void);
#endif
#ifdef CLUTTER
void free_clutter_objects_list(GSList **clutter_objects);
#endif
#if defined(OS2009)
void init_portrait(main_widget);
#endif
extern gboolean show_animation(GSList *clutter_objects);
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
static void omweather_plugin_class_finalize (OmweatherPluginClass *klass);
extern void omweather_plugin_realize (GtkWidget *widget);
extern gboolean omweather_plugin_expose_event (GtkWidget      *widget,
                                        GdkEventExpose *event);
static void omweather_plugin_class_init (OmweatherPluginClass *klass);
static void omweather_plugin_visible_notify (GObject                *object,
                                          GParamSpec             *spec,
                                          OmweatherPlugin *applet);
static void omweather_plugin_init (OmweatherPlugin *applet);
#endif 
/*******************************************************************************/
#endif
