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

#include "weather-home_preference.h"
/* Free memory allocated for stations list */
void free_list_stations (void)
{
 static GSList *stations_list_temp = NULL;
 struct station_and_weather_code *sc;

 if (stations_list != NULL)
 { 
  stations_list_temp = stations_list; 
  while (stations_list_temp != NULL)
  {
     sc = stations_list_temp->data;
     g_free(sc->station_name);
     g_free(sc->station_code);
     g_free(sc);
    stations_list_temp = g_slist_next(stations_list_temp);
  }
  g_slist_free(stations_list);
  stations_list = NULL;
 } 
}


void changed_country(void)
{
  GtkTreeModel *model;
  char flag; //Flag for country processing
  char flag_new_state; //Flag for new country or province or state
  unsigned char out_buffer[1024]; /* buffer for work with stations.txt files*/
  static gchar *gcountry_name = NULL;
  FILE *stations_file, *iso3166_file; 
  char country_name[52];
  char country_code [2];
  char temp_state_name[20];
  int count_state = 0; // Count state of file iso3166 


    /* Search Country defined ComboBox in iso file */
    flag = FALSE;    
    /* Clear the list. */
    model = gtk_combo_box_get_model (GTK_COMBO_BOX (states));
    gtk_list_store_clear (GTK_LIST_STORE (model));
    model = gtk_combo_box_get_model (GTK_COMBO_BOX (stations));
    gtk_list_store_clear (GTK_LIST_STORE (model));

    gcountry_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(countrys));
    /* Search country code */    
    if (iso3166_file = fopen(COUNTRYS_FILE,"r") )
    {
     while(!feof(iso3166_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); //Clear buffer
      fgets(out_buffer, sizeof(out_buffer), iso3166_file);//Read Next Line
      if (strlen(out_buffer)>0){
       if (strcmp("----------------------------------------------------------------------\n",out_buffer) == 0)
       {
         flag = (flag == TRUE) ? FALSE : TRUE ;
       } 	
       else
       if ( flag == TRUE)
       {
         if (strcmp("\n",out_buffer) != 0)
	 {
          sprintf(country_name,"%.38s",out_buffer);
	  if (strcmp(gcountry_name,country_name) == 0)
	  {
	    /* Write country code */
	    country_code[0]=out_buffer[48];
	    country_code[1]=out_buffer[49];	    
	    country_code[2]=0;
	  }    	  
	 } 
       }
      }	
     }
    }
    
    /* Search state or province on country and add it to combobox state */
    if(stations_file = fopen(STATIONS_FILE,"r"))
    {
     memset(temp_state_name, 0, sizeof(temp_state_name)); //Clear temporary value
     flag_new_state = FALSE;
     while(!feof(stations_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); //Clear buffer
      fgets(out_buffer, sizeof(out_buffer), stations_file);//Read Next Line
      if ( (strlen(out_buffer)>0) && (out_buffer[0] != "!") )
      {   
        /* Is it country or state or province name ? */
    	if ( out_buffer[19] != ' ' )
	{
	  sprintf(temp_state_name,"%.19s",out_buffer);
	  flag_new_state = TRUE;
	}  
	if ( (flag_new_state == TRUE) && country_code[0] == out_buffer[81] &&  country_code[1] == out_buffer[82])
	{
	    flag_new_state = FALSE;	
            gtk_combo_box_append_text(GTK_COMBO_BOX(states), temp_state_name);
	     count_state++;
	     if ((_weather_state_name != NULL) &&(strcmp(temp_state_name,_weather_state_name) == 0))
	     {

	      index_state = count_state;
	     }
	}    	  
      } 
     }
    }    
 g_free(gcountry_name);
 free_list_stations();
}


