/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
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
#include "weather-common.h"
#include "weather-data.h"
#include <stdio.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
gboolean
is_current_weather_valid(void){
    time_t      current_time = 0,
                diff_time = 0,
                current_data_last_update = 0;
    GHashTable  *location = NULL,
                *current = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if(!app->station_data)
        return FALSE;
    location = g_hash_table_lookup(app->station_data, "location");
    current = g_hash_table_lookup(app->station_data, "current");
    if(location && current){
        current_time = time(NULL); /* get current day */
        /* correct time for current location */
        if (g_hash_table_lookup(location, "station_time_zone"))
            diff_time = calculate_diff_time(atol(g_hash_table_lookup(location, "station_time_zone")));
#ifndef RELEASE
        fprintf(stderr, "\n>>>>>>>Diff time=%li<<<<<<\n", diff_time);
#endif
        current_time += diff_time;
        current_data_last_update = last_update_time_new(current);
        /* Check a valid time for current weather */
        if( (current_data_last_update >
                ( current_time - app->config->data_valid_interval)) &&
                (current_data_last_update < ( current_time + app->config->data_valid_interval)))
        return TRUE;
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

    return FALSE;
}
/*******************************************************************************/
Item *create_item(const char *name, const char *value) {
    Item *itm = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!name)
        return NULL;

    itm = g_new0(Item, 1);
    itm->name = g_string_new(name);
    if (value)
        itm->value = g_string_new(value);
    return itm;
}
/*******************************************************************************/
void destroy_item(Item ** item) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!(*item))
        return;
    if ((*item)->name) {
        g_string_free((*item)->name, TRUE);
        (*item)->name = NULL;
    }
    if ((*item)->value) {
        g_string_free((*item)->value, TRUE);
        (*item)->value = NULL;
    }
    g_free(*item);
    *item = NULL;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
GSList *add_item2object(GSList ** object, void *item) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (item)
        *object = g_slist_append(*object, item);
    return (*object);
}
/*******************************************************************************/
char *item_value(GSList * object, const char *name) {
    GString *tmp = NULL;
    Item *itm = NULL;
    char *result = "";
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (object) {
        tmp = g_string_new(name);
        while (object) {
            itm = (Item *) object->data;
            if (g_string_equal(itm->name, tmp)) {
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
time_t calculate_diff_time(int timezone) {

    time_t current_time, diff_time = 0, utc_time;
    struct tm *gmt;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
    current_time = time(NULL);  /* get current day */
    /* set Daylight */
    gmt = gmtime(&current_time);
    gmt->tm_isdst = 1;
    /* correct time for current location */
    utc_time = mktime(gmt);

    diff_time = utc_time - current_time + 60 * 60 * timezone;

    return diff_time;
}
/*******************************************************************************/
void destroy_object(GSList ** object) {
    Item *itm = NULL;
    GSList *tmp = *object;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    while (tmp) {
        itm = (Item *) tmp->data;
        if (itm)
            destroy_item(&itm);
        itm = NULL;
        tmp = g_slist_next(tmp);
    }
    g_slist_free(*object);
    *object = NULL;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}

/*******************************************************************************/
time_t last_update_time(GSList * object) {
    time_t last_update = 0;
    struct tm tm = { 0 };
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!object)
        return 0;

    strptime(item_value(object, "last_update"), "%D %I:%M", &tm);
    tm.tm_isdst = 1;
    last_update = mktime(&tm);

    /* Add 12 hours if  date have PM field */
    if (strstr(item_value(object, "last_update"), "PM"))
        last_update += 12 * 3600;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return last_update;
}
/*******************************************************************************/
time_t last_update_time_new(GHashTable *object) {
    time_t last_update = 0;
    struct tm tm = { 0 };
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!object || !g_hash_table_lookup(object, "last_update"))
        return 0;
    strptime(g_hash_table_lookup(object, "last_update"), "%D %I:%M", &tm);
    tm.tm_isdst = 1;
    last_update = mktime(&tm);

    /* Add 12 hours if  date have PM field */
    if(strstr(g_hash_table_lookup(object, "last_update"), "PM"))
        last_update += 12 * 3600;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return last_update;
}
/*******************************************************************************/
#ifndef RELEASE
void display_all_object_items(GSList * object) {
    Item *itm = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    fprintf(stderr, "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    while (object) {
        itm = (Item *) object->data;
        fprintf(stderr, "\n>%s: %s<", itm->name->str, itm->value->str);
        object = g_slist_next(object);
    }
    fprintf(stderr, "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}
#endif
/*******************************************************************************/
