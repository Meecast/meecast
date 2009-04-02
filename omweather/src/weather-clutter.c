/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
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
#ifdef CLUTTER
/*******************************************************************************/
#include "weather-clutter.h"
/*******************************************************************************/

/* Timeline handler */
void
frame_cb (ClutterTimeline *timeline, 
	  gint             frame_num, 
	  gpointer         data)
{
  SuperOH        *oh = (SuperOH *)data;
  /* Rotate everything clockwise about stage center*/
  clutter_actor_set_rotation (CLUTTER_ACTOR (oh->group),
                              CLUTTER_Z_AXIS,
                              frame_num,
                              100 / 2, 100 / 2, 0);


}
void show_animation(void)
{
    static GSList *list_temp = NULL;
    SuperOH *oh;
    ClutterTimeline *timeline;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!app->clutter_objects_list)
        return;
    list_temp = app->clutter_objects_list;
    while (list_temp != NULL) {
        oh = list_temp->data;
        
        /* Create a timeline to manage animation */
        /* FIX ME Important need to free */
        timeline = clutter_timeline_new (360, 60); /* num frames, fps */
        g_object_set(timeline, "loop", TRUE, NULL);   /* have it loop */
        /* fire a callback for frame change */
        g_signal_connect(timeline, "new-frame",  G_CALLBACK (frame_cb), oh);
        /* and start it */
        clutter_timeline_start (timeline);

        list_temp = g_slist_next(list_temp);
    }
}
#endif