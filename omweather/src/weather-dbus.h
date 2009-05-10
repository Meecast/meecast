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


void weather_initialize_dbus(void);
void weather_deinitialize_dbus(void);
#ifdef USE_CONIC
    extern void connection_cb(ConIcConnection * connection,ConIcConnectionEvent * event, gpointer user_data);
#endif
#if defined OS2009 || defined OS2008
    extern DBusHandlerResult get_mce_signal_cb(DBusConnection *conn, DBusMessage *msg, gpointer data);
#endif