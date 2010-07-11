/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2010 Vlad Vasiliev
 * Copyright (C) 2006-2010 Pavel Fialko
 *
 * Copyright (C) 2008 Andrew Zhilin
 *            az@pocketpcrussia.com
 *  for default icon set (Glance)
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
#include "weather-simple-popup.h"
#include "weather-simple-status.h"
#include "weather-utils.h"
#include "weather-home.h"
#include "weather-hash.h"
#include "weather-popup.h"
#include "weather-data.h"
/*******************************************************************************/
gboolean
jump_panarea(gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#if defined OS2009
    if ((gpointer) g_object_get_data(G_OBJECT(user_data), "length_of_jump"))
         hildon_pannable_area_scroll_to(HILDON_PANNABLE_AREA (user_data),-1, GPOINTER_TO_INT (g_object_get_data(G_OBJECT(user_data), "length_of_jump")));
    else
    if ((GtkWidget*) g_object_get_data(G_OBJECT(user_data), "selected_widget"))
        hildon_pannable_area_scroll_to_child(HILDON_PANNABLE_AREA (user_data),((GtkWidget*) g_object_get_data(G_OBJECT(user_data), "selected_widget")));
#endif
    return FALSE;
}
/*******************************************************************************/
GtkWidget*
create_mainbox_for_forecast_window(GtkWidget* window, gpointer user_data){
    GtkWidget       *view = NULL,
                    *main_vbox = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_vbox = gtk_vbox_new(FALSE, 0);
    g_object_set_data(G_OBJECT(window), "main_vbox", (gpointer)main_vbox);
    g_object_set_data(G_OBJECT(window), "user_data", (gpointer)user_data);
    gtk_box_pack_start(GTK_BOX(main_vbox), create_top_buttons_box(window, user_data), FALSE, TRUE, 0);
    if (app->config->view_mode == COLLAPSED_VIEW_MODE)
        gtk_box_pack_start(GTK_BOX(main_vbox), view = create_weather_collapsed_view(main_vbox, (gint)user_data), TRUE, TRUE, 0);
    else
        gtk_box_pack_start(GTK_BOX(main_vbox), view = create_weather_expanded_view(main_vbox, (gint)user_data), TRUE, TRUE, 0);

    g_timeout_add(100, (GtkFunction) jump_panarea, view);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

    return main_vbox;
}


void
weather_simple_window_popup(GtkWidget *widget, gpointer user_data){
    GtkWidget       *window = NULL;
#if defined OS2009
    HildonAppMenu   *menu = NULL;
#endif
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

#if defined OS2009
    window = hildon_stackable_window_new();
#else
    window = hildon_window_new();
#endif
    gtk_window_set_title(GTK_WINDOW(window), _("Forecast"));

    gtk_container_add(GTK_CONTAINER(window), create_mainbox_for_forecast_window(window, user_data));
#if defined OS2009
    menu = create_view_menu();
    hildon_window_set_app_menu(HILDON_WINDOW(window), menu);
#endif

    if(app->popup_window)
        gtk_widget_destroy(app->popup_window);
    app->popup_window = window;
    gtk_widget_show_all(GTK_WIDGET(window));
    g_signal_connect((gpointer)app->popup_window, "destroy_event",
                        G_CALLBACK(destroy_popup_window), GINT_TO_POINTER(1));
    g_signal_connect((gpointer)app->popup_window, "delete_event",
                        G_CALLBACK(destroy_popup_window), GINT_TO_POINTER(1));
}
/*******************************************************************************/
gchar*
get_next_station_name(const gchar *current_station_name, GtkListStore *user_stations_list){
    GtkTreeIter     iter;
    gboolean        valid,
                    skipped = FALSE,
                    first = FALSE,
                    ready = FALSE;
    gchar           *station_name = NULL;
    GtkTreePath     *path;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current_station_name)
        return NULL;

    path = gtk_tree_path_new_first();
    valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                    &iter, path);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter,
                            NAME_COLUMN, &station_name,
                            -1);
        /* Skip Empty stations */
        if(ready && (!strcmp(station_name, " ") || !strcmp(station_name, _("Unknown")))){
            skipped = TRUE;
            g_free(station_name);
            station_name = NULL;
            gtk_tree_path_next(path);
            valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter, path);
            if(!valid){
                gtk_tree_path_free(path);
                path = gtk_tree_path_new_first();
                valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter);
                if (first)
                    break;
                else
                    first = TRUE;
            }
            continue;
        }
        if(ready){
            break;
        }
        else{
            if(skipped || ((app->config->current_station_name) && (station_name) &&
                  !strcmp(app->config->current_station_name, station_name)))
                ready = TRUE;
            g_free(station_name);
            station_name = NULL;
            gtk_tree_path_next(path);

            valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter, path);
            if(!valid){
                valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter);
            }
        }
    }
    gtk_tree_path_free(path);
    return station_name;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

    return NULL;
}
/*******************************************************************************/
void
weather_simple_window_redraw(GtkWidget *window){
    GtkWidget* main_vbox;
    gpointer user_data;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    main_vbox = (GtkWidget*)g_object_get_data(G_OBJECT(window), "main_vbox");
    user_data = g_object_get_data(G_OBJECT(window), "user_data");
    #if defined CLUTTER || defined HILDONANIMATION
        free_clutter_objects_list(&app->clutter_objects_in_popup_form);
    #endif
    if (main_vbox)
        gtk_widget_destroy(main_vbox);
    main_vbox =  create_mainbox_for_forecast_window(window, user_data);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);
    gtk_widget_show(main_vbox);
    gtk_widget_show(window);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
