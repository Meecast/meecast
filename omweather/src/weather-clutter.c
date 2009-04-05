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
        /* Add the group to the stage */
//        clutter_container_add_actor (CLUTTER_CONTAINER (oh->stage),
//                               CLUTTER_ACTOR (oh->icon));
//        gtk_box_pack_start (GTK_BOX (oh->icon_widget), oh->clutter, TRUE, TRUE, 0);
//        gtk_widget_show(oh->icon_widget);
//        clutter_actor_show(oh->stage);
        if (oh->timeline)
            clutter_timeline_start (oh->timeline);
        list_temp = g_slist_next(list_temp);
    }
}
/*******************************************************************************/
void
change_actor_size(ClutterActor *actor, gint need_size)
{
    guint h,w;
    if (!actor)
        return;
    w = clutter_actor_get_width(actor);
    h = clutter_actor_get_height(actor);
    clutter_actor_set_width(actor,(((need_size*100)/GIANT_ICON_SIZE) * w/100)); /* GIANT_ICON_SIZE must be 128 */ 
    clutter_actor_set_height(actor,(((need_size*100)/GIANT_ICON_SIZE) * h/100)); /* GIANT_ICON_SIZE must be 128 */ 
    fprintf(stderr,"Need size %i width %i height %i\n",need_size,(((need_size*100)/GIANT_ICON_SIZE) * w/100),( need_size/GIANT_ICON_SIZE * h));
}
/*******************************************************************************/
GtkWidget *
create_clutter_main_icon(GdkPixbuf *icon_buffer, const char *icon_path, int icon_size)
{
    SuperOH *oh;
    GError *error = NULL;
    ClutterColor stage_color;
    gchar  buffer[1024];
    gchar  icon_name[3];
    gint   i;
    GList  *list ,*l, *ks;
    GSList *knots;
    GObject *object;
    ClutterKnot *knot;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    stage_color.red = app->config->background_color.red;
    stage_color.blue = app->config->background_color.blue;
    stage_color.green = app->config->background_color.green;
    stage_color.alpha = 0xff;

    oh = g_new(SuperOH, 1);
    oh->timeline = NULL;
    oh->icon = NULL;
    
    oh->clutter = gtk_clutter_embed_new();

    memset(buffer, 0, sizeof(buffer));
    memset(icon_name, 0, sizeof(icon_name));
    icon_name[0] = icon_path[strlen(icon_path) - 6];
    if (icon_name[0] >= '0' && icon_name[0] <= '9')
        icon_name[1] = icon_path[strlen(icon_path) - 5];
    else
        icon_name[0] = icon_path[strlen(icon_path) - 5];

    /* Download script */
    oh->script = clutter_script_new();
//    g_object_unref(oh->script);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.json", path_large_icon,icon_name);
    clutter_script_load_from_file(oh->script,buffer, &error);
    /* Fix Me Need free memory */
    if (error){
        g_free (oh);
        fprintf(stderr,"ERROR in loading clutter script\n");
        g_clear_error (&error);
        return NULL;
    }

    oh->icon_widget = gtk_vbox_new(FALSE, 0);

    gtk_widget_set_size_request (oh->clutter, icon_size, icon_size);
//    gtk_widget_set_size_request (oh->icon_widget, icon_size, icon_size);
    gtk_container_add (GTK_CONTAINER (oh->icon_widget), oh->clutter);
//    gtk_box_pack_start (GTK_BOX (oh->icon_widget), oh->clutter, TRUE, TRUE, 0);
    oh->stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (oh->clutter));
    /* and its background color */
    clutter_stage_set_color (CLUTTER_STAGE (oh->stage),
                  &stage_color);

    sprintf(buffer, "icon_name_%s", icon_name);
    if (oh->script)
        oh->icon = CLUTTER_ACTOR (clutter_script_get_object (oh->script, buffer));

    /* set valid size for actors */
    if (oh->icon){
        if CLUTTER_IS_GROUP(oh->icon)
           for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++)
               change_actor_size(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i),icon_size);
        else
            change_actor_size(oh->icon,icon_size);
    }
    list = clutter_script_list_objects(oh->script);
    for (l = list; l != NULL; l = l->next){
	    object = l->data;
	    if CLUTTER_IS_BEHAVIOUR_PATH(object){
		fprintf(stderr," BEHAVIOUR sssssssssssss\n");
		knots = clutter_behaviour_path_get_knots((object));
		for (ks = knots; ks != NULL; ks = ks->next){
		    knot = ks->data;
		    fprintf(stderr, "X: %i Y: %i\n", knot->x, knot->y);
		}
	    }
	    
    }

    /* Add the group to the stage */
    clutter_container_add_actor (CLUTTER_CONTAINER (oh->stage),
                               CLUTTER_ACTOR (oh->icon));
    /* Create a timeline to manage animation */
    oh->timeline = CLUTTER_TIMELINE (clutter_script_get_object (oh->script, "main-timeline"));
    app->clutter_objects_list = g_slist_append(app->clutter_objects_list, oh);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return oh->icon_widget;
}
#endif
