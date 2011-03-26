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

#define PANEL_HEIGHT 224
void finish_update(void);
gboolean g_finish_update(gpointer data);
gboolean g_auto_update(gpointer data);
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
Core::DataParser* dp = NULL;
MplPanelClient *panel = NULL;
ClutterActor   *panel_container = NULL;
ClutterActor   *bottom_container = NULL;
bool updating = false;
ClutterTimeline *refresh_timeline = NULL;
ClutterLayoutManager *main_vertical_layout = NULL;
GHashTable           *translate_hash=NULL;
guint timer = 0; /* timer */
guint timer_update = 0; /* timer for update */
pthread_t tid;
FILE *file;
ClutterActor     *active_background = NULL;
bool connected = true;

static void* update_weather_forecast(void* data){
    int i;
    int success = 0;
    Core::Station* station;
    

    for (i=0; i < config->stationsList().size();i++){
        station = config->stationsList().at(i);
        if (station->updateData(true)){
            success ++;

        }
    }

    updating = false;
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////
gboolean
detail_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){

    ClutterActor *temp_actor;
    mpl_panel_client_set_height_request (panel, PANEL_HEIGHT + 250);
    make_bottom_content((Core::Data*)user_data);
    if (active_background && clutter_actor_get_name(active_background) != NULL
        && !strcmp(clutter_actor_get_name(active_background), "active")) 
        clutter_actor_hide(active_background);    

    for (int i=0; i<clutter_group_get_n_children(CLUTTER_GROUP(actor)); i++){
        temp_actor = clutter_group_get_nth_child(CLUTTER_GROUP(actor), i);
        if (clutter_actor_get_name(temp_actor) != NULL && 
            !strcmp(clutter_actor_get_name(temp_actor), "active")){
            clutter_actor_show(temp_actor);    
            active_background = temp_actor;
        }
    }
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
    
    int error;
    if (!g_thread_create(update_weather_forecast, NULL, false, NULL)) {
        std::cerr << "error run thread " << error << std::endl;
    }else {
        updating = true;
        timer = g_timeout_add(1000, g_finish_update, NULL);
    }
    
}
gboolean g_finish_update(gpointer data)
{
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
    clutter_timeline_stop(refresh_timeline);
    make_window_content((MplPanelClutter*)panel);
}
gboolean g_auto_update(gpointer data)
{
    if (connected == true){

        refresh_button_event_cb(NULL, NULL, NULL);
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////////
gboolean
config_button_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){
    char *args[] = {"/usr/bin/omweather-settings", (char *) 0 };

    pid_t pID = fork();
    if (pID == 0){
        execv("/usr/bin/omweather-settings", args );
    }else{
        make_window_content((MplPanelClutter*)panel);
        mpl_panel_client_set_height_request (panel, PANEL_HEIGHT);
        mpl_panel_client_hide(panel);
    }
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
    ClutterActor     *temperature_box;
    ClutterLayoutManager *temperature_layout;
    ClutterActor     *group;
    ClutterActor     *label_temp_low;
    ClutterActor     *label_temp_hi;
    ClutterActor     *label_day;
    ClutterActor     *icon;
    ClutterActor     *background_passive;
    ClutterActor     *background_active;
    ClutterActor     *rectangle;
    ClutterLayoutManager *layout;
    PangoFontDescription *pfd = NULL;

    char             buffer[4096];
    
    /* Backgrounds */
    snprintf(buffer, (4096 -1), "%s/buttons_icons/passive.png",config->prefix_path().c_str());
    background_passive = clutter_texture_new_from_file(buffer, NULL);
    snprintf(buffer, (4096 -1), "%s/buttons_icons/active.png",config->prefix_path().c_str());
    background_active = clutter_texture_new_from_file(buffer, NULL);
    clutter_actor_set_name(background_active, "active");

    /* Icon */
    if (temp_data)
          snprintf(buffer, (4096 -1), "%s/icons/%s/%i.png",config->prefix_path().c_str(), config->iconSet().c_str(), temp_data->Icon());
    else
          snprintf(buffer, (4096 -1), "%s/icons/%s/na.png",config->prefix_path().c_str(), config->iconSet().c_str());  

    icon = clutter_texture_new_from_file(buffer, NULL);
    clutter_actor_set_size (icon, 64.0, 64.0);

    /* Temperatures */
    temperature_layout = clutter_box_layout_new ();
    temperature_box =  clutter_box_new(temperature_layout);

    clutter_actor_set_size(temperature_box, 74, 20);
    label_temp_low = clutter_text_new();
    label_temp_hi = clutter_text_new();
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label_temp_low));
    pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
    clutter_text_set_font_description(CLUTTER_TEXT(label_temp_low), pfd);
    clutter_text_set_font_description(CLUTTER_TEXT(label_temp_hi), pfd);
    if (temp_data){
        if (temp_data->temperature_low().value() != INT_MAX)
            snprintf(buffer, (4096 -1), "%0.f°", temp_data->temperature_low().value());
        else{
            if (temp_data->temperature().value() != INT_MAX)
                snprintf(buffer, (4096 -1), "%0.f°", 
                                      temp_data->temperature().value());
        }    
    }else
        snprintf(buffer, (4096 -1), "");
    clutter_text_set_text((ClutterText*)label_temp_low, buffer);
    clutter_text_set_color((ClutterText*)label_temp_low, clutter_color_new(136, 147, 151, 255));

    if (temp_data){
        if (temp_data->temperature_hi().value() != INT_MAX)
            snprintf(buffer, (4096 -1), "%0.f°",  temp_data->temperature_hi().value());
        
    }else
        snprintf(buffer, (4096 -1), "");
    clutter_text_set_text((ClutterText*)label_temp_hi, buffer);
    clutter_text_set_color((ClutterText*)label_temp_hi, clutter_color_new(48, 185, 226, 255));

    clutter_box_pack((ClutterBox*)temperature_box, label_temp_hi, 
                                 "x-align", CLUTTER_BOX_ALIGNMENT_START,
                                 "y-align", CLUTTER_BOX_ALIGNMENT_END,
                                 "expand", TRUE,
                                  NULL);
    clutter_box_pack((ClutterBox*)temperature_box, label_temp_low,
                                 "x-align", CLUTTER_BOX_ALIGNMENT_END,
                                 "y-align", CLUTTER_BOX_ALIGNMENT_END, 
                                 NULL);

    /* Day */
    label_day = clutter_text_new();
    clutter_text_set_color((ClutterText*)label_day, clutter_color_new(169, 169, 169, 255));
    if (temp_data){
        snprintf(buffer, (4096-1), "%s", temp_data->ShortDayName().c_str());
        clutter_text_set_text((ClutterText*)label_day, buffer);
    }
    layout = clutter_box_layout_new ();
    clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(layout), TRUE);
    clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (layout), 6);
    box =  clutter_box_new(layout);
    clutter_actor_set_size (box, 99.0, 129.0);
    group = clutter_group_new();

    /* stub rectangle */
    rectangle = clutter_rectangle_new();
    clutter_actor_set_size(rectangle, -1, 3);
    clutter_box_pack((ClutterBox*)box, rectangle, NULL);

    clutter_box_pack((ClutterBox*)box, label_day, NULL);
    clutter_box_pack((ClutterBox*)box, icon, NULL);
    clutter_box_pack((ClutterBox*)box, temperature_box,  "x-align", CLUTTER_BOX_ALIGNMENT_CENTER, NULL);

    /* stub rectangle */
    rectangle = clutter_rectangle_new();
    clutter_actor_set_size(rectangle, -1, 3);
    clutter_box_pack((ClutterBox*)box, rectangle, NULL);

    clutter_container_add_actor(CLUTTER_CONTAINER(group), background_active);
    clutter_actor_hide(background_active);    
    clutter_container_add_actor(CLUTTER_CONTAINER(group), background_passive);
    clutter_actor_set_depth (background_passive, 1);
    clutter_actor_set_depth (background_active, 2);
    clutter_container_add_actor(CLUTTER_CONTAINER(group), box);
    clutter_actor_set_depth (box, 5);
    clutter_actor_set_reactive(group, TRUE);

    /* connect the press event on detail button */
    g_signal_connect (group, "button-press-event", G_CALLBACK (detail_event_cb), temp_data);
    return group;
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
  ClutterLayoutManager *vlayout;
  ClutterActor     *vbox;
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
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
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

  /* vertical layout */

  vlayout = clutter_box_layout_new();
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(vlayout), TRUE);
  vbox = clutter_box_new(vlayout);


  label = clutter_text_new();
  about_text = _("Weather forecast for the Meego. Version ");
  about_text += OMWEATHER_VERSION;
  about_text += ".\n";
  about_text += _("Copyright(c) 2006-2011 by Vlad Vasilyeu and  Pavel Fialko\n");
  about_text += _("Copyright(c) 2010-2011 by Tanya Makova\n");
  about_text += _("Author and maintenance: Vlad Vasilyeu, <vlad@gas.by>\n");
  about_text += _("Maintenance: Pavel Fialko, <pavelnf@gmail.com>\nTanya Makova, <tanyshk@gmail.com>\n");
  about_text += _("Design UI and default iconset: Copyright 2006-2011 Andrew Zhilin, <az@pocketpcrussia.com>\n");
  about_text += _("Grzanka's Iconset: Copyright 2005 by Wojciech Grzanka, <wojciech@grzanka.pl>");
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.0);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  
  clutter_text_set_text((ClutterText*)label, about_text.c_str());
  clutter_box_pack((ClutterBox*)vbox, label, NULL);

  label = clutter_text_new();
  about_text = _("Translators: French - Nicolas Granziano, Philippe, Benot Knecht; Russian - Pavel Fialko, Vlad Vasiliev, Ed Bartosh;\n");
  about_text += _("Finnish - Marko Vertainen; German - Claudius Henrichs; Spanish - Alejandro Lopez; Slovak - Roman Moravčík\n");
  about_text += _("Italian - Pavel Fialko, Alessandro Pasotti, Salvatore Fiorenzo, Luca Onnis; Polish - Tomasz Dominikowski;");
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 0.85);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  
  clutter_text_set_text((ClutterText*)label, about_text.c_str());
  clutter_box_pack((ClutterBox*)vbox, label, NULL);

  label = clutter_text_new();
  about_text =  _("This software is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License\n");
  about_text += _("as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.");
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 0.80);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  
  clutter_text_set_text((ClutterText*)label, about_text.c_str());
  clutter_box_pack((ClutterBox*)vbox, label, NULL);

  clutter_box_pack((ClutterBox*)box, vbox, NULL);
  clutter_box_pack((ClutterBox*)bottom_container, box, NULL);

  /* connect the press event on button */
  g_signal_connect (bottom_container, "button-press-event", G_CALLBACK (remove_detail_event_cb), panel);
  clutter_actor_set_reactive(bottom_container, TRUE);

  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), bottom_container,
                          TRUE, TRUE, TRUE, CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_CENTER);


}

