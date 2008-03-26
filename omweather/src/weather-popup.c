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
/*******************************************************************************/
void popup_window_destroy(void){
    if(app->popup_window){
    	gtk_widget_destroy(app->popup_window);
	app->popup_window = NULL;
    }    
    if(app->popup_window_more){
	gtk_widget_destroy(app->popup_window_more);
	app->popup_window_more = NULL;
    }
}
/*******************************************************************************/
void popup_close(GtkWidget *widget, GdkEvent *event, gpointer user_data){
    popup_window_destroy();
}	
/*******************************************************************************/
gboolean popup_window_event_cb(GtkWidget *widget, GdkEvent *event, 
                    					    gpointer user_data){
    gint x, y, w, h;
#ifndef RELEASE
    fprintf(stderr, "BEGIN %s(): %i \n", __PRETTY_FUNCTION__, *event);
#endif
    gtk_widget_get_pointer(widget, &x, &y);
    w = widget->allocation.width;
    h = widget->allocation.height;
    if(!( (x >= 0) && (x <= w) && (y >= 0) && (y <= h) )){
    	popup_window_destroy();
    }	
    return TRUE;
}
/*******************************************************************************/
gboolean show_popup_window_handler(GtkWidget *widget, GdkEvent *event,
                    	    				    gpointer user_data){

    int		i, k;
    time_t	current_time = 0,
		utc_time,
		diff_time,
		current_data_last_update = 0;
    GtkWidget	*popup_frame = NULL,
		*popup_vbox = NULL,
		*popup_header_widget = NULL,
		*popup_temperature_range_widget = NULL,
		*popup_current_weather_widget = NULL,
		*popup_24_hours_widget = NULL,
		*popup_footer_widget = NULL,
		*separator_after_header = NULL,
		*separator_after_current = NULL,
		*separator_after_temperature = NULL,
		*separator_after_24_hours_widget = NULL;
    gboolean	first_day = FALSE,
		second_day = FALSE;

    #ifndef RELEASE
       fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
    #endif

/*For OS2008 - accelerating of showing popup window */    
#ifdef HILDON
    if(app->timer_for_os2008 != 0)
    	 g_source_remove(app->timer_for_os2008);
#endif

    /* if no one station present in list show only preference */
    if(!app->config->current_station_id){
	weather_window_settings(widget, event, user_data);
	return FALSE;
    }
    /* Search: Which button is pressed */
    for(i = 0;i < app->config->days_to_show; i++)
	if(app->buttons[i]->button == widget) 
	    break;
    /* Not found pressed button */
    if( i >= app->config->days_to_show )
	return FALSE;
    app->button_pressed = i;
    /* Create POPUP WINDOW */ 
    app->popup_window = gtk_window_new( GTK_WINDOW_POPUP );
    gtk_window_set_decorated(GTK_WINDOW(app->popup_window), FALSE);
    /* create popup window frame */
    popup_frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(app->popup_window), popup_frame);
    /* create frame vbox */    
    popup_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(popup_frame), popup_vbox);

    (app->config->separate) ? (k = -1) : (k = 0);

    /* prepare time additioanal value */
    current_time = time(NULL); /* get current day */
    /* correct time for current location */
    utc_time = mktime(gmtime(&current_time));
    diff_time = utc_time - current_time  + 60 * 60 * atol(item_value(wcs.day_data[i], "station_time_zone"));
    #ifndef RELEASE
    fprintf(stderr, "\n>>>>>>>Diff time=%li<<<<<<\n", diff_time);
    #endif
    current_time += diff_time;
/* check if fist day is pressed */
    if(i == 0){
	first_day = TRUE;
	gtk_window_move(GTK_WINDOW(app->popup_window), 180, 60);
    }
    else
	gtk_window_move(GTK_WINDOW(app->popup_window), 180, 140);
