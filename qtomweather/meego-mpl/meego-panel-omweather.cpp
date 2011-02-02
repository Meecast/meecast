/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2006-2011 Pavel Fialko
 * Copyright (C) 2010-2011 Tanya Makova
 * 	for the code
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

Core::Config *config;
Core::StationsList stationslist;


void init_omweather_core(void);
Core::DataParser *current_data(std::string& str);
int update_weather_forecast(Core::Config *config);

//////////////////////////////////////////////////////////////////////////////
gboolean
refresh_button_event_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   gpointer      user_data){
    update_weather_forecast(config);
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
    clutter_actor_show (icon);
    label = clutter_text_new();
    if (temp_data){
        if (temp_data->temperature_low().value() != INT_MAX)
            snprintf(buffer, (4096 -1), "%s\n%0.f°C\n%0.f°C", temp_data->ShortDayName().c_str(), temp_data->temperature_low().value(), temp_data->temperature_hi().value());
        else
            snprintf(buffer, (4096 -1), "%s\n%0.f°C", temp_data->ShortDayName().c_str(), temp_data->temperature_hi().value());
    }else
        snprintf(buffer, (4096 -1), "N/A°C\nN/A°C");

    clutter_text_set_text((ClutterText*)label, buffer);
    layout = clutter_box_layout_new ();
    box =  clutter_box_new(layout);
    clutter_box_pack((ClutterBox*)box, icon, NULL);
    clutter_box_pack((ClutterBox*)box, label, NULL);

    return box;
}
//////////////////////////////////////////////////////////////////////////////
static void
make_window_content (MplPanelClutter *panel)
{
  ClutterActor     *stage = mpl_panel_clutter_get_stage (panel);
  ClutterLayoutManager *forecast_layout;
  ClutterLayoutManager *top_layout;
  ClutterLayoutManager *main_vertical_layout;
  ClutterActor     *container;
  ClutterActor     *forecast_horizontal_container;
  ClutterActor     *top_container;
  ClutterActor     *box;
  ClutterActor     *label;
  ClutterActor     *icon;
  ClutterColor      black = {0, 0, 0, 0xff};
  ClutterColor      red =   {0xff, 0, 0, 0xff};
  char             buffer[4096];
  int i, period;
  Core::Data *temp_data = NULL;
  Core::DataParser* dp;
  PangoFontDescription *pfd = NULL;


  std::cerr<<"ggggggggggg   "<<config->stationsList().at(1)->fileName();
  dp = current_data(config->stationsList().at(1)->fileName());

  main_vertical_layout = clutter_box_layout_new (); 
  container =  clutter_box_new(main_vertical_layout);
  clutter_box_layout_set_vertical(CLUTTER_BOX_LAYOUT(main_vertical_layout), TRUE);

  forecast_layout = clutter_box_layout_new(); 
  forecast_horizontal_container = clutter_box_new(forecast_layout);
  clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (forecast_layout), 12);

  /* top layout */
  top_layout = clutter_box_layout_new(); 
  top_container = clutter_box_new(top_layout);

  /* station name */
  label = clutter_text_new();
  pfd = clutter_text_get_font_description(CLUTTER_TEXT(label));
  pango_font_description_set_size(pfd, pango_font_description_get_size(pfd) * 3);
  clutter_text_set_font_description(CLUTTER_TEXT(label), pfd);
  stationslist = config->stationsList();
  clutter_text_set_text((ClutterText*)label, stationslist.station_by_id("BOXX0014")->name().c_str());
  clutter_box_pack((ClutterBox*)top_container, label, NULL);

  /* null button */
  snprintf(buffer, (4096 -1), "%s/buttons_icons/null.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 80.0, 80.0);
  clutter_box_pack((ClutterBox*)top_container, icon, "expand", TRUE,  "x-fill", TRUE, NULL);

  /* refresh button */
  snprintf(buffer, (4096 -1), "%s/buttons_icons/refresh.png",config->prefix_path().c_str());
  icon = clutter_texture_new_from_file(buffer, NULL);
  clutter_actor_set_size (icon, 80.0, 80.0);
  clutter_actor_set_reactive(icon, TRUE);
  /* connect the press event on refresh button */
  g_signal_connect (icon, "button-press-event", G_CALLBACK (refresh_button_event_cb), NULL);
  clutter_box_pack((ClutterBox*)top_container, icon, "x-align", CLUTTER_BOX_ALIGNMENT_END, "x-fill", TRUE, NULL);

  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), top_container,
                          TRUE, TRUE, TRUE, CLUTTER_BOX_ALIGNMENT_CENTER, CLUTTER_BOX_ALIGNMENT_CENTER);

  clutter_actor_show (label);
  period = 0;
  for (i = 0; i < 8; i++){
      temp_data = dp->data().GetDataForTime(time(NULL) + period);
      period = period + 3600*24;
      box = make_day_actor(temp_data); 
      clutter_box_pack((ClutterBox*)forecast_horizontal_container, box, NULL);
  }
  clutter_box_layout_pack(CLUTTER_BOX_LAYOUT(main_vertical_layout), forecast_horizontal_container, 
                          FALSE, TRUE, TRUE, CLUTTER_BOX_ALIGNMENT_CENTER, CLUTTER_BOX_ALIGNMENT_START);
  clutter_actor_show (container);
  mpl_panel_clutter_set_child (panel, container);
}

int
main (int argc, char *argv[])
{
  MplPanelClient *panel;
  FILE *file;

  clutter_init (&argc, &argv);

  mx_style_load_from_file (mx_style_get_default (),
                          "/usr/share/meego-panel-omweather/theme/omweather-panel.css", NULL);

  panel = mpl_panel_clutter_new ("omweather",           /* the panel slot */
                                 "omweather",                   /* tooltip */
                                 "/usr/share/meego-panel-omweather/theme/omweather-panel.css", /*stylesheet */
                                "icon1",                 /* button style */
                                 TRUE);
  mpl_panel_client_set_height_request (panel, 150);
  init_omweather_core();
  //update_weather_forecast(config);
  make_window_content (MPL_PANEL_CLUTTER (panel));
  file = fopen("/tmp/1.log","wb");
  fclose(file);

  clutter_main ();

  return 0;
}
