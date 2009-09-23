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
#include "weather-event.h"
#include "weather-home.h"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif


/*******************************************************************************/
gboolean not_event = FALSE;
/*******************************************************************************/
static GSList *event_time_list = NULL;
/*******************************************************************************/
gboolean 
timer_handler(gpointer data){
    static GSList       *list_time_event_temp = NULL;
    struct event_time   *evt;
    time_t              current_time;
    int                 check;
#ifdef ENABLE_GPS
    double              distance;
#endif
#ifdef DEBUGEVENTS
    gchar               buffer[2048];
    char                *temp_string;
#endif
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (not_event == TRUE || !event_time_list)
        return TRUE;

    list_time_event_temp = event_time_list;
    /* get current time */
    current_time = time(NULL);
#ifdef DEBUGEVENTS
    temp_string = ctime(&current_time);
    fprintf(stderr, "\nCurrent Time: %s\n", temp_string);
/*    g_free(temp_string);*/
    print_list(NULL, 0);
#endif
    while (list_time_event_temp != NULL) {
        evt = list_time_event_temp->data;
        if (evt->time <= current_time) {
            switch (evt->type_event) {
            case CHANGE_DAY_PART:
#ifndef RELEASE
                fprintf(stderr, "CHANGE_DAY_PART %s\n", ctime(&evt->time));
#endif
                g_free(evt);
                event_time_list =
                    g_slist_remove(event_time_list, event_time_list->data);
                redraw_home_window(FALSE);
                break;
            case DBUSINITEVENT:
#ifndef RELEASE
                fprintf(stderr, "DBUSINITEVENT %s\n", ctime(&evt->time));
#endif
                g_free(evt);
                event_time_list =
                    g_slist_remove(event_time_list, event_time_list->data);
/*               weather_initialize_dbus();*/
                /* It is switch off the timer */
                check = g_source_remove(app->timer);
                timer(60000);   /*Reintilize timer One per minute */
                break;
            case UPDATE_AFTER_CONNECTED:
                /* delete periodic update */
#ifndef RELEASE
                fprintf(stderr, "Delete evt %s\n", ctime(&evt->time));
#endif
                g_free(evt);
                event_time_list =
                    g_slist_remove(event_time_list, event_time_list->data);
#ifndef RELEASE
                fprintf(stderr, "UPDATE by event\n");
#endif
                update_weather(TRUE);
                break;
#ifdef ENABLE_GPS
            case CHECK_GPS_POSITION:
                /* delete periodic update */
#ifndef RELEASE
                fprintf(stderr, "Delete evt %s\n", ctime(&evt->time));
#endif
                g_free(evt);
                event_time_list =
                    g_slist_remove(event_time_list, event_time_list->data);
#ifndef RELEASE
                fprintf(stderr, "UPDATE by event\n");
#endif

/* This is code for debug GPS 
                    fprintf(stderr,"Event:  Calculate CHECK_GPS_POSITION %f %f %f %f %f\n",
                                    app->gps_station.latitude,app->gps_station.longtitude,
                                    app->temporary_station_latitude,app->temporary_station_longtitude,
                                    calculate_distance(app->gps_station.latitude,app->gps_station.longtitude,
                                                    app->temporary_station_latitude,app->temporary_station_longtitude));

*/
                distance = calculate_distance(app->gps_station.latitude,app->gps_station.longtitude,
                                                    app->temporary_station_latitude,app->temporary_station_longtitude);
/*
                sprintf(buffer,"Event:  Calculate CHECK_GPS_POSITION %f %f %f %f %f\n",
                                    app->gps_station.latitude,app->gps_station.longtitude,
                                    app->temporary_station_latitude,app->temporary_station_longtitude,
                                    distance);
                write_log(buffer);
*/

                if (distance > 10 || distance < 0) {
                    /* Stop location service */
                    if (app->gps_was_started){
                          location_gpsd_control_stop(app->gps_control);
                          app->gps_was_started = FALSE;
                    }
                    get_nearest_station
                        (app->temporary_station_latitude,
                         app->temporary_station_longtitude,
                         &app->gps_station);
                    if ((strlen(app->gps_station.id0) > 0)
                        && (strlen(app->gps_station.name) > 0)) {

                        app->gps_station.latitude =
                            app->temporary_station_latitude;
                        app->gps_station.longtitude =
                            app->temporary_station_longtitude;
                        delete_all_gps_stations();
                        if (app->config->current_source)
                                g_free(app->config->current_source);
                            app->config->current_source =
                                g_strdup("weather.com");

                        add_station_to_user_list(app->gps_station.name,
                                                 app->gps_station.id0,
                                                 TRUE, app->config->current_source, -1);
                        if (!app->config->current_station_name
                            && !app->config->current_station_id) {
                            /* Fix me if source are not weather.com */
                            if (app->config->current_station_name)
                                g_free(app->config->current_station_name);
                            app->config->current_station_name =
                                g_strdup(app->gps_station.name);

                            if (app->config->current_station_id)
                                g_free(app->config->current_station_id);
                            app->config->current_station_id =
                                g_strdup(app->gps_station.id0);
                        }
                        config_save(app->config);
                        update_weather(FALSE);
                        redraw_home_window(FALSE);
                    }
                }
#ifdef ENABLE_GPS
                /* add periodic gps check */
                if (app->gps_station.latitude == 0
                    && app->gps_station.longtitude == 0)
                    add_gps_event(1);
                else
                    add_gps_event(1);
#endif
                break;
#endif
            default:
            case AUTOUPDATE:
                /* delete periodic update */
#ifndef RELEASE
                fprintf(stderr, "Delete evt %s\n", ctime(&evt->time));
#endif
                g_free(evt);
                event_time_list =
                    g_slist_remove(event_time_list, event_time_list->data);
#ifndef RELEASE
                fprintf(stderr, "UPDATE by event\n");
#endif
                update_weather(FALSE);
                /* add periodic update */
                add_periodic_event(current_time);
                break;
            }
            break;
        }
        list_time_event_temp = g_slist_next(list_time_event_temp);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return TRUE;
}

/*******************************************************************************/
#ifndef RELEASE
/*For debug */
void print_list(char *buff, size_t buff_size) {
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
    char tmp[3072];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    memset(tmp, 0, sizeof(tmp));
    if (!event_time_list)
        return;
    list_time_event_temp = event_time_list;
    sprintf(tmp, "Length %i\n", g_slist_length(list_time_event_temp));
    while (list_time_event_temp) {
        evt = list_time_event_temp->data;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,
                 "Event %i Time: %s\n", evt->type_event,
                 ctime(&evt->time));
        list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "\n");
    if (buff && buff_size)
        memcpy(buff, tmp, buff_size);
    else
        fprintf(stderr, tmp);
}
#endif
/*******************************************************************************/
void create_timer_with_interval(guint interval) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->timer = g_timeout_add(interval, (GtkFunction) timer_handler, NULL);    /* One per minute */
}

