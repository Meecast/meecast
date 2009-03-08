/* vim: set sw=4 ts=4 et: */
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
#ifdef OS2009
    #include <hildon/hildon-help.h>
#elif OS2008
    #include <hildon/hildon-help.h>
#else
    #include <osso-helplib.h>
#endif
/*******************************************************************************/
#define OMWEATHER_SETTINGS_STATIONS_HELP_ID	"help_omweather_settings_stations"
#define OMWEATHER_SETTINGS_VISUALS_HELP_ID	"help_omweather_settings_visuals"
#define OMWEATHER_SETTINGS_DISPLAY_HELP_ID	"help_omweather_settings_display"
#define OMWEATHER_SETTINGS_UNITS_HELP_ID	"help_omweather_settings_units"
#define OMWEATHER_SETTINGS_UPDATE_HELP_ID	"help_omweather_settings_update"
#define OMWEATHER_SETTINGS_SENSOR_HELP_ID	"help_omweather_settings_sensor"
#define OMWEATHER_ABOUT_HELP_ID			"help_omweather_about"
/*******************************************************************************/
void help_activated_handler(GtkWidget *window, gchar *help_id);
void create_about_dialog(void);	/* create help dialog */
/*******************************************************************************/
extern GtkWidget*
create_scrolled_window_with_text(const char* text, GtkJustification justification);
/*******************************************************************************/
#endif
