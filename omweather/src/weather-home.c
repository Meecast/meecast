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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>

#include <glib.h>

#include <hildon-home-plugin/hildon-home-plugin-interface.h>
#include "weather-home.h"

#include <unistd.h>
#include "weather-home_hash.h"

/* main struct */
OMWeatherApp	*app = NULL;

/* Translate  temperature Celsius to Farenhait */
int c2f(int temp){
    return (temp * 1.8f ) + 32;
}


/* Set font size. Usually on label widget */
void set_font_size(GtkWidget *widget, char font_size){
    PangoFontDescription *pfd = NULL;
    
    pfd = pango_font_description_copy( 
            pango_context_get_font_description(gtk_widget_get_pango_context(widget)));
    pango_font_description_set_absolute_size(pfd, font_size * PANGO_SCALE);	    
    gtk_widget_modify_font(GTK_WIDGET(widget), NULL);   /* this function is leaking */
    gtk_widget_modify_font(GTK_WIDGET(widget), pfd);   /* this function is leaking */
    pango_font_description_free(pfd);
}

static gboolean enter_button(GtkWidget *widget,
        			GdkEventCrossing *event){
    GtkButton	*button;
    GtkWidget	*event_widget;
 
    button = GTK_BUTTON(widget);
    event_widget = gtk_get_event_widget((GdkEvent*) event);
    button->in_button = FALSE;
    gtk_button_leave(button);
    return FALSE;
}

/* Change station to previos at main display */
static gboolean change_station_prev(GtkWidget *widget,
                    		    GdkEvent *event,
                    		    gpointer user_data){

    GSList			*tmplist = NULL;
    GSList			*tmplist_prev = NULL;
    struct weather_station	*ws;

    tmplist = stations_view_list;
    while(tmplist){
	ws = tmplist->data;
	/* Check active station */ 
	if((app->current_station_id != NULL) &&
		!strcmp(app->current_station_id, ws->id_station)){
	    /* Get last station if no previos station */
	    if(!tmplist_prev) 
    		tmplist_prev = g_slist_last(tmplist);
	    /* Get station data */
	    ws = tmplist_prev->data;
	    if(app->current_station_id)
    		g_free(app->current_station_id);
    	    app->current_station_id = g_strdup(ws->id_station); 
	    /* update current station name */
    	    if(app->current_station_name)
		g_free(app->current_station_name);
	    ws->name_station && (app->current_station_name = strdup(ws->name_station));
	    app->previos_days_to_show = app->days_to_show;
	    weather_frame_update(TRUE);
	    config_save_current_station();
	    break;
	} 
	tmplist_prev = tmplist;
	/* Next station in list */
	tmplist = g_slist_next(tmplist);
    }
    return TRUE;
}

/* Change station to next at main display */
static gboolean change_station_next(GtkWidget *widget,
                    		    GdkEvent *event,
                    		    gpointer user_data){

    GSList 			*tmplist = NULL;
    struct weather_station	*ws;

    tmplist = stations_view_list;
    while(tmplist){
	ws = tmplist->data;
	/* Check active station */ 
	if((app->current_station_id != NULL) &&
		!strcmp(app->current_station_id, ws->id_station)){
	    tmplist = g_slist_next(tmplist);
	    /* If no next station, get first */
	    if(!tmplist)
    		tmplist = stations_view_list;
	    /* Get station data */
	    ws = tmplist->data;
	    if(app->current_station_id)
    		g_free(app->current_station_id);
	    app->current_station_id = g_strdup(ws->id_station); 
	    /* update current station name */
	    if(app->current_station_name)
		g_free(app->current_station_name);
	    ws->name_station && (app->current_station_name = strdup(ws->name_station));
	    app->previos_days_to_show = app->days_to_show;
	    weather_frame_update(TRUE);
	    config_save_current_station();
	    break;
	}
	/* Next station in list */
	tmplist = g_slist_next(tmplist);
    }
    return TRUE;
}
/* Set default value */
void weather_buttons_init(void){
    int i;
/* Set default icon N/A */
    for(i = 0; i < app->days_to_show; i++){
	memset(&weather_days[i], 0, sizeof(weather_day));
	weather_days[i].night.icon = 48;
	weather_days[i].day.icon = 48;
	sprintf(weather_days[i].hi_temp, (char*)hash_table_find("N/A"));
	sprintf(weather_days[i].low_temp, (char*)hash_table_find("N/A"));
    }
    memset(&weather_current_day, 0, sizeof(weather_day));
}