GtkWidget*
create_top_buttons_box(GtkWidget* window, gpointer user_data){
    GtkWidget       *buttons_box = NULL,
                    *station_button = NULL,
                    *update_button = NULL;
    gchar           buffer[255],
                    full_filename[2048];
    struct stat     statv;
    gchar           *next_station = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

/* buttons */
    buttons_box = gtk_hbox_new(TRUE, 0);
    /* station button */
    *buffer = 0;
    next_station = get_next_station_name(app->config->current_station_name,
                                        app->user_stations_list);
    if (next_station){
        snprintf(buffer, sizeof(buffer) - 1, "%s: %s", _("next"), next_station);
        g_free(next_station);
    }else
        snprintf(buffer, sizeof(buffer) - 1, "%s: ", _("next"));
    station_button = create_button_with_2_line_text(app->config->current_station_name,
                                                    buffer, 18, 12);
    g_signal_connect(G_OBJECT(station_button), "button-release-event",
                            G_CALLBACK(change_station_next), window);
    /* prepare last update time*/
    if(app->station_data){
        *full_filename = 0;
        snprintf(full_filename, sizeof(full_filename) - 1, "%s/%s.xml",
                    app->config->cache_dir_name, app->config->current_station_id);
        if(stat(full_filename, &statv))
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", _("Last update: "), _("Unknown"));
        else{
            snprintf(buffer, sizeof(buffer) - 1, _("Last update: "));
            strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                    "%X %x", localtime(&statv.st_mtime));
        }
    }
    else{
        *buffer = 0;
        snprintf(buffer, sizeof(buffer) - 1, "%s%s", _("Last update: "), _("Unknown"));
    }
    /* update button */
    update_button = create_button_with_2_line_text(Q_("Action|Update"), buffer, 18, 12);
    g_signal_connect(G_OBJECT(update_button), "button-release-event",
                        G_CALLBACK(refresh_button_handler), NULL);


    gtk_box_pack_start(GTK_BOX(buttons_box), station_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttons_box), update_button, TRUE, TRUE, 0);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    gtk_widget_show(buttons_box);
    return buttons_box;
}
/*******************************************************************************/
GtkWidget*
create_weather_collapsed_view(GtkWidget *vbox, gint day_number){
    GtkWidget       *scrolled_window = NULL,
                    *main_vbox = NULL,
                    *line_hbox = NULL,
                    *line_text = NULL,
                    *line = NULL,
                    *label = NULL,
                    *previos_separator = NULL,
#if !defined OS2009
                    *vscrollbar = NULL,
#endif
                    *separator = NULL;
    GdkPixbuf       *icon_buffer;
    GtkWidget       *icon_image;
    const gchar     *wind_units_str[] = { "m/s", "km/h", "mi/h" };
    gchar           buffer[1024],
                    tmp[512],
                    icon[2048],
                    symbol = 'C',
                    *comma = NULL;
    GSList          *days = NULL;
    GHashTable      *day = NULL;
    gint            i = 0,
                    hi_temp,
                    low_temp;
    struct tm       tmp_time_date_struct = {0};
    gint            font_size = 12;
    gint            icon_size = 12;
    gint                length_to_selected = 1;
    gint                pre_length_to_selected = 1;
    gint                offset = 0,
                        offset1 = 0,
                        offset2 = 0,
                        offset3 = 0,
                        offset4 = 0,
                        offset5 = 0,
                        offset6 = 0,
                        whole_offset = 180,
                        offset_default = 0;
    GtkRequisition      requisition;
    gboolean         hyphenation;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_vbox = gtk_vbox_new(FALSE, 5);
#if defined OS2009
    scrolled_window = hildon_pannable_area_new ();
    g_object_set(G_OBJECT(scrolled_window), "mov-mode", HILDON_MOVEMENT_MODE_BOTH, NULL);
    hildon_pannable_area_add_with_viewport(HILDON_PANNABLE_AREA (scrolled_window), GTK_WIDGET (main_vbox));
#else
    /* scrolled window */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window),
                                            GTK_SHADOW_NONE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    vscrollbar = gtk_scrolled_window_get_vscrollbar(scrolled_window);
    hildon_helper_set_thumb_scrollbar (scrolled_window, TRUE);
    /* pack childs to the scrolled window */
    gtk_scrolled_window_add_with_viewport(scrolled_window, main_vbox);
#endif
#if defined CLUTTER || HILDONANIMATION
    /* For end of Clutter animation in popup window */
        free_clutter_objects_list(&app->clutter_objects_in_popup_form);
