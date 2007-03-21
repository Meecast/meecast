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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#ifndef _weather_home_common_h
#define _weather_home_common_h 1

#include <gtk/gtk.h>
#include <gconf/gconf-client.h>
#define _XOPEN_SOURCE /* glibc2 needs this */
#include <time.h>
#include <libgnomevfs/gnome-vfs.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include <hildon-widgets/hildon-banner.h>
#include <sys/types.h>
#include <libosso.h>
#include <wchar.h>
#include <stdio.h>
/* localization headers and defines */
#include <libintl.h>
#include <locale.h>
#define _(String) dgettext (GETTEXT_PACKAGE,String)

#define Max_count_weather_day	10
#define AUTOUPDATE		1
#define DAYTIMEEVENT		2
#define DAY_DOWNLOAD		10
#define OFFSET_CURRENT_WEATHER	4 /* 4 hours */
#define STATIONS_FILE		"/usr/share/omweather/stations.txt"
#define COUNTRYS_FILE		"/usr/share/omweather/iso3166-countrycodes.txt"
#define ICONS_PATH		"/usr/share/omweather/icons/"
#define streq(a, b)   (*(a)==*(b)&&!strcmp(a,b))
enum { ONE_ROW, ONE_COLUMN, TWO_ROWS, TWO_COLUMNS };
enum { METERS, KILOMETERS, INTERNATIONAL_MILES, IMPERIAL_MILES, SEA_MILES };
enum { METERS_S, KILOMETERS_S, MILES_S, METERS_H, KILOMETERS_H, MILES_H };
enum { CELSIUS, FAHRENHEIT };
enum { LARGE, MEDIUM, SMALL };

typedef struct
{
  int icon;
  gchar title[30];      /* Title */
  gchar hmid[30];        /* Humidity */
  int wind_speed;       /* Wind Speed */
  int wind_gust;        /* Wind Gust */
  float vis;              /* Visibilty */
  gchar wind_title[30]; /* Wind Title */
  gchar temp[10];        /* Temperature */
  time_t begin_time;    /* Begin time party of the day */  
  float	pressure;
  gchar	pressure_str[30];
}part_of_day;


typedef struct
{
  part_of_day day;      /* Or current weather */
  part_of_day night;
  gchar date[20];	/* Date */     
  time_t date_time;	/* Date of the year or current time*/  
  gchar dayshname[30];	/* Short name of day */
  gchar dayfuname[30];	/* Full name of day */
/*    wchar_t	dayshname[30];
    wchar_t	dayfuname[30];*/
  gchar hi_temp[10];   /* High temperature of day or real current temperature for current day */
  gchar low_temp[10];  /* Low temperature  of day or feels like temperature for current day */
  gchar location[50]; /* Location */
}weather_day;

typedef struct
{
	gint    error;
/*	weather_channel *channel_list; */
	xmlDoc	    *doc;
	xmlNode     *weather_com_root;
} weather_com_parser;

struct event_time
{
  time_t time;          /* Time event */
  short int type_event; /* Type of event: Automatic update event AUTOUPDATE, 
			   time of day event DAYTIMEEVENT */ 
};


struct time_update{ 
	gint  between_time;        /* Time in minutes  betwen two update weather */
	gchar *name_between_time;  /* Human string it time */
};


struct weather_station{
	gchar *id_station;
	gchar *name_station;
        GtkWidget *box;
        GtkWidget *button;
	GtkWidget *label_box;
};

typedef struct OMWeatherApplet{
    osso_context_t	*osso;
    GHashTable		*hash;
    GtkWidget		*top_widget;
    GtkWidget		*main_window;
    GtkWidget		*popup_window;
    gchar		*_weather_dir_name;
    gchar		*icon_set;
    gchar		*current_station_name;
    gboolean		show_update_window;
    gchar 		*iap_http_proxy_host;
    gint 		iap_http_proxy_port;

}OMWeatherApp;

weather_day weather_days[Max_count_weather_day];
weather_day weather_current_day;

/* Stations List of view in plugin */
GSList *stations_view_list; 

int get_weather_html( gboolean check_connect );
void free_list_time_event (void);
void add_periodic_event(void);
void time_event_add(time_t time_value, short int type_event);
void config_save();
void config_save_current_station();
void config_init();
void update_weather(void);
void weather_frame_update(gboolean check);
void hack_home_plugin_osso_for_nokia800(void);

extern OMWeatherApp	*app;

#endif
