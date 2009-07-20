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
void choose_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
    gint result;
    gchar                       *control_name = NULL;
    struct lists_struct        *list = NULL;
    GtkWidget *window               = NULL,
              *main_table           = NULL,
              *country_list_view    = NULL,
              *scrolled_window      = NULL;
    GtkWidget       *config = GTK_WIDGET(user_data);
    enum { UNKNOWN, SOURCE, COUNTRY, STATE, TOWN };
    gint type_button = UNKNOWN;

//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif
    control_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
    if(!strcmp("country_button", control_name)){
      type_button = COUNTRY;
    }
    if(!strcmp("source_button", control_name)){
      type_button = SOURCE;
    }

    list = (struct lists_struct*)g_object_get_data(G_OBJECT(config), "list");
    window = gtk_dialog_new();
    main_table = gtk_table_new(8, 8, FALSE);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW
                                        (scrolled_window),
                                        GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 620, 280);

    if (type_button == COUNTRY)
        country_list_view = create_tree_view(list->countries_list);
    if (type_button == SOURCE){
        country_list_view = create_tree_view(list->sources_list);
    }
    gtk_container_add(GTK_CONTAINER(scrolled_window),
                      GTK_WIDGET(country_list_view));
    gtk_table_attach_defaults(GTK_TABLE(main_table),
                              scrolled_window, 1, 2, 1, 2);
    gtk_widget_show_all(main_table);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       main_table, TRUE, TRUE, 0);
    gtk_widget_show_all(window);
    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    gtk_widget_destroy(window);

}
/*******************************************************************************/
void station_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif
    static struct       lists_struct list;
    gint result;
    GtkWidget *window               = NULL,
              *hbox                 = NULL,
              *radio1               = NULL,
              *main_table           = NULL,
              *main_label           = NULL,
              *label_set            = NULL,
              *source_label         = NULL,
              *source_label_name    = NULL,
              *manual_button        = NULL,
              *source_button        = NULL,
              *country_button       = NULL,
              *station_button       = NULL,
              *region_button        = NULL,
              *vertical0_alignmnet  = NULL,
              *vertical1_alignmnet  = NULL,
              *vertical2_alignmnet  = NULL,
              *left_alignmnet       = NULL,
              *right_alignmnet      = NULL,
              *save_button          = NULL,
              *sources              = NULL,
              *source_vertical_box  = NULL,
              *gps_button           = NULL;
    GSList    *group                = NULL;


