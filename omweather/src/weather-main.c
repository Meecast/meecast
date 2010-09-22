/* vim: set sw=4 ts=4 et: */
/* This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2010 Vlad Vasiliev
 * Copyright (C) 2006-2010 Pavel Fialko
 * 	for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
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
#include "weather-portrait.h"
#include "weather-utils.h"
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
    osso_context_t  *osso_context;
    osso_return_t   ret;
    GtkWidget* window;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif
    osso_context = osso_initialize("org.maemo.omweather", VERSION, TRUE, NULL);
    if(!osso_context){
        fprintf(stderr,"osso_initialize failed\n");
        return 1;
    }
    gtk_init(&argc, &argv);

/*    clutter_init(NULL, NULL); */

    window = create_omweather();
    if(window){
          app->main_view =  window;
          app->osso = osso_context;
          /* Create the hildon application and setup the title */
          app->app = HILDON_PROGRAM ( hildon_program_get_instance () );
          g_set_application_name (PACKAGE);
          ret = osso_rpc_set_default_cb_f (osso_context, dbus_callback, app->main_view);
          if (ret != OSSO_OK){
              fprintf (stderr, "osso_rpc_set_default_cb_f failed: %d.\n", ret);
              return 2;
          }
          hildon_program_add_window(app->app, HILDON_WINDOW(app->main_view));
          app->osso = osso_context;
          #if defined OS2009
             check_device_position(app->dbus_conn);
             if (app->portrait_position){
                fprintf(stderr,"Portrait Position\n ");
                init_portrait(app->main_view);
             }else{
                fprintf(stderr,"Landscape Position\n ");
                init_landscape(app->main_view);
             }
          #endif
          gtk_widget_show_all(GTK_WIDGET(app->main_view));
          gtk_main();
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
        change_state_window(user_data);
    }
#ifdef OS2009
    if(event->keyval == GDK_F5){
      if (app->portrait_position)
           init_landscape(widget);
      else
           init_portrait(widget);
    }
#endif
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return FALSE;
}


/*******************************************************************************/
GtkWidget*
create_omweather(void){
    GtkWidget *main_widget = NULL;

    GdkPixmap *background;
    GdkPixbuf *pixbuf;
    GtkStyle  *style;
    GError *error = NULL;

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

/* Set background image */

    pixbuf = gdk_pixbuf_new_from_file ("/usr/share/omweather/images/background.png",&error);
    if (error != NULL) {
        if (error->domain == GDK_PIXBUF_ERROR) {
            g_print ("Pixbuf Related Error:\n");
        }
        if (error->domain == G_FILE_ERROR) {
            g_print ("File Error: Check file permissions and state:\n");
        }
       g_printerr ("%s\n", error[0].message);
    }else{
       gdk_pixbuf_render_pixmap_and_mask (pixbuf, &background, NULL, 0);
       style = gtk_style_new();
       style->bg_pixmap[0] = background;
       gtk_widget_set_style (GTK_WIDGET(main_widget), GTK_STYLE(style));
        g_object_unref(style);
       g_object_unref(pixbuf);
    }

    gtk_window_set_title(GTK_WINDOW(main_widget), PACKAGE);
    gtk_window_set_default_size(GTK_WINDOW(main_widget), 800, 480);

    if(!omweather_init_OS2009(GTK_WIDGET(main_widget)))
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

    gtk_widget_modify_bg(main_widget, GTK_STATE_NORMAL,
			    &app->config->background_color);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

    return main_widget;
}/*******************************************************************************/
