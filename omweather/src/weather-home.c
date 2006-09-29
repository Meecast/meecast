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

#define VERSION "0.1"
#define APPNAME "omweather"


  GtkWidget  *box;
  GdkPixbuf *icon;
  GtkWidget *icon_image;
  GtkWidget *buttons [Max_count_web_button];
  GtkWidget *labels [Max_count_web_button];
  GtkWidget *boxs [Max_count_web_button];
  GtkWidget *update_window;     
  gint flag_update=0;
   
/* Create standard Hildon animation small window */
void 
create_window_update(void)
{
    update_window = hildon_banner_show_animation(weather_window_popup,NULL,"Update weather"); 
    g_object_ref(G_OBJECT(update_window));
}

//gchar *
int
get_weather_html(  )
{
    FILE *fd;
    gchar full_filename[2048];
    HTTP_Response hResponse;
    HTTP_Extra  hExtra;
    GString *url;

    
   if (_weather_station_id != NULL)
   {
     
    url = g_string_new (NULL);
/*        
    sprintf(full_filename, "%s/weather.test", _weather_dir_name);
        if (!(access (full_filename,W_OK))) return -1; // Not Write Access to cache weather xml file 
    if(!(fd = fopen(full_filename,"w"))){
          fprintf(stderr,"Could not open cache weather xml file %s.\n",full_filename);
          return -1;
    }
    fprintf(fd,"http://xoap.weather.com/weather/local/%s?cc=*&prod=xoap&par=1004517364&key=a29796f587f206b2&unit=m&dayf=5\n",_weather_station_id);
    fclose(fd);
*/    
        
    g_string_append_printf (url,"http://xoap.weather.com/weather/local/%s?cc=*&prod=xoap&par=1004517364&key=a29796f587f206b2&unit=m&dayf=5",_weather_station_id);
//      g_string_append (url,"s=");

    memset(&hExtra, '\0', sizeof(hExtra));
    hResponse = http_request(url->str,&hExtra,kHMethodGet,HFLAG_NONE);
    g_string_free (url,TRUE);
    if( hResponse.pError || strcmp(hResponse.szHCode,HTTP_RESPONSE_OK) )
        {
	     return -2;	       
        }
    sprintf(full_filename, "%s/weather.com.xml.new", _weather_dir_name);
    if (!(access (full_filename,W_OK))) return -1; // Not Write Access to cache weather xml file 
    if(!(fd = fopen(full_filename,"w"))){
          fprintf(stderr,"Could not open cache weather xml file %s.\n",full_filename);
          return -1;
    }
    fprintf (fd,"%s",hResponse.pData);
    fclose (fd);
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
//static gint
weather_window_popup_show (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{
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
    PangoFontDescription *pfd;
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
    gtk_window_move(GTK_WINDOW(weather_window_popup), 280,180);
    
    /* Begin TITLE */
    /* Location and date */
    vbox = gtk_vbox_new (FALSE, 0);
    hbox_title_location = gtk_hbox_new (FALSE, 0);
    hbox_title_date = gtk_hbox_new (FALSE, 0);
    label_location = gtk_label_new (weather_days[i].location);
    
    sprintf(buffer,"%s, %s",weather_days[i].dayshname,weather_days[i].date);
    label_date = gtk_label_new (buffer);
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_date) );
    pango_font_description_set_absolute_size(pfd,18*PANGO_SCALE);
    gtk_widget_modify_font( label_date,pfd );
    
    
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
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_temp) );
    pango_font_description_set_absolute_size(pfd,18*PANGO_SCALE);
    gtk_widget_modify_font( label_temp,pfd );

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
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_night) );
    pango_font_description_set_absolute_size(pfd,20*PANGO_SCALE);
    gtk_widget_modify_font( label_night,pfd );
    gtk_box_pack_start (GTK_BOX (vbox_night),label_night, FALSE, FALSE, 0);

 
    vbox_hu_night = gtk_vbox_new (FALSE, 0);    
    sprintf(buffer,"%s\nHumidity: %s%%\nWind: %s %im/s",weather_days[i].night.title,weather_days[i].night.hmid,weather_days[i].night.wind_title,weather_days[i].night.wind_speed*10/36);
    label_humidity_night = gtk_label_new (buffer);    
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_humidity_night) );
    pango_font_description_set_absolute_size(pfd,16*PANGO_SCALE);
    gtk_widget_modify_font( label_humidity_night,pfd );
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
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_day) );
    pango_font_description_set_absolute_size(pfd,20*PANGO_SCALE);
    gtk_widget_modify_font( label_day,pfd );
    gtk_box_pack_start (GTK_BOX (vbox_day),label_day, FALSE, FALSE, 0);

    vbox_hu_day = gtk_vbox_new (FALSE, 0);    
    sprintf(buffer,"%s\nHumidity: %s%%\nWind: %s %im/s",weather_days[i].day.title,weather_days[i].day.hmid,weather_days[i].night.wind_title,weather_days[i].day.wind_speed*10/36);
    label_humidity_day = gtk_label_new (buffer);    
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_humidity_day) );
    pango_font_description_set_absolute_size(pfd,16*PANGO_SCALE);
    gtk_widget_modify_font( label_humidity_day,pfd );
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
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_update) );
    pango_font_description_set_absolute_size(pfd,20*PANGO_SCALE);
    gtk_widget_modify_font( label_update,pfd );
    
    gtk_box_pack_start (GTK_BOX (hbox_foot),label_update, FALSE, FALSE, 5);
    /* End FOOT */
    
    /* Start PREFERENCE UPDATE */
    hbox_pref = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_BOX (hbox_pref), 4);
    button_pref = gtk_button_new_with_label ("Preference");
    g_signal_connect (button_pref, "clicked",
		    G_CALLBACK (weather_window_preference),
		    NULL);      
    
    gtk_box_pack_end (GTK_BOX (hbox_pref),button_pref, FALSE, FALSE, 0);
    /* End PREFERENCE UPDATE */
    /* Packing elements */
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
    gtk_container_add (GTK_CONTAINER (weather_window_popup), vbox);    
    separator_foot = gtk_hseparator_new ();		     
    gtk_box_pack_start (GTK_BOX (vbox), separator_foot, FALSE, TRUE, 0);    
    gtk_container_add (GTK_CONTAINER (vbox), hbox_foot);
    gtk_container_add (GTK_CONTAINER (vbox), hbox_pref);
    gtk_grab_add( weather_window_popup ); 		     

    g_signal_connect(G_OBJECT(weather_window_popup), "button-release-event", 
                                                G_CALLBACK(popup_window_event_cb), box);

    gtk_widget_show_all (weather_window_popup);
    
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
    PangoFontDescription *pfd;
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
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_date) );
    pango_font_description_set_absolute_size(pfd,18*PANGO_SCALE);
    gtk_widget_modify_font( label_date,pfd );

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
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_night) );
    pango_font_description_set_absolute_size(pfd,20*PANGO_SCALE);
    gtk_widget_modify_font( label_night,pfd );
    gtk_box_pack_start (GTK_BOX (vbox_night),label_night, FALSE, FALSE, 0);

    sprintf(buffer,"\n%s°C\n%s°C",weather_days[i].low_temp,weather_days[i].hi_temp);
    label_temp = gtk_label_new (buffer);
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_temp) );
    pango_font_description_set_absolute_size(pfd,20*PANGO_SCALE);
    gtk_widget_modify_font( label_temp,pfd );
 
    vbox_hu_night = gtk_vbox_new (FALSE, 0);    
    sprintf(buffer,"%s\nHumidity: %s%%\nWind: %s %im/s",weather_days[i].night.title,weather_days[i].night.hmid,weather_days[i].night.wind_title,weather_days[i].night.wind_speed*10/36);
    label_humidity_night = gtk_label_new (buffer);    
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_humidity_night) );
    pango_font_description_set_absolute_size(pfd,15*PANGO_SCALE);
    gtk_widget_modify_font( label_humidity_night,pfd );
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
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_day) );
    pango_font_description_set_absolute_size(pfd,20*PANGO_SCALE);
    gtk_widget_modify_font( label_day,pfd );
    gtk_box_pack_start (GTK_BOX (vbox_day),label_day, FALSE, FALSE, 0);

    sprintf(buffer,"\n%s°C\n%s°C",weather_days[i].low_temp,weather_days[i].hi_temp);
    label_temp = gtk_label_new (buffer);
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_temp) );
    pango_font_description_set_absolute_size(pfd,20*PANGO_SCALE);
    gtk_widget_modify_font( label_temp,pfd );
    
    vbox_hu_day = gtk_vbox_new (FALSE, 0);    
    sprintf(buffer,"%s\nHumidity: %s%%\nWind: %s %im/s",weather_days[i].day.title,weather_days[i].day.hmid,weather_days[i].night.wind_title,weather_days[i].day.wind_speed*10/36);
    label_humidity_day = gtk_label_new (buffer);    
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_humidity_day) );
    pango_font_description_set_absolute_size(pfd,15*PANGO_SCALE);
    gtk_widget_modify_font( label_humidity_day,pfd );
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
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(label_update) );
    pango_font_description_set_absolute_size(pfd,20*PANGO_SCALE);
    gtk_widget_modify_font( label_update,pfd );
    
    gtk_box_pack_start (GTK_BOX (hbox_foot),label_update, FALSE, FALSE, 0);
    /* End FOOT */
    /* Start PREFERENCE UPDATE */
    hbox_pref = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_BOX (hbox_pref), 4);
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
  PangoFontDescription *pfd;
  int i;
  gchar buffer[2048];
  
  int count_day=parse_weather_com_xml();
  if ( (count_day == 0) || (count_day == -1) ) {weather_buttons_init();count_day = 5; } // Error on xml file
  if (count_day >Max_count_web_button) count_day = Max_count_web_button; 
  for (i=0;i<count_day; i++)
  {
    buttons[i] = gtk_button_new ();
    gtk_button_set_relief (GTK_BUTTON(buttons[i]),GTK_RELIEF_NONE);
    gtk_button_set_focus_on_click (GTK_BUTTON(buttons[i]),FALSE);

    sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%s°\n%s°</span>",
            	_weather_font_color.red >> 8,
        	_weather_font_color.green >> 8,
        	_weather_font_color.blue >> 8,
                weather_days[i].dayshname,
		weather_days[i].hi_temp,
		weather_days[i].low_temp);    
    labels[i]=gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (labels[i]),buffer);
    
    gtk_label_set_justify(GTK_LABEL (labels[i]),GTK_JUSTIFY_RIGHT);
    pfd = pango_context_get_font_description( gtk_widget_get_pango_context(labels[i]) );
    /* Select size font on desktop */
    if (strcmp(_weather_icon_size,"Large") == 0)
     pango_font_description_set_absolute_size(pfd,FONT_MAIN_SIZE_LARGE*PANGO_SCALE);
    else
     pango_font_description_set_absolute_size(pfd,FONT_MAIN_SIZE_SMALL*PANGO_SCALE);    
    gtk_widget_modify_font( labels[i],pfd );

    /* Create box for image and label */
    boxs[i] = gtk_hbox_new (FALSE, 0);

    sprintf(buffer,"%s%i.png",path_large_icon,weather_days[i].day.icon);    
    /* Select size Icon */
    if (strcmp(_weather_icon_size,"Large") == 0)
     icon = gdk_pixbuf_new_from_file_at_size (buffer,64,64,NULL);
    else 
     icon = gdk_pixbuf_new_from_file_at_size (buffer,32,32,NULL);
    icon_image = gtk_image_new_from_pixbuf (icon);
    
    gtk_box_pack_start (GTK_BOX (boxs[i]), icon_image, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (boxs[i]), labels[i], FALSE, FALSE, 0);
    gtk_container_add (GTK_CONTAINER (buttons[i]), boxs[i]);
    gtk_box_pack_start (GTK_BOX (box), buttons[i], FALSE, FALSE, 0);
    g_signal_connect (buttons[i], "released",
		    G_CALLBACK (weather_window_popup_show),		
		    NULL);  
		    
    g_signal_connect (buttons[i], "enter",
		    G_CALLBACK (enter_button),
		    NULL);  
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
 if (get_weather_html() == 0)
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
 update_weather()
{
 create_window_update();
 flag_update = g_timeout_add (100, update_w, box);
 
 // g_signal_handler_disconnect(G_OBJECT(weather_window_popup),signal_press_popup_window);
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