/* check if second day is pressed */
    if(i == 1)
	second_day = TRUE;

    if(app->config->separate){
	if(!i)
	    popup_header_widget = create_header_widget(wcs.day_data[i]);
        else
	    popup_header_widget = create_header_widget(wcs.day_data[i - 1]);
    }
    else
	popup_header_widget = create_header_widget(wcs.day_data[i]);    
    if(popup_header_widget)
	gtk_box_pack_start(GTK_BOX(popup_vbox), popup_header_widget,
			    FALSE, FALSE, 0);
    /* added separator */
    separator_after_header = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_header,
			    FALSE, FALSE, 0);
    if(wcs.day_data[i + k]){
	if(first_day || (second_day && app->config->separate)){ /* if first or second day */
	    if(first_day){
		current_data_last_update = last_update_time(wcs.current_data);	    
		if(!app->config->separate){ /* if weather data isn't separated */
		    popup_temperature_range_widget = create_temperature_range_widget(wcs.day_data[i + k]);
		    if(popup_temperature_range_widget)
			gtk_box_pack_start(GTK_BOX(popup_vbox), popup_temperature_range_widget,
    					    FALSE, FALSE, 0);
		    separator_after_temperature = gtk_hseparator_new();
		    gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_temperature,
					FALSE, FALSE, 0);
		    /* current weather */
		    if(!wcs.current_data_is_invalid &&
			(current_data_last_update > ( current_time - app->config->data_valid_interval)) &&
        		    (current_data_last_update < ( current_time + app->config->data_valid_interval))){
			popup_current_weather_widget = create_current_weather_widget(wcs.current_data);
			if(popup_current_weather_widget)
			    gtk_box_pack_start(GTK_BOX(popup_vbox), popup_current_weather_widget,
						    FALSE, FALSE, 0);
			/* added separator */
			separator_after_current = gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_current,
			    			FALSE, FALSE, 0);
		    }
		    popup_24_hours_widget = create_24_hours_widget(wcs.day_data[i + k]);
		    if(popup_24_hours_widget)
			gtk_box_pack_start(GTK_BOX(popup_vbox), popup_24_hours_widget,  /* time(NULL) - current local time */
					    FALSE, FALSE, 0);
		    separator_after_24_hours_widget = gtk_hseparator_new();
		    gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_24_hours_widget,
					FALSE, FALSE, 0);		
		}
		else{/* if weather data is separated */
		    if(!wcs.current_data_is_invalid &&
			(current_data_last_update > ( current_time - app->config->data_valid_interval)) &&
        		    (current_data_last_update < ( current_time + app->config->data_valid_interval))){
			popup_current_weather_widget = create_current_weather_widget(wcs.current_data);
			if(popup_current_weather_widget)
			    gtk_box_pack_start(GTK_BOX(popup_vbox), popup_current_weather_widget,
						    FALSE, FALSE, 0);
			/* added separator */
			separator_after_current = gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_current,
			    			FALSE, FALSE, 0);
		    }
		}
	    }
	    else{ /* if second day and weather data is separated */
		popup_temperature_range_widget = create_temperature_range_widget(wcs.day_data[i + k]);
		if(popup_temperature_range_widget)
		    gtk_box_pack_start(GTK_BOX(popup_vbox), popup_temperature_range_widget,
					    FALSE, FALSE, 0);
		separator_after_temperature = gtk_hseparator_new();
		gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_temperature,
					FALSE, FALSE, 0);
		popup_24_hours_widget = create_24_hours_widget(wcs.day_data[i + k]);
		if(popup_24_hours_widget)
		    gtk_box_pack_start(GTK_BOX(popup_vbox), popup_24_hours_widget, /* time(NULL) - current local time */
					    FALSE, FALSE, 0);
		separator_after_24_hours_widget = gtk_hseparator_new();
		gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_24_hours_widget,
					FALSE, FALSE, 0);
	    }
	}
	else{/* not first or not second day */
	    popup_temperature_range_widget = create_temperature_range_widget(wcs.day_data[i + k]);
	    if(popup_temperature_range_widget)
		gtk_box_pack_start(GTK_BOX(popup_vbox),popup_temperature_range_widget,
				    FALSE, FALSE, 0);
	    separator_after_temperature = gtk_hseparator_new();
	    gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_temperature,
				FALSE, FALSE, 0);
	    popup_24_hours_widget = create_24_hours_widget(wcs.day_data[i + k]);
	    if(popup_24_hours_widget)				
		gtk_box_pack_start(GTK_BOX(popup_vbox), popup_24_hours_widget, /* time(NULL) - current local time */
				    FALSE, FALSE, 0);
	    separator_after_24_hours_widget = gtk_hseparator_new();
	    gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_24_hours_widget,
				FALSE, FALSE, 0);
	}
    }
    /* added footer to popup window */
    if(wcs.day_data[i + k])
	popup_footer_widget = create_footer_widget(TRUE);
    else
	popup_footer_widget = create_footer_widget(FALSE);
    if(popup_footer_widget)
	gtk_box_pack_start(GTK_BOX(popup_vbox), popup_footer_widget,
				FALSE, FALSE, 0);
    gtk_grab_add(app->popup_window);
    g_signal_connect(G_OBJECT(app->popup_window),
			"button-release-event", 
                    	G_CALLBACK(popup_window_event_cb), app->main_window);
    gtk_widget_show_all(app->popup_window);    

    return FALSE;
}
/*******************************************************************************/
GtkWidget* create_header_widget(GSList *day){
    GtkWidget	*main_widget = NULL,
		*location_button_hbox = NULL,
		*location_label = NULL,
		*icon = NULL,
		*button = NULL,
                *button_box = NULL,
		*button_label = NULL,
		*date_hbox = NULL,
		*date_label = NULL;
    GtkIconInfo *gtkicon_update;
    gchar       buffer[1024];
    struct tm	tmp_time_date_struct = {0};

/* Show full or short name station */
    location_label = gtk_label_new(app->config->current_station_name);
/*    set_font_color(location_label, 0x0000, 0x0000, 0x0000);*/
/* prepare icon */
    icon = gtk_image_new_from_icon_name("qgn_toolb_gene_refresh",
                                         GTK_ICON_SIZE_INVALID);
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 26);

