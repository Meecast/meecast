/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
#include "weather-popup.h"
#include "weather-utils.h"

/* Temporary  for debug */
#include <sys/time.h>
#include <time.h>

#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#define MOON_ICONS		"/usr/share/omweather/moon_icons/"
/*******************************************************************************/
/*
#ifdef HILDON
    if(app->timer_for_os2008 != 0)
    	 g_source_remove(app->timer_for_os2008);
#endif

*/
/*******************************************************************************/
GtkWidget* create_sun_time_widget(GSList *day){
    GtkWidget	*main_widget = NULL,
		*main_label;
    gchar	buffer[1024],
		time_buffer[1024];
    struct tm	time_show = {0};
    time_t 	current_time,
		utc_time;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!day)
	return NULL;
/* prepare additional time values */
    current_time = time(NULL);
    utc_time = mktime(gmtime(&current_time));
    current_time = utc_time + 60 * 60 * atol(item_value(day, "station_time_zone"));

    memset(buffer, 0, sizeof(buffer));
    memset(time_buffer, 0, sizeof(time_buffer));
    
    snprintf(buffer, sizeof(buffer) - 1, "%s", _("Sunrise: "));
    strptime(item_value(day, "24h_sunrise"), "%r", &time_show);
    if(strstr(item_value(day, "24h_sunrise"), "PM"))
	time_show.tm_hour += 12;
    strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			"%X", &time_show);

    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			" %s", _("Sunset: "));

    memset(time_buffer, 0, sizeof(time_buffer));
    strptime(item_value(day, "24h_sunset"), "%r", &time_show);
    if(strstr(item_value(day, "24h_sunset"), "PM"))
	time_show.tm_hour += 12;
    strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			"%X ", &time_show);

    main_label = gtk_label_new(buffer);
    set_font_size(main_label, 20);
    main_widget = gtk_hbox_new(FALSE, 10);
    
    /* Packing all to the box */
    gtk_box_pack_start(GTK_BOX(main_widget), main_label, TRUE, TRUE, 0);

    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_moon_phase_widget(GSList *current){
    GtkWidget	*main_widget = NULL,
		*main_label = NULL;
    gchar	buffer[1024],
		icon[50],
		*space_symbol = NULL;
    GdkPixbuf   *icon_buffer;                                                                                                             
    GtkWidget   *icon_image;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
	return NULL;

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			"%s",
			(char*)hash_table_find(item_value(current, "moon_phase"), FALSE));
    main_label = gtk_label_new(buffer);
    set_font_size(main_label, 18);

    main_widget = gtk_hbox_new(FALSE, 0);
/* Moon icon */
    sprintf(icon, "%s%s.png", MOON_ICONS, item_value(current, "moon_phase"));
    space_symbol = strchr(icon, ' ');
    if(space_symbol)
	*space_symbol = '_';
    icon_buffer = gdk_pixbuf_new_from_file_at_size(icon, GIANT_ICON_SIZE, GIANT_ICON_SIZE, NULL);
    if(icon_buffer){
	icon_image = gtk_image_new_from_pixbuf(icon_buffer);
	g_object_unref(G_OBJECT(icon_buffer));
    }
    else
    	icon_image = NULL;

    if(icon_image)
	gtk_box_pack_start(GTK_BOX(main_widget), icon_image, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(main_widget), main_label, FALSE, TRUE, 0);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_time_updates_widget(GSList *current){
    GtkWidget	*main_widget = NULL,
    		*label_update;
    gchar       buffer[1024],
		full_filename[2048];
    struct stat	statv;
    time_t	tmp_time = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
	return NULL;

    tmp_time = last_update_time(current);

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%s", _("Last update at server: "));
    if(tmp_time <= 0)	/* if weather data for station wasn't download */
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
    label_update = gtk_label_new(buffer);    
    set_font_size(label_update, 16);
    main_widget = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_widget), label_update, TRUE, TRUE, 0);

    return main_widget;
}
/*******************************************************************************/
void weather_window_popup(GtkWidget *widget, GdkEvent *event,
                    	    				    gpointer user_data){
    GtkWidget	*window_popup = NULL,
		*notebook = NULL,
		*tab = NULL,
		*current_tab = NULL,
		*label = NULL,
		*vbox = NULL,
		*buttons_box = NULL,
		*settings_button = NULL,
		*refresh_button = NULL,
		*close_button = NULL;
    gint	i = 0,
		active_tab = 0,
		k = 0;
    gchar	*day_name = NULL;
    time_t      current_time = 0,
                utc_time,
                diff_time,
                current_data_last_update = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* day number */
    active_tab = (gint)user_data;
/* if no one station present in list show settings window */
    if(!app->config->current_station_id){
	weather_window_settings(NULL, NULL, (gpointer)active_tab);
	return;
    }
/* Main window */
    window_popup = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_object_set_data(G_OBJECT(window_popup), "active_tab", (gpointer)active_tab);
    gtk_window_fullscreen(GTK_WINDOW(window_popup));
    /* create frame vbox */    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window_popup), vbox);
