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
 *
*/
#ifndef _weather_data_h
#define _weather_data_h 1
/*******************************************************************************/
#include <glib.h>
#include "weather-common.h"
/*******************************************************************************/
typedef struct{
    GString	*name;
    GString	*value;
}Item;
/*******************************************************************************/
typedef struct{
    GSList	*current_data;
    GSList	*day_data[Max_count_weather_day];
    gboolean	current_data_is_invalid;
}WeatherComStation;
/*******************************************************************************/
extern	WeatherComStation wcs;
/*******************************************************************************/
Item* create_item(const char *name, const char *value);
void destroy_item(Item **item);
GSList*	add_item2object(GSList **object, Item *item);
char* item_value(GSList *object, const char *name);
void destroy_object(GSList **object);
time_t last_update_time(GSList *object);
#ifndef RELEASE
void display_all_object_items(GSList *object);
#endif
/*******************************************************************************/
#endif
