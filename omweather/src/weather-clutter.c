/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
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
#include <hildon/hildon-remote-texture.h>
#include <sys/shm.h>
#ifdef HILDONANIMATION 
#define SIZE_OF_WINDOWS_HEAD 52

/*******************************************************************************/
do_animation(SuperOH *oh, ClutterActor  *clactor, GtkWidget *ha, gboolean fullwindow)
{
    gdouble scale_x, scale_y, angle;
    gfloat rx, ry, rz;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!ha || !oh || !clactor)
        return;
    /* set position */
    if (fullwindow) 
        hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha),
                    oh->icon_widget->allocation.x + clutter_actor_get_x(clactor),
                    oh->icon_widget->allocation.y + clutter_actor_get_y(clactor), 0);
    else
        hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha),
                    oh->icon_widget->allocation.x + clutter_actor_get_x(clactor),
                    oh->icon_widget->allocation.y + clutter_actor_get_y(clactor) + 
                    SIZE_OF_WINDOWS_HEAD, 0);
 //                fprintf(stderr,"111ddddddddddddd %s %i %i\n", clutter_actor_get_name(clactor),
//                          clutter_actor_get_x(clactor), clutter_actor_get_y(clactor));
    /* set opacity */ 
    hildon_animation_actor_set_opacity(HILDON_ANIMATION_ACTOR (ha),
                                       clutter_actor_get_opacity(clactor));
    /* set scale */
    clutter_actor_get_scale(clactor, &scale_x, &scale_y);
    hildon_animation_actor_set_scale(HILDON_ANIMATION_ACTOR (ha), scale_x, scale_y);
    /* set rotation */
    angle = clutter_actor_get_rotation(clactor, CLUTTER_X_AXIS, &rx, &ry, &rz);
    hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_X_AXIS,
                                        angle,(int)rx, (int)ry, (int)rz);
    angle = clutter_actor_get_rotation(clactor, CLUTTER_Y_AXIS, &rx, &ry, &rz);
    hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_Y_AXIS,
                                        angle,(int)rx, (int)ry, (int)rz);
    angle = clutter_actor_get_rotation(clactor, CLUTTER_Z_AXIS, &rx, &ry, &rz);
    hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_Z_AXIS,
                                        angle,(int)rx, (int)ry, (int)rz);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
gboolean
animation_cb (SuperOH *oh)
{
    ClutterActor  *clactor = NULL;
    GtkWidget *ha;
    GtkWidget *window;
    gint i;
    gboolean fullwindow = FALSE;
    if (!oh)
        return FALSE;
    if (oh->icon){

        window = g_object_get_data(G_OBJECT(oh->icon), "window");
        if (window && gdk_window_get_state(window->window) &  GDK_WINDOW_STATE_FULLSCREEN) 
            fullwindow = TRUE;
        else
            fullwindow = FALSE;

        if CLUTTER_IS_GROUP(oh->icon){
            for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){
                clactor = clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i);
                ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                do_animation(oh, clactor, ha, fullwindow);
            }
        }else{
            clactor = oh->icon;
            ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
            do_animation(oh, clactor, ha, fullwindow);
        }
    }else
        return FALSE;

    oh->duration--;
    if (oh->duration == 0)
        return FALSE;
    else
        return TRUE;
}
/*******************************************************************************/
gboolean
expose_event (GtkWidget *widget,GdkEventExpose *event,
			     gpointer data)
{
    cairo_t *cr;
    GdkPixbuf *pixbuf = (GdkPixbuf *) data;
   
    cr = gdk_cairo_create(widget->window);
    gdk_cairo_region(cr, event->region);    
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0.0, 0.0);
    cairo_paint(cr);
    cairo_destroy(cr);
    return TRUE;
}