//////////////////////////////////////////////////////////////////////////////
ClutterActor*
make_forecast_detail_box(Core::Data *temp_data, int period){
  
  ClutterActor     *icon_group;
  ClutterActor     *box;
  ClutterActor     *hbox;
  ClutterActor     *inthbox;
  ClutterActor     *label;
  ClutterActor     *label_data;
  ClutterActor     *vertical_container;
  ClutterActor     *intvertical_container;
  ClutterActor     *rectangle;
  ClutterLayoutManager *vertical_layout = NULL;
  ClutterLayoutManager *intvertical_layout = NULL;
  ClutterLayoutManager *hbox_layout = NULL;
  ClutterLayoutManager *inthbox_layout = NULL;
  ClutterLayoutManager *layout;
  PangoFontDescription *pfd = NULL;
  std::ostringstream ss;
  char             buffer[4096];
  ClutterScript   *script;
  ClutterTimeline *timeline;
  GError *error = NULL;
  ClutterActor     *icon = NULL;
  int i;
  GList  *list, *l;
  GObject *object;

  /* icon */
  icon_group = clutter_group_new();
  snprintf(buffer, (4096 -1), "%s/buttons_icons/passive.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 133.0, 190.0);
  clutter_container_add_actor(CLUTTER_CONTAINER(icon_group), icon);

  if (temp_data)
      snprintf(buffer, (4096 -1), "%s/icons/%s/%i.%s",config->prefix_path().c_str(),
                                  config->iconSet().c_str(), temp_data->Icon(), "json");
  else
      snprintf(buffer, (4096 -1), "%s/icons/%s/na.%s",config->prefix_path().c_str(), 
                                  config->iconSet().c_str(),"json");  

    script = clutter_script_new();
/*    g_object_unref(oh->script); */
    fprintf(stderr,"JSON SCRIPT: %s\n",buffer);
    clutter_script_load_from_file(script,buffer, &error);
    
/* Temporare disabled animation */
    if (TRUE || error){
        fprintf(stderr,"ERROR in loading clutter script\n");
        g_clear_error (&error);
        if (temp_data)
           snprintf(buffer, (4096 -1), "%s/icons/%s/%i.%s",config->prefix_path().c_str(),
                                          config->iconSet().c_str(), temp_data->Icon(), "png");
        else
           snprintf(buffer, (4096 -1), "%s/icons/%s/na.%s",config->prefix_path().c_str(), 
                                          config->iconSet().c_str(),"png");  
        icon = clutter_texture_new_from_file(buffer, NULL);
        clutter_actor_set_size (icon, 128.0, 128.0);
    }else{
        if (temp_data)
            sprintf(buffer, "icon_name_%i", temp_data->Icon());
        else
            sprintf(buffer, "icon_name_na");
        icon = CLUTTER_ACTOR (clutter_script_get_object (script, buffer));
        fprintf(stderr,"icon %p", icon);
        timeline = CLUTTER_TIMELINE (clutter_script_get_object (script, "main-timeline"));
        
        clutter_actor_set_size (icon, 128.0, 128.0);
        if CLUTTER_IS_GROUP(icon)
           for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(icon)); i++)
               change_actor_size_and_position(clutter_group_get_nth_child(CLUTTER_GROUP(icon),i), 128);
        else
           change_actor_size_and_position(icon, 128);
        list = clutter_script_list_objects(script);
        for (l = list; l != NULL; l = l->next){
           object = (GObject *)l->data;
           if CLUTTER_IS_BEHAVIOUR_PATH(object)
               change_path(clutter_behaviour_path_get_path((ClutterBehaviourPath *)(object)), 128);
        }
        clutter_actor_show (CLUTTER_ACTOR (icon));
        clutter_timeline_start (timeline);
    }

  clutter_container_add_actor(CLUTTER_CONTAINER(icon_group), icon);
  clutter_actor_set_position(icon, 5, 35);
  layout = clutter_box_layout_new ();
  box =  clutter_box_new(layout);
  clutter_box_pack((ClutterBox*)box, icon_group, NULL);
  clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(layout), icon_group,
				CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);

  /* vertical container */
  vertical_layout = clutter_box_layout_new ();
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(vertical_layout), TRUE);
  vertical_container =  clutter_box_new(vertical_layout);
  /* small horizontal container */
  hbox_layout = clutter_box_layout_new ();
  hbox =  clutter_box_new(hbox_layout);

  /* stub rectangle */
  rectangle = clutter_rectangle_new();
  clutter_actor_set_size(rectangle, 10, 30);
  clutter_box_pack((ClutterBox*)hbox, rectangle, NULL);
  /* Day period */
  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
