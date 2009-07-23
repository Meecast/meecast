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
#include "weather-simple-settings.h"
/*******************************************************************************/

void
widget_style_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif
    GtkWidget *vbox                 = NULL,
            *label                = NULL,
            *window               = NULL;
    gint result;

    window = gtk_dialog_new();
    gtk_widget_set_name(window, "widget_style_window");

    gtk_widget_show_all(window);
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (window)
        gtk_widget_destroy(window);


//#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
//#endif

}

void
highlight_current_item(GtkTreeView *tree_view, GtkListStore *list, gchar *current){
    GtkTreeIter     iter;
    gchar           *name = NULL;
  gboolean        valid;
    GtkTreePath     *path;
    GtkTreeModel    *model;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!current || !tree_view)
        return;

    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list),
                &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(list),
                           &iter,
                           0, &name,
                            -1);
        if(!strcmp(current, name)){
                model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
                path = gtk_tree_model_get_path(model, &iter);
                gtk_tree_view_set_cursor(GTK_TREE_VIEW(tree_view),
                                         path, NULL, FALSE);
                gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (tree_view), path, NULL, TRUE, 0.5, 0);
                gtk_tree_path_free(path);
                break;
        }
        valid  = gtk_tree_model_iter_next(GTK_TREE_MODEL(list),
                  &iter);
    }
}
/*******************************************************************************/
void
list_changed(GtkTreeSelection *sel,  gpointer user_data)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar     *name         = NULL;
  gchar     *control_name = NULL;
  GtkWidget *vbox                 = NULL,
            *label                = NULL,
            *window               = NULL,
            *temp_button          = NULL,
            *button               = NULL;
  gint      id = 0;
  enum { UNKNOWN, SOURCE, COUNTRY, STATE, TOWN };
  gint type_button = UNKNOWN;

//#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
//#endif
    button = (GtkWidget*)g_object_get_data(G_OBJECT(user_data), "button");
    label = (GtkWidget*)g_object_get_data(G_OBJECT(button), "label");
    vbox = (GtkWidget*)g_object_get_data(G_OBJECT(button), "vbox");
    window = (GtkWidget*)g_object_get_data(G_OBJECT(button), "window");

    if (label){
        gtk_widget_destroy(label);
        label = NULL;
    }
    if (gtk_tree_selection_get_selected(sel,&model, &iter)){
        gtk_tree_model_get(model, &iter, 0, &name, -1);
        label = gtk_label_new(name);
        g_object_set_data(G_OBJECT(button), "label", (gpointer)label);
        gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
        gtk_widget_show(button);
        gtk_widget_show(label);
    }
    control_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
    fprintf(stderr,"Control name %s\n", control_name);
    if(!strcmp("country_button", control_name))
        type_button = COUNTRY;
    if(!strcmp("source_button", control_name))
        type_button = SOURCE;
    if(!strcmp("region_button", control_name))
        type_button = STATE;
    if(!strcmp("station_button", control_name))
        type_button = TOWN;
    if (type_button == TOWN){
        g_object_set_data(G_OBJECT(window), "station_name", (gpointer)name);
        id = get_station_code(g_object_get_data(G_OBJECT(window), "station_source"),
                              GPOINTER_TO_INT(g_object_get_data(G_OBJECT(window), "station_region_id")), name);
        g_object_set_data(G_OBJECT(window), "station_code", (gpointer)id);
    }
    if (type_button == STATE){
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "station_button");
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
        id = get_state_code(g_object_get_data(G_OBJECT(window), "station_source"), name);
        g_object_set_data(G_OBJECT(button), "station_region_id", (gpointer)id);
        g_object_set_data(G_OBJECT(button), "station_region", (gpointer)name);
        g_object_set_data(G_OBJECT(window), "station_region_id", (gpointer)id);

        changed_state_handler(NULL, window);
    }
    if (type_button == COUNTRY){
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "station_button");
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "region_button");
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
        id = get_country_code(g_object_get_data(G_OBJECT(window), "station_source"), name);
        g_object_set_data(G_OBJECT(button), "station_country_id", (gpointer)id);
        g_object_set_data(G_OBJECT(button), "station_country", (gpointer)name);
        g_object_set_data(G_OBJECT(window), "station_country_id", (gpointer)id);
        changed_country_handler(NULL, window);
    }
    /* TO DO make all if we will have +1 source */
    if (type_button == SOURCE){
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "station_button");
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "region_button");
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "country_button");
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
        changed_sources_handler(NULL, window);
    }
    /* Destroy window */
    g_signal_emit_by_name(G_OBJECT(user_data), "close", NULL);

    user_data = NULL;
