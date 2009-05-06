/* vim: set sw=4 ts=4 et: */
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

/*******************************************************************************/
#ifndef _weather_dbus_h
#define _weather_dbus 1

/*******************************************************************************/
#include "weather-common.h"
#define GCONF_KEY_CURRENT_CONNECTIVITY	"/system/osso/connectivity/IAP/current"
#ifdef USE_CONIC
    #include <conic/conic.h>
    #define USER_DATA_MAGIC 0xaadcaadc
#endif

#include "weather-dbus.h"

#if defined OS2009 || defined OS2008
    #include <mce/dbus-names.h>
    #include <mce/mode-names.h>
#endif

#define MCE_MATCH_RULE "type='signal',interface='" MCE_SIGNAL_IF \
                        "',member='" MCE_DEVICE_MODE_SIG "'"

/*******************************************************************************/
void
weather_initialize_dbus(void) {

    gchar *tmp;
#ifdef USE_DBUS
    gchar *filter_string;
    DBusError error;
#endif
    GConfClient *gconf_client = NULL;
//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif
    if (!app->dbus_is_initialize) {
        /* Reseting values */
        app->iap_connecting = FALSE;
        app->iap_connected = FALSE;
        app->iap_connecting_timer = 0;

        /* Check connection */
        gconf_client = gconf_client_get_default();
        if (gconf_client) {
            tmp = gconf_client_get_string(gconf_client,
                                          GCONF_KEY_CURRENT_CONNECTIVITY,
                                          NULL);
            if (tmp) {
                app->iap_connected = TRUE;
                g_free(tmp);
            } else
                app->iap_connected = FALSE;
            gconf_client_clear_cache(gconf_client);
            g_object_unref(gconf_client);
        }
#ifdef USE_CONIC
        app->connection = con_ic_connection_new();
        if (app->connection != NULL) {
            g_object_set(app->connection, "automatic-connection-events",
                         TRUE, NULL);
            g_signal_connect(G_OBJECT(app->connection),
                             "connection-event",
                             G_CALLBACK(connection_cb),
                             GINT_TO_POINTER(USER_DATA_MAGIC));
        }
#else
    #ifndef NONMAEMO
        osso_iap_cb(iap_callback);
    #endif
#endif

#ifdef USE_DBUS
        /* Add D-BUS signal handler for 'status_changed' */
        app->dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);

#if !defined OS2008 && !defined OS2009
        filter_string =
            g_strdup_printf("interface=%s", ICD_DBUS_INTERFACE);
        /* add match */
        dbus_error_init (&error);
        dbus_bus_add_match(app->dbus_conn, filter_string, &error);
        if (dbus_error_is_set(&error)){
             fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
             dbus_error_free(&error);
        }
        g_free(filter_string);
        /* add the callback */
        dbus_connection_add_filter(app->dbus_conn,
                                   get_connection_status_signal_cb,
                                   NULL, NULL);

#endif

#if defined OS2009 || defined OS2008
        dbus_error_init (&error);
        dbus_bus_add_match(app->dbus_conn, MCE_MATCH_RULE, &error);
        if (dbus_error_is_set(&error)){
             fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
             dbus_error_free(&error);
        }
        if (!dbus_connection_add_filter(app->dbus_conn,
                                      get_mce_signal_cb, NULL, NULL)){
             fprintf(stderr,"Error dbus_connection_add_filter failed\n");
        }

#endif

#endif

/* For Debug on i386 */
#if ! defined (RELEASE) || defined (NONMAEMO)
        app->iap_connected = TRUE;
#endif
        app->dbus_is_initialize = TRUE;
    }
}
/*******************************************************************************/
void
weather_deinitialize_dbus(void) {

//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif

    if (app->dbus_conn){
#if !defined OS2008 && !defined OS2009
         dbus_bus_remove_match(app->dbus_conn, ICD_DBUS_INTERFACE, NULL);
         dbus_connection_remove_filter(app->dbus_conn,
             get_connection_status_signal_cb);
#endif
         dbus_connection_close(app->dbus_conn);
         dbus_connection_unref(app->dbus_conn);
    }

}
#endif
/*******************************************************************************/