/*******************************************************************************/
void timer(int interval) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->timer = g_timeout_add(interval,
                               (GtkFunction) timer_handler,
                               app->main_window);
}

/*******************************************************************************/
/* Free memory allocated for time event */
void free_list_time_event(void) {
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef DEBUGEVENTS
    fprintf(stderr, "Free ALL in list\n");
    print_list(NULL, 0);
#endif
    if (!event_time_list)
        return;
    list_time_event_temp = event_time_list;
    while (list_time_event_temp != NULL) {
#ifndef RELEASE
        fprintf(stderr, "delete\n");
#endif
        evt = list_time_event_temp->data;
        g_free(evt);
/*	list_time_event_temp = g_slist_remove(list_time_event_temp, list_time_event_temp->data);*/
        list_time_event_temp = g_slist_next(list_time_event_temp);
    }
    g_slist_free(event_time_list);
    event_time_list = NULL;
#ifdef DEBUGEVENTS
    fprintf(stderr, "list clean\n");
    print_list(NULL, 0);
#endif
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}

/*******************************************************************************/
/* Compare function for sort event list */
static gint compare_time(gconstpointer a, gconstpointer b) {
    struct event_time *evta, *evtb;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    evta = (struct event_time *)a;
    evtb = (struct event_time *)b;
    return (evta->time < evtb->time) ? -1 : (evta->time >
                                             evtb->time) ? +1 : 0;
}