#endif
    days = (GSList*)g_hash_table_lookup(app->station_data, "forecast");
    if(days){
        while(days){
            hyphenation = FALSE;
            day = (GHashTable*)(days->data);
            line = gtk_button_new();
            gtk_button_set_focus_on_click(GTK_BUTTON(line), FALSE);
            gtk_button_set_relief(GTK_BUTTON(line), GTK_RELIEF_NONE);
            g_signal_connect(G_OBJECT(line), "clicked",
                                G_CALLBACK(show_expanded_day_button_handler),
                                 GINT_TO_POINTER(i));
            g_object_set_data(G_OBJECT(line), "scrolled_window", (gpointer)scrolled_window);
            g_object_set_data(G_OBJECT(line), "vbox", (gpointer)vbox);

            /* line box */
            line_hbox = gtk_hbox_new(FALSE, 0);
            gtk_container_add(GTK_CONTAINER(line), line_hbox);
            /* icon */
            update_icons_set_base(NULL);
            *buffer = 0;
            snprintf(icon, sizeof(icon) - 1, "%s%s.png", app->config->icons_set_base,
                        (char*)g_hash_table_lookup(day, "day_icon"));
            switch (app->config->scale_in_popup){
                default:
                case 1: icon_size = SMALL_ICON_SIZE; break;
                case 2: icon_size = MEDIUM_ICON_SIZE; break;
                case 3: icon_size = BIG_ICON_SIZE; break;
                case 4: icon_size = LARGE_ICON_SIZE; break;
                case 5: icon_size = GIANT_ICON_SIZE; break;
                case 6: icon_size = SUPER_GIANT_ICON_SIZE; break;
            }
            icon_buffer = gdk_pixbuf_new_from_file_at_size(icon, icon_size,
                                                           icon_size, NULL);

/*            icon_image = create_icon_widget(icon_buffer, icon, SMALL_ICON_SIZE, &app->clutter_objects_in_popup_form); */
            icon_image = create_icon_widget(icon_buffer, icon, icon_size, NULL);
            if(icon_image){
                gtk_box_pack_start(GTK_BOX(line_hbox), icon_image, FALSE, TRUE, 0);
                gtk_box_pack_start(GTK_BOX(main_vbox), line, TRUE, TRUE, 0);
                gtk_box_pack_start(GTK_BOX(main_vbox), separator = gtk_hseparator_new(), TRUE, TRUE, 0);
            }
            /* day label */
            *buffer = 0;
            sprintf(buffer,"%s %s", (char*)g_hash_table_lookup(day, "day_name"),
                        (char*)g_hash_table_lookup(day, "day_date"));
            strptime(buffer, "%A %b %d", &tmp_time_date_struct);
            *buffer = 0;
            strftime(tmp, sizeof(tmp) - 1, "%A, %d %B", &tmp_time_date_struct);
            if(i == 0){
                comma = strchr(tmp, ',');
                if(comma)
                    snprintf(buffer, sizeof(buffer) - 1, "<span weight=\"bold\">%s%s", _("Today"), comma);
                else
                    snprintf(buffer, sizeof(buffer) - 1, "<span weight=\"bold\">%s%s", _("Today"), tmp);
            }
            else
                snprintf(buffer, sizeof(buffer) - 1, "<span weight=\"bold\">%s", tmp);
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1, "</span>");
            /* day text */
            *tmp = 0;
            snprintf(tmp, sizeof(tmp) - 1, "\n%s ", _("d:"));
            /* prepare temperature */
            if(!g_hash_table_lookup(day, "day_hi_temperature") ||
                   !strcmp(g_hash_table_lookup(day, "day_hi_temperature"), "N/A")){
                hi_temp = INT_MAX;
            }
            else
                hi_temp = atoi(g_hash_table_lookup(day, "day_hi_temperature"));
            if(!g_hash_table_lookup(day, "day_low_temperature") ||
                    !strcmp(g_hash_table_lookup(day, "day_low_temperature"), "N/A")){
                low_temp = INT_MAX;
            }
            else
                low_temp = atoi(g_hash_table_lookup(day, "day_low_temperature"));
            if(app->config->temperature_units == FAHRENHEIT){
                if(hi_temp != INT_MAX)
                    hi_temp = c2f(hi_temp);
                if(low_temp != INT_MAX)
                    low_temp = c2f(low_temp);
                symbol = 'F';
            }
            /* day temperature */
            if(hi_temp == INT_MAX)
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "<span weight=\"bold\">%s</span>, ",
                            (char*)hash_table_find("N/A", FALSE));
            else
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "<span weight=\"bold\">%d\302\260%c</span>, ",
                            hi_temp, symbol);
            /* day title */
            if(g_hash_table_lookup(day, "day_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%s, ",
                            (char*)hash_table_find(g_hash_table_lookup(day, "day_title"), FALSE));
            
            if ((app->config->scale_in_popup > 3 && strlen(tmp) > 65)|| app->config->scale_in_popup == 6)
                hyphenation = TRUE;
            if (hyphenation)
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"\n    ");
            
            /* wind speed */
            if(g_hash_table_lookup(day, "day_wind_speed"))
                switch(app->config->wind_units){
                    case BEAUFORT_SCALE:
                        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"%i ",
                                    (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed"))));
                    break;
                    case KILOMETERS_H:
                        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"%i %s ",
                                    (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed"))),
                                    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                    break;
                    default:
                        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"%.2f %s ",
                                    convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed"))),
                                    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                    break;
                }
            /* wind direction */
            if(g_hash_table_lookup(day, "day_wind_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "(%s)\n",
                            (char*)hash_table_find(g_hash_table_lookup(day, "day_wind_title"), FALSE));
            snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%s ", _("n:"));
            /* night temperature */
            if(low_temp == INT_MAX)
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "<span weight=\"bold\">%s</span>, ",
                            (char*)hash_table_find("N/A", FALSE));
            else
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "<span weight=\"bold\">%d\302\260%c</span>, ",
                            low_temp, symbol);
            /* night title */
            if(g_hash_table_lookup(day, "night_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%s, ",
                            (char*)hash_table_find(g_hash_table_lookup(day, "night_title"), FALSE));

            if (hyphenation)
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"\n    ");
 
            /* wind speed */
            if(g_hash_table_lookup(day, "night_wind_speed"))
                switch(app->config->wind_units){
                    case BEAUFORT_SCALE:
                        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"%i ",
                                    (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))));
                    break;
                    case KILOMETERS_H:
                    break;
                        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"%i %s ",
                                    (int)convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))),
                                    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));

                    default:
                        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"%.2f %s ",
                                    convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))),
                                    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                    break;
                }
            /* wind direction */
            if(g_hash_table_lookup(day, "night_wind_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "(%s)",
                            (char*)hash_table_find(g_hash_table_lookup(day, "night_wind_title"), FALSE));

            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1, "<i>%s</i>", tmp);

            line_text = gtk_label_new(NULL);
            gtk_label_set_justify(GTK_LABEL(line_text), GTK_JUSTIFY_FILL);
            gtk_label_set_markup(GTK_LABEL(line_text), buffer);

	        switch (app->config->scale_in_popup){
                case 2: font_size = 14; break;     
                case 3: font_size = 16; break;
                case 4: font_size = 18; break;
                case 5: font_size = 21; break;
	            case 6: font_size = 24; break;
                case 1:  	      
                default: font_size = 12; break;
            }
 
            set_font(line_text, NULL, font_size);

            gtk_box_pack_start(GTK_BOX(line_hbox), line_text, FALSE, TRUE, 10);

            /* Calculate ofset for jump */
            gtk_widget_size_request (line_text, &requisition);
            pre_length_to_selected = length_to_selected;
            length_to_selected = length_to_selected + requisition.height;
            gtk_widget_size_request (separator, &requisition);
            length_to_selected = length_to_selected + requisition.height;

            if(day_number == i + 1){
                switch (app->config->scale_in_popup){
                    case 2:  
                            offset2 = 70; 
                            offset_default = 43; 
                            break;     
                    case 3:  
                            offset2 = 90; 
                            offset_default = 55; 
                            break;
                    case 4:  
                            offset2 = 115; 
                            offset_default = 73; 
                            break;
                    case 5:  
                            offset2 = 190; 
                            offset_default = 105; 
                            break;
                    case 6: offset2 = 420; 
                            offset_default = 210;
                            break;
                    case 1:  	      
                    default:
                            offset2 = 60; 
                            offset_default = 35; 
                             break;

                }
                previos_separator = separator;
                switch (i){
                    case 0: break;
    //                case 1: if (current_widget && current) 
    //                            pre_length_to_selected = 0; 
    //                        break;
                    case 2: offset = offset2;break;
                    default: offset = i*offset_default;break;
                }
     
               g_object_set_data(G_OBJECT(scrolled_window), "length_of_jump", 
                       GINT_TO_POINTER(pre_length_to_selected+offset + whole_offset));
        }
        
        //    if(day_number == i)
        //        g_object_set_data(G_OBJECT(scrolled_window), "selected_widget", (gpointer)separator);
            /* next day */
            days = g_slist_next(days);
            i++;
        }
