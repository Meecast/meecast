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
#include <sys/shm.h>
#define SIZE_OF_WINDOWS_HEAD 52
#define GIANT_ICON_SIZE  128

extern gboolean key;
extern FILE *file_out;
typedef struct SuperOH{
  ClutterActor          *icon;
  ClutterGroup          *group;
  ClutterActor          *stage;
  ClutterScript         *script;
  ClutterTimeline       *timeline;
  GtkWidget             *clutter;
  GtkWidget             *icon_widget;
  GdkPixbuf             *bgpixb;
  ClutterActor          *bgtex;
  guint                 runtime;
  guint                 merge_id;
  guint                 duration;
  gchar                 *icon_name;
}SuperOH; 
typedef struct actor_property{
    gdouble scale_x, scale_y, angle_x, angle_y, angle_z;
    gint rxx, ryx, rzx;
    gint rxy, ryy, rzy;
    gint rxz, ryz, rzz;
    gint x,y,opacity;
}actor_property; 

gint counter = 0;
/*******************************************************************************/
pout(gchar *buffer){
    fprintf(stdout,"%s",buffer);
    fflush (stdout);
}
/*******************************************************************************/
pout2(gchar *buffer){
    fprintf(file_out,"%s",buffer);
    fflush (file_out);
}

/*******************************************************************************/
void
do_animation(SuperOH *oh, ClutterActor  *clactor, GtkWidget *ha, gboolean fullwindow)
{
    gdouble scale_x, scale_y, angle;
    gint rx, ry, rz;
    gint allocationx = 0, allocationy = 0;
    GtkWidget *parent = NULL;
    gchar  bufferout[2048];
    actor_property *property;

    if (!ha || !oh || !clactor)
        return;
    /* set position */
    /* Add offset for some parents widgets */
    /*
    parent = oh->icon_widget;
    while (!GTK_IS_WINDOW(parent = parent->parent))
    {
        //fprintf(stderr, "Name %s %i \n",gtk_widget_get_name(GTK_WIDGET(parent)), parent->allocation.y);     
        if (HILDON_IS_PANNABLE_AREA(parent)){
            allocationy =  allocationy + parent->allocation.y;
            allocationx =  allocationx + parent->allocation.x;
         }
    }
    */
//    if (fullwindow) 
//        hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha),
//                    oh->icon_widget->allocation.x + clutter_actor_get_x(clactor) + allocationx,
//                    oh->icon_widget->allocation.y + clutter_actor_get_y(clactor) + allocationy, 0);
//    else
//        hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha),
//                    oh->icon_widget->allocation.x + clutter_actor_get_x(clactor) + allocationx,
 //                   oh->icon_widget->allocation.y + clutter_actor_get_y(clactor) + allocationy +
 //                   SIZE_OF_WINDOWS_HEAD, 0);
 //                fprintf(stderr,"111ddddddddddddd %s %i %i\n", clutter_actor_get_name(clactor),
//                          clutter_actor_get_x(clactor), clutter_actor_get_y(clactor));
    /* set opacity */ 
//    hildon_animation_actor_set_opacity(HILDON_ANIMATION_ACTOR (ha),
//                                       clutter_actor_get_opacity(clactor));
    /* set scale */
//    clutter_actor_get_scale(clactor, &scale_x, &scale_y);
//    hildon_animation_actor_set_scale(HILDON_ANIMATION_ACTOR (ha), scale_x, scale_y);
    /* set rotation */
//    angle = clutter_actor_get_rotation(clactor, CLUTTER_X_AXIS, &rx, &ry, &rz);
//    hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_X_AXIS,
//                                       angle,(int)rx, (int)ry, (int)rz);
//    angle = clutter_actor_get_rotation(clactor, CLUTTER_Y_AXIS, &rx, &ry, &rz);
//    hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_Y_AXIS,
//                                        angle,(int)rx, (int)ry, (int)rz);
//    angle = clutter_actor_get_rotation(clactor, CLUTTER_Z_AXIS, &rx, &ry, &rz);
//    hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_Z_AXIS,
//                                        angle,(int)rx, (int)ry, (int)rz);
    sprintf(bufferout,"if (!list_temp) \n \
                        return FALSE; \n \
                clactor = list_temp->data; \n \
                ha = g_object_get_data(G_OBJECT(clactor), \"hildon_animation_actor\");\n \
                if (ha){\n \
                    /* Name %s */\n",clutter_actor_get_name(clactor));
   pout(bufferout);
//                    hildon_animation_actor_set_rotation (ha,\n \
//                                             HILDON_AA_Z_AXIS,\n \
//                                             r,\n \
//                                             0, 0, 0);\n \
   
    if (g_object_get_data(G_OBJECT(clactor),"property")){
        property = g_object_get_data(G_OBJECT(clactor),"property");
    }else{
        property = g_new(actor_property, 1);
        property->x = 0;
        property->y = 0;
        property->opacity = 0;
        property->rxx = 0; property->ryx = 0; property->rzx = 0;
        property->rxy = 0; property->ryy = 0; property->rzy = 0;
        property->rxz = 0; property->ryz = 0; property->rzz = 0;
        property->scale_x = 1;
        property->scale_y = 1;
        property->angle_x = 0;
        property->angle_y = 0;
        property->angle_z = 0;
        g_object_set_data(G_OBJECT(clactor), "property", property);
    }

    if (property->x != clutter_actor_get_x(clactor) ||
        property->y != clutter_actor_get_y(clactor)|| counter == 3){
        sprintf(bufferout,"                     if (!fullwindow) { \n \
                          hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha), \n \
                          oh->icon_widget->allocation.x + \n \
                          (((oh->icon_size*100)/GIANT_ICON_SIZE) * %i/100) + allocationx, \n \
                          oh->icon_widget->allocation.y + \n \
                          (((oh->icon_size*100)/GIANT_ICON_SIZE) * %i/100) + allocationy + SIZE_OF_WINDOWS_HEAD, 0);\n \
                    }else{\n \
                          hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha), \n \
                          oh->icon_widget->allocation.x + \n \
                          (((oh->icon_size*100)/GIANT_ICON_SIZE) * %i/100) + allocationx, \n \
                          oh->icon_widget->allocation.y + \n \
                          (((oh->icon_size*100)/GIANT_ICON_SIZE) * %i/100) + allocationy,\n \
                          0);\n \
                    }\n", clutter_actor_get_x(clactor), clutter_actor_get_y(clactor), clutter_actor_get_x(clactor), clutter_actor_get_y(clactor)); 
        property->x = clutter_actor_get_x(clactor);
        property->y = clutter_actor_get_y(clactor);
        pout(bufferout);
        sprintf(bufferout, "           <p x=\"%i\" y=\"%i\"></p>\n", clutter_actor_get_x(clactor), clutter_actor_get_y(clactor));
        pout2(bufferout);
 
    }
    if (property->opacity!= clutter_actor_get_opacity(clactor)){
        sprintf(bufferout,"                      hildon_animation_actor_set_opacity(HILDON_ANIMATION_ACTOR (ha), %i); \n", clutter_actor_get_opacity(clactor)); 
        property->opacity = clutter_actor_get_opacity(clactor);
        pout(bufferout);
        sprintf(bufferout, "           <o o=\"%i\"></o>\n", clutter_actor_get_opacity(clactor));
        pout2(bufferout);
 
    }
    clutter_actor_get_scale(clactor, &scale_x, &scale_y);
    if (property->scale_x != scale_x || property->scale_y != scale_y){
        sprintf(bufferout,"                      hildon_animation_actor_set_scale(HILDON_ANIMATION_ACTOR (ha), %f, %f); \n", scale_x, scale_y); 
        property->scale_x = scale_x;
        property->scale_y = scale_y;
        pout(bufferout);
        sprintf(bufferout, "           <s x=\"%f\" y=\"%f\" ></s>\n", scale_x, scale_y);
        pout2(bufferout);
 
    }
    angle = clutter_actor_get_rotation(clactor, CLUTTER_X_AXIS, &rx, &ry, &rz);
    if (angle != property->angle_x || rx != property->rxx || ry != property->ryx || rz != property->rzx){
        if (rx < 1024 || rx >= 0 ){
            sprintf(bufferout,"                      hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_X_AXIS, %f, %i, %i, %i);\n",angle, rx, ry, rz); 
            pout(bufferout);
            sprintf(bufferout, "           <rx a=\"%f\" x=\"%i\" y=\"%i\" z=\"%i\" ></rx>\n", angle, rx, ry, rz);
            pout2(bufferout);
        }
        property->angle_x = angle;
        property->rxx = rx;
        property->ryx = ry;
        property->rzx = rz;
    }
    angle = clutter_actor_get_rotation(clactor, CLUTTER_Y_AXIS, &rx, &ry, &rz);
    if (angle != property->angle_y || rx != property->rxy || ry != property->ryy || rz != property->rzy){
        if (rx < 1024 || rx >= 0){
            sprintf(bufferout,"                     hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_Y_AXIS, %f, %i, %i, %i);\n",angle, rx, ry, rz); 
            pout(bufferout);
            sprintf(bufferout, "           <ry a=\"%f\" x=\"%i\" y=\"%i\" z=\"%i\" ></ry>\n", angle, rx, ry, rz);
            pout2(bufferout);
 
        }
        property->angle_y = angle;
        property->rxy = rx;
        property->ryy = ry;
        property->rzy = rz;
    }
    angle = clutter_actor_get_rotation(clactor, CLUTTER_Z_AXIS, &rx, &ry, &rz);
    if (angle != property->angle_z || rx != property->rxz || ry != property->ryz || rz != property->rzz){
        if (rx < 1024 || rx >= 0){
            sprintf(bufferout,"                     hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), HILDON_AA_Z_AXIS, %f, %i, %i, %i);\n",angle, rx, ry, rz); 
            pout(bufferout);
            sprintf(bufferout, "           <rz a=\"%f\" x=\"%i\" y=\"%i\" z=\"%i\" ></rz>\n", angle, rx, ry, rz);
            pout2(bufferout);
 
        }
        property->angle_z = angle;
        property->rxz = rx;
        property->ryz = ry;
        property->rzz = rz;
    }
    sprintf(bufferout,"                 }\n \
                list_temp = g_slist_next(list_temp);\n \
                ");
        pout(bufferout);

 
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
    gchar  bufferout[2048];

    if (!oh)
        return FALSE;
    if (oh->icon){

        sprintf(bufferout,"  case %i:\n",counter);
        pout(bufferout);
        sprintf(bufferout,"                 list_temp = oh->list_images;\n \
                "); 
        pout(bufferout);

        sprintf(bufferout, "         <s n=\"%i\">\n", counter);
        pout2(bufferout);
 
        if CLUTTER_IS_GROUP(oh->icon){ 
            for (i=0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){
                clactor = clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i);
                ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                sprintf(bufferout, "         <a>\n", counter);
                pout2(bufferout);
                do_animation(oh, clactor, ha, fullwindow);
                sprintf(bufferout, "         </a>\n", counter);
                pout2(bufferout);
 
            }
        }else{
            clactor = oh->icon;
            ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
            sprintf(bufferout, "         <a>\n", counter);
            pout2(bufferout);
            do_animation(oh, clactor, ha, fullwindow);
            sprintf(bufferout, "         </a>\n", counter);
            pout2(bufferout);
 
        }
        sprintf(bufferout,"break;\n \
        "); 
        pout(bufferout);

        sprintf(bufferout, "         </s>\n");
        pout2(bufferout);
 
    }else
        return FALSE;
    counter ++;
    oh->duration--;
    if (oh->duration == 0){;
        fprintf(stderr,"Finished\n"); 
        sprintf(bufferout," default: return FALSE;  \
    } \n \
     oh->timeline++;  \n\
#ifdef DEBUGFUNCTIONCALL \n\
    END_FUNCTION; \n\
#endif \n\
     return TRUE;  \n}\n");
    pout(bufferout);
        key = FALSE;
        return FALSE;
    }
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
    gchar  bufferout[2048];
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
    if (!objects_list)
        return NULL;

    counter = 0;
    oh = g_new(SuperOH, 1);
    oh->timeline = NULL;
    oh->icon = NULL;

    memset(buffer, 0, sizeof(buffer));
    memset(icon_name, 0, sizeof(icon_name));
    icon_name[0] = icon_path[strlen(icon_path) - 6];
    if (icon_name[0] >= '0' && icon_name[0] <= '9')
        icon_name[1] = icon_path[strlen(icon_path) - 5];
    else
        icon_name[0] = icon_path[strlen(icon_path) - 5];
    
    oh->icon_name =g_strdup (icon_name);
    /* Download script */
    oh->script = clutter_script_new();
