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
	
*/

#include "weather-event.h"

static GSList *event_time_list = NULL;

gboolean 
timer_handler(gpointer data){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    time_t current_time;

    if(not_event == TRUE || !event_time_list)
	return TRUE;

    list_time_event_temp = event_time_list;  
    /* get current time */  
    current_time = time(NULL);
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	print_list();
	fprintf (stderr,"%i %i\n",evt->time,current_time);
	if(evt->time <= current_time){
	    fprintf(stderr,"BEGIN\n");
	    switch(evt->type_event){
		case DAYTIMEEVENT :
     		    weather_frame_update(FALSE);   
		break;
		case AUTOUPDATE:
		    /* Reinitialise autoupdate event */ 
		    /* delete periodic update */
                    event_time_list=g_slist_remove(event_time_list,event_time_list->data);
		    fprintf(stderr,"UPDATE\n");
		    update_weather();
                    /* add periodic update */
                    add_periodic_event();
    		break;		    
	    }
	    break;
	}             
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
return TRUE;    
}

/*For debug */
void print_list(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;

    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list;  
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	fprintf(stderr,"Time: %s,Event: %i\n",ctime(&evt->time),evt->type_event);
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
}

void timer(void){
    g_timeout_add(60000,
		    (GtkFunction)timer_handler,
		    app->main_window); /* One per minute */
}

/* Free memory allocated for time event */
void free_list_time_event(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;

    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list; 
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	g_free(evt);
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    g_slist_free(event_time_list);
    event_time_list = NULL;
}

/* Compare function for sort event list */
static gint compare_time(gconstpointer a, gconstpointer b){
    struct event_time *evta, *evtb;
    
    evta = (struct event_time*)a;
    evtb = (struct event_time*)b;
    return(evta->time < evtb->time) ? -1 : (evta->time > evtb->time) ? +1 : 0;
}

/* Add time event  to list */	  
void time_event_add(time_t time_value,short int type_event){
    struct event_time *evt;
    evt = g_new0(struct event_time, 1);
    evt->time = time_value;	  
    evt->type_event = type_event;	  
    event_time_list = g_slist_insert_sorted(event_time_list,evt,compare_time);
}

/* Add periodic time event  to list */	  
void add_periodic_event(void){
    if(_weather_periodic_update > 0)
	time_event_add(time(NULL)+_weather_periodic_update*60,AUTOUPDATE);
}

/* Remove periodic time event  from list */	  
void remove_periodic_event(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    
    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list;  
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	if(evt->type_event == AUTOUPDATE)
	    event_time_list=g_slist_remove(event_time_list,event_time_list->data);
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
}
