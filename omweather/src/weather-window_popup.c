/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
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
#include "weather-window_popup.h"

static gboolean 
popup_window_event_cb(GtkWidget *widget, 
                         GdkEvent *event, 
                         gpointer user_data){
    gint x, y, w, h;
    gtk_widget_get_pointer(widget, &x, &y);
    w = widget->allocation.width;
    h = widget->allocation.height;
    if(!( (x >= 0) && (x <= w) && (y >= 0) && (y <= h) )) 
        gtk_widget_destroy(widget);
    return TRUE; 
}


/* Show extended information about weather */
gboolean
weather_window_popup_show (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{
    GtkWidget *frame_popup;
    GtkWidget *hbox_title_location, *hbox_title_date, *hbox_day, *hbox_current, *hbox_night, *hbox_foot, *hbox_pref, *hbox_temp;
    GtkWidget *separator_title, *separator_current, *separator_temp, *separator_day, *separator_foot;
    GtkWidget *vbox, *vbox_current, *vbox_day, *vbox_night, *vbox_hu_day, *vbox_hu_night;
    GtkWidget *label_location;
    GtkWidget *label_date;
    GtkWidget *label_update;
    GtkWidget *label_current, *label_night, *label_day, *label_temp, *label_value_temp, *label_humidity_current, *label_humidity_night;
    GtkWidget *label_humidity_day, *vbox_hu_current;
    GdkPixbuf *icon;
    GtkWidget *icon_image_current, *icon_image_night, *icon_image_day, *icon_update;
    GtkWidget *button_update,  *button_pref;
    GtkIconInfo *gtkicon_update;
    gchar buffer[1024], full_filename[2048];
    time_t current_time;
    gboolean pressed_current_day = FALSE;
    struct stat statv;
    int i;

/* if no one station present in list show only preference */
    if(!_weather_station_id){
	weather_window_preference(widget, event, user_data);
	return FALSE;
    } 
   /* Search: Which button pressed */
    for(i = 0;i < days_to_show; i++)
	if(buttons[i] == widget) 
	    break;  
    /* Not found pressed button */
    if( i >= days_to_show)   
	return FALSE; 
    if(i==0){
	pressed_current_day = TRUE;
	/* get current day */  
	current_time = time(NULL);
    }
    i = boxs_offset[i];  
    /* Create POPUP WINDOW */ 
    weather_window_popup = gtk_window_new( GTK_WINDOW_POPUP );
    gtk_window_set_decorated(GTK_WINDOW(weather_window_popup), FALSE);
     
    frame_popup = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(weather_window_popup), frame_popup);

/*    GtkWidget *my_window = gtk_window_new( GTK_WINDOW_TOPLEVEL ); */
/* Show or current weather or forecast */
    if(pressed_current_day && 
	    (weather_current_day.date_time>(current_time-OFFSET_CURRENT_WEATHER*3600)) &&
            (weather_current_day.date_time<(current_time+OFFSET_CURRENT_WEATHER*3600))){
	gtk_window_move(GTK_WINDOW(weather_window_popup), 280, 90);
      /* Begin CURRENT */        
	hbox_current = gtk_hbox_new(FALSE, 0);
	sprintf(buffer,"%s%i.png", path_large_icon, weather_current_day.day.icon);
	icon = gdk_pixbuf_new_from_file_at_size(buffer, 64, 64,NULL);
	icon_image_current = gtk_image_new_from_pixbuf(icon);
	vbox_current = gtk_vbox_new(FALSE, 0);

	if(_weather_temperature_unit == 'C')
	    sprintf(buffer,"Now  \n%s\302\260C",weather_current_day.day.temp);    
	else
	    sprintf(buffer,"Now  \n%d\302\260F",c2f(atoi(weather_current_day.day.temp)));           
	label_current = gtk_label_new(buffer);
	set_font_size(label_current, 20);
	gtk_box_pack_start(GTK_BOX(vbox_current), label_current, FALSE, FALSE, 0);

	vbox_hu_current = gtk_vbox_new(FALSE, 0);    
	if(_weather_temperature_unit == 'C')
	    sprintf(buffer,"%s\nFeels like: %s\302\260C Visible: %.0f m\nHumidity: %s%%\nWind: %s %im/s Gust: %im/s",
        	    weather_current_day.day.title,
		    weather_current_day.low_temp,
		    weather_current_day.day.vis*1000,
		    weather_current_day.day.hmid,
		    weather_current_day.day.wind_title,
		    weather_current_day.day.wind_speed*10/36,
		    weather_current_day.day.wind_gust*10/36);
	else
	    sprintf(buffer,"%s\nFeels like: %s\302\260F Visible: %.0f m\nHumidity: %s%%\nWind: %s %im/s Gust: %im/s",
        	    weather_current_day.day.title,
		    c2f(atoi(weather_current_day.low_temp)),
		    weather_current_day.day.vis*1000,
		    weather_current_day.day.hmid,
		    weather_current_day.day.wind_title,
		    weather_current_day.day.wind_speed*10/36,
		    weather_current_day.day.wind_gust*10/36);
	label_humidity_current = gtk_label_new(buffer);    
	set_font_size(label_humidity_current,16);
	gtk_box_pack_start(GTK_BOX(vbox_hu_current),
			    label_humidity_current,
			    FALSE, FALSE, 0);
	separator_current = gtk_hseparator_new();

	gtk_box_pack_start(GTK_BOX(hbox_current),
			    icon_image_current,
			    FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox_current),
			    vbox_current, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox_current),
			    vbox_hu_current, FALSE, FALSE, 10);
