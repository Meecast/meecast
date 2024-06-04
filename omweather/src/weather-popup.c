/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2010 Vlad Vasiliev
 * Copyright (C) 2006-2010 Pavel Fialko
 * Copyright (C) 2008 Andrew Olmsted
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
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
#include "weather-popup.h"
#include "weather-utils.h"
#include "weather-data.h"
#include "weather-simple-popup.h"
#include "weather-sources.h"
#include "weather-hash.h"
#include "weather-help.h"
#include "weather-home.h"
#include "weather-settings.h"
#include <sys/time.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#define MOON_ICONS		"/usr/share/omweather/moon_icons/"
/*******************************************************************************/
void
destroy_popup_window(void){
    GSList    *tmp = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->tab_of_window_popup){ /* destroy old popup window */
        /* free Idles */
        tmp = app->tab_of_window_popup;
        while(tmp){
#ifndef RELEASE
             if(g_idle_remove_by_data(tmp->data))
#else
             g_idle_remove_by_data(tmp->data);
#endif
#ifndef RELEASE
                fprintf(stderr,"Succes\n");
             else
                fprintf(stderr,"Not Succes\n");
#endif
            tmp = g_slist_next(tmp);
        }
        g_slist_free(app->tab_of_window_popup);
        app->tab_of_window_popup = NULL;
    }
#if defined CLUTTER || HILDONANIMATION
    /* For end of Clutter animation in popup window */
    free_clutter_objects_list(&app->clutter_objects_in_popup_form);
#endif
    dbus_connection_remove_filter(app->dbus_conn, get_mce_signal_cb_popup, app->popup_window); 
    if (app->popup_window)
        gtk_widget_destroy(GTK_WIDGET(app->popup_window));
    app->popup_window = NULL;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
 
}
/*******************************************************************************/
/* For debug */

struct timeval tv1,tv2,dtv;

struct timezone tz;

/* Debug
void time_start() { gettimeofday(&tv1, &tz); }

double time_stop()

{  
    gettimeofday(&tv2, &tz);
    dtv.tv_sec= tv2.tv_sec  -  tv1.tv_sec;
    dtv.tv_usec=tv2.tv_usec -  tv1.tv_usec;
    if(dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }
        return dtv.tv_sec*1000.0+dtv.tv_usec/1000.0;
}
*/

GtkWidget*
create_sun_time_widget(GHashTable *day){
    GtkWidget   *main_widget = NULL,
                *main_label;
    gchar       buffer[1024],
                temp_buffer[1024],
                time_buffer[1024];
    struct tm   time_show = {0};
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!day)
        return NULL;
/* prepare additional time values */

    memset(buffer, 0, sizeof(buffer));
    memset(time_buffer, 0, sizeof(time_buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));

    if(g_hash_table_lookup(day, "day_sunrise")){
        snprintf(buffer, sizeof(buffer) - 1, "%s", _("Sunrise: "));
        strptime(g_hash_table_lookup(day, "day_sunrise"), "%r", &time_show);
        if(strstr(g_hash_table_lookup(day, "day_sunrise"), "PM"))
            time_show.tm_hour += 12;

        strftime(temp_buffer, sizeof(temp_buffer) - 1,
                    "%X", &time_show);
        /* Remove chars of seconds */
        memmove(temp_buffer + 5, temp_buffer + 8, 5);
        strcat(buffer, temp_buffer);
    }

    if(g_hash_table_lookup(day, "day_sunset")){
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                    " %s", _("Sunset: "));
        memset(time_buffer, 0, sizeof(time_buffer));
        memset(temp_buffer, 0, sizeof(temp_buffer));
        strptime(g_hash_table_lookup(day, "day_sunset"), "%r", &time_show);
        if(strstr(g_hash_table_lookup(day, "day_sunset"), "PM"))
            time_show.tm_hour += 12;

        strftime(temp_buffer, sizeof(temp_buffer) - 1,
                    "%X ", &time_show);
        /* Remove chars of seconds */
        memmove(temp_buffer + 5, temp_buffer + 8, 5);
        strcat(buffer, temp_buffer);
    }
    main_label = gtk_label_new(buffer);
    set_font(main_label, NULL, 10);
    main_widget = gtk_hbox_new(FALSE, 10);

    /* Packing all to the box */
    gtk_box_pack_start(GTK_BOX(main_widget), main_label, TRUE, TRUE, 0);

    return main_widget;
}
/*******************************************************************************/
GtkWidget*
create_moon_phase_widget(GHashTable *current){
    GtkWidget   *main_widget = NULL,
                *main_label = NULL;
    gchar       buffer[1024],
                icon[2048],
                *space_symbol = NULL,
                *phase_description = NULL;
    GdkPixbuf   *icon_buffer;
    GtkWidget   *icon_image;
    gint        icon_size = GIANT_ICON_SIZE;
    gint        font_size = 14;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
        return NULL;

    if (!g_hash_table_lookup(current, "moon_phase"))
        return NULL;
#ifdef OS2009
    switch (app->config->scale_in_popup){
        case 2: font_size = 14; break;     
        case 3: font_size = 16; break;
        case 4: font_size = 18; break;
        case 5: font_size = 21; break;
        case 6: font_size = 24; break;
        case 1:  	      
        default: font_size = 13; break;
    }
    switch (app->config->scale_in_popup){
        default:
        case 1: icon_size = SMALL_ICON_SIZE; break;
        case 2: icon_size = MEDIUM_ICON_SIZE; break;
        case 3: icon_size = BIG_ICON_SIZE; break;
        case 4: icon_size = LARGE_ICON_SIZE; break;
        case 5: icon_size = GIANT_ICON_SIZE; break;
        case 6: icon_size = SUPER_GIANT_ICON_SIZE; break;
    }
#endif

    memset(buffer, 0, sizeof(buffer));
    phase_description = g_strdup((char*)hash_table_find(g_hash_table_lookup(current, "moon_phase"), FALSE));
    
    /* replace space to \n */
    while (phase_description && (space_symbol = strchr(phase_description, ' ')))
         space_symbol[0] = '\n';

    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                "%s", phase_description);
    if(phase_description)
        g_free(phase_description);

    main_label = gtk_label_new(buffer);
    set_font(main_label, NULL, font_size);

    main_widget = gtk_hbox_new(FALSE, 0);

    /* Moon icon */
    snprintf(icon, (sizeof(icon) - 1), "%s%s.png", MOON_ICONS,
                (char*)g_hash_table_lookup(current, "moon_phase"));
    space_symbol = strchr(icon, ' ');
    if(space_symbol)
        *space_symbol = '_';
    icon_buffer = gdk_pixbuf_new_from_file_at_size(icon, icon_size, icon_size, NULL);
   // icon_image = create_icon_widget(icon_buffer, icon, GIANT_ICON_SIZE, &app->clutter_objects_in_popup_form);
    icon_image = create_icon_widget(icon_buffer, icon, icon_size, NULL);

    if(icon_image)
        gtk_box_pack_start(GTK_BOX(main_widget), icon_image, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(main_widget), main_label, FALSE, TRUE, 10);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return main_widget;
}
/*******************************************************************************/
GtkWidget*
create_time_updates_widget(GHashTable *current, gboolean change_color){
    GtkWidget   *main_widget = NULL,
                *label_update = NULL;
    gchar       buffer[1024],
                full_filename[2048];
    struct stat statv;
    time_t      tmp_time = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
        return NULL;

    tmp_time = last_update_time_new(current);

    memset(buffer, 0, sizeof(buffer));
    if(change_color)
        snprintf(buffer, sizeof(buffer) - 1, "<span foreground='#%02x%02x%02x'>",
                       app->config->font_color.red >> 8,
                       app->config->font_color.green >> 8,
                       app->config->font_color.blue >> 8);

    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                       " %s", _("Last update at server: "));
    if(tmp_time <= 0)   /* if weather data for station wasn't download */
        strcat(buffer, _("Unknown"));
    else{
        strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                    "%X %x", localtime(&tmp_time));
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                    " %s", _("station local time"));
    }
    strcat(buffer, "\n");
    sprintf(full_filename, "%s/%s.xml", app->config->cache_dir_name,
            app->config->current_station_id);
    if(stat(full_filename, &statv)){
        sprintf(buffer + strlen(buffer), "%s%s",
                _("Last update from server: "), _("Unknown"));
    }
    else{
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                    "%s", _("Last update from server: "));
        strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                    "%X %x", localtime(&statv.st_mtime));
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                    " %s", _("current location local time"));
    }
    if(change_color){
        strcat(buffer, "</span>");
        label_update = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label_update), buffer);
    }
    else
        label_update = gtk_label_new(buffer);

    set_font(label_update, NULL, 10);
    main_widget = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_widget), label_update, TRUE, TRUE, 0);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return main_widget;
}
/*******************************************************************************/
gboolean 
make_current_tab(GtkWidget *vbox){
    GtkWidget   *child = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->popup_window){
        child = create_current_tab(g_hash_table_lookup(app->station_data, "current"));
        if(app->popup_window){
            gtk_container_add(GTK_CONTAINER(vbox),child);
#ifndef CLUTTER
           gtk_widget_show_all(vbox);
#endif
        }
        else
           gtk_widget_destroy(GTK_WIDGET(child));
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return FALSE;
}
/*******************************************************************************/
gboolean 
make_tab(GtkWidget *vbox){
    GHashTable  *day = NULL;
    gchar       *day_name = NULL;
    GtkWidget   *child;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if(app->popup_window){
        day = (GHashTable*)g_object_get_data(G_OBJECT(vbox), "day");
        child = create_day_tab(g_hash_table_lookup(app->station_data, "current"),
                                day, &day_name);
        if(app->popup_window){
            gtk_container_add(GTK_CONTAINER(vbox), child);
#ifndef CLUTTER
            gtk_widget_show_all(vbox);
#endif
        }
        else
            gtk_widget_destroy(GTK_WIDGET(child));
        g_free(day_name);
    }
    return FALSE;
}

