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
 *
*/
/*******************************************************************************/
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
Item* create_item(const char *name, const char *value);
void destroy_item(Item **item);
GSList*	add_item2object(GSList **object, void *item);
char* item_value(GSList *object, const char *name);
void destroy_object(GSList **object);
time_t last_update_time(GSList *object);
time_t last_update_time_new(GHashTable *object);
time_t calculate_diff_time(int timezone);
#ifndef RELEASE
void display_all_object_items(GSList *object);
#endif
/*******************************************************************************/
#endif