/* station name */
    gtk_box_pack_start(GTK_BOX(vbox),
    			label = gtk_label_new(app->config->current_station_name),
			FALSE, TRUE, 20);
    set_font_size(label, 28);
/* create tabs widget */
    notebook = gtk_notebook_new();
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
/* Current weather */
    current_time = time(NULL); /* get current day */
    /* correct time for current location */
    utc_time = mktime(gmtime(&current_time));
    diff_time = utc_time - current_time
			+ 60 * 60
			* atol(item_value(wcs.day_data[active_tab], "station_time_zone"));
#ifndef RELEASE
    fprintf(stderr, "\n>>>>>>>Diff time=%li<<<<<<\n", diff_time);
#endif
    current_time += diff_time;

    current_data_last_update = last_update_time(wcs.current_data);
    if(!wcs.current_data_is_invalid &&
	    (current_data_last_update >
		( current_time - app->config->data_valid_interval)) &&
	    (current_data_last_update <
		( current_time + app->config->data_valid_interval)))
	current_tab = create_current_tab(wcs.current_data);
    if(current_tab)
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
				current_tab,
				gtk_label_new(_("Now")));
/* if weather is separated than hide one day */
    (app->config->separate) ? (k = 1) : (k = 0);
/* Day tabs */
    for(i = 0; i < app->config->days_to_show - k; i++){
	tab = create_day_tab(wcs.current_data, wcs.day_data[i],
				&day_name);
	if(tab)
	    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        				tab,
        				gtk_label_new(day_name));
	day_name && (g_free(day_name), day_name = NULL);
    }
    /* check if no data file for this station */
    if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0)
	gtk_box_pack_start(GTK_BOX(vbox), notebook,
			TRUE, TRUE, 0);
    else{
	gtk_box_pack_start(GTK_BOX(vbox),
			label = gtk_label_new(_("No weather data for this station.")),
			TRUE, TRUE, 0);
	set_font_size(label, 40);
    }
    gtk_widget_show(notebook);
/* Bottom buttons box */
    buttons_box = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(buttons_box, -1, 60);
    /* Settings button */
    settings_button = create_button_with_image(BUTTON_ICONS, "settings", 40, FALSE);
    g_signal_connect(G_OBJECT(settings_button), "button_press_event",
                        G_CALLBACK(settings_button_handler),
			(gpointer)window_popup);
    /* Refresh buton */
    refresh_button = create_button_with_image(BUTTON_ICONS, "refresh", 40, FALSE);
    g_signal_connect(G_OBJECT(refresh_button), "button_press_event",
                        G_CALLBACK(refresh_button_handler),
			(gpointer)window_popup);
    /* Close button */
    close_button = create_button_with_image(BUTTON_ICONS, "close", 40, FALSE);
    g_signal_connect(G_OBJECT(close_button), "button_press_event",
                        G_CALLBACK(popup_close_button_handler),
			(gpointer)window_popup);