/* Error Window */
void station_error_window(void){
   hildon_banner_show_information(app->main_window,
				    NULL,
				    _("Wrong station code \nor ZIP code!!!"));
}

/* Fill buttons data */
void weather_buttons_fill(gboolean check_error){
    int		i, offset = 0, count_day;
    gchar	buffer[2048], buffer_icon[2048];
    time_t	current_day,current_time, last_day = 0;
    struct tm	*tm;
    gboolean	flag_last_day = FALSE, error_station_code = FALSE;
    GSList	*tmplist = NULL;
    struct weather_station *ws = NULL;
    int		temp_hi,temp_hi_now,temp_low;
    char	font_size;
    gint	icon_size;
    gchar	*tmp_station_name;

/* Check main widget */
    if (!app->top_widget)
	return;
/* select image and font size */
    switch(app->icons_size){
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
    count_day = parse_weather_com_xml();

    if(check_error)
	if(count_day == -2){
	    count_day = 0;
	    fprintf(stderr, _("Error in xml file\n"));
	    error_station_code = TRUE;
	} /* Error in xml file */
/* get current day */  
    current_time = current_day = time(NULL);
    tm = localtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    current_day = mktime(tm);
/* free time event list */
    free_list_time_event();
/* add periodic update */
    add_periodic_event(time(NULL));
/* Search day of saving xml near current day */
    while( (offset < Max_count_weather_day) &&
	    (current_day > weather_days[offset].date_time) &&
	    (offset<count_day) )
	offset ++;
    for(i = 0; i < app->days_to_show; i++, offset++){
     /* If it first button add to evenet time change between nigth and day */
	if(current_day == weather_days[offset].date_time){
	    if(current_time < weather_days[offset].day.begin_time)
		time_event_add(weather_days[offset].day.begin_time,DAYTIMEEVENT);
	    if(current_time < weather_days[offset].night.begin_time)  
		time_event_add(weather_days[offset].night.begin_time,DAYTIMEEVENT);
	}
    /* Time event add to event list */
	if( (current_day < weather_days[offset].date_time) &&
	    (offset < count_day ) ){
	    time_event_add(weather_days[offset].date_time,DAYTIMEEVENT);  /* Add time event  to list */	  
	    last_day = weather_days[offset].date_time;	  
	}      
	if(offset < count_day){
     /* Prepare temperature value to show on display */
	    temp_hi_now = atoi(weather_current_day.day.temp); 
	    temp_hi = atoi(weather_days[offset].hi_temp);     
	    temp_low = atoi(weather_days[offset].low_temp);     
	    if(app->temperature_units == FAHRENHEIT ){
		temp_hi_now = c2f(temp_hi_now);
		temp_hi = c2f(temp_hi);
		temp_low = c2f(temp_low);
	    }  
     /* Show current weather or forecast */

	    if( (i==0) && 
		(weather_current_day.date_time>(current_time-OFFSET_CURRENT_WEATHER*3600)) &&
                (weather_current_day.date_time<(current_time+OFFSET_CURRENT_WEATHER*3600))){
      /* Add to evenet time change between current weather and forecast */
		time_event_add((weather_current_day.date_time+OFFSET_CURRENT_WEATHER*3600),DAYTIMEEVENT);
      /* Show current weather bold fonts */
		sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>%s\n%i\302\260\n</span>",
            	    app->font_color.red >> 8, app->font_color.green >> 8, app->font_color.blue >> 8,
            	    weather_days[offset].dayshname, temp_hi_now);
		sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_current_day.day.icon);
	    }		
    	    else{
      /* Show forecast */
		if(current_day == weather_days[offset].date_time){
       /* First icon - night(morning) or day or night (evening) */     
		    if(current_time<weather_days[offset].day.begin_time){  
    			sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].night.icon);
	 /* Show All temperatures */
    		    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n%i\302\260</span>",
            		app->font_color.red >> 8, app->font_color.green >> 8, app->font_color.blue >> 8,
            		weather_days[offset].dayshname, temp_low, temp_hi);
		    } 
		    else
			if(current_time<weather_days[offset].night.begin_time){
    			    sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].day.icon);    
       /* Show All temperatures */
			    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n%i\302\260</span>",
            			app->font_color.red >> 8, app->font_color.green >> 8, app->font_color.blue >> 8,
            			weather_days[offset].dayshname, temp_low, temp_hi);
			}		
			else{
    			    sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].night.icon);
	 /* Show only night temperature */
    			    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n</span>",
            			app->font_color.red >> 8, app->font_color.green >> 8, app->font_color.blue >> 8,
            			weather_days[offset].dayshname, temp_low);
			}
		}
		else{
       /* Create output string for full time of day (day and night)*/
		    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n%i\302\260</span>",
            		app->font_color.red >> 8, app->font_color.green >> 8, app->font_color.blue >> 8,
            		weather_days[offset].dayshname, temp_low, temp_hi);
		    sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].day.icon);
		}		    
	    }
	}
	else{
     /* Show N/A */
	    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%s\302\260\n%s\302\260</span>",
            	app->font_color.red >> 8, app->font_color.green >> 8, app->font_color.blue >> 8,
		_("N/A"), _("N/A"), _("N/A"));
	    sprintf(buffer_icon,"%s48.png",path_large_icon);
     /* Add time event to list for next day after last day in xml file */
	    if(!flag_last_day){
		time_event_add(last_day+24*60*60,DAYTIMEEVENT);  /* Add time event  to list */	  
		flag_last_day = TRUE;
	    } 
	}
     /* Write offset in wetaher data for this button */
	boxs_offset[i] = offset;
     /* Prepare butons for view */   
	app->buttons[i] = create_weather_day_button(buffer, buffer_icon, icon_size, app->transparency, font_size);
	g_signal_connect(app->buttons[i]->button, "released", G_CALLBACK(weather_window_popup_show), NULL);
	g_signal_connect(app->buttons[i]->button, "enter", G_CALLBACK(enter_button), NULL); 
    }/* for */

    if(g_slist_length(stations_view_list) > 0){
	tmplist = stations_view_list;
/* search current station */
        while(tmplist){
    	    ws = tmplist->data;
	    if ((ws->id_station)&&(app->current_station_id) && !strcmp(ws->id_station, app->current_station_id)) 
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
    create_panel(app->main_window, app->icons_layout, app->transparency, tmp_station_name, font_size);
    gtk_box_pack_start(GTK_BOX(app->top_widget), app->main_window, TRUE, TRUE, 0);
    gtk_widget_show_all(app->top_widget);

    if(error_station_code)
	station_error_window();
}

void weather_frame_update(gboolean check){
    
    free_memory(FALSE);
    if (app->main_window)
        gtk_widget_destroy(app->main_window);
    if(check) 
	weather_buttons_fill(TRUE);
    else
	weather_buttons_fill(FALSE);
}

/* Get Weather xml file from weather.com */
void update_weather(void){
   if (app->flag_updating == 0)
    app->flag_updating = g_timeout_add(100, (GSourceFunc)download_html, NULL);
    else
	fprintf(stderr,"Updating now yet\n");
}


void* hildon_home_applet_lib_initialize(void *state_data, 
					int *state_size,
					GtkWidget **widget){
    osso_context_t *osso;
    
    osso = osso_initialize(PACKAGE, VERSION, FALSE, NULL);
    if(!osso){
        g_debug(_("Error initializing the OMWeather applet"));
        return NULL;
    }
    #ifdef PC_EMULATOR
	fprintf(stderr, "\nOMWeather applet initialize %p %d\n",
			state_data, *state_size);
    #endif
    app = g_new0(OMWeatherApp, 1);
    memset(app, 0, sizeof(OMWeatherApp));
    app->osso = osso;
    app->flag_updating = 0;
/* create i18n hash for values coming from xml file */
    app->hash = hash_table_create();
    app->icons_size = LARGE;
    app->icons_layout = ONE_ROW;
    app->transparency = TRUE;
    app->days_to_show = app->previos_days_to_show = 5;
    app->distance_units = KILOMETERS;
    app->wind_units = METERS_S;
    app->temperature_units = CELSIUS;
/* Init gconf. */
    gnome_vfs_init();
    read_config();
/* Start timer */
    timer();
/* Initialize DBUS */
    weather_initialize_dbus();
/* Start main applet */ 
    app->top_widget = gtk_hbox_new(FALSE, 0);
    weather_buttons_fill(FALSE);
    (*widget) = app->top_widget;
    return (void*)osso;
}


int hildon_home_applet_lib_save_state(void *raw_data,
				    void **state_data, 
				    int *state_size){
    #ifdef PC_EMULATOR
	fprintf(stderr, "\nOMWeather applet save state\n");
    #endif
    (*state_data) = NULL;
    if(state_size)
	(*state_size) = 0;
    return 1;
}

void hildon_home_applet_lib_background(void *raw_data){
    #ifdef PC_EMULATOR
        fprintf(stderr, "\nOMWeather applet background\n");
    #endif
}

void hildon_home_applet_lib_foreground(void *raw_data){
    #ifdef PC_EMULATOR
	fprintf(stderr, "\nOMWeather applet foreground\n");
    #endif
}

void hildon_home_applet_lib_deinitialize(void *applet_data){
    osso_context_t *osso;
    gboolean check;
    
    #ifdef PC_EMULATOR
	fprintf(stderr, "\nOMWeather applet deinitialize\n");
    #endif
    /* It is switch off the timer */	
    check = g_source_remove(app->timer);

    config_save(); /* Not work!!!! Only 770. Why? I am not understand why this place not run when close applet 
			On n800 this work*/
			
    osso = (osso_context_t*)applet_data;
    if(app){
	app->top_widget = NULL;    
	free_memory(TRUE);
	g_free(app);
    }
    /* Deinitialize libosso */
    osso_deinitialize(osso);
}

GtkWidget* hildon_home_applet_lib_settings(void *applet_data,
					    GtkWindow *parent){
    #ifdef PC_EMULATOR
	fprintf(stderr, "\nOMWeather applet settings\n");
    #endif
    return NULL;
}
/* create days panel and station name panel */
void create_panel(GtkWidget* panel, gint layout, gboolean transparency, gchar* st_name, char f_size){
    gchar	buffer[2048];
    GtkWidget	*header_panel,
		*days_panel,
		*previos_station_name_btn, *previos_station_name, *previos_station_box,
		*next_station_name_btn, *next_station_name, *next_station_box,
		*station_name_btn, *station_name, *station_box;

    int		n, elements, x, y;


    if(app->days_to_show % 2)
	elements = app->days_to_show / 2 + 1;
    else
	elements = app->days_to_show / 2;
/* create header panel */
    header_panel = gtk_table_new(1, 3, FALSE);
/* create previos station button */
    sprintf(buffer, "<span weight=\"bold\" foreground='#%02x%02x%02x'>&lt;</span>",
	    app->font_color.red >> 8, app->font_color.green >> 8,
	    app->font_color.blue >> 8);
    previos_station_box		= gtk_hbox_new(FALSE, 0);
    previos_station_name_btn	= gtk_button_new();
    previos_station_name        = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(previos_station_name), buffer);
    gtk_label_set_justify(GTK_LABEL(previos_station_name), GTK_JUSTIFY_CENTER);
    set_font_size(previos_station_name, f_size);
    gtk_box_pack_start((GtkBox*) previos_station_box, previos_station_name, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER(previos_station_name_btn), previos_station_box);
    buffer[0] = '\0';
/* create next station button */
    sprintf(buffer, "<span weight=\"bold\" foreground='#%02x%02x%02x'>&gt;</span>",
	    app->font_color.red >> 8, app->font_color.green >> 8,
	    app->font_color.blue >> 8);
    next_station_box		= gtk_hbox_new(FALSE, 0);
    next_station_name_btn	= gtk_button_new();
    next_station_name        	= gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(next_station_name), buffer);
    gtk_label_set_justify(GTK_LABEL(next_station_name), GTK_JUSTIFY_CENTER);
    set_font_size(next_station_name, f_size);
    gtk_box_pack_start((GtkBox*) next_station_box, next_station_name, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER(next_station_name_btn), next_station_box);
    buffer[0] = '\0';
