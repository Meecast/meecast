/* vim: set sw=4 ts=4 et: */
/* This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * Copyright (C) 2008 Andrew Olmsted
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 /Now* modify it under the terms of the GNU Lesser General Public License
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
#include "weather-home.h"
#include "weather-popup.h"
#include "weather-simple-popup.h"
#include "weather-sources.h"
#include "weather-presets.h"
#include "weather-utils.h"
#include "weather-applet-expose.h"
#include "weather-config.h"
#include "weather-dbus.h"
#include "weather-event.h"
#include "weather-download.h"
#include "weather-settings.h"
#include "weather-presets.h"
#include <stdio.h>
#include <unistd.h>
#include <glib.h>
#include <math.h>
#if defined OS2008 || defined OS2009
    #include <libhildondesktop/libhildondesktop.h>
#elif NONMAEMO 
    #include <hildon/hildon.h>
#else
    #include <hildon-home-plugin/hildon-home-plugin-interface.h>
#endif
#include <unistd.h>
#include "weather-hash.h"
#include "weather-data.h"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#define APPLET_X_SIZE           200
#define APPLET_Y_SIZE           40
#define FONT_MAIN_SIZE_GIANT    18
#define FONT_MAIN_SIZE_LARGE    16
#define FONT_MAIN_SIZE_MEDIUM   14
#define FONT_MAIN_SIZE_SMALL    12
#define FONT_MAIN_SIZE_TINY     10
#define CLICK_TIMEOUT           500
/*******************************************************************************/
#if defined OS2008 && !defined (APPLICATION)
HD_DEFINE_PLUGIN(OMWeather, omweather, HILDON_DESKTOP_TYPE_HOME_ITEM)

struct _OMWeatherPrivate {
	GtkWidget	*desktop;
};
#endif

#if defined OS2009 && !defined (APPLICATION)
HD_DEFINE_PLUGIN_MODULE(OmweatherDesktopWidget, omweather_plugin, HD_TYPE_HOME_PLUGIN_ITEM)
/*
#define OMWEATHER_PLUGIN_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (obj,\
							                         OMWEATHER_TYPE_HOME_PLUGIN,\
							                         OmweatherPluginPrivate))
struct _OmweatherPluginPrivate
{
  gpointer data;
};
*/
#endif
/* main struct */
OMWeatherApp	*app = NULL;
/*******************************************************************************/
void
view_popup_menu (GtkWidget *treeview, GdkEventButton *event, gpointer userdata){
    fprintf(stderr,"dddddddddddddddddddddddddd\n");
/*
  gtk_widget_show_all(app->contextmenu);
  menu_init();
*/
    weather_window_settings(NULL, NULL);
}
/*******************************************************************************/
#ifdef CLUTTER
/* For start of Clutter animation in main form */
top_widget_expose(GtkWidget *widget, GdkEventExpose *event){
    show_animation(app->clutter_objects_in_main_form);
    gtk_widget_show_all(widget);
    g_signal_handlers_disconnect_by_func(G_OBJECT(widget),G_CALLBACK(top_widget_expose),NULL);
}
#endif
/*******************************************************************************/
/* Change station to previos at main display */
gboolean 
change_station_prev(GtkWidget *widget, GdkEvent *event,
                                   gpointer user_data){
    GtkTreeIter iter,
                prev_iter;
    gboolean    valid,
                ready = FALSE;
    gchar       *station_name = NULL,
                *station_code = NULL,
                *station_source = NULL;
    GtkTreePath *path;
    gint        day_number = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!(app->config->current_station_id))
        return FALSE;

    path = gtk_tree_path_new_first();
    valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                    &iter, path);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter,
                            NAME_COLUMN, &station_name,
                            ID0_COLUMN, &station_code,
                            3, &station_source,
                            -1);
        if(ready){
            /* update current station code */
            if(app->config->current_station_id)
                g_free(app->config->current_station_id);
            app->config->current_station_id = station_code; 
            /* update current station name */
            if(app->config->current_station_name)
                g_free(app->config->current_station_name);
            app->config->current_station_name = station_name;
            if(app->config->current_station_source)
                g_free(app->config->current_station_source);
            app->config->current_station_source = station_source;
            app->config->previos_days_to_show = app->config->days_to_show;
            redraw_home_window(FALSE);
            config_save(app->config);
            break;
        }else{
            if((app->config->current_station_name) && (station_name) &&
                  !strcmp(app->config->current_station_name, station_name))
                ready = TRUE;
            g_free(station_name);
            g_free(station_code);
            valid = gtk_tree_path_prev(path);
            if(!valid){
                valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                                        &iter);
                if(!valid)
                    return FALSE;
                do{
                    memcpy(&prev_iter, &iter, sizeof(GtkTreeIter));
                }while(gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                                    &iter));
                path = gtk_tree_model_get_path(GTK_TREE_MODEL(app->user_stations_list),
                                                    &prev_iter);
            }
            valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                                   &iter, path);
        }
    }
    gtk_tree_path_free(path);
/* show popup window if received param */
    if(user_data){
        day_number = (gint)g_object_get_data(G_OBJECT(user_data), "active_tab"); 
        destroy_popup_window(NULL);
        weather_window_popup(NULL, NULL, (gpointer)day_number);
    }
    return FALSE;
}
/*******************************************************************************/
/* Change station to next at main display */
gboolean
change_station_next(GtkWidget *widget, GdkEvent *event,
                                    gpointer user_data){
    GtkTreeIter iter;
    gboolean    valid,
                ready         = FALSE;
    gchar       *station_name = NULL,
                *station_code = NULL,
                *station_source = NULL;
    gboolean    first = FALSE;
    gboolean    skipped = FALSE;
    GtkTreePath *path;
    gint        day_number = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!(app->config->current_station_id))
        return FALSE;

    path = gtk_tree_path_new_first();
    valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                    &iter, path);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter,
                            NAME_COLUMN, &station_name,
                            ID0_COLUMN, &station_code,
                            3, &station_source,
                            -1);
        /* Skip Empty stations */
        if(ready && !strcmp(station_name, " ")){
            skipped = TRUE;
            gtk_tree_path_next(path);
            valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter, path);
            if(!valid){
                path = gtk_tree_path_new_first();
                valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter);
                if (first)
                    break;
                else
                    first = TRUE;
            }
            continue;
        }
        if(ready){
            /* update current station code */
            if(app->config->current_station_id)
                g_free(app->config->current_station_id);
            app->config->current_station_id = station_code;
            /* update current station name */
            if(app->config->current_station_name)
                g_free(app->config->current_station_name);
            app->config->current_station_name = station_name;
            /* update current station source */
            if(app->config->current_station_source)
                g_free(app->config->current_station_source);
            app->config->current_station_source = station_source;

            app->config->previos_days_to_show = app->config->days_to_show;
            redraw_home_window(FALSE);
            config_save(app->config);
            break;
        }
        else{
            if(skipped || ((app->config->current_station_name) && (station_name) &&
                  !strcmp(app->config->current_station_name, station_name)))
                ready = TRUE;
            g_free(station_name);
            g_free(station_code);
            g_free(station_source);
            gtk_tree_path_next(path);

            valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter, path);
            if(!valid){
                valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter);
            }
        }
    }
    gtk_tree_path_free(path);
