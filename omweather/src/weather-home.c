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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <http.h>

#include <glib.h>
#include <gtk/gtk.h>

#include <hildon-home-plugin/hildon-home-plugin-interface.h>
#include "weather-home.h"

#include <unistd.h>

#define VERSION "0.12"
#define APPNAME "omweather"




/* Set font size. Usually on label widget*/
void
set_font_size(GtkWidget *widget, char font_size)
{
    PangoFontDescription *pfd;    
    
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(widget) );
    pango_font_description_set_absolute_size(pfd,font_size*PANGO_SCALE);
    gtk_widget_modify_font( widget,pfd );    
}   
/* Create standard Hildon animation small window */
void 
create_window_update(void)
{
    update_window = hildon_banner_show_animation(weather_window_popup,NULL,"Update weather"); 
    g_object_ref(G_OBJECT(update_window));
}

void  iap_callback(struct iap_event_t *event, void *arg)
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
	   weather_frame_update(); 
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

gboolean get_connected(void)
{
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



//gchar *
int
get_weather_html( gboolean check_connect )
{
    FILE *fd;
    gchar full_filename[2048];
    HTTP_Response hResponse;
    HTTP_Extra  hExtra;
    GString *url;
   
   if (check_connect)  get_connected();    

   if (_weather_station_id != NULL)
   {
    url = g_string_new (NULL);        
    g_string_append_printf (url,"http://xoap.weather.com/weather/local/%s?cc=*&prod=xoap&par=1004517364&key=a29796f587f206b2&unit=m&dayf=10",_weather_station_id);
//      g_string_append (url,"s=");
    memset(&hExtra, '\0', sizeof(hExtra));
    hResponse = http_request(url->str,&hExtra,kHMethodGet,HFLAG_NONE);
    g_string_free (url,TRUE);
    if( hResponse.pError || strcmp(hResponse.szHCode,HTTP_RESPONSE_OK) )
        {
	     hildon_banner_show_information(box,NULL,"Did not download weather ");
	     return -2;	       
        }
	
    sprintf(full_filename, "%s/weather.com.xml.new", _weather_dir_name);

    if (!(access (full_filename,W_OK))) return -1; // Not Write Access to cache weather xml file 
      
    if(!(fd = fopen(full_filename,"w"))){
      hildon_banner_show_information(box,NULL,"Did not download 1111111111");     
          fprintf(stderr,"Could not open cache weather xml file %s.\n",full_filename);
          return -1;
    }
    fprintf (fd,"%s",hResponse.pData);
    fclose (fd);
      hildon_banner_show_information(box,NULL,"Weather updated");     
    return 0;
  }                                                                                                                          
}

 
static 
 gboolean popup_window_event_cb( GtkWidget *widget, 
                                 GdkEvent *event, 
                                 gpointer user_data )
{
     gint x, y;
     gtk_widget_get_pointer(widget, &x, &y);
     gint w = widget->allocation.width;
     gint h = widget->allocation.height;
     if (!( (x >= 0) && (x <= w) && (y >= 0) && (y <= h) )) {
         gtk_widget_destroy (widget);

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


/* Show extended information about weather */
static gboolean
weather_window_popup_show (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{
    GtkWidget *frame_popup;
    GtkWidget *hbox_title_location, *hbox_title_date, *hbox_day, *hbox_night, *hbox_foot, *hbox_pref, *hbox_temp;
    GtkWidget *separator_title, *separator_temp, *separator_day, *separator_foot;
    GtkWidget *vbox, *vbox_day, *vbox_night, *vbox_hu_day, *vbox_hu_night;
    GtkWidget *label_location;
    GtkWidget *label_date;
    GtkWidget *label_update;
    GtkWidget *label_night, *label_day, *label_temp, *label_value_temp, *label_humidity_night;
    GtkWidget *label_humidity_day ;
    GdkPixbuf *icon;
    GtkWidget *icon_image_night, *icon_image_day, *icon_update;
    GtkWidget *button_update,  *button_pref;
    GtkIconInfo *gtkicon_update;
    gchar buffer[1024];
    gchar full_filename[2048];
    struct stat statv;

    if (_weather_station_id == NULL )
    {
     fprintf (stderr,"ID NULL\n");
     weather_window_preference(widget, event, user_data);
     return FALSE;
    } 
   /* Search: Which button pressed */
    int i;
    for (i=0;i<Max_count_web_button;i++)
     if ( buttons[i] == widget ) 
      break;
    i = boxs_offset[i];  
    
    /* Create POPUP WINDOW */ 
//    weather_window_popup = gtk_window_new( GTK_WINDOW_POPUP );
     
     weather_window_popup =   gtk_window_new( GTK_WINDOW_TOPLEVEL );
     gtk_window_set_decorated (GTK_WINDOW(weather_window_popup),FALSE);
     
     frame_popup = gtk_frame_new(NULL);
     gtk_container_add (GTK_CONTAINER (weather_window_popup), frame_popup);

//    GtkWidget *my_window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_move(GTK_WINDOW(weather_window_popup), 280,160);
    
    /* Begin TITLE */
    /* Location and date */
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
    
    /* BEGIN TEMPERATURE */
    hbox_temp = gtk_hbox_new (FALSE, 0);
    label_temp = gtk_label_new ("Temperature: ");
    set_font_size(label_temp,18);

    sprintf(buffer,"%s°C-%s°C",weather_days[i].low_temp,weather_days[i].hi_temp);    
    label_value_temp = gtk_label_new (buffer);
    gtk_box_pack_start (GTK_BOX (hbox_temp),label_temp, FALSE, FALSE, 5);
    gtk_box_pack_start (GTK_BOX (hbox_temp),label_value_temp, FALSE, FALSE, 5);
    /* END TEMPERATURE */   
    /* Begin NIGHT */        
    hbox_night = gtk_hbox_new (FALSE, 0);
    sprintf(buffer,"%s%i.png",path_large_icon,weather_days[i].night.icon);    
    icon = gdk_pixbuf_new_from_file_at_size (buffer,64,64,NULL);
    icon_image_night = gtk_image_new_from_pixbuf (icon);
    vbox_night = gtk_vbox_new (FALSE, 0);
    label_night = gtk_label_new ("Night");
    
    set_font_size(label_night,20);
    
    gtk_box_pack_start (GTK_BOX (vbox_night),label_night, FALSE, FALSE, 0);

 
    vbox_hu_night = gtk_vbox_new (FALSE, 0);    
    sprintf(buffer,"%s\nHumidity: %s%%\nWind: %s %im/s",weather_days[i].night.title,weather_days[i].night.hmid,weather_days[i].night.wind_title,weather_days[i].night.wind_speed*10/36);
    label_humidity_night = gtk_label_new (buffer);    
    set_font_size(label_humidity_night,16);
    gtk_box_pack_start (GTK_BOX (vbox_hu_night),label_humidity_night, FALSE, FALSE, 0);
    
    gtk_box_pack_start (GTK_BOX (hbox_night),icon_image_night, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox_night),vbox_night, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox_night),vbox_hu_night, FALSE, FALSE, 10);
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

    vbox_hu_day = gtk_vbox_new (FALSE, 0);    
    sprintf(buffer,"%s\nHumidity: %s%%\nWind: %s %im/s",weather_days[i].day.title,weather_days[i].day.hmid,weather_days[i].night.wind_title,weather_days[i].day.wind_speed*10/36);
    label_humidity_day = gtk_label_new (buffer);    
    set_font_size(label_humidity_day,16);
    gtk_box_pack_start (GTK_BOX (vbox_hu_day),label_humidity_day, FALSE, FALSE, 0);
    
    gtk_box_pack_start (GTK_BOX (hbox_day), icon_image_day, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox_day),vbox_day, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox_day),vbox_hu_day, FALSE, FALSE, 10);
    /* End DAY */    
    
    /* Begin FOOT */
    hbox_foot = gtk_hbox_new (FALSE, 0);
    sprintf(full_filename, "%s/weather.com.xml", _weather_dir_name);
    if (stat (full_filename, &statv)) { sprintf(buffer,"Last update: Unknown"); }
     else { 
    	    sprintf(buffer,"Last update: \n%s",ctime(&statv.st_mtime));
            buffer[strlen(buffer)-1]=0; /* Remove Last \n */
          }
    label_update = gtk_label_new (buffer);    
    set_font_size(label_update,20);
    
    gtk_box_pack_start (GTK_BOX (hbox_foot),label_update, FALSE, FALSE, 5);
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
    /* Packing elements */
    if (i<DAY_DOWNLOAD) /* Check null data buttons */
    {
     gtk_container_add (GTK_CONTAINER (vbox), hbox_title_location);
     gtk_container_add (GTK_CONTAINER (vbox), hbox_title_date);
     separator_title = gtk_hseparator_new ();
     gtk_box_pack_start (GTK_BOX (vbox), separator_title, FALSE, TRUE, 0);
     separator_temp = gtk_hseparator_new ();
    
     gtk_container_add (GTK_CONTAINER (vbox), hbox_temp);
     gtk_box_pack_start (GTK_BOX (vbox), separator_temp, FALSE, TRUE, 0);
     gtk_container_add (GTK_CONTAINER (vbox), hbox_night);
    
     separator_day = gtk_hseparator_new ();
     gtk_box_pack_start (GTK_BOX (vbox), separator_day, FALSE, TRUE, 0);
     gtk_container_add (GTK_CONTAINER (vbox), hbox_day);
    }
    gtk_container_add (GTK_CONTAINER (frame_popup), vbox);    
    
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

    if (_weather_station_id == NULL )
    {
     fprintf (stderr,"ID NULL\n");
     weather_window_preference(widget, event, user_data);
     return FALSE;
    } 
   /* Search: Which button pressed */
    int i;
    for (i=0;i<Max_count_web_button;i++)
     if ( buttons[i] == widget ) 
      break;
    /* Create POPUP WINDOW */ 
    weather_window_popup = gtk_window_new( GTK_WINDOW_POPUP );
        
//    GtkWidget *my_window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_move(GTK_WINDOW(weather_window_popup), 250,180);
    
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


/* Set default value */
void weather_buttons_init(void)
{
 int i;
 /* Set default icon N/A */
 for (i=0;i<Max_count_web_button;i++)
 {
  weather_days[i].night.icon=48;
  weather_days[i].day.icon=48;
  sprintf(weather_days[i].hi_temp,"N/A");
  sprintf(weather_days[i].low_temp,"N/A");
 }
}


/* Fill buttons data */
void 
weather_buttons_fill(void)
{
  int i, offset, count_day;
  gchar buffer[2048];
  gchar buffer_icon[2048];
  time_t current_day,last_day;
  struct tm *tm;
  gboolean flag_last_day;

  flag_last_day = FALSE;
  offset = 0;
  weather_buttons_init();
  count_day=parse_weather_com_xml();
//  if  (count_day == -1)  {count_day=0; fprintf(stderr,"Error on xml file");} // Error on xml file

  /* get current day */  
  current_day = time(NULL);
  tm=localtime(&current_day);
  tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
  current_day = mktime(tm);
  /* free time event list */
  free_list_time_event();
  /* add periodic update */
  add_periodic_event();
    
  for (i=0;i<Max_count_web_button; i++)
  {    
    /* Search day of saving xml near current day */
    while ((offset<Max_count_weather_day) && (current_day > weather_days[offset].date_time) && (offset<count_day) )
    {
     offset ++;
    }
     /* If it first button add to evenet tine change between nigth and day */
    if  (current_day == weather_days[offset].date_time) 
    {
     if (time(NULL)<weather_days[offset].day.begin_time)
      time_event_add(weather_days[offset].day.begin_time,DAYTIMEEVENT);
     if (time(NULL)<weather_days[offset].night.begin_time)  
      time_event_add(weather_days[offset].night.begin_time,DAYTIMEEVENT);
    }
    /* Time event add to event list */
    if ( (current_day < weather_days[offset].date_time) && (offset < count_day ) )
    {
      
       time_event_add(weather_days[offset].date_time,DAYTIMEEVENT);  /* Add time event  to list */	  
      last_day = weather_days[offset].date_time;	  
    }      

    if ( offset < count_day)
    {
     /* Create output string */
     sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%s°\n%s°</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
                weather_days[offset].dayshname,weather_days[offset].hi_temp,weather_days[offset].low_temp);
     sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].day.icon);    		
     if  (current_day == weather_days[offset].date_time) 
     {
      if ((time(NULL)<weather_days[offset].day.begin_time) || 
         (time(NULL)>weather_days[offset].night.begin_time) ) 
       sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].night.icon);    
     }		    
    }
    else
    {
     /* Create output string */
     sprintf(buffer,"<span foreground='#%02x%02x%02x'>N/A\nN/A°\nN/A°</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
     sprintf(buffer_icon,"%s48.png",path_large_icon,weather_days[offset].day.icon);         
     /* Add time event to list for next day after last day in xml file */
     if (!flag_last_day)
     {
	time_event_add(last_day+24*60*60,DAYTIMEEVENT);  /* Add time event  to list */	  
	flag_last_day = TRUE;
     } 
    }

     /* Write offset in wetaher data for this button */
     boxs_offset[i] = offset;
     /* Prepare butons for view */   
     buttons[i] = gtk_button_new ();
     gtk_button_set_relief (GTK_BUTTON(buttons[i]),GTK_RELIEF_NONE);
     gtk_button_set_focus_on_click (GTK_BUTTON(buttons[i]),FALSE);    
     labels[i]=gtk_label_new (NULL);
     gtk_label_set_markup (GTK_LABEL (labels[i]),buffer);
     gtk_label_set_justify(GTK_LABEL (labels[i]),GTK_JUSTIFY_RIGHT);
     /* Select size font on desktop */
     if (strcmp(_weather_icon_size,"Large") == 0)
      set_font_size(labels[i],FONT_MAIN_SIZE_LARGE);
     else
      set_font_size(labels[i],FONT_MAIN_SIZE_SMALL);
     /* Create box for image and label */
     boxs[i] = gtk_hbox_new (FALSE, 0);
     /* Select size Icon */
     if (strcmp(_weather_icon_size,"Large") == 0)
      icon = gdk_pixbuf_new_from_file_at_size (buffer_icon,64,64,NULL);
     else 
      icon = gdk_pixbuf_new_from_file_at_size (buffer_icon,32,32,NULL);
     icon_image = gtk_image_new_from_pixbuf (icon);
     /* Packing buttons to box */
     gtk_box_pack_start (GTK_BOX (boxs[i]), icon_image, FALSE, FALSE, 0);
     gtk_box_pack_start (GTK_BOX (boxs[i]), labels[i], FALSE, FALSE, 0);
     gtk_container_add (GTK_CONTAINER (buttons[i]), boxs[i]);
     gtk_box_pack_start (GTK_BOX (box), buttons[i], FALSE, FALSE, 0);
     /* Connect signal button */
     g_signal_connect (buttons[i], "released",
		       G_CALLBACK (weather_window_popup_show),		
		       NULL);  		    
     g_signal_connect (buttons[i], "enter",
     		       G_CALLBACK (enter_button),
     		       NULL); 
    offset ++;
  }
  gtk_widget_show_all (box);
}