/* End CURRENT */        
    }    
    else
	gtk_window_move(GTK_WINDOW(weather_window_popup), 280, 160);
/* Begin TITLE */
/* Location and date */
    vbox = gtk_vbox_new(FALSE, 0);
    hbox_title_location = gtk_hbox_new(FALSE, 0);
    hbox_title_date = gtk_hbox_new(FALSE, 0);
/* Show full or short name station */
    if(i < DAY_DOWNLOAD)
	label_location = gtk_label_new(weather_days[i].location);
    else
	label_location = gtk_label_new(_weather_station_name);
    
    sprintf(buffer,"%s, %s", weather_days[i].dayshname, weather_days[i].date);
    label_date = gtk_label_new(buffer);
    set_font_size(label_date, 18);
/* Button UPDATE */
    gtkicon_update = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(),
                                         "qgn_toolb_gene_refresh",
                                         26, 0);
    icon_update = gtk_image_new_from_file(gtk_icon_info_get_filename(gtkicon_update));
    button_update = gtk_button_new();
    gtk_button_set_relief(GTK_BUTTON(button_update), GTK_RELIEF_NONE);
    gtk_button_set_focus_on_click(GTK_BUTTON(button_update), FALSE);
    gtk_container_add(GTK_CONTAINER(button_update), icon_update);
    gtk_widget_set_events(button_update, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(button_update, "clicked",
			G_CALLBACK(update_weather), NULL);      

    gtk_box_pack_start(GTK_BOX(hbox_title_location),
			    label_location, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox_title_location),
			    button_update, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(hbox_title_date),
			    label_date, FALSE, FALSE, 5);
/* End TITLE */
/* BEGIN TEMPERATURE */
    hbox_temp = gtk_hbox_new(FALSE, 0);
    label_temp = gtk_label_new("Temperature: ");
    set_font_size(label_temp, 18);
    
    if(!strcmp(weather_days[i].hi_temp,"N/A")){ 
	if(_weather_temperature_unit == 'C')
	    sprintf(buffer, "%s\302\260C", weather_days[i].low_temp);    
	else
	    sprintf(buffer, "%d\302\260F", c2f(atoi(weather_days[i].low_temp)));
    }
    else{
	if(_weather_temperature_unit == 'C')
	    sprintf(buffer, "%s\302\260C / %s\302\260C", weather_days[i].low_temp,weather_days[i].hi_temp);
	else
	    sprintf(buffer,"%d\302\260F / %d\302\260F", c2f(atoi(weather_days[i].low_temp)),c2f(atoi(weather_days[i].hi_temp)));
    }  
    label_value_temp = gtk_label_new(buffer);
    gtk_box_pack_start(GTK_BOX(hbox_temp), label_temp, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_temp), label_value_temp, FALSE, FALSE, 5);