/* prepare button */    
    button = gtk_button_new();
    gtk_button_set_focus_on_click(GTK_BUTTON(button), FALSE);
    gtk_button_set_image(GTK_BUTTON(button), icon);
    
    gtk_widget_set_events(button, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(button, "clicked",
		    	    G_CALLBACK(pre_update_weather), NULL);
    /* prepare location and button hbox */
    location_button_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(location_button_hbox),
				location_label, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(location_button_hbox),
				button, FALSE, FALSE, 2);
    if(day){
	/* prepare date label */
	date_hbox = gtk_hbox_new(FALSE, 0);
	#ifndef RELEASE
	    fprintf(stderr, "\nDate = %s Day name %s\n", item_value(day, "24h_date"),
						item_value(day, "24h_name"));
	#endif
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer,"%s %s", item_value(day, "24h_name"), item_value(day, "24h_date"));
        strptime(buffer, "%A %b %d", &tmp_time_date_struct);
	memset(buffer, 0, sizeof(buffer));
	strftime(buffer, sizeof(buffer) - 1, "%A, %d %B", &tmp_time_date_struct);
	date_label = gtk_label_new(buffer);
	set_font_size(date_label, 16); 
/*	set_font_color(date_label, 0x0000, 0x0000, 0x0000);*/
	gtk_box_pack_start(GTK_BOX(date_hbox), date_label, FALSE, FALSE, 5);
    }