/* create station name button */
    if(!st_name)
	sprintf(buffer, "<span weight=\"bold\" foreground='#%02x%02x%02x'>%s</span>",
	    app->font_color.red >> 8, app->font_color.green >> 8,
	    app->font_color.blue >> 8,
	    (char*)hash_table_find("NO STATION"));
    else
	sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>%s</span>",
        	app->font_color.red >> 8, app->font_color.green >> 8,
		app->font_color.blue >> 8, st_name);
    station_box		= gtk_hbox_new(FALSE, 0);
    station_name_btn	= gtk_button_new();
    station_name        = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(station_name), buffer);
    gtk_label_set_justify(GTK_LABEL(station_name), GTK_JUSTIFY_CENTER);
    set_font_size(station_name, f_size);
    gtk_box_pack_start((GtkBox*) station_box, station_name, TRUE, TRUE, 0);
    
    gtk_container_add(GTK_CONTAINER(station_name_btn), station_box);
    
/* check transparency */
    if(transparency){
    	gtk_button_set_relief(GTK_BUTTON(previos_station_name_btn), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(next_station_name_btn), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(station_name_btn), GTK_RELIEF_NONE); 
    }
/* disable on_focus event for header_panel buttons */
    gtk_button_set_focus_on_click(GTK_BUTTON(previos_station_name_btn), FALSE);
    gtk_button_set_focus_on_click(GTK_BUTTON(next_station_name_btn), FALSE);
    gtk_button_set_focus_on_click(GTK_BUTTON(station_name_btn), FALSE); 