/* END TEMPERATURE */   
/* Begin NIGHT */        
    hbox_night = gtk_hbox_new(FALSE, 0);
    sprintf(buffer, "%s%i.png", path_large_icon, weather_days[i].night.icon);
    icon = gdk_pixbuf_new_from_file_at_size(buffer, 64, 64, NULL);
    icon_image_night = gtk_image_new_from_pixbuf(icon);
    vbox_night = gtk_vbox_new(FALSE, 0);
    label_night = gtk_label_new("Night");
    set_font_size(label_night, 20);
    
    gtk_box_pack_start(GTK_BOX(vbox_night), label_night, FALSE, FALSE, 0);
 
    vbox_hu_night = gtk_vbox_new(FALSE, 0);    
    sprintf(buffer, "%s\nHumidity: %s%%\nWind: %s %im/s",
	    weather_days[i].night.title,
	    weather_days[i].night.hmid,
	    weather_days[i].night.wind_title,
	    weather_days[i].night.wind_speed * 10 / 36);
    label_humidity_night = gtk_label_new(buffer);    
    set_font_size(label_humidity_night, 16);
    gtk_box_pack_start(GTK_BOX(vbox_hu_night), label_humidity_night, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(hbox_night), icon_image_night, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_night), vbox_night, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_night), vbox_hu_night, FALSE, FALSE, 10);
/* End NIGHT */
/* Begin DAY */    
    hbox_day = gtk_hbox_new(FALSE, 0);
    sprintf(buffer,"%s%i.png", path_large_icon, weather_days[i].day.icon);    
    icon = gdk_pixbuf_new_from_file_at_size(buffer, 64, 64, NULL);
    icon_image_day = gtk_image_new_from_pixbuf(icon);
    vbox_day = gtk_vbox_new(FALSE, 0);
    label_day = gtk_label_new("Day  ");
    set_font_size(label_day, 20);
    gtk_box_pack_start(GTK_BOX(vbox_day), label_day, FALSE, FALSE, 0);

    vbox_hu_day = gtk_vbox_new(FALSE, 0);    
    sprintf(buffer, "%s\nHumidity: %s%%\nWind: %s %im/s",
	    weather_days[i].day.title,
	    weather_days[i].day.hmid,
	    weather_days[i].night.wind_title,
	    weather_days[i].day.wind_speed * 10 / 36);
    label_humidity_day = gtk_label_new(buffer);    
    set_font_size(label_humidity_day, 16);
    gtk_box_pack_start(GTK_BOX(vbox_hu_day), label_humidity_day, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(hbox_day), icon_image_day, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_day), vbox_day, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_day), vbox_hu_day, FALSE, FALSE, 10);
/* End DAY */    
/* Begin FOOT */
    hbox_foot = gtk_hbox_new(FALSE, 0);
    sprintf(full_filename, "%s/%s.xml", _weather_dir_name, _weather_station_id);
    if(stat(full_filename, &statv))
	sprintf(buffer, "Last update: Unknown");
    else{ 
    	sprintf(buffer,"Last update: \n%s", ctime(&statv.st_mtime));
        buffer[strlen(buffer) - 1] = 0; /* Remove Last \n */
    }
    label_update = gtk_label_new(buffer);    
    set_font_size(label_update, 20);
    
    gtk_box_pack_start(GTK_BOX(hbox_foot), label_update, FALSE, FALSE, 5);
