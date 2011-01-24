/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/*
 * Copyright (c) 2009 Intel Corp.
 *
 * Author: Tomas Frydrych <tf@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

/*
 * Simple test for out of process panels.
 *
 */

#include <meego-panel/mpl-panel-clutter.h>
#include <meego-panel/mpl-panel-common.h>

static void
make_window_content (MplPanelClutter *panel)
{
  ClutterActor     *stage = mpl_panel_clutter_get_stage (panel);
  ClutterActor     *label;
  ClutterColor      black = {0, 0, 0, 0xff};
  ClutterColor      red =   {0xff, 0, 0, 0xff};

#if 0
  label = clutter_text_new_with_text ("Sans 16pt", "This is a clutter panel");
  clutter_text_set_color  (CLUTTER_TEXT (label), &black);
  clutter_text_set_editable (CLUTTER_TEXT (label), TRUE);

  clutter_stage_set_key_focus (CLUTTER_STAGE (stage), label);
#else
//  label = clutter_rectangle_new_with_color (&black);
  label = clutter_rectangle_new_with_color (&red);
  clutter_actor_set_size (label, 50.0,50.0);
  clutter_actor_show (label);     
  g_message (G_STRLOC ": Previously failed panel  appeared");
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
  file = fopen("/tmp/1.log","wb");
  fclose(file);

  mx_style_load_from_file (mx_style_get_default (),
                          "/usr/share/meego-panel-omweather/theme/test-panel.css", NULL);

  /*
   * NB: the toolbar service indicates whether this panel requires access
   *     to the API provided by org.meego.Mpl.Toolbar -- if you need to do
   *     any application launching, etc., then pass TRUE.
   */
  panel = mpl_panel_clutter_new ("omweather",           /* the panel slot */
//  panel = mpl_panel_clutter_new ("datetime",           /* the panel slot */
                                 "omweather",                   /* tooltip */
                                 "/usr/share/meego-panel-omweather/theme/test-panel.css", /*stylesheet */
//                                 NULL, /*stylesheet */
                                "icon",                 /* button style */
//                                 "applications-button1",
                                 TRUE);
                    /* no toolbar service*/
  g_message (G_STRLOC ": Previously failed panel  appeared11111111111111");
  /*
   * Strictly speaking, it is not necessary to construct the window contents
   * at this point, the panel can instead hook to MplPanelClient::set-size
   * signal. However, once the set-size signal has been emitted, the panel
   * window must remain in a state suitable to it being shown.
   *
   * The panel can also hook into the MplPanelClient::show-begin signal, to be
   * when it is being shown, but this signal is assynchronous, so that the
   * panel might finish showing *before* the Panel handles this signal.
   */
  make_window_content (MPL_PANEL_CLUTTER (panel));

  clutter_main ();

  return 0;
}
