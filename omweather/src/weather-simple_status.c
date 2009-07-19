/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
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
#include "weather-simple_settings.h"
/*******************************************************************************/
void simple_settings_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/*    if(user_data)
        gtk_widget_destroy(GTK_WIDGET(user_data));
*/
    weather_simple_window_settings(user_data);
}
/*******************************************************************************/
void update_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->popup_window)
        destroy_popup_window();
    if(user_data)
        gtk_widget_destroy(GTK_WIDGET(user_data));
    update_weather(TRUE);
}
/*******************************************************************************/
GtkWidget*
create_stations_buttons(void)
{
  GtkWidget
          *box = NULL,
          *station1 = NULL,
          *station2 = NULL,
          *station3 = NULL,
          *station4 = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    box = gtk_hbox_new(TRUE, 5);
    station1 = gtk_button_new_with_label ("Station1");
    gtk_widget_set_size_request(station1, 120, 60);
    gtk_widget_show (station1);
    gtk_box_pack_start(GTK_BOX(box), station1, TRUE, TRUE, 0);
    station2 = gtk_button_new_with_label ("Station2");
    gtk_widget_show (station2);
    gtk_box_pack_start(GTK_BOX(box), station2, TRUE, TRUE, 0);
    station3 = gtk_button_new_with_label ("Station3");
    gtk_widget_show (station3);
    gtk_box_pack_start(GTK_BOX(box), station3, TRUE, TRUE, 0);
    station4 = gtk_button_new_with_label ("Station4");
    gtk_widget_show (station4);
    gtk_box_pack_start(GTK_BOX(box), station4, TRUE, TRUE, 0);
    return box;
}
/*******************************************************************************/
void
weather_simple_window_status(GtkWidget *widget, gpointer user_data){
  GtkWidget
          *window           = NULL,
          *main_table       = NULL,
          *collapsed_button = NULL,
          *expanded_button  = NULL,
          *settings_button  = NULL,
          *stations_box     = NULL,
          *update_button    = NULL,
          *help_button      = NULL,
          *left_alignmnet   = NULL,
          *right_alignmnet   = NULL,
          *vertical1_alignmnet  = NULL,
          *vertical2_alignmnet  = NULL,
          *vertical3_alignmnet  = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif


#if defined OS2009
    window = hildon_stackable_window_new();
#else
    window = hildon_window_new();
#endif
    gtk_widget_show(window);

    main_table = gtk_table_new(6,7, FALSE);

    left_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignmnet, 20, -1);
    gtk_table_attach((GtkTable*)main_table, left_alignmnet,
                                0, 1, 0, 1,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (left_alignmnet);

    stations_box = create_stations_buttons();
    gtk_widget_show (stations_box);
    gtk_table_attach((GtkTable*)main_table,stations_box,
                                1, 5, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    right_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(right_alignmnet, 20, -1);
    gtk_table_attach((GtkTable*)main_table, right_alignmnet,
                                6, 7, 0, 1,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (right_alignmnet);

    vertical1_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical1_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignmnet,
                                0, 6, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical1_alignmnet);


    collapsed_button = gtk_button_new_with_label (_("Collapsed"));
    gtk_widget_set_size_request(collapsed_button, 140, 60);
    gtk_widget_show (collapsed_button);
    gtk_table_attach((GtkTable*)main_table, collapsed_button,
                                1, 2, 3, 4, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    expanded_button = gtk_button_new_with_label (_("Expanded"));
    gtk_widget_set_size_request(expanded_button, 140, 60);
    gtk_widget_show (expanded_button);
    gtk_table_attach((GtkTable*)main_table, expanded_button,
                                2, 3, 3, 4, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    settings_button = gtk_button_new_with_label (_("Settings"));
    gtk_widget_set_size_request(settings_button, 300, 60);
    gtk_widget_show (settings_button);
    gtk_table_attach((GtkTable*)main_table, settings_button,
                                3, 5, 3, 4, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
    g_signal_connect(G_OBJECT(settings_button), "button-release-event",
                     G_CALLBACK(simple_settings_button_handler),
                     (gpointer)window);


    vertical2_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical2_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignmnet,
                                0, 6, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical2_alignmnet);


    help_button = gtk_button_new_with_label (_("About"));
    gtk_widget_set_size_request(help_button, 300, 60);
    gtk_widget_show (help_button);
    gtk_table_attach((GtkTable*)main_table, help_button,
                                1, 3, 5, 6, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 5, 0 );

    update_button = gtk_button_new_with_label (_("Update Now"));
    gtk_widget_set_size_request(update_button, 300, 60);
    gtk_widget_show (update_button);
    gtk_table_attach((GtkTable*)main_table, update_button,
                                3, 5, 5, 6, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    g_signal_connect(G_OBJECT(update_button), "button-release-event",
                     G_CALLBACK(update_button_handler),
                     (gpointer)window);


    vertical3_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical3_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignmnet,
                                0, 6, 6, 7,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical3_alignmnet);

    gtk_container_add (GTK_CONTAINER (window), main_table);

    gtk_widget_show(main_table);
}
/*******************************************************************************/