void changed_state(void)
{
  GtkTreeModel *model;
  char flag; //Flag for country processing
  char flag_necessary_state; //Flag for finding country or province or state
  unsigned char out_buffer[1024]; /* buffer for work with stations.txt files*/
  static gchar *gstate_name = NULL;
  FILE *stations_file; 
  char state_name[21];
  char temp_station_name[21];
  char temp_station_code[9];
  struct station_and_weather_code *sc;
  int count_station = 0; // Count station of state or region
  int i;
  

 /* Search Country in the ComboBox*/
    flag = FALSE;    
    /* Clear the list. */
    model = gtk_combo_box_get_model (GTK_COMBO_BOX (stations));
    gtk_list_store_clear (GTK_LIST_STORE (model));

    gstate_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(states));
    /* Search state or province on country and add stations to combobox*/
    if(stations_file = fopen(STATIONS_FILE,"r"))
    {
     memset(state_name, 0, sizeof(state_name)); //Clear temporary value
     flag_necessary_state = FALSE;
     while(!feof(stations_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); //Clear buffer
      fgets(out_buffer, sizeof(out_buffer), stations_file);//Read Next Line
      memset(temp_station_name, 0, sizeof(temp_station_name)); //Clear buffer
      if ( (strlen(out_buffer)>27) && ((char)out_buffer[0] != "!") )
      {  
        /* Is it country or state or province name ? */
    	if ( out_buffer[19] != ' ' )
	{
	  sprintf(state_name,"%.19s",out_buffer);
	  flag_necessary_state = (strcmp(state_name,gstate_name) == 0) ? TRUE : FALSE;
	} 

	/* Check for wrong string */ 
	if ((flag_necessary_state == TRUE)&& (strlen(out_buffer)>90) )
	{
	  /* Prepare strings station_name and code_name for work */
	  for (i=3;i<19;i++) temp_station_name[i-3] = out_buffer[i];
	  for (i=84;i<92;i++) temp_station_code[i-84] = out_buffer[i];
	  temp_station_code[8]=0;
	   /* Copy name station to combobox */
	  gtk_combo_box_append_text(GTK_COMBO_BOX(stations), temp_station_name);

          count_station++;
	  if ((_weather_station_name != NULL) &&(strcmp(temp_station_name,_weather_station_name) == 0))
	  {    
	    index_station = count_station;
          }
	
	  /* Add station and station code to list */	  
	  sc = g_new0(struct station_and_weather_code, 1);
	  sc->station_name = g_strdup(temp_station_name);	  
	  sc->station_code = g_strdup(temp_station_code);	  
	  stations_list = g_slist_append(stations_list, sc); /* Necessary free list  beyond !!! */	  
	}    	  
      } 
     }
    }    
 g_free(gstate_name);
}

void changed_stations(void)
{
  static gchar *gstation_name = NULL;
  struct station_and_weather_code *sc;
  static GSList *stations_list_temp = NULL;
  
  stations_list_temp = stations_list;
  
  gstation_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations));

  while (stations_list_temp != NULL)
  {
   sc = stations_list_temp->data;
   if (strcmp(sc->station_name,gstation_name) == 0)
   {
    if (weather_station_id != NULL) g_free(weather_station_id);
    weather_station_id = sc->station_code;	
    break;    
   }
   stations_list_temp = g_slist_next(stations_list_temp);
  }
}

