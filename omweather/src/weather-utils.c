/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2010 Vlad Vasiliev
 * Copyright (C) 2006-2010 Pavel Fialko
 * Copyright (C) 2008 Andrew Olmsted
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
/*******************************************************************************/
#include "weather-utils.h"
#include "weather-common.h"
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
#if defined (BSD) && !_POSIX_SOURCE
#include <sys/dir.h>
typedef struct dirent Dirent;
#else
#include <dirent.h>
#include <linux/fs.h>
typedef struct dirent Dirent;
#endif
/*******************************************************************************/
/* Hack for Maemo SDK 2.0 */
#ifndef DT_DIR
#define DT_DIR 4
#endif
/*******************************************************************************/
float convert_wind_units(int to, float value) {
    float result = value;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    switch (to) {
    default:
    case METERS_S:
        result *= 10.0f / 36.0f;
    break;
    case KILOMETERS_H:
        result *= 1.0f;
    break;
    case MILES_H:
        result /= 1.609344f;
    break;
    case BEAUFORT_SCALE:
        if(value < 1)
            return 0.0f;
        if(value >= 1.1f && value <= 5.5f)
            return 1.0f; 
        if(value >= 5.6f && value <= 11.0f)
            return 2.0f;
        if(value >= 12.0f && value <= 19.0f)
            return 3.0f;
        if(value >= 20.0f && value <= 28.0f)
            return 4.0f;
        if(value >= 29.0f && value <= 38.0f)
            return 5.0f;
        if(value >= 39.0f && value <= 49.0f)
            return 6.0f;
        if(value >= 50.0f && value <= 61.0f)
            return 7.0f;
        if(value >= 62.0f && value <= 74.0f)
            return 8.0f;
        if(value >= 75.0f && value <= 88.0f)
            return 9.0f;
        if(value >= 89.0f && value <= 102.0f)
            return 10.0f;
        if(value >= 103.0f && value <= 117.0f)
            return 11.0f;
        if(value >= 118.0f)
            return 12.0f;
    break;
    }
    return result;
}

/*******************************************************************************/
void
set_font(GtkWidget * widget, const gchar * description, const gint size) {
    PangoFontDescription *pfd = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!widget)
        return;
    if (!description) {
        pfd =
            pango_font_description_copy(pango_context_get_font_description
                                        (gtk_widget_get_pango_context
                                         (widget)));
        if (size > 0)
            pango_font_description_set_size(pfd, size * PANGO_SCALE);
    } else {
        pfd = pango_font_description_from_string(description);
        if (size > 0)
            pango_font_description_set_size(pfd,
                                            pango_font_description_get_size
                                            (pfd) * size);
    }
    gtk_widget_modify_font(GTK_WIDGET(widget), NULL);   /* this function is leaking */
    gtk_widget_modify_font(GTK_WIDGET(widget), pfd);    /* this function is leaking */

    pango_font_description_free(pfd);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}

/*******************************************************************************/
void
set_font_color(GtkWidget * widget, guint16 red, guint16 green,
               guint16 blue) {
    PangoAttribute *attr;
    PangoAttrList *attrs = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!widget)
        return;

    attrs = pango_attr_list_new();
    attr = pango_attr_foreground_new(red, green, blue);
    attr->start_index = 0;
    attr->end_index = G_MAXUINT;
    pango_attr_list_insert(attrs, attr);
    /* Set the attributes */
    g_object_set(widget, "attributes", attrs, NULL);
    pango_attr_list_unref(attrs);
}

/*******************************************************************************/
/* Convert Celsius temperature to Farenhait temperature */
float c2f(float temp) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    return (temp * 1.8f) + 32.0f;
}

/*******************************************************************************/
/* convert pressure */
float mb2inch(float pressure) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    return (pressure / 1000.0f * 100000.0f / (3.386389f * 1000.0f));
}

/*******************************************************************************/
float mb2mm(float pressure) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    return (pressure / 1000.0f * 100000.0f * 7.5006f * 0.001f);
}

/*******************************************************************************/
void swap_temperature(int *hi, int *low) {
    int tmp;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    tmp = *hi;
    *hi = *low;
    *low = tmp;
}