/* attach buttons to header panel */
    gtk_table_attach( (GtkTable*)header_panel, previos_station_name_btn, 0, 1, 0, 1 , GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach( (GtkTable*)header_panel, station_name_btn, 1, 2, 0, 1  , GTK_EXPAND, GTK_EXPAND, 0, 0); 
    gtk_table_attach( (GtkTable*)header_panel, next_station_name_btn, 2, 3, 0, 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
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
    for(n = 0, x = 0, y = 0; n < app->days_to_show; n++, x++){
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
/* attach to main panel header and days panels */
    gtk_table_attach( (GtkTable*)panel, header_panel, 0, 1, 0, 1 ,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
    gtk_table_attach( (GtkTable*)panel, days_panel, 0, 1, 1, 2 ,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
/* Connect signal button */
    g_signal_connect(previos_station_name_btn, "released", G_CALLBACK (change_station_prev), NULL);  		    
    g_signal_connect(previos_station_name_btn, "enter", G_CALLBACK (enter_button), NULL);     
    g_signal_connect(next_station_name_btn, "released", G_CALLBACK (change_station_next), NULL);  		    
    g_signal_connect(next_station_name_btn, "enter", G_CALLBACK (enter_button), NULL);     
    g_signal_connect(station_name_btn, "released", G_CALLBACK (change_station_next), NULL);  		    
    g_signal_connect(station_name_btn, "enter", G_CALLBACK (enter_button), NULL); 
    gtk_container_set_focus_child(GTK_CONTAINER(panel), station_name_btn); 
    if(station_name_btn)
	g_object_unref(station_name_btn);
}

/* free used memory from OMWeather struct */
void free_memory(gboolean flag){
    int    i;
    GSList *tmplist = NULL;
    struct weather_station *ws;
    
    if(flag){
	if(app->weather_dir_name){
	    g_free(app->weather_dir_name);
	    app->weather_dir_name = NULL;
	}
	if(app->icon_set){
	    g_free(app->icon_set);
	    app->icon_set = NULL;
	}
	if(app->current_country){
	    g_free(app->current_country);
	    app->current_country = NULL;
	}	
	if(app->current_station_name){
	    g_free(app->current_station_name);
	    app->current_station_name = NULL;
	}
        if(app->current_station_id){
	    g_free(app->current_station_id);
	    app->current_station_id = NULL;
	}
	if(app->iap_http_proxy_host){
	    g_free(app->iap_http_proxy_host);
	    app->iap_http_proxy_host = NULL;
	}
	if(app->hash){
	    g_hash_table_destroy(app->hash);
	    app->hash = NULL;
	}
	/* clean stations_view_list */
	if(stations_view_list){
	    if(g_slist_length(stations_view_list) > 0){
		tmplist = stations_view_list;
    		while(tmplist){
    		    ws = tmplist->data;
		    g_free(ws->id_station);
		    g_free(ws->name_station);
		    g_free(ws);
		    tmplist = g_slist_next(tmplist);
		}
	    }	    
	    g_slist_free(stations_view_list);
	    stations_view_list = NULL;
	}
    }
    
    if(app->top_widget){
#ifdef PC_EMULATOR
	fprintf(stderr, "\nDays current %d\n", app->days_to_show);
	fprintf(stderr, "\nDays previos %d\n", app->previos_days_to_show);
#endif
	for(i = 0; i < app->previos_days_to_show; i++)
	    delete_weather_day_button(FALSE, &(app->buttons[i]) );    
	if(app->main_window){
	    gtk_widget_destroy(app->main_window);
	    app->main_window = NULL;
	}	
    }	    
    else
	for(i = 0; i < app->previos_days_to_show; i++)
	    delete_weather_day_button( TRUE, &(app->buttons[i]) );
}

WDB* create_weather_day_button(const char *text, const char *icon, const int icon_size, gboolean transparency, char font_size){

    WDB	*new_day_button;
    
    new_day_button = g_new0(WDB, 1);
    if(!new_day_button)
	return NULL;
    /* create day icon buffer */
    new_day_button->icon_buffer
	    = gdk_pixbuf_new_from_file_at_size(icon,
						icon_size,
						icon_size, NULL);
    if(!new_day_button->icon_buffer)
	return NULL;
    /* create day icon image from buffer */
    new_day_button->icon_image = gtk_image_new_from_pixbuf(new_day_button->icon_buffer);
    g_object_unref(G_OBJECT(new_day_button->icon_buffer));
    /* create day button */
    new_day_button->button = gtk_button_new();
    if(transparency)
	gtk_button_set_relief(GTK_BUTTON(new_day_button->button), GTK_RELIEF_NONE);
    gtk_button_set_focus_on_click(GTK_BUTTON(new_day_button->button), FALSE);
    /* create day label */
    new_day_button->label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(new_day_button->label), text);
    gtk_label_set_justify(GTK_LABEL(new_day_button->label), GTK_JUSTIFY_RIGHT);
    /* Set font size for label */
    set_font_size(new_day_button->label, font_size);
    /* create day box to contain icon and label */
    new_day_button->box = gtk_hbox_new(FALSE, 0);
    /* Packing all to the box */
    gtk_box_pack_start(GTK_BOX(new_day_button->box), new_day_button->icon_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(new_day_button->box), new_day_button->label, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(new_day_button->button), new_day_button->box);
    return new_day_button;
}

void delete_weather_day_button(gboolean after_all_destroy,WDB **day){

    if(after_all_destroy){
	if(*day){
	    g_free(*day);
	    *day = NULL;
	}
    }
    else{
	if(*day){
	    fprintf(stderr,"destroy button\n");
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