//	if(day_number == i)
//                g_object_set_data(G_OBJECT(scrolled_window), "selected_widget", (gpointer)separator);
    }
    else{ /* no weather data */
        gtk_box_pack_start(GTK_BOX(main_vbox),
                            label = gtk_label_new(_("No weather data for this station.")), TRUE, TRUE, 0);
        set_font(label, NULL, 24);
    }
    gtk_widget_show_all(scrolled_window);

#ifdef HILDONANIMATION 
    g_signal_connect_after(scrolled_window, "expose-event", G_CALLBACK(popup_simple_window_expose), NULL);
#endif

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

    return scrolled_window;
}
/*******************************************************************************/
GtkWidget*
create_weather_expanded_view(GtkWidget *vbox, gint day_number){
    gint                i = 0;
    GHashTable          *current = NULL,
                        *day = NULL;
    GSList              *days = NULL;
    GtkWidget           *day_widget = NULL,
                        *current_widget = NULL,
                        *main_vbox = NULL,
                        *line = NULL,
                        *separator = NULL,
#if !defined OS2009
                        *vscrollbar = NULL,
#endif
                        *previos_separator = NULL;
    gchar               *day_name = NULL;
    GtkWidget           *scrolled_window = NULL;
    GSList              *tmp = NULL;
    GtkRequisition      requisition;
    gint                length_to_selected = 1;
    gint                pre_length_to_selected = 1;
    gint                offset = 0;
    gint                offset1 = 0;
    gint                offset2 = 0;
    gint                offset3 = 0;
    gint                offset4 = 0;
    gint                offset5 = 0;
    gint                offset_default = 0;
    gint                whole_offset = 0;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_vbox = gtk_vbox_new(FALSE, 5);
    /* scrolled window */
#if defined OS2009
    scrolled_window = hildon_pannable_area_new();
    g_object_set(G_OBJECT(scrolled_window), "mov-mode", HILDON_MOVEMENT_MODE_BOTH, NULL);
    hildon_pannable_area_add_with_viewport(HILDON_PANNABLE_AREA(scrolled_window), GTK_WIDGET(main_vbox));
#else
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window),
                                            GTK_SHADOW_NONE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                       GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    hildon_helper_set_thumb_scrollbar(scrolled_window, TRUE);
    gtk_scrolled_window_add_with_viewport(scrolled_window, main_vbox);
#endif
#if defined CLUTTER || HILDONANIMATION
    /* For end of Clutter animation in popup window */
        free_clutter_objects_list(&app->clutter_objects_in_popup_form);
