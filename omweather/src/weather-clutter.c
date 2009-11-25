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
#ifdef HILDONANIMATION 
#define SIZE_OF_WINDOWS_HEAD 52
/*******************************************************************************/
void 
free_animation_list(gpointer key, gpointer value_arg,
    gpointer user_data)
{
    GSList *list_of_event = (GSList *)value_arg;
    GSList      *list_temp = NULL;
    Event       *event = NULL;
    Event_l     *event_l = NULL;
    Event_p     *event_p = NULL;
    Event_o     *event_o = NULL;
    Event_s     *event_s = NULL;
    Event_r     *event_r = NULL;
 
    /* Free user data */
    fprintf(stderr, "KEY %s %p\n", key, list_of_event);
    list_temp = list_of_event;
    while(list_temp != NULL){
        if (list_temp->data){
            event = list_temp->data;
            switch ((gint)event->event_type){
                case POSITION_ACTOR:
                     event_p = event->event;
                     if (event_p)
                        g_free(event_p);
                     break;
                case OPACITY_ACTOR:
                     event_o = event->event;
                     if (event_o)
                        g_free(event_o);
                     break;
                case SCALE_ACTOR:
                     event_s = event->event;
                     if (event_s)
                        g_free(event_s);
                case ROTATE_ACTOR:
                     event_r = event->event;
                     if (event_r)
                        g_free(event_r);
                     break;
                case LOAD_ACTOR:
                     event_l = event->event;
                     if (event_l->name)
                        g_free(event_l->name);
                     if (event_l)
                        g_free(event_l);
                     break;
            }
            g_free(event);
        }
        list_temp = g_slist_next(list_temp);
    } 
    if (list_of_event)
        g_slist_free(list_of_event);   
}
/*******************************************************************************/
void 
free_icons_list(gpointer key, gpointer value_arg,
    gpointer user_data)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    GHashTable *hash = (GHashTable *)value_arg;
    xmlChar    *icon_name = (xmlChar *)key;
    g_hash_table_foreach(hash, free_animation_list, NULL);
    g_hash_table_destroy(hash);
    g_free(icon_name);

}
/*******************************************************************************/
void 
clear_animation_hash(GHashTable *hash)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (hash){
        g_hash_table_foreach(hash, free_icons_list, NULL);
        g_hash_table_destroy(hash);
        hash = NULL;
    }
}
/*******************************************************************************/
void
parse_animation_of_icon(xmlNode *node, GHashTable *icons){
    xmlNode     *child_node, *child_node2, *child_node3; 
    xmlChar     *value = NULL;
    xmlChar     *number_of_step = NULL;
    xmlChar     *icon_name = NULL;
    xmlChar     *temp_char = NULL;
    GHashTable  *icon_animation_hash = NULL; 
    GHashTable  *icon_animation_hash_temp = NULL; 
    Event       *event = NULL;
    Event_l     *event_l = NULL;
    Event_p     *event_p = NULL;
    Event_o     *event_o = NULL;
    Event_s     *event_s = NULL;
    Event_r     *event_r = NULL;
    GSList      *list_of_event = NULL;
    GSList      *temp_list_of_event = NULL;
    gint        number_actor_in_queue = 1;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    setlocale(LC_NUMERIC, "POSIX");
    while(node){
        if(node->type == XML_ELEMENT_NODE){
            /* name */
            if(!xmlStrcmp(node->name, (const xmlChar*)"icon")){
                value = xmlGetProp(node, (const xmlChar*)"name");
                icon_animation_hash = g_hash_table_new(g_str_hash, g_str_equal); 
                g_hash_table_insert(icons, g_strdup(value), icon_animation_hash);
                for(child_node = node->children; child_node; child_node = child_node->next){
                    if( child_node->type == XML_ELEMENT_NODE ){
                        if(!xmlStrcmp(child_node->name, (const xmlChar*)"s")){
                            number_of_step = xmlGetProp(child_node, (const xmlChar*)"n");
                            list_of_event = g_new0(GSList, 1);
                            number_actor_in_queue = 1;
                            for(child_node2 = child_node->children;
                               child_node2; child_node2 = child_node2->next){
                                if( child_node2->type == XML_ELEMENT_NODE ){
                                    if(!xmlStrcmp(child_node2->name, (const xmlChar*)"a")){
                                        for(child_node3 = child_node2->children; 
                                           child_node3; child_node3 = child_node3->next){
                                            /* load actor */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar*)"l")){
                                                event_l = g_new0(Event_l, 1);
                                                icon_name = xmlGetProp(child_node3, (const xmlChar*)"n");
                                                if(icon_name){
                                                    event_l->name = g_strdup(icon_name);
                                                    xmlFree(icon_name);
                                                }
                                                if (temp_char = xmlGetProp(child_node3, 
                                                                           (const xmlChar*)"h")){
                                                    event_l->height = atoi(temp_char); 
                                                    xmlFree(temp_char);
                                                }
                                                if (temp_char = xmlGetProp(child_node3, 
                                                                           (const xmlChar*)"w")){
                                                    event_l->width = atoi(temp_char); 
                                                    xmlFree(temp_char);
                                                }
                                                event = g_new0(Event, 1);
                                                event->event_type = LOAD_ACTOR;
                                                event->event = event_l;
                                                event->number = number_actor_in_queue;
                                                list_of_event = g_slist_append(list_of_event, event);
                                            }
                                            /* changed Position of actor */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar*)"p")){
                                                event_p = g_new0(Event_p, 1);
                                                if (temp_char = xmlGetProp(child_node3, 
                                                                           (const xmlChar*)"x")){
                                                    event_p->x = atoi(temp_char);
                                                    xmlFree(temp_char);
                                                }
                                                if (temp_char = xmlGetProp(child_node3, 
                                                                           (const xmlChar*)"y")){
                                                    event_p->y = atoi(temp_char);
                                                    xmlFree(temp_char);
                                                }
                                                event = g_new0(Event, 1);
                                                event->event_type = POSITION_ACTOR;
                                                event->event = event_p;
                                                event->number = number_actor_in_queue;
                                                list_of_event = g_slist_append(list_of_event, event);
                                            }
                                            /* changed Opacity of actor */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar*)"o")){
                                                event_o = g_new0(Event_o, 1);
                                                if (temp_char = xmlGetProp(child_node3, 
                                                                           (const xmlChar*)"o")){
                                                    event_o->o = atoi(temp_char);
                                                    xmlFree(temp_char);
                                                }
                                                event = g_new0(Event, 1);
                                                event->event_type = OPACITY_ACTOR;
                                                event->event = event_o;
                                                event->number = number_actor_in_queue;
                                                list_of_event = g_slist_append(list_of_event, event);
                                            }
                                            /* changed Scale of actor */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar*)"s")){
                                                event_s = g_new0(Event_s, 1);
                                                if (temp_char = xmlGetProp(child_node3,
                                                                           (const xmlChar*)"x")){
                                                    event_s->x = atof(temp_char);
                                                    xmlFree(temp_char);
                                                }
                                                if (temp_char = xmlGetProp(child_node3,
                                                                           (const xmlChar*)"y")){
                                                    event_s->y = atof(temp_char);
                                                    xmlFree(temp_char);
                                                }
                                                event = g_new0(Event, 1);
                                                event->event_type = SCALE_ACTOR;
                                                event->event = event_s;
                                                event->number = number_actor_in_queue;
                                                list_of_event = g_slist_append(list_of_event, event);
                                            }
                                            /* changed Rotation of actor */
                                            if(!xmlStrcmp(child_node3->name, (const xmlChar*)"rx")||
                                               !xmlStrcmp(child_node3->name, (const xmlChar*)"ry")||
                                               !xmlStrcmp(child_node3->name, (const xmlChar*)"rz")) {
                                                event_r = g_new0(Event_r, 1);
                                                if (temp_char = xmlGetProp(child_node3,
                                                                           (const xmlChar*)"x")){
                                                    event_r->x = atoi(temp_char);
                                                    xmlFree(temp_char);
                                                }
                                                if (temp_char = xmlGetProp(child_node3,
                                                                           (const xmlChar*)"y")){
                                                    event_r->y = atoi(temp_char);
                                                    xmlFree(temp_char);
                                                }
                                                if (temp_char = xmlGetProp(child_node3, (const xmlChar*)"z")){
                                                    event_r->x = atoi(temp_char);
                                                    xmlFree(temp_char);
                                                }
                                                if (temp_char = xmlGetProp(child_node3, 
                                                                           (const xmlChar*)"a")){
                                                    event_r->a = atof(temp_char);
                                                    xmlFree(temp_char);
                                                }

                                                if(!xmlStrcmp(child_node3->name, (const xmlChar*)"rx"))
                                                    event_r->d = HILDON_AA_X_AXIS;
                                                if(!xmlStrcmp(child_node3->name, (const xmlChar*)"ry"))
                                                    event_r->d = HILDON_AA_Y_AXIS;
                                                if(!xmlStrcmp(child_node3->name, (const xmlChar*)"rz"))
                                                    event_r->d = HILDON_AA_Z_AXIS;

                                                event = g_new0(Event, 1);
                                                event->event_type = ROTATE_ACTOR;
                                                event->event = event_r;
                                                event->number = number_actor_in_queue;
                                                list_of_event = g_slist_append(list_of_event, event);
                                            }

                                        }
                                        /*
                                        if (number_of_step && 
                                           !xmlStrcmp(child_node2->name, (const xmlChar*)"0")){
                                            list_of_event = g_slist_append(list_of_event, event);
                                            temp_list_of_event = list_of_event;
                                        }else{
                                            temp_list_of_event = g_slist_next(temp_list_of_event);
                                        }
                                        */
                                    }
                                    number_actor_in_queue ++;
                                }
                            }
                            g_hash_table_insert(icon_animation_hash, number_of_step, list_of_event);
                            fprintf(stderr, "Step %s %p\n", number_of_step, list_of_event);
                            xmlFree(number_of_step);
                        }
                    }
                }
                xmlFree(value);
            }
        }
        node = node->next;
    }
    setlocale(LC_NUMERIC, "");