/* Main window preference */
void
weather_window_preference (GtkWidget *widget,
                               GdkEvent *event,
                           gpointer user_data)
{

   FILE  *iso3166_file;
   char country_name[52];
  
   unsigned char out_buffer[1024]; /* buffer for work with stations.txt files*/
   GtkWidget *window_config, *notebook; 
   GtkWidget *label;
   GtkWidget *table;
   GtkWidget *font_color;
   char flag; //Flag for country processing
   char flag_update_icon; //Flag update main weather icon of desktop 
   char flag_update_xml; //Flag update xml file 
   int count_country = 0; // Count country of file iso3166 
   int index_country = 0; // Position country of the list 
   GdkColor _weather_font_color_temp; // Temporary for font color

//    gtk_widget_destroy(widget);
    window_config = gtk_dialog_new_with_buttons("Other Maemo Weather Settings",
            NULL, GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
            NULL);

//    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(window_config)->action_area),
//            btn_colors = gtk_button_new_with_label("Advanced..."));
    gtk_dialog_add_button(GTK_DIALOG(window_config),
            GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_config)->vbox),
            notebook = gtk_notebook_new(), TRUE, TRUE, 0);

    /* Location page. */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
            table = gtk_table_new(4, 2, FALSE),
            label = gtk_label_new("Location"));
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("Country:"),
            0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
            1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label),countrys = gtk_combo_box_new_text());

    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("State(Province):"),
            0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
            1, 2, 2, 3);
    gtk_container_add(GTK_CONTAINER(label),states = gtk_combo_box_new_text());

    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("Station(Place):"),
            0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
            1, 2, 3, 4);
    gtk_container_add(GTK_CONTAINER(label),stations = gtk_combo_box_new_text());

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
            table = gtk_table_new(1, 3, FALSE),
            label = gtk_label_new("Main Interface"));
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("Icon size:"),
            0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
            1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label),icon_size = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(icon_size), "Large");
    gtk_combo_box_append_text(GTK_COMBO_BOX(icon_size), "Small");
    if (strcmp(_weather_icon_size,"Large") == 0)
      gtk_combo_box_set_active (icon_size,0);
    else
      gtk_combo_box_set_active (icon_size,1);      

    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_label_new("Font color:"),
            0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
            label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
             1, 2, 1, 2);
    gtk_container_add(GTK_CONTAINER(label),font_color = gtk_color_button_new());
    gtk_color_button_set_color(font_color, &_weather_font_color);      

    /* Inserting Countrys to ComboBox from iso file*/
    flag = FALSE;
    if(iso3166_file = fopen(COUNTRYS_FILE,"r"))
    {
     while(!feof(iso3166_file))
     {
      memset(out_buffer, 0, sizeof(out_buffer)); //Clear buffer
      fgets(out_buffer, sizeof(out_buffer), iso3166_file);//Read Next Line
      if (strlen(out_buffer)>0){
       if (strcmp("----------------------------------------------------------------------\n",out_buffer) == 0)
       {
	flag = (flag == TRUE) ? FALSE : TRUE ;
       } 	
       else
       if ( flag == TRUE)
       {
         if (strcmp("\n",out_buffer) != 0)
	 {
          sprintf(country_name,"%.38s",out_buffer);
          gtk_combo_box_append_text(GTK_COMBO_BOX(countrys), country_name);
	  count_country++;
	  if ((_weather_country_name != NULL) &&(strcmp(country_name,_weather_country_name) == 0))
	  {
	    index_country = count_country;
	  }
	 } 
       }
      }	
     }
    }
    if (index_country != 0)
    {
     gtk_combo_box_set_active (countrys,index_country-1);      
     changed_country();
    }
     if (index_state != 0)
    {
     gtk_combo_box_set_active (states,index_state-1);      
     changed_state();
    }
    if (index_station != 0)
    {
     gtk_combo_box_set_active (stations,index_station-1);      
    }
        
    weather_station_id = g_strdup(_weather_station_id);

  g_signal_connect ((gpointer) countrys, "changed",
                    G_CALLBACK (changed_country),
                    NULL);	    
  g_signal_connect ((gpointer) states, "changed",
                    G_CALLBACK (changed_state),
                    NULL);	    
  g_signal_connect ((gpointer) stations, "changed",
                    G_CALLBACK (changed_stations),
                    NULL);	    

	    
   gtk_widget_show_all(window_config);
   flag_update_icon = FALSE;
   flag_update_xml = FALSE;
   while(GTK_RESPONSE_ACCEPT == gtk_dialog_run(GTK_DIALOG(window_config))) /* Press Buuton Ok */
    {
	_weather_station_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations));	
	if (_weather_station_name) /* If not select station not exit from prefernce window */
	{
	 _weather_country_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(countrys));
         _weather_state_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(states));

	 if ( strcmp(_weather_icon_size,gtk_combo_box_get_active_text(GTK_COMBO_BOX(icon_size))) != 0)
	 {  
	  flag_update_icon = TRUE;
	  _weather_icon_size = gtk_combo_box_get_active_text(GTK_COMBO_BOX(icon_size));	
	 }

	 if ( (_weather_station_id != NULL) && strcmp(_weather_station_id,weather_station_id) !=0 )
	  flag_update_xml = TRUE;

	 g_free(_weather_station_id);
	 _weather_station_id = g_strdup(weather_station_id);	
	 gtk_color_button_get_color(font_color, &_weather_font_color_temp);
	 if (( _weather_font_color_temp.red != _weather_font_color.red ) &&
	     ( _weather_font_color_temp.green != _weather_font_color.green ) &&
	     ( _weather_font_color_temp.blue != _weather_font_color.blue ))
	  {
           flag_update_icon = TRUE;     
	   _weather_font_color = _weather_font_color_temp;
	  } 

	 /* Necessary free list  beyond !!! */	   
         config_save();
	 if (flag_update_icon)
	 {
          weather_frame_update();
	  gtk_widget_destroy(weather_window_popup);
         }
	 
	 if (flag_update_xml)
	 {
	  update_weather();
          weather_frame_update();	  
         }

	 gtk_widget_destroy(window_config);
	 free_list_stations();
	 return;
	}
    }
   
   gtk_widget_destroy(weather_window_popup);    
   gtk_widget_destroy(window_config);

}