/*******************************************************************************/
realize (GtkWidget *widget)
{
    GdkScreen *screen;
    screen = gtk_widget_get_screen (widget);
    gtk_widget_set_colormap (widget, gdk_screen_get_rgba_colormap (screen));
}
/*******************************************************************************/
GtkWidget *
create_hildon_clutter_icon_animation(const char *icon_path, int icon_size, GSList **objects_list)
{
    SuperOH *oh;
    GError *error = NULL;
    ClutterActor  *clactor = NULL;
    ClutterColor stage_color;
    ClutterColor   text_color = {0, 0, 0, 255};
    gchar  buffer[1024];
    gchar  icon_name[3];
    gint   i;
    GList  *list ,*l; 
    GSList *knots, *path_list;
    GObject *object;
    GtkWidget *image;
    GtkWidget *ha;

    GdkPixbuf        *pixbuf;
    GdkPixmap *background;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
//    return NULL;
    stage_color.red = app->config->background_color.red;
    stage_color.blue = app->config->background_color.blue;
    stage_color.green = app->config->background_color.green;
    stage_color.alpha = 0xff;


    oh = g_new(SuperOH, 1);
    oh->timeline = NULL;
    oh->icon = NULL;

    oh->clutter = hildon_animation_actor_new();

    memset(buffer, 0, sizeof(buffer));
    memset(icon_name, 0, sizeof(icon_name));
    icon_name[0] = icon_path[strlen(icon_path) - 6];
    if (icon_name[0] >= '0' && icon_name[0] <= '9')
        icon_name[1] = icon_path[strlen(icon_path) - 5];
    else
        icon_name[0] = icon_path[strlen(icon_path) - 5];

    /* Download script */
    oh->script = clutter_script_new();
/*    g_object_unref(oh->script); */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.json", app->config->icons_set_base, icon_name);
//    clutter_init(NULL, NULL);
    oh->merge_id = clutter_script_load_from_file(oh->script,buffer, &error);


    /* Fix Me Need free memory */
    if (error){
        g_free (oh);
        fprintf(stderr,"ERROR in loading clutter script\n");
        g_clear_error (&error);
        return NULL;
    }

    oh->icon_widget = gtk_vbox_new(FALSE, 0);

    gtk_widget_set_size_request (oh->clutter, icon_size, icon_size);
    gtk_widget_set_size_request (oh->icon_widget, icon_size, icon_size);

    sprintf(buffer, "icon_name_%s", icon_name);
    if (oh->script)
        oh->icon = CLUTTER_ACTOR (clutter_script_get_object (oh->script, buffer));

    if (oh->icon){
        if CLUTTER_IS_GROUP(oh->icon)
           for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){

                ha = hildon_animation_actor_new();
                clactor = clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i);
                change_actor_size_and_position(clactor,icon_size);
//                fprintf(stderr,"ddddddddddddd %s\n", clutter_actor_get_name(clactor));
                pixbuf = gdk_pixbuf_new_from_file_at_size (clutter_actor_get_name(clactor), 
                                                           clutter_actor_get_width(clactor), 
                                                           clutter_actor_get_height(clactor), 
                                                           NULL);
                if (pixbuf){
                    image = gtk_image_new_from_pixbuf (pixbuf);
                    g_object_unref(G_OBJECT(pixbuf));
                }
                g_signal_connect(G_OBJECT(image), "expose_event",
                                         G_CALLBACK(expose_event), pixbuf);

                gtk_container_add (GTK_CONTAINER (ha), image);
                g_object_set_data(
                        G_OBJECT(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)), 
                                                            "hildon_animation_actor", ha);
           }
        else{
                ha = hildon_animation_actor_new();
                clactor = oh->icon;
                change_actor_size_and_position(clactor,icon_size);
//                fprintf(stderr,"ddddddddddddd %s\n", clutter_actor_get_name(clactor));
                pixbuf = gdk_pixbuf_new_from_file_at_size (clutter_actor_get_name(clactor), 
                                                           clutter_actor_get_width(clactor), 
                                                           clutter_actor_get_height(clactor), 
                                                           NULL);
                if (pixbuf){
                    image = gtk_image_new_from_pixbuf (pixbuf);
                    g_object_unref(G_OBJECT(pixbuf));
                }
                g_signal_connect(G_OBJECT(image), "expose_event",
                                         G_CALLBACK(expose_event), pixbuf);

                gtk_container_add (GTK_CONTAINER (ha), image);
                g_object_set_data(
                        G_OBJECT(oh->icon), "hildon_animation_actor", ha);
           }
    }
    list = clutter_script_list_objects(oh->script);
    for (l = list; l != NULL; l = l->next){
        object = l->data;
        if CLUTTER_IS_BEHAVIOUR_PATH(object){
            change_knots_path(path_list = clutter_behaviour_path_get_knots((object)),icon_size);
            if (path_list)
                g_slist_free(path_list);
        }
    }
    g_list_free (list);

    /* Create a timeline to manage animation */
    oh->timeline = CLUTTER_TIMELINE (clutter_script_get_object (oh->script, "main-timeline"));
    *objects_list = g_slist_append(*objects_list, oh);
    gtk_widget_show_all(oh->icon_widget);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return oh->icon_widget;
}
/************************************************************************************************/
show_hildon_animation(GSList *clutter_objects, GtkWidget *window){
    static GSList   *list_temp = NULL;
    SuperOH         *oh;
    ClutterActor  *clactor = NULL;
    GdkPixbuf *pixbuf;
    GError *error;
    error = NULL;
    GtkWidget *ha;
    gint i;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!clutter_objects)
        return;
    list_temp = clutter_objects;
    while(list_temp != NULL){
        oh = list_temp->data;
        /* Show actors */
        if (oh->icon){
            if CLUTTER_IS_GROUP(oh->icon){
                for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){
                    clactor = clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i);
                    ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                    g_object_set_data(
                            G_OBJECT(oh->icon), "window", window);
     
                    hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), window);
                    oh->duration = 1; 
                    animation_cb(oh);
    //               gdk_flush ();
                    realize(ha);
                    hildon_animation_actor_set_show (ha, 1);
                    gtk_widget_show_all(ha);
                }
            }else{
                    clactor = oh->icon;
                    ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                    g_object_set_data(
                            G_OBJECT(oh->icon), "window", window);
     
                    hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), window);
                    oh->duration = 1; 
                    animation_cb(oh);
    //               gdk_flush ();
                    realize(ha);
                    hildon_animation_actor_set_show (ha, 1);
                    gtk_widget_show_all(ha);
            }
        }
 
        /* Start animation */
        if (oh->timeline){
            clutter_timeline_start (oh->timeline);
            oh->duration = clutter_timeline_get_duration(oh->timeline) / 10;
            oh->runtime = g_timeout_add (10, (GSourceFunc)animation_cb, oh);
//            fprintf(stderr,"Duration %i\n",clutter_timeline_get_duration(oh->timeline));
        }
        list_temp = g_slist_next(list_temp);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void
