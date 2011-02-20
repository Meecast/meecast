/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/

#include "meego-netbookUX-common.h"
#include <meego-panel/mpl-panel-clutter.h>
#include <meego-panel/mpl-panel-common.h>
#include <mx/mx.h>

#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>

#include <libintl.h>
#include <locale.h>

/*#define _(String) dgettext (GETTEXT_PACKAGE, String)*/
#define GETTEXT_PACKAGE "omweather"
#include <glib/gi18n-lib.h>

#include <sstream>
#include <pthread.h>

#define PANEL_HEIGHT 150
void finish_update(void);
gboolean g_finish_update(gpointer data);
void init_omweather_core(void);
Core::DataParser *current_data(std::string& str);

static void make_window_content (MplPanelClutter *panel);
ClutterTimeline *create_update_animation(ClutterActor *actor);
void make_bottom_content(Core::Data *temp_data); 
void make_bottom_content_about(); 
Core::Config *create_and_fill_config(void);
GHashTable *hash_table_create(void);
void change_path(ClutterPath *path, gint need_size);
void change_actor_size_and_position(ClutterActor *actor, gint need_size);

/* Global section */
Core::Config *config;
Core::StationsList stationslist;
MplPanelClient *panel = NULL;
ClutterActor   *panel_container = NULL;
ClutterActor   *bottom_container = NULL;
bool updating = false;
ClutterTimeline *refresh_timeline = NULL;
ClutterLayoutManager *main_vertical_layout = NULL;
DBusConnection       *dbus_conn;
DBusConnection       *dbus_conn_session;
GHashTable           *translate_hash=NULL;
guint timer = 0; /* timer */
pthread_t tid;

FILE *file;

static void* update_weather_forecast(void* data){
    int i;
    int success = 0;
    Core::Station* station;
    
    //file = fopen("/tmp/1.log","ab");
    //fprintf(file, "in thread statiion count = %d\n", config->stationsList().size());
	//fclose(file);
	//sleep(5);
    for (i=0; i < config->stationsList().size();i++){
        //file = fopen("/tmp/1.log","ab");
        //fprintf(file, "in thread i = %d\n", i);
	    //fclose(file);
       
        station = config->stationsList().at(i);
        if (station->updateData(true)){
            success ++;
            //file = fopen("/tmp/1.log","ab");
            //fprintf(file, "in thread success = %d\n", success);
	        //fclose(file);
        }
    }
                                                    //return success;
    	//pthread_once_t once_control = PTHREAD_ONCE_INIT;
    	//finish_update();
    	/*
    int status = pthread_once(&once_control, finish_update);
    file = fopen("/tmp/1.log", "ab");
	fprintf(file, "after pthread once status = %d\n", status);
	fclose(file);

    if (status < 0){
	file = fopen("/tmp/1.log", "ab");
	fprintf(file, "pthread once failed %d\n", status);
	fclose(file);
    }*/
    updating = false;
    //file = fopen("/tmp/1.log","ab");
    //fprintf(file, "end thread\n");
	//fclose(file);

    //pthread_exit((void *)0);
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////
gboolean
detail_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){

    mpl_panel_client_set_height_request (panel, PANEL_HEIGHT + 250);
    make_bottom_content((Core::Data*)user_data);
}
//////////////////////////////////////////////////////////////////////////////
gboolean
remove_detail_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){
  clutter_actor_destroy(actor);
  mpl_panel_client_set_height_request (panel, PANEL_HEIGHT);
}
//////////////////////////////////////////////////////////////////////////////
gboolean
station_button_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){

    if (config->current_station_id() + 1 < config->stationsList().size()) 
       config->current_station_id(config->current_station_id() + 1);
    else
       config->current_station_id(0);

    make_window_content((MplPanelClutter*)user_data);
    mpl_panel_client_set_height_request (panel, PANEL_HEIGHT);
//    mpl_panel_client_show((MplPanelClient*)user_data);
}

