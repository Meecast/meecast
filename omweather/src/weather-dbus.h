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
/*******************************************************************************/
#ifndef _weather_dbus_h
#define _weather_dbus_h 1
/*******************************************************************************/
#include "weather-common.h"
#include "weather-event.h"
#if !defined OS2008 && !defined OS2009
#include <osso-ic.h>
#include <osso-ic-dbus.h>
#endif
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
/*******************************************************************************/
void check_current_connection(void);
void weather_initialize_dbus(gpointer applet);
void weather_deinitialize_dbus(void);
#ifdef USE_CONIC
    void connection_cb(ConIcConnection *connection, ConIcConnectionEvent *event, gpointer user_data);
#endif
#if defined OS2009 || defined OS2008
    DBusHandlerResult get_mce_signal_cb(DBusConnection *conn, DBusMessage *msg, gpointer data);
#endif
#ifndef USE_CONIC
#ifndef NONMAEMO
void iap_callback(struct iap_event_t *event, void *arg);
#endif
#endif
#ifdef USE_CONIC
    void connection_cb(ConIcConnection * connection,ConIcConnectionEvent * event, gpointer user_data);
#endif
DBusHandlerResult
get_omweather_signal_cb(DBusConnection *conn, DBusMessage *msg, gpointer data);
void send_dbus_signal (const gchar *interface, const gchar *path, const gchar *member);
int read_config(AppletConfig*);
void redraw_home_window(gboolean first_start);
void reload_omw_plugin(void);
/*******************************************************************************/
#endif
