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
#include "weather-stations.h"
#include "weather-config.h"
#include "weather-settings.h"
#include "weather-utils.h"
/*******************************************************************************/
void
widget_styles_save(GtkWidget *window){

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    GtkWidget
    *preset_now = NULL,
    *preset_now_plus_two = NULL,
    *preset_now_plus_three_h = NULL,
    *preset_now_plus_three_v = NULL,
    *preset_now_plus_seven = NULL,
    *selected_icon_set = NULL;

    GSList      *icon_set = NULL;


    preset_now = lookup_widget(window, "preset_now");
    preset_now_plus_two = lookup_widget(window, "preset_now_plus_two");
    preset_now_plus_three_v = lookup_widget(window, "preset_now_plus_three_v");
    preset_now_plus_three_h = lookup_widget(window, "preset_now_plus_three_h");
    preset_now_plus_seven = lookup_widget(window, "preset_now_plus_seven");

    if (preset_now && preset_now_plus_two && preset_now_plus_three_v && preset_now_plus_three_h && preset_now_plus_seven) {
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now)))
            app->config->icons_layout = PRESET_NOW;
        else
            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now_plus_two)))
                app->config->icons_layout = PRESET_NOW_PLUS_TWO;
            else
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now_plus_three_v)))
                    app->config->icons_layout = PRESET_NOW_PLUS_THREE_V;
                else
                    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now_plus_three_h)))
                        app->config->icons_layout = PRESET_NOW_PLUS_THREE_H;
                    else
                        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now_plus_seven)))
                            app->config->icons_layout = PRESET_NOW_PLUS_SEVEN;
                        else
                            app->config->icons_layout = PRESET_NOW;
    }
    /* icon set */
    icon_set =
        (GSList *) g_object_get_data(G_OBJECT(window),
                                     "iconsetlist");
    if (icon_set) {
        while (icon_set) {
            selected_icon_set =
                lookup_widget(window, (gchar *) icon_set->data);
            if (selected_icon_set) {
                if (gtk_toggle_button_get_active
                    (GTK_TOGGLE_BUTTON(selected_icon_set))) {
                    if (app->config->icon_set)
                        g_free(app->config->icon_set);
                    app->config->icon_set =
                        g_strdup((gchar *) icon_set->data);
                    break;
                }
            }
            icon_set = g_slist_next(icon_set);
        }
        update_icons_set_base(app->config->icon_set);
    }
/* save settings */
    config_save(app->config);
    redraw_home_window(FALSE);
}
/*******************************************************************************/
void
widget_style_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
    GtkWidget *vbox                 = NULL,
              *layouts_line         = NULL,
              *iconsets_line        = NULL,
              *window               = NULL,
              *widget_style_button  = NULL;
    gint result;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    vbox = gtk_vbox_new(TRUE, 2);
    window = gtk_dialog_new();
    gtk_widget_set_name(window, "widget_style_window");

    layouts_line = create_layouts_line(window, 40, SIMPLE_MODE);
    iconsets_line = create_iconsets_line(window, 40, SIMPLE_MODE);
    gtk_box_pack_start(vbox,
                       layouts_line, TRUE, TRUE, 10);
    gtk_box_pack_start(vbox,
                       iconsets_line, TRUE, TRUE, 10);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       vbox, TRUE, TRUE, 0);

    gtk_dialog_add_button (GTK_DIALOG (window), GTK_STOCK_SAVE, GTK_RESPONSE_YES);
    gtk_widget_show_all(window);
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (result == GTK_RESPONSE_YES)
        widget_styles_save(window);

    if (window)
        gtk_widget_destroy(window);


    widget_style_button = (gpointer)(g_object_get_data(G_OBJECT(button), 
                                                        "widget_style_button"));
    gtk_widget_destroy(widget_style_button);
    create_and_fill_widget_style_box(user_data);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
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
static void
row_activated_callback (GtkWidget         *tree_view,
                        GtkTreePath       *path,
                        GtkTreeViewColumn *column,
                        gpointer           user_data)
{
  GtkTreeIter       iter;
  gchar *name;

  GtkTreeModel *model = gtk_tree_view_get_model(tree_view);

  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter, 0, &name, -1);
  list_changed(NULL, user_data, name);
}
/******************************************************************************/
void
list_changed(GtkTreeSelection *sel,  gpointer user_data, gchar *name){
  GtkTreeIter iter;
  GtkTreeModel *model;
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

    if (sel && gtk_tree_selection_get_selected(sel,&model, &iter)){
        gtk_tree_model_get(model, &iter, 0, &name, -1);
    }
    if (name){
#if defined OS2009
        hildon_button_set_value(button, name);
#else
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
        label = gtk_label_new(name);
        g_object_set_data(G_OBJECT(button), "label", (gpointer)label);
        gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
        gtk_widget_show(label);
#endif
        gtk_widget_show(button);
    }
    control_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
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
#if defined OS2009
        hildon_button_set_value(temp_button, "");
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
#endif
        id = get_state_code(g_object_get_data(G_OBJECT(window), "station_source"), name);
        g_object_set_data(G_OBJECT(button), "station_region_id", (gpointer)id);
        g_object_set_data(G_OBJECT(button), "station_region", (gpointer)name);
        g_object_set_data(G_OBJECT(window), "station_region_id", (gpointer)id);

        changed_state_handler(NULL, window);
    }
    if (type_button == COUNTRY){
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "station_button");
#if defined OS2009
        hildon_button_set_value(temp_button, "");
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
#endif
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "region_button");
#if defined OS2009
        hildon_button_set_value(temp_button, "");
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
#endif
        id = get_country_code(g_object_get_data(G_OBJECT(window), "station_source"), name);
        g_object_set_data(G_OBJECT(button), "station_country_id", (gpointer)id);
        g_object_set_data(G_OBJECT(button), "station_country", (gpointer)name);
        g_object_set_data(G_OBJECT(window), "station_country_id", (gpointer)id);
        changed_country_handler(NULL, window);
    }
    /* TO DO make all if we will have +1 source */
    if (type_button == SOURCE){
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "station_button");
#if defined OS2009
        hildon_button_set_value(temp_button, "");
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
            g_object_set_data(G_OBJECT(temp_button), "label", NULL);
        }
