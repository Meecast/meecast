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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
/*******************************************************************************/
#ifndef _weather_home_common_h
#define _weather_home_common_h 1
/*******************************************************************************/
#include <gtk/gtk.h>
#include <gconf/gconf-client.h>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE /* glibc2 needs this */
#endif
#include <time.h>
#include <libgnomevfs/gnome-vfs.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <string.h>
#include <hildon-widgets/hildon-banner.h>
#include <sys/types.h>
#include <libosso.h>
#include <wchar.h>
#include <stdio.h>
#include <limits.h>
/* localization headers and defines */
#include <libintl.h>
#include <locale.h>
#define _(String) dgettext (GETTEXT_PACKAGE, String)

#define Max_count_weather_day		10
#define MAX_SETTINGS_PAGE_NUMBER	3
#define COUNTRYSFILE		"/usr/share/omweather/countrys.list"
#define REGIONSFILE		"/usr/share/omweather/regions.list"
#define LOCATIONSFILE		"/usr/share/omweather/locations.list"
#define CLOCK_FILE		"/usr/share/omweather/city_in_clock.txt"
#define ICONS_PATH		"/usr/share/omweather/icons/"
#define GCONF_KEY_CURRENT_CONNECTIVITY	"/system/osso/connectivity/IAP/current"
#define streq(a, b)   (*(a)==*(b)&&!strcmp(a,b))
/*******************************************************************************/
enum { AUTOUPDATE, DAYTIMEEVENT, DBUSINITEVENT, UPDATE_AFTER_CONNECTED};
enum { ONE_ROW, ONE_COLUMN, TWO_ROWS, TWO_COLUMNS };
enum { METERS, KILOMETERS, MILES, SEA_MILES };
enum {	METERS_S,
/*	KILOMETERS_S,
	MILES_S,
	METERS_H,
*/	KILOMETERS_H,
	MILES_H };
enum { CELSIUS, FAHRENHEIT };
enum { LARGE, MEDIUM, SMALL };
/*******************************************************************************/
typedef struct{
    int icon;
    gchar	title[80];		/* Title */
    gchar	hmid[80];       	/* Humidity */
    gchar	wind_speed[80];       	/* Wind Speed */
    gchar	wind_gust[80];        	/* Wind Gust */
    gchar	vis[80];              	/* Visibilty */
    gchar	wind_title[80];		/* Wind Title */
    gchar	temp[20];		/* Temperature */
    time_t	begin_time;		/* Begin time party of the day */  
    float	pressure;		/* Pressure value */
    gchar	pressure_str[80];	/* Pressure direction */
}part_of_day;
/*******************************************************************************/
typedef struct{
    part_of_day	day;		/* Or current weather */
    part_of_day	night;
    gchar	date[40];	/* Date */     
    time_t	date_time;	/* Date of the year or current time*/  
    time_t      zone;           /* time zone */
    gchar	sunrise[60];	/* sunrise */
    gchar	sunset[60];	/* sunset */
    gchar	dayshname[60];	/* Short name of day */
    gchar	dayfuname[60];	/* Full name of day */
    gchar	hi_temp[20];	/* High temperature of day or real current temperature for current day */
    gchar	low_temp[20];	/* Low temperature  of day or feels like temperature for current day */
    gchar	location[50];	/* Location */
}weather_day;
/*******************************************************************************/
typedef struct{
    gint	error;
    xmlDoc	*doc;
    xmlNode	*weather_com_root;
}weather_com_parser;
/*******************************************************************************/
struct event_time{
    time_t	time;          /* Time event */
    short	type_event; /* Type of event: Automatic update event AUTOUPDATE, 
			   time of day event DAYTIMEEVENT */ 
};
/*******************************************************************************/
struct time_update{ 
    gint	between_time;        /* Time in minutes  betwen two update weather */
    gchar	*name_between_time;  /* Human string it time */
};
/*******************************************************************************/
struct weather_station{
    gchar	*id_station;
    gchar	*name_station;
};
/*******************************************************************************/
struct station_and_weather_code {
    gchar	*station_name;
    gchar	*station_code;
};
/*******************************************************************************/
typedef struct weather_day_button_with_image{
    GtkWidget	*button;                                                                                               
    GtkWidget	*label;                                                                                                
    GtkWidget	*box;
    GdkPixbuf   *icon_buffer;                                                                                                             
    GtkWidget   *icon_image; 
}WDB;
/*******************************************************************************/
typedef struct applet_config{
    gchar	*cache_dir_name;
    gchar	*icon_set;
    gchar	*current_country;
    gchar	*current_station_name;
    gchar	*current_station_id;
    gchar 	*iap_http_proxy_host;
    gint 	iap_http_proxy_port;
    gint	update_interval;
    gint	icons_size;
    gint	icons_layout;
    gint	current_settings_page;
    gint	days_to_show;
    gint	previos_days_to_show;
    gint	distance_units;
    gint	wind_units;
    gint	temperature_units;
    guint	data_valid_interval;
    gboolean	transparency;
    gboolean	separate;
    gboolean	swap_hi_low_temperature;
    gboolean	hide_station_name;
    gboolean	downloading_after_connecting;
    GdkColor	font_color;
    GdkColor	background_color;
}AppletConfig;
/*******************************************************************************/
typedef struct OMWeatherApplet{
    osso_context_t	*osso;
    GHashTable		*hash;
    GtkWidget		*top_widget;
    GtkWidget		*main_window;
    GtkWidget		*popup_window;
    AppletConfig	*config;
    gboolean		show_update_window;
    gboolean		iap_connected;
    gboolean		iap_connecting;    
    long		iap_connecting_timer;  
    guint		timer;
    WDB			*buttons[Max_count_weather_day];
    guint		flag_updating;
    gboolean		dbus_is_initialize;
    weather_day		weather_current_day;
    weather_day		weather_days[Max_count_weather_day];
    GtkListStore	*countrys_list;
    GtkListStore	*regions_list;
    GtkListStore	*stations_list;
    GtkListStore	*time_update_list;
    GtkWidget 		*contextmenu;
}OMWeatherApp;
/*******************************************************************************/
/* Stations List of view in plugin */
GSList *stations_view_list; 

void free_list_time_event (void);
void time_event_add(time_t time_value, short int type_event);
void update_weather(void);
void weather_frame_update(gboolean check);
extern void read_config(void);
extern int new_read_config(AppletConfig*);
/*******************************************************************************/
extern OMWeatherApp	*app;
/*******************************************************************************/
#endif