/*******************************************************************************/
gboolean
make_hour_tab(GtkWidget *vbox){
    GtkWidget	*child;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->popup_window){
         child = create_hour_tab();
         gtk_container_add(GTK_CONTAINER(vbox), child);
#ifndef CLUTTER
         gtk_widget_show_all(vbox);
#endif
     }
     return FALSE;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return FALSE;
}
/*******************************************************************************/
GtkWidget*
create_toolbar_box(GtkCallback exit_function, GtkWidget *window, gboolean fullscreen_button){
    GtkWidget	*buttons_box = NULL,
                *settings_button = NULL,
                *refresh_button = NULL,
                *about_button = NULL,
                *maximize_button = NULL,
                *close_button = NULL;
#ifdef OS2009
    #define BUTTON_ICON_SIZE 64
#else
    #define BUTTON_ICON_SIZE 40
#endif
    /* Bottom buttons box */
    /*buttons_box = gtk_toolbar_new();
    hildon_window_add_toolbar(HILDON_WINDOW(window_popup),GTK_TOOLBAR(buttons_box));*/
    buttons_box = gtk_hbox_new(TRUE,0);
    gtk_widget_set_size_request(buttons_box, -1, 60);

    /* Settings button */
    /*settings_button = create_tool_item(BUTTON_ICONS, "settings", 40);*/
    settings_button = create_button_with_image(BUTTON_ICONS, "settings", BUTTON_ICON_SIZE, FALSE, FALSE);
    g_signal_connect(G_OBJECT(settings_button), "button-release-event",
                     G_CALLBACK(settings_button_handler),
                     (gpointer)app->popup_window);
    /* Refresh buton */
    /*refresh_button = create_tool_item(BUTTON_ICONS, "refresh", 40);*/
    refresh_button = create_button_with_image(BUTTON_ICONS, "refresh", BUTTON_ICON_SIZE, FALSE, FALSE);
        g_signal_connect(G_OBJECT(refresh_button), "button-release-event",
                     G_CALLBACK(refresh_button_handler),
                     (gpointer)app->popup_window);
    /* About buton */
    /*about_button = create_tool_item(BUTTON_ICONS, "about", 40);*/
    about_button = create_button_with_image(BUTTON_ICONS, "about", BUTTON_ICON_SIZE, FALSE, FALSE);
    g_signal_connect(G_OBJECT(about_button), "button-release-event",
                        G_CALLBACK(about_button_handler),
                        NULL);
    /* Close button */
    /*close_button = create_tool_item(BUTTON_ICONS, "close", 40);*/
    close_button = create_button_with_image(BUTTON_ICONS, "close", BUTTON_ICON_SIZE, FALSE, FALSE);
    g_signal_connect(G_OBJECT(close_button), "button-release-event",
                        G_CALLBACK(exit_function),
                        (gpointer)window);
    /* Fullscreen button */
    /*maximize_button = create_tool_item(BUTTON_ICONS, "maximize", 40);*/
    maximize_button = create_button_with_image(BUTTON_ICONS, "maximize", BUTTON_ICON_SIZE, FALSE, FALSE);
    g_signal_connect(G_OBJECT(maximize_button), "button-release-event",
                        G_CALLBACK(maximize_button_handler),
                        (gpointer)window);
/* Pack buttons to the buttons box */
    /*gtk_toolbar_insert(GTK_TOOLBAR(buttons_box), GTK_TOOL_ITEM(settings_button), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(buttons_box), GTK_TOOL_ITEM(refresh_button), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(buttons_box), GTK_TOOL_ITEM(about_button), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(buttons_box), GTK_TOOL_ITEM(close_button), -1);*/
    gtk_box_pack_start(GTK_BOX(buttons_box), settings_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(buttons_box), refresh_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(buttons_box), about_button, TRUE, TRUE, 5);
    if (fullscreen_button)
        gtk_box_pack_start(GTK_BOX(buttons_box), maximize_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(buttons_box), close_button, TRUE, TRUE, 5);
    return buttons_box;
}
/*******************************************************************************/
void
destroy_container(GtkWidget *widget, gpointer *data){
    gtk_widget_destroy(GTK_WIDGET(widget));
}
/******************************************************************************/
void
popup_switch_cb(GtkNotebook * nb, gpointer nb_page, gint page, gpointer data){
    GtkWidget *vbox = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    vbox = gtk_notebook_get_nth_page(nb, page);
#ifdef CLUTTER
    free_clutter_objects_list(&app->clutter_objects_in_popup_form);
#endif
    gtk_container_foreach(GTK_CONTAINER(vbox), (GtkCallback)destroy_container, NULL);

    /* Create needed Tab */
    if (!strcmp(_("Now"), gtk_notebook_get_tab_label_text(nb,vbox)))
        make_current_tab(vbox);
    else
        if (!strcmp(_("Detailed"), gtk_notebook_get_tab_label_text(nb,vbox)))
            make_hour_tab(vbox);
        else
            make_tab(vbox);

    gtk_widget_show_all(vbox);

}
/*******************************************************************************/
gboolean
weather_window_popup(GtkWidget *widget, GdkEvent *event, gpointer user_data){
    GtkWidget   *notebook = NULL,
                *tab = NULL,
                *hour_tab = NULL,
                *current_tab = NULL,
                *label = NULL,
                *vbox = NULL,
                *buttons_box = NULL,
                *label_box = NULL,
                *copyright_box = NULL,
               *no_weather_box = NULL;
    gint        active_tab = 0,
                k = 0,
                page = 0,
                i = 1;
    gchar       *day_name = NULL;
    time_t      current_time = 0,
                data_last_update = 0;
    GSList      *tmp = NULL;
    GHashTable  *day = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->popup_window || app->phase != ZERO_PHASE)
        return FALSE;
/* Debug */
/*  time_start(); */
/* day number */
    active_tab = (gint)user_data;
/* if no one station present in list show settings window */
    if(!app->config->current_station_id){
        weather_window_settings(NULL, (gpointer)active_tab);
        return FALSE;
    }
/* show simple window if it enabled */
    if(app->config->mode == SIMPLE_MODE){
        weather_simple_window_popup(widget, user_data);
        return TRUE;
    }
