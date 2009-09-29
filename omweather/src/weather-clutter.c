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
/*******************************************************************************/
gboolean
animation_cb (SuperOH *oh)
{
    HildonAnimationActor *actor = HILDON_ANIMATION_ACTOR (oh->clutter);

    static int x_inc = 1;
    static int y_inc = 1;
    static int x = 0;
    static int y = 0;
    static int r = 0;

    if (((x_inc > 0) && (x > 800)) ||
        ((x_inc < 0) && (x < 1)))
        x_inc = -x_inc;
    if (((y_inc > 0) && (y > 480)) ||
        ((y_inc < 0) && (y < 1)))
        y_inc = -y_inc;

    x += x_inc;
    y += y_inc;
    r ++;

    oh->duration--;
    if (oh->duration == 0)
        return FALSE;
    // Set animation actor position and rotation
//    hildon_animation_actor_set_position (actor, x, y);
    hildon_animation_actor_set_rotation (actor,
                                         HILDON_AA_Z_AXIS,
                                         r,
                                         0, 0, 0);
    return TRUE;
}

GtkWidget *
create_hildon_clutter_icon_animation(GdkPixbuf *icon_buffer, const char *icon_path, int icon_size, GSList **objects_list)
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
    GtkWidget *image;
    GtkWidget *ha;
    guchar *shm = 0; /* shared mem area */
    
    /* this could come from ftol, but we hardcode it for this example */
    static key_t shm_key = 0xCAFEBEEF;

    gint  bpp, width, height;
    GdkPixbuf        *pixbuf;
    guchar           *gpixels;

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
    oh->clutter = HILDON_REMOTE_TEXTURE
        (hildon_remote_texture_new ());
    /* Temp */
    image = gtk_image_new_from_file ("/usr/share/omweather/icons/Glance/0.png");
    gtk_container_add (GTK_CONTAINER (oh->clutter), image);

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
    fprintf(stderr, "dddddddddd %s\n", buffer);
//    clutter_init(NULL, NULL);
    fprintf(stderr, "dddddddddd %s\n", buffer);
    clutter_script_load_from_file(oh->script,buffer, &error);

    fprintf(stderr, "dddddddddd %s\n", buffer);

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

    fprintf(stderr,"hhhhhhhhhhhhhhhh %s\n", (gchar*)gtk_widget_get_name(GTK_WIDGET(app->top_widget->parent)));


//    hildon_animation_actor_set_anchor_from_gravity (HILDON_ANIMATION_ACTOR (oh->clutter),
//                                                           HILDON_AA_CENTER_GRAVITY);
//    hildon_animation_actor_set_anchor (HILDON_ANIMATION_ACTOR (oh->clutter), 0, 0);
    fprintf(stderr,"hhhhhhhhhhhhhhhh %s\n", icon_name);
//    oh->stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (oh->clutter));
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
           for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){
                change_actor_size_and_position(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i),icon_size);
                fprintf(stderr,"ddddddddddddd %s\n", clutter_actor_get_name(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)));
//                ha = hildon_animation_actor_new();
//                image = gtk_image_new_from_file (clutter_actor_get_name(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)));
                  ha = HILDON_REMOTE_TEXTURE(hildon_remote_texture_new ());

    g_debug("Loading Image %s...", clutter_actor_get_name(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)));
    pixbuf          = gdk_pixbuf_new_from_file (clutter_actor_get_name(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)), &error);
    width           = gdk_pixbuf_get_width (pixbuf);
    height          = gdk_pixbuf_get_height (pixbuf);
    bpp             = gdk_pixbuf_get_n_channels (pixbuf); /* assume 8 bit */
    gpixels         = gdk_pixbuf_get_pixels (pixbuf);
    g_debug("Creating Shared Memory");
    size_t shm_size = width*height*bpp;
    int shmid;
    /*
     * Create the segment, attach it to our data space, and copy in the
     * texture we loaded
     */
    if ((shmid = shmget(shm_key, shm_size, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (guchar *) -1) {
        perror("shmat");
        exit(1);
    }

    memcpy(shm, gpixels, shm_size);
    g_debug("Done.");

    hildon_remote_texture_set_image(ha, shm_key, width, height, bpp);
    fprintf(stderr,"sdssssssssss");
    /* Set the actual position on the screen */
    hildon_remote_texture_set_position (ha, 0, 20, 800, 480);
    hildon_remote_texture_set_show (ha, 1);


                gtk_container_add (GTK_CONTAINER (ha), image);
                g_object_set_data(G_OBJECT(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)), "hildon_animation_actor", ha);

           }
        else{
           change_actor_size_and_position(oh->icon,icon_size);
           fprintf(stderr,"Attention iiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
           }
    }
    list = clutter_script_list_objects(oh->script);
    for (l = list; l != NULL; l = l->next){
        object = l->data;
        if CLUTTER_IS_BEHAVIOUR_PATH(object)
            change_knots_path(clutter_behaviour_path_get_knots((object)),icon_size);
    }
    /* Add the group to the stage */