//////////////////////////////////////////////////////////////////////////////
gboolean
refresh_button_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){
    if (updating)
        return false; 
    clutter_timeline_start(refresh_timeline);
    
    //pthread_t tid;
    //pthread_attr_t attr;
    int error;
    //pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    //error = pthread_create(&tid, &attr, update_weather_forecast, NULL);
    //error = g_thread_create(&tid, &attr, update_weather_forecast, NULL);
    if (!g_thread_create(update_weather_forecast, NULL, false, NULL)) {
        std::cerr << "error run thread " << error << std::endl;
        file = fopen("/tmp/1.log","ab");
        fprintf(file, "error run thread= %d\n", error);
	    fclose(file);
    }else {
        updating = true;
	    timer = g_timeout_add(1000, g_finish_update, NULL);
    }
    
}
gboolean g_finish_update(gpointer data)
{
    //int error = pthread_tryjoin_np(tid, NULL);
    
    //file = fopen("/tmp/1.log", "ab");
    //fprintf(file, "in timer status = %d\n", updating);
    //fclose(file);
    if (updating == false){
        //updating = false;
        clutter_timeline_stop(refresh_timeline);
        make_window_content((MplPanelClutter*)panel);
        mpl_panel_client_set_height_request (panel, PANEL_HEIGHT);
        return false;
    }
    return true;
}
void finish_update(void)
{
    file = fopen("/tmp/1.log", "ab");
    fprintf(file, "in finish update\n");
    fclose(file);
    clutter_timeline_stop(refresh_timeline);
    make_window_content((MplPanelClutter*)panel);
}
//////////////////////////////////////////////////////////////////////////////
gboolean
config_button_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){
    char *args[] = {"/usr/bin/omweather-settings", (char *) 0 };

    pid_t pID = fork();
    if (pID == 0)
        execv("/usr/bin/omweather-settings", args );
    else
        mpl_panel_client_hide(panel);

}
//////////////////////////////////////////////////////////////////////////////
gboolean
about_button_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){
    
    mpl_panel_client_set_height_request (panel, PANEL_HEIGHT + 250);
    make_bottom_content_about();
   
}

//////////////////////////////////////////////////////////////////////////////
static ClutterActor*
make_day_actor(Core::Data *temp_data){
    ClutterActor     *box;
    ClutterActor     *label;
    ClutterActor     *icon;
    ClutterLayoutManager *layout;

    char             buffer[4096];
    if (temp_data)
          snprintf(buffer, (4096 -1), "%s/icons/%s/%i.png",config->prefix_path().c_str(), config->iconSet().c_str(), temp_data->Icon());
    else
          snprintf(buffer, (4096 -1), "%s/icons/%s/na.png",config->prefix_path().c_str(), config->iconSet().c_str());  

    icon = clutter_texture_new_from_file(buffer, NULL);
    clutter_actor_set_size (icon, 80.0, 80.0);
    //clutter_actor_show (icon);
    label = clutter_text_new();
    if (temp_data){
        if (temp_data->temperature_low().value() != INT_MAX)
            snprintf(buffer, (4096 -1), "%s\n%0.f°%s\n%0.f°%s", temp_data->ShortDayName().c_str(), temp_data->temperature_low().value(), config->TemperatureUnit().c_str(), temp_data->temperature_hi().value(), config->TemperatureUnit().c_str());
        else
            snprintf(buffer, (4096 -1), "%s\n%0.f°%s", temp_data->ShortDayName().c_str(), temp_data->temperature_hi().value(), config->TemperatureUnit().c_str());
    }else
        snprintf(buffer, (4096 -1), "N/A°C\nN/A°C");

    clutter_text_set_text((ClutterText*)label, buffer);
    layout = clutter_box_layout_new ();
    box =  clutter_box_new(layout);
    clutter_box_pack((ClutterBox*)box, icon, NULL);
    clutter_box_pack((ClutterBox*)box, label, NULL);
    clutter_actor_set_reactive(box, TRUE);
    /* connect the press event on refresh button */
    g_signal_connect (box, "button-press-event", G_CALLBACK (detail_event_cb),temp_data );

    return box;
}