/* Main window */
#if defined OS2009
     app->popup_window = hildon_stackable_window_new();
#else
     app->popup_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
#endif
    g_signal_connect((gpointer)app->popup_window, "destroy_event",
                        G_CALLBACK(destroy_popup_window), GINT_TO_POINTER(0));
    g_signal_connect((gpointer)app->popup_window, "delete_event",
                        G_CALLBACK(destroy_popup_window), GINT_TO_POINTER(0));
#if defined NONMAEMO
    gtk_window_set_default_size(GTK_WINDOW(app->popup_window), 640, 480);
#endif

#if defined CLUTTER
    gtk_signal_connect(GTK_OBJECT (app->popup_window), "delete_event",
                        GTK_SIGNAL_FUNC (destroy_popup_window), NULL);
#endif

/* set window title and icon */
    gtk_window_set_title(GTK_WINDOW(app->popup_window),
                            _("OMWeather Forecast"));
    gtk_window_set_modal(GTK_WINDOW(app->popup_window), TRUE);
#if defined OS2009  && defined APPLICATION
    gtk_window_set_destroy_with_parent(GTK_WINDOW(app->popup_window), TRUE);
#endif
    g_object_set_data(G_OBJECT(app->popup_window), "active_tab", (gpointer)active_tab);
#if defined NONMAEMO
    gtk_window_set_default_size(GTK_WINDOW(app->popup_window), 640, 480);
#else
    gtk_window_fullscreen(GTK_WINDOW(app->popup_window));
#endif

/* create frame vbox */
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(app->popup_window), vbox);

/* station name */
    label_box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(label_box),
           create_window_header(app->config->current_station_name, app->popup_window));
    gtk_box_pack_start(GTK_BOX(vbox), label_box, FALSE, TRUE, 0);
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(label_box),FALSE);

/* create tabs widget */
    notebook = gtk_notebook_new();
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);

/* check for data accesiability */
    if(!g_hash_table_lookup(app->station_data, "location"))
        goto skip;
    /* Check a valid time for current weather */
    if(app->current_is_valid)
        current_tab = gtk_vbox_new(FALSE, 0);

    if(current_tab){
        if(active_tab == 0){
            gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                                        current_tab,
                                        gtk_label_new(_("Now")));
            make_current_tab(current_tab);
        }
        else{
            gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                                        current_tab,
                                        gtk_label_new(_("Now")));
#ifndef CLUTTER
            g_idle_add((GSourceFunc)make_current_tab, current_tab);
#endif
            add_item2object(&(app->tab_of_window_popup), (void*)current_tab);
        }
    }

/* if weather is separated than hide one day */
    (app->config->separate && app->config->icons_layout < PRESET_NOW) ? (k = 0) : (k = 1);
    if(current_tab && (!app->config->separate || app->config->icons_layout >= PRESET_NOW) && active_tab != 0 )
        active_tab = active_tab + k;

    if((!app->config->separate|| app->config->icons_layout >= PRESET_NOW) && !current_tab )
        active_tab++;

/* Detailed weather tab */
    if(g_hash_table_lookup(app->station_data, "detail")){
        data_last_update = last_update_time_new(g_hash_table_lookup(app->station_data, "detail"));
        /* Check a valid time for hours forecast */
        if(app->config->show_weather_for_two_hours && 
          (current_time - 24 * 60 * 60) < data_last_update)
            hour_tab = gtk_vbox_new(FALSE, 0);
        if(hour_tab){
            gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                                        hour_tab,
                                        gtk_label_new(_("Detailed")));
           g_idle_add((GSourceFunc)make_hour_tab, hour_tab);
           add_item2object(&(app->tab_of_window_popup), (void*)hour_tab);
        }
    }
/* Day tabs */
   tmp = g_hash_table_lookup(app->station_data, "forecast");
   while(tmp && i < Max_count_weather_day){
        day = (GHashTable*)tmp->data;
        /* Acceleration of starting gtk_notebook */
        if(active_tab != i){
            /* Create the empty page */
            tab = create_pseudo_day_tab(g_hash_table_lookup(app->station_data, "current"),
                                        day, &day_name);
            if(tab){
                page = gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                                        tab,
                                        gtk_label_new(day_name));
                g_object_set_data(G_OBJECT(tab), "day", (gpointer)tmp->data);
#ifndef CLUTTER
                g_idle_add((GSourceFunc)make_tab,tab);
#endif
                add_item2object(&(app->tab_of_window_popup), (void*)tab);
           }
        }
        else{
             /* Create the page with data */
            tab = create_day_tab(g_hash_table_lookup(app->station_data, "current"),
                                    day, &day_name);
            if(tab){
               page = gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                                        tab,
                                        gtk_label_new(day_name));
               g_object_set_data(G_OBJECT(tab), "day", (gpointer)tmp->data);
           }
        }
        if(day_name){
            g_free(day_name);
            day_name = NULL;
        }
        tmp = g_slist_next(tmp);
        i++;
    }
/* prepare day tabs */
    if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0){
        gtk_box_pack_start(GTK_BOX(vbox), notebook,
                           TRUE, TRUE, 0);
        gtk_widget_show(notebook);
    }

    /* check if no data file for this station */
    if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0){
        if(active_tab == -1){
            hildon_banner_show_information(app->main_window,
                            NULL,
                            _("No weather data for this day."));
            destroy_popup_window();
            return FALSE;
        }
        else{
            if(app->config->separate && app->config->icons_layout < PRESET_NOW){
                if(!current_tab){
                    if(active_tab)
                        active_tab--;
                    else{
                        hildon_banner_show_information(app->main_window,
                                    NULL,
                                    _("No current weather data."));
                        destroy_popup_window();
                        return FALSE;
                    }
                }
            }
        }
    }
    else{
skip:
        gtk_widget_destroy(notebook);
        no_weather_box = gtk_event_box_new();
        gtk_container_add(GTK_CONTAINER(no_weather_box),label = gtk_label_new(_("No weather data for this station.")));
        gtk_box_pack_start(GTK_BOX(vbox), no_weather_box, TRUE, TRUE, 0);
        gtk_event_box_set_visible_window(GTK_EVENT_BOX(no_weather_box),FALSE);
        set_font(label, NULL, 24);
    }

    buttons_box = create_toolbar_box((GtkCallback)popup_close_button_handler, app->popup_window, FALSE);

#ifdef CLUTTER
    /* Connect to signal "changing notebook page" */
    g_signal_connect(G_OBJECT(notebook), "switch-page",
                     G_CALLBACK(popup_switch_cb), app->popup_window);
#endif
/* Show copyright widget */
    copyright_box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(copyright_box),
                  create_copyright_widget(app->config->current_station_source, 
                                            get_source_logo(app->sources_list, app->config->current_station_source)));
    gtk_box_pack_start(GTK_BOX(vbox),
                  copyright_box,
                  FALSE, TRUE, 0);
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(copyright_box),TRUE);

    gtk_box_pack_start(GTK_BOX(vbox),buttons_box,FALSE,FALSE,0);
    gtk_widget_show_all(app->popup_window);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    /* Debug
     fprintf(stderr,"Time: %lf msec Pi = %lf\n",time_stop(),weather_window_settings);
    */
    return FALSE;
}
/*******************************************************************************/
void
settings_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
    gint day_number
	= (gint)g_object_get_data(G_OBJECT(user_data), "active_tab");
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* For debug speed of creating setting window */
/* time_start();  */
    destroy_popup_window();
    weather_window_settings(NULL, (gpointer)day_number);
