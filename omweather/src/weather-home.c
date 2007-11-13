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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
/*******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <glib.h>
#if HILDON == 1
    #include <libhildondesktop/libhildondesktop.h>
#else
    #include <hildon-home-plugin/hildon-home-plugin-interface.h>
#endif
#include "weather-home.h"
#include <unistd.h>
#include "weather-home_hash.h"
#include "weather-data.h"
/*******************************************************************************/
#define APPLET_X_SIZE		200
#define APPLET_Y_SIZE		40
#define FONT_MAIN_SIZE_LARGE	14
#define FONT_MAIN_SIZE_MEDIUM	12
#define FONT_MAIN_SIZE_SMALL	10
/*******************************************************************************/
/* main struct */
OMWeatherApp	*app = NULL;
int		boxs_offset[Max_count_weather_day];
gchar		path_large_icon[_POSIX_PATH_MAX];
/*******************************************************************************/
/* Translate  temperature Celsius to Farenhait */
int c2f(int temp){
    return (temp * 1.8f ) + 32;
}
/*******************************************************************************/
/* Set font size. Usually on label widget */
void set_font_size(GtkWidget *widget, char font_size){
    PangoFontDescription *pfd = NULL;
    if (!widget)
	return;
    pfd = pango_font_description_copy( 
            pango_context_get_font_description(gtk_widget_get_pango_context(widget)));
    pango_font_description_set_absolute_size(pfd, font_size * PANGO_SCALE);	    
    gtk_widget_modify_font(GTK_WIDGET(widget), NULL);   /* this function is leaking */
    gtk_widget_modify_font(GTK_WIDGET(widget), pfd);   /* this function is leaking */
    pango_font_description_free(pfd);
}
/*******************************************************************************/
/* Set font color. Usually on label widget */
void set_font_color(GtkWidget *widget, guint16 red, guint16 green, guint16 blue){
    PangoAttribute	*attr;
    PangoAttrList	*attrs = NULL;

    if(!widget)
	return;

    attrs = pango_attr_list_new();
    attr = pango_attr_foreground_new(red,green,blue);
    attr->start_index = 0;
    attr->end_index = G_MAXUINT;
    pango_attr_list_insert(attrs, attr);
    /* Set the attributes */
    g_object_set(widget, "attributes", attrs, NULL);
    pango_attr_list_unref(attrs);
}
/*******************************************************************************/
/* Change station to previos at main display */
static gboolean change_station_prev(GtkWidget *widget,
                    		    GdkEvent *event,
                    		    gpointer user_data){

    GSList			*tmplist = NULL;
    GSList			*tmplist_prev = NULL;
    struct weather_station	*ws;

    tmplist = app->stations_view_list;
    while(tmplist){
	ws = tmplist->data;
	/* Check active station */ 
	if((app->config->current_station_id != NULL) &&
		!strcmp(app->config->current_station_id, ws->id_station)){
	    /* Get last station if no previos station */
	    if(!tmplist_prev) 
    		tmplist_prev = g_slist_last(tmplist);
	    /* Get station data */
	    ws = tmplist_prev->data;
	    if(app->config->current_station_id)
    		g_free(app->config->current_station_id);
    	    app->config->current_station_id = g_strdup(ws->id_station); 
	    /* update current station name */
    	    if(app->config->current_station_name)
		g_free(app->config->current_station_name);
	    ws->name_station && (app->config->current_station_name = strdup(ws->name_station));
	    app->config->previos_days_to_show = app->config->days_to_show;
	    weather_frame_update(TRUE);
	    new_config_save(app->config);
	    break;
	} 
	tmplist_prev = tmplist;
	/* Next station in list */
	tmplist = g_slist_next(tmplist);
    }
    return TRUE;
}
/*******************************************************************************/
/* Change station to next at main display */
static gboolean change_station_next(GtkWidget *widget,
                    		    GdkEvent *event,
                    		    gpointer user_data){

    GSList 			*tmplist = NULL;
    struct weather_station	*ws;

    tmplist = app->stations_view_list;
    while(tmplist){
	ws = tmplist->data;
	/* Check active station */ 
	if((app->config->current_station_id != NULL) &&
		!strcmp(app->config->current_station_id, ws->id_station)){
	    tmplist = g_slist_next(tmplist);
	    /* If no next station, get first */
	    if(!tmplist)
    		tmplist = app->stations_view_list;
	    /* Get station data */
	    ws = tmplist->data;
	    if(app->config->current_station_id)
    		g_free(app->config->current_station_id);
	    app->config->current_station_id = g_strdup(ws->id_station); 
	    /* update current station name */
	    if(app->config->current_station_name)
		g_free(app->config->current_station_name);
	    ws->name_station && (app->config->current_station_name = strdup(ws->name_station));
	    app->config->previos_days_to_show = app->config->days_to_show;
	    weather_frame_update(TRUE);
	    new_config_save(app->config);
	    break;
	}
	/* Next station in list */
	tmplist = g_slist_next(tmplist);
    }
    return TRUE;
}
/*******************************************************************************/
gboolean change_station_select(GtkWidget *widget,
                    		    gpointer user_data){

    GSList 			*tmplist = NULL;
    struct weather_station	*ws;
    
    tmplist = app->stations_view_list;
    
    while(tmplist){
	ws = tmplist->data;
	/* Check active station */ 
	if(!strcmp((char *)user_data, ws->id_station)){
	    if(app->config->current_station_id)
    		g_free(app->config->current_station_id);
	    app->config->current_station_id = g_strdup(ws->id_station); 
	    /* update current station name */
	    if(app->config->current_station_name)
		g_free(app->config->current_station_name);
	    ws->name_station && (app->config->current_station_name = strdup(ws->name_station));
	    app->config->previos_days_to_show = app->config->days_to_show;
	    weather_frame_update(TRUE);
	    new_config_save(app->config);
	    break;		
	}		
	/* Next station in list */
	tmplist = g_slist_next(tmplist);
    }
    return TRUE;
}
/*******************************************************************************/
/* Set default value */
void weather_buttons_init(void){
    int i;
/* Set default icon N/A */
    for(i = 0; i < app->config->days_to_show; i++){
	memset(&app->weather_days[i], 0, sizeof(weather_day));
	app->weather_days[i].night.icon = 48;
	app->weather_days[i].day.icon = 48;
	sprintf(app->weather_days[i].hi_temp, (char*)hash_table_find("N/A"));
	sprintf(app->weather_days[i].low_temp, (char*)hash_table_find("N/A"));
    }
    memset(&app->weather_current_day, 0, sizeof(weather_day));
}
/*******************************************************************************/
/* Error Window */
void station_error_window(void){
   hildon_banner_show_information(app->main_window,
				    NULL,
				    _("Wrong station code \nor ZIP code!!!"));
}
/*******************************************************************************/
/* Fill buttons data */
void weather_buttons_fill(gboolean check_error){
    int		i,
		j = 0,
		k = 0,
		offset = 0,
		count_day,
		year,
		current_month = 1;
    gchar	buffer[2048],
		buffer_icon[2048],
		date_in_string[255];
    time_t	current_day,
		current_time,
		last_day = 0,
		utc_time,
		date_time,
		day_begin_time,
		night_begin_time;
    long int    diff_time;
    struct tm	*tm = NULL,
		tmp_tm;
    gboolean	flag_last_day = FALSE, error_station_code = FALSE;
    GSList	*tmplist = NULL;
    struct weather_station *ws = NULL;
    int		temp_hi = 0, temp_low = 0, temp_current = 0;
    char	font_size;
    gint	icon_size;
    gchar	*tmp_station_name;

#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
#endif
/* Check main widget */
    if(!app->top_widget)
	return;
    tm = &tmp_tm;
/* select image and font size */
    switch(app->config->icons_size){
	default:
	case LARGE: 
	    font_size = FONT_MAIN_SIZE_LARGE;
	    icon_size = 64;
	break;
	case MEDIUM:
	    font_size = FONT_MAIN_SIZE_MEDIUM;
	    icon_size = 48;
	break;
	case SMALL:
	    font_size = FONT_MAIN_SIZE_SMALL;
	    icon_size = 32;
	break;
    }
/* Init weather buttons */
    weather_buttons_init();
    count_day = new_parse_weather_com_xml();
    
    if(check_error)
	if(count_day == -2){
	    count_day = 0;
	    fprintf(stderr, _("Error in xml file\n"));
	    error_station_code = TRUE;
	} /* Error in xml file */
/* Clear daytime elments in queue of timer */
    remove_daytime_event();
/* get current day */  
    current_time = time(NULL);
    utc_time = mktime(gmtime(&current_time));
    diff_time = utc_time - current_time + atol(item_value(wcs.day_data[0], "station_time_zone"));
    current_time = current_day = utc_time + atol(item_value(wcs.day_data[0], "station_time_zone"));
    tm = localtime(&current_day);
    year = 1900 + tm->tm_year;
    current_month = tm->tm_mon;
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    current_day = mktime(tm);

    memset(date_in_string, 0, sizeof(date_in_string));
    sprintf(date_in_string, "%s %i 00:00:00",
	    item_value(wcs.day_data[0], "24h_date"), year);

    strptime(date_in_string, "%b %d %Y %T", tm);
    /* Check New Year */
    if((current_month == 11) && (tm->tm_mon == 0)){
	sprintf(date_in_string, "%s %i 00:00:00",
		item_value(wcs.day_data[count_day - 1], "24h_date"), year + 1);
	strptime(date_in_string, "%b %d %Y %T", tm);
    }
    date_time = mktime(tm);

    offset = (int)( abs( (current_day - date_time) / (24 * 60 * 60) ) );

    if(offset >= Max_count_weather_day)
	offset = Max_count_weather_day;
    /* delete old weather data */
    for(i = 0; i < offset; i++)
	destroy_object(&(wcs.day_data[i]));
    /* move weather data */
    j = i;
    if(j > 0){
	for(; j < Max_count_weather_day; j++){
	    wcs.day_data[j - i] = wcs.day_data[j];
	    wcs.day_data[j] = NULL;
	}
    }
    j = 0;
    (app->config->separate) ? (k = 1) : (k = 0); /* add one more day if data is config->separate */
    for(i = 0; i < app->config->days_to_show; i++){
	if( i < Max_count_weather_day - offset + k ){
	    if(i == 0 || (app->config->separate && i == 1)){	/* first day */
		/* repeat the same data in second button for first day if data is config->separate */
		(app->config->separate && i == 1) ? (j = -1) : (j = 0);
		/* prepare temperature for first day */
		if(!strcmp(item_value(wcs.day_data[i + j], "24h_hi_temperature"), "N/A"))
		    temp_hi = INT_MAX;
		else
		    temp_hi = atoi(item_value(wcs.day_data[i + j], "24h_hi_temperature"));

		if(!strcmp(item_value(wcs.day_data[i + j], "24h_low_temperature"), "N/A"))
        	    temp_low = INT_MAX;
		else
		    temp_low = atoi(item_value(wcs.day_data[i + j], "24h_low_temperature"));
		if(!strcmp(item_value(wcs.current_data, "temperature"), "N/A"))
		    temp_current = INT_MAX;
		else
		    temp_current = atoi(item_value(wcs.current_data, "temperature"));
        	if(app->config->temperature_units == FAHRENHEIT){
		    if(temp_hi != INT_MAX)
            		temp_hi = c2f(temp_hi);
		    if(temp_low != INT_MAX)
            		temp_low = c2f(temp_low);
		    if(temp_current != INT_MAX)
			temp_current = c2f(temp_current);
        	}
	
		/* day begin */
		sprintf(date_in_string, "%s %i %s",
			item_value(wcs.day_data[i + j], "24h_date"),
			year,
			item_value(wcs.day_data[i + j], "24h_sunrise"));
		strptime(date_in_string, "%b %d %Y %I:%M %p", tm);
		day_begin_time = mktime(tm);
		/* night begin */
		sprintf(date_in_string, "%s %i %s",
			item_value(wcs.day_data[i + j], "24h_date"),
			year,
			item_value(wcs.day_data[i + j], "24h_sunset"));
		strptime(date_in_string, "%b %d %Y %I:%M %p", tm);
		night_begin_time = mktime(tm);
		/* add events for first day */
		if(current_time < day_begin_time)
        	    time_event_add(day_begin_time - diff_time, DAYTIMEEVENT);
		if(current_time < night_begin_time)
            	    time_event_add(night_begin_time - diff_time, DAYTIMEEVENT);
		
		current_time = utc_time + atol(item_value(wcs.day_data[i + j], "station_time_zone"));
		#ifndef RELEASE
		fprintf(stderr, "\nUTC time %s", ctime(&utc_time));
		fprintf(stderr, "Zone time %s", ctime(&current_time));
		fprintf(stderr, "\nLast update: %li\n", last_update_time(wcs.current_data));
		#endif
		/* check weather data for actuality */
		if( (last_update_time(wcs.current_data) > (current_time - app->config->data_valid_interval - 3600)) &&
            	    (last_update_time(wcs.current_data) < (current_time + app->config->data_valid_interval + 3600)) && i == 0){

		    time_event_add(last_update_time(wcs.current_data) + app->config->data_valid_interval - diff_time + 3600, DAYTIMEEVENT);
		    if(temp_current == INT_MAX)
			sprintf(buffer,
				"<span weight=\"bold\" foreground='#%02x%02x%02x'>%s\n%s\302\260\n</span>",
				app->config->font_color.red >> 8,
				app->config->font_color.green >> 8,
				app->config->font_color.blue >> 8,
				(app->config->separate) ? (_("Now")) : (item_value(wcs.day_data[i + j], "24h_name")),
				_("N/A") );
		    else
			sprintf(buffer,
				"<span weight=\"bold\" foreground='#%02x%02x%02x'>%s\n%i\302\260\n</span>",
				app->config->font_color.red >> 8,
				app->config->font_color.green >> 8,
				app->config->font_color.blue >> 8,
				(app->config->separate) ? (_("Now")) : (item_value(wcs.day_data[i + j], "24h_name")),
				temp_current );
		    sprintf(buffer_icon, "%s%s.png", path_large_icon, item_value(wcs.current_data, "icon"));
		}
		else{ /* if current data is not actual */
		    if(i == 0 && app->config->separate){ /* if current data isn't actual and first day */
		    
		    	sprintf(buffer, "<span foreground='#%02x%02x%02x'>%s\n%s\302\260</span>",
				    app->config->font_color.red >> 8,
				    app->config->font_color.green >> 8,
				    app->config->font_color.blue >> 8,
				    _("Now"),
				    _("N/A"));
			sprintf(buffer_icon, "%s48.png", path_large_icon);
		    }
		    else{ /* if first day and not config->separate data */
			/* if current time is night show night icon */
			if(current_time < day_begin_time)
			    sprintf(buffer_icon, "%s%s.png", path_large_icon, item_value(wcs.day_data[i + j], "night_icon"));
			else{/* if current time is day show day icon */
			    if(current_time < night_begin_time)
				sprintf(buffer_icon, "%s%s.png", path_large_icon, item_value(wcs.day_data[i + j], "day_icon"));
			    else
				sprintf(buffer_icon, "%s%s.png", path_large_icon, item_value(wcs.day_data[i + j], "night_icon"));
			}
			/* show temperature */
			if(app->config->swap_hi_low_temperature)
			    swap_temperature(&temp_hi, &temp_low);
			sprintf(buffer,
				"<span foreground='#%02x%02x%02x'>%s\n",
				app->config->font_color.red >> 8,
				app->config->font_color.green >> 8,
				app->config->font_color.blue >> 8,
				item_value(wcs.day_data[i + j], "24h_name"));
			if(temp_low == INT_MAX)
			    sprintf(buffer + strlen(buffer), "%s\302\260\n", _("N/A") );
			else
			    sprintf(buffer + strlen(buffer), "%i\302\260\n", temp_low );
			if(temp_hi == INT_MAX)
			    sprintf(buffer + strlen(buffer), "%s\302\260", _("N/A") );
			else
			    sprintf(buffer + strlen(buffer), "%i\302\260", temp_hi );
			strcat(buffer, "</span>");
		    }
		}

	    }
	    else{ /* other days, from two and to app->config->days_to_show */
	    	if(!strcmp(item_value(wcs.day_data[i + j], "24h_hi_temperature"), "N/A"))
		    temp_hi = INT_MAX;
		else
		    temp_hi = atoi(item_value(wcs.day_data[i + j], "24h_hi_temperature"));
		if(!strcmp(item_value(wcs.day_data[i + j], "24h_low_temperature"), "N/A"))
        	    temp_low = INT_MAX;
		else
		    temp_low = atoi(item_value(wcs.day_data[i + j], "24h_low_temperature"));
        	if(app->config->temperature_units == FAHRENHEIT){
		    if(temp_hi != INT_MAX)
            		temp_hi = c2f(temp_hi);
		    if(temp_low != INT_MAX)
            		temp_low = c2f(temp_low);
        	}
	        time_event_add(date_time, DAYTIMEEVENT);
                last_day = date_time;
		if(app->config->swap_hi_low_temperature)
		    swap_temperature(&temp_hi, &temp_low);
		sprintf(buffer_icon, "%s%s.png", path_large_icon, item_value(wcs.day_data[i + j], "day_icon"));
		sprintf(buffer,
			"<span foreground='#%02x%02x%02x'>%s\n",
			app->config->font_color.red >> 8,
			app->config->font_color.green >> 8,
			app->config->font_color.blue >> 8,
			item_value(wcs.day_data[i + j], "24h_name"));
		if(temp_low == INT_MAX)
		    sprintf(buffer + strlen(buffer), "%s\302\260\n", _("N/A") );
		else
		    sprintf(buffer + strlen(buffer), "%i\302\260\n", temp_low );
		if(temp_hi == INT_MAX)
		    sprintf(buffer + strlen(buffer), "%s\302\260", _("N/A") );
		else
		    sprintf(buffer + strlen(buffer), "%i\302\260", temp_hi );
		strcat(buffer, "</span>");		
	    }
	    boxs_offset[i] = i + offset + j;
	}
	else{ /* Show N/A for all others day buttons when it not inside range */
	    sprintf(buffer, "<span foreground='#%02x%02x%02x'>%s\n%s\302\260\n%s\302\260</span>",
			app->config->font_color.red >> 8,
			app->config->font_color.green >> 8,
			app->config->font_color.blue >> 8,
			_("N/A"), _("N/A"), _("N/A"));
	    sprintf(buffer_icon, "%s48.png", path_large_icon);
	    if(!flag_last_day && last_day){
		time_event_add(last_day + 24 * 60 * 60, DAYTIMEEVENT);
		flag_last_day = TRUE;
	    }
	    boxs_offset[i] = Max_count_weather_day;
    	}
	app->buttons[i] = create_weather_day_button(buffer, buffer_icon, icon_size,
						    app->config->transparency,
						    font_size, &(app->config->background_color));

	if(app->buttons[i]){
	    g_signal_connect(app->buttons[i]->button, "button_release_event", G_CALLBACK(show_popup_window_handler), NULL); 
	}    

    }/* end for */

    if(g_slist_length(app->stations_view_list) > 0){
	tmplist = app->stations_view_list;
/* search current station */
        while(tmplist){
    	    ws = tmplist->data;
	    if((ws->id_station) && (app->config->current_station_id)
				&& !strcmp(ws->id_station, app->config->current_station_id)) 
		break;
	    tmplist = g_slist_next(tmplist);
	}
	tmp_station_name = ws->name_station;
    }
    else{
	if(ws)
	    ws->name_station = NULL;
	tmp_station_name = NULL;
    }
/* create main panel */
    app->main_window = gtk_table_new(2, 1, FALSE);
    create_panel(app->main_window, app->config->icons_layout,
		    app->config->transparency, tmp_station_name, font_size);
    gtk_box_pack_start(GTK_BOX(app->top_widget), app->main_window, TRUE, TRUE, 0);


    gtk_widget_show_all(app->top_widget);
    if(error_station_code)
	station_error_window();
}
/*******************************************************************************/
void weather_frame_update(gboolean check){

    free_memory(FALSE);
    if(app->main_window){
        gtk_widget_destroy(app->main_window);
	app->main_window = NULL;
    }
    if(check) 
	weather_buttons_fill(TRUE);
    else
	weather_buttons_fill(FALSE);
}
/*******************************************************************************/
/* Get Weather xml file from weather.com */
void update_weather(void){
    if (app->flag_updating == 0)
	app->flag_updating = g_timeout_add(100, (GSourceFunc)download_html, NULL);
}
/*******************************************************************************/
void* hildon_home_applet_lib_initialize(void *state_data, 
					int *state_size,
					GtkWidget **widget){
    osso_context_t	*osso;
    
    osso = osso_initialize(PACKAGE, VERSION, FALSE, NULL);
    if(!osso){
        g_debug(_("Error initializing the OMWeather applet"));
        return NULL;
    }
    #ifndef RELEASE
	fprintf(stderr, "\nOMWeather applet initialize %p %d\n",
			state_data, *state_size);
    #endif
    app = g_new0(OMWeatherApp, 1);
    if(!app){
	fprintf(stderr, "\nCan not allocate memory for applet.\n");
	exit(1);
    }
    memset(app, 0, sizeof(OMWeatherApp));
    app->osso = osso;
    app->flag_updating = 0;
/* create i18n hash for values coming from xml file */
    app->hash = hash_table_create();
    app->dbus_is_initialize = FALSE;
/* prepare config struct */
    app->config = g_new0(AppletConfig, 1);
    if(!app->config){
        fprintf(stderr, "\nCan not allocate memory for config.\n");
        g_free(app);
        exit(1);
    }
/* Init gconf. */
    gnome_vfs_init();
    if(new_read_config(app->config)){
        fprintf(stderr, "\nCan not read config file.\n");
        g_free(app->config);
        g_free(app);
        exit(1);
    }

    app->time_update_list = create_time_update_list();
    app->show_update_window = FALSE;
    app->countrys_list = NULL;
    app->countrys_list = create_items_list(COUNTRYSFILE, -1, -1, NULL);
/* Start timer */
/*    timer(6000); */ /* One per secund */
    timer(60000); /* One per minute */

/* Start main applet */ 
    app->top_widget = gtk_hbox_new(FALSE, 0);
    weather_buttons_fill(FALSE);
/* Initialize DBUS */
    weather_initialize_dbus();
/*    
    time_event_add(time(NULL) + 5, DBUSINITEVENT);    
    add_periodic_event(time(NULL));
*/
    (*widget) = app->top_widget;
    return (void*)osso;
}
/*******************************************************************************/
int hildon_home_applet_lib_save_state(void *raw_data,
				    void **state_data, 
				    int *state_size){
    #ifndef RELEASE
	fprintf(stderr, "\nOMWeather applet save state\n");
    #endif
    (*state_data) = NULL;
    if(state_size)
	(*state_size) = 0;
    return 1;
}
/*******************************************************************************/
void hildon_home_applet_lib_background(void *raw_data){
    #ifndef RELEASE
        fprintf(stderr, "\nOMWeather applet background\n");
    #endif
}
/*******************************************************************************/
void hildon_home_applet_lib_foreground(void *raw_data){
    #ifndef RELEASE
	fprintf(stderr, "\nOMWeather applet foreground\n");
    #endif
}
/*******************************************************************************/
void hildon_home_applet_lib_deinitialize(void *applet_data){
    osso_context_t *osso;
    gboolean check;
    
    #ifndef RELEASE
	fprintf(stderr, "\nOMWeather applet deinitialize\n");
    #endif
    /* It is switch off the timer */	
    check = g_source_remove(app->timer);
    /* Clean the queue of event */ 
    free_list_time_event();
    /* If downloading then switch off it */
    if(app->flag_updating != 0){
	check = g_source_remove(app->flag_updating);
	clean_download();
    }

    new_config_save(app->config); /* Not work!!!! Only 770. Why? I am not understand why this place not run when close applet 
			On n800 this work */
			
    osso = (osso_context_t*)applet_data;
    if(app){
	app->top_widget = NULL;    
	free_memory(TRUE);
	if(app->config)
    	    g_free(app->config);
	if(app->countrys_list){
	    gtk_list_store_clear(app->countrys_list);
	    app->countrys_list = NULL;
	}
	if(app->regions_list){
	    gtk_list_store_clear(app->regions_list);
	    app->regions_list = NULL;
	}
	if(app->stations_list){
	    gtk_list_store_clear(app->stations_list);
	    app->stations_list = NULL;
	}
	if(app->time_update_list){
	    gtk_list_store_clear(app->time_update_list);
	    app->time_update_list = NULL;
	}
	g_free(app);
	app = NULL;
    }
    /* Deinitialize libosso */
    osso_deinitialize(osso);
}
/*******************************************************************************/
GtkWidget* hildon_home_applet_lib_settings(void *applet_data,
					    GtkWindow *parent){
    GtkWidget	*menu_item;

    #ifndef RELEASE
	fprintf(stderr, "\nOMWeather applet settings\n");
    #endif
    
    menu_item = gtk_menu_item_new_with_label(_("OMWeather settings"));
    g_signal_connect(menu_item, "activate",
	    G_CALLBACK(weather_window_settings), NULL);

    return menu_item;
}
/*******************************************************************************/
void menu_init(void){
    GtkWidget *menu_item;
    GSList *tmplist = NULL; /* Temporary for station list */
    struct weather_station *ws; /* Description Weather station */

#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
#endif

    app->contextmenu = gtk_menu_new();

    tmplist = app->stations_view_list;
    while(tmplist){
	ws = tmplist->data;
	gtk_menu_shell_append(GTK_MENU_SHELL(app->contextmenu),menu_item = gtk_menu_item_new_with_label(ws->name_station));
        g_signal_connect(G_OBJECT(menu_item), "activate",
                         G_CALLBACK(change_station_select), ws->id_station);
/* To do: active current station */
	tmplist = g_slist_next(tmplist);
    }

    gtk_widget_show_all(GTK_WIDGET(app->contextmenu));
    gtk_widget_tap_and_hold_setup(app->main_window, GTK_WIDGET(app->contextmenu), NULL, 0);
}
/*******************************************************************************/
/* create days panel and station name panel */
void create_panel(GtkWidget* panel, gint layout, gboolean transparency,
					    gchar* st_name, char f_size){
    gchar	buffer[2048];
    GtkWidget	*header_panel,
		*days_panel,
		*previos_station_name_btn = NULL,
		*previos_station_name = NULL,
		*previos_station_box = NULL,
		*next_station_name_btn = NULL,
		*next_station_name = NULL,
		*next_station_box = NULL,
		*station_name_btn = NULL,
		*station_name = NULL,
		*station_box = NULL;

    int		n, elements, x, y;

#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
#endif
    if(app->config->days_to_show % 2)
	elements = app->config->days_to_show / 2 + 1;
    else
	elements = app->config->days_to_show / 2;
/* create header panel */
    header_panel = gtk_table_new(1, 3, FALSE);
/* check number of elements in stations list */
    if(g_slist_length(app->stations_view_list) > 1 && !app->config->hide_station_name){
	/* create previos station button */
	sprintf(buffer, "<span weight=\"bold\" foreground='#%02x%02x%02x'>&lt;</span>",
		app->config->font_color.red >> 8, app->config->font_color.green >> 8,
		app->config->font_color.blue >> 8);
	previos_station_box		= gtk_hbox_new(FALSE, 0);
	previos_station_name_btn	= gtk_event_box_new();
	set_background_color(previos_station_name_btn, &(app->config->background_color));
	
	gtk_widget_set_events(previos_station_name_btn, GDK_BUTTON_RELEASE_MASK |
    							GDK_BUTTON_PRESS_MASK);
	previos_station_name        = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(previos_station_name), buffer);
	gtk_label_set_justify(GTK_LABEL(previos_station_name), GTK_JUSTIFY_CENTER);
	set_font_size(previos_station_name, f_size);
	gtk_box_pack_start((GtkBox*) previos_station_box, previos_station_name, TRUE, TRUE, 10);
	gtk_container_add (GTK_CONTAINER(previos_station_name_btn), previos_station_box);

	buffer[0] = 0;
	/* create next station button */
	sprintf(buffer, "<span weight=\"bold\" foreground='#%02x%02x%02x'>&gt;</span>",
		app->config->font_color.red >> 8, app->config->font_color.green >> 8,
		app->config->font_color.blue >> 8);
	next_station_box		= gtk_hbox_new(FALSE, 0);
	next_station_name_btn	= gtk_event_box_new();
	set_background_color(next_station_name_btn, &(app->config->background_color));	
	
	gtk_widget_set_events(next_station_name_btn, GDK_BUTTON_RELEASE_MASK|
						     GDK_BUTTON_PRESS_MASK);
	next_station_name        	= gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(next_station_name), buffer);
	gtk_label_set_justify(GTK_LABEL(next_station_name), GTK_JUSTIFY_CENTER);
	set_font_size(next_station_name, f_size);
        gtk_box_pack_start((GtkBox*) next_station_box, next_station_name, TRUE, TRUE, 10);
	gtk_container_add (GTK_CONTAINER(next_station_name_btn), next_station_box);
    }
    buffer[0] = 0;
    if(!app->config->hide_station_name){
/* create station name button */
        if(!st_name)
	    sprintf(buffer, "<span weight=\"bold\" foreground='#%02x%02x%02x'>%s</span>",
		    app->config->font_color.red >> 8, app->config->font_color.green >> 8,
		    app->config->font_color.blue >> 8,
		    (char*)hash_table_find("NO STATION"));
        else
	    sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>%s</span>",
        	    app->config->font_color.red >> 8, app->config->font_color.green >> 8,
		    app->config->font_color.blue >> 8, st_name);
	station_box		= gtk_hbox_new(FALSE, 0);
	station_name_btn	= gtk_event_box_new();
	set_background_color(station_name_btn, &(app->config->background_color));		
	gtk_widget_set_events(station_name_btn, GDK_BUTTON_RELEASE_MASK|
						GDK_BUTTON_PRESS_MASK);
	station_name        = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(station_name), buffer);
	gtk_label_set_justify(GTK_LABEL(station_name), GTK_JUSTIFY_CENTER);
	set_font_size(station_name, f_size);
	gtk_box_pack_start((GtkBox*) station_box, station_name, TRUE, TRUE, 0);
    
	gtk_container_add(GTK_CONTAINER(station_name_btn), station_box);
    }