#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
GHashTable*
parse_animation_file(const gchar *filename, const gchar *encoding){
    xmlDoc     *document = NULL;
    xmlNode    *root_node = NULL,
               *current_node = NULL;
    GHashTable *icons = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* check file accessibility */
    if(!access(filename, R_OK | F_OK)){
        document = xmlReadFile(filename, encoding, 0);
      if(document){
          root_node = xmlDocGetRootElement(document);
          current_node = root_node->children;
          icons = g_hash_table_new(g_str_hash, g_str_equal);
          parse_animation_of_icon(current_node, icons);
          xmlFreeDoc(document);
          return icons;
      } /* if(document) */
    } /* if(!access()) */
    return icons;
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
void
load_actor(SuperOH *oh, gchar *icon_name, gint width, gint height){

    GdkPixbuf  *pixbuf; 
    GtkWidget  *ha = NULL; 

    pixbuf = gdk_pixbuf_new_from_file_at_size (icon_name,
      (((oh->icon_size*100)/GIANT_ICON_SIZE) * width/100), 
      (((oh->icon_size*100)/GIANT_ICON_SIZE) * height/100), 
           NULL);

    if (pixbuf){ 
         oh->image = gtk_image_new_from_pixbuf (pixbuf);
         g_object_unref(G_OBJECT(pixbuf));
         oh->list_images = g_slist_append(oh->list_images, oh->image);
    }
    g_signal_connect(G_OBJECT(oh->image), "expose_event",
                                      G_CALLBACK(expose_event), pixbuf);
    ha = hildon_animation_actor_new();
    gtk_container_add (GTK_CONTAINER (ha), oh->image);
    g_object_set_data(
                    G_OBJECT(oh->image), "hildon_animation_actor", ha);
    hildon_animation_actor_set_opacity(HILDON_ANIMATION_ACTOR (ha), 0); 
}
/*******************************************************************************/
void
position_actor(SuperOH *oh, gint number, gint x, gint y, gboolean fullwindow){

    GtkWidget  *ha = NULL; 
    GtkWidget  *image = NULL; 
    gint allocationx = 0, allocationy = 0;
    gint i = 1;
    GSList      *list_temp = NULL;

    list_temp = oh->list_images;
    while(list_temp != NULL){ 
        if (i == number)
            break;
        i++;
        list_temp = g_slist_next(list_temp);
    }
    if (list_temp)
        image = list_temp->data;
    else
        return;
    ha = g_object_get_data(G_OBJECT(image), "hildon_animation_actor");
    if (ha){
        if (!fullwindow) { 
              hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha), 
              oh->icon_widget->allocation.x + 
              (((oh->icon_size*100)/GIANT_ICON_SIZE) * x/100) + allocationx, 
              oh->icon_widget->allocation.y + 
              (((oh->icon_size*100)/GIANT_ICON_SIZE) * y/100) + allocationy + SIZE_OF_WINDOWS_HEAD, 0);
        }else{
              hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha), 
              oh->icon_widget->allocation.x + 
              (((oh->icon_size*100)/GIANT_ICON_SIZE) * x/100) + allocationx, 
              oh->icon_widget->allocation.y + 
              (((oh->icon_size*100)/GIANT_ICON_SIZE) * y/100) + allocationy,
              0);
        }
    }
}
/*******************************************************************************/
void
opacity_actor(SuperOH *oh, gint number, gint opacity){

    GtkWidget   *ha = NULL; 
    GtkWidget   *image = NULL; 
    gint        i = 1;
    GSList      *list_temp = NULL;

    list_temp = oh->list_images;
    while(list_temp != NULL){ 
        if (i == number)
            break;
        i++;
        list_temp = g_slist_next(list_temp);
    }
    if (list_temp)
        image = list_temp->data;
    else
        return;
    ha = g_object_get_data(G_OBJECT(image), "hildon_animation_actor");
    if (ha){
        hildon_animation_actor_set_opacity(HILDON_ANIMATION_ACTOR (ha), opacity);
    }
}
/*******************************************************************************/
void
scale_actor(SuperOH *oh, gint number, gdouble x, gdouble y){

    GtkWidget   *ha = NULL; 
    GtkWidget   *image = NULL; 
    gint        i = 1;
    GSList      *list_temp = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
    list_temp = oh->list_images;
    while(list_temp != NULL){ 
        if (i == number)
            break;
        i++;
        list_temp = g_slist_next(list_temp);
    }
    if (list_temp)
        image = list_temp->data;
    else
        return;
    ha = g_object_get_data(G_OBJECT(image), "hildon_animation_actor");
    if (ha)
        hildon_animation_actor_set_scale(HILDON_ANIMATION_ACTOR (ha), x, y);
}
/*******************************************************************************/
void
rotation_actor(SuperOH *oh, gint number, gint direction, gdouble angle, gint x, gint y, gint z){

    GtkWidget   *ha = NULL; 
    GtkWidget   *image = NULL; 
    gint        i = 1;
    GSList      *list_temp = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
    list_temp = oh->list_images;
    while(list_temp != NULL){ 
        if (i == number)
            break;
        i++;
        list_temp = g_slist_next(list_temp);
    }
    if (list_temp)
        image = list_temp->data;
    else
        return;
    ha = g_object_get_data(G_OBJECT(image), "hildon_animation_actor");
    if (ha)
        hildon_animation_actor_set_rotation(HILDON_ANIMATION_ACTOR (ha), direction, angle, x, y, z);
}
/*******************************************************************************/