/* show popup window if received param */
    if(user_data){
        if(app->config->mode == SIMPLE_MODE){
            weather_simple_window_redraw(user_data);
            return FALSE;
        }
        day_number = (gint)g_object_get_data(G_OBJECT(user_data), "active_tab");
        destroy_popup_window(NULL);
        weather_window_popup(NULL, NULL, (gpointer)day_number);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return FALSE;
}
/*******************************************************************************/
gboolean 
change_station_select(GtkWidget *widget, gpointer user_data){
    GtkTreeIter iter;
    gboolean    valid;
    gchar       *station_name = NULL,
                *station_code = NULL,
                *station_source = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!(app->config->current_station_id) || (!(char*)user_data))
        return FALSE;

    if(!strcmp((char*)user_data, app->config->current_station_name))
        return FALSE;

    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                                  &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter,
                            NAME_COLUMN, &station_name,
                            ID0_COLUMN, &station_code,
                            3, &station_source,
                            -1);
	if( (station_name) &&
	    !strcmp((char*)user_data, station_name)){
        /* update current station code */
            if(app->config->current_station_id)
                g_free(app->config->current_station_id);
            app->config->current_station_id = station_code;
            /* update current station name */
            if(app->config->current_station_name)
                g_free(app->config->current_station_name);
            app->config->current_station_name = station_name;
            if(app->config->current_station_source)
                g_free(app->config->current_station_source);
            app->config->current_station_source = station_source;
            app->config->previos_days_to_show = app->config->days_to_show;
            redraw_home_window(FALSE);
            config_save(app->config);
            break;
        }
	else{
	    g_free(station_name);
	    g_free(station_code);
	}
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                                        &iter);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return FALSE;
}
/*******************************************************************************/
int 
calculate_offset_of_day(int count_day){
    long int    diff_time;
    int	        year,
                current_month,
                offset;
    time_t      current_day,
                current_time,
                date_time;
    struct tm   *tm = NULL;
    double      pre_offset = 0.0F;
    gchar       date_in_string[255];
    GSList      *tmp = NULL;
    GHashTable  *location = NULL,
                *first_day = NULL,
                *last_day = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
/* if no xml file is present return Max_count_weather_day */
    if(count_day < 1)
        return Max_count_weather_day;

    tmp = g_hash_table_lookup(app->station_data, "forecast");
    if (tmp){
        first_day = (GHashTable*)((g_slist_nth(tmp, 0))->data);
        last_day = (GHashTable*)((g_slist_last(tmp))->data);
        /* get current day */  
        current_time = time(NULL);
        location = g_hash_table_lookup(app->station_data, "location");
        if (location && g_hash_table_lookup(location, "station_time_zone"))
            diff_time = calculate_diff_time(atol(g_hash_table_lookup(location, "station_time_zone")));
        else
            diff_time = 0;
        current_time += diff_time;
        current_day = current_time;
        tm = localtime(&current_day);
        year = 1900 + tm->tm_year;
        current_month = tm->tm_mon;
        tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
        tm->tm_isdst = 1;
        current_day = mktime(tm);

        memset(date_in_string, 0, sizeof(date_in_string));
        sprintf(date_in_string, "%s %i 00:00:00",
                (char*)g_hash_table_lookup(first_day, "day_date"), year);

        strptime(date_in_string, "%b %d %Y %T", tm);
        /* Check New Year */
        if((current_month == 0) && (tm->tm_mon == 11)){
            sprintf(date_in_string, "%s %i 00:00:00",
                    (char*)g_hash_table_lookup(first_day, "day_date"), year - 1);
            strptime(date_in_string, "%b %d %Y %T", tm);
        }
        date_time = mktime(tm);
        /* calculate days offset */
        pre_offset = (double)(abs(current_day - date_time));
        pre_offset /= 24.0F * 60.0F * 60.0F;
        offset = (int)( round(pre_offset) );

        if(offset >= Max_count_weather_day)
            offset = Max_count_weather_day;
    } else
        offset = Max_count_weather_day;
    return offset;
}
/*******************************************************************************/
/* Filling data of buttons  */
void 
draw_home_window(gint count_day){
    gint        i,
                offset = 0,
                year = 0,
                current_month = 1;
    gchar       buffer[2048],
                buffer_icon[1024],
                temperature_string[1024],
                forecast_string[2048];
    time_t      current_day,
                current_time,
                last_day = 0,
                count_of_show_button = 0,
                day_begin_time,
                night_begin_time;
    long int    diff_time = 0;
    struct tm   *tm = NULL,
                tmp_tm;
    gboolean    flag_last_day = FALSE,
                is_na_day = FALSE;
    gint        wind_direction = UNKNOWN_DIRECTION;
    gfloat      wind_speed = -1;
    gchar       *tmp_station_name;
    WDB         *tmp_button = NULL;
    GSList      *tmp = NULL;
    GHashTable  *day = NULL,
                *first = NULL,
                *last = NULL,
                *tmp_day = NULL,
                *day_hash = NULL,
                *location = NULL;
#ifndef RELEASE
    time_t      tmp_time,
                utc_time;
#endif
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* Check main widget */
    if(!app->top_widget)
        return;
    tm = &tmp_tm;
    memset(temperature_string, 0, sizeof(temperature_string));
    memset(forecast_string, 0, sizeof(forecast_string));

/* Clear daytime elments in queue of timer */
    remove_daytime_event();

    if(count_day > 0){
        /* get current day */
        current_time = time(NULL);
        location = g_hash_table_lookup(app->station_data, "location");
        if (location && g_hash_table_lookup(location, "station_time_zone"))
            diff_time = calculate_diff_time(atol(g_hash_table_lookup(location, "station_time_zone")));
        else
            diff_time = 0;
        current_time += diff_time;
        current_day = current_time;
        tm = localtime(&current_day);
        year = 1900 + tm->tm_year;
        current_month = tm->tm_mon;
        tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
        tm->tm_isdst = 1;
        current_day = mktime(tm);

        offset = calculate_offset_of_day(count_day);
        /* delete old weather data */
        tmp = g_hash_table_lookup(app->station_data, "forecast");
        i = 0;
        while(tmp && i < offset){
            day = (GHashTable*)tmp->data;
            tmp = g_slist_remove(tmp, day);
            g_hash_table_foreach(day, free_fields, NULL);
            g_hash_table_destroy(day);
            i++;
        }
        g_hash_table_remove(app->station_data, "forecast");
        g_hash_table_insert(app->station_data, "forecast", tmp);

        if(tmp){
            first = (GHashTable*)(tmp->data);
            last = (GHashTable*)((g_slist_last(tmp))->data);
        }
    }
    /* Init popup Context menu */
    menu_init();
    i = 0;
    switch (app->config->icons_layout){
        case PRESET_NOW:
            count_of_show_button = 1;
            break;
        case PRESET_NOW_PLUS_TWO:
            count_of_show_button = 3;
            break;
        case PRESET_NOW_PLUS_THREE_H:
            count_of_show_button = 4;
            break;
        case PRESET_NOW_PLUS_THREE_V:
            count_of_show_button = 4;
            break;
        case PRESET_NOW_PLUS_SEVEN:
            count_of_show_button = 8;
            break;
 
        default:
            count_of_show_button = app->config->days_to_show;
    }
#ifdef APPLICATION
     count_of_show_button = app->config->days_to_show;
#endif
    while(i < count_of_show_button){
        if(tmp){
            day = (GHashTable*)tmp->data;
            /* add CHANGE_DAY_PART for all days */
            add_change_day_part_event(day, year, current_month);
            /* day begin */
            day_begin_time = get_day_part_begin_time(day, year, "day_sunrise");
            /* night begin */
            night_begin_time = get_day_part_begin_time(day, year, "day_sunset");
            /* add events for first day */
            if(current_time < day_begin_time)
                event_add(day_begin_time - diff_time, CHANGE_DAY_PART);
            if(current_time < night_begin_time)
                event_add(night_begin_time - diff_time, CHANGE_DAY_PART);

            #ifndef RELEASE
            fprintf(stderr, "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
            fprintf(stderr, "\nUTC time %s", ctime(&utc_time));
            fprintf(stderr, "Zone time %s", ctime(&current_time));
            tmp_time = last_update_time_new(g_hash_table_lookup(app->station_data, "current"));
            fprintf(stderr, "Last update: %s", ctime(&tmp_time));
            fprintf(stderr, "Current time: %s", ctime(&current_time));
            fprintf(stderr, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
            #endif

            if(i == 0){	/* first day */
                /* check weather data for actuality */
                if(app->current_is_valid){
                    /* add event for terminate valid period */
                    if ((g_hash_table_lookup(app->station_data, "current"))){
                        day_hash = g_hash_table_lookup(app->station_data, "current");
                        event_add(last_update_time_new(day_hash) + app->config->data_valid_interval - diff_time, CHANGE_DAY_PART);
                        buffer[0] = 0;
                        /* For only &wind_direction, &wind_speed parameters */ 
                        create_wind_parameters(day_hash, NULL,FALSE, &wind_direction, &wind_speed);
                        create_current_temperature_text(day_hash, buffer, TRUE, g_hash_table_lookup(day, "day_name"));
                        sprintf(buffer_icon, "%s%s.png", app->config->icons_set_base, (char*)
                            g_hash_table_lookup(day_hash, "icon"));
                    }
                }
                else{ /* if current data is not actual */
                    buffer[0] = 0;
                    /* if current data isn't actual and first day */
                    if(app->config->separate && app->config->icons_layout < PRESET_NOW ){
                        create_current_temperature_text(g_hash_table_lookup(app->station_data, "current"),
                                                        buffer, FALSE, g_hash_table_lookup(day, "day_name"));
                        sprintf(buffer_icon, "%s48.png", app->config->icons_set_base);
                    }
                    else{ /* if first day and not config->separate data */
                          /* if current time is night show night icon */
                         if(current_time > night_begin_time || current_time < day_begin_time){
                            create_day_temperature_text(day, buffer, FALSE, FALSE, FIRST_BUTTON);
                            /* displaying wind if necessary */
                            if(app->config->show_wind || app->config->icons_layout >= PRESET_NOW)
                                create_wind_parameters(day, buffer + strlen(buffer),FALSE,
                                                                        &wind_direction, &wind_speed);
                            sprintf(buffer_icon, "%s%s.png", app->config->icons_set_base,
                                    (char*)g_hash_table_lookup(day, "night_icon"));
                         }
                         else{
                            create_day_temperature_text(day, buffer, FALSE, FALSE, FIRST_BUTTON);
                            /* displaying wind if necessary */
                            if(app->config->show_wind || app->config->icons_layout >= PRESET_NOW)
                                create_wind_parameters(day, buffer + strlen(buffer),TRUE, 
                                                                        &wind_direction, &wind_speed);
                            sprintf(buffer_icon, "%s%s.png", app->config->icons_set_base,
                                    (char*)g_hash_table_lookup(day, "day_icon"));
                         }
                    }
                }
            }
            else{/* other days, from two and to app->config->days_to_show */
                buffer[0] = 0;
                if (app->config->separate && i == 1){
                    tmp_day = first;
                    /* Change day and night begin time */
                    day_begin_time = get_day_part_begin_time(tmp_day, year, "24h_sunrise");
                    night_begin_time = get_day_part_begin_time(tmp_day, year, "24h_sunset");
                }else
                    tmp_day = day;
                    create_day_temperature_text(tmp_day, buffer, FALSE, FALSE, OTHER_BUTTON);
                if((app->config->separate) && (i == 1) &&
                   (current_time > night_begin_time || current_time < day_begin_time)){
                       if(app->config->show_wind || app->config->icons_layout >= PRESET_NOW)
                           create_wind_parameters(tmp_day, buffer + strlen(buffer),FALSE, &wind_direction, &wind_speed);
                       sprintf(buffer_icon, "%s%s.png", app->config->icons_set_base,
                                (char*)g_hash_table_lookup(tmp_day, "night_icon"));
                }
                else{
                    if(app->config->show_wind || app->config->icons_layout >= PRESET_NOW)
                        create_wind_parameters(tmp_day, buffer + strlen(buffer),TRUE, &wind_direction, &wind_speed);
                    sprintf(buffer_icon, "%s%s.png", app->config->icons_set_base,
                            (char*)g_hash_table_lookup(tmp_day, "day_icon"));
                }
            }
        }
        else{ /* Show N/A for all others day buttons when it not inside range */
            is_na_day = TRUE;
            wind_speed = -1;
            wind_direction = UNKNOWN_DIRECTION;
            if(app->config->show_wind){
                if (!app->config->is_application_mode && app->config->icons_layout >= PRESET_NOW){
                    sprintf(buffer, "<span foreground='%s'>%s\302\260</span>",
                        PRESET_BIG_FONT_COLOR_FRONT,
                        _("N/A"));
                }else{
                    sprintf(buffer, "<span foreground='#%02x%02x%02x'>%s\n%s\302\260\n%s\302\260\n%s\n%s</span>",
                        app->config->font_color.red >> 8,
                        app->config->font_color.green >> 8,
                        app->config->font_color.blue >> 8,
                        _("N/A"), _("N/A"), _("N/A"),
                        _("N/A"), _("N/A"));
                }
            }
            else{
                if (!app->config->is_application_mode && app->config->icons_layout >= PRESET_NOW){
                    sprintf(buffer, "<span foreground='%s'>%s\302\260</span>",
                        PRESET_BIG_FONT_COLOR_FRONT,
                        _("N/A"));
                }else{
                    sprintf(buffer, "<span foreground='#%02x%02x%02x'>%s\n%s\302\260\n%s\302\260</span>",
                        app->config->font_color.red >> 8,
                        app->config->font_color.green >> 8,
                        app->config->font_color.blue >> 8,
                        _("N/A"), _("N/A"), _("N/A"));
                }
            }
            sprintf(buffer_icon, "%s48.png", app->config->icons_set_base);
            if(!flag_last_day && last_day){
                event_add(last_day + 24 * 60 * 60, CHANGE_DAY_PART);
                flag_last_day = TRUE;
            }
        }
    #if defined(NONMAEMO) || defined (APPLICATION)
        if(i == 0)/* First icon in APPLICATION MODE */
    #else
        /* First icon in COMBINATION layout or Presets layouts */
        if((app->config->icons_layout == COMBINATION && i == 0) ||
           (app->config->icons_layout >= PRESET_NOW && i == 0))
    #endif
            tmp_button = create_weather_day_button(buffer, buffer_icon, FIRST_BUTTON,
                                app->config->transparency,
                                FALSE,
                                &(app->config->background_color), wind_direction, wind_speed);
        else
            tmp_button = create_weather_day_button(buffer, buffer_icon, OTHER_BUTTON,
                                app->config->transparency,
                                TRUE,
                                &(app->config->background_color), wind_direction, wind_speed);
        g_signal_connect(tmp_button->button, "button-release-event",
                    G_CALLBACK(weather_window_popup),
                    (is_na_day ? (GINT_TO_POINTER(-1)) : (GINT_TO_POINTER(i))));
        if (app->config->clicking_type == SHORT_CLICK)
            g_signal_connect(tmp_button->button, "button-release-event",
                                G_CALLBACK(weather_window_popup),
                                (is_na_day ? (GINT_TO_POINTER(-1)) : (GINT_TO_POINTER(i))));
        add_item2object(&(app->buttons), (void*)tmp_button);

        if(!(app->config->separate && i == 1))
            tmp = g_slist_next(tmp);
            i++;
    }/* end while */
#if defined(OS2008) || defined(DEBUGTEMP)
    if(app->config->use_sensor &&
		    app->config->display_at == ICON)/* draw sensor data at the new icon */
	add_item2object(&(app->buttons),
			create_sensor_icon_widget(app->config->transparency,
						    TRUE,
						    &(app->config->background_color)));
#endif
    if(app->config->current_station_id)
        tmp_station_name = app->config->current_station_name;
    else
        tmp_station_name = NULL;
/* creating main panel */
    app->main_window = gtk_table_new(2, 3, FALSE);

#if defined CLUTTER
   g_signal_connect_after(app->main_window, "expose-event",
      G_CALLBACK(top_widget_expose), NULL);
#endif

#if defined(NONMAEMO) || defined (APPLICATION)
    create_panel(app->main_window, APPLICATION_MODE,
                 app->config->transparency, tmp_station_name); 
    gtk_table_attach(GTK_TABLE(app->main_window),
            create_time_updates_widget(g_hash_table_lookup(app->station_data, "current"), TRUE),
             0, 1, 2, 3, GTK_EXPAND, GTK_EXPAND, 0, 0);
#else
    create_panel(app->main_window, app->config->icons_layout,
                 app->config->transparency, tmp_station_name);
#endif
   gtk_box_pack_start(GTK_BOX(app->top_widget), app->main_window, TRUE, TRUE, 0);
   gtk_widget_show_all(app->top_widget);

    #if defined OS2008 || defined OS2009
/*
       if(!app->config->transparency && app->parent){
           gtk_widget_modify_bg(app->parent, GTK_STATE_NORMAL, &app->config->background_color);
       }
*/
    #endif
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void 
redraw_home_window(gboolean first_start){
    gint            count_day = -2;
    GSList          *tmp = NULL;
    gint            (*parser)(const gchar*, GHashTable*, gboolean);
    WDB             *tmp_button = NULL;
    gchar           buffer[255];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifndef RELEASE
    fprintf(stderr, "\nDays current %d\n", app->config->days_to_show);
    fprintf(stderr, "\nDays previos %d\n", app->config->previos_days_to_show);
#endif
    if(!first_start){
        /* delete previos station data */
        if(app->station_data){
            free_main_hash_table(app->station_data);
            /* free station details data */
            free_detaild_hash_table(app->station_data);
            g_hash_table_remove_all(app->station_data);
        }
        /* free days buttons */
        tmp = app->buttons;
        while(tmp){
            tmp_button = (WDB*)tmp->data;
            if (tmp_button){
               delete_weather_day_button(&tmp_button);
               tmp_button = NULL;
            }
            tmp = g_slist_next(tmp);
        }
        g_slist_free(app->buttons);
        app->buttons = NULL;
    }
#ifdef CLUTTER
    free_clutter_objects_list(&app->clutter_objects_in_main_form);
#endif
    if (app->main_window)
        gtk_widget_show_all(app->main_window);
    if(app->main_window){
        gtk_widget_destroy(app->main_window);
        app->main_window = NULL;
    }
    /* Check nill station_id */
    if (!!app->config->current_station_id 
        || (!strcmp(app->config->current_station_id," "))
        || (!strcmp(app->config->current_station_id,"")))
        count_day = -1;
    else{
        /* new parser */
        parser = get_source_parser(app->sources_list,
                                    app->config->current_station_source);
        if(parser){
            /* prepare xml file with full path */
            *buffer = 0;
            snprintf(buffer, sizeof(buffer) - 1, "%s/%s.xml",
                        app->config->cache_dir_name, app->config->current_station_id);
            count_day = parser(buffer, app->station_data, FALSE);
            /* check current weather */
            app->current_is_valid = is_current_weather_valid();
            #ifndef RELEASE
                if(app->current_is_valid)
                    fprintf(stderr, "\n>>>>>>>>>>>>>>>>>>Current is valid\n");
                else
                    fprintf(stderr, "\n>>>>>>>>>>>>>>>>>>Current is non valid\n");
            #endif
            /* detail data */
            if(app->config->show_weather_for_two_hours){
                *buffer = 0;
                snprintf(buffer, sizeof(buffer) - 1, "%s/%s_hour.xml",
                            app->config->cache_dir_name, app->config->current_station_id);
                parser(buffer, app->station_data, TRUE);
           }
        }
        if(count_day == -2){
            fprintf(stderr, _("Error in xml file\n"));
            hildon_banner_show_information(app->main_window,
                              NULL,
                              _("Wrong station code \nor ZIP code!!!"));
        } /* Error in xml file */
    }
    app->count_day = count_day;	/* store days number from xml file */
    draw_home_window(count_day);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
gboolean
remitted_update(void){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    redraw_home_window(FALSE);
    return FALSE;
}
/*******************************************************************************/
/* Get Weather xml file from weather.com */
void 
update_weather(gboolean show_update_window){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(show_update_window)
	app->show_update_window = TRUE;
    else
        app->show_update_window = FALSE;
    if(!app->flag_updating)
	app->flag_updating = g_timeout_add(100, (GSourceFunc)download_html, NULL);
}
/*******************************************************************************/

#if defined(NONMAEMO) || defined (APPLICATION)
gboolean
omweather_init_OS2009(GtkWidget *applet){
    GtkWidget *main_vbox, *main_hbox;
#elif OS2008
void
omweather_init(OMWeather *applet){
    GtkSettings *settings;
    GdkColormap *cm;
#elif OS2009
static void
omweather_plugin_init (OmweatherDesktopWidget *applet){
    HDHomePluginItem *myparent;
#else
void*
hildon_home_applet_lib_initialize(void *state_data, int *state_size,
					GtkWidget **widget){
#endif
    gchar       tmp_buff[2048];
#if ! (defined (OS2009) ||  defined (NONMAEMO) ||  defined (APPLICATION))
    osso_context_t	*osso = NULL;
    osso = osso_initialize(PACKAGE, VERSION, TRUE, NULL);
    if(!osso){
        g_debug(_("Error initializing the OMWeather applet"));
#if defined(NONMAEMO) || defined (APPLICATION)
        return FALSE;
#elif defined OS2008 || defined OS2009
        return;
#else
        return NULL;
#endif
    }
#endif
/* Checking noomweather.txt file */
    if ((access("/media/mmc1/noomweather.txt", R_OK) == 0)||
        (access("/media/mmc2/noomweather.txt", R_OK) == 0))
#if defined(NONMAEMO) || defined (APPLICATION)
       return FALSE;
#elif defined (OS2008) || defined (OS2009)
       return;
#else
       return NULL;
#endif

    app = g_new0(OMWeatherApp, 1);
    if(!app){
        fprintf(stderr, "\nCan not allocate memory for applet.\n");
        exit(1);
    }
    memset(app, 0, sizeof(OMWeatherApp));
#if ! (defined (OS2009) ||  defined (NONMAEMO) ||  defined (APPLICATION))
    app->osso = osso;
#endif
    app->flag_updating = 0;
/* create i18n hash for values coming from xml file */
    app->hash = hash_table_create();
    app->dbus_is_initialize = FALSE;
/* prepare config struct */
    app->config = g_new0(AppletConfig, 1);
    if(!app->config){
        fprintf(stderr, "\nCan not allocate memory for config.\n");
        g_free(app);
#if  defined (NONMAEMO) ||  defined (APPLICATION)
        return FALSE;
#elif OS2008 
        return;
#elif OS2009 
        return;
#else
        return NULL;
#endif
    }
/* list of user selected stations */
    app->user_stations_list = create_user_stations_list();
/* list of user selected alerts */
/*    app->user_alerts_list = create_user_alerts_list();*/
    #ifdef USE_CONIC
       app->connection = NULL;
    #endif
#ifdef CLUTTER
    /* Init clutter */
    if (gtk_clutter_init (NULL, NULL) != CLUTTER_INIT_SUCCESS)
        fprintf (stderr,"Unable to initialize GtkClutter");
    /* Fix ME added config value */
    app->clutter_script = NULL;
#endif
    app->clutter_objects_in_main_form = NULL;
    app->clutter_objects_in_popup_form = NULL;

/* Init gconf. */
    gnome_vfs_init();
    if(read_config(app->config)){
        fprintf(stderr, "\nCan not read config file.\n");
        g_free(app->config);
        g_free(app);
#if  defined (NONMAEMO) || defined (APPLICATION)
    return FALSE;
#elif OS2008
        return;
#elif OS2009
        return;
#else
    return NULL;
#endif
    }
    /* Initialize DBUS */
    weather_initialize_dbus(); /* TODO connect this function with app->dbus_is_initialize */

    app->sources_list = create_sources_list(SOURCESPATH, &app->sources_number, &app->handles);
    app->station_data = g_hash_table_new(g_str_hash, g_str_equal);
    app->time_update_list = create_time_update_list();
    app->show_update_window = FALSE;
    app->popup_window = NULL;
    app->settings_window = NULL;
    app->contextmenu = NULL;
    app->tab_of_window_popup = NULL;
    app->dbus_conn = NULL;
    app->current_is_valid = FALSE;
/* Start timer */
    timer(60000);  /* One per minute */
/* Start main applet */
    app->top_widget = gtk_hbox_new(FALSE, 0);
#if  defined(NONMAEMO) || defined (APPLICATION)
    /* pack for window in Application mode */
    app->config->is_application_mode = TRUE;
    main_vbox = gtk_vbox_new(FALSE, 0);
    main_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_vbox), gtk_alignment_new(0.5, 0.5, 1, 1), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_vbox), main_hbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_hbox), gtk_alignment_new(0.5, 0.5, 1, 1), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_hbox), app->top_widget, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_hbox), gtk_alignment_new(0.5, 0.5, 1, 1), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_vbox), gtk_alignment_new(0.5, 0.5, 1, 1), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_vbox), create_toolbar_box(omweather_destroy, applet, TRUE), FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(applet), main_vbox);