/* Prepairing */


    window = gtk_dialog_new();
    gtk_widget_set_name(window, "simple_settings_window");
    main_table = gtk_table_new(8, 8, FALSE);

    g_object_set_data(G_OBJECT(window), "list", (gpointer)&list);

    left_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignmnet, 5, -1);
    gtk_table_attach((GtkTable*)main_table, left_alignmnet,
                                0, 1, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (left_alignmnet);


    main_label = gtk_label_new((gchar*)g_object_get_data(G_OBJECT(button), "station_label"));
    set_font(main_label, NULL, 20);
    gtk_widget_show (main_label);
    gtk_table_attach((GtkTable*)main_table,main_label,
                                1, 8, 1, 2,
                                GTK_FILL | GTK_SHRINK | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );
    vertical1_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical1_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignmnet,
                                0, 3, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical1_alignmnet);

    label_set = gtk_label_new(_("Set"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 40, -1);
    gtk_table_attach((GtkTable*)main_table,label_set,
                                1, 2, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );



    hbox = gtk_hbox_new(TRUE, 0);

    group = NULL;
    manual_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(manual_button), gtk_label_new(_("Manual")));
    gtk_widget_set_size_request(manual_button, 174, 50);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(manual_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox), manual_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(manual_button), group);
    gps_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(gps_button), gtk_label_new(_("GPS")));
    gtk_widget_set_size_request(gps_button, 174, 50);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(gps_button), FALSE);
    group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(manual_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(gps_button), group);
    gtk_box_pack_start (GTK_BOX (hbox), gps_button, TRUE, TRUE, 0);
    gtk_widget_show (hbox);


    gtk_table_attach((GtkTable*)main_table, hbox,
                                2, 5, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    vertical2_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical2_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignmnet,
                                0, 8, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    /* Button Source */
    source_button = gtk_button_new();
    source_label = gtk_label_new(_("Source"));
    source_label_name = gtk_label_new((gchar*)g_object_get_data(G_OBJECT(button), "station_source"));
    set_font(source_label, NULL, 12);

    source_vertical_box = gtk_vbox_new(TRUE, 2);
    gtk_widget_show(source_vertical_box);

    gtk_box_pack_start(GTK_BOX(source_vertical_box), source_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(source_vertical_box), source_label_name, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (source_button), source_vertical_box);
    gtk_widget_show (source_button);


    gtk_widget_set_name(source_button, "source_button");
    gtk_widget_set_size_request(source_button, 150, 50);
    gtk_table_attach((GtkTable*)main_table, source_button,
                                2, 3, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );
    g_signal_connect(G_OBJECT(source_button), "button-release-event",
                     G_CALLBACK(choose_button_handler),
                     window);

    country_button = gtk_button_new_with_label (_("Country"));
    gtk_widget_set_name(country_button, "country_button");
    gtk_widget_set_size_request(country_button, 150, 50);
    gtk_table_attach((GtkTable*)main_table, country_button,
                                3, 4, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );
    g_signal_connect(G_OBJECT(country_button), "button-release-event",
                     G_CALLBACK(choose_button_handler),
                     window);

    region_button = gtk_button_new_with_label (_("Region"));
    gtk_widget_set_name(region_button, "region_button");
    gtk_widget_set_size_request(region_button, 150, 50);
    gtk_table_attach((GtkTable*)main_table, region_button,
                                2, 3, 6, 7,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    station_button = gtk_button_new_with_label (_("Town"));
    gtk_widget_set_name(station_button, "station_button");
    gtk_widget_set_size_request(station_button, 150, 50);
    gtk_table_attach((GtkTable*)main_table, station_button,
                                3, 4, 6, 7,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    save_button = gtk_button_new_with_label (_("Save"));
    gtk_widget_set_size_request(save_button, 150, 60);
    gtk_widget_show (save_button);
    gtk_table_attach((GtkTable*)main_table, save_button,
                                5, 6, 6, 8, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 20, 20 );
    gtk_widget_show (save_button);

    right_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(right_alignmnet, 5, -1);
    gtk_table_attach((GtkTable*)main_table, right_alignmnet,
                                4, 5, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (right_alignmnet);

    gtk_widget_show (main_table);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       main_table, TRUE, TRUE, 0);

    memset(&list, 0, sizeof(struct lists_struct));
    /* create sources list from aviable sources */
    list.sources_list = app->sources_list;
    /* Set default value to country  */
    if(list.sources_list && app->config->current_source){
        g_object_set_data(G_OBJECT(window), "current_source", (gpointer)app->config->current_source);
        /* fill countries list */
        changed_sources_handler(sources, window);
        changed_country_handler(sources, window);

    }



    gtk_widget_show_all(window);
    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    gtk_widget_destroy(window);

}

/*******************************************************************************/
GtkWidget*
create_station_button(gchar* station_label_s, gchar* station_name_s, gchar *station_code_s, gchar *station_source_s)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    GtkWidget *station_label = NULL,
              *station_name  = NULL,
              *vertical_box  = NULL,
              *button = NULL;

    button = gtk_button_new();

    g_object_set_data(G_OBJECT(button), "station_label", (gpointer)station_label_s);
    g_object_set_data(G_OBJECT(button), "station_name", (gpointer)station_name_s);
    g_object_set_data(G_OBJECT(button), "station_code", (gpointer)station_code_s);
    g_object_set_data(G_OBJECT(button), "station_source", (gpointer)station_source_s);

    station_label = gtk_label_new(station_label_s);
    set_font(station_label, NULL, 12);
    gtk_widget_show (station_label);


    station_name = gtk_label_new (station_name_s);
//    set_font_color(station_name, 0, 100, 100);
    set_font(station_name, NULL, 18);
    gtk_widget_show(station_name);
    vertical_box = gtk_vbox_new(TRUE, 2);
    gtk_widget_show(vertical_box);

    gtk_box_pack_start(GTK_BOX(vertical_box), station_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vertical_box), station_name, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (button), vertical_box);

    g_signal_connect(G_OBJECT(button), "button-release-event",
                     G_CALLBACK(station_setup_button_handler),
                     (gpointer)button);
    gtk_widget_set_size_request(button, 135, 60);
    gtk_widget_show (button);

    return button;
}
/*******************************************************************************/
GtkWidget*
create_and_full_stations_buttons(void)
{
  GtkWidget
          *box     = NULL,
          *station = NULL;
    gboolean valid = FALSE;
    GtkTreeIter     iter;
    gchar   *station_selected = NULL,
            *station_name = NULL,
            *station_code = NULL,
            *station_source = NULL;
    gint  station_number = 1;
    char buffer[512];

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    box = gtk_hbox_new(TRUE, 5);
    valid =
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                      (app->user_stations_list), &iter);
    while (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                           &iter,
                           0, &station_name,
                           1, &station_code, 3, &station_source, -1);
        snprintf(buffer, sizeof(buffer) - 1, "Station%i", station_number);
        /* Attention !!!!!! check memory leak for g_strdup(buffer) */
        station = create_station_button(g_strdup(buffer), station_name, station_code, station_source);
        gtk_box_pack_start(GTK_BOX(box), station, TRUE, TRUE, 0);
        valid =
            gtk_tree_model_iter_next(GTK_TREE_MODEL
                                     (app->user_stations_list), &iter);
        station_number++;
        /* Only  *four* station for simple mode */
        if (station_number > 4)
            break;
    }

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
          *medium_alignmnet     = NULL,
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
