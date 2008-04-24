/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
/*******************************************************************************/
float convert_wind_units(int to, float value){
    float	result = value;
    switch(to){
	    default:
	    case METERS_S: result *= 10.0f / 36.0f; break;
/*	    case KILOMETERS_S: result /= 3600.0f; break;
 *
	    case MILES_S: result /= (1.609344f * 3600.0f); break;
	    case METERS_H: result *= 1000.0f; break;
*/	    case KILOMETERS_H: result *= 1.0f; break;
	    case MILES_H: result /= 1.609344f; break;
	}
    return result;
}
/*******************************************************************************/
void set_font_size(GtkWidget *widget, char font_size){
    PangoFontDescription *pfd = NULL;
#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
#endif
    if(!widget)
	return;
    pfd = pango_font_description_copy( 
            pango_context_get_font_description(gtk_widget_get_pango_context(widget)));
    pango_font_description_set_absolute_size(pfd, font_size * PANGO_SCALE);	    
    gtk_widget_modify_font(GTK_WIDGET(widget), NULL);   /* this function is leaking */
    gtk_widget_modify_font(GTK_WIDGET(widget), pfd);   /* this function is leaking */
    pango_font_description_free(pfd);
}
/*******************************************************************************/
void set_font_color(GtkWidget *widget, guint16 red, guint16 green, guint16 blue){
    PangoAttribute	*attr;
    PangoAttrList	*attrs = NULL;

    if(!widget)
	return;

    attrs = pango_attr_list_new();
    attr = pango_attr_foreground_new(red,green,blue);
    attr->start_index = 0;
    attr->end_index = G_MAXUINT;
    pango_attr_list_insert(attrs, attr);
    /* Set the attributes */
    g_object_set(widget, "attributes", attrs, NULL);
    pango_attr_list_unref(attrs);
}
/*******************************************************************************/
/* Convert Celsius temperature to Farenhait temperature */
int c2f(int temp){
    return (temp * 1.8f ) + 32;
}
/*******************************************************************************/
void swap_temperature(int *hi, int *low){
    int tmp;
    
    tmp = *hi; *hi = *low; *low = tmp;
}
/*******************************************************************************/
void set_background_color(GtkWidget *widget, GdkColor *bgc){
/* undo previos changes */
    gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, NULL);
/* set one color for all states of widget */
    gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, bgc);
}
/*******************************************************************************/
GtkWidget* lookup_widget(GtkWidget* widget, const gchar* widget_name){
    GtkWidget	*found_widget = NULL;

    found_widget = (GtkWidget*) g_object_get_data(G_OBJECT(widget),
                                                 widget_name);
    if(!found_widget)
	g_warning("Widget not found: %s", widget_name);
    return found_widget;
}
/*******************************************************************************/
GtkWidget* create_button_with_image(const char *path, const char *image_name,
					int image_size, gboolean with_border){
    GtkIconInfo	*gtkicon = NULL;
    GtkWidget	*button = NULL,
		*icon = NULL;
    GdkPixbuf   *icon_buffer = NULL;
    gchar	buffer[512];

    /* prepare icon */
    if(path){
	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer) - 1, "%s/%s.png",
		    path, image_name);
	icon_buffer = gdk_pixbuf_new_from_file_at_size(buffer, image_size,
							image_size, NULL);
	if(icon_buffer){
	    icon = gtk_image_new_from_pixbuf(icon_buffer);
	    g_object_unref(G_OBJECT(icon_buffer));
	}
    }
    else{
	gtkicon = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(),
        	                    	image_name, image_size, 0);
	icon = gtk_image_new_from_file(gtk_icon_info_get_filename(gtkicon));
	gtk_icon_info_free(gtkicon);
    }
    if(with_border){
	button = gtk_button_new();
	gtk_button_set_focus_on_click(GTK_BUTTON(button), FALSE);
	if(icon)
	    gtk_button_set_image(GTK_BUTTON(button), icon);
	gtk_button_set_focus_on_click(GTK_BUTTON(button), FALSE);
    }
    else{
	button = gtk_event_box_new();
	if(icon)
	    gtk_container_add(GTK_CONTAINER(button), icon);
    }
	gtk_widget_set_events(button, GDK_BUTTON_RELEASE_MASK | GDK_BUTTON_PRESS_MASK);

    return button;
}
/*******************************************************************************/
