/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008-2009 Andrew Zhilin
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

#ifndef _weather_event_h
#define _weather_event_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
extern gboolean not_event;
/*******************************************************************************/
gboolean timer_handler(gpointer data);
void timer(int interval);
void create_timer_with_interval(guint interval);
void add_periodic_event(time_t last_update);
void add_gps_event(guint interval);
void add_current_time_event(void);
void remove_daytime_event(void);
void remove_periodic_event(void);
void print_list(char *buff, size_t buff_size);
void event_add(time_t time_value, short type_event);
time_t next_update(void);
void free_list_time_event(void);
/*******************************************************************************/
#endif