/* prepare main vbox */
    main_widget = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_widget),
				location_button_hbox, FALSE, FALSE, 0);
    if(day)
	gtk_box_pack_start(GTK_BOX(main_widget),
				date_hbox, FALSE, FALSE, 0);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_temperature_range_widget(GSList *day){
    GtkWidget	*main_widget = NULL,
		*temperature_title_label,
		*temperature_value_label;
    int		hi_temp,
		low_temp;
    gchar	symbol = 'C',
		buffer[1024];

    if(!day)
	return NULL;    
/* prepare temperature */
/* draw temperature, if any of temperature not aviable draw N/A */
    if(!strcmp(item_value(day, "24h_hi_temperature"), "N/A"))
	hi_temp = INT_MAX;
    else
	hi_temp = atoi(item_value(day, "24h_hi_temperature"));
    if(!strcmp(item_value(day, "24h_low_temperature"), "N/A"))
	low_temp = INT_MAX;
    else
	low_temp = atoi(item_value(day, "24h_low_temperature"));
    if(app->config->temperature_units == FAHRENHEIT){
	if(hi_temp != INT_MAX)
	    hi_temp = c2f(hi_temp);
	if(low_temp != INT_MAX)
	    low_temp = c2f(low_temp);
	symbol = 'F';
    }
    if(app->config->swap_hi_low_temperature)
	swap_temperature(&hi_temp, &low_temp);
    memset(buffer, 0, sizeof(buffer));
    /* prepare low temperature */
    if(low_temp == INT_MAX)
	strncat(buffer, (char*)hash_table_find("N/A"),
			( (strlen((char*)hash_table_find("N/A")) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) :
		        (strlen((char*)hash_table_find("N/A"))) ) );
    else
	snprintf(buffer, sizeof(low_temp) + 
			( (strlen(("\302\260%c")) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) : (strlen("\302\260%c")) ),
			"%d\302\260%c", low_temp, symbol);
    /* adding separate symbol */
    strncat(buffer, " / ",  
		    ( (strlen((" / ")) > sizeof(buffer)) ?
		      (sizeof(buffer) - 1) : (strlen(" / ")) ) );
    /* prepare hi temperature */
    if(hi_temp == INT_MAX)
	strncat(buffer, (char*)hash_table_find("N/A"),
			( (strlen((char*)hash_table_find("N/A")) > sizeof(buffer)) ?
			(sizeof(buffer) - 1) :
			(strlen((char*)hash_table_find("N/A"))) ) );
    else
	snprintf(buffer + strlen(buffer), sizeof(hi_temp) + 
			( (strlen(("\302\260%c")) > sizeof(buffer) - strlen(buffer)) ?
		        (sizeof(buffer) - strlen(buffer) - 1) : (strlen("\302\260%c")) ),
			"%d\302\260%c", hi_temp, symbol);
    /* prepare temperature hbox */
    temperature_value_label = gtk_label_new(buffer);
    set_font_size(temperature_value_label, 18);
/*    set_font_color(temperature_value_label, 0x0000, 0x0000, 0x0000);*/
    main_widget = gtk_hbox_new(FALSE, 10);
    temperature_title_label = gtk_label_new(_("Temperature: "));
    set_font_size(temperature_title_label, 16);
    gtk_box_pack_start(GTK_BOX(main_widget), temperature_title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_widget), temperature_value_label, FALSE, FALSE, 0);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_current_weather_widget(GSList *current){
    GtkWidget	*main_widget = NULL,
		*temperature_vbox,
		*temperature_label,
		*main_data_vbox,
		*main_data_label,
		*icon_temperature_vbox;
    gchar	buffer[1024],
		*units;
    const gchar	*wind_units_str[] = {	"m/s",
/*					"km/s",
					"mi/s",
					"m/h",
*/					"km/h",
					"mi/h"
				    };
    GdkPixbuf	*icon = NULL;
    GtkWidget	*icon_image = NULL;
    float	tmp_distance = 0;

    if(!current)
	return NULL;
    sprintf(buffer,"%s%s.png", path_large_icon, item_value(current, "icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer, BIG_ICON_SIZE, BIG_ICON_SIZE, NULL);
    icon_image = gtk_image_new_from_pixbuf(icon);
    if(icon)
        g_object_unref(icon);
    
/* prepare current temperature */
    temperature_vbox = gtk_vbox_new(FALSE, 0);
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, _("Now: "));
    sprintf(buffer + strlen(buffer), "%d\302\260",
		((app->config->temperature_units == CELSIUS) ? ( atoi(item_value(current, "temperature")))
							: ( c2f(atoi(item_value(current, "temperature"))))));
    (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C")) )
						: ( strcat(buffer, _("F")) );
    temperature_label = gtk_label_new(buffer);
    set_font_size(temperature_label, 14);
/*    set_font_color(temperature_label, 0x0000, 0x0000, 0x0000);*/
    gtk_box_pack_start(GTK_BOX(temperature_vbox), temperature_label, FALSE, FALSE, 0);
    
/* prepare "feels like", "visible", "pressure", "humidity", "wind", "gust" */
/* feels like */
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, item_value(current, "title"));
    strcat(buffer, _("\nFeels like: "));
    sprintf(buffer + strlen(buffer), "%d\302\260", 
	    (app->config->temperature_units == CELSIUS) ? (atoi(item_value(current, "feel_like"))) 
						: (c2f(atoi(item_value(current, "feel_like")))));
    (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C")) )
					: ( strcat(buffer, _("F")) );
/* humidity */
    strcat(buffer, _(" Humidity: "));
    if( strcmp(item_value(current, "humidity"), "N/A") )
	sprintf(buffer + strlen(buffer), "%d%%",
		atoi(item_value(current, "humidity")));
    else
	sprintf(buffer + strlen(buffer), "%s",
		    (char*)hash_table_find((gpointer)"N/A"));
/* visible */
    strcat(buffer, _("\nVisible: "));
    if( !strcmp(item_value(current, "humidity"), "Unlimited") )
	sprintf(buffer + strlen(buffer), "%s",
                (char*)hash_table_find((gpointer)"Unlimited"));
    else
	if( strcmp(item_value(current, "visible"), "N/A") ){
	    tmp_distance = atof(item_value(current, "visible"));
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
                    (char*)hash_table_find((gpointer)"N/A"));
/* pressure */
    strcat(buffer, _("\nPressure: "));
    sprintf(buffer + strlen(buffer), "%.2f %s, ", atof(item_value(current, "pressure")),
		    _("mb"));
    strcat(buffer, item_value(current, "pressure_direction"));
/* wind */
    strcat(buffer, _("\nWind: "));
    sprintf(buffer + strlen(buffer), "%s", item_value(current, "wind_direction"));
    if( strcmp(item_value(current, "wind_speed"), "N/A") )
	sprintf(buffer + strlen(buffer), " %.2f %s", 
		    convert_wind_units(app->config->wind_units, atof(item_value(current, "wind_speed"))),
		    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units]));
/* gust */
    if( strcmp(item_value(current, "wind_gust"), "N/A") ){
	strcat(buffer, _(" Gust: "));
	sprintf(buffer + strlen(buffer), "%.2f %s",
		    convert_wind_units(app->config->wind_units, atof(item_value(current, "wind_gust"))),
		    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units]));
    }

    main_data_vbox = gtk_vbox_new(FALSE, 0);	
    main_data_label = gtk_label_new(buffer);
    set_font_size(main_data_label, 17);
/*    set_font_color(main_data_label, 0x0000, 0x0000, 0x0000);*/
    gtk_box_pack_start(GTK_BOX(main_data_vbox), main_data_label,
			    FALSE, FALSE, 0);
			    