#endif
 
    gtk_widget_set_size_request(scrolled_window, -1, -1);
    if(!app->station_data)
        return NULL;
    current = (GHashTable*)g_hash_table_lookup(app->station_data, "current");
    days = (GSList*)g_hash_table_lookup(app->station_data, "forecast");
    if(!days)
        return NULL;

    if(day_number == 0){ /* if selected Today, than adding Now, if it aviable */
        /* prepare for Now data */
        if(app->current_is_valid){
            line = gtk_button_new();
            gtk_button_set_focus_on_click(GTK_BUTTON(line), FALSE);
            gtk_button_set_relief(GTK_BUTTON(line), GTK_RELIEF_NONE);
            g_signal_connect(G_OBJECT(line), "clicked",
                                G_CALLBACK(show_detailes_of_current_day_button_handler),
                                 GINT_TO_POINTER(i+1-app->config->separate));
            g_object_set_data(G_OBJECT(line), "scrolled_window", (gpointer)scrolled_window);
            g_object_set_data(G_OBJECT(line), "vbox", (gpointer)vbox);
            current_widget = create_current_tab(current);
            if(current){
                gtk_container_add(GTK_CONTAINER(line), current_widget);
                gtk_widget_size_request (current_widget, &requisition);
                length_to_selected = requisition.height;
                gtk_box_pack_start(GTK_BOX(main_vbox), line, FALSE, TRUE, 0);
                gtk_box_pack_start(GTK_BOX(main_vbox), separator = gtk_hseparator_new(), FALSE, TRUE, 0);
                gtk_widget_size_request (separator, &requisition);
                length_to_selected = length_to_selected + requisition.height;
            }
        }
    }

    /* Day tabs */
    tmp = g_hash_table_lookup(app->station_data, "forecast");
    while(tmp && i < Max_count_weather_day){
        day = (GHashTable*)tmp->data;
            line = gtk_button_new();
            gtk_button_set_focus_on_click(GTK_BUTTON(line), FALSE);
            gtk_button_set_relief(GTK_BUTTON(line), GTK_RELIEF_NONE);
            if (i == 0)
                g_signal_connect(G_OBJECT(line), "clicked",
                                            G_CALLBACK(show_detailes_of_current_day_button_handler),
                                                                             GINT_TO_POINTER(i+1));
            else
                g_signal_connect(G_OBJECT(line), "clicked",
                                G_CALLBACK(show_collapsed_day_button_handler),
                                 GINT_TO_POINTER(i+1));

        g_object_set_data(G_OBJECT(line), "scrolled_window", (gpointer)scrolled_window);
        g_object_set_data(G_OBJECT(line), "vbox", (gpointer)vbox);

        day_widget = create_day_tab(current, day, &day_name);
        gtk_container_add(GTK_CONTAINER(line), day_widget);
        gtk_box_pack_start(GTK_BOX(main_vbox), line, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(main_vbox), separator = gtk_hseparator_new(), FALSE, TRUE, 0);
        /* Calculate ofset for jump */
        gtk_widget_size_request (day_widget, &requisition);
        pre_length_to_selected = length_to_selected;
        length_to_selected = length_to_selected + requisition.height;
        gtk_widget_size_request (separator, &requisition);
        length_to_selected = length_to_selected + requisition.height;

        if(day_number == i && !(i == 0 && current_widget && current))
            previos_separator = separator;
        /* If activited day and not current weather */
        if(day_number  == i && !(i == 0 && current_widget && current)){
            g_object_set_data(G_OBJECT(scrolled_window), "selected_widget", (gpointer)previos_separator);
            switch (app->config->scale_in_popup){
                case 2:  
                        offset1 = 110; 
                        offset2 = 190; 
                        offset3 = 90; 
                        offset4 = 85; 
                        offset5 = 80; 
                        offset_default = 77;   
                        whole_offset = 140;
                        break;     
                case 3:  
                        offset1 = 140; 
                        offset2 = 240; 
                        offset3 = 110; 
                        offset4 = 115; 
                        offset5 = 110; 
                        offset_default = 110; 
                        whole_offset = 140;
                        break;
                case 4:  
                        offset1 = 175; 
                        offset2 = 300; 
                        offset3 = 140; 
                        offset4 = 130; 
                        offset5 = 134; 
                        offset_default = 130; 
                        whole_offset = 140;
                        break;
                case 5:  
                        offset1 = 170; 
                        offset2 = 320; 
                        offset3 = 140; 
                        offset4 = 140; 
                        offset5 = 140; 
                        offset_default = 135; 
                        whole_offset = 140;
                        break;
                case 6: 
                        offset1 = 125;
                        offset2 = 250;
                        offset3 = 110; 
                        offset4 = 110; 
                        offset5 = 120; 
                        offset_default = 120;
                        whole_offset = 140;
                        break;
                case 1:  	      
                default:
                        offset1 = 251; 
                        offset2 = 310; 
                        offset3 = 120; 
                        offset4 = 105; 
                        offset5 = 95; 
                        offset_default = 87; 
                        break;
            }

            switch (i){
                case 0: whole_offset = 0;break;
                case 1: if (current_widget && current) 
                            pre_length_to_selected = 0; 
                            offset = offset1;
                        break;
                case 2: offset = offset2;break;
                case 3: offset = i*offset3;break;
                case 4: offset = i*offset4;break;
                case 5: offset = i*offset5;break;
                default: offset = i*offset_default;break;
            }
            g_object_set_data(G_OBJECT(scrolled_window), "length_of_jump", GINT_TO_POINTER(pre_length_to_selected+offset + whole_offset));
        }
        if (day_number == Max_count_weather_day - 1 && i == Max_count_weather_day - 1){
            g_object_set_data(G_OBJECT(scrolled_window), "selected_widget", (gpointer)line);
        }
        tmp = g_slist_next(tmp);
        i++;
        g_free(day_name);
    }
    if(day_number + 1 == i){
        g_object_set_data(G_OBJECT(scrolled_window), "selected_widget", (gpointer)separator);
    }

   
    gtk_widget_show_all(main_vbox);
    gtk_widget_show_all(scrolled_window);
