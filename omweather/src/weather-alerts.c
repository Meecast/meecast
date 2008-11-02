/* # vim: sw=4 ts=4 expandtab ai */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
 * Copyright (C) 2008      Andrew Olmsted
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
#include "weather-alerts.h"
#include "weather-utils.h"
/*******************************************************************************/
GtkWidget *create_alerts_page(GtkWidget * window) {
    GtkWidget *main_widget = NULL,
        *scrolled_window = NULL,
        *user_alerts_list_view = NULL,
        *alerts_list_view = NULL,
        *buttons_box = NULL, *add_button = NULL, *remove_button = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_widget = gtk_hbox_new(FALSE, 0);
    /* User alerts list */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW
                                        (scrolled_window),
                                        GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 250, 280);
    gtk_box_pack_start(GTK_BOX(main_widget), scrolled_window, FALSE,
                       FALSE, 0);

    user_alerts_list_view = create_tree_view(app->user_alerts_list);
    GLADE_HOOKUP_OBJECT(window, user_alerts_list_view,
                        "user_alerts_list_view");
    gtk_container_add(GTK_CONTAINER(scrolled_window),
                      GTK_WIDGET(user_alerts_list_view));
    /* buttons */
    buttons_box = gtk_vbox_new(FALSE, 0);
    add_button = create_button_with_image(NULL, "qgn_list_hw_button_left",
                                          26, TRUE, FALSE);
    remove_button =
        create_button_with_image(NULL, "qgn_list_hw_button_right", 26,
                                 TRUE, FALSE);
    gtk_box_pack_start(GTK_BOX(buttons_box), add_button, FALSE, FALSE, 80);
    gtk_box_pack_start(GTK_BOX(buttons_box), remove_button, FALSE,
                       FALSE, 80);
    gtk_box_pack_start(GTK_BOX(main_widget), buttons_box, FALSE, FALSE,
                       10);
    /* Alerts list */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW
                                        (scrolled_window),
                                        GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 250, 280);
    gtk_box_pack_start(GTK_BOX(main_widget), scrolled_window, FALSE,
                       FALSE, 0);

    alerts_list_view = create_tree_view(create_alerts_list());
    GLADE_HOOKUP_OBJECT(window, user_alerts_list_view, "alerts_list_view");
    gtk_container_add(GTK_CONTAINER(scrolled_window),
                      GTK_WIDGET(alerts_list_view));
    gtk_widget_show_all(main_widget);
    return main_widget;
}

/*******************************************************************************/
GtkListStore *create_user_alerts_list(void) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    return gtk_list_store_new(1, G_TYPE_STRING);
}

/*******************************************************************************/
void fill_user_alerts_list(GSList * source_list, GtkListStore ** list) {
    GtkTreeIter iter;
    gchar *alert_name = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    while (source_list) {
        alert_name = strdup((gchar *) source_list->data);
        if (alert_name) {
            gtk_list_store_append(*list, &iter);
            gtk_list_store_set(*list, &iter, 0, alert_name, -1);
            alert_name && (g_free(alert_name), alert_name = NULL);
        }
        source_list = g_slist_next(source_list);
    }
}

/*******************************************************************************/
GSList *create_list_of_user_alerts(GtkListStore * list) {
    GSList *stlist = NULL;
    GtkTreeIter iter;
    gboolean valid = FALSE;
    gchar *alert_name = NULL, *str = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list), &iter);
    while (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, 0, &alert_name,
                           -1);
        str = g_strdup(alert_name);
        stlist = g_slist_append(stlist, str);
        g_free(alert_name);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list), &iter);
    }
    return stlist;
}

/*******************************************************************************/
GtkListStore *create_alerts_list(void) {
    GtkListStore *list = NULL;
    GtkTreeIter iter;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    list = gtk_list_store_new(1, G_TYPE_STRING);

    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("Storm"), -1);

    return list;
}

/*******************************************************************************/
