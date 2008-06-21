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
#ifndef _weather_home_h
#define _weather_home_h 1
/*******************************************************************************/
#include "weather-common.h"
#include <locale.h>
#include <libintl.h>
/*******************************************************************************/
gboolean change_station_select(GtkWidget *widget, gpointer user_data);
WDB* create_weather_day_button(const char *text, const char *icon,
				const int icon_size, gboolean transparency,
				char font_size, GdkColor *color);
void delete_weather_day_button(WDB **day);
void draw_home_window(gint count_day);
void update_weather(gboolean show_update_window);
void redraw_home_window(gboolean first_start);
void create_panel(GtkWidget* panel, gint layout, gboolean transparency, gchar* st_name,
		    char f_size, gchar *temperature_string, gchar *forecast_string);
void weather_window_popup_show(GtkWidget *widget,GdkEvent *event,gpointer user_data);
gboolean download_html(gpointer data);
void free_memory(void);
void menu_init(void);
gboolean switch_timer_handler(gpointer data);
gboolean expose_main_window(GtkWidget *widget, GdkEventExpose *event);
gboolean expose_parent(GtkWidget *widget, GdkEventExpose *event);
gboolean remitted_update(void);
GtkWidget* create_current_weather_simple_widget(GSList *current, char f_size);
GtkListStore* create_user_stations_list(void);
GtkWidget* create_forecast_weather_simple_widget(char f_size,
						    gchar *temperature_string,
						    gchar *forecast_string);
int calculate_offset_of_day(int count_day);
/*******************************************************************************/
extern void swap_temperature(int *hi, int *low);
extern float c2f(float temp);
extern void set_font_size(GtkWidget *widget, char font_size);
extern void set_font_color(GtkWidget *widget, guint16 red, guint16 green, guint16 blue);
extern void set_background_color(GtkWidget *widget, GdkColor *bgc);
extern void timer(int interval);
extern void create_timer_with_interval(guint interval);
extern void weather_initialize_dbus(void);
extern void add_periodic_event(time_t last_update);
extern time_t calculate_diff_time(int timezone);
extern void remove_daytime_event(void);
extern void clean_download(void);
extern void weather_window_settings(GtkWidget *widget,GdkEvent *event,gpointer user_data);
extern int new_read_config(AppletConfig*);
extern void config_save(AppletConfig*);
extern GtkListStore* create_items_list(const char *path, const char *filename,
				long start, long end, long *items_number);
extern GtkListStore* create_time_update_list(void);
extern gboolean show_popup_window_handler(GtkWidget *widget, GdkEvent *event,
				    gpointer user_data);
extern int parse_weather_com_xml(void);
extern int new_parse_weather_com_xml(void);
extern time_t last_update_time(GSList *object);
extern float convert_wind_units(int to, float value);
extern void initial_gps_connect(void);
extern void weather_window_popup(GtkWidget *widget, GdkEvent *event, gpointer user_data);
extern float mb2inch(float pressure);
#if defined(OS2008) || defined(DEBUGTEMP)
extern void read_sensor(gint need_redraw);
extern WDB* create_sensor_icon_widget(const int icon_size,
					    gboolean transparency,
					    char font_size, GdkColor *color);
#endif
extern void free_list_time_event(void);
extern void time_event_add(time_t time_value, short int type_event);
/*******************************************************************************/
extern gchar	path_large_icon[];
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
#endif