#ifdef HILDONANIMATION 
    g_signal_connect_after(scrolled_window, "expose-event", G_CALLBACK(popup_simple_window_expose), NULL);
#endif
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return scrolled_window;
}
/*******************************************************************************/
void
show_collapsed_day_button_handler(GtkWidget *button, gpointer user_data){
    GtkWidget *view;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    view = create_weather_collapsed_view(g_object_get_data(G_OBJECT(button), "vbox"), GPOINTER_TO_INT(user_data));
    gtk_widget_destroy(g_object_get_data(G_OBJECT(button), "scrolled_window"));
    gtk_box_pack_start(GTK_BOX(g_object_get_data(G_OBJECT(button), "vbox")), view, TRUE, TRUE, 0);
    g_timeout_add(100, (GtkFunction) jump_panarea, view);
}
/*******************************************************************************/
void
show_expanded_day_button_handler(GtkWidget *button, gpointer user_data){
    GtkWidget *view;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    view = create_weather_expanded_view(g_object_get_data(G_OBJECT(button), "vbox"), GPOINTER_TO_INT(user_data));
    gtk_widget_destroy(g_object_get_data(G_OBJECT(button), "scrolled_window"));
    gtk_box_pack_start(GTK_BOX(g_object_get_data(G_OBJECT(button), "vbox")), view, TRUE, TRUE, 0);
    g_timeout_add(100, (GtkFunction) jump_panarea, view);
}
/*******************************************************************************/
void
show_detailes_of_current_day_button_handler(GtkWidget *button, gpointer user_data){
    GtkWidget *view;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    view = create_weather_for_two_hours_collapsed_view(g_object_get_data(G_OBJECT(button), "vbox"), GPOINTER_TO_INT(user_data));
    if (!view)
        view = create_weather_collapsed_view(g_object_get_data(G_OBJECT(button), "vbox"), GPOINTER_TO_INT(user_data));
    gtk_widget_destroy(g_object_get_data(G_OBJECT(button), "scrolled_window"));
    gtk_box_pack_start(GTK_BOX(g_object_get_data(G_OBJECT(button), "vbox")), view, TRUE, TRUE, 0);
    g_timeout_add(100, (GtkFunction) jump_panarea, view);
}
/*******************************************************************************/
GtkWidget*
create_weather_for_two_hours_collapsed_view(GtkWidget *vbox, gint day_number){
    GtkWidget       *scrolled_window = NULL,
                    *main_vbox = NULL,
                    *line = NULL,
                    *line_text = NULL,
#if !defined OS2009
                    *vscrollbar = NULL,
#endif
                    *line_hbox = NULL;
    GdkPixbuf       *icon;
    GtkWidget       *icon_image;
    const gchar     *wind_units_str[] = {"m/s", "km/h", "mi/h" };
    gchar           buffer[1024],
                    tmp[512],
                    buff[1024],
                    hour_last_update[1024];
    GHashTable      *hour_weather = NULL;
    GSList          *hours_weather = NULL;
    gint            timezone = 0,
                    count = 0;
    time_t          current_time,
                    hours_time,
                    utc_time;
    gdouble         difference = -1,
                    prev_difference = -1;
    struct tm       tmp_tm = {0};
    size_t          date_size = 0;
    gboolean        flag = FALSE,
                    new_day = FALSE;
    struct tm       *gmt;
    gint            font_size = 12;
    gint            icon_size = SMALL_ICON_SIZE;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_vbox = gtk_vbox_new(FALSE, 5);
    /* scrolled window */
#if defined OS2009
    scrolled_window = hildon_pannable_area_new ();
    g_object_set(G_OBJECT(scrolled_window), "mov-mode", HILDON_MOVEMENT_MODE_BOTH, NULL);
    hildon_pannable_area_add_with_viewport(HILDON_PANNABLE_AREA (scrolled_window), GTK_WIDGET (main_vbox));
#else
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_add_with_viewport(scrolled_window, main_vbox);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window),
                                            GTK_SHADOW_NONE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                       GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    vscrollbar = gtk_scrolled_window_get_vscrollbar(scrolled_window);
    hildon_helper_set_thumb_scrollbar (scrolled_window, TRUE);