/* fprintf(stderr,"Time: %lf msec Pi = %lf\n",time_stop(),weather_window_settings);*/
}
/*******************************************************************************/
void refresh_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->popup_window)
        destroy_popup_window();
    update_weather(TRUE);
}
/*******************************************************************************/
void about_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
    create_about_dialog();
}
/*******************************************************************************/
void maximize_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    change_state_window(GTK_WIDGET(user_data));
}
/*******************************************************************************/
void popup_close_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    destroy_popup_window();
}
/*******************************************************************************/
GtkWidget*
create_pseudo_day_tab(GHashTable *current, GHashTable *day, gchar **day_name){
    GtkWidget   *main_widget = NULL;
    gchar       buffer[1024];
    struct tm   tmp_time_date_struct = {0};

    main_widget = gtk_vbox_new(FALSE, 0);
    /* prepare localized day name */
    if (g_hash_table_lookup(day, "day_name")){
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer,"%s", (char*)g_hash_table_lookup(day, "day_name"));
        strptime(buffer, "%a", &tmp_time_date_struct);
        *day_name = g_strdup(buffer);
    }

    return main_widget;
}
/*******************************************************************************/
GtkWidget*
create_day_tab(GHashTable *current, GHashTable *day, gchar **day_name){
    gint        font_size = 13,
                icon_size = BIG_ICON_SIZE; 
    GtkWidget   *main_widget = NULL,
                *day_condition = NULL,
                *night_condition = NULL,
                *day_night_hbox = NULL,
                *day_vbox = NULL,
                *night_vbox = NULL,
                *title = NULL,
                *day_icon = NULL,
                *night_icon = NULL,
                *day_label = NULL,
                *night_label = NULL,
                *day_label_temperature_hbox = NULL,
                *night_label_temperature_hbox = NULL,
                *day_icon_text_hbox = NULL,
                *night_icon_text_hbox = NULL,
                *day_text_vbox = NULL,
                *night_text_vbox = NULL,
                *day_text = NULL,
                *night_text = NULL,
                *day_length = NULL;
    gchar       buffer[1024],
                temp_buffer[255],
                *units,
                symbol = 'C';
    struct tm   tmp_time_date_struct = {0};
    GdkPixbuf   *icon = NULL;
    gint        hi_temp,
                low_temp,
                day_invalid_count = 0,
                night_invalid_count = 0;
    const gchar *wind_units_str[] = { "m/s", "km/h", "mi/h" };
    struct tm   time_sunrise = {0};
    struct tm   time_sunset = {0};
    int         int_day_length = 0;
    float       tmp_pressure = 0.0f;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!day && !current)
        return NULL;
    /* prepare font size for Fremantle */
#ifdef OS2009
    switch (app->config->scale_in_popup){
        case 2: font_size = 14; break;     
        case 3: font_size = 16; break;
        case 4: font_size = 18; break;
        case 5: font_size = 21; break;
        case 6: font_size = 24; break;
        case 1:  	      
        default: font_size = 13; break;
    }
    switch (app->config->scale_in_popup){
        default:
        case 1: icon_size = SMALL_ICON_SIZE; break;
        case 2: icon_size = MEDIUM_ICON_SIZE; break;
        case 3: icon_size = BIG_ICON_SIZE; break;
        case 4: icon_size = LARGE_ICON_SIZE; break;
        case 5: icon_size = GIANT_ICON_SIZE; break;
        case 6: icon_size = SUPER_GIANT_ICON_SIZE; break;
    }
#endif
    /* prepare temperature */
    if(!g_hash_table_lookup(day, "day_hi_temperature") ||
            !strcmp(g_hash_table_lookup(day, "day_hi_temperature"), "N/A")){
        hi_temp = INT_MAX;
        day_invalid_count++;
    }
    else
        hi_temp = atoi(g_hash_table_lookup(day, "day_hi_temperature"));
    if(!g_hash_table_lookup(day, "day_low_temperature") ||
            !strcmp(g_hash_table_lookup(day, "day_low_temperature"), "N/A")){
        low_temp = INT_MAX;
        night_invalid_count++;
    }
    else
        low_temp = atoi(g_hash_table_lookup(day, "day_low_temperature"));
    if(app->config->temperature_units == FAHRENHEIT){
        if(hi_temp != INT_MAX)
            hi_temp = c2f(hi_temp);
        if(low_temp != INT_MAX)
            low_temp = c2f(low_temp);
        symbol = 'F';
    }
    main_widget = gtk_vbox_new(FALSE, 0);
    /* prepare localized day name */
    if(g_hash_table_lookup(day, "day_name")){
        *buffer = 0;
        sprintf(buffer,"%s", (char*)g_hash_table_lookup(day, "day_name"));
        strptime(buffer, "%a", &tmp_time_date_struct);
        *day_name = g_strdup(buffer);
        /* prepare title */
        *buffer = 0;
        sprintf(buffer,"%s %s", (char*)g_hash_table_lookup(day, "day_name"),
                (char*)g_hash_table_lookup(day, "day_date"));
        strptime(buffer, "%A %b %d", &tmp_time_date_struct);
        memset(buffer, 0, sizeof(buffer));
        strftime(buffer, sizeof(buffer) - 1, "%A, %d %B", &tmp_time_date_struct);
        title = gtk_label_new(buffer);
        set_font(title, NULL, font_size + 2);
    }
    /* create 24 hours data widget */
#ifdef OS2009
    if (icon_size == SUPER_GIANT_ICON_SIZE)
        day_night_hbox = gtk_vbox_new(FALSE, 5);
    else
#endif
        day_night_hbox = gtk_hbox_new(FALSE, 1);
    /* add day  items to main widget */
    /* check for N/A fields in day */
    if(g_hash_table_lookup(day, "day_title") &&
            !strcmp((char*)g_hash_table_lookup(day, "day_title"), "N/A"))
        day_invalid_count++;
    if(g_hash_table_lookup(day, "day_humidity") &&
            !strcmp(g_hash_table_lookup(day, "day_humidity"), "N/A"))
        day_invalid_count++;
    if(g_hash_table_lookup(day, "day_wind_title") &&
            !strcmp((char*)g_hash_table_lookup(day, "day_wind_title"), "N/A"))
        day_invalid_count++;
    if(g_hash_table_lookup(day, "day_wind_speed") &&
            !strcmp((char*)g_hash_table_lookup(day, "day_wind_speed"), "N/A"))
        day_invalid_count++;
    if(day_invalid_count < 5){
        /* day data */
        day_vbox = gtk_vbox_new(FALSE, 0);
        /* hbox for day label and temperature */
        day_label_temperature_hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(day_vbox), day_label_temperature_hbox,
                            TRUE, TRUE, 0);
        /* prepare day label */
        day_label = gtk_label_new(_("Day:"));
        gtk_box_pack_start(GTK_BOX(day_label_temperature_hbox),
                            day_label, FALSE, TRUE, 0);
        /* day condition */
        *buffer = 0;
        if(g_hash_table_lookup(day, "day_title"))
            snprintf(buffer, sizeof(buffer) - 1, "%s",
                      (char*)hash_table_find(g_hash_table_lookup(day, "day_title"), FALSE));
        else
            snprintf(buffer, sizeof(buffer) - 1, "%s",
                      (char*)hash_table_find("N/A", FALSE));
        gtk_box_pack_start(GTK_BOX(day_label_temperature_hbox),
              day_condition = gtk_label_new(buffer), TRUE, TRUE, 0);
        /* hbox for icon and text */
        day_icon_text_hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(day_vbox),
                            day_icon_text_hbox, FALSE, TRUE, 0);
        /* day icon */
        update_icons_set_base(NULL);
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s%s.png", app->config->icons_set_base,
                    (char*)g_hash_table_lookup(day, "day_icon"));
        icon = gdk_pixbuf_new_from_file_at_size(buffer, icon_size, icon_size, NULL);