//#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
//#endif
}
/*******************************************************************************/
void
save_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
    GtkTreeIter iter;
    gboolean valid;
    GtkWidget *stations_box;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    iter = add_station_to_user_list(g_strdup(g_object_get_data(G_OBJECT(user_data), "station_name")),
                                      g_strdup(g_object_get_data(G_OBJECT(user_data), "station_code")),
                                      FALSE,
                                      g_strdup(g_object_get_data(G_OBJECT(user_data), "station_source")),
                                      GPOINTER_TO_INT(g_object_get_data(G_OBJECT(user_data), "station_number")));
    valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list), &iter);
    if (valid)
        gtk_list_store_remove(app->user_stations_list, &iter);
    /* Update config file */
    config_save(app->config);
    stations_box = (gpointer)(g_object_get_data(G_OBJECT(user_data), "station_box"));
    gtk_widget_destroy(stations_box);
    stations_box = create_and_full_stations_buttons((GtkTable*)(g_object_get_data(G_OBJECT(user_data), "settings_window_table")));
    gtk_widget_show (stations_box);
    gtk_table_attach((GtkTable*)(g_object_get_data(G_OBJECT(user_data), "settings_window_table")),
                                stations_box, 1, 2, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    /* Destroy window */
    g_signal_emit_by_name(G_OBJECT(user_data), "close", NULL);
}
/*******************************************************************************/
void
choose_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
    gint result;
    gchar                       *control_name = NULL;
    struct lists_struct        *list = NULL;
    GtkWidget *window               = NULL,
              *main_table           = NULL,
              *list_view            = NULL,
              *scrolled_window      = NULL;
    GtkWidget       *config = GTK_WIDGET(user_data);
    enum { UNKNOWN, SOURCE, COUNTRY, STATE, TOWN };
    gint type_button = UNKNOWN;
    GtkTreeSelection  *sel;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    control_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
    if(!strcmp("country_button", control_name))
        type_button = COUNTRY;
    if(!strcmp("source_button", control_name))
        type_button = SOURCE;
    if(!strcmp("region_button", control_name))
        type_button = STATE;
    if(!strcmp("station_button", control_name))
        type_button = TOWN;

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

    if (type_button == COUNTRY){
        list_view = create_tree_view(list->countries_list);
        highlight_current_item(list_view, list->countries_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_country"));
        gtk_widget_set_name(list_view, "countries_list");
    }
    if (type_button == SOURCE){
        list_view = create_tree_view(list->sources_list);
        highlight_current_item(list_view, list->sources_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_source"));
        gtk_widget_set_name(list_view, "sources_list");
    }
    if (type_button == STATE){
        list_view = create_tree_view(list->regions_list);
        highlight_current_item(list_view, list->regions_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_region"));
        gtk_widget_set_name(list_view, "states_list");
    }
    if (type_button == TOWN){
        list_view = create_tree_view(list->stations_list);
        highlight_current_item(list_view, list->stations_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_name"));
        gtk_widget_set_name(list_view, "stations_list");
    }

    gtk_container_add(GTK_CONTAINER(scrolled_window),
                      GTK_WIDGET(list_view));
    gtk_table_attach_defaults(GTK_TABLE(main_table),
                              scrolled_window, 1, 2, 1, 2);
    g_object_set_data(G_OBJECT(window), "button", (gpointer)button);
    sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (list_view));
    g_signal_connect (sel, "changed",G_CALLBACK (list_changed), window);

    gtk_widget_show_all(main_table);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       main_table, TRUE, TRUE, 0);
    gtk_widget_show_all(window);
    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (window)
        gtk_widget_destroy(window);

}
/*******************************************************************************/
GtkWidget*
create_button(gchar* name, gchar* value, gchar* button_name, gchar* parameter_name, GtkWidget* widget){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
  GtkWidget *button = NULL,
           *label_name,
           *vertical_box,
           *label;

    button = gtk_button_new();
    label = gtk_label_new(name);
    label_name = gtk_label_new(value);
    set_font(label, NULL, 12);

    vertical_box = gtk_vbox_new(TRUE, 2);
    gtk_widget_show(vertical_box);

    gtk_box_pack_start(GTK_BOX(vertical_box), label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vertical_box), label_name, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (button), vertical_box);
    gtk_widget_show (button);

    g_object_set_data(G_OBJECT(button), "vbox", (gpointer)vertical_box);
    g_object_set_data(G_OBJECT(button), "label", (gpointer)label_name);
    g_object_set_data(G_OBJECT(button), "window", (gpointer)widget);
    g_object_set_data(G_OBJECT(button), parameter_name, (gpointer)value);

    gtk_widget_set_name(button, button_name);
    gtk_widget_set_size_request(button, 180, 80);
    g_signal_connect(G_OBJECT(button), "button-release-event",
                     G_CALLBACK(choose_button_handler),
                     widget);
    return button;

}
/*******************************************************************************/
void station_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    static struct       lists_struct list;
    gint result;
    GtkWidget *window               = NULL,
              *hbox                 = NULL,
              *radio1               = NULL,
              *main_table           = NULL,
              *main_label           = NULL,
              *label_set            = NULL,
              *source_vertical_box  = NULL,
              *source_label         = NULL,
              *source_label_name    = NULL,
              *manual_button        = NULL,
              *source_button        = NULL,
              *country_button       = NULL,
              *country_vertical_box = NULL,
              *country_label        = NULL,
              *country_label_name   = NULL,
              *region_button        = NULL,
              *region_vertical_box  = NULL,
              *region_label         = NULL,
              *region_label_name    = NULL,
              *station_button       = NULL,
              *station_vertical_box = NULL,
              *station_label        = NULL,
              *station_label_name   = NULL,
              *vertical0_alignmnet  = NULL,
              *vertical1_alignmnet  = NULL,
              *vertical2_alignmnet  = NULL,
              *left_alignmnet       = NULL,
              *right_alignmnet      = NULL,
              *save_button          = NULL,
              *sources              = NULL,
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
     source_button = create_button(_("Source"),(gchar*)g_object_get_data(G_OBJECT(button), "station_source"),
                                   "source_button", "station_source", window);
     g_object_set_data(G_OBJECT(window), "source_button", (gpointer)source_button);
     gtk_table_attach((GtkTable*)main_table, source_button,
                                2, 3, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    /* Button Country */
    country_button = create_button(_("Country"),(gchar*)g_object_get_data(G_OBJECT(button), "station_country"),
                                   "country_button", "station_country", window);
    g_object_set_data(G_OBJECT(window), "country_button", (gpointer)country_button);
    gtk_table_attach((GtkTable*)main_table, country_button,
                                3, 4, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    /* Button region */
    region_button = create_button(_("Region"),(gchar*)g_object_get_data(G_OBJECT(button), "station_region"),
                                   "region_button", "station_region", window);
    g_object_set_data(G_OBJECT(window), "region_button", (gpointer)region_button);
    gtk_table_attach((GtkTable*)main_table, region_button,
                                2, 3, 6, 7,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    /* Button station */
    station_button = create_button(_("Town"),(gchar*)g_object_get_data(G_OBJECT(button), "station_name"),
                                   "station_button", "station_name", window);
    g_object_set_data(G_OBJECT(window), "station_button", (gpointer)station_button);
    gtk_table_attach((GtkTable*)main_table, station_button,
                                3, 4, 6, 7,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );
    save_button = gtk_button_new_with_label (_("Save"));
    gtk_widget_set_size_request(save_button, 180, 80);
    gtk_widget_show (save_button);
    gtk_table_attach((GtkTable*)main_table, save_button,
                                5, 6, 6, 8, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 10, 10);
    gtk_widget_show (save_button);
    g_signal_connect(G_OBJECT(save_button), "button-release-event",
                     G_CALLBACK(save_button_handler),
                     window);

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

    g_object_set_data(G_OBJECT(window), "current_source", (gpointer)app->config->current_source);
    g_object_set_data(G_OBJECT(window), "station_region_id", (gpointer)g_object_get_data(G_OBJECT(button), "station_region_id"));
    g_object_set_data(G_OBJECT(window), "station_region", (gpointer)g_object_get_data(G_OBJECT(button), "station_region"));
    g_object_set_data(G_OBJECT(window), "station_country_id", (gpointer)g_object_get_data(G_OBJECT(button), "station_country_id"));
    g_object_set_data(G_OBJECT(window), "station_country", (gpointer)g_object_get_data(G_OBJECT(button), "station_country"));
    g_object_set_data(G_OBJECT(window), "station_source", (gpointer)g_object_get_data(G_OBJECT(button), "station_source"));
    g_object_set_data(G_OBJECT(window), "station_number", (gpointer)g_object_get_data(G_OBJECT(button), "station_number"));
    g_object_set_data(G_OBJECT(window), "settings_window_table", (gpointer)g_object_get_data(G_OBJECT(button), "settings_window_table"));
    g_object_set_data(G_OBJECT(window), "station_box", (gpointer)g_object_get_data(G_OBJECT(button), "station_box"));
    /* fill countries list */
    changed_sources_handler(NULL, window);
    changed_country_handler(NULL, window);
    changed_state_handler(NULL, window);


    gtk_widget_show_all(window);
    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    gtk_widget_destroy(window);

}

/*******************************************************************************/
GtkWidget*
create_station_button(gint station_number, gchar* station_name_s, gchar *station_code_s, gchar *station_source_s,
                      gint country_id, gchar *station_country_s, gint region_id, gchar *station_region_s)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    GtkWidget *station_label = NULL,
              *station_name  = NULL,
              *vertical_box  = NULL,
              *button = NULL;
    char buffer[512];

    button = gtk_button_new();

    g_object_set_data(G_OBJECT(button), "station_name", (gpointer)station_name_s);
    g_object_set_data(G_OBJECT(button), "station_code", (gpointer)station_code_s);
    g_object_set_data(G_OBJECT(button), "station_source", (gpointer)station_source_s);
    g_object_set_data(G_OBJECT(button), "station_country", (gpointer)station_country_s);
    g_object_set_data(G_OBJECT(button), "station_region", (gpointer)station_region_s);
    g_object_set_data(G_OBJECT(button), "station_country_id", (gpointer)country_id);
    g_object_set_data(G_OBJECT(button), "station_region_id", (gpointer)region_id);
    g_object_set_data(G_OBJECT(button), "station_number", (gpointer)station_number);

    snprintf(buffer, sizeof(buffer) - 1, "Station %i", station_number);
    station_label = gtk_label_new(buffer);
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
    gtk_widget_set_size_request(button, 135, 80);
    gtk_widget_show (button);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return button;
}
/*******************************************************************************/
GtkWidget*
create_and_full_stations_buttons(GtkWidget *main_table)
{
  GtkWidget
          *box     = NULL,
          *station = NULL;
    gboolean valid = FALSE;
    gboolean valid2 = FALSE;
    GtkTreeIter     iter;
    GtkTreeIter     iter2;
    gchar   *station_selected = NULL,
            *station_name = NULL,
            *station_code = NULL,
            *station_source = NULL,
            *station_country = NULL,
            *station_region = NULL;
    gint    station_country_id,
            station_region_id;

    GtkListStore *allinformation_list = NULL;
    gint  station_number = 0;
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

        allinformation_list = get_all_information_about_station(station_source, station_code);
        valid2 = gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                              (allinformation_list), &iter2);

        if(valid2)
          gtk_tree_model_get(GTK_TREE_MODEL(allinformation_list),
                                   &iter2,
                                   0, &station_country,
                                   1, &station_region,
                                   2, &station_country_id,
                                   3, &station_region_id,
                                   -1);

        station = create_station_button(station_number,  station_name, station_code, station_source, station_country_id,
                                        station_country, station_region_id, station_region );
        g_object_set_data(G_OBJECT(station), "settings_window_table", (gpointer)main_table);
        g_object_set_data(G_OBJECT(station), "station_box", (gpointer)box);
        gtk_box_pack_start(GTK_BOX(box), station, TRUE, TRUE, 0);
        valid =
            gtk_tree_model_iter_next(GTK_TREE_MODEL
                                     (app->user_stations_list), &iter);
        station_number++;

        /* Only *four* station for simple mode */
        if (station_number > 3)
            break;
    }
    /* Added nil station_button */
    while (station_number < 4){
        station = create_station_button(station_number,  _("Unknown"), -1, app->config->current_source, -1,
                                        _("Unknown"), -1, _("Unknown"));
        g_object_set_data(G_OBJECT(station), "settings_window_table", (gpointer)main_table);
        g_object_set_data(G_OBJECT(station), "station_box", (gpointer)box);
        gtk_box_pack_start(GTK_BOX(box), station, TRUE, TRUE, 0);
        station_number++;
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
  GtkWidget
          *units_label           = NULL,
          *units_description      = NULL,
          *units_box             = NULL;

  gchar   *units_string = NULL;
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

    stations_box = create_and_full_stations_buttons(main_table);
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

    /* temperature */
    if(app->config->temperature_units == CELSIUS)
        units_string = "C";
    else
        units_string = "F";

    /* distance units */
    if(app->config->distance_units == METERS)
        units_string = g_strjoin(", ", units_string,_("m"), NULL);
        else{
            if(app->config->distance_units == KILOMETERS)
                 units_string = g_strjoin(", ", units_string,_("km"), NULL);
            else{
               if(app->config->distance_units == MILES)
                 units_string = g_strjoin(", ", units_string,_("mi"), NULL);
               else
                 units_string = g_strjoin(", ", units_string,_("seami"), NULL);
            }
        }

    /* wind units */
    if(app->config->wind_units == METERS_S)
        units_string = g_strjoin(", ", units_string,_("m/s"), NULL);
    else{
        if(app->config->wind_units == KILOMETERS_H)
            units_string = g_strjoin(", ", units_string,_("km/h"), NULL);
        else
            units_string = g_strjoin(", ", units_string,_("mi/h"), NULL);
    }

    /* pressure */
    if(app->config->pressure_units == MB)
        units_string = g_strjoin(", ", units_string,_("mb"), NULL);
    else{
        if(app->config->pressure_units == INCH)
            units_string = g_strjoin(", ", units_string,_("inch"), NULL);
        else
            units_string = g_strjoin(", ", units_string,_("mm"), NULL);
    }

    fprintf(stderr, "\nUnits:  %s\n", units_string);

    units_button = gtk_button_new ();
    units_label = gtk_label_new(_("Units"));
    /* TO DO !!!!!! check memory leak in units_string */
    set_font(units_label, NULL, 12);
    gtk_widget_show (units_label);

    units_description = gtk_label_new (units_string);
    set_font(units_description, NULL, 18);
    gtk_widget_show(units_description);
    units_box = gtk_vbox_new(TRUE, 2);
    gtk_widget_show(units_box);

    gtk_box_pack_start(GTK_BOX(units_box), units_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(units_box), units_description, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (units_button), units_box);


    gtk_widget_set_size_request(units_button, 490, 70);
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
    gtk_widget_set_size_request(widget_style_button, 490, 70);
    gtk_widget_show (widget_style_button);
    gtk_table_attach((GtkTable*)main_table, widget_style_button,
                                1, 2, 5, 6, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
    g_signal_connect(G_OBJECT(widget_style_button), "button-release-event",
                   G_CALLBACK(widget_style_setup_button_handler),
                   (gpointer)widget_style_button);

    vertical3_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical3_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignmnet,
                                0, 6, 6, 7,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical3_alignmnet);

    update_button = gtk_button_new_with_label (_("Update"));
    gtk_widget_set_size_request(update_button, 490, 70);
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

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
