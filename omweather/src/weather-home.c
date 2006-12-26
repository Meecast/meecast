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
#include "weather-home.h"

#include <unistd.h>

#define VERSION "0.14"
#define APPNAME "omweather"

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
//      g_string_append (url,"s=");
    memset(&hExtra, '\0', sizeof(hExtra));
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

/* Change station at main display */
static gboolean
change_station (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{

  GSList *tmplist = NULL;
  struct weather_station *ws;
  tmplist = stations_view_list;
  while (tmplist != NULL)
  {
   ws = tmplist->data;
   if ( ws->button == widget ) 
   {
     /* Check active station */ 
     if (strcmp(_weather_station_id, ws->id_station)!=0)
     {
      if (_weather_station_id != NULL) g_free(_weather_station_id);
        _weather_station_id = g_strdup(ws->id_station); 
      weather_frame_update();
      config_save_current_station();
      break;
     } 
   }     
   tmplist = g_slist_next(tmplist);
  }
 return TRUE;
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
      weather_frame_update();
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
      if (tmplist == NULL)
          tmplist = stations_view_list;
      /* Get station data */
      ws = tmplist ->data;
      if (_weather_station_id != NULL) g_free(_weather_station_id);
        _weather_station_id = g_strdup(ws->id_station); 
      weather_frame_update();
      config_save_current_station();
      break;
     }
   /* Next station in list */
   tmplist = g_slist_next(tmplist);
  }
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
  GtkWidget *stations_hbox, *box_not_station, *station_name_button;
  GtkWidget *label_start,*label_end, *button_prev_st, *button_next_st, *box_prev_st, *box_next_st;

  int i, offset, count_day;
  gchar buffer[2048];
  gchar buffer_icon[2048];
  time_t current_day,current_time,last_day;
  struct tm *tm;
  gboolean flag_last_day;
  guint count_stations;
  GSList *tmplist = NULL;
  struct weather_station *ws;
  int temp_hi,temp_hi_now,temp_low;

  flag_last_day = FALSE;
  offset = 0;
  last_day = 0;
  /* Init weather buttons */
  weather_buttons_init();
  count_day=parse_weather_com_xml();