gboolean
choose_icon_timeline(SuperOH *oh)
{
    GHashTable  *icon_animation_hash = NULL;
    GSList      *list_of_event = NULL;
    GSList      *list_temp = NULL;
    Event       *event = NULL;
    Event_l     *event_l = NULL;
    Event_p     *event_p = NULL;
    Event_o     *event_o = NULL;
    Event_s     *event_s = NULL;
    Event_r     *event_r = NULL;
    GtkWidget   *ha = NULL; 
    gchar       count_buffer[10];
    gboolean    fullwindow; 
    GtkWidget   *window;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
    if (!oh->icon_name)
        return FALSE;
    icon_animation_hash = g_hash_table_lookup(app->animation_hash, oh->icon_name);
    if (icon_animation_hash){
        switch (oh->timeline){
            case 0: 
                    list_of_event = g_hash_table_lookup(icon_animation_hash, "0");
                    list_temp = list_of_event;
                    while(list_temp != NULL){
                        event = list_temp->data;
                        if (event && event->event_type == LOAD_ACTOR){
                            event_l = event->event;
                            if (event_l)
                                load_actor(oh, event_l->name, event_l->width, event_l->height);
                        }
                        list_temp = g_slist_next(list_temp);
                    }
                    break;
            case 1:
                    list_temp = oh->list_images;
                    while(list_temp != NULL){
                         ha = g_object_get_data(G_OBJECT(list_temp->data), "hildon_animation_actor");
                         hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), oh->window);
                         // Set anchor point to the actor center
                         hildon_animation_actor_set_anchor_from_gravity (HILDON_ANIMATION_ACTOR (ha),
                                                                                        HILDON_AA_NW_GRAVITY);
                         realize(ha);
                         gtk_widget_show_all (ha);
                         list_temp = g_slist_next(list_temp);
                    }
                    oh->runtime = g_timeout_add (oh->delay, choose_icon_timeline, oh); 
                    break;
            case 2:
                    g_source_remove(oh->runtime);
                    oh->runtime = g_timeout_add (50, choose_icon_timeline, oh);
                    break;
            default:
                    snprintf(count_buffer, sizeof(count_buffer) - 1, "%i", oh->timeline);
                    list_of_event = g_hash_table_lookup(icon_animation_hash, count_buffer);
                    if (list_of_event){
                        window = oh->window; 
                        if (window && (gdk_window_get_state(window->window) &  GDK_WINDOW_STATE_FULLSCREEN 
                           || !strcmp(gtk_widget_get_name(window), "OmweatherDesktopWidget")))
                            fullwindow = TRUE;
                        else
                            fullwindow = FALSE;
 
                        list_temp = list_of_event;
                        while(list_temp != NULL){
                            event = list_temp->data;
                            if (event && event->event_type == LOAD_ACTOR){
                                event_l = event->event;
                                if (event_l)
                                     load_actor(oh, event_l->name, event_l->width, event_l->height);
                            }
                            if (event && event->event_type == POSITION_ACTOR){
                                event_p = event->event;
                                if (event_p)
                                    position_actor(oh, event->number, event_p->x, event_p->y, fullwindow);
                            }
                            if (event && event->event_type == OPACITY_ACTOR){
                                event_o = event->event;
                                if (event_o)
                                    opacity_actor(oh, event->number, event_o->o);
                            }
                            if (event && event->event_type == SCALE_ACTOR){
                                event_s = event->event;
                                if (event_s)
                                    scale_actor(oh, event->number, event_s->x, event_s->y);
                            }
                            if (event && event->event_type == ROTATE_ACTOR){
                                event_r = event->event;
                                if (event_r)
                                    rotation_actor(oh, event->number, event_r->d, event_r->a, 
                                                       event_r->x, event_r->y, event_r->z);
                            }
                            list_temp = g_slist_next(list_temp);
                        }
                    }else
                        return FALSE;
        }
        oh->timeline ++;
    }
    return TRUE;
