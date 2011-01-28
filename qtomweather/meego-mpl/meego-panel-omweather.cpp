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

#include "core.h"
#include <meego-panel/mpl-panel-clutter.h>
#include <meego-panel/mpl-panel-common.h>
#include <mx/mx.h>


void init_omweather_core(void);


//////////////////////////////////////////////////////////////////////////////


static void
make_window_content (MplPanelClutter *panel)
{
  ClutterActor     *stage = mpl_panel_clutter_get_stage (panel);
  ClutterLayoutManager *layout;
  ClutterActor     *label;
  ClutterActor     *icon;
  ClutterActor     *container;
  ClutterActor     *box;
  ClutterColor      black = {0, 0, 0, 0xff};
  ClutterColor      red =   {0xff, 0, 0, 0xff};
  char             buffer[4096];
  int i;

  layout = clutter_box_layout_new (); 
  container =  clutter_box_new(layout);
  clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (layout), 12);
  i = 1;
  for (i = 0; i<10; i++){
      snprintf(buffer, (4096 -1), "%s/%i.png","/usr/share/meego-panel-omweather/theme/icons/Glance",i);
      icon = clutter_texture_new_from_file(buffer, NULL);
      clutter_actor_set_size (icon, 80.0, 80.0);
      clutter_actor_show (icon);
      label = clutter_text_new();
      snprintf(buffer, (4096 -1), "Th\n%i°C\n%i°C", i-5, i+7);
      clutter_text_set_text((ClutterText*)label, buffer);
      layout = clutter_box_layout_new ();
      box =  clutter_box_new(layout);
      clutter_box_pack((ClutterBox*)box, icon, NULL);
      clutter_box_pack((ClutterBox*)box, label, NULL);
      clutter_box_pack((ClutterBox*)container, box, NULL);
  }

  clutter_actor_show (container);
  mpl_panel_clutter_set_child (panel, container);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &red);
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
  mpl_panel_client_set_height_request (panel, 100);
  init_omweather_core();
  make_window_content (MPL_PANEL_CLUTTER (panel));
  file = fopen("/tmp/1.log","wb");
  fclose(file);

  clutter_main ();

  return 0;
}