/* End FOOT */
/* Start PREFERENCE UPDATE */
    hbox_pref = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox_pref), 4);
    button_pref = gtk_button_new_with_label("Preference");
    g_signal_connect(button_pref, "clicked",
		    G_CALLBACK(weather_window_preference), NULL);      
    
    gtk_box_pack_end(GTK_BOX(hbox_pref), button_pref, FALSE, FALSE, 0);
/* End PREFERENCE UPDATE */
/* get current day */  
    current_time = time(NULL);
/* Packing elements */
    gtk_container_add(GTK_CONTAINER(vbox), hbox_title_location);
/* Check null data buttons */
    if(i < DAY_DOWNLOAD){
	gtk_container_add(GTK_CONTAINER(vbox), hbox_title_date);
	separator_title = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(vbox), separator_title, FALSE, TRUE, 0);
	separator_temp = gtk_hseparator_new();

	gtk_container_add(GTK_CONTAINER(vbox), hbox_temp);
	gtk_box_pack_start(GTK_BOX(vbox), separator_temp, FALSE, TRUE, 0);

	separator_day = gtk_hseparator_new();
	/* Show or current weather or forecast */
	if(pressed_current_day && 
		(weather_current_day.date_time>(current_time-OFFSET_CURRENT_WEATHER*3600)) &&
                (weather_current_day.date_time<(current_time+OFFSET_CURRENT_WEATHER*3600))){
	    gtk_container_add(GTK_CONTAINER(vbox), hbox_current);
	    gtk_box_pack_start(GTK_BOX(vbox), separator_current, FALSE, TRUE, 0);
	}
        /* First icon - morning, day or evening */     
	if((current_time>weather_days[i].day.begin_time) &&
    		(current_time<weather_days[i].night.begin_time)){
	/*  Day  */	
	    gtk_container_add(GTK_CONTAINER(vbox), hbox_day); 
	    gtk_box_pack_start(GTK_BOX(vbox), separator_day, FALSE, TRUE, 0);
	    gtk_container_add(GTK_CONTAINER(vbox), hbox_night);
	} 
	else{
	    if(current_time<weather_days[i].night.begin_time){     
	/*  Morning */	
		gtk_container_add(GTK_CONTAINER(vbox), hbox_night);
		gtk_box_pack_start(GTK_BOX(vbox), separator_day, FALSE, TRUE, 0);
		gtk_container_add(GTK_CONTAINER(vbox), hbox_day);
	    }
	    else
	/* Evening */
		gtk_container_add (GTK_CONTAINER (vbox), hbox_night);
	}
    }
    gtk_container_add(GTK_CONTAINER(frame_popup), vbox);    
    separator_foot = gtk_hseparator_new();		     
    gtk_box_pack_start(GTK_BOX(vbox), separator_foot, FALSE, TRUE, 0);    
    gtk_container_add(GTK_CONTAINER(vbox), hbox_foot);
    
    gtk_container_add(GTK_CONTAINER(vbox), hbox_pref);
    gtk_grab_add(weather_window_popup);

    g_signal_connect(G_OBJECT(weather_window_popup),
			"button-release-event", 
                        G_CALLBACK(popup_window_event_cb), box);

    gtk_widget_show_all(weather_window_popup);
    return TRUE;
}