//  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
  pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  switch (period){
      case DAY: 
               clutter_text_set_text((ClutterText*)label, _("Day"));
               clutter_text_set_color((ClutterText*)label, clutter_color_new(49, 194, 238, 255));
               break;
      case NIGHT:
               clutter_text_set_text((ClutterText*)label, _("Night"));
               clutter_text_set_color((ClutterText*)label, clutter_color_new(124, 134, 137, 255));
               break;
      case CURRENT:
               clutter_text_set_text((ClutterText*)label, _("Now"));
               clutter_text_set_color((ClutterText*)label, clutter_color_new(49, 194, 238, 255));
               break;
  }
  clutter_box_pack((ClutterBox*)hbox, label, NULL);
  clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(hbox_layout), label, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);

  clutter_box_pack((ClutterBox*)vertical_container, hbox, NULL);
  clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(vertical_layout), hbox, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);

  /* Long gray horizontal rectangle */
  rectangle = clutter_rectangle_new_with_color(clutter_color_new(211, 211, 211, 255));
  clutter_actor_set_size(rectangle, 390, 1);
  clutter_box_pack((ClutterBox*)vertical_container, rectangle, NULL);


  /* small horizontal container */
  hbox_layout = clutter_box_layout_new ();
  hbox =  clutter_box_new(hbox_layout);

  /* stub rectangle */
  rectangle = clutter_rectangle_new();
  clutter_actor_set_size(rectangle, 10, 140);
  clutter_box_pack((ClutterBox*)hbox, rectangle, NULL);
  /* vertical container */
  intvertical_layout = clutter_box_layout_new ();
  clutter_box_layout_set_spacing(CLUTTER_BOX_LAYOUT(intvertical_layout), 3);
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(intvertical_layout), TRUE);
  intvertical_container =  clutter_box_new(intvertical_layout);
  clutter_actor_set_size(intvertical_container, 380, -1);
  /* stub rectangle */
  rectangle = clutter_rectangle_new();
  clutter_actor_set_size(rectangle, 380, 10);
  clutter_box_pack((ClutterBox*)intvertical_container, rectangle, NULL);

  /* Description of forecast */   
  if (temp_data->Text().compare("N/A") != 0){
    label = clutter_text_new();
    label_data = clutter_text_new();
    clutter_text_set_color((ClutterText*)label, clutter_color_new(128, 128, 128, 255));
    clutter_text_set_color((ClutterText*)label_data, clutter_color_new(128, 128, 128, 255));
 //   pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
 //   pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
 //   clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    clutter_text_set_text((ClutterText*)label,
                      (const gchar*)g_hash_table_lookup(translate_hash,
                                    (void*) temp_data->Text().c_str()));
    clutter_box_pack((ClutterBox*)intvertical_container, label, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(intvertical_layout), label, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }
  /* Temperature */ 
  if (temp_data->temperature_hi().value() != INT_MAX || temp_data->temperature().value() != INT_MAX ||
      temp_data->temperature_low().value() != INT_MAX ){
    /* small horizontal container */
    inthbox_layout = clutter_box_layout_new ();
    inthbox =  clutter_box_new(inthbox_layout);
    label = clutter_text_new();
    label_data = clutter_text_new();
    clutter_text_set_color((ClutterText*)label, clutter_color_new(128, 128, 128, 255));
    clutter_text_set_color((ClutterText*)label_data, clutter_color_new(128, 128, 128, 255));
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    //pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.1);
    pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Temperature:");
    ss << " ";
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label, NULL);
    ss.str("");
    if (temp_data->temperature().value() != INT_MAX)  
	    ss << (int)temp_data->temperature().value() << "°" << config->TemperatureUnit();
    else{
        if (temp_data->temperature_low().value() != INT_MAX || 
            temp_data->temperature_hi().value() != INT_MAX){
           if (temp_data->temperature_low().value() != INT_MAX){
               ss << (int)temp_data->temperature_low().value() << "°" << config->TemperatureUnit();
           }
           ss << " .. "; 
           ss << (int)temp_data->temperature_hi().value() << "°" << config->TemperatureUnit();
        }
    }
    clutter_text_set_text((ClutterText*)label_data, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label_data, NULL);
    clutter_box_pack((ClutterBox*)intvertical_container, inthbox, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(intvertical_layout), inthbox, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }
  /* Pressure */ 
  if (temp_data->Pressure() != INT_MAX){
    /* small horizontal container */
    inthbox_layout = clutter_box_layout_new ();
    inthbox =  clutter_box_new(inthbox_layout);
    label = clutter_text_new();
    label_data = clutter_text_new();
    clutter_text_set_color((ClutterText*)label, clutter_color_new(128, 128, 128, 255));
    clutter_text_set_color((ClutterText*)label_data, clutter_color_new(128, 128, 128, 255));
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
//    pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Pressure:") <<" ";
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label, NULL);
    ss.str("");
    ss << temp_data->Pressure();
    clutter_text_set_text((ClutterText*)label_data, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label_data, NULL);
    clutter_box_pack((ClutterBox*)intvertical_container, inthbox, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(intvertical_layout), inthbox, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }
  /* Humidity */
  if (temp_data->Humidity() != INT_MAX){
    /* small horizontal container */
    inthbox_layout = clutter_box_layout_new ();
    inthbox =  clutter_box_new(inthbox_layout);
    label = clutter_text_new();
    label_data = clutter_text_new();
    clutter_text_set_color((ClutterText*)label, clutter_color_new(128, 128, 128, 255));
    clutter_text_set_color((ClutterText*)label_data, clutter_color_new(128, 128, 128, 255));
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    //pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Humidity:") <<" ";
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    ss.str("");
    ss<< temp_data->Humidity() << "%";
    clutter_text_set_text((ClutterText*)label_data, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label, NULL);
    clutter_box_pack((ClutterBox*)inthbox, label_data, NULL);
    clutter_box_pack((ClutterBox*)intvertical_container, inthbox, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(intvertical_layout), inthbox, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }

  if (temp_data->WindDirection().compare("N/A") != 0){
    /* small horizontal container */
    inthbox_layout = clutter_box_layout_new ();
    inthbox =  clutter_box_new(inthbox_layout);
    label = clutter_text_new();
    label_data = clutter_text_new();
    clutter_text_set_color((ClutterText*)label, clutter_color_new(128, 128, 128, 255));
    clutter_text_set_color((ClutterText*)label_data, clutter_color_new(128, 128, 128, 255));
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
    //pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Wind:") << " ";
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label, NULL);
    ss.str("");
    ss << _(temp_data->WindDirection().c_str());
    clutter_text_set_text((ClutterText*)label_data, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label_data, NULL);
    clutter_box_pack((ClutterBox*)intvertical_container, inthbox, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(intvertical_layout), inthbox, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }

  if (temp_data->WindSpeed().value() != INT_MAX){
    /* small horizontal container */
    inthbox_layout = clutter_box_layout_new ();
    inthbox =  clutter_box_new(inthbox_layout);
    label = clutter_text_new();
    label_data = clutter_text_new();
    clutter_text_set_color((ClutterText*)label, clutter_color_new(128, 128, 128, 255));
    clutter_text_set_color((ClutterText*)label_data, clutter_color_new(128, 128, 128, 255));
    pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
    pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
    //pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.2);
    clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
    ss.str("");
    ss << _("Speed:") <<" ";
    clutter_text_set_text((ClutterText*)label, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label, NULL);
    ss.str("");
    temp_data->WindSpeed().units(config->WindSpeedUnit());
    ss <<temp_data->WindSpeed().value() << config->WindSpeedUnit();
    clutter_text_set_text((ClutterText*)label_data, ss.str().c_str());
    clutter_box_pack((ClutterBox*)inthbox, label_data, NULL);
    clutter_box_pack((ClutterBox*)intvertical_container, inthbox, NULL);
    clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(intvertical_layout), inthbox, 
			    CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  }

  clutter_box_pack((ClutterBox*)hbox, intvertical_container, 
                    "x-align", CLUTTER_BOX_ALIGNMENT_START,
                    "y-align", CLUTTER_BOX_ALIGNMENT_START, 
                    "expand", TRUE,
                    NULL);
  clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(hbox_layout), intvertical_container,
				CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);

  clutter_box_pack((ClutterBox*)vertical_container, hbox,
                    "y-align", CLUTTER_BOX_ALIGNMENT_CENTER, 
                    "expand", TRUE,
                      NULL);
  clutter_box_pack((ClutterBox*)box, vertical_container, NULL);
  clutter_box_layout_set_alignment(CLUTTER_BOX_LAYOUT(layout), vertical_container,
				CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_CENTER);

  return box;
} 
//////////////////////////////////////////////////////////////////////////////
void
make_bottom_content(Core::Data *temp_data) {
  
  ClutterLayoutManager *bottom_layout;
  ClutterActor     *label, *label1;
  ClutterActor     *vertical_container;
  ClutterLayoutManager *hbox_layout = NULL;
  ClutterLayoutManager *provider_layout = NULL;
  ClutterLayoutManager *dayname_layout = NULL;
  ClutterActor     *hbox;
  ClutterActor     *box;
  ClutterActor     *daynamebox;
  ClutterActor     *providerbox;
  ClutterActor     *rectangle;
  std::string      day_name;
  PangoFontDescription *pfd = NULL;
  std::ostringstream ss, ss1;
  int i;
  
  if (mpl_panel_client_get_height_request (panel) > PANEL_HEIGHT){
      if (bottom_container)
          clutter_actor_destroy(bottom_container);
  }
  /* To do Display message */
  if (!temp_data)
      return;

  /* bottom layout */
  
  bottom_layout = clutter_box_layout_new();
  clutter_box_layout_set_spacing(CLUTTER_BOX_LAYOUT(bottom_layout), 0);
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(bottom_layout), TRUE);
  bottom_container = clutter_box_new(bottom_layout);
 

  /* Day name */
  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  //pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.3);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  if (temp_data)
      if (temp_data->Current())
          day_name = _("Now");
      else
          day_name = temp_data->FullDayName() + " " + temp_data->DayOfMonthName() +", " + temp_data->FullMonthName(); 
  
  clutter_text_set_text((ClutterText*)label, day_name.c_str());
  clutter_text_set_color((ClutterText*)label, clutter_color_new(102, 102, 102, 255));

  dayname_layout = clutter_box_layout_new();
  daynamebox = clutter_box_new(dayname_layout);

  clutter_actor_set_size (daynamebox, -1, 50.0);
  clutter_box_pack((ClutterBox*)daynamebox, label,
                    "x-align", CLUTTER_BOX_ALIGNMENT_CENTER,
                    "y-align", CLUTTER_BOX_ALIGNMENT_CENTER, 
                    "expand", TRUE,
                    NULL);
  clutter_box_pack((ClutterBox*)bottom_container, daynamebox, NULL);

  /* Small horizontal rectangle */
  rectangle = clutter_rectangle_new_with_color(clutter_color_new(43, 43, 43, 255));
  clutter_actor_set_size(rectangle, 1024-10,2);
  clutter_box_pack((ClutterBox*)bottom_container, rectangle, NULL);

  /* main box */
  hbox_layout = clutter_box_layout_new();
  hbox = clutter_box_new(hbox_layout);
  clutter_actor_set_size(hbox, 1024 - 10, -1);

  /* Added day or current weather foreacast */
  if (!temp_data->Current())
      box   =  make_forecast_detail_box(temp_data, DAY);
  else 
      box   =  make_forecast_detail_box(temp_data, CURRENT);
  clutter_actor_set_size(box, 511, -1);
  clutter_box_pack((ClutterBox*)hbox, box,
                   "x-align", CLUTTER_BOX_ALIGNMENT_START,
                   "expand", TRUE, NULL);

  /* added night weather forecast */ 
  if (!temp_data->Current()){
      if (temp_data->StartTime() - temp_data->EndTime() > 8*3600)
          temp_data = dp->data().GetDataForTime(temp_data->EndTime() + 3600);
      else
          temp_data = dp->data().GetDataForTime(temp_data->StartTime() + 13*3600);
      if (temp_data){
          if (temp_data->temperature_hi().value() != INT_MAX)
            temp_data->temperature_hi().units(config->TemperatureUnit());
          if (temp_data->temperature_low().value() != INT_MAX)
            temp_data->temperature_low().units(config->TemperatureUnit());
          if (temp_data->temperature().value() != INT_MAX)
            temp_data->temperature().units(config->TemperatureUnit());

          box =  make_forecast_detail_box(temp_data, NIGHT);
          clutter_actor_set_size(box, 511, -1);
          clutter_box_pack((ClutterBox*)hbox, box,
                           "x-align", CLUTTER_BOX_ALIGNMENT_END,
                           "expand", TRUE, NULL);
      }
  }

  clutter_box_pack((ClutterBox*)bottom_container, hbox, NULL);

  /* Small horizontal rectangle */
  rectangle = clutter_rectangle_new_with_color(clutter_color_new(43, 43, 43, 255));
  clutter_actor_set_size(rectangle, 1024-10,2);
  clutter_box_pack((ClutterBox*)bottom_container, rectangle, NULL);
  /* Provider name */

  provider_layout = clutter_box_layout_new();
  providerbox = clutter_box_new(provider_layout);

  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  //pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.4);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  ss.str("");
  ss << _("Provided by");
  ss << " ";
  clutter_text_set_text((ClutterText*)label, ss.str().c_str());
  clutter_text_set_color((ClutterText*)label, clutter_color_new(102, 102, 102, 255));
  clutter_actor_set_size (providerbox, -1, 40.0);
  clutter_box_pack((ClutterBox*)providerbox, label,
                    "x-align", CLUTTER_BOX_ALIGNMENT_CENTER,
                    "y-align", CLUTTER_BOX_ALIGNMENT_CENTER,
                    "expand", TRUE,
                    NULL);

  //clutter_box_pack((ClutterBox*)providerbox, label, NULL);

  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  //pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.3);
  pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  ss.str("");
  if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0
                                              &&  config->stationsList().at(config->current_station_id()))
     ss <<  config->stationsList().at(config->current_station_id())->sourceName();

  clutter_text_set_text((ClutterText*)label, ss.str().c_str());
  clutter_text_set_color((ClutterText*)label, clutter_color_new(49, 194, 238, 255));
  //clutter_box_pack((ClutterBox*)providerbox, label, NULL);
  clutter_box_pack((ClutterBox*)providerbox, label,
                    "x-align", CLUTTER_BOX_ALIGNMENT_CENTER,
                    "y-align", CLUTTER_BOX_ALIGNMENT_CENTER,
                    "expand", TRUE,
                    NULL);

  clutter_box_pack((ClutterBox*)bottom_container, providerbox, NULL);
  
  /* connect the press event on button */
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
  ClutterActor     *rectangle;
  ClutterActor     *refresh_group;
  char             buffer[4096];
  int i, period;
  Core::Data *temp_data = NULL;
  Core::Data *temp_data_day = NULL;
  PangoFontDescription *pfd = NULL;
  time_t current_day;
  struct tm   *tm = NULL;
  int year, current_month;

  if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0 
                                              &&  config->stationsList().at(config->current_station_id()))
      dp = current_data(config->stationsList().at(config->current_station_id())->fileName());

  main_vertical_layout = clutter_box_layout_new ();
  if (panel_container){
      clutter_actor_destroy(panel_container);
      bottom_container = NULL;
      active_background = NULL;
  }
  if (refresh_timeline)
       g_object_unref(refresh_timeline);
  panel_container =  clutter_box_new(main_vertical_layout);
  clutter_actor_set_width(panel_container , 1024 - 10);
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(main_vertical_layout), TRUE);

  forecast_layout = clutter_box_layout_new(); 
  forecast_horizontal_container = clutter_box_new(forecast_layout);
  clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (forecast_layout), 0);
  clutter_actor_set_width(forecast_horizontal_container , 1024 - 10);

  /* top layout */
  top_layout = clutter_box_layout_new(); 
  top_container = clutter_box_new(top_layout);
  clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (top_layout), 3);

  /* stub rectangle */
  rectangle = clutter_rectangle_new();
  clutter_actor_set_size(rectangle, 5, 44);
  clutter_box_pack((ClutterBox*)top_container, rectangle, NULL);

  /* refresh button */
  refresh_group = clutter_group_new();
  snprintf(buffer, (4096 -1), "%s/buttons_icons/update_background.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 44.0, 44.0);
  clutter_container_add_actor(CLUTTER_CONTAINER(refresh_group), icon);
  snprintf(buffer, (4096 -1), "%s/buttons_icons/update_arrows.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_container_add_actor(CLUTTER_CONTAINER(refresh_group), icon);
  clutter_actor_set_reactive(refresh_group, TRUE);
  refresh_timeline = create_update_animation(icon);
  /* connect the press event on refresh button */
  g_signal_connect (refresh_group, "button-press-event", G_CALLBACK (refresh_button_event_cb), panel);
  clutter_box_pack((ClutterBox*)top_container, refresh_group, "x-align", CLUTTER_BOX_ALIGNMENT_START, "x-fill", TRUE, NULL);
 
  /* stub rectangle */
  rectangle = clutter_rectangle_new();
  clutter_actor_set_size(rectangle, 6, 64);
  clutter_box_pack((ClutterBox*)top_container, rectangle, NULL);

  /* station name */
  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 1.8);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  clutter_text_set_color(CLUTTER_TEXT(label), clutter_color_new(102, 102, 102, 255));
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
  clutter_actor_set_size (icon, 44.0, 44.0);
  clutter_box_pack((ClutterBox*)top_container, icon, "expand", TRUE,  "x-fill", TRUE, NULL);

  /* about button */
  snprintf(buffer, (4096 -1), "%s/buttons_icons/about.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 44.0, 44.0);
  clutter_actor_set_reactive(icon, TRUE);
  /* connect the press event on about button */
  g_signal_connect (icon, "button-press-event", G_CALLBACK (about_button_event_cb), panel);
  clutter_box_pack((ClutterBox*)top_container, icon, "x-align", CLUTTER_BOX_ALIGNMENT_END, "x-fill", TRUE, NULL);

  /* config button */
  snprintf(buffer, (4096 -1), "%s/buttons_icons/config.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 44.0, 44.0);
  clutter_actor_set_reactive(icon, TRUE);
  /* connect the press event on config button */
  g_signal_connect (icon, "button-press-event", G_CALLBACK (config_button_event_cb), NULL);
  clutter_box_pack((ClutterBox*)top_container, icon, "x-align", CLUTTER_BOX_ALIGNMENT_END, "x-fill", TRUE, NULL);

  /* stub rectangle */
  rectangle = clutter_rectangle_new();
  clutter_actor_set_size(rectangle, 5, 5);
  clutter_box_pack((ClutterBox*)top_container, rectangle, NULL);

  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), top_container,
                          TRUE, TRUE, TRUE, CLUTTER_BOX_ALIGNMENT_CENTER, CLUTTER_BOX_ALIGNMENT_CENTER);

  /* Change panel icon */
  if (dp)
      temp_data = dp->data().GetDataForTime(time(NULL));
  if (temp_data ){
      snprintf(buffer, (4096 -1), "icon%s%i", config->iconSet().c_str(), temp_data->Icon());
      mpl_panel_client_request_button_style (MPL_PANEL_CLIENT(panel), buffer);
  }else
      mpl_panel_client_request_button_style (MPL_PANEL_CLIENT(panel), "iconna");
  
  /* set current day */ 
  current_day = time(NULL);
  tm = localtime(&current_day);
  year = 1900 + tm->tm_year;
  current_month = tm->tm_mon;
  tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
  tm->tm_isdst = 1;
  current_day = mktime(tm);

  /* day buttons */
  period = 0;
  snprintf(buffer, (4096 -1), "%s/buttons_icons/border.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_box_pack((ClutterBox*)forecast_horizontal_container, icon, NULL);
  for (i = 0; i < 10; i++){
      if (dp){
          if (i==0)
            temp_data_day = dp->data().GetDataForTime(time(NULL)); /* weather forecast for current time */
          else
            temp_data_day = dp->data().GetDataForTime( current_day + 12*3600 + period);
          if (temp_data_day->temperature_hi().value() != INT_MAX)
            temp_data_day->temperature_hi().units(config->TemperatureUnit());
          if (temp_data_day->temperature_low().value() != INT_MAX)
            temp_data_day->temperature_low().units(config->TemperatureUnit());
          if (temp_data_day->temperature().value() != INT_MAX)
            temp_data_day->temperature().units(config->TemperatureUnit());

      }else
          temp_data_day = NULL;
      period = period + 3600*24;
    
      box = make_day_actor(temp_data_day);
      clutter_box_pack((ClutterBox*)forecast_horizontal_container, box, NULL);
      if (i < 9) {
          snprintf(buffer, (4096 -1), "%s/buttons_icons/divider.png",config->prefix_path().c_str());
          icon = clutter_texture_new_from_file(buffer, NULL);
          clutter_box_pack((ClutterBox*)forecast_horizontal_container, icon, NULL);
      }
  }
  snprintf(buffer, (4096 -1), "%s/buttons_icons/border.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_box_pack((ClutterBox*)forecast_horizontal_container, icon, NULL);

  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), forecast_horizontal_container, 
                          FALSE, FALSE, FALSE, CLUTTER_BOX_ALIGNMENT_START, CLUTTER_BOX_ALIGNMENT_START);
  clutter_actor_show (panel_container);
  mpl_panel_clutter_set_child (panel, panel_container);
}

