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
#include <http.h>

#include <glib.h>
#include <gtk/gtk.h>

#include <hildon-home-plugin/hildon-home-plugin-interface.h>
#include <hildon-widgets/hildon-banner.h>
#include "weather-home.h"

#include <unistd.h>

#ifndef VERSION
#define VERSION "0.16"
#endif
#ifndef APPNAME
#define APPNAME "omweather"
#endif

/* Translate  temperature Celsius to Farenhait */
int c2f(int temp)
{
#define TEMP_C_TO_F(c)                  (((c) * 1.8) + 32.0)
return (temp*1.8)+32;
}

/* Set font size. Usually on label widget */
void
set_font_size(GtkWidget *widget, char font_size)
{
    PangoFontDescription *pfd;    
    
    pfd = pango_context_get_font_description(gtk_widget_get_pango_context(widget));
    pango_font_description_set_absolute_size(pfd, font_size * PANGO_SCALE);
    gtk_widget_modify_font(widget, pfd);    
}   
/* Create standard Hildon animation small window */
void create_window_update(){
    update_window = hildon_banner_show_animation(weather_window_popup,
						    NULL,
						    "Update weather");
    g_object_ref(G_OBJECT(update_window));
}

/* Callback function for request  connection to Internet */
void  
iap_callback(struct iap_event_t *event, void *arg)
{

    switch (event->type)
    {
    case OSSO_IAP_CONNECTED:
	if (get_weather_html(FALSE) != 0)
	{
	 hildon_banner_show_information(box,NULL,"Did not download weather");
	}
	else
	{
	   weather_frame_update(FALSE); 
	   hildon_banner_show_information(box,NULL,"Weather updated");
	}   
	break;
    case OSSO_IAP_DISCONNECTED:
	break;
    case OSSO_IAP_ERROR:
	hildon_banner_show_information(box,NULL,"Not connected to Internet");
	break;
    }    

}

/* Check connect to Internet and connection if it not */
gboolean 
get_connected(void)
{
    /* Register a callback function for IAP related events. */
    if (osso_iap_cb(iap_callback) != OSSO_OK)
    {
	return FALSE;
    }
    if (osso_iap_connect(OSSO_IAP_ANY, OSSO_IAP_REQUESTED_CONNECT, NULL) != OSSO_OK)
    {
	return FALSE;
    }
    return TRUE;
}