/* Show extended information about weather */
static gint
weather_window_popup_show_future (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{
    GtkWidget *hbox_title_location, *hbox_title_date, *hbox_day, *hbox_night, *hbox_foot, *hbox_pref;
    GtkWidget *separator_title, *separator_day, *separator_foot;
    GtkWidget *vbox, *vbox_day, *vbox_night, *vbox_hu_day, *vbox_hu_night;
    GtkWidget *label_location;
    GtkWidget *label_date;
    GtkWidget *label_update;
    GtkWidget *label_night, *label_day, *label_temp, *label_humidity_night;
    GtkWidget *label_humidity_day ;
    GdkPixbuf *icon;
    GtkWidget *icon_image_night, *icon_image_day, *icon_update;
    GtkWidget *button_update,  *button_pref;
    GtkIconInfo *gtkicon_update;
    gchar buffer[1024];
    gchar full_filename[2048];
    struct stat statv;
    int i;

    if (_weather_station_id == NULL )
    {
/*     fprintf (stderr,"ID NULL\n"); */
     weather_window_preference(widget, event, user_data);
     return FALSE;
    } 
   /* Search: Which button pressed */
    for (i=0;i<days_to_show;i++)
     if ( buttons[i] == widget ) 
      break;
    /* Create POPUP WINDOW */ 
    weather_window_popup = gtk_window_new( GTK_WINDOW_POPUP );
        
/*    GtkWidget *my_window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
*/    gtk_window_move(GTK_WINDOW(weather_window_popup), 250,180);
    
    /* Begin TITLE */
    vbox = gtk_vbox_new (FALSE, 0);
    hbox_title_location = gtk_hbox_new (FALSE, 0);
    hbox_title_date = gtk_hbox_new (FALSE, 0);
    label_location = gtk_label_new (weather_days[i].location);
    
    sprintf(buffer,"%s, %s",weather_days[i].dayshname,weather_days[i].date);

    label_date = gtk_label_new (buffer);
    set_font_size(label_date,18);

    /* Button UPDATE */
    gtkicon_update = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(),
                                         "qgn_toolb_gene_refresh",
                                         26, 0);
    icon_update = gtk_image_new_from_file(gtk_icon_info_get_filename(gtkicon_update));
    button_update = gtk_button_new ();
    gtk_button_set_relief (GTK_BUTTON(button_update),GTK_RELIEF_NONE);
    gtk_button_set_focus_on_click (GTK_BUTTON(button_update),FALSE);
    gtk_container_add (GTK_CONTAINER (button_update), icon_update);
    gtk_widget_set_events (button_update, GDK_BUTTON_PRESS_MASK);
    g_signal_connect (button_update, "clicked",
		    G_CALLBACK (update_weather),
		    NULL);      

    gtk_box_pack_start (GTK_BOX (hbox_title_location),label_location, FALSE, FALSE, 5);
    gtk_box_pack_end   (GTK_BOX (hbox_title_location),button_update, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (hbox_title_date),label_date, FALSE, FALSE, 5);
    /* End TITLE */
    /* Begin NIGHT */        
    hbox_night = gtk_hbox_new (FALSE, 0);
    sprintf(buffer,"%s%i.png",path_large_icon,weather_days[i].night.icon);    
    icon = gdk_pixbuf_new_from_file_at_size (buffer,64,64,NULL);
    icon_image_night = gtk_image_new_from_pixbuf (icon);
    vbox_night = gtk_vbox_new (FALSE, 0);
    label_night = gtk_label_new ("Night");
    set_font_size(label_night,20);
    gtk_box_pack_start (GTK_BOX (vbox_night),label_night, FALSE, FALSE, 0);

    sprintf(buffer,"\n%s°C\n%s°C",weather_days[i].low_temp,weather_days[i].hi_temp);
    label_temp = gtk_label_new (buffer);
    set_font_size(label_temp,20);
 
    vbox_hu_night = gtk_vbox_new (FALSE, 0);    
    sprintf(buffer,"%s\nHumidity: %s%%\nWind: %s %im/s",weather_days[i].night.title,weather_days[i].night.hmid,weather_days[i].night.wind_title,weather_days[i].night.wind_speed*10/36);
    label_humidity_night = gtk_label_new (buffer);    
    set_font_size(label_humidity_night,15);
    gtk_box_pack_start (GTK_BOX (vbox_hu_night),label_humidity_night, FALSE, FALSE, 0);
    
    gtk_box_pack_start (GTK_BOX (hbox_night),icon_image_night, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox_night),vbox_night, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox_night),label_temp, FALSE, FALSE, 10);
    gtk_box_pack_start (GTK_BOX (hbox_night),vbox_hu_night, FALSE, FALSE, 0);
    /* End NIGHT */
    /* Begin DAY */    
    hbox_day = gtk_hbox_new (FALSE, 0);
    sprintf(buffer,"%s%i.png",path_large_icon,weather_days[i].day.icon);    
    icon = gdk_pixbuf_new_from_file_at_size (buffer,64,64,NULL);
    icon_image_day = gtk_image_new_from_pixbuf (icon);
    vbox_day = gtk_vbox_new (FALSE, 0);
    label_day = gtk_label_new ("Day  ");
    set_font_size(label_day,20);
    gtk_box_pack_start (GTK_BOX (vbox_day),label_day, FALSE, FALSE, 0);

    sprintf(buffer,"\n%s°C\n%s°C",weather_days[i].low_temp,weather_days[i].hi_temp);
    label_temp = gtk_label_new (buffer);
    set_font_size(label_temp,20);
    
    vbox_hu_day = gtk_vbox_new (FALSE, 0);    
    sprintf(buffer,"%s\nHumidity: %s%%\nWind: %s %im/s",weather_days[i].day.title,weather_days[i].day.hmid,weather_days[i].night.wind_title,weather_days[i].day.wind_speed*10/36);
    label_humidity_day = gtk_label_new (buffer);    
    set_font_size(label_humidity_day,15);
    gtk_box_pack_start (GTK_BOX (vbox_hu_day),label_humidity_day, FALSE, FALSE, 0);
    
    gtk_box_pack_start (GTK_BOX (hbox_day), icon_image_day, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox_day),vbox_day, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox_day),label_temp, FALSE, FALSE, 10);
    gtk_box_pack_start (GTK_BOX (hbox_day),vbox_hu_day, FALSE, FALSE, 0);
    /* End DAY */    
    
    /* Begin FOOT */
    hbox_foot = gtk_hbox_new (FALSE, 0);
    sprintf(full_filename, "%s/weather.com.xml", _weather_dir_name);
    if (stat (full_filename, &statv)) { sprintf(buffer,"Last update: Unknown"); }
     else { 
    	    sprintf(buffer,"Last update: %s",ctime(&statv.st_mtime));
            buffer[strlen(buffer)-1]=0; /* Remove Last \n */
          }
    label_update = gtk_label_new (buffer);    
    set_font_size(label_update,20);
    
    gtk_box_pack_start (GTK_BOX (hbox_foot),label_update, FALSE, FALSE, 0);
    /* End FOOT */
    /* Start PREFERENCE UPDATE */
    hbox_pref = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox_pref), 4);
    button_pref = gtk_button_new_with_label ("Preference");
    g_signal_connect (button_pref, "clicked",
		    G_CALLBACK (weather_window_preference),
		    NULL);      
    
    gtk_box_pack_end (GTK_BOX (hbox_pref),button_pref, FALSE, FALSE, 0);
    /* End PREFERENCE UPDATE */
    gtk_container_add (GTK_CONTAINER (vbox), hbox_title_location);
    gtk_container_add (GTK_CONTAINER (vbox), hbox_title_date);
    separator_title = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (vbox), separator_title, FALSE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (vbox), hbox_night);
    separator_day = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (vbox), separator_day, FALSE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (vbox), hbox_day);
    gtk_container_add (GTK_CONTAINER (weather_window_popup), vbox);    
    separator_foot = gtk_hseparator_new ();		     
    gtk_box_pack_start (GTK_BOX (vbox), separator_foot, FALSE, TRUE, 0);    
    gtk_container_add (GTK_CONTAINER (vbox), hbox_foot);
    gtk_container_add (GTK_CONTAINER (vbox), hbox_pref);
    gtk_grab_add( weather_window_popup ); 		     
    g_signal_connect(G_OBJECT(weather_window_popup), "button-release-event", 
                          G_CALLBACK(popup_window_event_cb), box);
    
    			  
    gtk_widget_show_all (weather_window_popup);
    return TRUE;	
}