#else
    app->config->is_application_mode = FALSE;
#endif
#if !defined OS2008 ||  defined (APPLICATION)
    redraw_home_window(TRUE);
#endif
#if defined(OS2008) || defined(OS2009) || defined(NONMAEMO)
#if defined OS2008 && !defined (APPLICATION)
    applet->queueRefresh = TRUE;
#endif

#ifdef ENABLE_GPS
    app->gps_device = NULL;
    initial_gps_control();
/* Hack for OS2009 */
    initial_gps_connect();
#endif

    app->widget_first_start = TRUE;

#if defined OS2008  || ! defined (APPLICATION)
    gtk_widget_set_name(GTK_WIDGET(app->top_widget), PACKAGE_NAME);
#endif
    snprintf(tmp_buff, sizeof(tmp_buff) - 1, "%s/%s",
                    STYLEPATH, "style.rc");
    gtk_rc_parse(tmp_buff);
#if defined OS2008 && !defined (APPLICATION)
    applet->priv = G_TYPE_INSTANCE_GET_PRIVATE(applet, TYPE_OMWEATHER, OMWeatherPrivate);
    settings = gtk_settings_get_default();
    cm = gdk_screen_get_rgba_colormap(gdk_screen_get_default());
    if(cm)
      gtk_widget_set_colormap(GTK_WIDGET(applet), cm);
#endif
#if defined OS2009 && !defined(APPLICATION)
//  applet->priv = OMWEATHER_PLUGIN_GET_PRIVATE (applet);
    hd_home_plugin_item_set_settings (HD_HOME_PLUGIN_ITEM (applet), TRUE);
    g_signal_connect (applet, "show-settings",
        G_CALLBACK (weather_window_settings), NULL);
#endif 

#if  defined(NONMAEMO) || defined (APPLICATION)
    return TRUE;
#else
    gtk_container_add(GTK_CONTAINER(applet), app->top_widget);
#endif

#else
    (*widget) = app->top_widget;
    return (void*)osso;
