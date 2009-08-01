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
*/
/*******************************************************************************/
#ifndef _weather_presets_h
#define _weather_presets_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
#define PRESET_BIG_IMAGE_SIZE 128
#define PRESET_NORMAL_IMAGE_SIZE 64
#define PRESET_NORMAL_WIND_IMAGE_SIZE 42
#define PRESET_NOW_BACKGROUND                   "OMW_widget_background_single_now.png"
#define PRESET_NOW_BACKGROUND_PLUS_TWO          "OMW_widget_background_2block_now.png"
#define PRESET_NOW_BACKGROUND_PLUS_THREE_V      "OMW_widget_background_3block_now.png"
#define PRESET_NOW_BACKGROUND_HORIZONTAL        "OMW_widget_background_horizontal_now.png"
#define PRESET_NOW_BACKGROUND_TOWN              "OMW_widget_button_single_town.png"
#define PRESET_NOW_BACKGROUND_TOWN_PLUS_TWO     "OMW_widget_button_2block_town.png"
#define PRESET_NOW_BACKGROUND_TOWN_PLUS_THREE_V "OMW_widget_button_3block_town.png"
#define PRESET_MIDDLE_HORIZONTAL_BACKGROUND     "OMW_widget_button_forecast_horizontal_middle.png"
#define PRESET_RIGHT_HORIZONTAL_BACKGROUND      "OMW_widget_button_forecast_horizontal_right_end.png"
#define PRESET_NOW_BACKGROUND_TOWN_HORIZONTAL   "OMW_widget_button_horizontal_town.png"
#define PRESET_LEFT_VERTICAL_BACKGROUND         "OMW_widget_button_forecast_block_left_end.png"
#define PRESET_RIGHT_VERTICAL_BACKGROUND        "OMW_widget_button_forecast_block_right_end.png"
#define PRESET_WIND_SOUTH           "wind_south.png"
#define PRESET_WIND_SOUTH_EAST      "wind_south_east.png"
#define PRESET_WIND_SOUTH_WEST      "wind_south_west.png"
#define PRESET_WIND_NORTH           "wind_north.png"
#define PRESET_WIND_NORTH_EAST      "wind_north_east.png"
#define PRESET_WIND_NORTH_WEST      "wind_north_west.png"
#define PRESET_WIND_WEST            "wind_west.png"
#define PRESET_WIND_EAST            "wind_east.png"
#define PRESET_WIND_SOUTH_WARN           "wind_south_warning.png"
#define PRESET_WIND_SOUTH_EAST_WARN      "wind_south_east_warning.png"
#define PRESET_WIND_SOUTH_WEST_WARN      "wind_south_west_warning.png"
#define PRESET_WIND_NORTH_WARN           "wind_north_warning.png"
#define PRESET_WIND_NORTH_EAST_WARN      "wind_north_east_warning.png"
#define PRESET_WIND_NORTH_WEST_WARN      "wind_north_west_warning.png"
#define PRESET_WIND_WEST_WARN            "wind_west_warning.png"
#define PRESET_WIND_EAST_WARN            "wind_east_warning.png"
#define PRESET_WIND_SOUTH           "wind_south.png"
#define PRESET_WIND_NORTH           "wind_north.png"
#define PRESET_WIND_EAST            "wind_east.png"
#define PRESET_WIND_WEST            "wind_west.png"
#define PRESET_WIND_SOUTH_EAST      "wind_south_east.png"
#define PRESET_WIND_NORTH_EAST      "wind_north_east.png"
#define PRESET_WIND_SOUTH_WEST      "wind_south_west.png"
#define PRESET_WIND_NORTH_WEST      "wind_north_west.png"
/* Strong wind > 30 km/h */
#define STRONG_WIND 30 
/*******************************************************************************/
void composition_now(WDB *new_day_button, gint layout);
void composition_left_vertical_day_button(WDB *new_day_button, gint layout);
void composition_central_vertical_day_button(WDB *new_day_button);
void composition_right_vertical_day_button(WDB *new_day_button, gint layout);
void composition_central_horizontal_day_button(WDB *new_day_button);
void composition_right_horizontal_day_button(WDB *new_day_button);
void fill_weather_day_button_presets(WDB *new_day_button, const char *text, const char *icon,
                const gint icon_size, gboolean transparency,
                gboolean draw_day_label, gint wind_direction, gfloat wind_speed);
GtkWidget *next_station_preset_now(gint layout);
void fill_weather_day_button_preset_now(WDB *new_day_button, const char *text, const char *icon,
                const gint icon_size, gboolean transparency,
                gboolean draw_day_label, gint wind_direction, gfloat wind_speed);
/*******************************************************************************/
#endif
