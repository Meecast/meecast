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
#include "weather-simple-popup.h"
#include "weather-utils.h"
#include "weather-home.h"
/*******************************************************************************/
void
weather_simple_window_popup(GtkWidget *widget, gpointer user_data){
    GtkWidget       *window = NULL,
                    *main_vbox = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

#if defined OS2009
    window = hildon_stackable_window_new();
#else
    window = hildon_window_new();
#endif
    gtk_window_set_title(GTK_WINDOW(window), "OM Weather");
    main_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);
    gtk_widget_show(window);

    gtk_box_pack_start(GTK_BOX(main_vbox), create_top_buttons_box(), FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_vbox), create_collapsed_view(), TRUE, TRUE, 0);
    gtk_widget_show_all(main_vbox);
//    if(app->popup_window)
//        gtk_widget_destroy(app->popup_window);
//    app->popup_window = window;
}
/*******************************************************************************/
gchar*
get_next_station_name(const gchar *current_station_name, GtkListStore *user_stations_list){
    GtkTreeIter     iter;
    gboolean        valid,
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
                            &iter, NAME_COLUMN, &station_name, -1);
        if(ready)
            return station_name;
        else{
            fprintf(stderr,"Name %s %s\n",current_station_name, station_name);
            if((current_station_name) && (station_name) &&
                  !strcmp(current_station_name, station_name))
                ready = TRUE;
            g_free(station_name);
            gtk_tree_path_next(path);

            valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter, path);
            if(!valid)
                valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter);
        }
    }
    gtk_tree_path_free(path);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

    return NULL;
}
/*******************************************************************************/
GtkWidget*
create_top_buttons_box(void){
    GtkWidget       *buttons_box = NULL,
                    *station_button = NULL,
                    *update_button = NULL;
    gchar           buffer[255],
                    full_filename[2048];
    struct stat     statv;
/* buttons */
    buttons_box = gtk_hbox_new(TRUE, 0);
    /* station button */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s: %s",
                _("next"),
                get_next_station_name(app->config->current_station_name,
                                        app->user_stations_list));
    station_button = create_button_with_2_line_text(app->config->current_station_name,
                                                    buffer, 18, 12);
//    g_signal_connect(G_OBJECT(station_button), "button_press_event",
//                            G_CALLBACK(change_station_next), GINT_TO_POINTER(1));
    gtk_widget_set_size_request(station_button, -1, 80);
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
    update_button = create_button_with_2_line_text(_("Update"), buffer, 18, 12);
    gtk_widget_set_size_request(update_button, -1, 80);

    gtk_box_pack_start(GTK_BOX(buttons_box), station_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttons_box), update_button, TRUE, TRUE, 0);
    return buttons_box;
}
/*******************************************************************************/
GtkWidget*
create_collapsed_view(void){
    GtkWidget       *scrolled_window = NULL,
                    *main_vbox = NULL,
                    *line_hbox = NULL,
                    *line_text = NULL;
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
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* scrolled window */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window),
                                            GTK_SHADOW_OUT);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    /* pack childs to the scrolled window */
    main_vbox = gtk_vbox_new(FALSE, 5);
    gtk_scrolled_window_add_with_viewport(scrolled_window, main_vbox);


    days = (GSList*)g_hash_table_lookup(app->station_data, "forecast");
    if(days){
        while(days){
            day = (GHashTable*)(days->data);
            /* line box */
            line_hbox = gtk_hbox_new(FALSE, 0);
            /* icon */
            *buffer = 0;
            snprintf(icon, sizeof(icon) - 1, "%s%s.png", app->config->icons_set_base,
                        (char*)g_hash_table_lookup(day, "day_icon"));
            icon_buffer = gdk_pixbuf_new_from_file_at_size(icon, SMALL_ICON_SIZE,
                                                            SMALL_ICON_SIZE, NULL);
            icon_image = create_icon_widget(icon_buffer, icon, SMALL_ICON_SIZE, &app->clutter_objects_in_popup_form);
            if(icon_image){
                gtk_box_pack_start(GTK_BOX(line_hbox), icon_image, FALSE, TRUE, 0);
                gtk_box_pack_start(GTK_BOX(main_vbox), line_hbox, TRUE, TRUE, 0);
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
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%s, ",
                            (char*)hash_table_find("N/A", FALSE));
            else
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%d\302\260%c, ",
                            hi_temp, symbol);
            /* day title */
            if(g_hash_table_lookup(day, "day_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%s, ",
                            (char*)hash_table_find(g_hash_table_lookup(day, "day_title"), FALSE));
            /* wind speed */
            if(g_hash_table_lookup(day, "day_wind_speed"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"%.2f %s ",
                            convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "day_wind_speed"))),
                            (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
            /* wind direction */
            if(g_hash_table_lookup(day, "day_wind_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "(%s); ",
                            (char*)hash_table_find(g_hash_table_lookup(day, "day_wind_title"), FALSE));
            snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%s ", _("n:"));
            /* night temperature */
            if(low_temp == INT_MAX)
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%s, ",
                            (char*)hash_table_find("N/A", FALSE));
            else
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%d\302\260%c, ",
                            low_temp, symbol);
            /* night title */
            if(g_hash_table_lookup(day, "night_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "%s, ",
                            (char*)hash_table_find(g_hash_table_lookup(day, "night_title"), FALSE));
            /* wind speed */
            if(g_hash_table_lookup(day, "night_wind_speed"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1,"%.2f %s ",
                            convert_wind_units(app->config->wind_units, atof(g_hash_table_lookup(day, "night_wind_speed"))),
                            (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
            /* wind direction */
            if(g_hash_table_lookup(day, "night_wind_title"))
                snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp) - 1, "(%s)",
                            (char*)hash_table_find(g_hash_table_lookup(day, "night_wind_title"), FALSE));

            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1, "<i>%s</i>", tmp);

            line_text = gtk_label_new(NULL);
            gtk_label_set_justify(GTK_LABEL(line_text), GTK_JUSTIFY_FILL);
            gtk_label_set_markup(GTK_LABEL(line_text), buffer);
            set_font(line_text, NULL, 12);
            gtk_box_pack_start(GTK_BOX(line_hbox), line_text, FALSE, TRUE, 10);
            /* next day */
            days = g_slist_next(days);
            i++;
        }
    }
    gtk_widget_show_all(scrolled_window);
    return scrolled_window;
}
/*******************************************************************************/