change_actor_size_and_position(ClutterActor *actor, gint need_size)
{
    guint h,w;
    gint x,y;
    if (!actor)
        return;
    w = clutter_actor_get_width(actor);
    h = clutter_actor_get_height(actor);
    x = clutter_actor_get_x(actor);
    y = clutter_actor_get_y(actor);
    clutter_actor_set_width(actor,(((need_size*100)/GIANT_ICON_SIZE) * w/100)); /* GIANT_ICON_SIZE must be 128 */ 
    clutter_actor_set_height(actor,(((need_size*100)/GIANT_ICON_SIZE) * h/100)); /* GIANT_ICON_SIZE must be 128 */ 
    clutter_actor_set_x(actor,(((need_size*100)/GIANT_ICON_SIZE) * x/100)); /* GIANT_ICON_SIZE must be 128 */ 
    clutter_actor_set_y(actor,(((need_size*100)/GIANT_ICON_SIZE) * y/100)); /* GIANT_ICON_SIZE must be 128 */ 
    
}
/*******************************************************************************/
void
change_knots_path(GSList *knots, gint need_size)
{
    GList  *ks;
    ClutterKnot *knot;
    for (ks = knots; ks != NULL; ks = ks->next){
        knot = ks->data;
        knot->x = (((need_size*100)/GIANT_ICON_SIZE) * knot->x/100);
        knot->y = (((need_size*100)/GIANT_ICON_SIZE) * knot->y/100);
    }
}

/*******************************************************************************/

