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

#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include <time.h>
#include <libgnomevfs/gnome-vfs.h>
#define Max_count_web_button 5
#define Max_count_weather_day 10
#define AUTOUPDATE 1
#define DAYTIMEEVENT 2
#define DAY_DOWNLOAD 10
#define OFFSET_CURRENT_WEATHER 4 /* 4 hours */
#define STATIONS_FILE "/usr/share/weather/stations.txt"
#define COUNTRYS_FILE "/usr/share/weather/iso3166-countrycodes.txt"
#define DEFAULT_ICON "/usr/share/weather/icons/Crystal/large_icon/"


typedef struct
{
  int icon;
  gchar title[26]; /* Title */
  gchar hmid[5]; /* Humidity */
  int wind_speed; /* Wind Speed */
  gchar wind_title[11]; /* Wind Title */
  time_t begin_time;    /* Begin time party of the day */  
}part_of_day;


typedef struct
{
  part_of_day day;      /* Or current weather */
  part_of_day night;
  gchar date[10];     /* Date */     
  time_t date_time;    /* Date of the year or current time*/  
  gchar dayshname[3]; /* Short name of day */
  gchar dayfuname[20];/* Full name of day */
  gchar hi_temp[4];   /* High temperature of day or real current temperature for current day */
  gchar low_temp[4];  /* Low temperature  of day or feels like temperature for current day */
  gchar location[50]; /* Location */
  
}weather_day;

typedef struct
{
	gint    error;
//	weather_channel *channel_list;
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
	gchar *name_station;
	gchar *id_station;
        GtkWidget *box;
	GtkWidget *label_box;
};

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
void weather_frame_update(void);