GtkWidget *
weather_frame_new (void)
{
  box =  gtk_hbox_new ( FALSE , 0); 
  weather_buttons_fill();
  return box;
}

void
weather_frame_update (void)
{
 int i;
 for (i=0;i<Max_count_web_button; i++)
 {
  if (buttons[i]) gtk_widget_destroy(buttons[i]);
 }
 weather_buttons_fill();
}

/* For window update */
void 
update_w(gpointer data)
{
 if (get_weather_html(TRUE) == 0)
 {
   weather_frame_update();
 }
 gtk_timeout_remove(flag_update);
 flag_update = 0;
 g_object_unref(G_OBJECT(update_window));
 gtk_widget_destroy (update_window);
 gtk_widget_destroy (weather_window_popup);
}

void 
update_weather(void)
{
 create_window_update();
 flag_update = g_timeout_add (100, update_w, box);
}




void *
hildon_home_applet_lib_initialize (void *state_data, 
				   int *state_size,
				   GtkWidget **applet_return)
{
 GtkWidget *frame;
 fprintf (stderr, "Weather applet initialize %p %d\n",
	   state_data, *state_size);
 /* Init gconf. */
 gnome_vfs_init();
 config_init ();
 /* Init weather buttons */
 weather_buttons_init();
 /* Start timer */
 timer();
 /* Start main applet */ 
 frame = weather_frame_new ();
 *applet_return = frame;
  return NULL;
}


int
hildon_home_applet_lib_save_state (void *raw_data,
				   void **state_data, 
				   int *state_size)
{
    fprintf (stderr, "hello-world save_state\n");
//  config_save();    
//  weather_frame_update();
  *state_data = NULL;
  *state_size = 0;
  return 0;
}

void
hildon_home_applet_lib_background (void *raw_data)
{
  fprintf (stderr, "hello-world background\n");
}

void
hildon_home_applet_lib_foreground (void *raw_data)
{
  fprintf (stderr, "hello-world foreground\n");
}

void
hildon_home_applet_lib_deinitialize (void *raw_data)
{
  config_save(); //Not work!!!! Why? I am not understand why this place not run when close applet
  fprintf (stderr, "hello-world deinitialize\n");
}

GtkWidget *
hildon_home_applet_lib_settings (void *applet_data,
				 GtkWindow *parent)
{
  fprintf (stderr, "hello-world settings\n");

  return NULL;
}