/*    g_object_unref(oh->script); */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.json", "/usr/share/omweather/icons/Glance/", icon_name);
    fprintf(stderr,"JSON SCRIPT: %s\n",buffer);
    oh->merge_id = clutter_script_load_from_file(oh->script,buffer, &error);


    if (error){
        g_free (oh);
        fprintf(stderr,"ERROR in loading clutter script\n");
        g_clear_error (&error);
        key = FALSE;
        return NULL;
    }

    oh->icon_widget = gtk_vbox_new(FALSE, 0);

    gtk_widget_set_size_request (oh->icon_widget, icon_size, icon_size);
   
    sprintf(bufferout, "<icon name= \"%s\"> \n", icon_name);
    pout2(bufferout);

    sprintf(bufferout,"gboolean\n");
    pout(bufferout);
    sprintf(bufferout,"icon%s_timeline (SuperOH *oh)\n", icon_name);
    pout(bufferout);
    sprintf(bufferout,"{ \n \
       GdkPixbuf  *pixbuf; \n \
       gchar      buffer[1024]; \n \
       GtkWidget  *ha = NULL; \n \
       GtkWidget  *window = NULL; \n \
       GSList     *list_temp = NULL;\n \
       GtkWidget  *clactor = NULL; \n \
       gint allocationx = 0, allocationy = 0; \n \
       gboolean fullwindow ; \n \
\n \
       if (oh->timeline >=2){ \n \
            window = oh->window; \n \
            if (window && (gdk_window_get_state(window->window) &  GDK_WINDOW_STATE_FULLSCREEN || \n \
                           !strcmp(gtk_widget_get_name(window), \"OmweatherDesktopWidget\")))\n \
                fullwindow = TRUE;\n \
            else\n\
                fullwindow = FALSE;\n \
       } \n \
       if (!oh)\n \
           return FALSE;\n \
       switch (oh->timeline){\n \
           case 0: \n \
                  memset(buffer, 0, sizeof(buffer));\n"); 
    pout(bufferout);
    sprintf(buffer, "icon_name_%s", icon_name);
    if (oh->script)
        oh->icon = CLUTTER_ACTOR (clutter_script_get_object (oh->script, buffer));

    sprintf(bufferout, "         <s n=\"0\">\n");
    pout2(bufferout);
 
    if (oh->icon){
        if CLUTTER_IS_GROUP(oh->icon){ 
           for (i = 0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){
                ha = hildon_animation_actor_new();
                clactor = clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i);
                sprintf(bufferout,"             pixbuf = gdk_pixbuf_new_from_file_at_size (\"%s\",\n",clutter_actor_get_name(clactor));
                pout(bufferout);
                sprintf(bufferout,"              (((oh->icon_size*100)/GIANT_ICON_SIZE) * %i/100), \n", clutter_actor_get_width(clactor));
                pout(bufferout);
                sprintf(bufferout,"              (((oh->icon_size*100)/GIANT_ICON_SIZE) * %i/100), \n", clutter_actor_get_height(clactor));
                pout(bufferout);
                sprintf(bufferout,"                   NULL);\n");
                pout(bufferout);
                sprintf(bufferout,"\n \
            if (pixbuf){ \n \
                oh->image = gtk_image_new_from_pixbuf (pixbuf);\n \
                g_object_unref(G_OBJECT(pixbuf));\n \
                oh->list_images = g_slist_append(oh->list_images, oh->image);\n \
            }\n \
            g_signal_connect(G_OBJECT(oh->image), \"expose_event\",\n \
                                             G_CALLBACK(expose_event), pixbuf);\n \
            ha = hildon_animation_actor_new();\n \
            gtk_container_add (GTK_CONTAINER (ha), oh->image);\n \
            g_object_set_data(\n \
                           G_OBJECT(oh->image), \"hildon_animation_actor\", ha);\n"); 
            
                pout(bufferout);

               
                sprintf(bufferout, "         <a><l n=\"%s\" h=\"%i\" w=\"%i\"></l></a>\n", clutter_actor_get_name(clactor), clutter_actor_get_height(clactor), clutter_actor_get_width(clactor));

                 pout2(bufferout);

               sprintf(bufferout,"             hildon_animation_actor_set_opacity(HILDON_ANIMATION_ACTOR (ha), 0); \n" ); 
                pout(bufferout);

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
           fprintf(stderr, "Count of objects %i\n",i);
        }else{
                ha = hildon_animation_actor_new();
                clactor = oh->icon;
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
                sprintf(bufferout, "         <lactor n=\"%s\" h=\"%i\" w=\"%i\"></lactor>\n", clutter_actor_get_name(clactor), clutter_actor_get_height(clactor), clutter_actor_get_width(clactor));
                pout2(bufferout);
                }
    }
    sprintf(bufferout, "         </s>\n");
    pout2(bufferout);
 

    sprintf(bufferout,"     break;\n");
    pout(bufferout);
