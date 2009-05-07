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
#include "weather-portrait.h"

#if defined OS2009 || defined OS2008
    #include <mce/dbus-names.h>
    #include <mce/mode-names.h>
#endif



#if defined OS2009 || defined OS2008
DBusHandlerResult
get_mce_signal_cb(DBusConnection *conn, DBusMessage *msg, gpointer data){

    DBusMessageIter iter;
    const char *mode_name = NULL;


    if (dbus_message_is_signal(msg, MCE_SIGNAL_IF, MCE_DEVICE_MODE_SIG)){
        if (dbus_message_iter_init(msg, &iter)){
            dbus_message_iter_get_basic(&iter, &mode_name);
            fprintf(stderr,"New status %s\n",mode_name);
        }else
            fprintf(stderr,"message did not have argument");
    }

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}
#endif
#if defined OS2009 
void
size_requested(GtkWidget *w, GtkRequisition *geo){
  g_warning("SIZE REQUEST %dx%d",
            geo->width, geo->height);
}

void
size_allocated(GtkWidget *w, GtkAllocation *geo){
  g_warning("SIZE ALLOCATION %dx%d%+d%+d",
            geo->width, geo->height, geo->x, geo->y);
}

void
set_portrait(GtkWidget *self, char const *prop, guint32 value){
  gdk_property_change(gtk_widget_get_toplevel(self)->window,
                      gdk_atom_intern_static_string (prop),
                      gdk_x11_xatom_to_atom(XA_CARDINAL), 32,
                      GDK_PROP_MODE_REPLACE, (gpointer)&value, 1);
}



void 
init_portrait(GtkWidget *win){
  g_signal_connect(win, "size-request",  G_CALLBACK(size_requested),  NULL);
  g_signal_connect(win, "size-allocate", G_CALLBACK(size_allocated), NULL);

  gtk_widget_realize(win);

  set_portrait(win, "_HILDON_PORTRAIT_MODE_SUPPORT", 1);

  set_portrait(win, "_HILDON_PORTRAIT_MODE_REQUEST", 1);
}

#endif