//  if  (count_day == -1)  {count_day=0; fprintf(stderr,"Error on xml file");} // Error on xml file

  /* get current day */  
  current_time = current_day = time(NULL);
  tm=localtime(&current_day);
  tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
  current_day = mktime(tm);
  /* free time event list */
  free_list_time_event();
  /* add periodic update */
  add_periodic_event();


  /* Search day of saving xml near current day */
  while ((offset<Max_count_weather_day) && (current_day > weather_days[offset].date_time) && (offset<count_day) )
  {
   offset ++;
  }

  /* Select layout for button */
  if (_weather_layout == HORIZONTAL)
   box = gtk_hbox_new ( FALSE , 0);    
  else
   box = gtk_vbox_new ( FALSE , 0);    
   
  for (i=0;i<Max_count_web_button; i++)
  {    
     /* If it first button add to evenet time change between nigth and day */
    if  (current_day == weather_days[offset].date_time) 
    {
     if (current_time < weather_days[offset].day.begin_time)
      time_event_add(weather_days[offset].day.begin_time,DAYTIMEEVENT);
     if (current_time < weather_days[offset].night.begin_time)  
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
     /* Prepare temperature value to show on display */
     temp_hi_now=atoi(weather_current_day.day.temp); 
     temp_hi=atoi(weather_days[offset].hi_temp);     
     temp_low=atoi(weather_days[offset].low_temp);     
     if (_weather_temperature_unit == 'F')
     {
       temp_hi_now = c2f(temp_hi_now);
       temp_hi = c2f(temp_hi);
       temp_low = c2f(temp_low);
     }  
     /* Show or current weather or forecast */
     if ((i==0) && (weather_current_day.date_time>(current_time-OFFSET_CURRENT_WEATHER*3600)) 
                && (weather_current_day.date_time<(current_time+OFFSET_CURRENT_WEATHER*3600)))
     {
      /* Add to evenet time change between current weather and forecast */
      time_event_add((weather_current_day.date_time+OFFSET_CURRENT_WEATHER*3600),DAYTIMEEVENT);
      /* Show current weather bold fonts */
      sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>%s\n%i\302\260\n</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
                weather_days[offset].dayshname,
		temp_hi_now);
      sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_current_day.day.icon);    		       
     }		
     else
     {
      /* Show forecast */
      if  (current_day == weather_days[offset].date_time) 
      {
       /* First icon - night(morning) or day or night (evening) */     
       if (current_time<weather_days[offset].day.begin_time) 
       {  
         sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].night.icon);    
	 /* Show All temperatures */
         sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n%i\302\260</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
                weather_days[offset].dayshname,temp_low,temp_hi);
       } 
       else
       if (current_time<weather_days[offset].night.begin_time)
       {
        sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].day.icon);    
       /* Show All temperatures */
       sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n%i\302\260</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
                weather_days[offset].dayshname,temp_low,temp_hi);
       }		
       else
       {
         sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].night.icon);           
	 /* Show only night temperature */
         sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
                weather_days[offset].dayshname,temp_low); 
       }
      }
      else
      {
       /* Create output string for full time of day (day and night)*/
//       sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%s°\n%s°</span>",
       sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s\n%i\302\260\n%i\302\260</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
                weather_days[offset].dayshname,temp_hi,temp_low);
       sprintf(buffer_icon,"%s%i.png",path_large_icon,weather_days[offset].day.icon);    		
      }		    
     }
    } 
    else
    {
     /* Show N/A */
     sprintf(buffer,"<span foreground='#%02x%02x%02x'>N/A\nN/A\302\260\nN/A\302\260</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
     sprintf(buffer_icon,"%s48.png",path_large_icon);         
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
     if (_enable_transparency)
      gtk_button_set_relief (GTK_BUTTON(buttons[i]),GTK_RELIEF_NONE);
     gtk_button_set_focus_on_click (GTK_BUTTON(buttons[i]),FALSE);    
     labels[i]=gtk_label_new (NULL);
     gtk_label_set_markup (GTK_LABEL (labels[i]),buffer);
     gtk_label_set_justify(GTK_LABEL (labels[i]),GTK_JUSTIFY_RIGHT);
     /* Select size font on desktop */
     if (strcmp(_weather_icon_size,"Large") == 0)
      set_font_size(labels[i],FONT_MAIN_SIZE_LARGE);
     else
      if (strcmp(_weather_icon_size,"Medium") == 0)
       set_font_size(labels[i],FONT_MAIN_SIZE_MEDIUM);      
      else
       set_font_size(labels[i],FONT_MAIN_SIZE_SMALL);
     /* Create box for image and label */
     boxs[i] = gtk_hbox_new (FALSE, 0);
     /* Select size Icon */
     if (strcmp(_weather_icon_size,"Large") == 0)
      icon = gdk_pixbuf_new_from_file_at_size (buffer_icon,64,64,NULL);
     else
      if (strcmp(_weather_icon_size,"Medium") == 0)
       icon = gdk_pixbuf_new_from_file_at_size (buffer_icon,48,48,NULL);
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
  

   /* Forming table with name station and button */
   count_stations = g_slist_length(stations_view_list);
   if (count_stations >0)
    gtk_box_pack_start(GTK_BOX(box_zero),
                      main_table = gtk_table_new( 2,count_stations,FALSE), TRUE, TRUE, 0);
   else
   {
    /* Forming label NO STATION */
    gtk_box_pack_start(GTK_BOX(box_zero),
                      main_table = gtk_table_new( 2,1,FALSE), TRUE, TRUE, 0);   		      
    gtk_table_attach_defaults(GTK_TABLE(main_table),	    
            box_not_station = gtk_button_new(),
	    0, 1 , 0, 1);		      
    if (_enable_transparency) 
     gtk_button_set_relief (GTK_BUTTON(box_not_station),GTK_RELIEF_NONE);
    gtk_button_set_focus_on_click (GTK_BUTTON(box_not_station),FALSE);    	    
    stations_hbox = gtk_hbox_new (FALSE, 0);
    label_start=gtk_label_new (NULL);
    sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>/ NO SELECTED STATION \\</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
    gtk_label_set_markup (GTK_LABEL (label_start),buffer);		

             
    if (strcmp(_weather_icon_size,"Large") == 0)
       set_font_size(label_start,FONT_MAIN_SIZE_LARGE);
    else 
     if (strcmp(_weather_icon_size,"Medium") == 0)
      set_font_size(label_start,FONT_MAIN_SIZE_MEDIUM);              
     else
      set_font_size(label_start,FONT_MAIN_SIZE_SMALL);              
       
    gtk_box_pack_start (GTK_BOX (stations_hbox), label_start, FALSE, FALSE, 0); 
    gtk_container_add (GTK_CONTAINER (box_not_station),stations_hbox);  
    
   }		      
		      
   tmplist = stations_view_list;
   i = 0;
   while (tmplist != NULL)
   {
      ws = tmplist->data;
      
      /* If Vertical layout and not selected station skip iteration */
      if ( (_weather_layout == VERTICAL) && (strcmp(ws->id_station,_weather_station_id) != 0)) 
      {
       tmplist = g_slist_next(tmplist);
       continue;
      }
      gtk_table_attach_defaults(GTK_TABLE(main_table),	    
                                ws->box = gtk_hbox_new (FALSE, 0),      
	                        i, i+1 , 0, 1);

      button_prev_st = gtk_button_new();
      button_next_st = gtk_button_new(); 
      ws->button = gtk_button_new(); 
      box_prev_st = gtk_hbox_new(FALSE, 0);
      box_next_st = gtk_hbox_new(FALSE, 0); 

      if (_enable_transparency)
      {
       gtk_button_set_relief (GTK_BUTTON(ws->button),GTK_RELIEF_NONE);
       gtk_button_set_relief (GTK_BUTTON(button_prev_st),GTK_RELIEF_NONE);
       gtk_button_set_relief (GTK_BUTTON(button_next_st),GTK_RELIEF_NONE);              
      } 
      gtk_button_set_focus_on_click (GTK_BUTTON(ws->button),FALSE);    	    
      gtk_button_set_focus_on_click (GTK_BUTTON(button_prev_st),FALSE);    	          
      gtk_button_set_focus_on_click (GTK_BUTTON(button_next_st),FALSE);    	    
      
      stations_hbox = gtk_hbox_new (FALSE, 0);
      ws->label_box=gtk_label_new (NULL);
      label_start=gtk_label_new (NULL);
      label_end=gtk_label_new (NULL);
      
      /* Check selected station */
      if ((_weather_station_id) && (strcmp(ws->id_station,_weather_station_id)==0))
      {
       sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>%s</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
                ws->name_station);
       gtk_label_set_markup (GTK_LABEL (ws->label_box),buffer);		

       if  (_weather_layout == VERTICAL)
        sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>&lt;</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
       else		
        sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>/</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
       
       gtk_label_set_markup (GTK_LABEL (label_start),buffer);		
       if  (_weather_layout == VERTICAL)
        sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>\></span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
       else
        sprintf(buffer,"<span weight=\"bold\" foreground='#%02x%02x%02x'>\\</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
		
       gtk_label_set_markup (GTK_LABEL (label_end),buffer);		
       
      }
      else
      {
       sprintf(buffer,"<span foreground='#%02x%02x%02x'>%s</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8,
                ws->name_station);
       gtk_label_set_markup (GTK_LABEL (ws->label_box),buffer);		
       sprintf(buffer,"<span  foreground='#%02x%02x%02x'>/</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
       gtk_label_set_markup (GTK_LABEL (label_start),buffer);				
       sprintf(buffer,"<span  foreground='#%02x%02x%02x'>\\</span>",
            	_weather_font_color.red >> 8,_weather_font_color.green >> 8,_weather_font_color.blue >> 8);
       gtk_label_set_markup (GTK_LABEL (label_end),buffer);				       
      }		


      if (strcmp(_weather_icon_size,"Large") == 0)
       set_font_size(ws->label_box,FONT_MAIN_SIZE_LARGE);
      else 
       if (strcmp(_weather_icon_size,"Medium") == 0)
        set_font_size(ws->label_box,FONT_MAIN_SIZE_MEDIUM);       
       else	     
        set_font_size(ws->label_box,FONT_MAIN_SIZE_SMALL);       
       
       
      if (_enable_transparency && (_weather_layout == HORIZONTAL))
      { 
       gtk_box_pack_start (GTK_BOX (stations_hbox), label_start, FALSE, FALSE, 0); 
       gtk_box_pack_end (GTK_BOX (stations_hbox), label_end, FALSE, FALSE, 0);       
      }
      
      if  (_weather_layout == VERTICAL)
      {      
       gtk_box_pack_start (GTK_BOX (box_prev_st), label_start, FALSE, FALSE, 0); 
       gtk_container_add (GTK_CONTAINER (button_prev_st),box_prev_st);       
       
       gtk_box_pack_start (GTK_BOX (box_next_st), label_end, FALSE, FALSE, 0); 
       gtk_container_add (GTK_CONTAINER (button_next_st),box_next_st);              
      
       gtk_box_pack_end (GTK_BOX (stations_hbox), ws->label_box, TRUE, TRUE, 0); 
       gtk_container_add (GTK_CONTAINER (ws->button),stations_hbox);       
       
       gtk_box_pack_start (GTK_BOX (ws->box), button_prev_st, FALSE, FALSE, 0); 
       gtk_box_pack_start (GTK_BOX (ws->box), ws->button, FALSE, FALSE, 0); 
       gtk_box_pack_start (GTK_BOX (ws->box), button_next_st, FALSE, FALSE, 0);        

       
       /* Connect signal button */
       g_signal_connect (button_prev_st, "released",
		        G_CALLBACK (change_station_prev),		
		        NULL);  		    
       g_signal_connect (button_prev_st, "enter",
     		        G_CALLBACK (enter_button),
     		        NULL); 

       /* Connect signal button */
       g_signal_connect (button_next_st, "released",
		        G_CALLBACK (change_station_next),		
		        NULL);  		    
       g_signal_connect (button_next_st, "enter",
     		        G_CALLBACK (enter_button),
     		        NULL); 

      }
      else
      {
       gtk_box_pack_end (GTK_BOX (stations_hbox), ws->label_box, TRUE, TRUE, 0); 
       gtk_container_add (GTK_CONTAINER (ws->button),stations_hbox);       
       gtk_container_add (GTK_CONTAINER (ws->box),ws->button);
      }			

       /* Connect signal button */
       g_signal_connect (ws->button, "released",
		        G_CALLBACK (change_station),		
		        NULL);  		    
       g_signal_connect (ws->button, "enter",
     		        G_CALLBACK (enter_button),
     		        NULL); 
      i++;	    
      tmplist = g_slist_next(tmplist);
   }
//      /* Select size font on desktop */	    
   if (count_stations >0)
   gtk_table_attach_defaults(GTK_TABLE(main_table),	    
            box,
            0,count_stations, 1, 2);
   else
   {
   gtk_table_attach_defaults(GTK_TABLE(main_table),	    
            box,
            0, 1, 1, 2);          	    
   }	    
   gtk_widget_show_all (box_zero);
}

GtkWidget *
weather_frame_new (void)
{
  box_zero =  gtk_hbox_new ( FALSE , 0); 
  weather_buttons_fill();
  return box_zero;
  
}

void
weather_frame_update (void)
{
  gtk_widget_destroy(main_table);
  box = gtk_hbox_new ( FALSE , 0);  
  weather_buttons_fill();
}

/* For window update */
static gboolean 
update_w(gpointer data)
{

 if (get_weather_html(TRUE) == 0)
 {
   weather_frame_update();
 }
 gtk_timeout_remove(flag_update);
 flag_update = 0;
 g_object_unref(G_OBJECT(update_window));
 if (update_window) gtk_widget_destroy (update_window);
 if (weather_window_popup) gtk_widget_destroy (weather_window_popup);
 return TRUE;
}

void 
update_weather(void)
{
 create_window_update();
 flag_update = g_timeout_add (100, (GFunc)update_w, NULL);
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