#endif
}
/*******************************************************************************/
#if !defined(OS2008) && !defined(OS2009)
int 
hildon_home_applet_lib_save_state(void *raw_data, void **state_data, 
								int *state_size){
    (*state_data) = NULL;
    if(state_size)
	(*state_size) = 0;
    return 1;
}
/*******************************************************************************/
void 
hildon_home_applet_lib_background(void *raw_data){
}
/*******************************************************************************/
void 
hildon_home_applet_lib_foreground(void *raw_data){
}
#endif
/*******************************************************************************/
#if defined OS2008 || defined OS2009 || defined APPLICATION || defined NONMAEMO
void 
omweather_destroy(GtkObject *widget){
#else
void 
hildon_home_applet_lib_deinitialize(void *applet_data){
#endif
    osso_context_t *osso = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#if defined OS2008 || !defined (APPLICATION)
    if(!app)
        return;
#endif
    /* remove switch timer */
    if(app->switch_timer > 0)
        g_source_remove(app->switch_timer);
    /* It is switch off the timer */	
    if(app->timer > 0)
        g_source_remove(app->timer);
    /* Clean the queue of event */ 
    free_list_time_event();
#ifdef CLUTTER
    /* Clean clutter objects list */
    free_clutter_objects_list(&app->clutter_objects_in_main_form);
    free_clutter_objects_list(&app->clutter_objects_in_popup_form);
#endif
    /* If downloading then switch off it */
    if(app->flag_updating != 0){
        g_source_remove(app->flag_updating);
        clean_download();
    }
    if(app->timer_for_os2008 != 0)
        g_source_remove(app->timer_for_os2008);
    config_save(app->config); /* Not work!!!! Only 770. Why? I am not understand why this place not run when close applet 
			On n800 this work */

    /* destroy popup */
    destroy_popup_window(NULL);
    /* destroy settings window */
    if (app->settings_window){
        gtk_widget_destroy(app->settings_window);
        app->settings_window = NULL;
    }
#if defined OS2008 && ! defined APPLICATION
    g_signal_handler_disconnect(app->parent,app->signal_size_request);
    g_signal_handler_disconnect(app->parent_parent,app->signal_press);
    g_signal_handler_disconnect(app->parent_parent,app->signal_release);
    g_signal_handler_disconnect(app->parent,app->my_applet_signal_release);
    /* disconnected from HildonDesktopHomeItem expose-event */
    g_signal_handler_disconnect(app->parent,app->signal_item_expose);
    g_signal_handler_disconnect(app->parent_parent,app->signal_area_changed);
    /* disconnected from HildonHomeArea expose-event */
    g_signal_handler_disconnect(app->parent_parent,app->signal_area_expose);
    g_signal_handler_disconnect(app->top_widget,app->signal_expose);
    if(app->pixbuf){
        gdk_pixbuf_unref(app->pixbuf);
        app->pixbuf = NULL;
    }
#endif

#ifdef OS2008
    /* remove sensor time */
    if(app->sensor_timer > 0)
        g_source_remove(app->sensor_timer);
#endif
#ifdef ENABLE_GPS
    deinitial_gps_control();
#endif
#ifdef USE_DBUS
    weather_deinitialize_dbus();
#endif
#if !(defined OS2008 || defined OS2009 || defined APPLICATION || defined NONMAEMO)
    osso = (osso_context_t*)applet_data;
#endif
    if(app){
        app->top_widget = NULL;
        app->main_window = NULL;
        free_memory();
        if(app->config)
            g_free(app->config);

        if(app->handles){
            unload_parsers(app->handles);
            g_slist_free(app->handles);
            app->handles = NULL;
        }
        if(app->time_update_list){
            gtk_list_store_clear(app->time_update_list);
            g_object_unref(app->time_update_list);
        }
        if(app->user_stations_list){
            gtk_list_store_clear(app->user_stations_list);
            g_object_unref(app->user_stations_list);
        }
        if(app->station_data){
            g_hash_table_remove_all(app->station_data);
            g_hash_table_destroy(app->station_data);
        }
    }
    /* Deinitialize libosso */
    osso_deinitialize(osso);

    if(app){
        g_free(app);
        app = NULL;
    }
#if defined OS2008 && ! defined APPLICATION
    gtk_object_destroy(widget);
#elif defined APPLICATION  || defined NONMAEMO
    gtk_main_quit();
#endif
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
GtkWidget* 
hildon_home_applet_lib_settings(void *applet_data, GtkWindow *parent){
    GtkWidget    *menu_item;

#ifndef RELEASE
    fprintf(stderr, "\nOMWeather applet settings\n");
#endif

    menu_item = gtk_menu_item_new_with_label(_("OMWeather settings"));
    g_signal_connect(menu_item, "activate",G_CALLBACK(weather_window_settings), NULL);

    return menu_item;
}
/*******************************************************************************/
void 
menu_init(void){
    GtkWidget	*menu_item = NULL;
    GtkTreeIter	iter;
    gboolean	valid;
    gchar	*station_name = NULL,
		*station_code = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->contextmenu = gtk_menu_new();
    /* About item */
    menu_item = gtk_menu_item_new_with_label(_("About"));
    g_signal_connect(G_OBJECT(menu_item), "activate",
                        G_CALLBACK(about_button_handler), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(app->contextmenu), menu_item);
    /* Setting item */
    menu_item = gtk_menu_item_new_with_label(_("Settings"));
    g_signal_connect(G_OBJECT(menu_item), "activate",
                        G_CALLBACK(weather_window_settings), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(app->contextmenu), menu_item);
    /* Full forecast item*/
    menu_item = gtk_menu_item_new_with_label(_("Full forecast"));
    g_signal_connect(G_OBJECT(menu_item), "activate",
                        G_CALLBACK(weather_window_popup), (GINT_TO_POINTER(-1)));
    gtk_menu_shell_append(GTK_MENU_SHELL(app->contextmenu), menu_item);

    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                                  &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter,
                            NAME_COLUMN, &station_name,
                            ID0_COLUMN, &station_code,
                            -1);
    if(station_name){
        gtk_menu_shell_append(GTK_MENU_SHELL(app->contextmenu),
            menu_item = gtk_menu_item_new_with_label(station_name));
        g_free(station_code);
        station_code = NULL;
    }
        g_signal_connect(G_OBJECT(menu_item), "activate",
                    	    G_CALLBACK(change_station_select), station_name);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                                        &iter);
    }
    gtk_widget_show_all(GTK_WIDGET(app->contextmenu));
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
/* For Combination layout */
GtkWidget* 
create_forecast_weather_simple_widget(GHashTable *day){
    GtkWidget       *temperature_label = NULL,
                    *main_data_vbox = NULL,
                    *main_data_label = NULL,
                    *main_widget = NULL,
                    *temperature_vbox = NULL;
    gchar           buffer[1024];
    time_t          day_begin_time,
                    night_begin_time,
                    current_time;
    gint            year;
    struct tm       *tm;
    const gchar     *wind_units_str[] = { "m/s", "km/h", "mi/h" };
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!day)
        return NULL;
    /* prepare forecast temperature */
    temperature_vbox = gtk_vbox_new(FALSE, 0);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>",
                app->config->font_color.red >> 8,
                app->config->font_color.green >> 8,
                app->config->font_color.blue >> 8);
    sprintf(buffer + strlen(buffer), "%s</span>", _("Forecast: \n"));
    create_day_temperature_text(day, buffer + strlen(buffer), TRUE, TRUE, OTHER_BUTTON);
    temperature_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(temperature_label), buffer);
    gtk_label_set_justify(GTK_LABEL(temperature_label), GTK_JUSTIFY_CENTER);

    set_font(temperature_label, app->config->font, -1);
    gtk_box_pack_start(GTK_BOX(temperature_vbox), temperature_label, FALSE, FALSE, 0);

    current_time = time(NULL);
    tm = localtime(&current_time);
    year = 1900 + tm->tm_year;
    /* day begin */
    day_begin_time = get_day_part_begin_time(day, year, "day_sunrise");
    /* night begin */
    night_begin_time = get_day_part_begin_time(day, year, "day_sunset");

    /* prepare "title" "humidity", "wind", "gust" */
    main_data_label = gtk_label_new(NULL);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>",
                app->config->font_color.red >> 8,
                app->config->font_color.green >> 8,
                app->config->font_color.blue >> 8);
    if((current_time > day_begin_time) && (current_time < night_begin_time)){
        strcat(buffer, (gchar*)hash_table_find(g_hash_table_lookup(day, "day_title"), FALSE));
        strcat(buffer, _("\nHumidity: "));
        if(strcmp(g_hash_table_lookup(day, "day_humidity"), "N/A"))
            sprintf(buffer + strlen(buffer), "%s%%\n",
                    (char*)g_hash_table_lookup(day, "day_humidity"));
        else
            sprintf(buffer + strlen(buffer), "%s\n",
                (gchar*)hash_table_find("N/A", FALSE));
        strcat(buffer, _("Wind: "));
        sprintf(buffer + strlen(buffer), "%s %.2f %s",
              (gchar*)hash_table_find(g_hash_table_lookup(day, "day_wind_title"), TRUE),
        convert_wind_units(app->config->wind_units,
                atof(g_hash_table_lookup(day, "day_wind_speed"))),
                (gchar*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    }
    else{
        strcat(buffer, (gchar*)hash_table_find(g_hash_table_lookup(day, "night_title"), FALSE));
        strcat(buffer, _("\nHumidity: "));
        if(strcmp(g_hash_table_lookup(day, "night_humidity"), "N/A"))
            sprintf(buffer + strlen(buffer), "%s%%\n",
                    (char*)g_hash_table_lookup(day, "night_humidity"));
        else
            sprintf(buffer + strlen(buffer), "%s\n", (gchar*)hash_table_find("N/A", FALSE));
        strcat(buffer, _("Wind: "));
        sprintf(buffer + strlen(buffer), "%s %.2f %s", (char*)g_hash_table_lookup(day, "night_wind_title"),
        convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))),
            (gchar*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    }
    strcat(buffer, "</span>");
    gtk_label_set_markup(GTK_LABEL(main_data_label), buffer);
    set_font(main_data_label, app->config->font, -1);

    main_data_vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_data_vbox), main_data_label, FALSE, FALSE, 0);
        /* prepare main widget */
    main_widget = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_widget), temperature_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_widget), main_data_vbox, FALSE, FALSE, 0);

    return main_widget;
}
/*******************************************************************************/
GtkWidget* 
create_current_weather_simple_widget(GHashTable *current){
    GtkWidget       *main_widget = NULL,
                    *temperature_vbox,
                    *temperature_label,
                    *main_data_vbox,
                    *main_data_label,
                    *icon_temperature_vbox;
    gchar           buffer[1024],
                    *units;
    const gchar     *wind_units_str[] = { "m/s", "km/h", "mi/h" };
    float           tmp_distance = 0.0f,
                    tmp_pressure = 0.0f;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
        return NULL;
/* prepare current temperature */
    temperature_vbox = gtk_vbox_new(FALSE, 0);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>",
				app->config->font_color.red >> 8,
				app->config->font_color.green >> 8,
				app->config->font_color.blue >> 8);
    
    sprintf(buffer + strlen(buffer), _("Now: "));
    sprintf(buffer + strlen(buffer), "\n%d\302\260",
		((app->config->temperature_units == CELSIUS) ? ( atoi(g_hash_table_lookup(current, "day_hi_temperature")))
							: ((int)c2f(atof(g_hash_table_lookup(current, "day_hi_temperature"))))));
    (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C")) )
						: ( strcat(buffer, _("F")) );
    strcat(buffer,"</span>");
    temperature_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(temperature_label), buffer);
    gtk_label_set_justify(GTK_LABEL(temperature_label), GTK_JUSTIFY_CENTER);
    set_font(temperature_label, app->config->font, 2);

    gtk_box_pack_start(GTK_BOX(temperature_vbox), temperature_label, FALSE, FALSE, 0);
    