#endif
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "region_button");
#if defined OS2009
        hildon_button_set_value(temp_button, "");
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
            g_object_set_data(G_OBJECT(temp_button), "label", NULL);
        }
#endif
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "country_button");
#if defined OS2009
        hildon_button_set_value(temp_button, "");
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
            g_object_set_data(G_OBJECT(temp_button), "label", NULL);
        }
#endif
        g_object_set_data(G_OBJECT(window), "station_source", name);
        g_object_set_data(G_OBJECT(button), "station_source", name);
        changed_sources_handler(NULL, window);
    }
    /* Destroy window */
    g_signal_emit_by_name(G_OBJECT(user_data), "close", NULL);

    user_data = NULL;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void
save_station(GtkWidget *window){
    GtkTreeIter iter;
    gboolean valid;
    gboolean is_gps;
    GtkWidget *stations_box;
    gchar
            *station_name = NULL,
            *station_code = NULL,
            *station_source = NULL,
            *station_country = NULL,
            *station_region = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if (gtk_toggle_button_get_active(g_object_get_data(G_OBJECT(window), "gps")))
        is_gps = TRUE;
    else
        is_gps = FALSE;

    iter = add_station_to_user_list(g_strdup(g_object_get_data(G_OBJECT(window), "station_name")),
                                      g_strdup(g_object_get_data(G_OBJECT(window), "station_code")),
                                      is_gps,
                                      g_strdup(g_object_get_data(G_OBJECT(window), "station_source")),
                                      GPOINTER_TO_INT(g_object_get_data(G_OBJECT(window), "station_number")));
    valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list), &iter);
    if (valid){
          gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                           &iter,
                           0, &station_name,
                           1, &station_code,
                           2, &is_gps,
                           3, &station_source, -1);

          delete_station_from_user_list(station_name, NULL);
//        gtk_list_store_remove(app->user_stations_list, &iter);
    }
    /* Update config file */
    config_save(app->config);
    stations_box = (gpointer)(g_object_get_data(G_OBJECT(window), "station_box"));
    gtk_widget_destroy(stations_box);
    stations_box = create_and_fill_stations_buttons((GtkTable*)(g_object_get_data(G_OBJECT(window), "settings_window_table")));
    gtk_widget_show (stations_box);
    gtk_table_attach((GtkTable*)(g_object_get_data(G_OBJECT(window), "settings_window_table")),
                                stations_box, 1, 2, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

//    app->config->station_country_button_pressed = FALSE;
  //  app->config->station_source_button_pressed = FALSE;
  //  app->config->station_region_button_pressed = FALSE;
  //  app->config->station_name_button_pressed = FALSE;
}
/*******************************************************************************/
void
choose_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
    gint                    result;
    gchar                   *control_name = NULL,
                            dialog_name[255];
    struct lists_struct     *list = NULL;
    GtkWidget               *window = NULL,
                            *main_table = NULL,
                            *list_view = NULL,
                            *scrolled_window = NULL;
    GtkWidget               *config = GTK_WIDGET(user_data);
    enum { UNKNOWN, SOURCE, COUNTRY, STATE, TOWN };
    gint                    type_button = UNKNOWN;
    GtkTreeSelection        *sel;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

fprintf(stderr, "\nchoose_button_handler %d\n", type_button);
    *dialog_name = 0;
    control_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
    if(!strcmp("country_button", control_name)){
        type_button = COUNTRY;
        snprintf(dialog_name, sizeof(dialog_name) - 1, _("Selecting country"));
        fprintf(stderr, "\ncountry_button %d\n", type_button);
//        app->config->station_country_button_pressed = TRUE;
    }
    if(!strcmp("source_button", control_name)){
        type_button = SOURCE;
        snprintf(dialog_name, sizeof(dialog_name) - 1, _("Selecting source"));
        fprintf(stderr, "\nsource_button %d\n", type_button);
  //      app->config->station_source_button_pressed = TRUE;
    }
    if(!strcmp("region_button", control_name)){
     // && 
                           //         (app->config->station_country_button_pressed == TRUE)){
        type_button = STATE;
        snprintf(dialog_name, sizeof(dialog_name) - 1, _("Selecting region"));
        fprintf(stderr, "\nregion_button %d\n", type_button);
    //    app->config->station_region_button_pressed = TRUE;
    }
    if(!strcmp("station_button", control_name)){  // && 
//                                     (app->config->station_country_button_pressed == TRUE) && 
  //                                  (app->config->station_region_button_pressed == TRUE)){
        type_button = TOWN;
        snprintf(dialog_name, sizeof(dialog_name) - 1, _("Selecting station"));
        fprintf(stderr, "\nstation_button %d\n", type_button);
    //    app->config->station_name_button_pressed = TRUE;
    }

    list = (struct lists_struct*)g_object_get_data(G_OBJECT(config), "list");
    /* This is a very serious error */
    if(!list)
        return;
    window = gtk_dialog_new_with_buttons(dialog_name, NULL,
                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            NULL);
    main_table = gtk_table_new(8, 8, FALSE);

#if defined OS2009
    scrolled_window = hildon_pannable_area_new ();
#else
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW
                                        (scrolled_window),
                                        GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
#endif
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 620, 280);