//    clutter_container_add_actor (CLUTTER_CONTAINER (oh->stage),
//                               CLUTTER_ACTOR (oh->icon));
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
    ClutterActor    *texture;
    GdkPixbuf *pixbuf;
    GError *error;
    error = NULL;
    GtkWidget *ha;
    gint i;
    #define SIZE_OF_WINDOWS_HEAD 50
//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif
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
    /* Show actors */
    if (oh->icon){
        if CLUTTER_IS_GROUP(oh->icon)
           for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){
               fprintf(stderr,"ddddddddddddd %s\n", clutter_actor_get_name(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)));
               ha = g_object_get_data(G_OBJECT(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)), "hildon_animation_actor");
//               hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), window);
               hildon_remote_texture_set_parent(ha, GTK_WINDOW(window));
//               gdk_flush ();
//               if (gdk_window_get_state(window->window) &  GDK_WINDOW_STATE_FULLSCREEN) 
  //                  hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha), oh->icon_widget->allocation.x, oh->icon_widget->allocation.y , 0);
//               else
//                     hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha), oh->icon_widget->allocation.x, oh->icon_widget->allocation.y + SIZE_OF_WINDOWS_HEAD, 0);
               hildon_remote_texture_set_position (ha, 0, 0, 800, 480);
               if (gdk_window_get_state(window->window) &  GDK_WINDOW_STATE_FULLSCREEN) 
                     hildon_remote_texture_set_offset(ha, oh->icon_widget->allocation.x, oh->icon_widget->allocation.y);
               else
                     hildon_remote_texture_set_offset(ha, oh->icon_widget->allocation.x, oh->icon_widget->allocation.y + SIZE_OF_WINDOWS_HEAD);
              fprintf(stderr, "Position %i %i\n", oh->icon_widget->allocation.x, oh->icon_widget->allocation.y + SIZE_OF_WINDOWS_HEAD);
 
               gtk_widget_show_all(ha);
           }
        else{
               fprintf(stderr,"Attention ddddddddddddd %s\n", clutter_actor_get_name(clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i)));
           }
    }
 
//  g_timeout_add (100, (GSourceFunc)animation_cb, oh->clutter);
        list_temp = g_slist_next(list_temp);
//    }
//    return;
//#if 0
       fprintf(stderr,"ddddddddd\n");
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
        
       fprintf(stderr,"hhhhhhhhhh\n");
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

       fprintf(stderr,"111111111hhhhhhhhhh\n");
       /*
       clutter_container_add_actor (CLUTTER_CONTAINER (oh->stage),
                               CLUTTER_ACTOR (texture));
       clutter_container_lower_child(CLUTTER_CONTAINER (oh->stage),CLUTTER_ACTOR (texture),NULL);
        */
       fprintf(stderr,"22222222222hhhhhhhh\n");
        /* Start animation */
        if (oh->timeline){
            fprintf(stderr,"33333333222hhhhhhhh\n");
            clutter_timeline_start (oh->timeline);
//            g_timeout_add (10, (GSourceFunc)animation_cb, oh);
            fprintf(stderr,"Duration %i\n",clutter_timeline_get_duration(oh->timeline) );
            oh->duration = clutter_timeline_get_duration(oh->timeline) / 10;
        }
        list_temp = g_slist_next(list_temp);
    }
//#endif
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
        gtk_widget_destroy(oh->clutter);
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
