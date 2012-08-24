/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen 
 *
 * Copyright (C) 2012 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _main_h
#define _main_h 1
#endif
/*******************************************************************************/
#include <Elementary.h>
#include <Evas.h>
#include "common.h"
/*******************************************************************************/
enum { UNKNOWN, TEMPERATURE_UNITS, WIND_SPEED_UNITS, PRESSURE_UNITS };
struct _Setting_window {
    Evas_Object *top_main_window;
    Evas_Object *menu;
};
struct _list_item_data {
    int index;
    int magic;
    Elm_Object_Item *item;
    Evas_Object *icon;
    char text1[50];
    char text2[50];
    void *app;
};
typedef struct _list_item_data list_item_data;
#define MAX_TEMPERATURE_ITEM_NUM 2
static char *title_temperature[MAX_TEMPERATURE_ITEM_NUM] = {
    "Celcius",
    "Fahrenheit"
};
static char *temperature_in_config[MAX_TEMPERATURE_ITEM_NUM] = {
    "C",
    "F"
};
#define MAX_WIND_SPEED_ITEM_NUM 3
static char *title_wind_speed[MAX_WIND_SPEED_ITEM_NUM] = {
    "m/s",
    "km/h",
    "mi/h"
};
static char *wind_speed_in_config[MAX_WIND_SPEED_ITEM_NUM] = {
    "m/s",
    "km/h",
    "mi/h"
};
#define MAX_PRESSURE_ITEM_NUM 3
static char *title_pressure[MAX_PRESSURE_ITEM_NUM] = {
    "mbar",
    "Pa",
    "mmHg"
};
static char *pressure_in_config[MAX_PRESSURE_ITEM_NUM] = {
    "mbar",
    "Pa",
    "mmHg"
};

void create_setting_window(void *data);
static Elm_Genlist_Item_Class _itc;
static Elm_Genlist_Item_Class _itc_sub;
void create_main_window(void *data);
void create_location_window(void *data);
void create_sources_window(void *data);
void create_countries_window(void *data);
void create_regions_window(void *data);
void create_stations_window(void *data);