/* Get Weather xml file from weather.com */
int
get_weather_html( gboolean check_connect )
{
    FILE *fd;
    gchar full_filename[2048];
    HTTP_Response hResponse;
    HTTP_Extra  hExtra;
    GString *url;
    GSList *tmplist = NULL;
    struct weather_station *ws;
    
    
   if (check_connect)  get_connected();    

   tmplist = stations_view_list;
   while (tmplist != NULL)
   {
     ws = tmplist->data;
   if (ws->id_station != NULL)
   {
    url = g_string_new (NULL);        
    g_string_append_printf (url,"http://xoap.weather.com/weather/local/%s?cc=*&prod=xoap&par=1004517364&key=a29796f587f206b2&unit=m&dayf=10",ws->id_station);
/*      g_string_append (url,"s=");
*/    memset(&hExtra, '\0', sizeof(hExtra));
    hResponse = http_request(url->str,&hExtra,kHMethodGet,HFLAG_NONE);
    g_string_free (url,TRUE);
    if( hResponse.pError || strcmp(hResponse.szHCode,HTTP_RESPONSE_OK) )
        {
	     hildon_banner_show_information(box,NULL,"Did not download weather ");
	     return -2;	       
        }
	
    sprintf(full_filename, "%s/%s.xml.new", _weather_dir_name,ws->id_station);

    if(!(fd = fopen(full_filename,"w"))){
      hildon_banner_show_information(box,NULL,"Did not open save xml file");     
      fprintf(stderr,"Could not open cache weather xml file %s.\n",full_filename);
      return -1;
    }
    fprintf (fd,"%s",hResponse.pData);
    fclose (fd);
      hildon_banner_show_information(box,NULL,"Weather updated");     
  }
   tmplist = g_slist_next(tmplist);
  }
   
  return 0;                                                                                                                         
}

 
void hack_home_plugin_osso_for_nokia800(void)
{
  FILE *oss_conf_home_plugin_file;  
  FILE *oss_conf_home_plugin_file_new;  
  char  out_buffer[2048];
  gchar *real_path;
  gchar *real_path_new;
  gboolean flag_1 = FALSE;
  gboolean flag_2 = FALSE;
  
  GtkRequisition requisition;                                                                                                           
                                                                                                                                            
  gtk_widget_size_request(box_zero, &requisition);                                                                                      
  fprintf(stderr, "\nW -%d\n", requisition.width);                                                                                      
  fprintf(stderr, "\nH -%d\n", requisition.height);

  real_path = gnome_vfs_expand_initial_tilde("~/.osso/hildon-home/applet_manager.conf");
  real_path_new = gnome_vfs_expand_initial_tilde("~/.osso/hildon-home/applet_manager.conf.new");
  if (((oss_conf_home_plugin_file = fopen(real_path,"r")) != NULL)&&
     (oss_conf_home_plugin_file_new = fopen(real_path_new,"w")) != NULL)
    {
     while(!feof(oss_conf_home_plugin_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); /* Clear buffer */
      fgets(out_buffer, sizeof(out_buffer), oss_conf_home_plugin_file); /* Read Next Line */ 
      fprintf (stderr,"%s",out_buffer);
      if ( strcmp(out_buffer,"[/usr/share/applications/hildon-home/omweather-home.desktop]\n") == 0 ) {flag_1=TRUE;
      }
      if ((flag_1) || (flag_2))
      {
       if (strstr(out_buffer,"X-home-applet-width=") != NULL)
       {
       fprintf (oss_conf_home_plugin_file_new,"X-home-applet-width=%i\n", requisition.width);
/*       fprintf (oss_conf_home_plugin_file_new,"X-home-applet-width=%i\n", 300); */
        fprintf (stderr,"New: X-home-applet-width=%i\n", requisition.width);
        flag_2=TRUE;
        flag_1=FALSE;
       }
       else
       {
        if (strstr(out_buffer,"X-home-applet-height=") != NULL)
        {
         flag_2=FALSE;
         fprintf (oss_conf_home_plugin_file_new,"X-home-applet-height=%i\n", requisition.height);
/*         fprintf (oss_conf_home_plugin_file_new,"X-home-applet-height=%i\n", 300); */
         fprintf (stderr,"New: X-home-applet-height=%i\n", requisition.height);
        }
        else
        {
         fprintf (stderr,"Old: %s",out_buffer);      
         fputs(out_buffer , oss_conf_home_plugin_file_new);
        }
       }
      } 
     }
     fclose(oss_conf_home_plugin_file);
     fclose(oss_conf_home_plugin_file_new);
     unlink(real_path);
     rename(real_path_new,real_path);    
    } 
}



static gboolean
enter_button (GtkWidget        *widget,
              GdkEventCrossing *event)
{
   GtkButton *button;
   GtkWidget *event_widget;
 
   button = GTK_BUTTON (widget);
   event_widget = gtk_get_event_widget ((GdkEvent*) event);
   button->in_button = FALSE;
   gtk_button_leave (button);
   return FALSE;
}