//        day_icon = create_icon_widget(icon, buffer, BIG_ICON_SIZE, &app->clutter_objects_in_popup_form);
        day_icon = create_icon_widget(icon, buffer, icon_size, NULL);
        gtk_box_pack_start(GTK_BOX(day_icon_text_hbox),
                            day_icon, TRUE, TRUE, 5);
        /* prepare day text */
        gtk_box_pack_start(GTK_BOX(day_icon_text_hbox),
                            day_text_vbox = gtk_vbox_new(FALSE, 0),
                            TRUE, TRUE, 0);
        *buffer = 0;
        /* day temperature */
        if(hi_temp == INT_MAX)
            snprintf(buffer, sizeof(buffer) - 1, "%s: %s", _("Temperature"),
                      (char*)hash_table_find("N/A", FALSE));
        else
            snprintf(buffer, sizeof(buffer) - 1, "%s: %d \302\260%c", _("Temperature"),
                      hi_temp, symbol);
        /* day precipitation */
        if(g_hash_table_lookup(day, "day_ppcp")){
            strcat(buffer, _("\nPrecipitation: "));
            snprintf(buffer + strlen(buffer), sizeof(buffer) - 1, "%s%%",
                (char*)hash_table_find(g_hash_table_lookup(day, "day_ppcp"), FALSE));
        }
        if(g_hash_table_lookup(day, "day_precipitation")){
            strcat(buffer, _("\nPrecipitation: "));
            snprintf(buffer + strlen(buffer), sizeof(buffer) - 1, "%s%s",
                (char*)hash_table_find(g_hash_table_lookup(day, "day_precipitation"), FALSE), _("mm"));
        }
        /* day humididty */ 
        if(g_hash_table_lookup(day, "day_humidity")){
            strcat(buffer, _("\nHumidity: "));
            if(g_hash_table_lookup(day, "day_humidity") && strcmp(g_hash_table_lookup(day, "day_humidity"), "N/A"))
                sprintf(buffer + strlen(buffer), "%s%%\n",
                    (char*)g_hash_table_lookup(day, "day_humidity"));
            else{
                sprintf(buffer + strlen(buffer), "%s\n",
                        (char*)hash_table_find("N/A", FALSE));
            }
        }
        /* day pressure */
        if(g_hash_table_lookup(day, "day_pressure") && strcmp(g_hash_table_lookup(day, "day_pressure"), "N/A")){
            strcat(buffer, _("\nPressure: "));
            tmp_pressure = atof((char*)g_hash_table_lookup(day, "day_pressure"));
            switch(app->config->pressure_units){
                default:
                case MB:   units = _("mb"); 
                           sprintf(buffer + strlen(buffer), "%.0f %s\n", tmp_pressure, units);
                           break;
                case INCH: units = _("inHg"); tmp_pressure = mb2inch(tmp_pressure); 
                           sprintf(buffer + strlen(buffer), "%.2f %s\n", tmp_pressure, units);
                           break;
                case MM: units = _("mmHg"); tmp_pressure = mb2mm(tmp_pressure); 
                         sprintf(buffer + strlen(buffer), "%.0f %s\n", tmp_pressure, units);
                         break;
            }
        }
        /* day wind */
        strcat(buffer, _("Wind: "));
        if(g_hash_table_lookup(day, "day_wind_title") && g_hash_table_lookup(day, "day_wind_speed"))
            switch(app->config->wind_units){
                case BEAUFORT_SCALE:
                    sprintf(buffer + strlen(buffer), "%s\n%s: %i",
                        (char*)hash_table_find(g_hash_table_lookup(day, "day_wind_title"), FALSE),
                        _("Speed"),
                        (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed"))));
                break;
                case KILOMETERS_H:
                    sprintf(buffer + strlen(buffer), "%s\n%s: %i %s",
                            (char*)hash_table_find(g_hash_table_lookup(day, "day_wind_title"), FALSE),
                            _("Speed"),
                            (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed"))),
                            (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                break;
                default:
                    sprintf(buffer + strlen(buffer), "%s\n%s: %.2f %s",
                            (char*)hash_table_find(g_hash_table_lookup(day, "day_wind_title"), FALSE),
                            _("Speed"),
                            convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed"))),
                            (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                break;
            }
        /* sunrise */
        if(g_hash_table_lookup(day, "day_sunrise")){
            *temp_buffer = 0;
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1, "\n%s", _("Sunrise: "));
            strptime(g_hash_table_lookup(day, "day_sunrise"), "%r", &time_sunrise);
            if(strstr(g_hash_table_lookup(day, "day_sunrise"), "PM"))
                time_sunrise.tm_hour += 12;

            strftime(temp_buffer, sizeof(temp_buffer) - 1, "%X", &time_sunrise);
            /* Remove chars of seconds */
            memmove(temp_buffer + 5, temp_buffer + 8, 5);
            strcat(buffer, temp_buffer);
        }
        day_text = gtk_label_new(buffer);
        set_font(day_label, NULL, font_size);
        set_font(day_condition, NULL, font_size - 1);
        set_font(day_text, NULL, font_size - 1);
        gtk_box_pack_start(GTK_BOX(day_text_vbox),
                        day_text, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(day_night_hbox),
                        day_vbox, TRUE, FALSE, 1);
    }
    /* night data */
    night_vbox = gtk_vbox_new(FALSE, 0);
    /* hbox for night label and temperature */
    night_label_temperature_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(night_vbox),
                        night_label_temperature_hbox,
                        TRUE, TRUE, 0);
    /* prepare night label */
    night_label = gtk_label_new(_("Night:"));
    gtk_box_pack_start(GTK_BOX(night_label_temperature_hbox),
                        night_label, FALSE, TRUE, 0);
    /* night condition */
    *buffer = 0;
    if(g_hash_table_lookup(day, "night_title"))
        snprintf(buffer, sizeof(buffer) - 1, "%s",
                  (char*)hash_table_find(g_hash_table_lookup(day, "night_title"), FALSE));
    else
        snprintf(buffer, sizeof(buffer) - 1, "%s",
                  (char*)hash_table_find("N/A", FALSE));
    gtk_box_pack_start(GTK_BOX(night_label_temperature_hbox),
                        night_condition = gtk_label_new(buffer), FALSE, TRUE, 40);
    /* hbox for icon and text */
    night_icon_text_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(night_vbox),
                        night_icon_text_hbox, TRUE, TRUE, 0);
    /* night icon */
    update_icons_set_base(NULL);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.png", app->config->icons_set_base,
                (char*)g_hash_table_lookup(day, "night_icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer, icon_size, icon_size, NULL);
//    night_icon = create_icon_widget(icon, buffer, BIG_ICON_SIZE, &app->clutter_objects_in_popup_form);
    night_icon = create_icon_widget(icon, buffer, icon_size, NULL);
    gtk_box_pack_start(GTK_BOX(night_icon_text_hbox),
                            night_icon, TRUE, TRUE, 5);
    /* prepare night text */
    gtk_box_pack_start(GTK_BOX(night_icon_text_hbox),
                            night_text_vbox = gtk_vbox_new(FALSE, 0),
                            TRUE, TRUE, 0);
    *buffer = 0;
    /* night temperature */
    if(low_temp == INT_MAX)
        snprintf(buffer, sizeof(buffer) - 1, "%s: %s", _("Temperature"),
                  (char*)hash_table_find("N/A", FALSE));
    else
        snprintf(buffer, sizeof(buffer) - 1, "%s: %d \302\260%c", _("Temperature"),
                  low_temp, symbol);
    /* Night precipitation */
    if(g_hash_table_lookup(day, "night_ppcp")){
        strcat(buffer, _("\nPrecipitation: "));
        snprintf(buffer + strlen(buffer), sizeof(buffer) - 1, "%s%%",
            (char*)hash_table_find(g_hash_table_lookup(day, "night_ppcp"), FALSE));
    }
    if(g_hash_table_lookup(day, "night_precipitation")){
            strcat(buffer, _("\nPrecipitation: "));
            snprintf(buffer + strlen(buffer), sizeof(buffer) - 1, "%s%s",
                (char*)hash_table_find(g_hash_table_lookup(day, "night_precipitation"), FALSE), _("mm"));
    }
    /* night humidity */
    if(g_hash_table_lookup(day, "night_humidity")){
        strcat(buffer, _("\nHumidity: "));
        if(g_hash_table_lookup(day, "night_humidity") && strcmp(g_hash_table_lookup(day, "night_humidity"), "N/A"))
            sprintf(buffer + strlen(buffer), "%s%%",
                    (char*)g_hash_table_lookup(day, "night_humidity"));
        else{
            sprintf(buffer + strlen(buffer), "%s",
                    (char*)hash_table_find("N/A", FALSE));
        }
    }
    if(g_hash_table_lookup(day, "night_pressure") && strcmp(g_hash_table_lookup(day, "night_pressure"), "N/A")){
        strcat(buffer, _("\nPressure: "));
        tmp_pressure = atof((char*)g_hash_table_lookup(day, "night_pressure"));
            switch(app->config->pressure_units){
                default:
                case MB:   units = _("mb"); 
                           sprintf(buffer + strlen(buffer), "%.0f %s\n", tmp_pressure, units);
                           break;
                case INCH: units = _("inHg"); tmp_pressure = mb2inch(tmp_pressure); 
                           sprintf(buffer + strlen(buffer), "%.2f %s\n", tmp_pressure, units);
                           break;
                case MM: units = _("mmHg"); tmp_pressure = mb2mm(tmp_pressure); 
                         sprintf(buffer + strlen(buffer), "%.0f %s\n", tmp_pressure, units);
                         break;
            }
    }
    strcat(buffer, _("Wind: "));
    if(g_hash_table_lookup(day, "night_wind_title") && g_hash_table_lookup(day, "night_wind_speed"))
        switch(app->config->wind_units){
            case BEAUFORT_SCALE:
                sprintf(buffer + strlen(buffer), "%s\n%s: %i",
                        (char*)hash_table_find(g_hash_table_lookup(day, "night_wind_title"), FALSE),
                        _("Speed"),
                        (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))));
            break;
            case KILOMETERS_H:
                sprintf(buffer + strlen(buffer), "%s\n%s: %i %s",
                        (char*)hash_table_find(g_hash_table_lookup(day, "night_wind_title"), FALSE),
                        _("Speed"),
                        (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))),
                        (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
            break;
            default:
                sprintf(buffer + strlen(buffer), "%s\n%s: %.2f %s",
                        (char*)hash_table_find(g_hash_table_lookup(day, "night_wind_title"), FALSE),
                        _("Speed"),
                        convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))),
                        (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
            break;
       }
    /* sunset */
    if(g_hash_table_lookup(day, "day_sunset")){
        *temp_buffer = 0;
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1, "\n%s", _("Sunset: "));
        strptime(g_hash_table_lookup(day, "day_sunset"), "%r", &time_sunset);
        if(strstr(g_hash_table_lookup(day, "day_sunset"), "PM"))
            time_sunset.tm_hour += 12;

        strftime(temp_buffer, sizeof(temp_buffer) - 1, "%X", &time_sunset);
        /* Remove chars of seconds */
        memmove(temp_buffer + 5, temp_buffer + 8, 5);
        strcat(buffer, temp_buffer);
    }
    night_text = gtk_label_new(buffer);
    set_font(night_label, NULL, font_size);
    set_font(night_condition, NULL, font_size - 1);
    set_font(night_text, NULL, font_size - 1);
    gtk_box_pack_start(GTK_BOX(night_text_vbox),
                      night_text, TRUE, TRUE, 0);
    /* add day and night items to main widget */
    /* check for N/A fields in night */
    if(night_invalid_count < 5)
        gtk_box_pack_start(GTK_BOX(day_night_hbox),
                      night_vbox, TRUE, FALSE, 5);
    else
        gtk_widget_destroy(night_vbox);
    /* day name with date */
    gtk_box_pack_start(GTK_BOX(main_widget), title, FALSE, FALSE, 5);
    /* day and/or night data */
    gtk_box_pack_start(GTK_BOX(main_widget), day_night_hbox, FALSE, FALSE, 5);
    /* day length */
    int_day_length = (((time_sunset.tm_hour*60) + time_sunset.tm_min) -
                 ((time_sunrise.tm_hour*60) + time_sunrise.tm_min));
    if (int_day_length > 1 && !day_invalid_count ){
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer) -1,"%s: %i:%02i", _("Day length"), (int)int_day_length/60, 
                        int_day_length%60);
        day_length = gtk_label_new(buffer);
        set_font(day_length, NULL, font_size + 1);
        gtk_box_pack_start(GTK_BOX(main_widget), day_length, FALSE, FALSE, 5);
    }

    /* last update time */
    if(current && app->config->mode != SIMPLE_MODE)
        gtk_box_pack_start(GTK_BOX(main_widget),
                           create_time_updates_widget(current, FALSE),
                          TRUE, FALSE, 5);
    gtk_widget_show_all(main_widget);