/* 
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
*/
    /* Create a timeline to manage animation */
    oh->timeline = CLUTTER_TIMELINE (clutter_script_get_object (oh->script, "main-timeline"));
    *objects_list = g_slist_append(*objects_list, oh);
//    gtk_widget_show_all(oh->icon_widget);
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
    gchar  bufferout[2048];
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
//                    ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
//                    g_object_set_data(
//                            G_OBJECT(oh->icon), "window", window);
     
//                    hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), window);
//                    oh->duration = 1; 
  //                  animation_cb(oh);
    //               gdk_flush ();
  //                  realize(ha);
  //                  hildon_animation_actor_set_show (ha, 1);
  //                  gtk_widget_show_all(ha);
                      
                }
            }else{
                    clactor = oh->icon;
                    ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                    g_object_set_data(
                            G_OBJECT(oh->icon), "window", window);
     
    //                hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), window);
//                    oh->duration = 1; 
//                    animation_cb(oh);
    //               gdk_flush ();
                    realize(ha);
      //              hildon_animation_actor_set_show (ha, 1);
        //            gtk_widget_show_all(ha);
            }
        }
       sprintf(bufferout,"\n \
     case 1:  \n \
            list_temp = oh->list_images;\n \
            while(list_temp != NULL){\n \
                ha = g_object_get_data(G_OBJECT(list_temp->data), \"hildon_animation_actor\");\n \
                hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), oh->window);\n \
                // Set anchor point to the actor center\n \
                hildon_animation_actor_set_anchor_from_gravity (HILDON_ANIMATION_ACTOR (ha),\n \
                                                                               HILDON_AA_NW_GRAVITY);\n \
                realize(ha);\n \
                gtk_widget_show_all (ha);\n \
                list_temp = g_slist_next(list_temp);\n \
            }\n \
            g_source_remove(oh->runtime); \n \
            oh->runtime = g_timeout_add (oh->delay, icon%s_timeline, oh); \n \
            break;\n \
     case 2:\n \
            g_source_remove(oh->runtime);\n \
            oh->runtime = g_timeout_add (50, icon%s_timeline, oh); \n \
            break;\n \
            ", oh->icon_name, oh->icon_name);
            pout(bufferout);
 
            list_temp = g_slist_next(list_temp);
    }
    /* Start animation */
    if (oh->timeline){
        counter = 3;
        oh->duration = clutter_timeline_get_duration(oh->timeline)/50;
        oh->runtime = g_timeout_add (50, (GSourceFunc)animation_cb, oh);
        clutter_timeline_start (oh->timeline);
//        fprintf(stderr,"Duration %i\n",clutter_timeline_get_duration(oh->timeline));
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
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
                for (i = 0; i < clutter_group_get_n_children(CLUTTER_GROUP(oh->icon)); i++){
                    clactor = clutter_group_get_nth_child(CLUTTER_GROUP(oh->icon),i);
                    ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                    gtk_widget_destroy(ha);
                }
            }else{
                clactor = oh->icon;
                ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
                gtk_widget_destroy(ha);
            }
            oh->icon = NULL;
        }
        fprintf(stderr,"aaaaaaaaaa\n");
        if (oh->icon_widget){
            gtk_widget_destroy(oh->icon_widget);
            oh->icon_widget = NULL;
        }
        //clutter_script_unmerge_objects(oh->script, oh->merge_id);
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

