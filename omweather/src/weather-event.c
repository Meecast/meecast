/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
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
	
*/

#include "weather-event.h"

void 
timer_handler(gpointer data)
{
 static GSList *list_time_event_temp = NULL;
 struct event_time *evt;
 time_t current_time;

 if (event_time_list != NULL)
 { 
  list_time_event_temp = event_time_list;  
  /* get current time */  
  current_time = time(NULL);
  while (list_time_event_temp != NULL)
  {
   evt = list_time_event_temp->data;
   if (evt->time < current_time)
   {
    weather_frame_update();   
    break;
   }
   list_time_event_temp = g_slist_next(list_time_event_temp);
  }
 } 
}

void 
timer(void)
{
 flag_event = g_timeout_add (60000, timer_handler, box); /* One per minute */
}


/* Free memory allocated for time event */
void 
free_list_time_event (void)
{
 static GSList *list_time_event_temp = NULL;
 struct event_time *evt;

 if (event_time_list != NULL)
 { 
  list_time_event_temp = event_time_list; 
  while (list_time_event_temp != NULL)
  {
     evt = list_time_event_temp->data;
     g_free(evt);
     list_time_event_temp = g_slist_next(list_time_event_temp);
  }
  g_slist_free(event_time_list);
  event_time_list = NULL;
 } 
}

/* Add time event  to list */	  
void time_event_add(time_t time_value)
{
  struct event_time *evt;
  evt = g_new0(struct event_time, 1);
  evt->time = time_value;	  
  event_time_list = g_slist_append(event_time_list,evt); 
}