#if 0
    if (!strcmp(oh->icon_name,"0")){ icon0_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"1")){ icon1_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"2")){ icon2_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"3")){ icon3_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"4")){ icon4_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"5")){ icon5_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"6")){ icon6_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"7")){ icon7_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"8")){ icon8_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"9")){ icon9_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"10")){ icon10_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"11")){ icon11_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"12")){ icon12_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"13")){ icon13_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"14")){ icon14_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"15")){ icon15_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"16")){ icon16_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"17")){ icon17_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"18")){ icon18_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"19")){ icon19_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"20")){ icon20_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"21")){ icon21_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"22")){ icon22_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"23")){ icon23_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"24")){ icon24_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"25")){ icon25_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"26")){ icon26_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"27")){ icon27_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"28")){ icon28_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"29")){ icon29_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"30")){ icon30_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"31")){ icon31_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"32")){ icon32_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"33")){ icon33_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"34")){ icon34_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"35")){ icon35_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"36")){ icon36_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"37")){ icon37_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"38")){ icon38_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"39")){ icon39_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"40")){ icon40_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"41")){ icon41_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"42")){ icon42_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"43")){ icon43_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"44")){ icon44_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"45")){ icon45_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"46")){ icon46_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"47")){ icon47_timeline (oh); return; }
    if (!strcmp(oh->icon_name,"48")){ icon48_timeline (oh); return; }