/* prepare icon and temperature vbox */
    icon_temperature_vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(icon_temperature_vbox), temperature_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(icon_temperature_vbox), icon_image, FALSE, FALSE, 0);    
/* prepare main widget */
    main_widget = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_widget), icon_temperature_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_widget), main_data_vbox, FALSE, FALSE, 0);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_24_hours_widget(GSList *day){
    GtkWidget	*main_widget = NULL,
		*night_hbox = NULL, *day_hbox = NULL,
		*night_icon_label_vbox, *day_icon_label_vbox,
		*night_icon = NULL,
		*day_icon = NULL,
		*night_label, *day_label,
		*night_data_label, *day_data_label,
		*separator_after_night = NULL;
    GdkPixbuf	*icon = NULL;
    gchar	buffer[1024];
    const gchar	*wind_units_str[] = {	"m/s",
/*					"km/s",
					"mi/s",
					"m/h",
*/					"km/h",
					"mi/h" };
    time_t 	current_day,
		utc_time,
		current_time,
		day_begin_time,
		night_begin_time;
    int 	year = 0,
		month = 0;
    struct tm	*tm = NULL, tmp_tm;
    char	date_in_string[255];

    if(!day)
	return NULL;
#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
#endif
    tm = &tmp_tm;
    memset(buffer, 0, sizeof(buffer));

/* prepare additional time values */
    current_time = time(NULL);
    tm = localtime(&current_time);
    year = 1900 + tm->tm_year;
    utc_time = mktime(gmtime(&current_time));
    current_day = current_time = utc_time + 60 * 60 * atol(item_value(day, "station_time_zone"));
    tm = localtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    current_day = mktime(tm);
/* save current month number */
    month = tm->tm_mon;
/* prepare night data */
    sprintf(buffer, "%s%s.png", path_large_icon, item_value(day, "night_icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer, BIG_ICON_SIZE, BIG_ICON_SIZE, NULL);
    night_icon = gtk_image_new_from_pixbuf(icon);
    
    if(icon)
	g_object_unref(icon);
/* prepare night label */
    night_label = gtk_label_new(_("Night:"));
    set_font_size(night_label, 17);
/*    set_font_color(night_label, 0x0000, 0x0000, 0x0000);*/
/* preapare night data */
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, item_value(day, "night_title"));
    strcat(buffer, _("\nHumidity: "));
    if(strcmp(item_value(day, "night_humidity"), "N/A"))
	sprintf(buffer + strlen(buffer), "%s%%\n",
		item_value(day, "night_humidity"));
    else
	sprintf(buffer + strlen(buffer), "%s\n",
			(char*)hash_table_find((gpointer)"N/A"));
    strcat(buffer, _("Wind: "));
    sprintf(buffer + strlen(buffer), "%s %.2f %s", item_value(day, "night_wind_title"),
			convert_wind_units(app->config->wind_units, atof(item_value(day, "night_wind_speed"))),
			(char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units]));
    night_data_label = gtk_label_new(buffer);
    set_font_size(night_data_label, 17);
/*    set_font_color(night_data_label, 0x0000, 0x0000, 0x0000);*/
/* prepare icon and label vbox */
    night_icon_label_vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(night_icon_label_vbox), night_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(night_icon_label_vbox), night_icon, FALSE, FALSE, 0);
/* prepare night hbox */
    night_hbox = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(night_hbox), night_icon_label_vbox, FALSE, FALSE, 0);	    
    gtk_box_pack_start(GTK_BOX(night_hbox), night_data_label, FALSE, FALSE, 0);	    
/* prepare day data */
    sprintf(buffer, "%s%s.png", path_large_icon, item_value(day, "day_icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer, BIG_ICON_SIZE, BIG_ICON_SIZE, NULL);
    day_icon = gtk_image_new_from_pixbuf(icon);
    if(icon)
	g_object_unref(icon);
/* prepare day label */
    day_label = gtk_label_new(_("Day:"));
    set_font_size(day_label, 17);
/*    set_font_color(day_label, 0x0000, 0x0000, 0x0000);*/
/* preapare day data */
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, item_value(day, "day_title"));
    strcat(buffer, _("\nHumidity: "));
    if(strcmp(item_value(day, "day_humidity"), "N/A"))
	sprintf(buffer + strlen(buffer), "%s%%\n",
		item_value(day, "day_humidity"));
    else
	sprintf(buffer + strlen(buffer), "%s\n",
			(char*)hash_table_find((gpointer)"N/A"));
    strcat(buffer, _("Wind: "));
    sprintf(buffer + strlen(buffer), "%s %.2f %s", item_value(day, "day_wind_title"),
			convert_wind_units(app->config->wind_units, atof(item_value(day, "day_wind_speed"))),
			(char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units]));
    day_data_label = gtk_label_new(buffer);
    set_font_size(day_data_label, 17);