void free_clutter_objects_list(GSList **clutter_objects) {
    static GSList *list_temp = NULL;
    SuperOH *oh;
    ClutterActor  *clactor = NULL;
    GtkWidget *ha;
    gint i;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!*clutter_objects)
        return;
    list_temp = *clutter_objects;
    while (list_temp != NULL) {
        oh = list_temp->data;
        if (oh->timeline)
            clutter_timeline_stop(oh->timeline);
        g_source_remove(oh->runtime);
        if (oh->icon){
            if CLUTTER_IS_GROUP(oh->icon){
                for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){
                    clactor = clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i);
                    ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                    gtk_widget_destroy(ha);
                    gtk_widget_destroy(clactor);
                }
            }else{
                clactor = oh->icon;
                ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                gtk_widget_destroy(ha);
                gtk_widget_destroy(clactor);
            }
            oh->icon = NULL;
        }
        gtk_widget_destroy(oh->icon_widget);
        gtk_widget_destroy(oh->clutter);
        clutter_script_unmerge_objects(oh->script, oh->merge_id);
        g_object_unref(oh->script);
        g_free(oh);
        oh = NULL;
        list_temp = g_slist_next(list_temp);
    }
    g_slist_free(*clutter_objects);
    *clutter_objects = NULL;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}


#endif

