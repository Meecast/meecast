/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 * Copyryght (C) 2008 Andrew Olmsted 
 *	for the semi-transparency and coloured backgrounds code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 * 
 * Parts of this file are based on:
 * desktop-plugin-example - Example Desktop Plugin with real transparency
 * Copyright (C) 2008  Tommi Saviranta  <wnd@iki.fi>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses>.
 */
/*******************************************************************************/
#include "weather-applet-expose.h"
#include <values.h>
#include <cairo.h>
/*******************************************************************************/
#if defined OS2008  && !defined (APPLICATION)
gboolean expose_parent(GtkWidget * widget, GdkEventExpose * event) {

    OMWeather *plugin = OMWEATHER(widget);
    GdkDrawable *drawable;
    gint x_offset, y_offset;
    XRenderColor color;
    Picture picture;
    XserverRegion region;
    cairo_t *cr;
    gint radius = 0;
    gint width, height, x, y;

    if (GTK_WIDGET_DRAWABLE(widget) == FALSE) {
        return FALSE;
    }

   gtk_widget_set_size_request(GTK_WIDGET(widget), -1, -1);
    gdk_window_get_internal_paint_info(widget->window, &drawable,
                                       &x_offset, &y_offset);


    picture = hildon_desktop_picture_from_drawable(drawable);

    if (picture == None) {
        return FALSE;
    }

    plugin->clip.x = event->area.x - x_offset;
    plugin->clip.y = event->area.y - y_offset;
    plugin->clip.width = event->area.width + x_offset;
    plugin->clip.height = event->area.height + y_offset;

    region = XFixesCreateRegion(GDK_DISPLAY(), &plugin->clip, 1);

    XFixesSetPictureClipRegion(GDK_DISPLAY(), picture, 0, 0, region);


    color.red = color.blue = color.green = 0;
    color.alpha = 0;

    XRenderFillRectangle(GDK_DISPLAY(), PictOpSrc, picture, &color,
                             0, 0,
                             widget->allocation.width,
                             widget->allocation.height);

    if (app->config->icons_layout < PRESET_NOW){

        radius = app->config->corner_radius;
        cr = gdk_cairo_create(drawable);
        cairo_set_source_rgba(cr,
                              (double)app->config->background_color.red /
                              (MAXSHORT * 2 + 1),
                              (double)app->config->background_color.green /
                              (MAXSHORT * 2 + 1),
                              (double)app->config->background_color.blue /
                              (MAXSHORT * 2 + 1),
                              (double)app->config->alpha_comp / 100);

        width = plugin->clip.width;
        height = plugin->clip.height;
        x = plugin->clip.x;
        y = plugin->clip.y;

        if ((radius > height / 2) || (radius > width / 2)) {
            if (width < height) {
                radius = width / 2 - 1;
            } else {
                radius = height / 2 - 2;
            }
        }
      
        cairo_move_to(cr, x + radius, y);
        cairo_line_to(cr, x + width - radius, y);
        cairo_curve_to(cr, x + width - radius, y, x + width, y, x + width,
                       y + radius);
        cairo_line_to(cr, x + width, y + height - radius);
        cairo_curve_to(cr, x + width, y + height - radius, x + width,
                       y + height, x + width - radius, y + height);
        cairo_line_to(cr, x + radius, y + height);
        cairo_curve_to(cr, x + radius, y + height, x, y + height, x,
                       y + height - radius);
        cairo_line_to(cr, x, y + radius);
        cairo_curve_to(cr, x, y + radius, x, y, x + radius, y);

        cairo_fill(cr);

        XFixesDestroyRegion(GDK_DISPLAY(), region);
        XRenderFreePicture(GDK_DISPLAY(), picture);

        cairo_destroy(cr);
    }
    if (plugin->queueRefresh) {
        redraw_home_window(TRUE);
        plugin->queueRefresh = FALSE;
    }

     
        
    return
        GTK_WIDGET_CLASS(g_type_class_peek_parent
                         (GTK_FRAME_GET_CLASS(widget)))->expose_event
        (widget, event);
}
#endif
/*******************************************************************************/
#if defined OS2009  && !defined (APPLICATION)
void
omweather_plugin_realize (GtkWidget *widget)
{
    GdkScreen *screen;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    screen = gtk_widget_get_screen (widget);
    gtk_widget_set_colormap (widget,
                           gdk_screen_get_rgba_colormap (screen));

    gtk_widget_set_app_paintable (widget,
                                TRUE);

    GTK_WIDGET_CLASS(g_type_class_peek_parent
                         (GTK_FRAME_GET_CLASS(widget)))->realize(widget);
}

gboolean
omweather_plugin_expose_event(GtkWidget * widget, GdkEventExpose * event) {

    cairo_t *cr;
    gint radius = 0;
    gint width, height, x, y;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if (GTK_WIDGET_DRAWABLE(widget) == FALSE){
        return FALSE;
    }

    /* Create cairo context */
    cr = gdk_cairo_create (GDK_DRAWABLE (widget->window));
    gdk_cairo_region (cr, event->region);
    cairo_clip (cr);

    /* Draw alpha background */
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.0);
    cairo_paint (cr);

    /* Free context */
    cairo_destroy (cr);

    radius = app->config->corner_radius;
    cr = gdk_cairo_create(GDK_DRAWABLE (widget->window));
    cairo_set_source_rgba(cr,
                          (double)app->config->background_color.red /
                          (MAXSHORT * 2 + 1),
                          (double)app->config->background_color.green /
                          (MAXSHORT * 2 + 1),
                          (double)app->config->background_color.blue /
                          (MAXSHORT * 2 + 1),
                          (double)app->config->alpha_comp / 100);

    width = event->area.width;
    height = event->area.height;
    x = event->area.x;
    y = event->area.y;

    if ((radius > height / 2) || (radius > width / 2)) {
        if (width < height) {
            radius = width / 2 - 1;
        } else {
            radius = height / 2 - 2;
        }
    }

    cairo_move_to(cr, x + radius, y);
    cairo_line_to(cr, x + width - radius, y);
    cairo_curve_to(cr, x + width - radius, y, x + width, y, x + width,
                   y + radius);
    cairo_line_to(cr, x + width, y + height - radius);
    cairo_curve_to(cr, x + width, y + height - radius, x + width,
                   y + height, x + width - radius, y + height);
    cairo_line_to(cr, x + radius, y + height);
    cairo_curve_to(cr, x + radius, y + height, x, y + height, x,
                   y + height - radius);
    cairo_line_to(cr, x, y + radius);
    cairo_curve_to(cr, x, y + radius, x, y, x + radius, y);

    /* Draw alpha background */
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);

    cairo_fill(cr);
    cairo_destroy(cr);
    return GTK_WIDGET_CLASS(g_type_class_peek_parent
                         (GTK_FRAME_GET_CLASS(widget)))->expose_event(widget, event);

}
#endif