void
make_bottom_content_about() {
  
  ClutterLayoutManager *bottom_layout;
  ClutterActor     *icon;
  ClutterActor     *label;
  std::string      about_text;
  PangoFontDescription *pfd = NULL;
  ClutterLayoutManager *layout;
  ClutterActor     *box;
  char             buffer[4096];
  std::ostringstream ss;

  if (mpl_panel_client_get_height_request (panel) > PANEL_HEIGHT){
      if (bottom_container)
          clutter_actor_destroy(bottom_container);
  }

  /* bottom layout */

  bottom_layout = clutter_box_layout_new();
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(bottom_layout), TRUE);
  bottom_container = clutter_box_new(bottom_layout);

  layout = clutter_box_layout_new ();
  box =  clutter_box_new(layout);

  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.6);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  clutter_text_set_text((ClutterText*)label, _("About"));
  clutter_box_pack((ClutterBox*)bottom_container, label, NULL);


  /* icon */
  snprintf(buffer, (4096 -1), "%s/icons/%s/%i.png",config->prefix_path().c_str(),
                                  config->iconSet().c_str(), 21);

  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 256.0, 256.0);
  clutter_box_pack((ClutterBox*)box, icon, NULL);
  clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(bottom_layout), icon,
				CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_CENTER);
  
  label = clutter_text_new();
  about_text = _("Weather forecast for the Meego. Version 0.3.\n");
  about_text += _("Author and maintenance: Vlad Vasiliev, <vlad@gas.by>\n");
  about_text += _("Maintenance: Pavel Fialko, <pavelnf@gmail.com>\nTanya Makova, <tanyshk@gmail.com>\n");
  about_text += _("Design UI and default iconset: Andrew Zhilin, <az@pocketpcrussia.com>\n");

  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.4);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  
  clutter_text_set_text((ClutterText*)label, about_text.c_str());
  clutter_box_pack((ClutterBox*)box, label, NULL);

  clutter_box_pack((ClutterBox*)bottom_container, box, NULL);

  /* connect the press event on refresh button */
  g_signal_connect (bottom_container, "button-press-event", G_CALLBACK (remove_detail_event_cb), panel);
  clutter_actor_set_reactive(bottom_container, TRUE);

  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), bottom_container,
                          TRUE, TRUE, TRUE, CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_CENTER);


}

//////////////////////////////////////////////////////////////////////////////
void
make_bottom_content(Core::Data *temp_data) {
  
  ClutterLayoutManager *bottom_layout;
  ClutterActor     *icon = NULL;
  ClutterActor     *label;
  ClutterLayoutManager *layout;
  ClutterActor     *vertical_container;
  ClutterLayoutManager *vertical_layout = NULL;
  ClutterActor     *box;
  std::string      day_name;
  char             buffer[4096];
  PangoFontDescription *pfd = NULL;
  std::ostringstream ss;
  int i;
  GList  *list, *l;
  GObject *object;
  ClutterScript   *script;
  ClutterTimeline *timeline;
  GError *error = NULL;

  if (mpl_panel_client_get_height_request (panel) > PANEL_HEIGHT){
      if (bottom_container)
          clutter_actor_destroy(bottom_container);
  }
  /* To do Display message */
  if (!temp_data)
      return;
  /* bottom layout */
  
  bottom_layout = clutter_box_layout_new();
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(bottom_layout), TRUE);
  bottom_container = clutter_box_new(bottom_layout);
  /* icon */
  if (temp_data)
      snprintf(buffer, (4096 -1), "%s/icons/%s/%i.%s",config->prefix_path().c_str(),
                                  config->iconSet().c_str(), temp_data->Icon(), "json");
  else
      snprintf(buffer, (4096 -1), "%s/icons/%s/na.%s",config->prefix_path().c_str(), 
                                  config->iconSet().c_str(),"json");  

 // icon = clutter_texture_new_from_file(buffer, NULL);
    script = clutter_script_new();
