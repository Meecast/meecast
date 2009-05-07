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
/*
            if (!strcmp(mode_name, "landscape")) {
                
            }else{
                if (!strcmp(mode_name, "portrait")) {
                
                }
*/                
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

void 
init_landscape(GtkWidget *win){

  gtk_widget_realize(win);

  set_portrait(win, "_HILDON_PORTRAIT_MODE_SUPPORT", 0);

  set_portrait(win, "_HILDON_PORTRAIT_MODE_REQUEST", 0);
}

gchar *get_device_position(DBusConnection *connection)
{
    DBusError derror;
    char *mode, *ret;
    DBusMessage *message, *msg, *reply;
    DBusPendingCall* pending;

    message = dbus_message_new_method_call(MCE_SERVICE,
                                   MCE_REQUEST_PATH,
                                   MCE_REQUEST_IF,
                                   MCE_DEVICE_ORIENTATION_GET);

    dbus_error_init(&derror);
    reply = dbus_connection_send_with_reply_and_block(connection, message,
                                                      -1, &derror);
    dbus_message_unref(message);
    if (dbus_error_is_set(&derror)) {
            fprintf(stderr,"Getting device mode from MCE failed: %s", derror.message);
            dbus_error_free(&derror);
            return g_strdup("landscape");
    }

    if (!dbus_message_get_args(reply, NULL,
                               DBUS_TYPE_STRING, &mode,
                               DBUS_TYPE_INVALID)) {
        error("Invalid arguments for MCE get_device_orientation reply");
        dbus_message_unref(reply);
        return g_strdup("landscape");
    }
    ret = g_strdup(mode);
    dbus_message_unref(reply);
    return ret;
}

#endif