/* prepare "feels like", "visible", "pressure", "humidity", "wind", "gust" */
/* feels like */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>",
				app->config->font_color.red >> 8,
				app->config->font_color.green >> 8,
				app->config->font_color.blue >> 8);

    strcat(buffer, hash_table_find(g_hash_table_lookup(current, "title"), FALSE));
    strcat(buffer, _("\nFL: "));
    sprintf(buffer + strlen(buffer), "%d\302\260", 
	    (app->config->temperature_units == CELSIUS) ? (atoi(g_hash_table_lookup(current, "feel_like"))) 
						: ((int)c2f(atof(g_hash_table_lookup(current, "feel_like")))));
    (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C")) )
					: ( strcat(buffer, _("F")) );
/* humidity */
    strcat(buffer, _(" H: "));
    if( strcmp(g_hash_table_lookup(current, "humidity"), "N/A") )
	sprintf(buffer + strlen(buffer), "%d%%",
            atoi(g_hash_table_lookup(current, "humidity")));
    else
	sprintf(buffer + strlen(buffer), "%s",
		    (char*)hash_table_find("N/A", FALSE));
/* visible */
    strcat(buffer, _("\nV: "));
    if( !strcmp(g_hash_table_lookup(current, "humidity"), "Unlimited") )
        sprintf(buffer + strlen(buffer), "%s",
                (char*)hash_table_find("Unlimited", FALSE));
    else
        if( strcmp(g_hash_table_lookup(current, "visible"), "N/A") ){
            tmp_distance = atof(g_hash_table_lookup(current, "visible"));
            switch(app->config->distance_units){
                default:
                case METERS: units = _("m"); tmp_distance *= 1000.0f; break;
                case KILOMETERS: units = _("km"); tmp_distance *= 1.0f; break;
                case MILES: units = _("mi"); tmp_distance /= 1.609344f; break;
                case SEA_MILES: units = _("mi"); tmp_distance /= 1.852f; break;
            }
            sprintf(buffer + strlen(buffer), "%.2f %s", tmp_distance, units);
        }
        else
            sprintf(buffer + strlen(buffer), "%s",
                    (char*)hash_table_find("N/A", FALSE));
/* pressure */
    tmp_pressure = atof(g_hash_table_lookup(current, "pressure"));
    switch(app->config->pressure_units){
        default:
        case MB: units = _("mb"); break;
        case INCH: units = _("inHg"); tmp_pressure = mb2inch(tmp_pressure); break;
        case MM: units = _("mmHg"); tmp_pressure = mb2mm(tmp_pressure); break;
    }
    strcat(buffer, _("\nP: "));
    sprintf(buffer + strlen(buffer), "%.2f %s, ", tmp_pressure, units);
    strcat(buffer, hash_table_find(g_hash_table_lookup(current, "pressure_direction"), FALSE));
/* wind */
    strcat(buffer, _("\nW: "));
    sprintf(buffer + strlen(buffer), "%s", (char*)hash_table_find(g_hash_table_lookup(current, "wind_direction"), FALSE));
    if( strcmp(g_hash_table_lookup(current, "wind_speed"), "N/A") )
        sprintf(buffer + strlen(buffer), " %.2f %s", 
            convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(current, "wind_speed"))),
            (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
/* gust */
    if( strcmp(g_hash_table_lookup(current, "wind_speed"), "N/A") ){
        strcat(buffer, _(" G: "));
    sprintf(buffer + strlen(buffer), "%.2f %s",
            convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(current, "wind_speed"))),
            (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    }
    strcat(buffer,"</span>");

    main_data_vbox = gtk_vbox_new(FALSE, 0);
    main_data_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(main_data_label), buffer);
    set_font(main_data_label, app->config->font, -1);
    gtk_box_pack_start(GTK_BOX(main_data_vbox), main_data_label, FALSE, FALSE, 0);
/* prepare icon and temperature vbox */
    icon_temperature_vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(icon_temperature_vbox), temperature_vbox, FALSE, FALSE, 0);
/* prepare main widget */
    main_widget = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_widget), icon_temperature_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_widget), main_data_vbox, FALSE, FALSE, 0);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return main_widget;
}
/*******************************************************************************/
/* create days panel and station name panel */
void 
create_panel(GtkWidget* panel, gint layout, gboolean transparency,
								gchar *st_name){
    gchar	buffer[2048];
    GtkWidget	*header_panel = NULL,
		*days_panel = NULL,
/*		*days_panel_with_buttons = NULL,*/
		*combination_vbox = NULL,
		*current_weather_widget = NULL,
		*previos_station_name_btn = NULL,
		*previos_station_name = NULL,
		*previos_station_box = NULL,
		*next_station_name_btn = NULL,
		*next_station_name = NULL,
		*next_station_box = NULL,
		*station_name_btn = NULL,
		*station_name = NULL,
		*station_box = NULL;

    int		n,
		elements,
		x,
		y,
		total_elements = 0;
    GtkTreeIter	iter;
    gboolean	valid = FALSE,
		user_stations_list_has_two_or_more_elements = FALSE;
    GSList	*tmp = NULL;


#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* calculate number of elements to display */
    total_elements = app->config->days_to_show;
#if defined(OS2008) || defined(DEBUGTEMP)
/* draw sensor data at the new icon */
    if(app->config->use_sensor && app->config->display_at == ICON)
        total_elements++;
#endif
    if(total_elements % 2)
	    elements = total_elements / 2 + 1;
    else
	    elements = total_elements / 2;
/* create header panel */
    header_panel = gtk_table_new(1, 3, FALSE);
/*    header_panel = gtk_hbox_new(FALSE, 0);*/
    if (layout < PRESET_NOW || layout == APPLICATION_MODE){
        /* check number of elements in user stations list */
        valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                                      &iter);
        if(valid && gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                &iter))
        user_stations_list_has_two_or_more_elements = TRUE;
        /* draw arrows */
        if(app->config->show_arrows && user_stations_list_has_two_or_more_elements){
        /* create previos station button */
        sprintf(buffer,
            "<span weight=\"bold\" size=\"large\" foreground='#%02x%02x%02x'>&lt;</span>",
            app->config->font_color.red >> 8,
            app->config->font_color.green >> 8,
            app->config->font_color.blue >> 8);
        previos_station_box = gtk_hbox_new(FALSE, 0);
        previos_station_name_btn = gtk_event_box_new();
        set_background_color(previos_station_name_btn, &(app->config->background_color));

        gtk_widget_set_events(previos_station_name_btn, GDK_BUTTON_PRESS_MASK);
        previos_station_name = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(previos_station_name), buffer);
        gtk_label_set_justify(GTK_LABEL(previos_station_name), GTK_JUSTIFY_CENTER);
        set_font(previos_station_name, app->config->font, -1);
        gtk_box_pack_start((GtkBox*) previos_station_box, previos_station_name, TRUE, TRUE, 15);
        gtk_container_add(GTK_CONTAINER(previos_station_name_btn), previos_station_box);

        buffer[0] = 0;
        /* create next station button */
        sprintf(buffer,
            "<span weight=\"bold\" size=\"large\" foreground='#%02x%02x%02x'>&gt;</span>",
            app->config->font_color.red >> 8,
            app->config->font_color.green >> 8,
            app->config->font_color.blue >> 8);
        next_station_box = gtk_hbox_new(FALSE, 0);
        next_station_name_btn = gtk_event_box_new();
        set_background_color(next_station_name_btn, &(app->config->background_color));

        gtk_widget_set_events(next_station_name_btn, GDK_BUTTON_PRESS_MASK);
        next_station_name = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(next_station_name), buffer);
        gtk_label_set_justify(GTK_LABEL(next_station_name), GTK_JUSTIFY_CENTER);
        set_font(next_station_name, app->config->font, -1);
            gtk_box_pack_start((GtkBox*) next_station_box, next_station_name, TRUE, TRUE, 15);
        gtk_container_add (GTK_CONTAINER(next_station_name_btn), next_station_box);
        }
        buffer[0] = 0;
        if(app->config->show_station_name){
        /* create station name button */
            if(!st_name)
            sprintf(buffer,
                "<span weight=\"bold\" size=\"large\" foreground='#%02x%02x%02x'>%s</span>",
                app->config->font_color.red >> 8,
                app->config->font_color.green >> 8,
                app->config->font_color.blue >> 8,
                (char*)hash_table_find("NO STATION", FALSE));
            else
    #if defined(OS2008) || defined(DEBUGTEMP)
            {
            if(app->config->use_sensor &&
                app->config->display_at == STATION_NAME)/* draw sensor data at the station name */
                sprintf(buffer,
                "<span weight=\"bold\" size=\"large\" foreground='#%02x%02x%02x'>%s (%.2f)</span>",
                    app->config->font_color.red >> 8,
                app->config->font_color.green >> 8,
                app->config->font_color.blue >> 8, st_name, app->sensor_data);
            else
    #endif
            sprintf(buffer,
                "<span weight=\"bold\" size=\"large\" foreground='#%02x%02x%02x'>%s</span>",
                    app->config->font_color.red >> 8,
                app->config->font_color.green >> 8,
                app->config->font_color.blue >> 8, st_name);
    #if defined(OS2008) || defined(DEBUGTEMP)
            }
    #endif
            station_box = gtk_hbox_new(FALSE, 0);
            station_name_btn = gtk_event_box_new();
            set_background_color(station_name_btn, &(app->config->background_color));
            gtk_widget_set_events(station_name_btn, GDK_BUTTON_PRESS_MASK);
            station_name = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(station_name), buffer);
            gtk_label_set_justify(GTK_LABEL(station_name), GTK_JUSTIFY_CENTER);
            gtk_widget_set_name(station_name, "station_name");

            if(layout == COMBINATION || layout == APPLICATION_MODE)
                set_font(station_name, app->config->font, 2);
            else
                set_font(station_name, app->config->font, -1);
            gtk_box_pack_start((GtkBox*)station_box, station_name, TRUE, TRUE, 0);
            gtk_container_add(GTK_CONTAINER(station_name_btn), station_box);
        }
#if defined OS2008 || defined OS2009
        if(previos_station_name_btn)
            gtk_event_box_set_visible_window(GTK_EVENT_BOX(previos_station_name_btn), FALSE);
        if(next_station_name_btn)
            gtk_event_box_set_visible_window(GTK_EVENT_BOX(next_station_name_btn), FALSE);
        if(station_name_btn)
            gtk_event_box_set_visible_window(GTK_EVENT_BOX(station_name_btn), FALSE);
#else
        /* check config->transparency */
        if(transparency){
            if(previos_station_name_btn)
                gtk_event_box_set_visible_window(GTK_EVENT_BOX(previos_station_name_btn), FALSE);
            if(next_station_name_btn)
                gtk_event_box_set_visible_window(GTK_EVENT_BOX(next_station_name_btn), FALSE);
            if(station_name_btn)
                gtk_event_box_set_visible_window(GTK_EVENT_BOX(station_name_btn), FALSE);
        }

#endif

        /*    days_panel_with_buttons = gtk_hbox_new(FALSE, 0);*/
        /* attach buttons to header panel */
        if(previos_station_name_btn)
        gtk_table_attach( (GtkTable*)header_panel,
                    previos_station_name_btn,
                    0, 1, 0, 1 , GTK_EXPAND, GTK_EXPAND, 0, 0);

        if(station_name_btn)
        /*	gtk_box_pack_start((GtkBox*)header_panel, station_name_btn, TRUE, TRUE, 0); */
        gtk_table_attach( (GtkTable*)header_panel,
                station_name_btn,
                1, 2, 0, 1  , GTK_EXPAND, GTK_EXPAND, 0, 0); 

        if(next_station_name_btn)
        gtk_table_attach( (GtkTable*)header_panel,
                    next_station_name_btn,
                    2, 3, 0, 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    }
