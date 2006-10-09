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


typedef struct
{
  int icon;
  gchar title[26]; /* Title */
  gchar hmid[5]; /* Humidity */
  int wind_speed; /* Wind Speed */
  gchar wind_title[11]; /* Wind Title */
}part_of_day;


typedef struct
{
  part_of_day day;
  part_of_day night;
  gchar date[10];     /* Date */     
//  struct tm date_tm; 	      /* Date of the year*/  
  time_t date_time; 	      /* Date of the year*/  
  gchar dayshname[3]; /* Short name of day */
  gchar dayfuname[20];/* Full name of day */
  gchar hi_temp[4];   /* High temperature of day */
  gchar low_temp[4];  /* Low temperature  of day */
  gchar location[50]; /* Location */
  
}weather_day;

typedef struct
{
	gint    error;
//	weather_channel *channel_list;
	xmlDoc	    *doc;
	xmlNode     *weather_com_root;
} weather_com_parser;


weather_day weather_days[Max_count_web_button];



