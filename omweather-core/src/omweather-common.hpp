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
#ifndef _omweather_common_hpp
#define _omweather_common_hpp 1
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <string>
#include <vector>
#include <gtk/gtk.h>
#ifdef ENABLE_NLS
#include <libintl.h>
#endif
/*******************************************************************************/
/*
union Param{
    bool        as_bool;
    int         as_int;
    char        *as_string;
    GdkColor    as_color;
};
*/
class Param{
    public:
        union{
            bool        bool_param;
            int         int_param;
            char*       string_param;
            GdkColor    color_param;
        };
        inline int toInt(){ return int_param; }
        inline bool toBool(){ return bool_param; }
        inline char* toString(){ return string_param; }
        inline GdkColor toColor(){ return color_param; }
};
/*******************************************************************************/
enum { INVALID_VALUE = 0, TINY, SMALL, MEDIUM, LARGE, GIANT };
enum { ONE_ROW, ONE_COLUMN, TWO_ROWS, TWO_COLUMNS, COMBINATION, PRESET_NOW,
        PRESET_NOW_PLUS_TWO, PRESET_NOW_PLUS_THREE_V, PRESET_NOW_PLUS_THREE_H,
        PRESET_NOW_PLUS_SEVEN, APPLICATION_MODE };
enum { RIGHT, LEFT, TOP, BOTTOM, NOTHING };
enum { METERS, KILOMETERS, MILES, SEA_MILES };
enum { METERS_S, KILOMETERS_H, MILES_H };
enum { CELSIUS, FAHRENHEIT };
enum { MB, INCH, MM };
enum { SIMPLE_MODE, EXTENDED_MODE };
enum { SHORT_CLICK, LONG_CLICK };
enum { COLLAPSED_VIEW_MODE, EXPANDED_VIEW_MODE };
enum { SETTINGS_STATIONS_PAGE, SETTINGS_VISUALS_PAGE, SETTINGS_DISPLAY_PAGE,
       SETTINGS_UNITS_PAGE, SETTINGS_UPDATE_PAGE,
#if defined(OS2008) || defined(OS2009) || defined(NONMAEMO)
       SETTINGS_SENSOR_PAGE,
#endif
       ALERTS_PAGE, MAX_SETTINGS_PAGE_NUMBER, ABOUT_PAGE
};
enum { ICON, STATION_NAME };
/*******************************************************************************/
#define _(String) dgettext(GETTEXT_PACKAGE, String)
#define Max_count_weather_day   10
#define ICONS_PATH              "/usr/share/omweather/icons/"
/*******************************************************************************/
#endif