/*    set_font_color(day_data_label, 0x0000, 0x0000, 0x0000);*/
/* prepare icon and label vbox */
    day_icon_label_vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(day_icon_label_vbox), day_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(day_icon_label_vbox), day_icon, FALSE, FALSE, 0);
/* prepare day hbox */
    day_hbox = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(day_hbox), day_icon_label_vbox, FALSE, FALSE, 0);	    
    gtk_box_pack_start(GTK_BOX(day_hbox), day_data_label, FALSE, FALSE, 0);	    
/* prepare main widget */
    main_widget = gtk_vbox_new(FALSE, 0);
    separator_after_night = gtk_hseparator_new();
/* set the part of firts 24 hours */
/* check for new year */
    if(month == 11 && !strncmp(item_value(day, "24h_date"), "Jan", 3))
	year++;
/* calculate the begin of night and day */
    /* day begin */
    sprintf(date_in_string, "%s %i %s",
	    item_value(day, "24h_date"),
	    year,
	    item_value(day, "24h_sunrise"));
    strptime(date_in_string, "%b %d %Y %I:%M %p", tm);
    day_begin_time = mktime(tm);
    /* night begin */
    sprintf(date_in_string, "%s %i %s",
	    item_value(day, "24h_date"),
	    year,
	    item_value(day, "24h_sunset"));
    strptime(date_in_string, "%b %d %Y %I:%M %p", tm);
    night_begin_time = mktime(tm);
/* First icon - morning, day or evening */
    if((current_time > day_begin_time) && (current_time < night_begin_time)){
	/* first add day */	
	    gtk_box_pack_start(GTK_BOX(main_widget), day_hbox, FALSE, FALSE, 0);
	    gtk_box_pack_start(GTK_BOX(main_widget), separator_after_night, FALSE, FALSE, 0);
	    gtk_box_pack_start(GTK_BOX(main_widget), night_hbox, FALSE, FALSE, 0);
    } 
    else{
	if((current_time < night_begin_time)){     
	    /* Morning */
	    gtk_box_pack_start(GTK_BOX(main_widget), night_hbox, FALSE, FALSE, 0);
	    gtk_box_pack_start(GTK_BOX(main_widget), separator_after_night, FALSE, FALSE, 0);
	    gtk_box_pack_start(GTK_BOX(main_widget), day_hbox, FALSE, FALSE, 0);
	}
	else{
	    /* Evening */
	    gtk_box_pack_start(GTK_BOX(main_widget), night_hbox, FALSE, FALSE, 0);
	    /* Free memory for not used widget */
	    if(day_hbox)
    		gtk_object_destroy(GTK_OBJECT(day_hbox));
	    if(separator_after_night)
		gtk_object_destroy(GTK_OBJECT(separator_after_night));
	}
    }
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_footer_widget(gboolean enable_more_info){
    GtkWidget	*main_widget = NULL,
    		*button = NULL,
		*popup_window_button_more = NULL,
		*button_close = NULL;
		
    if(enable_more_info){
	/* prepare More button */
	popup_window_button_more = gtk_button_new_with_label(">>");
	set_font_size(popup_window_button_more, 17);
	g_signal_connect(popup_window_button_more, "clicked",
			G_CALLBACK(popup_window_more_show), NULL);
    }
/* prepare Settings button */
    button = gtk_button_new_with_label(_("Settings"));
    set_font_size(button, 14);
    g_signal_connect(button, "clicked",
		    G_CALLBACK(weather_window_settings), NULL);
/* prepare Close button */
    button_close = gtk_button_new_with_label(_("Close"));
    set_font_size(button_close, 14);
    g_signal_connect(button_close, "clicked",
		    G_CALLBACK(popup_close), NULL);      		    
		    		    
/* prepare main widget */
    main_widget = gtk_hbox_new(FALSE, 10);
    if(enable_more_info)
	gtk_box_pack_start(GTK_BOX(main_widget), popup_window_button_more, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_widget), button_close, TRUE, FALSE, 0);	
    gtk_box_pack_end(GTK_BOX(main_widget), button, FALSE, FALSE, 0);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_sun_time_widget(GSList *day){
    GtkWidget	*main_widget = NULL,
		*main_label;
    gchar	buffer[1024],
		time_buffer[1024],
		icon[1024];
    struct tm	time_show = {0};
    time_t 	current_time,
		utc_time;
    GdkPixbuf   *icon_buffer;                                                                                                             
    GtkWidget   *icon_image; 

    if(!day)
	return NULL;
/* prepare additional time values */
    current_time = time(NULL);
    utc_time = mktime(gmtime(&current_time));
    current_time = utc_time + 60 * 60 * atol(item_value(day, "station_time_zone"));
/* Sun icon */
    sprintf(icon, "%s32.png", path_large_icon);
    icon_buffer = gdk_pixbuf_new_from_file_at_size(icon, BIG_ICON_SIZE, BIG_ICON_SIZE, NULL);
    if(icon_buffer){
	/* create Sun icon image from buffer */
	icon_image = gtk_image_new_from_pixbuf(icon_buffer);
	g_object_unref(G_OBJECT(icon_buffer));
    }
    else
    	icon_image = NULL;

    memset(buffer, 0, sizeof(buffer));
    memset(time_buffer, 0, sizeof(time_buffer));
    
    snprintf(buffer, sizeof(buffer) - 1, "%s", _("Sunrise: "));
    strptime(item_value(day, "24h_sunrise"), "%r", &time_show);
    if(strstr(item_value(day, "24h_sunrise"), "PM"))
	time_show.tm_hour += 12;
    strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			"%X", &time_show);

    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			"\n\n%s", _("Sunset: "));

    memset(time_buffer, 0, sizeof(time_buffer));
    strptime(item_value(day, "24h_sunset"), "%r", &time_show);
    if(strstr(item_value(day, "24h_sunset"), "PM"))
	time_show.tm_hour += 12;
    strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			"%X ", &time_show);

    main_label = gtk_label_new(buffer);
    set_font_size(main_label, 14);