#endif
    gtk_widget_set_size_request (scrolled_window, -1, 300);

    hours_weather = g_hash_table_lookup(g_hash_table_lookup(app->station_data, "detail"), "hours_data");

    if(hours_weather){
        while(hours_weather){
            hour_weather = (GHashTable*)hours_weather->data;

            count++;

          /*Get time in current location*/
            current_time = time(NULL);
            gmt = gmtime(&current_time);
            gmt->tm_isdst = 1;
            utc_time = mktime(gmt);
            timezone = atol(g_hash_table_lookup(g_hash_table_lookup(app->station_data, 
                                    "location"), "station_time_zone"));
            current_time = utc_time + 60 * 60 *timezone;

            *hour_last_update = 0;
            memset(buff, 0, sizeof(buff));

            /*Prepare date from xml file*/
            snprintf(hour_last_update + strlen(hour_last_update),
                    sizeof(hour_last_update) - strlen(hour_last_update) - 1,
                    "%s",
                    (char*)g_hash_table_lookup(g_hash_table_lookup(app->station_data, 
                                               "detail"), "last_update"));
            date_size = strcspn(hour_last_update, " ");
            strncpy(buff, hour_last_update, date_size);

            *hour_last_update = 0;
            strcat(hour_last_update, buff);
            strcat(hour_last_update, " ");
            if (g_hash_table_lookup(hour_weather, "hours")){
                strcat(hour_last_update, (char*)g_hash_table_lookup(hour_weather, "hours"));
                strcat(hour_last_update, ":00:00");
            }

            strptime(hour_last_update, "%D %T", &tmp_tm);
            hours_time = mktime(&tmp_tm);
            if (g_hash_table_lookup(hour_weather, "hours")){
                if((count > 1) && 
                            (!strcmp((char*)g_hash_table_lookup(hour_weather, "hours"), "00")))
                    new_day = TRUE;
                if(new_day)
                    hours_time += 24*60*60;
                difference = difftime(hours_time, current_time);
            }    
            if((difference < 0 && difference > -60*60) || difference >= 0 ||
                                      (prev_difference > 0 &&  difference<0)) {
                flag = TRUE; 
                line =  gtk_button_new();
                gtk_button_set_focus_on_click(GTK_BUTTON(line), FALSE);
                            gtk_button_set_relief(GTK_BUTTON(line), GTK_RELIEF_NONE);
                *buffer = 0;
                g_object_set_data(G_OBJECT(line), "scrolled_window",
                                                       (gpointer)scrolled_window);
                g_object_set_data(G_OBJECT(line), "vbox", (gpointer)vbox);
                g_signal_connect(G_OBJECT(line), "clicked",
                                G_CALLBACK(show_collapsed_day_button_handler),
                                GINT_TO_POINTER(day_number));
                line_hbox = gtk_hbox_new(FALSE, 0);
                gtk_container_add(GTK_CONTAINER(line), line_hbox);

                /*icon*/
                update_icons_set_base(NULL);
                sprintf(buffer,"%s%s.png", app->config->icons_set_base,
                           (char*)g_hash_table_lookup(hour_weather, "hour_icon"));
                switch (app->config->scale_in_popup){
                    default:
                    case 1: icon_size = SMALL_ICON_SIZE; break;
                    case 2: icon_size = MEDIUM_ICON_SIZE; break;
                    case 3: icon_size = BIG_ICON_SIZE; break;
                    case 4: icon_size = LARGE_ICON_SIZE; break;
                    case 5: icon_size = GIANT_ICON_SIZE; break;
                    case 6: icon_size = SUPER_GIANT_ICON_SIZE; break;
                }
                icon = gdk_pixbuf_new_from_file_at_size(buffer, icon_size,
                                                           icon_size, NULL);

/*            icon_image = create_icon_widget(icon_buffer, icon, SMALL_ICON_SIZE, &app->clutter_objects_in_popup_form); */
                icon_image = create_icon_widget(icon, buffer, icon_size, NULL);

                if(icon_image){
                    gtk_box_pack_start(GTK_BOX(line_hbox), icon_image, FALSE, TRUE, 0);
                    gtk_box_pack_start(GTK_BOX(main_vbox), line, TRUE, TRUE, 0);
                }

                *buffer = 0;
                *tmp = 0;
                /* hour */
                sprintf(tmp + strlen(tmp), "%s", _("Forecast at: "));
                sprintf(tmp + strlen(tmp), "%s:00\n",
                                (char*)g_hash_table_lookup(hour_weather, "hours"));
                snprintf(buffer, sizeof(buffer) - 1, 
                                                "<span weight=\"bold\">%s", tmp);
                snprintf(buffer + strlen(buffer), 
                                    sizeof(buffer) - strlen(buffer) - 1,"</span>");
                *tmp = 0;
                /* temperature */
                if (g_hash_table_lookup(hour_weather, "hour_temperature")){
                    sprintf(tmp, " %d\302\260",
                            ((app->config->temperature_units == CELSIUS) ?
                           ( atoi(g_hash_table_lookup(hour_weather, "hour_temperature"))) :
                      ( (int)c2f(atoi(g_hash_table_lookup(hour_weather, "hour_temperature"))))));
                    (app->config->temperature_units == CELSIUS) ? ( strcat(tmp, _("C")))
                                                   : ( strcat(tmp, _("F")));
                 }

                /* feels like */
                if (g_hash_table_lookup(hour_weather, "hour_feels_like")){
                    sprintf(tmp + strlen(tmp), " (%s  %d\302\260", _("feels like"),
                                 (app->config->temperature_units == CELSIUS) ?
                                 (atoi(g_hash_table_lookup(hour_weather, "hour_feels_like"))) :
                        ((int)c2f(atoi(g_hash_table_lookup(hour_weather, "hour_feels_like")))));
                    (app->config->temperature_units == CELSIUS) ? ( strcat(tmp, _("C")))
                                                                 : ( strcat(tmp, _("F")));
                    sprintf(tmp + strlen(tmp), ")");
                }

                /* title */
                if(g_hash_table_lookup(hour_weather, "hour_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1," , %s, ",
                       (char*)hash_table_find(g_hash_table_lookup(hour_weather, "hour_title"), 
                        FALSE));

                if (app->config->scale_in_popup > 4)
                        sprintf(tmp + strlen(tmp), "\n");

                /* wind speed */
                if(g_hash_table_lookup(hour_weather, "hour_wind_speed"))
                    switch(app->config->wind_units){
                        case BEAUFORT_SCALE:
                            snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,
                                        "%i ", (int)convert_wind_units(app->config->wind_units, 
                                                                   atof(g_hash_table_lookup(hour_weather, "hour_wind_speed"))));
                        break;
                        case KILOMETERS_H:
                            snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,
                                        "%i %s ", (int)convert_wind_units(app->config->wind_units, 
                                       atof(g_hash_table_lookup(hour_weather, "hour_wind_speed"))),
                                        (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                        break;
                        default:
                            snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,
                                        "%.2f %s ", convert_wind_units(app->config->wind_units, 
                                       atof(g_hash_table_lookup(hour_weather, "hour_wind_speed"))),
                                        (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
                        break;
                    }
                /* wind direction */
                if(g_hash_table_lookup(hour_weather, "hour_wind_direction"))
                  snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"(%s)",
                        (char*)hash_table_find(g_hash_table_lookup(hour_weather,
                                                           "hour_wind_direction"),
                                                            FALSE));

                /* humidity */
                if(g_hash_table_lookup(hour_weather, "hour_humidity"))
                    sprintf(tmp + strlen(tmp), ",  %d%%",
                         atoi(g_hash_table_lookup(hour_weather, "hour_humidity")));
                snprintf(buffer + strlen(buffer), 
                                        sizeof(buffer) - strlen(buffer) - 1, 
                                        "<i>%s</i>", tmp);


                line_text = gtk_label_new(NULL);
                gtk_label_set_justify(GTK_LABEL(line_text), GTK_JUSTIFY_FILL);
                gtk_label_set_markup(GTK_LABEL(line_text), buffer);

                switch (app->config->scale_in_popup){
                    case 2: font_size = 14; break;     
                    case 3: font_size = 16; break;
                    case 4: font_size = 18; break;
                    case 5: font_size = 21; break;
                    case 6: font_size = 24; break;
                    case 1:  	      
                    default: font_size = 12; break;
                }
                set_font(line_text, NULL, font_size);

                gtk_box_pack_start(GTK_BOX(line_hbox), line_text, FALSE, TRUE, 10);
                gtk_box_pack_start(GTK_BOX(main_vbox), gtk_hseparator_new(), FALSE, TRUE, 0);
                hours_weather = g_slist_next(hours_weather);

                prev_difference = difference;
        }
        else{
            prev_difference = difference;
            hours_weather = g_slist_next(hours_weather);
            difference = 0;
        }

        if(!hours_weather)
             break;
     }

    }else{
    /* Not hours data - return NULL */
        gtk_widget_destroy(scrolled_window);
        return NULL;
    }
 if (flag){
    gtk_widget_show_all(scrolled_window);
    return scrolled_window;
 }else{
    gtk_widget_destroy(scrolled_window);
    return NULL;
 }

}
/*******************************************************************************/
#if defined OS2009
HildonAppMenu*
create_view_menu(void){
    HildonAppMenu       *menu = NULL;
    GtkWidget           *menu_item;
    menu = HILDON_APP_MENU(hildon_app_menu_new());
    /* settings item */
    menu_item = hildon_gtk_button_new(HILDON_SIZE_AUTO);
    gtk_button_set_label(GTK_BUTTON(menu_item), _("Settings"));
    g_signal_connect_after(menu_item, "clicked",
                        G_CALLBACK(simple_settings_button_handler), NULL);
    gtk_widget_show_all(menu_item);
    hildon_app_menu_append(menu, GTK_BUTTON(menu_item));
    /* help item */
    menu_item = hildon_gtk_button_new(HILDON_SIZE_AUTO);
    gtk_button_set_label(GTK_BUTTON(menu_item), _("About"));
    g_signal_connect_after(menu_item, "clicked",
                        G_CALLBACK(about_button_handler), NULL);
    gtk_widget_show_all(menu_item);
    hildon_app_menu_append(menu, GTK_BUTTON(menu_item));
    /* donation item */
    menu_item = hildon_gtk_button_new(HILDON_SIZE_AUTO);
    gtk_button_set_label(GTK_BUTTON(menu_item), _("Donation"));
    g_signal_connect_after(menu_item, "clicked",
                        G_CALLBACK(donation_button_handler), NULL);
    gtk_widget_show_all(menu_item);
    hildon_app_menu_append(menu, GTK_BUTTON(menu_item));
 
    gtk_widget_show_all(GTK_WIDGET(menu));
    return menu;
}
/*******************************************************************************/
/* For start of Clutter animation in popup window */
void
popup_simple_window_expose(GtkWidget *widget, GdkEventExpose *event){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
#ifdef HILDONANIMATION
    show_hildon_animation(app->clutter_objects_in_popup_form, app->popup_window);
//    gtk_widget_show_all(widget);
    g_signal_handlers_disconnect_by_func(G_OBJECT(widget),G_CALLBACK(popup_simple_window_expose),NULL);
#endif
}
/*******************************************************************************/
void
click_url_and_destroy_window(GtkWidget* button, gpointer window)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    click_url(button);
    gtk_widget_destroy(GTK_WIDGET(window));

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
void donation_button_handler(void){
    GtkWidget   *dialog, *url, *label;
    gint    result;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

#if !OS2009
    dialog =
        gtk_dialog_new_with_buttons(_("Donation"), NULL,
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    _("OK"), GTK_RESPONSE_ACCEPT, NULL);
#else
    dialog =
        gtk_dialog_new_with_buttons(_("Donation"), NULL,
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT, NULL);

#endif
/* text */
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),
                        label = gtk_label_new(_("You can make donation here: ")),
                        FALSE, FALSE, 5);
    set_font(label, NULL, 20);
/* url */
    url = gtk_button_new_with_label("http://omweather.garage.maemo.org/");
    gtk_button_set_relief(GTK_BUTTON(url), GTK_RELIEF_NONE);
    g_signal_connect(url, "clicked",
          G_CALLBACK(click_url_and_destroy_window), dialog);
    gtk_button_set_focus_on_click(GTK_BUTTON(url), FALSE);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), url, FALSE, FALSE, 5);
    set_font(url, NULL, 12);
    gtk_widget_show_all(dialog);
/* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
#endif


