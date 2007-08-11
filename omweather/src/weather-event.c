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
/*******************************************************************************/
#include "weather-event.h"
/*******************************************************************************/
static GSList *event_time_list = NULL;
/*******************************************************************************/
gboolean timer_handler(gpointer data){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    time_t current_time;
#ifndef RELEASE
    char   *temp_string;
    fprintf(stderr, "Begin %s(): \n", __PRETTY_FUNCTION__);
#endif
    if(not_event == TRUE || !event_time_list)
	return TRUE;

    list_time_event_temp = event_time_list;  
    /* get current time */  
    current_time = time(NULL);
#ifndef RELEASE
    temp_string = ctime(&current_time);
    fprintf(stderr,"\nCurrent Time: %s\n",  temp_string);
/*    g_free(temp_string);*/
    print_list(NULL, 0);
#endif	
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	if(evt->time <= current_time){
	    switch(evt->type_event){
		case DAYTIMEEVENT :
     		    weather_frame_update(FALSE);   
		break;
		default:
		case AUTOUPDATE:
		    /* delete periodic update */
		#ifndef RELEASE
		    fprintf(stderr,"Delete evt %s\n",ctime(&evt->time));
		#endif
		    g_free(evt);
                    event_time_list = g_slist_remove(event_time_list, event_time_list->data);
		#ifndef RELEASE
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
/*******************************************************************************/
#ifndef RELEASE
/*For debug */
void print_list(char *buff, size_t buff_size){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    char	tmp[3072];

    memset(tmp, 0, sizeof(tmp));
    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list;
    sprintf(tmp, "Length %i\n",g_slist_length (list_time_event_temp)  );    
    while(list_time_event_temp){
	evt = list_time_event_temp->data;
	snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,
		    "Event %i Time: %s\n", 
		    evt->type_event, ctime(&evt->time));
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "\n");
    if(buff && buff_size)
	memcpy(buff, tmp, buff_size);
    else
	fprintf(stderr, tmp);
}
#endif
/*******************************************************************************/
void create_timer_with_interval(guint interval){
    app->timer = g_timeout_add(interval,
			       (GtkFunction)timer_handler,
			       NULL); /* One per minute */
}
/*******************************************************************************/
/* Free memory allocated for time event */
void free_list_time_event(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    
    #ifndef RELEASE
    fprintf(stderr,"Free ALL in list\n");
    print_list(NULL, 0);
    #endif
    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list; 
    while(list_time_event_temp != NULL){
        #ifndef RELEASE
	fprintf(stderr,"delete\n");
	#endif
	evt = list_time_event_temp->data;
	g_free(evt);
/*	list_time_event_temp = g_slist_remove(list_time_event_temp, list_time_event_temp->data);*/
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    g_slist_free(event_time_list);
    event_time_list = NULL;
    #ifndef RELEASE
    fprintf(stderr,"list clean\n");
    print_list(NULL, 0);
    #endif

}
/*******************************************************************************/
/* Compare function for sort event list */
static gint compare_time(gconstpointer a, gconstpointer b){
    struct event_time *evta, *evtb;
    
    evta = (struct event_time*)a;
    evtb = (struct event_time*)b;
    return(evta->time < evtb->time) ? -1 : (evta->time > evtb->time) ? +1 : 0;
}
/*******************************************************************************/
/* Add time event  to list */	  
void time_event_add(time_t time_value, short type_event){
    struct event_time *evt = NULL;

    #ifndef RELEASE
    /* fprintf(stderr,"time_event_add in list\n");
    print_list(NULL, 0); */
    #endif
    if( time_value && time_value > time(NULL)){ 
	evt = g_new0(struct event_time, 1);
	if (evt != NULL){
	    evt->time = time_value;	  
	    evt->type_event = type_event;
	    event_time_list = g_slist_insert_sorted(event_time_list,evt,compare_time);
	}    
	else
	    fprintf(stderr,"evt NULL\n");
    }
    #ifndef RELEASE
    /* fprintf(stderr,"time_event_add in list finished\n");
    print_list(NULL, 0); */
    #endif
}
/*******************************************************************************/
/* Add periodic time event  to list */	  
void add_periodic_event(time_t last_update){

    #ifndef RELEASE
    fprintf(stderr,"Add in list\n");
    print_list(NULL, 0);
    #endif

    if(app->config->update_interval > 0)
	time_event_add(last_update + app->config->update_interval * 60, AUTOUPDATE);

    #ifndef RELEASE
    fprintf(stderr,"Item added to list\n");
    print_list(NULL, 0);
    #endif
	
}
/*******************************************************************************/
/* Remove periodic time event  from list */	  
void remove_periodic_event(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    
    #ifndef RELEASE
    fprintf(stderr,"Periodic remove from list\n");
    print_list(NULL, 0);
    #endif

    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list;  
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	if(evt->type_event == AUTOUPDATE){
	    event_time_list = g_slist_remove(event_time_list, list_time_event_temp->data);
	    list_time_event_temp = event_time_list;  
	    g_free(evt);
	}
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }

    #ifndef RELEASE
    fprintf(stderr,"Periodic is remove from list\n");
    print_list(NULL, 0);
    #endif
}
/*******************************************************************************/
void remove_daytime_event(void){
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    
    #ifndef RELEASE
    fprintf(stderr,"DAYTIMEEVENT remove from list\n");
    print_list(NULL, 0);
    #endif

    if(!event_time_list)
	return;
    list_time_event_temp = event_time_list;  
    while(list_time_event_temp != NULL){
	evt = list_time_event_temp->data;
	if(evt->type_event == DAYTIMEEVENT){
	    event_time_list = g_slist_remove(event_time_list, list_time_event_temp->data);
	    list_time_event_temp = event_time_list;  
	    g_free(evt);
	}
	list_time_event_temp = g_slist_next(list_time_event_temp);
    }

    #ifndef RELEASE
    fprintf(stderr,"DAYTIMEEVENT is remove from list\n");
    print_list(NULL, 0);
    #endif
}
/*******************************************************************************/
time_t next_update(void){
    GSList	*tmp_list = NULL;
    time_t	result	= 0;

    if(!event_time_list)
	return result;
    tmp_list = event_time_list;
    while(tmp_list){
	if(((struct event_time*)tmp_list->data)->type_event == AUTOUPDATE){
	    result = ((struct event_time*)tmp_list->data)->time;
	    break;
	}
	tmp_list = g_slist_next(tmp_list);
    }
    return result;
}
/*******************************************************************************/