#if defined CLUTTER
    g_signal_connect_after(main_widget, "expose-event",
         G_CALLBACK(popup_window_expose), NULL);
#endif
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return main_widget;
}
/*******************************************************************************/
GtkWidget*
create_current_tab(GHashTable *current){
    GtkWidget   *main_widget = NULL,
                *icon_text_hbox = NULL,
                *text = NULL,
                *icon_image = NULL;
    gchar       buffer[1024],
                *units;
    const gchar *wind_units_str[] = { "m/s", "km/h", "mi/h" };
    GdkPixbuf   *icon = NULL;
    float       tmp_distance = 0.0f,
                tmp_pressure = 0.0f;
    gint        font_size, icon_size;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
        return NULL;
#ifdef OS2009
    switch (app->config->scale_in_popup){
        case 2: font_size = 14; break;     
        case 3: font_size = 16; break;
        case 4: font_size = 18; break;
        case 5: font_size = 21; break;
        case 6: font_size = 24; break;
        case 1:  	      
        default: font_size = 13; break;
    }
    switch (app->config->scale_in_popup){
        default:
        case 1: icon_size = SMALL_ICON_SIZE; break;
        case 2: icon_size = MEDIUM_ICON_SIZE; break;
        case 3: icon_size = BIG_ICON_SIZE; break;
        case 4: icon_size = LARGE_ICON_SIZE; break;
        case 5: icon_size = GIANT_ICON_SIZE; break;
        case 6: icon_size = SUPER_GIANT_ICON_SIZE; break;
    }
#endif

    main_widget = gtk_vbox_new(FALSE, 0);
    icon_text_hbox = gtk_hbox_new(FALSE, 0);
/* icon */
    sprintf(buffer,"%s%s.png", app->config->icons_set_base,
                (char*)g_hash_table_lookup(current, "icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer, icon_size, icon_size, NULL);
/* This code for animation */
/*        day_icon = create_icon_widget(icon, buffer, BIG_ICON_SIZE, &app->clutter_objects_in_popup_form); */
    icon_image = create_icon_widget(icon, buffer, icon_size, NULL);
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), icon_image, TRUE, TRUE, 0);
    /* temperature */
    memset(buffer, 0, sizeof(buffer));
    if (g_hash_table_lookup(current, "title"))
        sprintf(buffer, "%s\n", (char *)hash_table_find(g_hash_table_lookup(current, "title"), FALSE));
    sprintf(buffer + strlen(buffer), "%s",  _("Temperature: "));
    if (g_hash_table_lookup(current, "day_hi_temperature")){
        sprintf(buffer + strlen(buffer), "  %d\302\260",
                    ((app->config->temperature_units == CELSIUS) ?
                    ( atoi(g_hash_table_lookup(current, "day_hi_temperature"))) :
                    ( (int)c2f(atoi(g_hash_table_lookup(current, "day_hi_temperature"))))));
        (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")) )
                                                    : ( strcat(buffer, _("F\n")) );
    }
    /* feels like */
    sprintf(buffer + strlen(buffer), "%s", _("Feels like:"));
    if (g_hash_table_lookup(current, "feel_like")){
        sprintf(buffer + strlen(buffer), "  %d\302\260",
                    (app->config->temperature_units == CELSIUS) ?
                    (atoi(g_hash_table_lookup(current, "feel_like"))) :
                    ((int)c2f(atoi(g_hash_table_lookup(current, "feel_like")))));
        (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")) )
                                                    : ( strcat(buffer, _("F\n")) );
    }
/* humidity */
    if (g_hash_table_lookup(current, "humidity"))
        if( strcmp(g_hash_table_lookup(current, "humidity"), "N/A") ){
            sprintf(buffer + strlen(buffer), "%s", _("Humidity:"));
            sprintf(buffer + strlen(buffer), "  %d%%\n",
                    atoi(g_hash_table_lookup(current, "humidity")));
        }
/* visible */
    if (g_hash_table_lookup(current, "visible")){
        if( strcmp(g_hash_table_lookup(current, "visible"), "N/A") ){
            sprintf(buffer + strlen(buffer), "%s", _("Visible:"));
            if( !strcmp(g_hash_table_lookup(current, "visible"), "Unlimited") )
                sprintf(buffer + strlen(buffer), "  %s\n",
                        (char*)hash_table_find("Unlimited", FALSE));
            else{
                tmp_distance = atof(g_hash_table_lookup(current, "visible"));
                switch(app->config->distance_units){
                    default:
                    case METERS: units = _("m"); tmp_distance *= 1000.0f; break;
                    case KILOMETERS: units = _("km"); tmp_distance *= 1.0f; break;
                    case MILES: units = _("mi"); tmp_distance /= 1.609344f; break;
                    case SEA_MILES: units = _("mi"); tmp_distance /= 1.852f; break;
                }
                sprintf(buffer + strlen(buffer), "  %.2f %s\n", tmp_distance, units);
            }
        }
    }
/* pressure */
    if (g_hash_table_lookup(current, "pressure")){
        if( strcmp(g_hash_table_lookup(current, "pressure"), "N/A") ){
            sprintf(buffer + strlen(buffer), "%s", _("Pressure:"));
            tmp_pressure = atof(g_hash_table_lookup(current, "pressure"));
            switch(app->config->pressure_units){
                default:
                case MB: units = _("mb"); break;
                case INCH: units = _("inHg"); tmp_pressure = mb2inch(tmp_pressure); break;
                case MM: units = _("mmHg"); tmp_pressure = mb2mm(tmp_pressure); break;
            }
            sprintf(buffer + strlen(buffer), "  %.2f %s,", tmp_pressure, units);
            if (g_hash_table_lookup(current, "pressure_direction"))
                sprintf(buffer + strlen(buffer), "  %s\n",
                    (char *)hash_table_find(g_hash_table_lookup(current, "pressure_direction"), FALSE));
            else
                sprintf(buffer + strlen(buffer), "\n");
        }
    }
/* wind */
    if (g_hash_table_lookup(current, "wind_direction"))
    if( strcmp(g_hash_table_lookup(current, "wind_direction"), "N/A") ){
        sprintf(buffer + strlen(buffer), "%s", _("Wind:"));
        sprintf(buffer + strlen(buffer), "  %s\n",
                    (char *)hash_table_find(g_hash_table_lookup(current, "wind_direction"), FALSE));
        if (g_hash_table_lookup(current, "wind_speed")){
            if( strcmp(g_hash_table_lookup(current, "wind_speed"), "N/A") )
                sprintf(buffer + strlen(buffer), "%s", _("Speed:"));
                switch(app->config->wind_units){
                    case BEAUFORT_SCALE:
                        sprintf(buffer + strlen(buffer), "  %i\n",
                                (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(current, "wind_speed"))));
                    break;
                    case KILOMETERS_H:
                        sprintf(buffer + strlen(buffer), "  %i %s\n",
                                (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(current, "wind_speed"))),
                                (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                    break;
                    default:
                        sprintf(buffer + strlen(buffer), "  %.2f %s\n",
                                convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(current, "wind_speed"))),
                                (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                    break;
                }
        }
    }
/* gust */
    if (g_hash_table_lookup(current, "wind_gust"))
        if( strcmp(g_hash_table_lookup(current, "wind_gust"), "N/A") ){
            sprintf(buffer + strlen(buffer), "%s", _("Gust:"));
            sprintf(buffer + strlen(buffer), "  %.2f %s\n",
                    convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(current, "wind_gust"))),
                    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
        }

    text = gtk_label_new(buffer);
    set_font(text, NULL, font_size);
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), text, TRUE, TRUE, 0);
#ifndef OS2009
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), create_moon_phase_widget(current),
                        TRUE, TRUE, 0);
