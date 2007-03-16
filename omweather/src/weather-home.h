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

#ifndef _weather_home_h
#define _weather_home_h 1

#include "weather-home_common.h"
#include <libosso.h>
/* #include <pthread.h> */
#include <osso-ic.h>
#include <pthread.h>
#include <locale.h>
#include <libintl.h>

#define APPLET_X_SIZE		200
#define APPLET_Y_SIZE		40
#define FONT_MAIN_SIZE_LARGE	14
#define FONT_MAIN_SIZE_MEDIUM	12
#define FONT_MAIN_SIZE_SMALL	9
#define HTTP_RESPONSE_OK	"200"

void hello_world_dialog_show(void);
void weather_buttons_init(void);
void weather_buttons_fill(gboolean check_error);
void weather_com_destroy_parser( weather_com_parser *weather_com );
void timer(void);
void update_weather(void);
void set_font_size(GtkWidget *widget, char font_size);
void create_panel(GtkWidget* panel, gint layout, gboolean transparency, gchar* st_name, char f_size);
weather_com_parser *weather_com_parser_new( const gchar * weather_com );
int parse_weather_com_xml(void);
int c2f(int temp);
static gboolean update_w(gpointer data);
gboolean weather_window_popup_show(GtkWidget *widget,GdkEvent *event,gpointer user_data);
gboolean download_html(gpointer data);
/* Hack for osso OS2007 */
void hack_home_plugin_osso_for_nokia800(void);

/* Default values */
GdkColor DEFAULT_FONT_COLOR = {0, 0x0d00, 0x2a00, 0xc000};
GdkColor DEFAULT_COLOR_MARK = {0, 0x0000, 0x0000, 0xc000};
/* CONFIGURATION INFORMATION. */
gchar		*_weather_dir_name = NULL;
gchar		*_weather_country_name = NULL;
gchar		*_weather_state_name = NULL;
gchar		*_weather_station_name = NULL;
gchar		*_weather_station_id = NULL;
int		_weather_icon_size = 0;
int		_weather_layout = ONE_ROW;
gboolean	 _enable_transparency = TRUE;
int   		 _weather_periodic_update = 0;
GdkColor	_weather_font_color = {0,0,0,0};
int		_weather_temperature_unit = CELSIUS;
gint		days_to_show	= 5;
gint		distance_units	= METERS;
gint		wind_units	= METERS_S;
gchar		*icon_set	= NULL;

/* List future event */
GSList *event_time_list = NULL;

/* List times between two updates */
GSList *time_update_list = NULL;

/* Window Popup */
GtkWidget *weather_window_popup;  
/*Temp value for downloading */
GString *url,*full_filename;
/* Value use for not call event function */
gboolean not_event = FALSE;
GtkWidget *box,*box_zero;
GtkWidget *main_table;
GdkPixbuf *icon;
GtkWidget *icon_image;
GtkWidget *buttons [Max_count_weather_day];
GtkWidget *labels [Max_count_weather_day];
GtkWidget *boxs [Max_count_weather_day];
int 	   boxs_offset [Max_count_weather_day];
GtkWidget *update_window;     
guint flag_update=0;
gchar  path_large_icon[_POSIX_PATH_MAX];
pthread_t weather_update_thread;
pthread_t window_animation_thread;

typedef struct _HomeAppletHandler HomeAppletHandler;

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

    osso_context_t *osso;
#endif