/* Pack buttons to the buttons box */
    gtk_box_pack_start(GTK_BOX(buttons_box), settings_button, FALSE, FALSE, 30);
    gtk_box_pack_start(GTK_BOX(buttons_box), refresh_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(buttons_box), close_button, FALSE, FALSE, 30);
    gtk_box_pack_start(GTK_BOX(vbox), buttons_box, FALSE, FALSE, 0);
/* check pressed day data accessibility */
    if((gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0) &&
	    !wcs.day_data[active_tab] && !app->config->separate){
    	hildon_banner_show_information(app->main_window,
					NULL,
					_("No weather data for this day."));
	gtk_widget_destroy(window_popup);
    }
    else{
	if(app->config->separate){
	    if(!current_tab){
		if(active_tab)
		    active_tab--;
		else{
		    hildon_banner_show_information(app->main_window,
					NULL,
					_("No current weather data."));
		    gtk_widget_destroy(window_popup);
		    return;
		}
	    }
	}
	else
	    if(current_tab && active_tab)
		    active_tab++;
	gtk_widget_show_all(window_popup);
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), active_tab);
    }
}

/*******************************************************************************/
/* For debug */
struct timeval tv1,tv2,dtv;

struct timezone tz;

void time_start() { gettimeofday(&tv1, &tz); }

double time_stop()

{  

    gettimeofday(&tv2, &tz);
    dtv.tv_sec= tv2.tv_sec  -  tv1.tv_sec;
    dtv.tv_usec=tv2.tv_usec -  tv1.tv_usec;
    if(dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }
        return dtv.tv_sec*1000.0+dtv.tv_usec/1000.0;
}
/*******************************************************************************/
void settings_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
    gint day_number
	= (gint)g_object_get_data(G_OBJECT(user_data), "active_tab");
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
time_start();  
    gtk_widget_destroy(GTK_WIDGET(user_data));
    weather_window_settings(NULL, NULL, (gpointer)day_number);
 fprintf(stderr,"Time: %lf msec Pi = %lf\n",time_stop(),weather_window_settings);
}
/*******************************************************************************/
void refresh_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gtk_widget_destroy(GTK_WIDGET(user_data));
/*    pre_update_weather(); */
    update_weather(TRUE);
}
/*******************************************************************************/
void popup_close_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gtk_widget_destroy(GTK_WIDGET(user_data));
}
/*******************************************************************************/
GtkWidget* create_day_tab(GSList *current, GSList *day, gchar **day_name){
    const gint	font_size = 18;
    GtkWidget	*main_widget = NULL,
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
		*night_text = NULL;
    gchar	buffer[1024],
		symbol = 'C';
    struct tm	tmp_time_date_struct = {0};
    GdkPixbuf	*icon = NULL;
    gint	hi_temp,
		low_temp,
		day_invalid_count = 0,
		night_invalid_count = 0;
    const gchar	*wind_units_str[] = { "m/s", "km/h", "mi/h" };
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!day)
	return NULL;
    /* prepare temperature */
    if(!strcmp(item_value(day, "24h_hi_temperature"), "N/A")){
	hi_temp = INT_MAX;
	day_invalid_count++;
    }
    else
	hi_temp = atoi(item_value(day, "24h_hi_temperature"));
    if(!strcmp(item_value(day, "24h_low_temperature"), "N/A")){
	low_temp = INT_MAX;
	night_invalid_count++;
    }
    else
	low_temp = atoi(item_value(day, "24h_low_temperature"));
    if(app->config->temperature_units == FAHRENHEIT){
	(hi_temp != INT_MAX) && ( hi_temp = c2f(hi_temp) );
	(low_temp != INT_MAX) && ( low_temp = c2f(low_temp) );
	symbol = 'F';
    }
    main_widget = gtk_vbox_new(FALSE, 0);
    /* prepare localized day name */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"%s", item_value(day, "24h_name"));
    strptime(buffer, "%a", &tmp_time_date_struct);
    *day_name = g_strdup(buffer);
    /* prepare title */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"%s %s", item_value(day, "24h_name"), item_value(day, "24h_date"));
    strptime(buffer, "%A %b %d", &tmp_time_date_struct);
    memset(buffer, 0, sizeof(buffer));
    strftime(buffer, sizeof(buffer) - 1, "%A, %d %B", &tmp_time_date_struct);
    title = gtk_label_new(buffer);
    set_font_size(title, font_size + 2); 
    /* create 24 hours data widget */
    day_night_hbox = gtk_hbox_new(FALSE, 10);
    /* day data */
    day_vbox = gtk_vbox_new(FALSE, 0);
    /* hbox for day label and temperature */
    day_label_temperature_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(day_vbox),
			day_label_temperature_hbox,
			TRUE, TRUE, 0);
    /* prepare day label */
    day_label = gtk_label_new(_("Day:"));
    gtk_box_pack_start(GTK_BOX(day_label_temperature_hbox),
			day_label, FALSE, TRUE, 0);
    /* day temperature */
    memset(buffer, 0, sizeof(buffer));
    if(hi_temp == INT_MAX)
	strncat(buffer, (char*)hash_table_find("N/A", FALSE),
			( (strlen((char*)hash_table_find("N/A", FALSE)) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) :
		        (strlen((char*)hash_table_find("N/A", FALSE))) ) );
    else
	snprintf(buffer, sizeof(hi_temp) + 
			( (strlen(("\302\260%c")) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) : (strlen("\302\260%c")) ),
			"%d\302\260%c", hi_temp, symbol);
    gtk_box_pack_start(GTK_BOX(day_label_temperature_hbox),
			gtk_label_new(buffer), FALSE, TRUE, 40);
    /* hbox for icon and text */
    day_icon_text_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(day_vbox),
			day_icon_text_hbox, TRUE, TRUE, 0);
    /* day icon */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.png", path_large_icon, item_value(day, "day_icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer,
					    BIG_ICON_SIZE,
					    BIG_ICON_SIZE, NULL);
    day_icon = gtk_image_new_from_pixbuf(icon);
    if(icon)
	g_object_unref(icon);
    gtk_box_pack_start(GTK_BOX(day_icon_text_hbox),
			day_icon, TRUE, TRUE, 5);