/*    g_object_unref(oh->script); */
    fprintf(stderr,"JSON SCRIPT: %s\n",buffer);
    clutter_script_load_from_file(script,buffer, &error);
    

    if (error){
        fprintf(stderr,"ERROR in loading clutter script\n");
        g_clear_error (&error);
    }else{
        if (temp_data)
            sprintf(buffer, "icon_name_%i", temp_data->Icon());
        else
            sprintf(buffer, "icon_name_na");
        icon = CLUTTER_ACTOR (clutter_script_get_object (script, buffer));
        fprintf(stderr,"icon %p", icon);
        timeline = CLUTTER_TIMELINE (clutter_script_get_object (script, "main-timeline"));
        clutter_actor_set_size (icon, 256.0, 256.0);
        
        if CLUTTER_IS_GROUP(icon)
           for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(icon)); i++)
               change_actor_size_and_position(clutter_group_get_nth_child(CLUTTER_GROUP(icon),i), 256);
        else
           change_actor_size_and_position(icon, 256);
        list = clutter_script_list_objects(script);
        for (l = list; l != NULL; l = l->next){
           object = (GObject *)l->data;
           if CLUTTER_IS_BEHAVIOUR_PATH(object)
               change_path(clutter_behaviour_path_get_path((ClutterBehaviourPath *)(object)), 256);
        }
        clutter_actor_show (CLUTTER_ACTOR (icon));
        clutter_timeline_start (timeline);
    }

  layout = clutter_box_layout_new ();
  box =  clutter_box_new(layout);
  clutter_box_pack((ClutterBox*)box, icon, NULL);
  clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(layout), icon,
				CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_CENTER);
  
  /* vertical container */
  vertical_layout = clutter_box_layout_new ();
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(vertical_layout), TRUE);
  
  vertical_container =  clutter_box_new(vertical_layout);
  //clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(layout), box, 
	//		    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  //clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(main_vertical_layout), TRUE);
  /* Day name */
  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.5);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  if (temp_data)
      if (temp_data->Current())
          day_name = _("Now");
      else
          day_name = temp_data->FullDayName() + " " + temp_data->DayOfMonthName() +", " + temp_data->FullMonthName(); 
  
  clutter_text_set_text((ClutterText*)label, day_name.c_str());
  clutter_box_pack((ClutterBox*)bottom_container, label, NULL);
  
  if (temp_data->Text().compare("N/A") != 0){
    label = clutter_text_new();
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    clutter_text_set_text((ClutterText*)label,
                      (const gchar*)g_hash_table_lookup(translate_hash,
                                    (void*) temp_data->Text().c_str()));
    clutter_box_pack((ClutterBox*)vertical_container, label, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(vertical_layout), label, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }
  
  if (temp_data->temperature_hi().value() != INT_MAX){
    label = clutter_text_new();
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Temperature:");
    ss << " ";
    if (temp_data->temperature_low().value() != INT_MAX){
	ss << temp_data->temperature_low().value() << "°" << config->TemperatureUnit() << " .. ";
    }
    ss << temp_data->temperature_hi().value() << "°" << config->TemperatureUnit();
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)vertical_container, label, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(vertical_layout), label, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }
  
  if (temp_data->Pressure() != INT_MAX){
    label = clutter_text_new();
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Pressure:") <<" "<< temp_data->Pressure();
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)vertical_container, label, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(vertical_layout), label, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }

  if (temp_data->Humidity() != INT_MAX){
    label = clutter_text_new();
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Humidity:") <<" "<< temp_data->Humidity() << "%";
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)vertical_container, label, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(vertical_layout), label, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }

  if (temp_data->WindDirection().compare("N/A") != 0){
    label = clutter_text_new();
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Wind:") << " "<< _(temp_data->WindDirection().c_str());
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)vertical_container, label, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(vertical_layout), label, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }

  if (temp_data->WindSpeed() != INT_MAX){
    label = clutter_text_new();
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Speed:") <<" "<<temp_data->WindSpeed() << _("m/s");
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)vertical_container, label, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(vertical_layout), label, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }
  
  clutter_box_pack((ClutterBox*)bottom_container, box, NULL);
  //clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(bottom_layout), box,
