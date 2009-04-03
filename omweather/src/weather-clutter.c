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
#include "weather-clutter.h"
#ifdef CLUTTER
/*******************************************************************************/
void
show_animation(void){
    static GSList	*list_temp = NULL;
    SuperOH		*oh;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!app->clutter_objects_list)
        return;
    list_temp = app->clutter_objects_list;
    while(list_temp != NULL){
        oh = list_temp->data;
        clutter_timeline_start (oh->timeline);
        list_temp = g_slist_next(list_temp);
    }
}
/*******************************************************************************/
GtkWidget *
create_clutter_main_icon(GdkPixbuf *icon_buffer, int icon_size)
{
    GtkWidget *icon_widget;
    SuperOH *oh;
    GtkWidget *clutter;
    ClutterColor stage_color;
    GError *error = NULL;
    gchar  buffer[1024];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
    stage_color.red = app->config->background_color.red;
    stage_color.blue = app->config->background_color.blue;
    stage_color.green = app->config->background_color.green;
    stage_color.alpha = 0xff;

    oh = g_new(SuperOH, 1);
    /* Download script */
    oh->script = clutter_script_new();
//    g_object_unref(oh->script);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%sclutter.json", path_large_icon);
    fprintf (stderr,"%s\n",buffer);
    clutter_script_load_from_file(oh->script,buffer, &error);
    /* Fix Me Need free memory */
    if (error){
        g_free (oh);
        fprintf(stderr,"ERROR in loading clutter script\n");
        g_clear_error (&error);
        return NULL;
    }

    icon_widget = gtk_vbox_new(FALSE, 0);
    clutter = gtk_clutter_embed_new();
    gtk_widget_set_size_request (clutter, icon_size, icon_size);
    gtk_container_add (GTK_CONTAINER (icon_widget), clutter);
    oh->stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter));
    /* and its background color */
    clutter_stage_set_color (CLUTTER_STAGE (oh->stage),
                  &stage_color);
    oh->icon = CLUTTER_ACTOR (clutter_script_get_object (oh->script, "icon"));

    /* Add the group to the stage */
    clutter_container_add_actor (CLUTTER_CONTAINER (oh->stage),
                               CLUTTER_ACTOR (oh->icon));
    /* Create a timeline to manage animation */
    oh->timeline = CLUTTER_TIMELINE (clutter_script_get_object (oh->script, "main-timeline"));
    app->clutter_objects_list = g_slist_append(app->clutter_objects_list, oh);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
 
    return icon_widget;
}
#endif