//if(type_button){
     if (type_button == COUNTRY){
        list_view = create_tree_view(list->countries_list);
        highlight_current_item(list_view, list->countries_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_country"));
  //      gtk_widget_set_name(list_view, "countries_list");
        fprintf(stderr, "\nCOUNTRY %d\n", type_button);
    }
    if (type_button == SOURCE){
        list_view = create_tree_view(list->sources_list);
        highlight_current_item(list_view, list->sources_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_source"));
        gtk_widget_set_name(list_view, "sources_list");
        fprintf(stderr, "\nSOURCE %d\n", type_button);
    }
    if ((type_button == STATE)){
     // && (app->config->station_country_button_pressed == TRUE)){
        list_view = create_tree_view(list->regions_list);
        highlight_current_item(list_view, list->regions_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_region"));
        gtk_widget_set_name(list_view, "states_list");
        fprintf(stderr, "\nSTATE %d\n", type_button);
    }
    if ((type_button == TOWN)){
     //&& (app->config->station_country_button_pressed == TRUE)
       //                       && (app->config->station_region_button_pressed == TRUE)){
        list_view = create_tree_view(list->stations_list);
        highlight_current_item(list_view, list->stations_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_name"));
        gtk_widget_set_name(list_view, "stations_list");
        fprintf(stderr, "\nTOWN %d\n", type_button);
    }
//}
    gtk_table_attach_defaults(GTK_TABLE(main_table),
                              scrolled_window, 1, 2, 1, 2);
    g_object_set_data(G_OBJECT(window), "button", (gpointer)button);
    sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (list_view));
#if defined OS2009
    g_signal_connect (list_view, "row-activated",
                    G_CALLBACK (row_activated_callback), window);
#else
    g_signal_connect (sel, "changed",G_CALLBACK (list_changed), window);
#endif


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
void
units_save(GtkWidget *window){

 GtkWidget
           *celsius = NULL,
           *fahrenheit = NULL,
           *meters = NULL,
           *kilometers = NULL,
           *miles = NULL,
           *sea_miles = NULL,
           *meters_s = NULL,
           *kilometers_h = NULL,
           *miles_h = NULL,
           *pressure_mb = NULL,
           *pressure_inHg = NULL,
           *pressure_mmHg = NULL,
           *units_box = NULL;

#ifdef DEBUGFUNCTIONCALL
     START_FUNCTION;
#endif

    celsius = lookup_widget(window, "celsius_button");
    fahrenheit = lookup_widget(window, "fahrenheit_button");

    if(celsius && fahrenheit){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(celsius)))
            app->config->temperature_units = CELSIUS;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fahrenheit)))  
            app->config->temperature_units = FAHRENHEIT;
    }

    meters = lookup_widget(window, "meters_button");
    kilometers = lookup_widget(window, "kilometers_button");
    miles = lookup_widget(window, "miles_button");
    sea_miles = lookup_widget(window, "sea_miles_button");

    if(meters && kilometers && miles && sea_miles){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(meters))) 
            app->config->distance_units = METERS;
        else{
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(kilometers)))
                app->config->distance_units = KILOMETERS;
            else{
                if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(miles)))
                    app->config->distance_units = MILES;
                else
                    app->config->distance_units = SEA_MILES;
            }
        }
    }

    meters_s = lookup_widget(window, "meters_s_button");
    kilometers_h = lookup_widget(window, "kilometers_h_button");
    miles_h = lookup_widget(window, "miles_h_button");

    if(meters_s && kilometers_h && miles_h){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(meters_s)))
            app->config->wind_units = METERS_S;
        else{
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(kilometers_h)))
                app->config->wind_units = KILOMETERS_H;
            else
                app->config->wind_units = MILES_H;
        }
    }

    pressure_mb = lookup_widget(window, "pressure_mb_button");
    pressure_inHg = lookup_widget(window, "pressure_inHg_button");
    pressure_mmHg = lookup_widget(window, "pressure_mmHg_button");

    if(pressure_mb && pressure_inHg && pressure_mmHg){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pressure_mb)))
            app->config->pressure_units = MB;
        else{
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pressure_inHg)))
                app->config->pressure_units = INCH;
            else
                app->config->pressure_units = MM;
        }
    }

}
/*******************************************************************************/
void
units_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    GtkWidget *window = NULL,
              *main_table = NULL,
              *result = NULL,
              *left_alignmnet = NULL,
              *vertical1_alignmnet = NULL,
              *vertical2_alignmnet = NULL,
              *vertical3_alignmnet = NULL,
              *main_label = NULL,
              *label_set = NULL,
              *label_set1 = NULL,
              *hbox_temperature = NULL,
              *hbox_distance = NULL,
              *hbox_speed = NULL,
              *hbox_pressure = NULL,
              *group_temperature = NULL,
              *group_distance = NULL,
              *group_speed = NULL,
              *group_pressure = NULL,
              *celsius_button = NULL,
              *fahrenheit_button = NULL,
              *meters_button = NULL,
              *kilometers_button = NULL,
              *miles_button = NULL,
              *sea_miles_button = NULL,
              *meters_s_button = NULL,
              *kilometers_h_button = NULL,
              *miles_h_button = NULL,
              *pressure_mb_button = NULL,
              *pressure_inHg_button = NULL,
              *pressure_mmHg_button = NULL,
              *save_button = NULL,
              *vbox = NULL,
              *units_button = NULL;

    window = gtk_dialog_new_with_buttons(_("Units"), NULL, 
                                     GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, NULL);
    gtk_widget_set_name(window, "units_simple_settings_window");
    main_table = gtk_table_new(8, 8, FALSE);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), main_table, TRUE, TRUE, 0);

    left_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignmnet, 5, -1);

    gtk_table_attach((GtkTable*)main_table, left_alignmnet,
                                0, 1, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );

    label_set = gtk_label_new(_("Temp."));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 120, -1);

    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 1, 2,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );


    hbox_temperature = gtk_hbox_new(TRUE, 0);
    group_temperature = NULL;

    celsius_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(celsius_button), gtk_label_new(_("C")));
    gtk_widget_set_size_request(celsius_button, 174, 50);
    GLADE_HOOKUP_OBJECT(window, celsius_button, "celsius_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(celsius_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_temperature), celsius_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(celsius_button), group_temperature);

    fahrenheit_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(fahrenheit_button), gtk_label_new(_("F")));
    gtk_widget_set_size_request(fahrenheit_button, 174, 50);
    GLADE_HOOKUP_OBJECT(window, fahrenheit_button, "fahrenheit_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(fahrenheit_button), FALSE);
    group_temperature = gtk_radio_button_get_group(GTK_RADIO_BUTTON(celsius_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(fahrenheit_button), group_temperature);
    gtk_box_pack_end (GTK_BOX (hbox_temperature), fahrenheit_button, TRUE, TRUE, 0);

    if(app->config->temperature_units == CELSIUS)
        gtk_toggle_button_set_active (celsius_button, TRUE);
    else
        gtk_toggle_button_set_active (fahrenheit_button, TRUE);

    gtk_table_attach((GtkTable*)main_table, hbox_temperature,
                                     2, 3, 1, 2,
                                     GTK_FILL | GTK_EXPAND,
                                     (GtkAttachOptions)0, 20, 0 );

    vertical1_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical1_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignmnet,
                                0, 8, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

    label_set = gtk_label_new(_("Distance"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 120, -1);

    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    hbox_distance = gtk_hbox_new(TRUE, 0);
    group_distance = NULL;
    gtk_box_pack_start (GTK_BOX (hbox_distance), label_set, TRUE, TRUE, 0);
    meters_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(meters_button), gtk_label_new(_("m")));
    gtk_widget_set_size_request(meters_button, 43, 50);
    GLADE_HOOKUP_OBJECT(window, meters_button, "meters_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(meters_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_distance), meters_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(meters_button), group_distance);

    kilometers_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(kilometers_button), gtk_label_new(_("km")));
    gtk_widget_set_size_request(kilometers_button, 43, 50);
    GLADE_HOOKUP_OBJECT(window, kilometers_button, "kilometers_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(kilometers_button), FALSE);
    group_distance = gtk_radio_button_get_group(GTK_RADIO_BUTTON(meters_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(kilometers_button), group_distance);
    gtk_box_pack_start (GTK_BOX (hbox_distance), kilometers_button, TRUE, TRUE, 0);

    miles_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(miles_button), gtk_label_new(_("mph")));
    gtk_widget_set_size_request(miles_button, 43, 50);
    GLADE_HOOKUP_OBJECT(window, miles_button, "miles_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(miles_button), FALSE);
    group_distance = gtk_radio_button_get_group(GTK_RADIO_BUTTON(kilometers_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(miles_button), group_distance);
    gtk_box_pack_start (GTK_BOX (hbox_distance), miles_button, TRUE, TRUE, 0);

    sea_miles_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(sea_miles_button), gtk_label_new(_("mi")));
    gtk_widget_set_size_request(sea_miles_button, 43, 50);
    GLADE_HOOKUP_OBJECT(window, sea_miles_button, "sea_miles_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(sea_miles_button), FALSE);
    group_distance = gtk_radio_button_get_group(GTK_RADIO_BUTTON(miles_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(sea_miles_button), group_distance);
    gtk_box_pack_start (GTK_BOX (hbox_distance), sea_miles_button, TRUE, TRUE, 0);

    if(app->config->distance_units == METERS)
        gtk_toggle_button_set_active (meters_button, TRUE);
    else{
        if(app->config->distance_units == KILOMETERS)
            gtk_toggle_button_set_active (kilometers_button, TRUE);    
        else{
            if(app->config->distance_units == MILES)
                gtk_toggle_button_set_active (miles_button, TRUE);
            else
                gtk_toggle_button_set_active (sea_miles_button, TRUE);
        }
    }

    gtk_table_attach((GtkTable*)main_table, hbox_distance,
                                2, 3, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                               (GtkAttachOptions)0, 20, 0 );

    vertical2_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical2_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignmnet,
                                0, 8, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );


    label_set = gtk_label_new(_("Speed"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 120, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    hbox_speed = gtk_hbox_new(TRUE, 0);
    group_speed = NULL;
    meters_s_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(meters_s_button), gtk_label_new(_("m/s")));
    gtk_widget_set_size_request(meters_s_button, 58, 50);
    GLADE_HOOKUP_OBJECT(window, meters_s_button, "meters_s_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(meters_s_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_speed), meters_s_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(meters_s_button), group_speed);

    kilometers_h_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(kilometers_h_button), gtk_label_new(_("km/h")));
    gtk_widget_set_size_request(kilometers_h_button, 58, 50);
    GLADE_HOOKUP_OBJECT(window, kilometers_h_button, "kilometers_h_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(kilometers_h_button), FALSE);
    group_speed = gtk_radio_button_get_group(GTK_RADIO_BUTTON(meters_s_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(kilometers_h_button), group_speed);
    gtk_box_pack_start (GTK_BOX (hbox_speed), kilometers_h_button, TRUE, TRUE, 0);

    miles_h_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(miles_h_button), gtk_label_new(_("mi/h")));
    gtk_widget_set_size_request(miles_h_button, 58, 25);
    GLADE_HOOKUP_OBJECT(window, miles_h_button, "miles_h_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(miles_h_button), FALSE);
    group_speed = gtk_radio_button_get_group(GTK_RADIO_BUTTON(kilometers_h_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(miles_h_button), group_speed);
    gtk_box_pack_start (GTK_BOX (hbox_speed), miles_h_button, TRUE, TRUE, 0);

    if(app->config->wind_units == METERS_S)
        gtk_toggle_button_set_active (meters_s_button, TRUE);
    else{
        if(app->config->wind_units == KILOMETERS_H)
            gtk_toggle_button_set_active (kilometers_h_button, TRUE);
        else
            gtk_toggle_button_set_active (miles_h_button, TRUE);
    }

    gtk_table_attach((GtkTable*)main_table, hbox_speed,
                                2, 3, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    vertical3_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical3_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignmnet,
                                0, 8, 6, 7,
                               (GtkAttachOptions)0,
                               GTK_FILL |  GTK_SHRINK,
                               0, 0 );

    label_set = gtk_label_new(_("Pressure"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 120, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 7, 8,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    hbox_pressure = gtk_hbox_new(TRUE, 0);
    group_pressure = NULL;
    pressure_mb_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(pressure_mb_button), gtk_label_new(_("mb")));
    gtk_widget_set_size_request(pressure_mb_button, 58, 50);
    GLADE_HOOKUP_OBJECT(window, pressure_mb_button, "pressure_mb_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(pressure_mb_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_pressure), pressure_mb_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(pressure_mb_button), group_pressure);

    pressure_inHg_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(pressure_inHg_button), gtk_label_new(_("inHg")));
    gtk_widget_set_size_request(pressure_inHg_button, 58, 50);
    GLADE_HOOKUP_OBJECT(window, pressure_inHg_button, "pressure_inHg_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(pressure_inHg_button), FALSE);
    group_pressure = gtk_radio_button_get_group(GTK_RADIO_BUTTON(pressure_mb_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(pressure_inHg_button), group_pressure);
    gtk_box_pack_start (GTK_BOX (hbox_pressure), pressure_inHg_button, TRUE, TRUE, 0);

    pressure_mmHg_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(pressure_mmHg_button), gtk_label_new(_("mmHg")));
    gtk_widget_set_size_request(pressure_mmHg_button, 58, 25);
    GLADE_HOOKUP_OBJECT(window, pressure_mmHg_button, "pressure_mmHg_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(pressure_mmHg_button), FALSE);
    group_pressure = gtk_radio_button_get_group(GTK_RADIO_BUTTON(pressure_inHg_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(pressure_mmHg_button), group_pressure);
    gtk_box_pack_start (GTK_BOX (hbox_pressure), pressure_mmHg_button, TRUE, TRUE, 0);

    if(app->config->pressure_units == MB)
        gtk_toggle_button_set_active (pressure_mb_button, TRUE);
    else{
        if(app->config->pressure_units == INCH)
            gtk_toggle_button_set_active (pressure_inHg_button, TRUE);
        else
            gtk_toggle_button_set_active (pressure_mmHg_button, TRUE);
    }
    gtk_table_attach((GtkTable*)main_table, hbox_pressure,
                                2, 3, 7, 8,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    gtk_dialog_add_button (GTK_DIALOG (window), GTK_STOCK_SAVE, GTK_RESPONSE_YES);
    gtk_widget_show_all(window);
    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (result == GTK_RESPONSE_YES)
        units_save(window);
    if(window)
        gtk_widget_destroy(window);

    units_button = (gpointer)(g_object_get_data(G_OBJECT(button), "units_button"));
    gtk_widget_destroy(units_button);
    create_and_fill_units_box(user_data);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
void
update_save(GtkWidget *window){
    GtkWidget   *never_update = NULL,
                *one_hour_update = NULL,
                *four_hours_update = NULL,
                *one_day_update = NULL,
                *gsm = NULL,
                *wlan = NULL;

#ifdef DEBUGFUNCTIONCALL
   START_FUNCTION;
#endif

    never_update = lookup_widget(window, "never_button");
    one_hour_update = lookup_widget(window, "one_hour_button");
    four_hours_update = lookup_widget(window, "four_hours_button");
    one_day_update = lookup_widget(window, "one_day_button");

    if(never_update && one_hour_update && four_hours_update && one_day_update){
             if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(never_update))) 
                 app->config->update_interval = 0;
        else{
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(one_hour_update)))
                app->config->update_interval = 60;
            else{
                if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(four_hours_update)))
                    app->config->update_interval = 240;
                else{
                    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(one_day_update)))
                        app->config->update_interval = 1440;
                }
            }
        }
    }

    gsm = lookup_widget(window, "gsm_button");
    wlan = lookup_widget(window, "wlan_button");

    if(gsm && wlan){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gsm)))
            app->config->update_gsm = TRUE;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wlan)))
            app->config->update_wlan = TRUE;
        if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gsm)))
            app->config->update_gsm = FALSE;
        if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wlan)))
            app->config->update_wlan = FALSE;
    }

}
/*******************************************************************************/
void
update_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    GtkWidget *window = NULL,
              *main_table = NULL,
              *result = NULL,
              *left_alignmnet = NULL,
              *vertical1_alignmnet = NULL,
              *vertical2_alignmnet = NULL,
              *vertical3_alignmnet = NULL,
              *vbox = NULL,
              *hbox_period = NULL,
              *hbox_method = NULL,
              *label_set = NULL,
              *group_period = NULL,
              *group_method = NULL,
              *never_button = NULL,
              *one_hour_button = NULL,
              *four_hours_button = NULL,
              *one_day_button = NULL,
              *gsm_button = NULL,
              *wlan_button = NULL,
              *update_button = NULL;


    window = gtk_dialog_new_with_buttons(_("Update"), NULL,
                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, NULL);
    gtk_widget_set_name(window, "update_simple_settings_window");

    main_table = gtk_table_new(8, 8, FALSE);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), main_table, TRUE, TRUE, 0);

    left_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignmnet, 5, -1);
    gtk_table_attach((GtkTable*)main_table, left_alignmnet,
                                0, 1, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );

    label_set = gtk_label_new(_("Period"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 120, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 1, 2,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    hbox_period = gtk_hbox_new(TRUE, 0);
    group_period = NULL;

    never_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(never_button), gtk_label_new(_("never")));
    gtk_widget_set_size_request(never_button, 105, 50);
    GLADE_HOOKUP_OBJECT(window, never_button, "never_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(never_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_period), never_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(never_button), group_period);

    one_hour_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(one_hour_button), gtk_label_new(_("1 hour")));
    gtk_widget_set_size_request(one_hour_button, 105, 50);
    GLADE_HOOKUP_OBJECT(window, one_hour_button, "one_hour_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(one_hour_button), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(never_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(one_hour_button), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_period), one_hour_button, TRUE, TRUE, 0);

    four_hours_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(four_hours_button), gtk_label_new(_("4 hours")));
    gtk_widget_set_size_request(four_hours_button, 105, 50);
    GLADE_HOOKUP_OBJECT(window, four_hours_button, "four_hours_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(four_hours_button), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(one_hour_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(four_hours_button), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_period), four_hours_button, TRUE, TRUE, 0);

    one_day_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(one_day_button), gtk_label_new(_("1 day")));
    gtk_widget_set_size_request(one_day_button, 105, 50);
    GLADE_HOOKUP_OBJECT(window, one_day_button, "one_day_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(one_day_button), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(four_hours_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(one_day_button), group_period);
    gtk_box_pack_end (GTK_BOX (hbox_period), one_day_button, TRUE, TRUE, 0);

    if(app->config->update_interval == 0)
         gtk_toggle_button_set_active (never_button, TRUE);
    else{
        if(app->config->update_interval == 60)
            gtk_toggle_button_set_active (one_hour_button, TRUE);
        else{
            if(app->config->update_interval == 240)
                gtk_toggle_button_set_active (four_hours_button, TRUE);
            else{
                if(app->config->update_interval == 1440)
                    gtk_toggle_button_set_active (one_day_button, TRUE);
            }
        }
    }

    gtk_table_attach((GtkTable*)main_table, hbox_period,
                                2, 3, 1, 2,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    vertical1_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical1_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignmnet,
                                0, 8, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

    label_set = gtk_label_new(_("GSM"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 120, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    gsm_button = gtk_check_button_new();
    gtk_widget_set_size_request(gsm_button, 50, 50);
    GLADE_HOOKUP_OBJECT(window, gsm_button, "gsm_button");

    if(app->config->update_gsm == TRUE)
        gtk_toggle_button_set_active (gsm_button, TRUE);
    else
        gtk_toggle_button_set_active (gsm_button, FALSE);

    gtk_table_attach((GtkTable*)main_table, gsm_button,
                                2, 3, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    vertical2_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical1_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignmnet,
                                0, 8, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

    label_set = gtk_label_new(_("WLAN"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 120, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    wlan_button = gtk_check_button_new();
    gtk_widget_set_size_request(wlan_button, 50, 50);
    GLADE_HOOKUP_OBJECT(window, wlan_button, "wlan_button");

    if(app->config->update_wlan == TRUE)
        gtk_toggle_button_set_active (wlan_button, TRUE);
    else
        gtk_toggle_button_set_active (wlan_button, FALSE);

    gtk_table_attach((GtkTable*)main_table, wlan_button,
                                2, 3, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    vertical3_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical1_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignmnet,
                                0, 8, 6, 7,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

    label_set = gtk_label_new(_("Use GSM and/or WLAN for updating \nthe weather information."));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 120, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 3, 7, 8,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    gtk_dialog_add_button (GTK_DIALOG (window), GTK_STOCK_SAVE, GTK_RESPONSE_YES);
    gtk_widget_show_all(window);

    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (result == GTK_RESPONSE_YES)
        update_save(window);
    if(window)
        gtk_widget_destroy(window);

    update_button = (gpointer)(g_object_get_data(G_OBJECT(button), "update_button"));
    gtk_widget_destroy(update_button);
    create_and_fill_update_box((gpointer)user_data);

#ifdef DEBUGFUNCTIONCALL
   END_FUNCTION;
#endif
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

    struct lists_struct     *list = NULL;

    list = (struct lists_struct*)g_object_get_data(G_OBJECT(widget), "list");

    button = create_button_with_2_line_text(name, value, 18, 12);


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
void
station_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    struct lists_struct list;
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

    window = gtk_dialog_new_with_buttons(_("Configuring station"), NULL,
                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            NULL);
    gtk_widget_set_name(window, "simple_settings_window");



    /* create sources list from aviable sources */
    memset(&list, 0, sizeof(struct lists_struct));
    list.sources_list = app->sources_list;
    g_object_set_data(G_OBJECT(window), "list", (gpointer)&list);

    g_object_set_data(G_OBJECT(window), "current_source", (gpointer)app->config->current_source);
    g_object_set_data(G_OBJECT(window), "station_region_id", (gpointer)g_object_get_data(G_OBJECT(button), "station_region_id"));
    g_object_set_data(G_OBJECT(window), "station_region", (gpointer)g_object_get_data(G_OBJECT(button), "station_region"));
    g_object_set_data(G_OBJECT(window), "station_country_id", (gpointer)g_object_get_data(G_OBJECT(button), "station_country_id"));
    g_object_set_data(G_OBJECT(window), "station_country", (gpointer)g_object_get_data(G_OBJECT(button), "station_country"));
    g_object_set_data(G_OBJECT(window), "station_source", (gpointer)g_object_get_data(G_OBJECT(button), "station_source"));
    g_object_set_data(G_OBJECT(window), "station_number", (gpointer)g_object_get_data(G_OBJECT(button), "station_number"));
    g_object_set_data(G_OBJECT(window), "settings_window_table", (gpointer)g_object_get_data(G_OBJECT(button), "settings_window_table"));
    g_object_set_data(G_OBJECT(window), "station_box", (gpointer)g_object_get_data(G_OBJECT(button), "station_box"));
    g_object_set_data(G_OBJECT(window), "station_is_gps", (gpointer)g_object_get_data(G_OBJECT(button), "station_is_gps"));

    changed_sources_handler(NULL, window);
    changed_country_handler(NULL, window);
    changed_state_handler(NULL, window);


    main_table = gtk_table_new(8, 8, FALSE);


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
    g_object_set_data(G_OBJECT(window), "gps", (gpointer)gps_button);
    if (g_object_get_data(G_OBJECT(button), "station_is_gps"))
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gps_button),
                                     TRUE);
    else
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(manual_button),
                                     TRUE);


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
    station_button = create_button(_("City"),(gchar*)g_object_get_data(G_OBJECT(button), "station_name"),
                                   "station_button", "station_name", window);
    g_object_set_data(G_OBJECT(window), "station_button", (gpointer)station_button);
    gtk_table_attach((GtkTable*)main_table, station_button,
                                3, 4, 6, 7,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

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

    gtk_dialog_add_button (GTK_DIALOG (window), GTK_STOCK_SAVE, GTK_RESPONSE_YES);
    gtk_widget_show_all(window);

    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));

    if (result == GTK_RESPONSE_YES)
        save_station(window);
    if (window)
        gtk_widget_destroy(window);

}

/*******************************************************************************/
GtkWidget*
create_station_button(gint station_number, gchar* station_name_s, gchar *station_code_s, gchar *station_source_s,
                      gint country_id, gchar *station_country_s, gint region_id, gchar *station_region_s,
                      gboolean is_gps)
{
    GtkWidget *station_label = NULL,
              *station_name  = NULL,
              *vertical_box  = NULL,
              *button = NULL;
    char buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    snprintf(buffer, sizeof(buffer) - 1, "Station %i", station_number + 1);
    button = create_button_with_2_line_text(buffer, station_name_s, 18, 12);
    g_object_set_data(G_OBJECT(button), "station_name", (gpointer)station_name_s);
    g_object_set_data(G_OBJECT(button), "station_code", (gpointer)station_code_s);
    g_object_set_data(G_OBJECT(button), "station_source", (gpointer)station_source_s);
    g_object_set_data(G_OBJECT(button), "station_country", (gpointer)station_country_s);
    g_object_set_data(G_OBJECT(button), "station_region", (gpointer)station_region_s);
    g_object_set_data(G_OBJECT(button), "station_country_id", (gpointer)country_id);
    g_object_set_data(G_OBJECT(button), "station_region_id", (gpointer)region_id);
    g_object_set_data(G_OBJECT(button), "station_number", (gpointer)station_number);
    if (is_gps)
        g_object_set_data(G_OBJECT(button), "station_is_gps", (gpointer)1);
    else
        g_object_set_data(G_OBJECT(button), "station_is_gps", (gpointer)0);


    g_signal_connect(G_OBJECT(button), "button-release-event",
                     G_CALLBACK(station_setup_button_handler),
                     (gpointer)button);
    gtk_widget_set_size_request(button, 150, 80);
//    gtk_widget_show (button);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return button;
}
/*******************************************************************************/
GtkWidget*
create_and_fill_stations_buttons(GtkWidget *main_table)
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
    gboolean is_gps;

    GtkListStore *allinformation_list = NULL;
    gint  station_number = 0;
    char buffer[512];

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    box = gtk_hbox_new(TRUE, 10);
    valid =
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                      (app->user_stations_list), &iter);
    while (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                           &iter,
                           0, &station_name,
                           1, &station_code,
                           2, &is_gps,
                           3, &station_source, -1);
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
                                        station_country, station_region_id, station_region, is_gps);
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
                                        _("Unknown"), -1, _("Unknown"), FALSE);
        g_object_set_data(G_OBJECT(station), "settings_window_table", (gpointer)main_table);
        g_object_set_data(G_OBJECT(station), "station_box", (gpointer)box);
        gtk_box_pack_start(GTK_BOX(box), station, TRUE, TRUE, 0);
        station_number++;
    }

    return box;
}
/*******************************************************************************/
void
create_and_fill_units_box(GtkWidget *main_table){

  GtkWidget
          *units_button          = NULL,
          *units_label           = NULL,
          *units_description     = NULL,
          *units_box             = NULL;

  gchar   *units_string = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    /* temperature */
    if(app->config->temperature_units == CELSIUS)
        units_string = "C";
    else
        units_string = "F";

    /* distance units */
    if(app->config->distance_units == METERS)
        units_string = g_strjoin(", ", units_string, _("m"), NULL);
        else{
            if(app->config->distance_units == KILOMETERS)
                 units_string = g_strjoin(", ", units_string, _("km"), NULL);
            else{
               if(app->config->distance_units == MILES)
                 units_string = g_strjoin(", ", units_string, _("mph"), NULL);
               else
                 units_string = g_strjoin(", ", units_string, _("mi"), NULL);
            }
        }

    /* wind units */
    if(app->config->wind_units == METERS_S)
        units_string = g_strjoin(", ", units_string, _("m/s"), NULL);
    else{
        if(app->config->wind_units == KILOMETERS_H)
            units_string = g_strjoin(", ", units_string, _("km/h"), NULL);
        else
            units_string = g_strjoin(", ", units_string, _("mi/h"), NULL);
    }

    /* pressure */
    if(app->config->pressure_units == MB)
        units_string = g_strjoin(", ", units_string, _("mb"), NULL);
    else{
        if(app->config->pressure_units == INCH)
            units_string = g_strjoin(", ", units_string, _("inch"), NULL);
        else
            units_string = g_strjoin(", ", units_string, _("mm"), NULL);
    }
    /* TO DO !!!!!! check memory leak in units_string */
    units_button = create_button_with_2_line_text(_("Units"), units_string, 18, 12);

    gtk_widget_set_size_request(units_button, 490, 70);
    gtk_table_attach((GtkTable*)main_table, units_button,
                                1, 2, 3, 4, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    gtk_widget_show (units_button);
    gtk_widget_show (main_table);
    g_object_set_data(G_OBJECT(units_button), "settings_window_table", (gpointer)main_table);
    g_object_set_data(G_OBJECT(units_button), "units_button", (gpointer)units_button);

    g_signal_connect(G_OBJECT(units_button), "button-release-event",
                              G_CALLBACK(units_button_handler), (gpointer)main_table);

}
/*******************************************************************************/
void
create_and_fill_update_box(GtkWidget *main_table){
    GtkWidget
                *update_button          = NULL,
                *update_label           = NULL,
                *update_description     = NULL,
                *update_box             = NULL;

    gchar   *update_string = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif


    if(app->config->update_interval == 0)
        update_string = "never";
    else{
        if(app->config->update_interval == 60)
            update_string = "1 hour";
        else{
            if(app->config->update_interval == 240)
                update_string = "4 hours";
            else{
                if(app->config->update_interval == 1440)
                    update_string = "1 day";
            }
        }
    }

    if(app->config->update_gsm && app->config->update_wlan)
        update_string = g_strjoin(", ", update_string, _("GSM+WLAN"), NULL);
    else{
        if(app->config->update_gsm)
            update_string = g_strjoin(", ", update_string, _("GSM"), NULL);
        else{
            if(app->config->update_wlan)
                update_string = g_strjoin(", ", update_string, _("WLAN"), NULL);
        }
    }

    update_button = create_button_with_2_line_text(_("Update"), update_string, 18, 12);

    gtk_widget_set_size_request(update_button, 490, 70);
    gtk_widget_show (update_button);
    gtk_table_attach((GtkTable*)main_table, update_button,
                                1, 2, 7, 8, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    gtk_widget_show (update_button);
    gtk_widget_show (main_table);
    g_object_set_data(G_OBJECT(update_button), "settings_window_table", (gpointer)main_table);
    g_object_set_data(G_OBJECT(update_button), "update_button", (gpointer)update_button);
    g_signal_connect(G_OBJECT(update_button), "button-release-event",
                                 G_CALLBACK(update_button_handler), (gpointer)main_table);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
void
create_and_fill_widget_style_box(GtkWidget *main_table){
    GtkWidget
               *widget_style_button           = NULL,
               *widget_style_label            = NULL,
               *widget_style_description      = NULL,
               *widget_style_hbox             = NULL,
               *widget_style_vbox             = NULL,
               *widget_style_icon             = NULL,
               *alignmnet                     = NULL;  

    GdkPixbuf *icon_buffer = NULL;
    gchar buffer[256];
    gchar   *widget_style_string = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if(app->config->icons_layout == PRESET_NOW_PLUS_SEVEN)
        widget_style_string = "Now + 7 days vert.";
    else{
        if(app->config->icons_layout == PRESET_NOW_PLUS_TWO)
            widget_style_string = "Now, today and tomorrow";
        else{
            if(app->config->icons_layout == PRESET_NOW_PLUS_THREE_V)
                widget_style_string = "Now + 3 days vert.";
            else{
                if(app->config->icons_layout == PRESET_NOW)
                    widget_style_string = "Now";
                else{
                    if(app->config->icons_layout == PRESET_NOW_PLUS_THREE_H)
                        widget_style_string = "Now + 3 days hor.";
                }
            }
        }
    }

    /*Icon image*/
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%s%s/44.png", ICONS_PATH,
                      (gchar *) (app->config->icon_set));
    icon_buffer =
                 gdk_pixbuf_new_from_file_at_size(buffer, 60,
                                                  60, NULL);
    if (icon_buffer) {
               widget_style_icon = gtk_image_new_from_pixbuf(icon_buffer);
               g_object_unref(G_OBJECT(icon_buffer));
      }
#if defined OS2009
    widget_style_button = create_button_with_2_line_text(_("Widget style"), widget_style_string, 18, 12);
    hildon_button_set_image (HILDON_BUTTON (widget_style_button), widget_style_icon);
    hildon_button_set_image_position (HILDON_BUTTON (widget_style_button), GTK_POS_RIGHT);
#else
    gtk_widget_show(widget_style_description);

    widget_style_button = gtk_button_new ();
    widget_style_label = gtk_label_new(_("Widget style"));

    set_font(widget_style_label, NULL, 12);
//    gtk_widget_set_size_request(widget_style_label, 380, -1);
    gtk_widget_show (widget_style_label);

    widget_style_description = gtk_label_new (widget_style_string);
    set_font(widget_style_description, NULL, 18);
//    gtk_widget_set_size_request(widget_style_description, 380, -1);

     gtk_widget_show(widget_style_description);

      widget_style_vbox = gtk_vbox_new(TRUE, 2);
      gtk_widget_show(widget_style_vbox);

      widget_style_hbox = gtk_hbox_new(FALSE, 0);
      gtk_widget_show(widget_style_hbox);

      gtk_widget_show (widget_style_icon);
     //    gtk_widget_set_size_request(widget_style_icon, 60, -1);

      //    alignmnet = gtk_alignment_new(0, 0, 0, 0);
//    alignmnet = gtk_alignment_new(0, 0, 0, 0);
  //  gtk_widget_set_size_request(alignmnet, -1, 20);
  //  gtk_widget_show(alignmnet);

    gtk_box_pack_start(GTK_BOX(widget_style_vbox), alignmnet, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widget_style_vbox), widget_style_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widget_style_vbox), widget_style_description, TRUE, TRUE, 0);
   // gtk_widget_set_size_request(widget_style_vbox, 380, -1);
    //gtk_box_pack_start(GTK_BOX(widget_style_hbox), alignmnet, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widget_style_hbox), widget_style_vbox, TRUE, TRUE, 0);
   // gtk_box_pack_start(GTK_BOX(widget_style_hbox), alignmnet, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(widget_style_hbox), widget_style_icon, TRUE, TRUE, 0);
   // gtk_box_pack_start(GTK_BOX(widget_style_hbox), alignmnet, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (widget_style_button), widget_style_hbox);

#endif
    gtk_widget_set_size_request(widget_style_button, 490, 70);
    gtk_widget_show (widget_style_button);
    gtk_table_attach((GtkTable*)main_table, widget_style_button,
                                1, 2, 5, 6, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    gtk_widget_show (widget_style_button);
    gtk_widget_show (main_table);
    g_object_set_data(G_OBJECT(widget_style_button), "settings_window_table",
                                        (gpointer)main_table);
    g_object_set_data(G_OBJECT(widget_style_button), "widget_style_button", 
                                        (gpointer)widget_style_button);
    
    g_signal_connect(G_OBJECT(widget_style_button), "button-release-event",
                            G_CALLBACK(widget_style_setup_button_handler),
                           (gpointer)main_table);
}
/*******************************************************************************/
void
weather_simple_window_settings(gpointer user_data){
  GtkWidget
          *window               = NULL,
          *main_table           = NULL,
          *widget_style_button  = NULL,
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
  gint    result;


#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    window = gtk_dialog_new();
    gtk_widget_show(window);
    gtk_window_set_title(GTK_WINDOW(window), _("Settings"));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);

    main_table = gtk_table_new(4,9, FALSE);

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

    stations_box = create_and_fill_stations_buttons(main_table);
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

    create_and_fill_units_box(main_table);

    vertical2_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical2_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignmnet,
                                0, 3, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show(vertical2_alignmnet);

    /*widget_style_button = gtk_button_new_with_label (_("Widget style"));
    gtk_widget_set_size_request(widget_style_button, 490, 70);
    gtk_widget_show (widget_style_button);
    gtk_table_attach((GtkTable*)main_table, widget_style_button,
                                1, 2, 5, 6, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
    g_signal_connect(G_OBJECT(widget_style_button), "button-release-event",
                   G_CALLBACK(widget_style_setup_button_handler),
                   (gpointer)widget_style_button);*/

    create_and_fill_widget_style_box(main_table);

    vertical3_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical3_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignmnet,
                                0, 6, 6, 7,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical3_alignmnet);

    create_and_fill_update_box(main_table);

/*    update_button = gtk_button_new_with_label (_("Update"));
    gtk_widget_set_size_request(update_button, 490, 70);
    gtk_widget_show (update_button);
    gtk_table_attach((GtkTable*)main_table, update_button,
                                1, 2, 7, 8, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    g_signal_connect(G_OBJECT(update_button), "button-release-event",
                           G_CALLBACK(update_button_handler), (gpointer)main_table);
*/
    vertical4_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical4_alignmnet, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical4_alignmnet,
                                0, 6, 8, 9,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical4_alignmnet);

    right_alignmnet = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(right_alignmnet, 5, -1);
    gtk_table_attach((GtkTable*)main_table, right_alignmnet,
                                4, 5, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (right_alignmnet);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       main_table, TRUE, TRUE, 0);
    gtk_dialog_add_button (GTK_DIALOG (window), GTK_STOCK_SAVE, GTK_RESPONSE_YES);

    gtk_widget_show(main_table);

/* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (result == GTK_RESPONSE_YES)
        /* Save config file */
        config_save(app->config);

    if (window)
        gtk_widget_destroy(window);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