/* create days panel */
    switch(layout){
        default:
        case ONE_ROW:
            days_panel = gtk_table_new(1, Max_count_weather_day, FALSE);
            break;
        case ONE_COLUMN:
            days_panel = gtk_table_new(Max_count_weather_day, 1, FALSE);
            break;
        case TWO_ROWS:
            days_panel = gtk_table_new(2, elements, FALSE);
            break;
        case TWO_COLUMNS:
            days_panel = gtk_table_new(elements, 2, FALSE);
            break;
        case COMBINATION:
        case APPLICATION_MODE:
            days_panel = gtk_table_new(Max_count_weather_day, 2, FALSE);
            break;
        case PRESET_NOW:
            days_panel = gtk_table_new(2, 1, FALSE);
            break;
        case PRESET_NOW_PLUS_TWO:
            days_panel = gtk_table_new(2, 3, FALSE);
            break;
        case PRESET_NOW_PLUS_THREE_H:
            days_panel = gtk_table_new(4, 2, FALSE);
            break; 
        case PRESET_NOW_PLUS_THREE_V:
            days_panel = gtk_table_new(3, 3, FALSE);
            break; 
        case PRESET_NOW_PLUS_SEVEN:
            days_panel = gtk_table_new(8, 2, FALSE);
            break; 

 }

/* add padding around the outside of the container so the text
 * is not right to the very edge */
    gtk_container_set_border_width(GTK_CONTAINER(days_panel),2);
    tmp = app->buttons;
    if (layout >= PRESET_NOW && layout < APPLICATION_MODE){
        /* attach days buttons for simple(presets) mode */
        switch(layout){
            case PRESET_NOW_PLUS_TWO:
                composition_now((WDB*)tmp->data, PRESET_NOW_PLUS_TWO);
                gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 0 + 2, 0, 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                if(tmp)
                    tmp = g_slist_next(tmp);
                if(tmp){
                        composition_left_vertical_day_button((WDB*)tmp->data, layout);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 0 + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                        tmp = g_slist_next(tmp);
                }
                if(tmp){
                        composition_right_vertical_day_button((WDB*)tmp->data, layout);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                1, 1 + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                }
                gtk_table_attach((GtkTable*)days_panel,
                                (GtkWidget*)next_station_preset_now(PRESET_NOW_PLUS_TWO),
                                0, 0 + 2, 2, 3, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
            break;
            case PRESET_NOW_PLUS_THREE_V:
                composition_now((WDB*)tmp->data, PRESET_NOW_PLUS_THREE_V);
                gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 0 + 3, 0, 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                if(tmp)
                    tmp = g_slist_next(tmp);
                if(tmp){
                        composition_left_vertical_day_button((WDB*)tmp->data, layout);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 0 + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                        tmp = g_slist_next(tmp);
                }
                if(tmp){
                        composition_central_vertical_day_button((WDB*)tmp->data);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                1, 1 + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                        tmp = g_slist_next(tmp);
                }
                if(tmp){
                        composition_right_vertical_day_button((WDB*)tmp->data, layout);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                2, 2 + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                }

                gtk_table_attach((GtkTable*)days_panel,
                                (GtkWidget*)next_station_preset_now(PRESET_NOW_PLUS_THREE_V),
                                0, 0 + 3, 2, 3, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

            break;
            case PRESET_NOW_PLUS_THREE_H:
                composition_now((WDB*)tmp->data, PRESET_NOW_PLUS_THREE_H);
                gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 0 + 1, 0, 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                if(tmp)
                    tmp = g_slist_next(tmp);
                 if(tmp){
                        composition_central_horizontal_day_button((WDB*)tmp->data);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                1, 1 + 1, 0, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                        tmp = g_slist_next(tmp);
                }

                if(tmp){
                        composition_central_horizontal_day_button((WDB*)tmp->data);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                2, 2 + 1, 0, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                        tmp = g_slist_next(tmp);
                }
                if(tmp){
                        composition_right_horizontal_day_button((WDB*)tmp->data);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                3, 3 + 1, 0, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                        tmp = g_slist_next(tmp);
                }


                gtk_table_attach((GtkTable*)days_panel,
                                (GtkWidget*)next_station_preset_now(PRESET_NOW_PLUS_THREE_H),
                                0, 0 + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

            break;
            case PRESET_NOW_PLUS_SEVEN:
                composition_now((WDB*)tmp->data, PRESET_NOW_PLUS_SEVEN);
                gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 0 + 1, 0, 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                if(tmp)
                    tmp = g_slist_next(tmp);
                for(n = 1; n < 7; n++){
                     if(tmp){
                            composition_central_horizontal_day_button((WDB*)tmp->data);
                            gtk_table_attach((GtkTable*)days_panel,
                                    ((WDB*)tmp->data)->button,
                                    n+0, n+1, 0, 2, (GtkAttachOptions)0,
                                    (GtkAttachOptions)0, 0, 0 );
                            tmp = g_slist_next(tmp);
                    }
                }
                if(tmp){
                        composition_right_horizontal_day_button((WDB*)tmp->data);
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                n+1, n+1 + 1, 0, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                        tmp = g_slist_next(tmp);
                }


                gtk_table_attach((GtkTable*)days_panel,
                                (GtkWidget*)next_station_preset_now(PRESET_NOW_PLUS_SEVEN),
                                0, 0 + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

            break;
 
            default:
            case PRESET_NOW:
                composition_now((WDB*)tmp->data, PRESET_NOW);
                gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 0 + 1, 0, 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                gtk_table_attach((GtkTable*)days_panel,
                                (GtkWidget*)next_station_preset_now(PRESET_NOW),
                                0, 0 + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
            break;
        }
    }else{
        /* attach days buttons for expert mode */
        for(n = 0, x = 0, y = 0; n < total_elements; n++, x++){
            if(tmp){
                switch(layout){
                    default:
                    case ONE_ROW:
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                n, n + 1, 0, 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
                        break;
                    case ONE_COLUMN:
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 1, n, n + 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0);
                        break;
                    case TWO_ROWS:
                        if(n == elements){
                        x = 0; y = 1;
                        }
                        if(!y)
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                x, x + 1, 0, 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0);
                        else
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                x, x + 1, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0);
                        break;
                    case TWO_COLUMNS:
                        if(n == elements){
                        x = 0; y = 1;
                        }
                        if(!y)
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 1, x, x + 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0);
                        else
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                1, 2, x, x + 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0);
                        break;
                    case COMBINATION:
                    case APPLICATION_MODE:
                        if(!n)
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                0, 1, 0, 1, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0);
                        else
                        gtk_table_attach((GtkTable*)days_panel,
                                ((WDB*)tmp->data)->button,
                                x - 1, x, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0);
                        break;
                }
            }
            tmp = g_slist_next(tmp);
        }
    }
    /* attach to main panel header and days panels */
    if (layout == COMBINATION || layout == APPLICATION_MODE){
        combination_vbox = gtk_vbox_new(FALSE, 0);
      
        if(app->current_is_valid){    
           if(g_hash_table_lookup(app->station_data, "current"))
                current_weather_widget
                  = create_current_weather_simple_widget(g_hash_table_lookup(app->station_data, "current"));
        }
        else{
            if(g_hash_table_lookup(app->station_data, "forecast"))
                current_weather_widget 
                  = create_forecast_weather_simple_widget(((GSList*)(g_hash_table_lookup(app->station_data, "forecast")))->data);
        }
        gtk_box_pack_start(GTK_BOX(combination_vbox), header_panel, FALSE, FALSE, 0);
        if(current_weather_widget)
            gtk_box_pack_start(GTK_BOX(combination_vbox), current_weather_widget, FALSE, FALSE, 0);
        gtk_table_attach((GtkTable*)days_panel, combination_vbox,
                1, Max_count_weather_day, 0, 1,
                (GtkAttachOptions)0,
                (GtkAttachOptions)0, 0, 0);
    }else
        gtk_table_attach((GtkTable*)panel, header_panel, 0, 1, 0, 1,
                (GtkAttachOptions)0, (GtkAttachOptions)0, 0, 0);
/*
    if(previos_station_name_btn)
	gtk_box_pack_start((GtkBox*)days_panel_with_buttons, previos_station_name_btn, TRUE, TRUE, 0);
    if(days_panel)
	gtk_box_pack_start((GtkBox*)days_panel_with_buttons, days_panel, TRUE, TRUE, 0);
    if(next_station_name_btn)
        gtk_box_pack_start((GtkBox*)days_panel_with_buttons, next_station_name_btn, TRUE, TRUE, 0);

    gtk_table_attach((GtkTable*)panel, days_panel_with_buttons, 0, 1, 1, 2,
			(GtkAttachOptions)0, (GtkAttachOptions)0, 0, 0);
*/
    gtk_table_attach((GtkTable*)panel, days_panel, 0, 1, 1, 2,
              (GtkAttachOptions)0, (GtkAttachOptions)0, 0, 0);
/* Connect signal button */
    if(previos_station_name_btn)
        g_signal_connect(previos_station_name_btn, "button-press-event",
                G_CALLBACK(change_station_prev), NULL);
    if(next_station_name_btn)
        g_signal_connect(next_station_name_btn, "button-press-event",
                G_CALLBACK(change_station_next), NULL);
    if(station_name_btn){
        g_signal_connect(station_name_btn, "button-press-event",
                G_CALLBACK(change_station_next), NULL);
        gtk_container_set_focus_child(GTK_CONTAINER(panel), station_name_btn);
    }
    else
        gtk_container_set_focus_child(GTK_CONTAINER(panel), panel);
    if(station_name_btn)
        g_object_unref(station_name_btn);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void