/* check config->transparency */
    if(transparency){
    	if(previos_station_name_btn)
	    gtk_event_box_set_visible_window(GTK_EVENT_BOX(previos_station_name_btn), FALSE);
	if(next_station_name_btn)
	    gtk_event_box_set_visible_window(GTK_EVENT_BOX(next_station_name_btn), FALSE);
	if(station_name_btn)
	    gtk_event_box_set_visible_window(GTK_EVENT_BOX(station_name_btn), FALSE);
    }
/* attach buttons to header panel */
    if(previos_station_name_btn)
	gtk_table_attach( (GtkTable*)header_panel,
			    previos_station_name_btn,
			    0, 1, 0, 1 , GTK_EXPAND, GTK_EXPAND, 0, 0);
    if(station_name_btn)
	gtk_table_attach( (GtkTable*)header_panel,
			station_name_btn,
			1, 2, 0, 1  , GTK_EXPAND, GTK_EXPAND, 0, 0); 
    if(next_station_name_btn)
	gtk_table_attach( (GtkTable*)header_panel,
			    next_station_name_btn,
			    2, 3, 0, 1, GTK_EXPAND, GTK_EXPAND, 0, 0);

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
    }
/* attach days buttons */
    for(n = 0, x = 0, y = 0; n < app->config->days_to_show; n++, x++){
      if (app->buttons[n]){
	switch(layout){
	    default:
	    case ONE_ROW:
		gtk_table_attach( (GtkTable*)days_panel, app->buttons[n]->button, n, n + 1, 0, 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0 );
	    break;
	    case ONE_COLUMN:
		gtk_table_attach( (GtkTable*)days_panel, app->buttons[n]->button, 0, 1, n, n + 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
	    break;
	    case TWO_ROWS:
		if(n == elements){
		    x = 0; y = 1;
		}
		if(!y)
		    gtk_table_attach( (GtkTable*)days_panel, app->buttons[n]->button, x, x + 1, 0, 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
		else
		    gtk_table_attach( (GtkTable*)days_panel, app->buttons[n]->button, x, x + 1, 1, 2,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
	    break;
	    case TWO_COLUMNS:
		if(n == elements){
		    x = 0; y = 1;
		}
		if(!y)
		    gtk_table_attach( (GtkTable*)days_panel, app->buttons[n]->button, 0, 1, x, x + 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
		else
		    gtk_table_attach( (GtkTable*)days_panel, app->buttons[n]->button, 1, 2, x, x + 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
	    break;
	}
      }	
    }
/* attach to main panel header and days panels */
    gtk_table_attach( (GtkTable*)panel, header_panel, 0, 1, 0, 1 ,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
    gtk_table_attach( (GtkTable*)panel, days_panel, 0, 1, 1, 2 ,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
/* Connect signal button */
    if(previos_station_name_btn){
	g_signal_connect(previos_station_name_btn, "button-release-event",
			    G_CALLBACK (change_station_prev), NULL);
    }
    if(next_station_name_btn){
	g_signal_connect(next_station_name_btn, "button-release-event",
			    G_CALLBACK (change_station_next), NULL);  		    
    }
    if(station_name_btn){
        g_signal_connect(station_name_btn, "button-release-event",
			    G_CALLBACK (change_station_next), NULL);  		    
	gtk_container_set_focus_child(GTK_CONTAINER(panel), station_name_btn); 
	menu_init();
    }
    else
	gtk_container_set_focus_child(GTK_CONTAINER(panel), panel);
    if(station_name_btn)
	g_object_unref(station_name_btn);
}

/*******************************************************************************/
/* free used memory from OMWeather struct */
void free_memory(gboolean flag){
    int    i;
    GSList *tmplist = NULL;
    struct weather_station *ws;
    
    if(flag){
	if(app->config->cache_dir_name){
	    g_free(app->config->cache_dir_name);
	    app->config->cache_dir_name = NULL;
	}
	if(app->config->icon_set){
	    g_free(app->config->icon_set);
	    app->config->icon_set = NULL;
	}
	if(app->config->current_country){
	    g_free(app->config->current_country);
	    app->config->current_country = NULL;
	}	
	if(app->config->current_station_name){
	    g_free(app->config->current_station_name);
	    app->config->current_station_name = NULL;
	}
        if(app->config->current_station_id){
	    g_free(app->config->current_station_id);
	    app->config->current_station_id = NULL;
	}
	if(app->config->iap_http_proxy_host){
	    g_free(app->config->iap_http_proxy_host);
	    app->config->iap_http_proxy_host = NULL;
	}
	if(app->hash){
	    g_hash_table_destroy(app->hash);
	    app->hash = NULL;
	}
	/* clean stations_view_list */
	if(app->stations_view_list){
	    if(g_slist_length(app->stations_view_list) > 0){
		tmplist = app->stations_view_list;
    		while(tmplist){
    		    ws = tmplist->data;
		    g_free(ws->id_station);
		    g_free(ws->name_station);
		    g_free(ws);
		    tmplist = g_slist_next(tmplist);
		}
	    }	    
	    g_slist_free(app->stations_view_list);
	    app->stations_view_list = NULL;
	}
    }

    destroy_object(&(wcs.current_data));    
    if(app->top_widget){
#ifndef RELEASE
	fprintf(stderr, "\nDays current %d\n", app->config->days_to_show);
	fprintf(stderr, "\nDays previos %d\n", app->config->previos_days_to_show);
#endif

	for(i = 0; i < app->config->previos_days_to_show; i++){
	    delete_weather_day_button(FALSE, &(app->buttons[i]) );
	    destroy_object(&(wcs.day_data[i]));
	}
	if(app->main_window){
	    gtk_widget_destroy(app->main_window);
	    app->main_window = NULL;
	}	
    }	    
    else
	for(i = 0; i < app->config->days_to_show; i++)
	    delete_weather_day_button( TRUE, &(app->buttons[i]) );
}
/*******************************************************************************/
WDB* create_weather_day_button(const char *text, const char *icon,
				const int icon_size, gboolean transparency,
				char font_size, GdkColor *color){

    WDB	*new_day_button;

#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
#endif
    
    new_day_button = g_new0(WDB, 1);
    if(!new_day_button)
	return NULL;
    /* create day button */
    new_day_button->button = gtk_event_box_new();
    gtk_widget_set_events(new_day_button->button, GDK_BUTTON_RELEASE_MASK|
						  GDK_BUTTON_PRESS_MASK);
    set_background_color(new_day_button->button, color);
    
    if(transparency)
      gtk_event_box_set_visible_window(GTK_EVENT_BOX(new_day_button->button), FALSE);
    /* create day label */
    new_day_button->label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(new_day_button->label), text);
    gtk_label_set_justify(GTK_LABEL(new_day_button->label), GTK_JUSTIFY_RIGHT);
    /* Set font size for label */
    set_font_size(new_day_button->label, font_size);
    /* create day box to contain icon and label */
    new_day_button->box = gtk_hbox_new(FALSE, 0);
    /* create day icon buffer */
    new_day_button->icon_buffer
	    = gdk_pixbuf_new_from_file_at_size(icon,
						icon_size,
						icon_size, NULL);
    if(new_day_button->icon_buffer){
	/* create day icon image from buffer */
	new_day_button->icon_image = gtk_image_new_from_pixbuf(new_day_button->icon_buffer);
	g_object_unref(G_OBJECT(new_day_button->icon_buffer));
    }
    else
    	new_day_button->icon_image = NULL;

    /* Packing all to the box */
    if(new_day_button->icon_buffer)
	gtk_box_pack_start(GTK_BOX(new_day_button->box), new_day_button->icon_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(new_day_button->box), new_day_button->label, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(new_day_button->button), new_day_button->box);

    return new_day_button;
}
/*******************************************************************************/
void delete_weather_day_button(gboolean after_all_destroy, WDB **day){

    if(after_all_destroy){
	if(*day){
	    g_free(*day);
	    *day = NULL;
	}
    }
    else{
	if(*day){
	    if( (*day)->icon_image ){
		gtk_widget_destroy( (*day)->icon_image );
		(*day)->icon_image = NULL;
	    }
	    if( (*day)->label ){
		gtk_widget_destroy( (*day)->label );    
		(*day)->label = NULL;
	    }    
	    if( (*day)->box ){
		gtk_widget_destroy( (*day)->box );
		(*day)->box = NULL;
	    }    
	    if( (*day)->button ){
		gtk_widget_destroy( (*day)->button );
		(*day)->button = NULL;
	    }    
	    g_free(*day);
	    *day = NULL;
	}
    }
}
/*******************************************************************************/
void swap_temperature(int *hi, int *low){
    int tmp;
    
    tmp = *hi; *hi = *low; *low = tmp;
}
/*******************************************************************************/
void set_background_color(GtkWidget *widget, GdkColor *bgc){
/* undo previos changes */
    gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, NULL);
/* set one color for all states of widget */
    gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, bgc);
}
/*******************************************************************************/