#endif
    gtk_box_pack_start(GTK_BOX(main_widget), icon_text_hbox, TRUE, TRUE, 0);
#ifdef OS2009
    if (app->config->scale_in_popup == 6)
        gtk_box_pack_start(GTK_BOX(main_widget), create_moon_phase_widget(current),
                        TRUE, TRUE, 0);
    else
        gtk_box_pack_start(GTK_BOX(icon_text_hbox), create_moon_phase_widget(current),
                        TRUE, TRUE, 0);
#endif
    if(app->config->mode != SIMPLE_MODE)
        /* last update time */
        gtk_box_pack_start(GTK_BOX(main_widget),
                            create_time_updates_widget(current, FALSE),
                            TRUE, FALSE, 5);
    gtk_widget_show_all(main_widget);

#if defined CLUTTER
    g_signal_connect_after(main_widget, "expose-event",
         G_CALLBACK(popup_window_expose), NULL);
#endif

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_hour_tab(void){
    GtkWidget       *main_widget = NULL,
                    *window = NULL,
                    *window_tmp = NULL, 
                    *icon_text_hbox = NULL,
                    *text = NULL,
                    *icon_image = NULL;
    GHashTable      *hour_weather = NULL;
    GSList          *tmp = NULL;
    gchar           buffer[1024];
    const gchar     *wind_units_str[] = { "m/s", "km/h", "mi/h" };
    GdkPixbuf       *icon = NULL;
    gint            i = 0,
                    period = 6;

#ifdef DEBUGFUNCTIONCALL 
    START_FUNCTION;
#endif
    if(!g_hash_table_lookup(app->station_data, "detail"))
        return NULL;

    main_widget = gtk_vbox_new(FALSE, 0);
    window_tmp = gtk_hbox_new(FALSE, 0);
    tmp = g_hash_table_lookup(g_hash_table_lookup(app->station_data, "detail"), "hours_data");

    while(tmp){
        hour_weather = (GHashTable*)tmp->data;
        window = gtk_vbox_new(FALSE, 0);
        icon_text_hbox = gtk_hbox_new(FALSE, 0);
/* icon */
        sprintf(buffer,"%s%s.png", app->config->icons_set_base,
                (char*)g_hash_table_lookup(hour_weather, "hour_icon"));
        icon = gdk_pixbuf_new_from_file_at_size(buffer, SMALL_ICON_SIZE,
                                                   SMALL_ICON_SIZE, NULL);
//        icon_image = create_icon_widget(icon, buffer, SMALL_ICON_SIZE, &app->clutter_objects_in_popup_form);
        icon_image = create_icon_widget(icon, buffer, SMALL_ICON_SIZE, NULL);
        gtk_box_pack_start(GTK_BOX(icon_text_hbox), icon_image, TRUE, FALSE, 0);
        memset(buffer, 0, sizeof(buffer));
/* hour */
        sprintf(buffer + strlen(buffer), "%s", _("Forecast at: "));
/* TODO fix this item to correct displaing localized time */
        sprintf(buffer + strlen(buffer), "%s:00\n",
                                (char*)g_hash_table_lookup(hour_weather, "hours"));
/* title */
        sprintf(buffer + strlen(buffer), "%s\n",
                        (char*)hash_table_find(g_hash_table_lookup(hour_weather, "hour_title"), FALSE));
/* temperature */
        sprintf(buffer + strlen(buffer), "%s",  _("Temperature: "));
        sprintf(buffer + strlen(buffer), " %d\302\260",
                   ((app->config->temperature_units == CELSIUS) ?
                   ( atoi(g_hash_table_lookup(hour_weather, "hour_temperature"))) :
                   ( (int)c2f(atoi(g_hash_table_lookup(hour_weather, "hour_temperature"))))));
        (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")))
                                  : ( strcat(buffer, _("F\n")));
/* feels like */
        sprintf(buffer + strlen(buffer), "%s", _("Feels like:"));
        sprintf(buffer + strlen(buffer), "  %d\302\260",
                  (app->config->temperature_units == CELSIUS) ?
                  (atoi(g_hash_table_lookup(hour_weather, "hour_feels_like"))) :
                  ((int)c2f(atoi(g_hash_table_lookup(hour_weather, "hour_feels_like")))));
        (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")))
                                                    : ( strcat(buffer, _("F\n")));
   /* humidity */
        sprintf(buffer + strlen(buffer), "%s", _("Humidity:"));
        if( strcmp(g_hash_table_lookup(hour_weather, "hour_humidity"), "N/A") ){
            sprintf(buffer + strlen(buffer), "  %d%%\n",
                           atoi(g_hash_table_lookup(hour_weather, "hour_humidity")));
        }
        else{
            sprintf(buffer + strlen(buffer), "%s\n",
                            (char*)hash_table_find("N/A", FALSE));
        }
   /* wind */
        if( strcmp(g_hash_table_lookup(hour_weather, "hour_wind_direction"), "N/A") ){
            sprintf(buffer + strlen(buffer), "%s", _("Wind:"));
            sprintf(buffer + strlen(buffer), "  %s\n",
                        (char*)hash_table_find(g_hash_table_lookup(hour_weather, "hour_wind_direction"), TRUE));
            if( strcmp(g_hash_table_lookup(hour_weather, "hour_wind_speed"), "N/A") )
                sprintf(buffer + strlen(buffer), "%s", _("Speed:"));
                switch(app->config->wind_units){
                    case BEAUFORT_SCALE:
                        sprintf(buffer + strlen(buffer), "  %i\n",
                                (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(hour_weather, "hour_wind_speed"))));
                    break;
                    case KILOMETERS_H:
                        sprintf(buffer + strlen(buffer), "  %i %s\n",
                                (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(hour_weather, "hour_wind_speed"))),
                                (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                    break;
                    default:
                        sprintf(buffer + strlen(buffer), "  %.2f %s\n",
                                convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(hour_weather, "hour_wind_speed"))),
                                (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                    break;
                }
        }
   /* gust */
        if( g_hash_table_lookup(hour_weather, "hour_wind_gust")&&
            strcmp(g_hash_table_lookup(hour_weather, "hour_wind_gust"), "N/A") ){
            sprintf(buffer + strlen(buffer), "%s", _("Gust:"));
            sprintf(buffer + strlen(buffer), "  %.2f %s\n",
                 convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(hour_weather, "hour_wind_gust"))),
                 (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
        }
        text = gtk_label_new(buffer);
        set_font(text, NULL, 12);
        gtk_box_pack_start(GTK_BOX(window), icon_text_hbox, TRUE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(window), text, TRUE, FALSE, 0); 
        gtk_box_pack_start(GTK_BOX(window_tmp), window, TRUE, FALSE, 0);
        for (i = 1; i < period; i++){
            tmp = g_slist_next(tmp);
            if(!tmp)
                break;
        }
   }

    gtk_box_pack_start(GTK_BOX(main_widget), window_tmp, TRUE, TRUE, 0);
   /* last update time */
    if(hour_weather)
        gtk_box_pack_start(GTK_BOX(main_widget),
                             create_time_updates_widget(hour_weather, FALSE),
                             TRUE, FALSE, 5);
    gtk_widget_show_all(main_widget);

#if defined CLUTTER
    g_signal_connect_after(main_widget, "expose-event",
         G_CALLBACK(popup_window_expose), NULL);
#endif

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return main_widget;
}
/*******************************************************************************/
#ifdef ENABLE_BROWSER_INTERFACE
void
click_url(GtkButton *button) {
  browser_url(gtk_button_get_label(button));
}
#endif

/*******************************************************************************/
GtkWidget* create_copyright_widget(const gchar *text, const gchar *image){
    GtkWidget       *main_widget = NULL,
                    *hbox = NULL,
                    *label= NULL,
                    *url = NULL,
                    *icon = NULL;
    GdkPixbuf       *icon_buffer = NULL;
    gchar           image_buffer[512],
                    text_buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_widget = gtk_vbox_new(FALSE, 0);
    hbox = gtk_hbox_new(FALSE, 0);
/* text */
    gtk_box_pack_start(GTK_BOX(hbox),
                        label = gtk_label_new(_("Weather data provided by: ")),
                        FALSE, FALSE, 5);
    set_font(label, NULL, 12);
/* url */
    *text_buffer = 0;
    snprintf(text_buffer, sizeof(text_buffer) - 1, "http://%s",
                text);
    url = gtk_button_new_with_label(text_buffer);
    gtk_button_set_relief(GTK_BUTTON(url), GTK_RELIEF_NONE);
    g_signal_connect(url, "clicked",
          G_CALLBACK(click_url), text_buffer);
    gtk_button_set_focus_on_click(GTK_BUTTON(url), FALSE);
    gtk_box_pack_start(GTK_BOX(hbox), url, FALSE, FALSE, 5);
    set_font(url, NULL, 12);
/* icon */
    if(image){
        sprintf(image_buffer, "%s%s", COPYRIGHT_ICONS, image);
        icon_buffer = gdk_pixbuf_new_from_file_at_size(image_buffer,
                                                        SMALL_ICON_SIZE,
                                                        SMALL_ICON_SIZE,
                                                        NULL);
        if(icon_buffer){
            icon = gtk_image_new_from_pixbuf(icon_buffer);
            g_object_unref(G_OBJECT(icon_buffer));
        }
        else
            icon = NULL;
        if(icon)
            gtk_box_pack_start(GTK_BOX(hbox), icon, FALSE, FALSE, 0);
    }
    gtk_box_pack_start(GTK_BOX(main_widget), hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(main_widget), gtk_hseparator_new(), FALSE, TRUE, 5);

    return main_widget;
}
/*******************************************************************************/
GtkWidget*
create_window_header(const gchar *station_name, GtkWidget *popup_window){
    GtkWidget       *main_widget = NULL,
                    *vbox = NULL,
                    *label = NULL,
                    *previous_button = NULL,
                    *next_button = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_widget = gtk_hbox_new(FALSE, 0);
    vbox = gtk_vbox_new(FALSE,20);
/* station name */
    label = gtk_label_new(station_name);
    set_font(label, NULL, 28);
/* previous button */
    previous_button = create_button_with_image(BUTTON_ICONS, "left_arrow",
                                                26, FALSE, FALSE);
/* next button */
    next_button = create_button_with_image(BUTTON_ICONS, "right_arrow",
                                                26, FALSE, FALSE);
    if(previous_button){
        gtk_box_pack_start(GTK_BOX(main_widget), previous_button, FALSE, FALSE, 10);
        g_signal_connect(G_OBJECT(previous_button), "button_press_event",
                            G_CALLBACK(change_station_prev), popup_window);
    }
    if(label)
        gtk_box_pack_start(GTK_BOX(main_widget), label, TRUE, TRUE, 0);
    if(next_button){
        gtk_box_pack_start(GTK_BOX(main_widget), next_button, FALSE, FALSE, 10);
        g_signal_connect(G_OBJECT(next_button), "button_press_event",
                            G_CALLBACK(change_station_next), popup_window);
    }
    gtk_box_pack_start(GTK_BOX(vbox), main_widget, FALSE, FALSE, 20);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return vbox;
}
/*******************************************************************************/
#ifdef CLUTTER
/* For start of Clutter animation in popup window */
void
popup_window_expose(GtkWidget *widget, GdkEventExpose *event){
    show_animation(app->clutter_objects_in_popup_form);
    gtk_widget_show_all(widget);
    g_signal_handlers_disconnect_by_func(G_OBJECT(widget),G_CALLBACK(popup_window_expose),NULL);
}
#endif
/*******************************************************************************/
#ifdef HILDONANIMATION
/* For start of Clutter animation in popup window */
void
popup_window_expose(GtkWidget *widget, GdkEventExpose *event){
    show_hildon_animation(app->clutter_objects_in_popup_form, app->popup_window);
    gtk_widget_show_all(widget);
    g_signal_handlers_disconnect_by_func(G_OBJECT(widget),G_CALLBACK(popup_window_expose),NULL);
}
#endif

/*******************************************************************************/