DBusHandlerResult
get_omweather_signal_cb(DBusConnection *conn, DBusMessage *msg, gpointer data){
    if (dbus_message_is_signal(msg, "org.meego.omweather", "reload_config")){
        delete config;
        config = create_and_fill_config();
        if (timer_update) {
            g_source_remove(timer_update);
            timer_update = 0;
        }
        if (config->UpdatePeriod() != INT_MAX)
            timer_update = g_timeout_add(config->UpdatePeriod()*1000, g_auto_update, NULL);
        make_window_content(MPL_PANEL_CLUTTER (panel));
    }
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}
DBusHandlerResult
get_connman_signal_cb(DBusConnection *conn, DBusMessage *msg, gpointer data){

    if (dbus_message_is_signal(msg, "org.moblin.connman.Manager", "StateChanged")){
        char *arg = NULL;
        dbus_message_get_args(msg, NULL, DBUS_TYPE_STRING, &arg);
        if (arg && strcmp(arg, "online") == 0){
            if (config->UpdateConnect())
                refresh_button_event_cb(NULL, NULL, NULL);
            connected = true;
        }
        else
            connected = false;
    }
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}


void
dbus_init(void){
  DBusError   error;
  DBusConnection       *dbus_conn;
  DBusConnection       *dbus_conn_session;

  DBusGConnection *connection;
  DBusGProxy      *manager;

  GError                     *error1 = NULL;

  dbus_error_init (&error);
  dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
  if (dbus_error_is_set(&error)){
      dbus_error_free(&error);
  }
  dbus_conn_session = dbus_bus_get(DBUS_BUS_SESSION, &error);
  if (dbus_error_is_set(&error)){
      dbus_error_free(&error);
  }
  /* Hack */
  connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error1);
  if (error1)
    {
      g_debug ("Error connecting to bus: %s",
               error1->message);
      g_clear_error (&error1);
    }

  if (dbus_conn){
      //dbus_connection_setup_with_g_main(dbus_conn, NULL);
      dbus_bus_add_match(dbus_conn,
                         "type='signal',interface='org.moblin.connman.Manager'", &error);
      if (dbus_error_is_set(&error)){


           fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
           dbus_error_free(&error);
      }
      // add the callback
      dbus_connection_add_filter(dbus_conn,
                                 get_connman_signal_cb,
                                 NULL, NULL);

  }


  if (dbus_conn_session){
      dbus_bus_add_match(dbus_conn_session, "type='signal', interface='org.meego.omweather'", &error);
      if (dbus_error_is_set(&error)){
           fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
           dbus_error_free(&error);
      }
      // add the callback
      dbus_connection_add_filter(dbus_conn_session,
                                 get_omweather_signal_cb,
                                 NULL, NULL);
  }


}
int
main (int argc, char *argv[])
{
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
      snprintf(buffer, (4096 -1), "icon%s%i", config->iconSet().c_str(), temp_data->Icon());
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
  dbus_init();
  if (config->UpdatePeriod() != INT_MAX){
      timer_update = g_timeout_add(config->UpdatePeriod()*1000, g_auto_update, NULL);
      //  timer_update = g_timeout_add(5000, g_auto_update, NULL);
  }
  clutter_threads_enter();
  clutter_main ();
  clutter_threads_leave();

  return 0;
}
