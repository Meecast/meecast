/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2009 Andrew Zhilin
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
#ifndef _weather_clutter_h
#define _weather_clutter_h 1
/*******************************************************************************/
#include "weather-common.h"
#if defined CLUTTER
/*******************************************************************************/
/*******************************************************************************/
gboolean show_animation(GSList *clutter_objects);
GtkWidget *create_clutter_icon_animation(GdkPixbuf *icon_buffer, const char *icon_path, int icon_size, GSList **objects_list);
void change_actor_size_and_position(ClutterActor *actor, gint need_size);
void change_knots_path(GSList *knots, gint need_size);
#endif
/*******************************************************************************/
#endif