/* preapare day text */
    gtk_box_pack_start(GTK_BOX(day_icon_text_hbox),
			day_text_vbox = gtk_vbox_new(FALSE, 0),
			TRUE, TRUE, 0);
    memset(buffer, 0, sizeof(buffer));
    if(!strcmp((char*)item_value(day, "day_title"), "N/A"))
	day_invalid_count++;
    strcat(buffer, (char*)hash_table_find(item_value(day, "day_title"), FALSE));
    strcat(buffer, _("\nHumidity: "));
    if(strcmp(item_value(day, "day_humidity"), "N/A"))
	sprintf(buffer + strlen(buffer), "%s%%\n",
		item_value(day, "day_humidity"));
    else{
	sprintf(buffer + strlen(buffer), "%s\n",
			(char*)hash_table_find("N/A", FALSE));
	day_invalid_count++;
    }
    strcat(buffer, _("Wind: "));
    if(!strcmp((char*)item_value(day, "day_wind_title"), "N/A"))
	day_invalid_count++;
    if(!strcmp((char*)item_value(day, "day_wind_speed"), "N/A"))
	day_invalid_count++;
    sprintf(buffer + strlen(buffer), "%s\n%.2f %s",
	    (char*)hash_table_find(item_value(day, "day_wind_title"), FALSE),
	    convert_wind_units(app->config->wind_units, atof(item_value(day, "day_wind_speed"))),
	    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    day_text = gtk_label_new(buffer);
    set_font_size(day_label, font_size);
    set_font_size(day_text, font_size);
    gtk_box_pack_start(GTK_BOX(day_text_vbox),
			day_text, TRUE, TRUE, 0);
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
    /* night temperature */
    memset(buffer, 0, sizeof(buffer));
    if(low_temp == INT_MAX)
	strncat(buffer, (char*)hash_table_find("N/A", FALSE),
			( (strlen((char*)hash_table_find("N/A", FALSE)) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) :
		        (strlen((char*)hash_table_find("N/A", FALSE))) ) );
    else
	snprintf(buffer, sizeof(low_temp) + 
			( (strlen(("\302\260%c")) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) : (strlen("\302\260%c")) ),
			"%d\302\260%c", low_temp, symbol);
    gtk_box_pack_start(GTK_BOX(night_label_temperature_hbox),
			gtk_label_new(buffer), FALSE, TRUE, 40);
    /* hbox for icon and text */
    night_icon_text_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(night_vbox),
			night_icon_text_hbox, TRUE, TRUE, 0);
    /* night icon */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.png", path_large_icon, item_value(day, "night_icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer,
					    BIG_ICON_SIZE,
					    BIG_ICON_SIZE, NULL);
    night_icon = gtk_image_new_from_pixbuf(icon);
    if(icon)
	g_object_unref(icon);
    gtk_box_pack_start(GTK_BOX(night_icon_text_hbox),
			night_icon, TRUE, TRUE, 5);
