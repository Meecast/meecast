/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
        icon =
            gtk_image_new_from_file(gtk_icon_info_get_filename(gtkicon));
        gtk_icon_info_free(gtkicon);
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
    GtkTreeSelection *list_selection = NULL;
    GtkCellRenderer *renderer = NULL;
    GtkTreeViewColumn *column = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* create the tree view model LIST */
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list));
/* make the list component single selectable */
    list_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    gtk_tree_selection_set_mode(list_selection, GTK_SELECTION_SINGLE);
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
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_NONE);
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
/* get icon set names */
int create_icon_set_list(gchar *dir_path, GSList ** store, gchar *type){
    Dirent	*dp;
    DIR		*dir_fd;
    gint	sets_number = 0,
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
            if(dp->d_type == t){
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