#endif
}

/************************************************************************************************/
GtkWidget *
create_hildon_clutter_icon_animation(const char *icon_path, int icon_size, GSList **objects_list)
{
    SuperOH *oh;
    GError *error = NULL;
    GtkWidget  *clactor = NULL;
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
    if (!objects_list)
        return NULL;
    if (!app->animation_hash)
        return NULL;
    oh = g_new(SuperOH, 1);
    oh->timeline = 0;
    oh->image = NULL;
    oh->list_images = NULL;
    oh->icon_size = icon_size;

    memset(buffer, 0, sizeof(buffer));
    memset(icon_name, 0, sizeof(icon_name));
    icon_name[0] = icon_path[strlen(icon_path) - 6];
    if (icon_name[0] >= '0' && icon_name[0] <= '9')
        icon_name[1] = icon_path[strlen(icon_path) - 5];
    else
        icon_name[0] = icon_path[strlen(icon_path) - 5];

    oh->icon_name = g_strdup(icon_name);
    
    oh->icon_widget = gtk_vbox_new(FALSE, 0);

    gtk_widget_set_size_request (oh->icon_widget, icon_size, icon_size);

    sprintf(buffer, "icon_name_%s", icon_name);
    choose_icon_timeline(oh);
    *objects_list = g_slist_append(*objects_list, oh);
    gtk_widget_show_all(oh->icon_widget);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return oh->icon_widget;
}
/************************************************************************************************/
show_hildon_animation(GSList *clutter_objects, GtkWidget *window){
    GSList   *list_temp = NULL;
    SuperOH         *oh;
    GtkWidget  *clactor = NULL;
    GdkPixbuf *pixbuf;
    GError *error;
    error = NULL;
    GtkWidget *ha;
    gint i;
    gint one_delay = 1500;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!clutter_objects)
        return;

    i = 0;
    list_temp = clutter_objects;
    while(list_temp != NULL){
        oh = list_temp->data;
        if (oh->timeline <2){
            oh->window = window;
            oh->duration = 40;
            oh->delay = i * one_delay;
            choose_icon_timeline(oh);
            /* Start animation */
//            oh->runtime = g_timeout_add (50, icon0_timeline, oh);
            i++;
        }
        list_temp = g_slist_next(list_temp);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void free_clutter_objects_list(GSList **clutter_objects) {
    static GSList *list_temp = NULL;
    static GSList *list_temp_images = NULL;
    SuperOH *oh;
    GtkWidget  *clactor = NULL;
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
        g_source_remove(oh->runtime);
        list_temp_images = oh->list_images;
        while(list_temp_images != NULL){
            ha = g_object_get_data(G_OBJECT(list_temp_images->data), "hildon_animation_actor");
            gtk_widget_destroy(ha);
            list_temp_images = g_slist_next(list_temp_images);
        }
        if (oh->list_images)
            g_slist_free(oh->list_images);

        if (oh->icon_name)
            g_free(oh->icon_name);

        if (oh->icon_widget){
            gtk_widget_destroy(oh->icon_widget);
            oh->icon_widget = NULL;
        }
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
#if 0
/*******************************************************************************/
do_animation(SuperOH *oh, ClutterActor  *clactor, GtkWidget *ha, gboolean fullwindow)
{
    gdouble scale_x, scale_y, angle;
    gfloat rx, ry, rz;
    gint allocationx = 0, allocationy = 0;
    GtkWidget *parent = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!ha || !oh || !clactor)
        return;
    /* set position */
    /* Add offset for some parents widgets */
    parent = oh->icon_widget;
    while (!GTK_IS_WINDOW(parent = parent->parent))
    {
        //fprintf(stderr, "Name %s %i \n",gtk_widget_get_name(GTK_WIDGET(parent)), parent->allocation.y);     
        if (HILDON_IS_PANNABLE_AREA(parent)){
            allocationy =  allocationy + parent->allocation.y;
            allocationx =  allocationx + parent->allocation.x;
         }
    }
    if (fullwindow) 
        hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha),
                    oh->icon_widget->allocation.x + clutter_actor_get_x(clactor) + allocationx,
                    oh->icon_widget->allocation.y + clutter_actor_get_y(clactor) + allocationy + oh->duration , 0);
    else
        hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha),
                    oh->icon_widget->allocation.x + clutter_actor_get_x(clactor) + allocationx,
                    oh->icon_widget->allocation.y + clutter_actor_get_y(clactor) + allocationy +
                    SIZE_OF_WINDOWS_HEAD + oh->duration, 0);
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
do_animation1(SuperOH *oh, ClutterActor  *clactor, GtkWidget *ha, gboolean fullwindow)
{
    gdouble scale_x, scale_y, angle;
    gfloat rx, ry, rz;
    gint allocationx = 0, allocationy = 0;
    GtkWidget *parent = NULL;
//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif
    if (!ha || !oh || !clactor)
        return;
    /* set position */
    /* Add offset for some parents widgets */
    parent = oh->icon_widget;
    while (!GTK_IS_WINDOW(parent = parent->parent))
    {
        fprintf(stderr, "Name %s %i \n",gtk_widget_get_name(GTK_WIDGET(parent)), parent->allocation.y);     
        if (HILDON_IS_PANNABLE_AREA(parent)){
            allocationy =  allocationy + parent->allocation.y;
            allocationx =  allocationx + parent->allocation.x;
         }
    }
    if (fullwindow) 
        hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha),
                    oh->icon_widget->allocation.x + clutter_actor_get_x(clactor) + allocationx,
                    oh->icon_widget->allocation.y + clutter_actor_get_y(clactor) + allocationy + oh->duration*2, 0);
    else
        hildon_animation_actor_set_position_full (HILDON_ANIMATION_ACTOR (ha),
                    oh->icon_widget->allocation.x + clutter_actor_get_x(clactor) + allocationx,
                    oh->icon_widget->allocation.y + clutter_actor_get_y(clactor) + allocationy +
                    SIZE_OF_WINDOWS_HEAD + oh->duration*2, 0);
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
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
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
                do_animation1(oh, clactor, ha, fullwindow);
                fprintf(stderr,"ggggggggggggggg\n");
            }
        }else{
            clactor = oh->icon;
            ha = g_object_get_data(G_OBJECT(clactor), "hildon_animation_actor");
            do_animation1(oh, clactor, ha, fullwindow);
        }
    }else
        return FALSE;

    oh->duration--;
    if (oh->duration == 0)
        return FALSE;
    else
        return TRUE;
}
/
#endif
