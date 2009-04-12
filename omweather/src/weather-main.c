/* vim: set sw=4 ts=4 et: */
/* This file is part of Other Maemo Weather(omweather)
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
 *
*/
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "weather-main.h"
#include "weather-home.h"
/*******************************************************************************/
gint
dbus_callback(const gchar *interface, const gchar *method, GArray *arguments,
		gpointer data,osso_rpc_t *retval){

    fprintf(stderr,"hello-world dbus: %s, %s\n", interface, method);
    if(!strcmp (method, "top_application"))
	gtk_window_present (GTK_WINDOW (data));

    retval->type = DBUS_TYPE_INVALID;
    return OSSO_OK;
}
/*******************************************************************************/
int
main(int argc, char *argv[]){
    HildonProgram   *OMWeather = NULL;
    osso_context_t  *osso_context;
    osso_return_t   ret;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif
    osso_context = osso_initialize("omweather_app", VERSION, TRUE, NULL);
    if(!osso_context){
        fprintf(stderr,"osso_initialize failed\n");
        return 1;
    }
    gtk_init(&argc, &argv);
    
    OMWeather = create_omweather();
    app->main_view = OMWeather;
    if(app->main_view){
          /* Create the hildon application and setup the title */
          app->app = HILDON_PROGRAM ( hildon_program_get_instance () );
          g_set_application_name (PACKAGE);
          ret = osso_rpc_set_default_cb_f (osso_context, dbus_callback, app->main_view);
          if (ret != OSSO_OK){
              fprintf (stderr, "osso_rpc_set_default_cb_f failed: %d.\n", ret);
              return 2;
          }
          hildon_program_add_window( app->app, app->main_view);
          app->osso = osso_context;
          gtk_widget_show_all(app->main_view);
          gtk_main();
          gtk_widget_destroy(app->main_view);
    }
    osso_deinitialize(osso_context);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

    return 0;
}
/*******************************************************************************/
gboolean
main_window_button_key_press_cb(GtkWidget *widget, GdkEventKey *event,
				gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(event->keyval == GDK_F6){
    /* The "Full screen" hardware key has been pressed */
        if(app->fullscreen == GDK_WINDOW_STATE_FULLSCREEN)
            gtk_window_unfullscreen (GTK_WINDOW(user_data));
        else
            gtk_window_fullscreen (GTK_WINDOW(user_data));
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return FALSE;
}
/*******************************************************************************/
gboolean
main_window_state_event_cb(GtkWidget *widget, GdkEventWindowState *event,
				gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->fullscreen = (event->new_window_state & GDK_WINDOW_STATE_FULLSCREEN);
    return FALSE;
}
/*******************************************************************************/
HildonWindow*
create_omweather(void){
    GtkWidget	*main_widget = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* This code for non hildon system */
/*
    main_widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_widget), PACKAGE);
*/
#if defined OS2009
    main_widget = hildon_stackable_window_new ();
#else
    main_widget = hildon_window_new();
#endif
    gtk_window_set_title(GTK_WINDOW(main_widget), PACKAGE);
    gtk_window_set_default_size(GTK_WINDOW(main_widget), 640, 480);
    gtk_widget_show_all(main_widget);

    if(!omweather_init_OS2009(main_widget))
	return NULL;
/* signals */
    g_signal_connect((gpointer)main_widget, "destroy_event",
			G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect((gpointer)main_widget, "delete_event",
			G_CALLBACK (gtk_main_quit), NULL);

/* Connect to keypress */
    g_signal_connect (main_widget, "key_press_event",
                      G_CALLBACK (main_window_button_key_press_cb),
                      main_widget);
/* For fullscreen/ unfullscreen mode */
    g_signal_connect (main_widget, "window_state_event",
                      G_CALLBACK (main_window_state_event_cb),
                      NULL);

    gtk_widget_modify_bg(main_widget, GTK_STATE_NORMAL,
			    &app->config->background_color);

    return main_widget;
}/*******************************************************************************/