free_fields(gpointer key, gpointer val, gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(val){
/*        fprintf(stderr,"Key %s Value %s %p\n",key, val, val); */
        g_free(val);
        val = NULL;
    }
}
/*******************************************************************************/
void
free_main_hash_table(GHashTable *table){
    GSList          *tmp = NULL;
    GHashTable      *hashtable = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* free station location data */
    hashtable = g_hash_table_lookup(table, "location");
    if(hashtable){
        g_hash_table_foreach(hashtable, free_fields, NULL);
        g_hash_table_remove_all(hashtable);
        g_hash_table_unref(hashtable);
    }
    /* free station current data */
    if(hashtable){
        hashtable = g_hash_table_lookup(table, "current");
        g_hash_table_foreach(hashtable, free_fields, NULL);
        g_hash_table_remove_all(hashtable);
        g_hash_table_unref (hashtable);
    }
    /* free station days data */
    tmp = g_hash_table_lookup(table, "forecast");
    while(tmp){
        hashtable = (GHashTable*)tmp->data;
        g_hash_table_foreach(hashtable, free_fields, NULL);
        g_hash_table_remove_all(hashtable);
        g_hash_table_unref (hashtable);
        tmp = g_slist_next(tmp);
    }
    tmp = g_hash_table_lookup(table, "forecast");
    if(tmp)
        g_slist_free (tmp);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void
free_detaild_hash_table(GHashTable *table){
    GSList              *tmp1 = NULL;
    GHashTable          *tmp = NULL,
                        *hour_weather = NULL;
    gpointer            pointer_result;

    tmp = g_hash_table_lookup(table, "detail");
    if(tmp){
        tmp1 = g_hash_table_lookup(tmp, "hours_data");
        pointer_result = g_hash_table_lookup(tmp, "last_update");
        if(pointer_result)
            g_free(pointer_result);
        while(tmp1){
            hour_weather = (GHashTable*)tmp1->data;
            g_hash_table_foreach(hour_weather, free_fields, NULL);
            g_hash_table_remove_all(hour_weather);
            g_hash_table_unref(hour_weather);
            tmp1 = g_slist_next(tmp1);
        }
        tmp1 = g_hash_table_lookup(tmp, "hours_data");
        g_slist_free(tmp1);
        g_hash_table_remove_all(tmp);
        g_hash_table_unref (tmp);
    }
}
/*******************************************************************************/
/* free used memory from OMWeather struct */
void
free_memory(void){
    GSList          *tmp = NULL;
    WDB             *tmp_button = NULL;
    gboolean        valid = FALSE;
    GtkTreeIter     iter;
    GHashTable      *hashtable = NULL;
    gchar           *source_name;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* delete main window */
    if(app->main_window){
        gtk_widget_destroy(app->main_window);
        app->main_window = NULL;
    }
    free_main_hash_table(app->station_data);
    free_detaild_hash_table(app->station_data);
    /* delete hash station data */
    if(app->station_data){
       g_hash_table_remove_all(app->station_data);
    }

    /* free days button */
    tmp = app->buttons;
    while(tmp){
        tmp_button = (WDB*)tmp->data;
        if (tmp_button){
           delete_weather_day_button(&tmp_button);
           tmp_button = NULL;
        }
        tmp = g_slist_next(tmp);
    }
    g_slist_free(app->buttons);
    app->buttons = NULL;

    /* Free config->cache_directory */
    if (app->config->cache_directory){
       g_free (app->config->cache_directory);
       app->config->cache_directory = NULL;
    }

    if(app->config->cache_dir_name){
        g_free(app->config->cache_dir_name);
        app->config->cache_dir_name = NULL;
    }
    if(app->config->icon_set){
        g_free(app->config->icon_set);
        app->config->icon_set = NULL;
    }
    if(app->config->font){
        g_free(app->config->font);
        app->config->font = NULL;
    }
    if(app->config->current_country){
        g_free(app->config->current_country);
        app->config->current_country = NULL;
    }
    if(app->config->current_source){
        g_free(app->config->current_source);
        app->config->current_source = NULL;
    }
    if(app->config->current_station_name){
        g_free(app->config->current_station_name);
        app->config->current_station_name = NULL;
    }
    if(app->config->current_station_id){
        g_free(app->config->current_station_id);
        app->config->current_station_id = NULL;
    }
    if(app->config->current_station_source){
        g_free(app->config->current_station_source);
        app->config->current_station_source = NULL;
    }
    if(app->config->icons_set_base){
        g_free(app->config->icons_set_base);
        app->config->icons_set_base = NULL;
    }
    if(app->config->iap_http_proxy_host){
        g_free(app->config->iap_http_proxy_host);
        app->config->iap_http_proxy_host = NULL;
    }
    if(app->hash){
        g_hash_table_destroy(app->hash);
        app->hash = NULL;
    }
    if(app->sources_list){
      /* free sources list */
      valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                              (app->sources_list),
                                              &iter);
      while (valid) {
          gtk_tree_model_get(GTK_TREE_MODEL
                                   (app->sources_list), &iter,
                                   0, &source_name,
                                   1, &hashtable, -1);
          g_free(source_name);
          free_hashtable_with_source(hashtable);
          valid =
                  gtk_tree_model_iter_next(GTK_TREE_MODEL
                                                         (app->sources_list), &iter);
      }
      gtk_list_store_clear(app->sources_list);
      g_object_unref(app->sources_list);
      app->sources_list = NULL;
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void
fill_weather_day_button_expand(WDB *new_day_button, const char *text, const char *icon,
                const gint icon_size, gboolean transparency,
                gboolean draw_day_label)
{
    GdkPixbuf   *icon_buffer;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* create day label */
    if(draw_day_label){
            new_day_button->label = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(new_day_button->label), text);
        if  (app->config->text_position == LEFT ||
             app->config->text_position == RIGHT ||
             app->config->text_position == NOTHING)
            gtk_label_set_justify(GTK_LABEL(new_day_button->label), GTK_JUSTIFY_RIGHT);
        else
            gtk_label_set_justify(GTK_LABEL(new_day_button->label), GTK_JUSTIFY_CENTER);
            /* Set font size for label */
            set_font(new_day_button->label, app->config->font, -1);
            gtk_widget_set_name(new_day_button->label, "day_label");
    }else
        new_day_button->label = NULL;
   /* create day icon buffer */
    icon_buffer =
          gdk_pixbuf_new_from_file_at_size(icon,
                        icon_size,
                        icon_size, NULL);
    if (icon_buffer)
        /* create day icon image from buffer */
        new_day_button->icon_image = create_icon_widget(icon_buffer, icon, icon_size, &app->clutter_objects_in_main_form);
    else
        new_day_button->icon_image = NULL;
    /* Packing all to the box */ 
    /* create day box to contain icon and label */
    if  (app->config->text_position == LEFT ||
         app->config->text_position == RIGHT ||
         app->config->text_position == NOTHING) 
        new_day_button->box = gtk_hbox_new(FALSE, 0);
    else
        new_day_button->box = gtk_vbox_new(FALSE, 0);

    if (new_day_button->icon_image){
        if(app->config->text_position == RIGHT ||
           app->config->text_position == BOTTOM){
             gtk_box_pack_start(GTK_BOX(new_day_button->box),
                 new_day_button->icon_image, FALSE, FALSE, 0);
             if (new_day_button->label)
                 gtk_box_pack_start(GTK_BOX(new_day_button->box),
                         new_day_button->label, FALSE, FALSE, 0);
        }
        else{
            if(app->config->text_position == LEFT ||
               app->config->text_position == TOP){
                gtk_box_pack_start(GTK_BOX(new_day_button->box),
                    new_day_button->label, FALSE, FALSE, 0);
                gtk_box_pack_start(GTK_BOX(new_day_button->box),
                    new_day_button->icon_image, FALSE, FALSE, 0);
            }else
                gtk_box_pack_start(GTK_BOX(new_day_button->box),
                    new_day_button->icon_image, FALSE, FALSE, 0);
        }
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
 
}

WDB*
create_weather_day_button(const char *text, const char *icon,
                const gint type_of_button, gboolean transparency,
                gboolean draw_day_label, GdkColor *color,  gint wind_direction, gint  wind_speed){

    WDB     *new_day_button = NULL;
    int     icon_size;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    new_day_button = g_new0(WDB, 1);
    if(!new_day_button)
        return NULL;
    /* create day button */
    new_day_button->button = gtk_event_box_new();
    gtk_widget_set_events(new_day_button->button, GDK_BUTTON_RELEASE_MASK);
    #ifdef NONMAEMO
       g_signal_connect(new_day_button->button, "popup-menu", (GCallback)view_popup_menu, NULL);
    #else
       gtk_widget_tap_and_hold_setup(new_day_button->button, GTK_WIDGET(app->contextmenu),
                                NULL, 0);
    #endif
#if defined OS2008 || defined OS2009
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(new_day_button->button), FALSE);
#else
    set_background_color(new_day_button->button, color);
    if(transparency)
        gtk_event_box_set_visible_window(GTK_EVENT_BOX(new_day_button->button), FALSE);
#endif
/* select image size */
    switch(app->config->icons_size){
        case GIANT:
            icon_size = GIANT_ICON_SIZE;
            break;
        default:
        case LARGE:
            icon_size = LARGE_ICON_SIZE;
            break;
        case MEDIUM:
            icon_size = MEDIUM_ICON_SIZE;
            break;
        case SMALL:
            icon_size = SMALL_ICON_SIZE;
            break;
        case TINY:
            icon_size = TINY_ICON_SIZE;
            break;
    }
#if defined(NONMAEMO) || defined (APPLICATION)
    icon_size = MEDIUM_ICON_SIZE; 
#endif
    if (!app->config->is_application_mode && app->config->icons_layout >= PRESET_NOW)
        if (type_of_button == FIRST_BUTTON)
            fill_weather_day_button_preset_now(new_day_button, text, icon,
                icon_size, transparency, draw_day_label, wind_direction,  wind_speed);
        else
            fill_weather_day_button_presets(new_day_button, text, icon,
                icon_size, transparency, draw_day_label, wind_direction,  wind_speed);
    else{
        if (type_of_button == FIRST_BUTTON)
            icon_size = icon_size * 2;
        fill_weather_day_button_expand(new_day_button, text, icon, 
                icon_size, transparency, draw_day_label);
    }
    gtk_container_add(GTK_CONTAINER(new_day_button->button), new_day_button->box);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return new_day_button;
}
/*******************************************************************************/
void 
delete_weather_day_button(WDB **day){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(*day){
        g_free(*day);
        *day = NULL;
    }
}
/*******************************************************************************/
gboolean 
switch_timer_handler(gpointer data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->popup_window){
        destroy_popup_window(NULL);
    }
    change_station_next(NULL, NULL, NULL);
    return TRUE;
}
/*******************************************************************************/
GtkListStore* 
create_user_stations_list(void){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    return gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING,
                                 G_TYPE_BOOLEAN, G_TYPE_STRING);
}
/*******************************************************************************/
void 
add_change_day_part_event(GHashTable *day, guint year, guint month){
    gchar       buffer[255];
    struct tm   tm = {0};
    time_t      time;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1,
                "%s %i 00:00:00", (char*)g_hash_table_lookup(day, "day_date"), year);
    strptime(buffer, "%b %d %Y %T", &tm);
    /* Check New Year */
    if(month == 11 && tm.tm_mon == 0){
        snprintf(buffer, sizeof(buffer) - 1,
                    "%s %i 00:00:00", (char*)g_hash_table_lookup(day, "day_date"), year + 1);
        strptime(buffer, "%b %d %Y %T", &tm);
    }
    time = mktime(&tm);
    event_add(time, CHANGE_DAY_PART);
}
/*******************************************************************************/
time_t 
get_day_part_begin_time(GHashTable *day, guint year, const gchar *day_part){
    gchar       buffer[255];
    struct tm   tm = {0};
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (g_hash_table_lookup(day, "day_date") && g_hash_table_lookup(day, day_part)){
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer) - 1,
                    "%s %i %s", (char*)g_hash_table_lookup(day, "day_date"), year,
                    (char*)g_hash_table_lookup(day, day_part));
        strptime(buffer, "%b %d %Y %I:%M %p", &tm);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
 
    return  mktime(&tm);
}
/*******************************************************************************/
void 
create_wind_parameters(GHashTable *day, gchar *buffer, gboolean is_day, gint *direction, float *speed){
    gchar   *wind_direction = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
  /* Is it data current day for? */ 
    if(!buffer){
        if(!g_hash_table_lookup(day, "wind_speed") ||
            !strcmp((char*)g_hash_table_lookup(day, "wind_speed"), "N/A"))
            *speed = -1;
        else
            *speed = convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "wind_speed")));

        if(!g_hash_table_lookup(day, "wind_direction") ||
            !strcmp((char*)g_hash_table_lookup(day, "wind_direction"), "N/A"))
            *direction = UNKNOWN_DIRECTION;
        else{
            wind_direction = (char*)hash_table_find(g_hash_table_lookup(day, "wind_direction"), TRUE);
            *direction = choose_wind_direction(g_hash_table_lookup(day, "wind_direction"));
        }
        return;
    }
    
    if((is_day && ((!g_hash_table_lookup(day, "day_wind_speed")|| 
                   (!g_hash_table_lookup(day, "night_wind_speed")))||
           (!strcmp((char*)g_hash_table_lookup(day, "day_wind_speed"), "N/A")) &&
           !strcmp((char*)g_hash_table_lookup(day, "night_wind_speed"), "N/A")))){
            if (buffer && app->config->icons_layout < PRESET_NOW)
                sprintf(buffer + strlen(buffer),
                        "<span foreground='#%02x%02x%02x'>\n%s\n%s</span>",
                        app->config->font_color.red >> 8,
                        app->config->font_color.green >> 8,
                        app->config->font_color.blue >> 8,
                        _("N/A"), _("N/A"));
            *direction = UNKNOWN_DIRECTION;
            *speed = -1;
    }
    else{
        if((is_day && !strcmp((char*)g_hash_table_lookup(day, "day_wind_title"), "N/A")) &&
                !strcmp((char*)g_hash_table_lookup(day, "day_wind_speed"), "N/A"))
            is_day = FALSE;

        if(is_day){
            *direction = choose_wind_direction(g_hash_table_lookup(day, "day_wind_title"));
            wind_direction = (char*)hash_table_find(g_hash_table_lookup(day, "day_wind_title"), TRUE);
            if(buffer && app->config->icons_layout < PRESET_NOW){
                sprintf(buffer + strlen(buffer),
                        "<span foreground='#%02x%02x%02x'>\n%s",
                        app->config->font_color.red >> 8,
                        app->config->font_color.green >> 8,
                        app->config->font_color.blue >> 8,
                        wind_direction);
            }
            if (g_hash_table_lookup(day, "day_wind_speed")){
                *speed = convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed")));
                if(buffer && app->config->icons_layout < PRESET_NOW){
                    if(app->config->show_wind_gust)
                        sprintf(buffer + strlen(buffer), "%.1f</span>",
                                convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed"))));
                   else
                        sprintf(buffer + strlen(buffer),"</span>");
                }
            }
       }
       else{
            *direction = choose_wind_direction(g_hash_table_lookup(day, "night_wind_title"));
            wind_direction = (char*)hash_table_find(g_hash_table_lookup(day, "night_wind_title"), TRUE);
            if (g_hash_table_lookup(day, "night_wind_speed")){
                *speed = convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed")));
                if(buffer && app->config->icons_layout < PRESET_NOW){
                    sprintf(buffer + strlen(buffer),
                            "<span foreground='#%02x%02x%02x'>\n%s",
                            app->config->font_color.red >> 8,
                            app->config->font_color.green >> 8,
                            app->config->font_color.blue >> 8,
                            wind_direction);
                    if(app->config->show_wind_gust)
                        sprintf(buffer + strlen(buffer), "%.1f</span>",
                        convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))));
                    else
                        sprintf(buffer + strlen(buffer),"</span>");
                }
            }
       }
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void 
create_current_temperature_text(GHashTable *day, gchar *buffer, gboolean valid,
                                                        const gchar *day_name){
    gint    temp_current = INT_MAX;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if(strcmp(g_hash_table_lookup(day, "day_hi_temperature"), "N/A"))
        temp_current = atoi(g_hash_table_lookup(day, "day_hi_temperature"));

    if(app->config->temperature_units == FAHRENHEIT)
        if( temp_current != INT_MAX )
            temp_current = c2f(temp_current);

    if(temp_current == INT_MAX || !valid)
        if (!app->config->is_application_mode && app->config->icons_layout < PRESET_NOW)
            sprintf(buffer,"<span>%s</span>",_("N/A") );
        else{
            sprintf(buffer,
                "<span foreground='#%02x%02x%02x'>%s\n%s\302\260\n</span>",
                app->config->font_color.red >> 8,
                app->config->font_color.green >> 8,
                app->config->font_color.blue >> 8,
                (app->config->separate) ? (_("Now")) : (day_name),
                _("N/A") );
        }
    else
        if (!app->config->is_application_mode && app->config->icons_layout >= PRESET_NOW)
            sprintf(buffer,"<span stretch='ultracondensed' foreground='%s'>%i\302\260</span>",
                            PRESET_BIG_FONT_COLOR_FRONT, temp_current);
        else
            sprintf(buffer,
                "<span weight=\"bold\" foreground='#%02x%02x%02x'>%s\n%i\302\260\n</span>",
                app->config->font_color.red >> 8,
                app->config->font_color.green >> 8,
                app->config->font_color.blue >> 8,
                (app->config->separate) ? (_("Now")) : (day_name),
                temp_current );
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void
create_day_temperature_text(GHashTable *day, gchar *buffer, gboolean valid,
                                                gboolean for_combination_mode, gint button_number){
    gint        temp_hi = INT_MAX,
                temp_low = INT_MAX;
    gchar       delemiter[2] ;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    memset(delemiter, 0, sizeof(delemiter));
    if (g_hash_table_lookup(day, "day_hi_temperature"))
        if(strcmp(g_hash_table_lookup(day, "day_hi_temperature"), "N/A"))
            temp_hi = atoi(g_hash_table_lookup(day, "day_hi_temperature"));
    if (g_hash_table_lookup(day, "day_low_temperature"))
        if(strcmp(g_hash_table_lookup(day, "day_low_temperature"), "N/A"))
            temp_low = atoi(g_hash_table_lookup(day, "day_low_temperature"));

    if(app->config->temperature_units == FAHRENHEIT){
        if( temp_hi != INT_MAX )
            temp_hi = c2f(temp_hi);
        if( temp_low != INT_MAX )
            temp_low = c2f(temp_low);
    }

    if (((app->config->text_position == TOP || app->config->text_position == BOTTOM ||
        app->config->icons_layout == PRESET_NOW) && !(app->config->icons_layout > PRESET_NOW)) || 
        button_number == FIRST_BUTTON){
        delemiter[0] = '/';
    }
    else{
        delemiter[0] = '\n';
    }

    if(app->config->swap_hi_low_temperature)
        swap_temperature(&temp_hi, &temp_low);

    /* For presets mode */
    if (!app->config->is_application_mode){
        if (app->config->icons_layout == PRESET_NOW || button_number == FIRST_BUTTON){
            sprintf(buffer,"<span stretch='ultracondensed' foreground='%s'>",
                                PRESET_BIG_FONT_COLOR_FRONT);
            if(temp_low != INT_MAX)
                sprintf(buffer + strlen(buffer), "%i\302\260", temp_low);
            if(temp_hi != INT_MAX)
                sprintf(buffer + strlen(buffer), "%s%i\302\260", delemiter, temp_hi );
            strcat(buffer, "</span>");
            return;
        }
        if (app->config->icons_layout > PRESET_NOW){
            sprintf(buffer,"%s\n",(char*)g_hash_table_lookup(day, "day_name"));
            if(temp_low != INT_MAX)
                sprintf(buffer + strlen(buffer), "%i\302\260", temp_low);
            if(temp_hi != INT_MAX)
                sprintf(buffer + strlen(buffer), "\n%i\302\260",  temp_hi );
            return;
        }
    }
    if(!for_combination_mode){
        sprintf(buffer,
            "<span %s foreground='#%02x%02x%02x'>%s\n",
            (valid) ? ("weight=\"bold\"") : (""),
            app->config->font_color.red >> 8,
            app->config->font_color.green >> 8,
            app->config->font_color.blue >> 8,
            (char*)g_hash_table_lookup(day, "day_name"));
        if(temp_low == INT_MAX)
            sprintf(buffer + strlen(buffer), "%s\302\260%s", _("N/A"), delemiter );
        else
            sprintf(buffer + strlen(buffer), "%i\302\260%s", temp_low, delemiter );
        if(temp_hi == INT_MAX)
            sprintf(buffer + strlen(buffer), "%s\302\260", _("N/A") );
        else
            sprintf(buffer + strlen(buffer), "%i\302\260", temp_hi );
        strcat(buffer, "</span>");
    }
    else{
        sprintf(buffer,
            "<span %s foreground='#%02x%02x%02x'>",
            (valid) ? ("weight=\"bold\"") : (""),
            app->config->font_color.red >> 8,
            app->config->font_color.green >> 8,
            app->config->font_color.blue >> 8);
        if(temp_low == INT_MAX)
            sprintf(buffer + strlen(buffer), "%s\302\260 - ", _("N/A") );
        else
            sprintf(buffer + strlen(buffer), "%i\302\260 - ", temp_low );
        if(temp_hi == INT_MAX)
            sprintf(buffer + strlen(buffer), "%s\302\260", _("N/A") );
        else
            sprintf(buffer + strlen(buffer), "%i\302\260", temp_hi );
        strcat(buffer, "</span>");
    }
}
/*******************************************************************************/
#if defined OS2008 && !defined (APPLICATION)
GtkWidget*
settings_menu(HildonDesktopHomeItem *home_item, GtkWindow *parent){
    GtkWidget		*menu_item = NULL;
    OMWeatherPrivate	*priv = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    priv = OMWEATHER_GET_PRIVATE(OMWEATHER(home_item));
    priv->desktop = GTK_WIDGET(parent);
    menu_item = gtk_menu_item_new_with_label(_("OMWeather settings"));

    g_signal_connect_swapped(G_OBJECT(menu_item), "activate",
			G_CALLBACK(weather_window_settings), NULL);

    return menu_item;
}
/*******************************************************************************/
static void
omweather_class_init(OMWeatherClass *klass){
    HildonDesktopHomeItemClass	*applet_class;
    GtkWidgetClass		*widget_class;
    GtkObjectClass		*gtk_object_class;

    g_type_class_add_private(klass, sizeof(OMWeatherPrivate));
    applet_class = HILDON_DESKTOP_HOME_ITEM_CLASS(klass);
    widget_class = GTK_WIDGET_CLASS(klass);

    gtk_object_class = GTK_OBJECT_CLASS(klass);
    applet_class->settings = settings_menu;
    gtk_object_class->destroy = omweather_destroy;
    widget_class->expose_event = expose_parent;
}

#endif
/*******************************************************************************/

#if defined OS2009  && !defined APPLICATION

omweather_plugin_widget_finalize (GObject *object)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    G_OBJECT_CLASS (omweather_plugin_parent_class)->finalize (object);
}