#ifdef CLUTTER
/*******************************************************************************/
show_animation(GSList *clutter_objects){
    static GSList   *list_temp = NULL;
    SuperOH         *oh;
    ClutterActor    *texture;
    GdkPixbuf *pixbuf;
    GError *error;
    error = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!clutter_objects)
        return;
    list_temp = clutter_objects;
    while(list_temp != NULL){
        oh = list_temp->data;
        /* Add the group to the stage */
/*        
        clutter_container_add_actor (CLUTTER_CONTAINER (oh->stage),
                               CLUTTER_ACTOR (oh->icon));
        gtk_box_pack_start (GTK_BOX (oh->icon_widget), oh->clutter, TRUE, TRUE, 0);
        gtk_widget_show(oh->icon_widget);
        clutter_actor_show(oh->stage);
     gtk_widget_show_all(oh->icon_widget);

*/

       /* Make background texture */
       gtk_box_pack_start (GTK_BOX (oh->icon_widget), oh->clutter, TRUE, TRUE, 0);
       pixbuf = gdk_pixbuf_get_from_drawable (NULL, oh->icon_widget->window, gtk_widget_get_colormap(oh->icon_widget),
                   oh->icon_widget->allocation.x, oh->icon_widget->allocation.y, 0, 0,
                   oh->icon_widget->allocation.width, oh->icon_widget->allocation.height);
/*
       gdk_pixbuf_save (pixbuf , "screenie.png", "png", NULL, NULL);
       pixbuf = gdk_pixbuf_new_from_file("./redhand.png", NULL);

       texture =gtk_clutter_texture_new_from_pixbuf(pixbuf);
*/       
       texture = clutter_texture_new();
       clutter_texture_set_from_rgb_data(texture,gdk_pixbuf_get_pixels(pixbuf),FALSE,
             oh->icon_widget->allocation.width,oh->icon_widget->allocation.height,
             gdk_pixbuf_get_rowstride (pixbuf),3,0,&error);
       if (pixbuf)
           g_object_unref(pixbuf);
       if (error){
            g_warning ("Unable to set the pixbuf: %s", error->message);
                  g_error_free (error);
       }

       clutter_container_add_actor (CLUTTER_CONTAINER (oh->stage),
                               CLUTTER_ACTOR (texture));
       clutter_container_lower_child(CLUTTER_CONTAINER (oh->stage),CLUTTER_ACTOR (texture),NULL);

        /* Start animation */
        if (oh->timeline)
            clutter_timeline_start (oh->timeline);
        list_temp = g_slist_next(list_temp);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
void
change_actor_size_and_position(ClutterActor *actor, gint need_size)
{
    guint h,w;
    gint x,y;
    if (!actor)
        return;
    w = clutter_actor_get_width(actor);
    h = clutter_actor_get_height(actor);
    x = clutter_actor_get_x(actor);
    y = clutter_actor_get_y(actor);
    clutter_actor_set_width(actor,(((need_size*100)/GIANT_ICON_SIZE) * w/100)); /* GIANT_ICON_SIZE must be 128 */ 
    clutter_actor_set_height(actor,(((need_size*100)/GIANT_ICON_SIZE) * h/100)); /* GIANT_ICON_SIZE must be 128 */ 
    clutter_actor_set_x(actor,(((need_size*100)/GIANT_ICON_SIZE) * x/100)); /* GIANT_ICON_SIZE must be 128 */ 
    clutter_actor_set_y(actor,(((need_size*100)/GIANT_ICON_SIZE) * y/100)); /* GIANT_ICON_SIZE must be 128 */ 
    
}
/*******************************************************************************/
void
change_knots_path(GSList *knots, gint need_size)
{
    GList  *ks;
    ClutterKnot *knot;
    for (ks = knots; ks != NULL; ks = ks->next){
        knot = ks->data;
        knot->x = (((need_size*100)/GIANT_ICON_SIZE) * knot->x/100);
        knot->y = (((need_size*100)/GIANT_ICON_SIZE) * knot->y/100);
    }
}

/*******************************************************************************/
/* Set the clutter colors form the current gtk theme */
static void
create_colors (GtkWidget *widget, ClutterColor *stage, ClutterColor *text)
{
  gtk_clutter_get_bg_color (widget, GTK_STATE_NORMAL, stage);
  gtk_clutter_get_text_color (widget, GTK_STATE_NORMAL, text);
}
/*******************************************************************************/
GtkWidget *
create_clutter_icon_animation(GdkPixbuf *icon_buffer, const char *icon_path, int icon_size, GSList **objects_list)
{
    SuperOH *oh;
    GError *error = NULL;
    ClutterColor stage_color;
    ClutterColor   text_color = {0, 0, 0, 255};
    gchar  buffer[1024];
    gchar  icon_name[3];
    gint   i;
    GList  *list ,*l;
    GSList *knots;
    GObject *object;

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
/*    g_object_unref(oh->script); */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.json", app->config->icons_set_base, icon_name);
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
    gtk_widget_set_size_request (oh->icon_widget, icon_size, icon_size);



    oh->stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (oh->clutter));
    /* and its background color */

/*    clutter_stage_set_color (CLUTTER_STAGE (oh->stage),
                  &stage_color);
*/

    sprintf(buffer, "icon_name_%s", icon_name);
    if (oh->script)
        oh->icon = CLUTTER_ACTOR (clutter_script_get_object (oh->script, buffer));

    /* set valid size for actors */
    if (oh->icon){
        if CLUTTER_IS_GROUP(oh->icon)
           for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++)
               change_actor_size_and_position(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i),icon_size);
        else
           change_actor_size_and_position(oh->icon,icon_size);
    }
    list = clutter_script_list_objects(oh->script);
    for (l = list; l != NULL; l = l->next){
        object = l->data;
        if CLUTTER_IS_BEHAVIOUR_PATH(object)
            change_knots_path(clutter_behaviour_path_get_knots((object)),icon_size);
    }
    /* Add the group to the stage */
    clutter_container_add_actor (CLUTTER_CONTAINER (oh->stage),
                               CLUTTER_ACTOR (oh->icon));
    /* Create a timeline to manage animation */
    oh->timeline = CLUTTER_TIMELINE (clutter_script_get_object (oh->script, "main-timeline"));
    *objects_list = g_slist_append(*objects_list, oh);
    gtk_widget_show_all(oh->icon_widget);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return oh->icon_widget;
}
/************************************************************************************************/
void free_clutter_objects_list(GSList **clutter_objects) {
    static GSList *list_temp = NULL;
    SuperOH *oh;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!*clutter_objects)
        return;
    list_temp = *clutter_objects;
    while (list_temp != NULL) {
        oh = list_temp->data;
        if (oh->timeline)
            clutter_timeline_stop(oh->timeline);
/*
        clutter_actor_destroy(oh->stage);
        g_object_unref(oh->script);
*/
        gtk_widget_destroy(oh->icon_widget);
        g_object_unref(oh->script);
        g_free(oh);
        list_temp = g_slist_next(list_temp);
    }
    g_slist_free(*clutter_objects);
    *clutter_objects = NULL;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}

#endif
