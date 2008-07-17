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
#ifdef OS2008
/*******************************************************************************/
static gboolean launcher_changed(OMWeather *applet){
    gtk_widget_queue_draw(GTK_WIDGET(applet));
    return FALSE;
}
/*******************************************************************************/
gboolean expose_parent(GtkWidget *widget, GdkEventExpose *event){

    OMWeather		*plugin = OMWEATHER(widget);
    GdkDrawable		*drawable;
    gint		x_offset,
			y_offset;
    XRenderColor	color;
    Picture		picture;
    Picture		grip,mask;
    gint		width,height;
    XserverRegion	region;

	if (GTK_WIDGET_DRAWABLE(widget) == FALSE) {
		return FALSE;
	}
	gtk_widget_set_size_request (GTK_WIDGET(widget), -1, -1);
	gdk_window_get_internal_paint_info(widget->window, &drawable,
			&x_offset, &y_offset);
	

	picture = hildon_desktop_picture_from_drawable(drawable);

	if (picture == None) {
		return FALSE;
	}
	
	hildon_desktop_picture_and_mask_from_file("/usr/share/omweather/grip.png",
			&grip,
			&mask,
			&width,
			&height);

	if ((plugin->clip.x!=event->area.x - x_offset)||(plugin->clip.y!=event->area.y - y_offset)||(plugin->clip.width!=event->area.width)||(plugin->clip.height!=event->area.height))
	{
		if (!plugin->updateTimeout)
			plugin->updateTimeout=g_timeout_add(1000,(GSourceFunc)launcher_changed,plugin);
	}
	plugin->clip.x = event->area.x - x_offset;
	plugin->clip.y = event->area.y - y_offset;
	plugin->clip.width = event->area.width;
	plugin->clip.height = event->area.height;
 
	region = XFixesCreateRegion(GDK_DISPLAY(), &plugin->clip, 1);

	XFixesSetPictureClipRegion(GDK_DISPLAY(), picture, 0, 0, region);

	color.red = color.blue = color.green = 0;
	color.alpha = 0;

	XRenderFillRectangle(GDK_DISPLAY(), PictOpSrc, picture, &color,
			0, 0,
			widget->allocation.width,
			widget->allocation.height);

	if (grip) {
		XRenderComposite(GDK_DISPLAY(), PictOpSrc,
				grip,
				mask,
				picture,
				0, 0,
				0, 0,
				widget->allocation.width-width, widget->allocation.height-height,
				widget->allocation.width, widget->allocation.height);
	}

	XFixesDestroyRegion(GDK_DISPLAY(), region);
	XRenderFreePicture(GDK_DISPLAY(), picture);

	if (plugin->queueRefresh)
	{	
		redraw_home_window(TRUE);
		plugin->queueRefresh=FALSE;
	}
	
        return GTK_WIDGET_CLASS(
            g_type_class_peek_parent(
                GTK_FRAME_GET_CLASS(widget)))->expose_event(widget, event);
}
#endif
/*******************************************************************************/