//				CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  clutter_box_pack((ClutterBox*)box, vertical_container, NULL);
  clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(layout), vertical_container,
				CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_CENTER);

  /* connect the press event on refresh button */
  g_signal_connect (bottom_container, "button-press-event", G_CALLBACK (remove_detail_event_cb), panel);
  clutter_actor_set_reactive(bottom_container, TRUE);

  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), bottom_container,
                          TRUE, TRUE, TRUE, CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_CENTER);


}
//////////////////////////////////////////////////////////////////////////////
static void
make_window_content (MplPanelClutter *panel)
{
  ClutterActor     *stage = mpl_panel_clutter_get_stage (panel);
  ClutterLayoutManager *forecast_layout;
  ClutterLayoutManager *top_layout;
  ClutterActor     *forecast_horizontal_container;
  ClutterActor     *top_container;
  ClutterActor     *box;
  ClutterActor     *label;
  ClutterActor     *icon;
  char             buffer[4096];
  int i, period;
  Core::Data *temp_data = NULL;
  Core::DataParser* dp = NULL;
  PangoFontDescription *pfd = NULL;

  if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0 
                                              &&  config->stationsList().at(config->current_station_id()))
      dp = current_data(config->stationsList().at(config->current_station_id())->fileName());

  main_vertical_layout = clutter_box_layout_new ();
  if (panel_container)
      clutter_actor_destroy(panel_container);
  if (refresh_timeline)
       g_object_unref(refresh_timeline);
  panel_container =  clutter_box_new(main_vertical_layout);
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(main_vertical_layout), TRUE);

  forecast_layout = clutter_box_layout_new(); 
  forecast_horizontal_container = clutter_box_new(forecast_layout);
  clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (forecast_layout), 9);
  clutter_actor_set_width(forecast_horizontal_container , 1024-10);

  /* top layout */
  top_layout = clutter_box_layout_new(); 
  top_container = clutter_box_new(top_layout);
  clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (top_layout), 9);

  
  /* station name */
  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 2);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  stationslist = config->stationsList();
  if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0
                                              &&  config->stationsList().at(config->current_station_id()))
      clutter_text_set_text((ClutterText*)label, config->stationsList().at(config->current_station_id())->name().c_str());
  else
      clutter_text_set_text((ClutterText*)label, "Unknown");
  clutter_actor_set_reactive(label, TRUE);
  g_signal_connect (label, "button-press-event", G_CALLBACK (station_button_event_cb), panel);
  clutter_box_pack((ClutterBox*)top_container, label, NULL);

  /* null button */
  snprintf(buffer, (4096 -1), "%s/buttons_icons/null.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 48.0, 48.0);
  clutter_box_pack((ClutterBox*)top_container, icon, "expand", TRUE,  "x-fill", TRUE, NULL);

  /* about button */
  snprintf(buffer, (4096 -1), "%s/buttons_icons/about.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 48.0, 48.0);
  clutter_actor_set_reactive(icon, TRUE);
  /* connect the press event on about button */
  g_signal_connect (icon, "button-press-event", G_CALLBACK (about_button_event_cb), panel);
  clutter_box_pack((ClutterBox*)top_container, icon, "x-align", CLUTTER_BOX_ALIGNMENT_END, "x-fill", TRUE, NULL);

  /* config button */
  snprintf(buffer, (4096 -1), "%s/buttons_icons/config.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 48.0, 48.0);
  clutter_actor_set_reactive(icon, TRUE);
  /* connect the press event on config button */
  g_signal_connect (icon, "button-press-event", G_CALLBACK (config_button_event_cb), NULL);
  clutter_box_pack((ClutterBox*)top_container, icon, "x-align", CLUTTER_BOX_ALIGNMENT_END, "x-fill", TRUE, NULL);

  /* refresh button */
  snprintf(buffer, (4096 -1), "%s/buttons_icons/refresh.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 48.0, 48.0);

  clutter_actor_set_reactive(icon, TRUE);
  refresh_timeline = create_update_animation(icon);
  /* connect the press event on refresh button */
  g_signal_connect (icon, "button-press-event", G_CALLBACK (refresh_button_event_cb), panel);
  clutter_box_pack((ClutterBox*)top_container, icon, "x-align", CLUTTER_BOX_ALIGNMENT_END, "x-fill", TRUE, NULL);

  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), top_container,
                          TRUE, TRUE, TRUE, CLUTTER_BOX_ALIGNMENT_CENTER, CLUTTER_BOX_ALIGNMENT_CENTER);

  /* Change panel icon */
  if (dp)
      temp_data = dp->data().GetDataForTime(time(NULL));
  if (temp_data ){
      snprintf(buffer, (4096 -1), "icon%i", temp_data->Icon());
      mpl_panel_client_request_button_style (MPL_PANEL_CLIENT(panel), buffer);
  }else
      mpl_panel_client_request_button_style (MPL_PANEL_CLIENT(panel), "iconna");

  /* day buttons */
  period = 0;
  for (i = 0; i < 9; i++){
      if (dp)
          temp_data = dp->data().GetDataForTime(time(NULL) + period);
      else
          temp_data = NULL;
      period = period + 3600*24;
      box = make_day_actor(temp_data);
        clutter_box_pack((ClutterBox*)forecast_horizontal_container, box, NULL);
  }
  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), forecast_horizontal_container, 
                          FALSE, TRUE, TRUE, CLUTTER_BOX_ALIGNMENT_CENTER, CLUTTER_BOX_ALIGNMENT_START);
  clutter_actor_show (panel_container);
  mpl_panel_clutter_set_child (panel, panel_container);
}