/* Change station to previos at main display */
static gboolean
change_station_prev (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{

  GSList *tmplist = NULL;
  GSList *tmplist_prev = NULL;
  struct weather_station *ws;
  tmplist = stations_view_list;
  while (tmplist != NULL)
  {
   ws = tmplist->data;
     /* Check active station */ 
     if (strcmp(_weather_station_id, ws->id_station) == 0)
     {
     /* Get last station if no previos station */
      if (tmplist_prev == NULL) 
          tmplist_prev = g_slist_last(tmplist);
      /* Get station data */
      ws = tmplist_prev ->data;
      if (_weather_station_id != NULL) g_free(_weather_station_id);
        _weather_station_id = g_strdup(ws->id_station); 
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
static gboolean
change_station_next (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{

  GSList *tmplist = NULL;
  struct weather_station *ws;
  tmplist = stations_view_list;
  while (tmplist != NULL)
  {
   ws = tmplist->data;
     /* Check active station */ 
     if (strcmp(_weather_station_id, ws->id_station) == 0)
     {
      tmplist = g_slist_next(tmplist);
      /* If no next station, get first */
      if (!tmplist)
          tmplist = stations_view_list;
      /* Get station data */
      ws = tmplist ->data;
      if (_weather_station_id != NULL) g_free(_weather_station_id);
        _weather_station_id = g_strdup(ws->id_station); 
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
    for(i = 0; i < days_to_show; i++){
	weather_days[i].night.icon = 48;
	weather_days[i].day.icon = 48;
	sprintf(weather_days[i].hi_temp, "N/A");
	sprintf(weather_days[i].low_temp, "N/A");
    }
}


/* Error Window */
void station_error_window(void){
   hildon_banner_show_information(box, NULL, "Wrong station code \nor ZIP code!!!");
}

/* Fill buttons data */
/* by Pavel */
void weather_buttons_fill(gboolean check_error){
    int i, offset = 0, count_day;
    gchar buffer[2048], buffer_icon[2048];
    time_t current_day,current_time, last_day = 0;
    struct tm *tm;
    gboolean flag_last_day = FALSE, error_station_code = FALSE;
    GSList *tmplist = NULL;
    struct weather_station *ws = NULL;
    int temp_hi,temp_hi_now,temp_low;
    char	font_size;
    gint	icon_size;
    gchar	*tmp_station_name;
    
/* select image and font size */
    if(!strcmp(_weather_icon_size,"Large")){
	font_size = FONT_MAIN_SIZE_LARGE;
	icon_size = 64;
    }
    else
	if(!strcmp(_weather_icon_size,"Medium")){
	    font_size = FONT_MAIN_SIZE_MEDIUM;
	    icon_size = 48;
	}
	else{
	    font_size = FONT_MAIN_SIZE_SMALL;
	    icon_size = 32;
	}
  /* Init weather buttons */
    weather_buttons_init();
    count_day = parse_weather_com_xml();
    
    if(check_error)
	if(count_day == -2){
	    count_day = 0;
	    fprintf(stderr,"Error in xml file\n");
	    error_station_code = TRUE;
    } /* Error in xml file */
/* 
    if(count_day == -1){
	count_day = 0;
	fprintf(stderr,"Error on xml file");
    }
*/
  /* get current day */  
    current_time = current_day = time(NULL);
    tm = localtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    current_day = mktime(tm);
  /* free time event list */
    free_list_time_event();
  /* add periodic update */
    add_periodic_event();

  /* Search day of saving xml near current day */
    while( (offset < Max_count_weather_day) &&
	    (current_day > weather_days[offset].date_time) &&
	    (offset<count_day) )
	offset ++;
    for(i = 0; i < days_to_show; i++, offset++){
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
	    if(_weather_temperature_unit == 'F'){
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
            	    _weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
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
            		_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
            		weather_days[offset].dayshname,temp_low,temp_hi);
		    } 
		    else
			if(current_time<weather_days[offset].night.begin_time){
    			    sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].day.icon);    
       /* Show All temperatures */
			    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n%i\302\260</span>",
            			_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
            			weather_days[offset].dayshname,temp_low,temp_hi);
			}		
			else{
    			    sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].night.icon);
	 /* Show only night temperature */
    			    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n</span>",
            			_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
            			weather_days[offset].dayshname,temp_low);
			}
		}
		else{
       /* Create output string for full time of day (day and night)*/
		    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n%i\302\260</span>",
            		_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
            		weather_days[offset].dayshname,temp_hi,temp_low);
		    sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].day.icon);    		
		}		    
	    }
	}
	else{
     /* Show N/A */
	    sprintf(buffer,"<span foreground='#%02x%02x%02x'>N/A\nN/A\302\260\nN/A\302\260</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
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
	buttons[i] = gtk_button_new();
	if(_enable_transparency)
	    gtk_button_set_relief(GTK_BUTTON(buttons[i]),GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(buttons[i]),FALSE);

	labels[i] = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(labels[i]),buffer);
	gtk_label_set_justify(GTK_LABEL(labels[i]),GTK_JUSTIFY_RIGHT);
     /* Select size font on desktop and icon size */
	set_font_size(labels[i], font_size);
	icon = gdk_pixbuf_new_from_file_at_size(buffer_icon,
						icon_size,
						icon_size, NULL);
     /* Create box for image and label */
	boxs[i] = gtk_hbox_new(FALSE, 0);
	icon_image = gtk_image_new_from_pixbuf(icon);
     /* Packing buttons to box */
	gtk_box_pack_start(GTK_BOX(boxs[i]), icon_image, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(boxs[i]), labels[i], FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(buttons[i]), boxs[i]);
 /* Connect signal button */
	g_signal_connect(buttons[i], "released", G_CALLBACK (weather_window_popup_show), NULL);  		    
	g_signal_connect(buttons[i], "enter", G_CALLBACK (enter_button), NULL); 
    }/* for */


    if(g_slist_length(stations_view_list) > 0){
	tmplist = stations_view_list;
/* search current station */
        while (tmplist){
    	    ws = tmplist->data;
	    if (!strcmp(ws->id_station,_weather_station_id)) 
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
    box = gtk_table_new(2, 1, FALSE);
    create_panel(box, _weather_layout, _enable_transparency, tmp_station_name, font_size);
    gtk_box_pack_start(GTK_BOX(box_zero), box, TRUE, TRUE, 0);
    gtk_widget_show_all(box_zero);

/*     hack_home_plugin_osso_for_nokia800();     */
    
    if(error_station_code)
	station_error_window();
}

void weather_frame_update(gboolean check){
    gtk_widget_destroy(box);
    if(check) 
	weather_buttons_fill(TRUE);
    else
	weather_buttons_fill(FALSE);
}

/* For window update */
static gboolean update_w(gpointer data){
    if(!get_weather_html(TRUE))
	weather_frame_update(TRUE);
    gtk_timeout_remove(flag_update);
    flag_update = 0;
    g_object_unref(G_OBJECT(update_window));
    if(update_window)
	gtk_widget_destroy(update_window);
    if(weather_window_popup)
	gtk_widget_destroy(weather_window_popup);
    return TRUE;
}

void update_weather(void){
    create_window_update();
    flag_update = g_timeout_add(100, (GSourceFunc)update_w, NULL);
}


void* hildon_home_applet_lib_initialize(void *state_data, 
					int *state_size,
					GtkWidget **widget){
    osso_context_t *osso;
    
    osso = osso_initialize(APPNAME, VERSION, FALSE, NULL);
    if(!osso){
        g_debug("Error initializing the osso maemo omweather applet");
        return NULL;
    }
    fprintf(stderr, "\nWeather applet initialize %p %d\n",
		    state_data, *state_size);
    
/*    hack_home_plugin_osso_for_nokia800();		    */
/* Init gconf. */
    gnome_vfs_init();
    config_init();
/* Start timer */
    timer();
/* Start main applet */ 
    box_zero = gtk_hbox_new(FALSE, 0); 
    weather_buttons_fill(FALSE);
    (*widget) = box_zero;
    return (void*)osso;
}


int
hildon_home_applet_lib_save_state(void *raw_data,
				    void **state_data, 
				    int *state_size){
    fprintf(stderr, "\nhello-world save_state\n");
/*    config_save();   
    weather_frame_update(FALSE);
*/    (*state_data) = NULL;
    if(state_size)
	(*state_size) = 0;
    return 1;
}

void
hildon_home_applet_lib_background(void *raw_data){
    fprintf(stderr, "\nhello-world background\n");
}

void
hildon_home_applet_lib_foreground(void *raw_data){
    fprintf(stderr, "\nhello-world foreground\n");
}

void
hildon_home_applet_lib_deinitialize(void *applet_data){
    osso_context_t *osso;
    config_save(); /* Not work!!!! Why? I am not understand why this place not run when close applet */
    fprintf(stderr, "\nhello-world deinitialize\n");

    osso  = (osso_context_t*)applet_data;
    /* Deinitialize libosso */
    osso_deinitialize(osso);
/*    hack_home_plugin_osso_for_nokia800(); */
}

GtkWidget*
hildon_home_applet_lib_settings(void *applet_data,
				 GtkWindow *parent){
    fprintf(stderr, "\nhello-world settings\n");
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


    if(days_to_show % 2)
	elements = days_to_show / 2 + 1;
    else
	elements = days_to_show / 2;
/* create header panel */
    header_panel = gtk_table_new(1, 3, FALSE);
/* create previos station button */
    sprintf(buffer, "<span weight=\"bold\" foreground='#%02x%02x%02x'>&lt;</span>",
	    _weather_font_color.red >> 8, _weather_font_color.green >> 8,
	    _weather_font_color.blue >> 8);
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
	    _weather_font_color.red >> 8, _weather_font_color.green >> 8,
	    _weather_font_color.blue >> 8);
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
	sprintf(buffer, "<span weight=\"bold\" foreground='#%02x%02x%02x'>NO STATION</span>",
	    _weather_font_color.red >> 8, _weather_font_color.green >> 8,
	    _weather_font_color.blue >> 8);
    else
	sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>%s</span>",
        	_weather_font_color.red >> 8, _weather_font_color.green >> 8,
		_weather_font_color.blue >> 8, st_name);
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
    gtk_table_attach_defaults( (GtkTable*)header_panel, previos_station_name_btn, 0, 1, 0, 1 );
    gtk_table_attach_defaults( (GtkTable*)header_panel, station_name_btn, 1, 2, 0, 1 );
    gtk_table_attach_defaults( (GtkTable*)header_panel, next_station_name_btn, 2, 3, 0, 1 );
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
    for(n = 0, x = 0, y = 0; n < days_to_show; n++, x++){
	switch(layout){
	    default:
	    case ONE_ROW:
		gtk_table_attach_defaults( (GtkTable*)days_panel, buttons[n], n, n + 1, 0, 1 );
	    break;
	    case ONE_COLUMN:
		gtk_table_attach_defaults( (GtkTable*)days_panel, buttons[n], 0, 1, n, n + 1);
	    break;
	    case TWO_ROWS:
		if(n == elements){
		    x = 0; y = 1;
		}
		if(!y)
		    gtk_table_attach_defaults( (GtkTable*)days_panel, buttons[n], x, x + 1, 0, 1);
		else
		    gtk_table_attach_defaults( (GtkTable*)days_panel, buttons[n], x, x + 1, 1, 2);
	    break;
	    case TWO_COLUMNS:
		if(n == elements){
		    x = 0; y = 1;
		}
		if(!y)
		    gtk_table_attach_defaults( (GtkTable*)days_panel, buttons[n], 0, 1, x, x + 1);
		else
		    gtk_table_attach_defaults( (GtkTable*)days_panel, buttons[n], 1, 2, x, x + 1);
	    break;
	}
    }
/* attach to main panel header and days panels */
    gtk_table_attach_defaults( (GtkTable*)panel, header_panel, 0, 1, 0, 1 );
    gtk_table_attach_defaults( (GtkTable*)panel, days_panel, 0, 1, 1, 2 );
/* Connect signal button */
    g_signal_connect (previos_station_name_btn, "released", G_CALLBACK (change_station_prev), NULL);  		    
    g_signal_connect (previos_station_name_btn, "enter", G_CALLBACK (enter_button), NULL); 
    g_signal_connect (next_station_name_btn, "released", G_CALLBACK (change_station_next), NULL);  		    
    g_signal_connect (next_station_name_btn, "enter", G_CALLBACK (enter_button), NULL); 
    g_signal_connect (station_name_btn, "released", G_CALLBACK (change_station_next), NULL);  		    
    g_signal_connect (station_name_btn, "enter", G_CALLBACK (enter_button), NULL); 
}

