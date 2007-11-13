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
/*******************************************************************************/
#include "weather-data.h"
#include <stdio.h>
/*******************************************************************************/
WeatherComStation wcs = { NULL };
/*******************************************************************************/
Item* create_item(const char *name, const char *value){
    Item	*itm;

    if(!name)
	return NULL;
    
    itm = g_new0(Item, 1);
    itm->name = g_string_new(name);
    value && (itm->value = g_string_new(value));
    return itm;
}
/*******************************************************************************/
void destroy_item(Item **item){
    if(!(*item))
	return;
    if((*item)->name)
	g_string_free((*item)->name, TRUE);
    if((*item)->value)
	g_string_free((*item)->value, TRUE);
    g_free(*item);
    *item = NULL;
}
/*******************************************************************************/
GSList*	add_item2object(GSList **object, Item *item){
    if(item)
	*object = g_slist_append(*object, item);
    return (*object);
}
/*******************************************************************************/
char* item_value(GSList *object, const char *name){
    GString	*tmp = NULL;
    Item	*itm = NULL;
    char	*result = "";

    if(object){
        tmp = g_string_new(name);
	while(object){
	    itm = (Item*)object->data;
	    if(g_string_equal(itm->name, tmp)){
		result = itm->value->str;
		break;
	    }
	    object = g_slist_next(object);
	}
	g_string_free(tmp, TRUE);
    }
    return result;
}
/*******************************************************************************/
void destroy_object(GSList **object){
    Item	*itm = NULL;
    GSList	*tmp = *object;

    while(tmp){
	itm = (Item*)tmp->data;
	if(itm)
	    destroy_item(&itm);
	tmp = g_slist_next(tmp);
    }
    g_slist_free(*object);
    *object = NULL;
}
/*******************************************************************************/
time_t last_update_time(GSList *object){
    time_t	last_update = 0;
    struct tm	tm = {0};

    if(!object)
	return 0;
    
    strptime(item_value(object, "last_update"), "%D %I:%M", &tm);
    last_update = mktime(&tm);
    /* Add 12 hours if  date have PM field */
    if(strstr(item_value(object, "last_update"), "PM"))
	last_update += 12 * 3600;
    return last_update;
}
/*******************************************************************************/
#ifndef RELEASE
void display_all_object_items(GSList *object){
    Item	*itm = NULL;
    fprintf(stderr, "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    while(object){
	itm = (Item*)object->data;
	fprintf(stderr, "\n>%s: %s<", itm->name->str, itm->value->str);
	object = g_slist_next(object);
    }
    fprintf(stderr, "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}
#endif
/*******************************************************************************/