/*******************************************************************************/
gboolean draw_label_gradient(GtkWidget * widget, GdkEventExpose * event) {
    GdkDrawable *drawable;
    gint x_offset, y_offset;
    cairo_pattern_t *pattern;
    cairo_t *cr;
    gint x, y, width, height;

    gdk_window_get_internal_paint_info(widget->window, &drawable,
                                       &x_offset, &y_offset);

    cr = gdk_cairo_create(drawable);

    width = event->area.width + x_offset;
    height = event->area.height + y_offset;
    x = event->area.x - x_offset;
    y = event->area.y - y_offset;

    pattern = cairo_pattern_create_linear(0, y, 0, y + height);

    cairo_pattern_add_color_stop_rgb(pattern, 0.0, 0.8, 0.8, 0.8);
    cairo_pattern_add_color_stop_rgb(pattern, 1.0, 1.0, 1.0, 1.0);
    cairo_set_source(cr, pattern);

    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x + width, y);
    cairo_line_to(cr, x + width, y + height);
    cairo_line_to(cr, x, y + height);
    cairo_line_to(cr, x, y);

    cairo_fill(cr);
    cairo_pattern_destroy(pattern);
    return FALSE;
}

/*******************************************************************************/
gboolean draw_top_gradient(GtkWidget * widget, GdkEventExpose * event) {
    GdkDrawable *drawable;
    gint x_offset, y_offset;
    cairo_pattern_t *pattern;
    cairo_t *cr;
    gint x, y, width, height;

    gdk_window_get_internal_paint_info(widget->window, &drawable,
                                       &x_offset, &y_offset);

    cr = gdk_cairo_create(drawable);

    width = event->area.width + x_offset;
    height = event->area.height + y_offset;
    x = event->area.x - x_offset;
    y = event->area.y - y_offset;

    pattern = cairo_pattern_create_linear(0, y, 0, y + height);

    cairo_pattern_add_color_stop_rgb(pattern, 0.0, 1.0, 1.0, 1.0);
    cairo_pattern_add_color_stop_rgb(pattern, 0.05, 0.8, 0.8, 0.8);
    cairo_pattern_add_color_stop_rgb(pattern, 1.0, 0.8, 0.8, 0.8);
    cairo_set_source(cr, pattern);

    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x + width, y);
    cairo_line_to(cr, x + width, y + height);
    cairo_line_to(cr, x, y + height);
    cairo_line_to(cr, x, y);

    cairo_fill(cr);
    cairo_pattern_destroy(pattern);
    return FALSE;
}

/*******************************************************************************/
gboolean draw_bottom_gradient(GtkWidget * widget, GdkEventExpose * event) {
    GdkDrawable *drawable;
    gint x_offset, y_offset;
    cairo_pattern_t *pattern;
    cairo_t *cr;
    gint x, y, width, height;

    gdk_window_get_internal_paint_info(widget->window, &drawable,
                                       &x_offset, &y_offset);

    cr = gdk_cairo_create(drawable);

    width = event->area.width + x_offset;
    height = event->area.height + y_offset;
    x = event->area.x - x_offset;
    y = event->area.y - y_offset;

    pattern = cairo_pattern_create_linear(0, y, 0, y + height);

    cairo_pattern_add_color_stop_rgb(pattern, 0.0, 0.8, 0.8, 0.8);
    /*cairo_pattern_add_color_stop_rgb(pattern,0.9,0.8,0.8,0.8); */
    cairo_pattern_add_color_stop_rgb(pattern, 1.0, 0.47, 0.47, 0.47);
    cairo_set_source(cr, pattern);

    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x + width, y);
    cairo_line_to(cr, x + width, y + height);
    cairo_line_to(cr, x, y + height);
    cairo_line_to(cr, x, y);

    cairo_fill(cr);
    cairo_pattern_destroy(pattern);
    return FALSE;
}

/*******************************************************************************/
void set_background_color(GtkWidget * widget, GdkColor * bgc) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* undo previos changes */
    gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, NULL);
/* set one color for all states of widget */
    gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, bgc);
}

/*******************************************************************************/
GtkWidget *lookup_widget(GtkWidget * widget, const gchar * widget_name) {
    GtkWidget *found_widget = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    found_widget = (GtkWidget *) g_object_get_data(G_OBJECT(widget),
                                                   widget_name);
    if (!found_widget)
        g_warning("Widget not found: %s", widget_name);
    return found_widget;
}

/*******************************************************************************/
GtkWidget *create_tool_item(const char *path, const char *image_name,
                            int image_size) {
    GtkToolItem *button = NULL;
    GtkWidget *icon = NULL;
    GdkPixbuf *icon_buffer = NULL;
    gchar buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* prepare icon */
    if (path) {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer) - 1, "%s/%s.png", path,
                 image_name);
        icon_buffer =
            gdk_pixbuf_new_from_file_at_size(buffer, image_size,
                                             image_size, NULL);
        if (icon_buffer) {
            icon = gtk_image_new_from_pixbuf(icon_buffer);
            g_object_unref(G_OBJECT(icon_buffer));
        }
    }
    button = gtk_tool_button_new(icon, image_name);
    gtk_tool_item_set_expand(button, TRUE);

    return GTK_WIDGET(button);
}