/*    set_font_color(main_label, 0x0000, 0x0000, 0x0000);*/
    main_widget = gtk_hbox_new(FALSE, 10);
    
    /* Packing all to the box */
    if(icon_image)
	gtk_box_pack_start(GTK_BOX(main_widget), icon_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_widget), main_label, FALSE, FALSE, 0);

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

    if(!current)
	return NULL;
#ifndef RELEASE
       fprintf(stderr, "Begin %s(): \n", __PRETTY_FUNCTION__);
#endif
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			"%s",
			(char*)hash_table_find(item_value(current, "moon_phase")));
    main_label = gtk_label_new(buffer);
    set_font_size(main_label, 14);
/*    set_font_color(main_label, 0x0000, 0x0000, 0x0000);*/

    main_widget = gtk_hbox_new(FALSE, 10);
/* Moon icon */
    sprintf(icon, "%s%s.png", MOON_ICONS, item_value(current, "moon_phase"));
    space_symbol = strchr(icon, ' ');
    if(space_symbol)
	*space_symbol = '_';
/*    sprintf(icon, "%s31.png", path_large_icon);*/
    icon_buffer = gdk_pixbuf_new_from_file_at_size(icon, BIG_ICON_SIZE, BIG_ICON_SIZE, NULL);
    if(icon_buffer){
	icon_image = gtk_image_new_from_pixbuf(icon_buffer);
	g_object_unref(G_OBJECT(icon_buffer));
    }
    else
    	icon_image = NULL;

    if(icon_image)
	gtk_box_pack_start(GTK_BOX(main_widget), icon_image, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(main_widget), main_label, FALSE, FALSE, 0);
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

    if(!current)
	return NULL;

    tmp_time = last_update_time(current);
    
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%s", _("Last update at server: \n"));
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
		_("Last update from server: \n"), _("Unknown"));
    }	
    else{ 
	snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
		    "%s", _("Last update from server: \n"));
    	strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
		"%X %x", localtime(&statv.st_mtime));
	snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
		" %s", _("current location local time"));
    }
    label_update = gtk_label_new(buffer);    
    set_font_size(label_update, 14);
