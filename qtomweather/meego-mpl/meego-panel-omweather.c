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

#include <meego-panel/mpl-panel-clutter.h>
#include <meego-panel/mpl-panel-common.h>

static void
make_window_content (MplPanelClutter *panel)
{
  ClutterActor     *stage = mpl_panel_clutter_get_stage (panel);
  ClutterActor     *label;
  ClutterColor      black = {0, 0, 0, 0xff};
  ClutterColor      red =   {0xff, 0, 0, 0xff};

#if 1
  label = clutter_text_new_with_text ("Sans 16pt", "This is a clutter panel");
  clutter_text_set_color  (CLUTTER_TEXT (label), &black);
  clutter_text_set_editable (CLUTTER_TEXT (label), TRUE);

  clutter_stage_set_key_focus (CLUTTER_STAGE (stage), label);
#else
//  label = clutter_rectangle_new_with_color (&black);
  label = clutter_rectangle_new_with_color (&red);
  clutter_actor_set_size (label, 50.0,50.0);
  clutter_actor_show (label);
#endif

  mpl_panel_clutter_set_child (panel, label);
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
  make_window_content (MPL_PANEL_CLUTTER (panel));
  file = fopen("/tmp/1.log","wb");
  fclose(file);

  clutter_main ();

  return 0;
}
