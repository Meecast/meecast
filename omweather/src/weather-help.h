/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
#ifndef _weather_help_h
#define _weather_help_h 1
/*******************************************************************************/
#include "weather-common.h"
#include <libosso.h>
#ifdef HILDON
    #include <hildon/hildon-help.h>
#else
    #include <osso-helplib.h>
#endif
/*******************************************************************************/
#define OMWEATHER_SETTINGS_HELP_ID		"help_omweather_settings"
#define OMWEATHER_ABOUT_HELP_ID			"help_omweather_about"
#define OMWEATHER_ADD_STATION_HELP_ID		"help_omweather_add_station"
#define OMWEATHER_ADD_CUSTOM_STATION_HELP_ID	"help_omweather_add_custom_station"
/*******************************************************************************/
void help_activated_handler(GtkWidget *window, gchar *help_id);
/*******************************************************************************/
#endif
