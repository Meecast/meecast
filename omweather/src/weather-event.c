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

gboolean timer_handler(gpointer data){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    time_t current_time;
#ifdef PC_EMULATOR
    char   *temp_string;
#endif

    if(not_event == TRUE || !event_time_list)
	return TRUE;

    list_time_event_temp = event_time_list;  
    /* get current time */  
    current_time = time(NULL);
#ifdef PC_EMULATOR    
    temp_string = ctime(&current_time);
    fprintf(stderr,"\nCurrent Time: %s\n",  temp_string);
/*    g_free(temp_string);*/
    print_list();
#endif	
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	if(evt->time <= current_time){
	    switch(evt->type_event){
		case DAYTIMEEVENT :
     		    weather_frame_update(FALSE);   
		break;
		case DBUSINITEVENT:
		#ifdef PC_EMULATOR
		    fprintf(stderr,"DBUSINITEVENT %s\n",ctime(&evt->time));
		#endif
		    g_free(evt);
                    event_time_list = g_slist_remove(event_time_list, event_time_list->data);
		    weather_initialize_dbus();
		break;    
		default:
		case AUTOUPDATE:
		    /* delete periodic update */
		#ifdef PC_EMULATOR
		    fprintf(stderr,"Delete evt %s\n",ctime(&evt->time));
		#endif
		    g_free(evt);
                    event_time_list = g_slist_remove(event_time_list, event_time_list->data);
		#ifdef PC_EMULATOR
		    fprintf(stderr,"UPDATE by event\n");
		#endif
		    app->show_update_window = FALSE;
		    update_weather();
                    /* add periodic update */
                    add_periodic_event(current_time);
		    
    		break;		    
	    }
	    break;
	}             
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    return TRUE;    
}

#ifdef PC_EMULATOR
/*For debug */
void print_list(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;

    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list;
    fprintf(stderr, "\n");
    while(list_time_event_temp){
	evt = list_time_event_temp->data;
	fprintf(stderr,"Event %i Time: %s", evt->type_event, ctime(&evt->time));
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    fprintf(stderr, "\n");
}
#endif

void timer(void){
    app->timer = g_timeout_add(60000,
			       (GtkFunction)timer_handler,
			       app->main_window); /* One per minute */
}

/* Free memory allocated for time event */
void free_list_time_event(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    
    #ifdef PC_EMULATOR
    fprintf(stderr,"Free ALL in list\n");
    print_list();
    #endif
    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list; 
    while(list_time_event_temp != NULL){
        #ifdef PC_EMULATOR
	fprintf(stderr,"delete\n");
	#endif
	evt = list_time_event_temp->data;
	g_free(evt);
/*	list_time_event_temp = g_slist_remove(list_time_event_temp, list_time_event_temp->data);*/
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    g_slist_free(event_time_list);
    event_time_list = NULL;
    #ifdef PC_EMULATOR
    fprintf(stderr,"list clean\n");
    print_list();
    #endif

}

/* Compare function for sort event list */
static gint compare_time(gconstpointer a, gconstpointer b){
    struct event_time *evta, *evtb;
    
    evta = (struct event_time*)a;
    evtb = (struct event_time*)b;
    return(evta->time < evtb->time) ? -1 : (evta->time > evtb->time) ? +1 : 0;
}

/* Add time event  to list */	  
void time_event_add(time_t time_value, short type_event){
    struct event_time *evt;

    #ifdef PC_EMULATOR
    fprintf(stderr,"time_event_add in list\n");
    print_list();
    #endif

    evt = g_new0(struct event_time, 1);
    evt->time = time_value;	  
    evt->type_event = type_event;
    event_time_list = g_slist_insert_sorted(event_time_list,evt,compare_time);

    #ifdef PC_EMULATOR
    fprintf(stderr,"time_event_add in list finished\n");
    print_list();
    #endif
    
}

/* Add periodic time event  to list */	  
void add_periodic_event(time_t last_update){

    #ifdef PC_EMULATOR
    fprintf(stderr,"Add in list\n");
    print_list();
    #endif

    if(app->update_interval > 0)
	time_event_add(last_update + app->update_interval * 60, AUTOUPDATE);

    #ifdef PC_EMULATOR
    fprintf(stderr,"Item added to list\n");
    print_list();
    #endif
	
}

/* Remove periodic time event  from list */	  
void remove_periodic_event(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    
    #ifdef PC_EMULATOR
    fprintf(stderr,"Periodic remove from list\n");
    print_list();
    #endif

    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list;
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	if(evt->type_event == AUTOUPDATE){
	    g_free(evt);
	    list_time_event_temp = g_slist_remove(list_time_event_temp, list_time_event_temp->data);
	}
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    event_time_list = list_time_event_temp;
    #ifdef PC_EMULATOR
    fprintf(stderr,"Periodic is remove from list\n");
    print_list();
    #endif
}