/*******************************************************************************/
GtkWidget *create_button_with_image(const char *path,
                                    const char *image_name,
                                    int image_size, gboolean with_border,
                                    gboolean toggled) {
    GtkIconInfo *gtkicon = NULL;
    GtkWidget *button = NULL, *icon = NULL;
    GdkPixbuf *icon_buffer = NULL;
    gchar buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* prepare icon */
    if (path) {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer) - 1, "%s/%s.png", path,
                 image_name);
        icon_buffer =
            gdk_pixbuf_new_from_file_at_size(buffer, image_size,
                                             image_size, NULL);
        if (icon_buffer) {
            icon = gtk_image_new_from_pixbuf(icon_buffer);
            g_object_unref(G_OBJECT(icon_buffer));
        }
    } else {
        gtkicon =
            gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(),
                                       image_name, image_size, 0);
        if (gtkicon){
            icon =
                 gtk_image_new_from_file(gtk_icon_info_get_filename(gtkicon));
            gtk_icon_info_free(gtkicon);
        }else
            icon = NULL;
    }
    if (with_border) {
        if (toggled) {
            button = gtk_radio_button_new(NULL);
            gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(button), FALSE);
        } else
            button = gtk_button_new();
        gtk_button_set_focus_on_click(GTK_BUTTON(button), FALSE);
        if(icon){
            #ifdef OS2008
               gtk_button_set_image(GTK_BUTTON(button), icon);
            #else
               gtk_container_add (GTK_CONTAINER (button), icon);
            #endif
        }
        gtk_button_set_focus_on_click(GTK_BUTTON(button), FALSE);
    } else {
        button = gtk_event_box_new();
        gtk_event_box_set_visible_window(GTK_EVENT_BOX(button), FALSE);
        if (icon)
            gtk_container_add(GTK_CONTAINER(button), icon);
    }
    gtk_widget_set_events(button, GDK_BUTTON_RELEASE_MASK);

    return button;
}

/*******************************************************************************/
GtkWidget *create_tree_view(GtkListStore * list) {
    GtkWidget *tree_view = NULL;
#if !defined OS2009
    GtkTreeSelection *list_selection = NULL;
#endif
    GtkCellRenderer *renderer = NULL;
    GtkTreeViewColumn *column = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* create the tree view model LIST */
#if defined OS2009
    tree_view = hildon_gtk_tree_view_new_with_model(HILDON_UI_MODE_NORMAL, GTK_TREE_MODEL(list));
#else
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list));
#endif
/* make the list component single selectable */
#if !defined OS2009 
    list_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    gtk_tree_selection_set_mode(list_selection, GTK_SELECTION_SINGLE);