static void
omweather_plugin_class_finalize (OmweatherDesktopWidgetClass *object)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

   omweather_destroy(NULL);
}


static void
omweather_plugin_class_init (OmweatherDesktopWidgetClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  object_class->finalize = omweather_plugin_widget_finalize;
  widget_class->realize = omweather_plugin_realize;
  widget_class->expose_event = omweather_plugin_expose_event;
}

static void
omweather_plugin_visible_notify (GObject                *object,
                                          GParamSpec             *spec,
                                          OmweatherDesktopWidget *applet)
{
  gboolean visible;

  g_object_get (object, "is-on-current-desktop", &visible, NULL);

  g_debug ("is-on-current-desktop changed. visible: %u", visible);
}

OmweatherDesktopWidget*
omweather_plugin_widget_new (void)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

  return g_object_new (OMWEATHER_TYPE_DESKTOP_WIDGET, NULL);
}
/*
static void
omweather_plugin_init (OmweatherDesktopWidget *applet)
{
  GtkWidget *label;


  label = gtk_label_new ("An example applet");
  gtk_widget_show (label);

  gtk_container_add (GTK_CONTAINER (applet), label);

  g_signal_connect (applet, "notify::is-on-current-desktop",
                    G_CALLBACK (omweather_plugin_visible_notify), applet);
}
*/
#endif
