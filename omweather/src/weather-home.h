/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
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

#define APPLET_X_SIZE 200
#define APPLET_Y_SIZE 40
#define FONT_MAIN_SIZE_LARGE 14
#define FONT_MAIN_SIZE_SMALL 9
 GdkColor DEFAULT_COLOR_MARK = {0, 0x0000, 0x0000, 0xc000};



#include "weather-home_common.h"

void hello_world_dialog_show (void);

#include <libosso.h>
//#include <pthread.h>
#include <gconf/gconf-client.h>
#include <osso-ic.h>


#define HTTP_RESPONSE_OK "200"



void weather_buttons_init(void);
weather_com_parser *weather_com_parser_new( const gchar * weather_com );
int parse_weather_com_xml(void);
/* void update_weather(GtkWidget *widget,
		    GdkEvent *event, 
                    gpointer user_data);
*/		    
void update_weather();
void weather_window_preference (GtkWidget *widget,GdkEvent *event,gpointer user_data);

void weather_com_destroy_parser( weather_com_parser *weather_com );
void update_weather(void);
void weather_frame_update(void);
void timer(void);


gchar  path_large_icon[]="/usr/share/weather/icons/Crystal/large_icon/";

/* Window Popup */
GtkWidget *weather_window_popup;  

/* Default values */
GdkColor DEFAULT_FONT_COLOR = {0, 0x0d00, 0x2a00, 0xc000};

/* CONFIGURATION INFORMATION. */
gchar *_weather_dir_name = NULL;
gchar *_weather_country_name = NULL;
gchar *_weather_state_name = NULL;
gchar *_weather_station_name = NULL;
gchar *_weather_station_id = NULL;
gchar *_weather_icon_size = NULL;
int    _weather_periodic_update = 0;
GdkColor _weather_font_color = {0,0,0,0};

/* List future event */
GSList *event_time_list = NULL;

/* List times between two updates */
GSList *time_update_list = NULL;


GtkWidget *box;
GdkPixbuf *icon;
GtkWidget *icon_image;
GtkWidget *buttons [Max_count_web_button];
GtkWidget *labels [Max_count_web_button];
GtkWidget *boxs [Max_count_web_button];
GtkWidget *boxs_offset [Max_count_web_button];
GtkWidget *update_window;     
gint flag_update=0;