#endif
/* add name column to the view */
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
    g_object_set(G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    gtk_tree_view_column_set_expand(column, TRUE);

    gtk_tree_view_column_pack_start(column, renderer, TRUE);
    gtk_tree_view_column_set_attributes(column, renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
/* return widget to caller */
    return tree_view;
}

/*******************************************************************************/
GtkWidget *create_scrolled_window_with_text(const char *text,
                                            GtkJustification
                                            justification) {

    GtkWidget *text_view = NULL, *scrolled_window = NULL;
    GtkTextBuffer *text_buffer = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    text_view = gtk_text_view_new();
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer), text, -1);
    /* set params of text view */
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_justification(GTK_TEXT_VIEW(text_view),
                                    justification);
    gtk_text_view_set_overwrite(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    /* scrolled window */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW
                                        (scrolled_window), GTK_SHADOW_OUT);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 700, 250);
    /* pack childs to the scrolled window */
    gtk_container_add(GTK_CONTAINER(scrolled_window),
                      GTK_WIDGET(text_view));
    return scrolled_window;
}
/*******************************************************************************/
/* Free icon set list */
void
free_icon_set_list(GSList *iconset)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    GSList *tmp = NULL;
    if (iconset) {
        tmp = (GSList *) iconset;
        while (tmp) {
            if (tmp->data)
                g_free((gchar *) (tmp->data));
            tmp = g_slist_next(tmp);
        }
        g_slist_free (iconset);
    }
}
/*******************************************************************************/
/* get icon set names */
int 
create_icon_set_list(gchar *dir_path, GSList ** store, gchar *type){
    Dirent  *dp;
    DIR     *dir_fd;
    gint    sets_number = 0,
            t = DT_DIR;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!strcmp(type, "dir"))
        t = DT_DIR;
    if(!strcmp(type, "file"))
        t = DT_REG;
    dir_fd = opendir(dir_path);
    if(dir_fd){
        while((dp = readdir(dir_fd))){
            if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
                continue;
            if(dp->d_type == t || dp->d_type == DT_LNK){
                *store = g_slist_append(*store, g_strdup(dp->d_name));
                sets_number++;
            }
        }
        closedir(dir_fd);
    }
    else{
        *store = g_slist_append(*store, app->config->icon_set);
        sets_number++;
    }
    return sets_number;
}
/*******************************************************************************/
GtkWidget *
create_icon_widget(GdkPixbuf *icon_buffer, const char *icon_path, int icon_size, GSList **objects_list)
{
    GtkWidget *icon_widget = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

#ifdef CLUTTER
    icon_widget = create_clutter_icon_animation(icon_buffer, icon_path, icon_size, objects_list);
    if (!icon_widget){
        icon_widget = gtk_image_new_from_pixbuf(icon_buffer);
        g_object_unref(G_OBJECT(icon_buffer));
    }
#else

#ifdef HILDONANIMATION
    if (app->config->animation){
#ifndef APPLICATION
        if (app->applet_visible && !app->notanimation)
#endif
            icon_widget = create_hildon_clutter_icon_animation(icon_path, icon_size, objects_list);
            
    }
    if (!icon_widget)
        icon_widget = gtk_image_new_from_pixbuf(icon_buffer);
    g_object_unref(G_OBJECT(icon_buffer));
#else
    icon_widget = gtk_image_new_from_pixbuf(icon_buffer);
    if (icon_buffer)
        g_object_unref(G_OBJECT(icon_buffer));
#endif
#endif
    return icon_widget;
}
/******************************************************************************/
void
update_icons_set_base(const char *icon_set_name){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gchar buffer[1024];
    int fd = -1;

    /* check current iconset directory */
    if (!icon_set_name && app && app->config->icons_set_base && (fd = open(app->config->icons_set_base, O_RDONLY)) != -1) {
        close(fd);
        return;
    }

    if(app->config->icons_set_base){
         g_free(app->config->icons_set_base);
         app->config->icons_set_base = NULL;
    }
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s%s/", ICONS_PATH, icon_set_name);
    if (!icon_set_name || (fd = open(buffer, O_RDONLY)) == -1) {
        snprintf(buffer, sizeof(buffer) - 1, "%s%s/", ICONS_PATH, "Glance");
        if (app->config->icon_set){
            g_free(app->config->icon_set);
            app->config->icon_set = g_strdup("Glance");
        }
    }else
        close(fd);
    
    app->config->icons_set_base = g_strdup(buffer);
    /* Create icon hash */

#ifdef HILDONANIMATION 
    /* Fix me Free memory for previous hash */
    if (app->animation_hash)
        app->animation_hash = clear_animation_hash(app->animation_hash);

    if (app->config->animation){
        snprintf(buffer,sizeof(buffer) - 1, "%sanimation.xml", app->config->icons_set_base);
        app->animation_hash = parse_animation_file(buffer,"UTF-8");
    }
#endif

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/******************************************************************************/
/* Fullscreen/Unfullscreen window */
void
change_state_window(GtkWidget *window){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (window){
        if (gdk_window_get_state(window->window) &  GDK_WINDOW_STATE_FULLSCREEN)
            gtk_window_unfullscreen (GTK_WINDOW(window));
        else
            gtk_window_fullscreen (GTK_WINDOW(window));
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
gint
choose_wind_direction(gchar *buffer)
{
    /* if buffer is null or CALM */
    if (!buffer || 
        (buffer[0] == 'C' && buffer[1] == 'A'))
        return UNKNOWN_DIRECTION;
    if (buffer[0] == 'N'){
        if (strlen(buffer) > 1){
            if (buffer[1] == 'W')
                return TO_SOUTH_EAST;
            else{
               if (buffer[1] == 'E')
                    return TO_SOUTH_WEST;
               else
                    return TO_SOUTH;
            }
        }else   
            return TO_SOUTH;
    }else{
        if (buffer[0] == 'S'){
            if (strlen(buffer) > 1){
                if (buffer[1] == 'W')
                    return TO_NORTH_EAST;
                else{
                    if (buffer[1] == 'E')
                        return TO_NORTH_WEST;
                    else
                        return TO_NORTH;
                }
            }else
                    return TO_NORTH;
        }else
        if (buffer[0] == 'W')
            return TO_EAST;
        if (buffer[0] == 'E')
            return TO_WEST;
    }
    return UNKNOWN_DIRECTION;
}

/*******************************************************************************/
GtkWidget*
create_button_with_2_line_text(const gchar *first_line_text, const gchar *second_line_text,
                                const gint first_line_text_size, const gint second_line_text_size){
    GtkWidget
#if !defined OS2009
              *first_line = NULL,
              *second_line  = NULL,
              *vertical_box  = NULL,
#endif
              *button = NULL;
#if defined OS2009
  button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT | HILDON_SIZE_AUTO_WIDTH,
                     HILDON_BUTTON_ARRANGEMENT_VERTICAL,
                     first_line_text,
                     second_line_text);
  hildon_button_set_alignment(HILDON_BUTTON(button),0,0.5,0,1);
  hildon_button_set_style(HILDON_BUTTON(button), HILDON_BUTTON_STYLE_PICKER);
#else
    button = gtk_button_new();

    first_line = gtk_label_new(first_line_text);
    set_font(first_line, NULL, first_line_text_size);
    gtk_widget_show(first_line);

    second_line = gtk_label_new(second_line_text);
    set_font(second_line, NULL, second_line_text_size);
    gtk_widget_show(second_line);

    vertical_box = gtk_vbox_new(FALSE, 2);
    gtk_widget_show(vertical_box);

    g_object_set_data(G_OBJECT(button), "vbox", (gpointer)vertical_box);
    g_object_set_data(G_OBJECT(button), "label", (gpointer)second_line);

    gtk_box_pack_start(GTK_BOX(vertical_box), first_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vertical_box), second_line, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(button), vertical_box);


#endif
    gtk_widget_show(button);
    return button;
}
/*******************************************************************************/
#ifdef ENABLE_BROWSER_INTERFACE
gboolean
browser_url(gchar *url) {

  osso_return_t ret;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

  ret = osso_rpc_run_with_defaults(app->osso, "osso_browser",
          OSSO_BROWSER_OPEN_NEW_WINDOW_REQ, NULL,
          DBUS_TYPE_STRING, url,
          DBUS_TYPE_BOOLEAN, FALSE, DBUS_TYPE_INVALID);

  if(ret != OSSO_OK){
    fprintf(stderr,"Unable to display URL in browser!");
    return FALSE;
  }

  return TRUE;
}
#endif
/*******************************************************************************/
/* For debugging   */
void
write_log(char *string){
#ifndef RELEASE
    FILE    *filelog;
    filelog = fopen("/tmp/omw.log","a+");
    if(!filelog)
        return;
    fprintf(filelog,"\n%s\n", string);
    fflush(filelog);
    fclose(filelog);
#endif
}
/*******************************************************************************/
/* Reload omweather's plugin */
void
reload_omw_plugin(void)
{
    FILE    *file_in;
    FILE    *file_out;
    gchar buffer[2048];

    if (app->settings_window){
        gtk_widget_destroy(app->settings_window);
        app->settings_window = NULL;
    }
    if(app->popup_window){
       gtk_widget_destroy(app->popup_window);
       app->popup_window = NULL;
    }
    hildon_banner_show_information(NULL, NULL,
                            _("OMWeather is reloading"));
    if (!fork()) {
        file_in = fopen("/home/user/.config/hildon-desktop/home.plugins","r");
        file_out = fopen("/tmp/home.plugins","w");
        if (!file_in || !file_out)
            exit(1);
        while (!feof(file_in)) {
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer) - 1, file_in);
            if (buffer && strcmp(buffer, "[omweather-home.desktop-0]\n") &&
                strcmp(buffer, "X-Desktop-File=/usr/share/applications/hildon-home/omweather-home.desktop\n"))
                fputs(buffer, file_out);
        }
        fclose(file_out);
        fclose(file_in);
        file_in = fopen("/tmp/home.plugins","r");
        file_out = fopen("/home/user/.config/hildon-desktop/home.plugins","w");
        if (!file_in || !file_out)
            exit(1);
        while (!feof(file_in)) {
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer) - 1, file_in);
            fputs(buffer, file_out);
        }
        fclose(file_out);
        fclose(file_in);
        sleep (2);
        file_in = fopen("/tmp/home.plugins","r");
        file_out = fopen("/home/user/.config/hildon-desktop/home.plugins","w");
        if (!file_in || !file_out)
            exit(1);
        while (!feof(file_in)) {
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer) - 1, file_in);
            fputs(buffer, file_out);
        }
        fputs("\n", file_out);
        fputs("[omweather-home.desktop-0]\n",file_out);
        fputs("X-Desktop-File=/usr/share/applications/hildon-home/omweather-home.desktop\n",file_out);
        fclose(file_out);
        fclose(file_in);
        exit(1);
      }
}