/*    set_font_color(label_update, 0x0000, 0x0000, 0x0000);*/
    main_widget = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_widget), label_update, FALSE, FALSE, 0);

    return main_widget;
}
/*******************************************************************************/
float convert_wind_units(int to, float value){
    float	result = value;
    switch(to){
	    default:
	    case METERS_S: result *= 10.0f / 36.0f; break;
/*	    case KILOMETERS_S: result /= 3600.0f; break;
 *
	    case MILES_S: result /= (1.609344f * 3600.0f); break;
	    case METERS_H: result *= 1000.0f; break;
*/	    case KILOMETERS_H: result *= 1.0f; break;
	    case MILES_H: result /= 1.609344f; break;
	}
    return result;
}
/*******************************************************************************/
void popup_window_more_show(GtkWidget *widget, GdkEvent *event, gpointer user_data){
    gtk_widget_hide(app->popup_window);
    weather_window_popup_more_show();	    
}
/*******************************************************************************/
void weather_window_popup_more_show(void){
    int		i;
    time_t	current_time = 0,
		utc_time;
    GtkWidget	*popup_frame = NULL,
		*popup_vbox = NULL,
		*popup_header_widget = NULL,
		*popup_sun_time_widget = NULL,
		*popup_window_moon_widget = NULL,
		*popup_time_updates_widget = NULL,
		*popup_footer_widget = NULL,
		*separator_after_header = NULL,
		*separator_after_sun_time = NULL,
		*separator_after_moon_phase = NULL,
		*separator_after_updates_phase = NULL;
    gboolean	first_day = FALSE;

    #ifndef RELEASE
       fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
    #endif
    i = app->button_pressed;
    /* Not found pressed button */
    if( i >= app->config->days_to_show )
	return;
    /* Create POPUP WINDOW */ 
    app->popup_window_more = gtk_window_new( GTK_WINDOW_POPUP );
    gtk_window_set_decorated(GTK_WINDOW(app->popup_window_more), FALSE);
    /* create popup window frame */
    popup_frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(app->popup_window_more), popup_frame);
    /* create frame vbox */    
    popup_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(popup_frame), popup_vbox);
    /* prepare time additioanal value */
    current_time = time(NULL); /* get current day */
    /* correct time for current location */
    utc_time = mktime(gmtime(&current_time));
    current_time = utc_time + 60 * 60 * atol(item_value(wcs.day_data[i], "station_time_zone"));
    /* check if fist day is pressed */
    if(i == 0){
	first_day = TRUE;
	gtk_window_move(GTK_WINDOW(app->popup_window_more), 180, 60);
    }
    else
	gtk_window_move(GTK_WINDOW(app->popup_window_more), 180, 140);
    /* added header to popup window */
    popup_header_widget = create_header_widget(wcs.day_data[i]);
    if(popup_header_widget)
	gtk_box_pack_start(GTK_BOX(popup_vbox), popup_header_widget,
			    FALSE, FALSE, 0);
    /* added separator */
    separator_after_header = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_header,
			    FALSE, FALSE, 0);
    /* sunrise & sunset */
    popup_sun_time_widget = create_sun_time_widget(wcs.day_data[i]);
    if(popup_sun_time_widget)
	gtk_box_pack_start(GTK_BOX(popup_vbox), popup_sun_time_widget,
			       FALSE, FALSE, 0);
    separator_after_sun_time = gtk_hseparator_new();                                                                    
    gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_sun_time,
                                            FALSE, FALSE, 0);
    if(first_day){
	/* moon phase */
	popup_window_moon_widget = create_moon_phase_widget(wcs.current_data);
	if(popup_window_moon_widget)
	    gtk_box_pack_start(GTK_BOX(popup_vbox),popup_window_moon_widget,
				        FALSE, FALSE, 0);
	separator_after_moon_phase = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_moon_phase,
                                            FALSE, FALSE, 0);
    }
    /* create time updates */
    popup_time_updates_widget = create_time_updates_widget(wcs.current_data);
    if(popup_time_updates_widget)
	gtk_box_pack_start(GTK_BOX(popup_vbox),popup_time_updates_widget,
				      FALSE, FALSE, 0);
    separator_after_updates_phase = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(popup_vbox), separator_after_updates_phase,
                                           FALSE, FALSE, 0);			    
    /* added footer to popup window */
    popup_footer_widget = create_footer_more_widget();
    if(popup_footer_widget)
	gtk_box_pack_start(GTK_BOX(popup_vbox), popup_footer_widget,
			    FALSE, FALSE, 0);
    gtk_grab_add(app->popup_window_more);
    g_signal_connect(G_OBJECT(app->popup_window_more),
			"button-release-event", 
                        G_CALLBACK(popup_window_event_cb), app->main_window);

    gtk_widget_show_all(app->popup_window_more);
}
/*******************************************************************************/
GtkWidget* create_footer_more_widget(void){
    GtkWidget	*main_widget = NULL,
    		*button = NULL,
		*popup_window_button_more = NULL,
		*button_close = NULL;
/* prepare More button */
    popup_window_button_more = gtk_button_new_with_label("<<");
    set_font_size(popup_window_button_more, 14);
    g_signal_connect(popup_window_button_more, "clicked",
		    G_CALLBACK(popup_window_more_close), NULL);
/* prepare Close button */
    button_close = gtk_button_new_with_label(_("Close"));
    set_font_size(button_close, 14);
    g_signal_connect(button_close, "clicked",
		    G_CALLBACK(popup_close), NULL);      		    
		    
/* prepare Settings button */
    button = gtk_button_new_with_label(_("Settings"));
    set_font_size(button, 14);
    g_signal_connect(button, "clicked",
		    G_CALLBACK(weather_window_settings), NULL);
/* prepare main widget */
    main_widget = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_widget), popup_window_button_more, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_widget), button_close, TRUE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(main_widget), button, FALSE, FALSE, 0);
    
    return main_widget;
}
/*******************************************************************************/
void popup_window_more_close(GtkWidget *widget, GdkEvent *event,
                    	    				    gpointer user_data){
    gtk_widget_show(app->popup_window);
    if(app->popup_window_more){
	    gtk_widget_destroy(app->popup_window_more);
	    app->popup_window_more = NULL;
    }
}
/*******************************************************************************/
