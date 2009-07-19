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

void station_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif
    gint result;
    GtkWidget *window               = NULL,
              *main_table           = NULL,
              *main_label           = NULL,
              *label_set            = NULL,
              *manual_button        = NULL,
              *source_button        = NULL,
              *country_button       = NULL,
              *station_button       = NULL,
              *region_button        = NULL,
              *gps_button           = NULL;

    window = gtk_dialog_new();
    main_table = gtk_table_new(4,8, FALSE);
    main_label = gtk_label_new((gchar*)user_data);
    gtk_widget_show (main_label);
    gtk_table_attach((GtkTable*)main_table,main_label,
                                1, 8, 1, 2, 
                                GTK_FILL | GTK_SHRINK | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );
    label_set = gtk_label_new(_("Set"));
    gtk_widget_set_size_request(label_set, 40, -1);
    gtk_table_attach((GtkTable*)main_table,label_set,
                                1, 2, 2, 3,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    manual_button = gtk_button_new_with_label (_("Manual"));
    gtk_widget_set_size_request(manual_button, 100, -1);
    gtk_table_attach((GtkTable*)main_table,manual_button,
                                2, 3, 2, 3,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    gps_button = gtk_button_new_with_label (_("GPS"));
    gtk_widget_set_size_request(gps_button, 100, -1);
    gtk_table_attach((GtkTable*)main_table,gps_button,
                                3, 4, 2, 3,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    source_button = gtk_button_new_with_label (_("Source"));
    gtk_widget_set_size_request(source_button, 100, -1);
    gtk_table_attach((GtkTable*)main_table, source_button,
                                2, 3, 2, 3,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    country_button = gtk_button_new_with_label (_("Country"));
    gtk_widget_set_size_request(country_button, 100, -1);
    gtk_table_attach((GtkTable*)main_table, country_button,
                                3, 4, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    region_button = gtk_button_new_with_label (_("Region"));
    gtk_widget_set_size_request(region_button, 100, -1);
    gtk_table_attach((GtkTable*)main_table, region_button,
                                4, 5, 2, 3,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    station_button = gtk_button_new_with_label (_("Town"));
    gtk_widget_set_size_request(station_button, 100, -1);
    gtk_table_attach((GtkTable*)main_table, station_button,
                                4, 5, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );


    gtk_widget_show (main_table);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       main_table, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
/* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    gtk_widget_destroy(window);

}

/*******************************************************************************/
GtkWidget*
create_station_button(gchar* station_name)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    GtkWidget *station = NULL;
    station = gtk_button_new_with_label (station_name);
    g_signal_connect(G_OBJECT(station), "button-release-event",
                     G_CALLBACK(station_setup_button_handler),
                     (gpointer)station_name);
    gtk_widget_set_size_request(station, 120, 60);
    gtk_widget_show (station);
    return station;
}
/*******************************************************************************/
GtkWidget*
create_and_full_stations_buttons(void)
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
    station1 = create_station_button("Station1");
    gtk_box_pack_start(GTK_BOX(box), station1, TRUE, TRUE, 0);
    station2 = create_station_button("Station2");;
    gtk_box_pack_start(GTK_BOX(box), station2, TRUE, TRUE, 0);
    station3 = create_station_button("Station3");
    gtk_box_pack_start(GTK_BOX(box), station3, TRUE, TRUE, 0);
    station4 = create_station_button("Station4");
    gtk_box_pack_start(GTK_BOX(box), station4, TRUE, TRUE, 0);
    return box;
}
/*******************************************************************************/
void
weather_simple_window_settings(gpointer user_data){
  GtkWidget
          *window               = NULL,
          *main_table           = NULL,
          *units_button         = NULL,
          *widget_style_button  = NULL,
          *save_button          = NULL,
          *stations_box         = NULL,
          *update_button        = NULL,
          *left_alignmnet       = NULL,
          *right_alignmnet      = NULL,
          *medium_alignmnet      = NULL,
          *vertical0_alignmnet  = NULL,
          *vertical1_alignmnet  = NULL,
          *vertical2_alignmnet  = NULL,
          *vertical3_alignmnet  = NULL,
          *vertical4_alignmnet  = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif


#if defined OS2009
    window = hildon_stackable_window_new();
#else
    window = hildon_window_new();
#endif
    gtk_widget_show(window);

    main_table = gtk_table_new(4,8, FALSE);

    left_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignmnet, 5, -1);
    gtk_table_attach((GtkTable*)main_table, left_alignmnet,
                                0, 1, 0, 8,
                                 GTK_FILL,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (left_alignmnet);

    vertical0_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical0_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical0_alignmnet,
                                0, 3, 0, 1,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical0_alignmnet);

    stations_box = create_and_full_stations_buttons();
    gtk_widget_show (stations_box);
    gtk_table_attach((GtkTable*)main_table,stations_box,
                                1, 2, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );


    vertical1_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical1_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignmnet,
                                0, 3, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical1_alignmnet);


    units_button = gtk_button_new_with_label (_("Units"));
    gtk_widget_set_size_request(units_button, 490, 60);
    gtk_widget_show (units_button);
    gtk_table_attach((GtkTable*)main_table, units_button,
                                1, 2, 3, 4, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    vertical2_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical2_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignmnet,
                                0, 3, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical2_alignmnet);


    widget_style_button = gtk_button_new_with_label (_("Widget_style"));
    gtk_widget_set_size_request(widget_style_button, 490, 60);
    gtk_widget_show (widget_style_button);
    gtk_table_attach((GtkTable*)main_table, widget_style_button,
                                1, 2, 5, 6, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    vertical3_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical3_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignmnet,
                                0, 6, 6, 7,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical3_alignmnet);

    update_button = gtk_button_new_with_label (_("Update"));
    gtk_widget_set_size_request(update_button, 490, 60);
    gtk_widget_show (update_button);
    gtk_table_attach((GtkTable*)main_table, update_button,
                                1, 2, 6, 7, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );


    vertical4_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical4_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical4_alignmnet,
                                0, 6, 7, 8,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical4_alignmnet);

    medium_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(medium_alignmnet, 50, -1);
    gtk_table_attach((GtkTable*)main_table, medium_alignmnet,
                                2, 3, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (medium_alignmnet);

    save_button = gtk_button_new_with_label (_("Save"));
    gtk_widget_set_size_request(save_button, 150, 60);
    gtk_widget_show (save_button);
    gtk_table_attach((GtkTable*)main_table, save_button,
                                3, 4, 6, 7, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (save_button);

    right_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(right_alignmnet, 5, -1);
    gtk_table_attach((GtkTable*)main_table, right_alignmnet,
                                4, 5, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (right_alignmnet);

    gtk_container_add (GTK_CONTAINER (window), main_table);

    gtk_widget_show(main_table);
}
/*******************************************************************************/