/* preapare night text */
    gtk_box_pack_start(GTK_BOX(night_icon_text_hbox),
			night_text_vbox = gtk_vbox_new(FALSE, 0),
			TRUE, TRUE, 0);
    memset(buffer, 0, sizeof(buffer));
    if(!strcmp((char*)item_value(day, "night_title"), "N/A"))
        night_invalid_count++;
    strcat(buffer, (char*)hash_table_find(item_value(day, "night_title"), FALSE));
    strcat(buffer, _("\nHumidity: "));
    if(strcmp(item_value(day, "night_humidity"), "N/A"))
	sprintf(buffer + strlen(buffer), "%s%%\n",
		item_value(day, "night_humidity"));
    else{
	sprintf(buffer + strlen(buffer), "%s\n",
			(char*)hash_table_find("N/A", FALSE));
	night_invalid_count++;			
    }
    strcat(buffer, _("Wind: "));
    if(!strcmp((char*)item_value(day, "night_wind_title"), "N/A"))
        night_invalid_count++;
    if(!strcmp((char*)item_value(day, "night_wind_speed"), "N/A"))
        night_invalid_count++;
    sprintf(buffer + strlen(buffer), "%s\n%.2f %s",
	    (char*)hash_table_find(item_value(day, "night_wind_title"), FALSE),
	    convert_wind_units(app->config->wind_units, atof(item_value(day, "night_wind_speed"))),
	    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    night_text = gtk_label_new(buffer);
    set_font_size(night_label, font_size);
    set_font_size(night_text, font_size);
    gtk_box_pack_start(GTK_BOX(night_text_vbox),
			night_text, TRUE, TRUE, 0);
    /* add day and night items to main widget */
    /* check for N/A fields in day */
    if(day_invalid_count < 5)
	gtk_box_pack_start(GTK_BOX(day_night_hbox),
			    day_vbox,
			    TRUE, FALSE, 5);
    else
	gtk_widget_destroy(day_vbox);
    /* check for N/A fields in night */
    if(night_invalid_count < 5)
	gtk_box_pack_start(GTK_BOX(day_night_hbox),
			    night_vbox,
			    TRUE, FALSE, 5);
    else
	gtk_widget_destroy(night_vbox);
    /* day name with date */
    gtk_box_pack_start(GTK_BOX(main_widget), title, FALSE, FALSE, 5);
    /* day and/or night data */
    gtk_box_pack_start(GTK_BOX(main_widget), day_night_hbox, FALSE, FALSE, 5);
    /* sunrise and sunset time */
    gtk_box_pack_start(GTK_BOX(main_widget),
			create_sun_time_widget(day),
			TRUE, FALSE, 0);
    /* last update time */
    if(current)
	gtk_box_pack_start(GTK_BOX(main_widget),
			    create_time_updates_widget(current),
			    TRUE, FALSE, 5);
    gtk_widget_show_all(main_widget);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_current_tab(GSList *current){
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
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
	return NULL;
    main_widget = gtk_vbox_new(FALSE, 0);
    icon_text_hbox = gtk_hbox_new(FALSE, 0);
/* icon */
    sprintf(buffer,"%s%s.png", path_large_icon, item_value(current, "icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer, GIANT_ICON_SIZE,
						    GIANT_ICON_SIZE, NULL);
    icon_image = gtk_image_new_from_pixbuf(icon);
    if(icon)
        g_object_unref(icon);
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), icon_image, TRUE, TRUE, 0);
    /* temperature */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s\n", item_value(current, "title"));
    sprintf(buffer + strlen(buffer), "%s",  _("Temperature: "));
    sprintf(buffer + strlen(buffer), "  %d\302\260",
                ((app->config->temperature_units == CELSIUS) ?
			( atoi(item_value(current, "temperature"))) :
			( c2f(atoi(item_value(current, "temperature"))))));
    (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")) )
                                                : ( strcat(buffer, _("F\n")) );
    /* feels like */
    sprintf(buffer + strlen(buffer), "%s", _("Feels like:"));
    sprintf(buffer + strlen(buffer), "  %d\302\260",
	    (app->config->temperature_units == CELSIUS) ?
		(atoi(item_value(current, "feel_like"))) :
		(c2f(atoi(item_value(current, "feel_like")))));
    (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")) )
                                                : ( strcat(buffer, _("F\n")) );
/* humidity */
    if( strcmp(item_value(current, "humidity"), "N/A") ){
	sprintf(buffer + strlen(buffer), "%s", _("Humidity:"));
        sprintf(buffer + strlen(buffer), "  %d%%\n",
                atoi(item_value(current, "humidity")));
    }
/* visible */
    if( strcmp(item_value(current, "visible"), "N/A") ){
	sprintf(buffer + strlen(buffer), "%s", _("Visible:"));
	if( !strcmp(item_value(current, "visible"), "Unlimited") )
    	    sprintf(buffer + strlen(buffer), "  %s\n",
            	    (char*)hash_table_find("Unlimited", FALSE));
	else{
    	    tmp_distance = atof(item_value(current, "visible"));
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
/* pressure */
    if( strcmp(item_value(current, "pressure"), "N/A") ){
	sprintf(buffer + strlen(buffer), "%s", _("Pressure:"));

	tmp_pressure = atof(item_value(current, "pressure"));
	switch(app->config->pressure_units){
	    default:
	    case MB: units = _("mb"); break;
	    case INCH: units = _("inHg"); tmp_pressure = mb2inch(tmp_pressure); break;
	}

	sprintf(buffer + strlen(buffer), "  %.2f %s,", tmp_pressure, units);
	sprintf(buffer + strlen(buffer), "  %s\n",
		    item_value(current, "pressure_direction"));
    }
/* wind */
    if( strcmp(item_value(current, "wind_direction"), "N/A") ){
	sprintf(buffer + strlen(buffer), "%s", _("Wind:"));
	sprintf(buffer + strlen(buffer), "  %s",
		item_value(current, "wind_direction"));
	if( strcmp(item_value(current, "wind_speed"), "N/A") )
	    sprintf(buffer + strlen(buffer), "  %.2f %s\n",
		    convert_wind_units(app->config->wind_units, atof(item_value(current, "wind_speed"))),
		    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    }
/* gust */
    if( strcmp(item_value(current, "wind_gust"), "N/A") ){
        sprintf(buffer + strlen(buffer), "%s", _("Gust:"));
	sprintf(buffer + strlen(buffer), "  %.2f %s\n",
		convert_wind_units(app->config->wind_units, atof(item_value(current, "wind_gust"))),
		(char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    }

    text = gtk_label_new(buffer);
    set_font_size(text, 18);
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), text, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), create_moon_phase_widget(current),
			TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(main_widget), icon_text_hbox, TRUE, TRUE, 0);
    /* last update time */
    gtk_box_pack_start(GTK_BOX(main_widget),
			    create_time_updates_widget(current),
			    TRUE, FALSE, 5);
    gtk_widget_show_all(main_widget);
    return main_widget;
}
/*******************************************************************************/