/*******************************************************************************/
/* Add time event  to list */
void event_add(time_t time_value, short type_event) {
    struct event_time *evt = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef DEBUGEVENTS
    fprintf(stderr, "event_add in list\n");
    print_list(NULL, 0);
#endif
    if (time_value && time_value > time(NULL)) {
        evt = g_new0(struct event_time, 1);
        if (evt != NULL) {
            evt->time = time_value;
            evt->type_event = type_event;
            event_time_list =
                g_slist_insert_sorted(event_time_list, evt, compare_time);
        } else
            fprintf(stderr, "evt NULL\n");
    }
#ifdef DEBUGEVENTS
    fprintf(stderr, "event_add in list finished\n");
    print_list(NULL, 0);
#endif
}

/*******************************************************************************/
/*  Addition the periodic time in the list of events  for weather forecast updating.  
    Interval is a count of minutes for the next interval */
void add_gps_event(guint interval) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef DEBUGEVENTS
    fprintf(stderr, "Add in list\n");
    print_list(NULL, 0);
#endif
    event_add(time(NULL) + interval * 60, CHECK_GPS_POSITION);  /* Every 'interval' minutes */
#ifdef DEBUGEVENTS
    fprintf(stderr, "Item added to list\n");
    print_list(NULL, 0);
#endif
}

/*******************************************************************************/
/*  Addition the periodic time in the list of events  for GPS checking */
void add_periodic_event(time_t last_update) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef DEBUGEVENTS
    fprintf(stderr, "Add in list\n");
    print_list(NULL, 0);
#endif
    event_add(last_update + app->config->update_interval * 60, AUTOUPDATE);
#ifdef DEBUGEVENTS
    fprintf(stderr, "Item added to list\n");
    print_list(NULL, 0);
#endif
}

/*******************************************************************************/
/* Addition the current time in the list of events  for weather forecast updating */
void add_current_time_event(void) {
    time_t current_time;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef DEBUGEVENTS
    fprintf(stderr, "Add in list\n");
    print_list(NULL, 0);
#endif
    /* get current time */
    current_time = time(NULL);
    event_add(current_time + 4, UPDATE_AFTER_CONNECTED);        /* The current time plus 4 seconds */

#ifdef DEBUGEVENTS
    fprintf(stderr, "Item added to list\n");
    print_list(NULL, 0);
#endif
}

/*******************************************************************************/
/* Remove periodic time event  from list */
void remove_periodic_event(void) {
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef DEBUGEVENTS
    fprintf(stderr, "Periodic remove from list\n");
    print_list(NULL, 0);
#endif
    if (!event_time_list)
        return;
    list_time_event_temp = event_time_list;
    while (list_time_event_temp != NULL) {
        evt = list_time_event_temp->data;
        if (evt->type_event == AUTOUPDATE) {
            event_time_list =
                g_slist_remove(event_time_list,
                               list_time_event_temp->data);
            list_time_event_temp = event_time_list;
            g_free(evt);
        }
        list_time_event_temp = g_slist_next(list_time_event_temp);
    }

#ifdef DEBUGEVENTS
    fprintf(stderr, "Periodic is remove from list\n");
    print_list(NULL, 0);
#endif
}

/*******************************************************************************/
void remove_daytime_event(void) {
    static GSList *list_time_event_temp = NULL;
    struct event_time *evt;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef DEBUGEVENTS
    fprintf(stderr, "CHANGE_DAY_PART remove from list\n");
    print_list(NULL, 0);
#endif
    if (!event_time_list)
        return;
    list_time_event_temp = event_time_list;
    while (list_time_event_temp != NULL) {
        evt = list_time_event_temp->data;
        if (evt->type_event == CHANGE_DAY_PART) {
            event_time_list =
                g_slist_remove(event_time_list,
                               list_time_event_temp->data);
            list_time_event_temp = event_time_list;
            g_free(evt);
        }
        list_time_event_temp = g_slist_next(list_time_event_temp);
    }

#ifdef DEBUGEVENTS
    fprintf(stderr, "CHANGE_DAY_PART is remove from list\n");
    print_list(NULL, 0);
#endif
}

/*******************************************************************************/
time_t next_update(void) {
    GSList *tmp_list = NULL;
    time_t result = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!event_time_list)
        return result;
    tmp_list = event_time_list;
    while (tmp_list) {
        if (((struct event_time *)tmp_list->data)->type_event ==
            AUTOUPDATE) {
            result = ((struct event_time *)tmp_list->data)->time;
            break;
        }
        tmp_list = g_slist_next(tmp_list);
    }
    return result;
}

/*******************************************************************************/