DBusHandlerResult
get_omweather_signal_cb(DBusConnection *conn, DBusMessage *msg, gpointer data){
    if (dbus_message_is_signal(msg, "org.meego.omweather", "reload_config")){
        delete config;
        config = create_and_fill_config();
        make_window_content(MPL_PANEL_CLUTTER (panel));
    }
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}
void
dbus_init(void){
  DBusError   error;

  dbus_error_init (&error);
  dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
  dbus_conn_session = dbus_bus_get(DBUS_BUS_SESSION, NULL);
  if (dbus_conn_session){
      dbus_bus_add_match(dbus_conn_session, "type='signal', interface='org.meego.omweather'", &error);
      if (dbus_error_is_set(&error)){
           fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
           dbus_error_free(&error);
      }
      /* add the callback */
      dbus_connection_add_filter(dbus_conn_session,
                                 get_omweather_signal_cb,
                                 NULL, NULL);
  }

}
int
main (int argc, char *argv[])
{
  //FILE *file;
  char buffer[4096];
  Core::Data *temp_data = NULL;
  Core::DataParser* dp = NULL;

  g_thread_init(NULL);
  clutter_threads_init();
  clutter_init (&argc, &argv);

  init_omweather_core();
  translate_hash = hash_table_create();
  /* prepairing icon */
  if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0 &&
      config->stationsList().at(config->current_station_id()))
      dp = current_data(config->stationsList().at(config->current_station_id())->fileName());

  if (dp)
      temp_data = dp->data().GetDataForTime(time(NULL));
  if (temp_data )
      snprintf(buffer, (4096 -1), "icon%i", temp_data->Icon());
  else
      snprintf(buffer, (4096 -1), "iconna");

  /* init Meego panel */
  mx_style_load_from_file (mx_style_get_default (),
                          "/usr/share/meego-panel-omweather/theme/omweather-panel.css", NULL);

  panel = mpl_panel_clutter_new ("omweather",           /* the panel slot */
                                 "omweather",           /* tooltip */
                                 "/usr/share/meego-panel-omweather/theme/omweather-panel.css", /*stylesheet */
                                 buffer,                /* button style */
                                 TRUE);
  mpl_panel_client_set_height_request (panel, PANEL_HEIGHT);
  mpl_panel_client_request_button_style (MPL_PANEL_CLIENT(panel), buffer);

  //update_weather_forecast(config);
  make_window_content (MPL_PANEL_CLUTTER (panel));
  file = fopen("/tmp/1.log","ab");
  fclose(file);
  dbus_init();
  clutter_threads_enter();
  clutter_main ();
  clutter_threads_leave();

  return 0;
}
