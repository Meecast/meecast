/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
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
#include "weather-sensor.h"
#include "weather-utils.h"
#include "weather-settings.h"
#include "weather-home.h"
#include <string.h>
#include <stdlib.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#if defined(OS2008) || defined(DEBUGTEMP) || defined(OS2009)
#if defined(OS2008) || defined(OS2009)	/* sensor path at Nokia N810 */
#define SENSOR "/sys/devices/platform/i2c_omap.1/i2c-1/1-0048/temp1_input"
#else                           /* sensor path at the PC */
#define SENSOR "/sys/devices/platform/w83627hf.656/temp2_input"
#endif
#endif
#if defined(OS2008) || defined(DEBUGTEMP) || defined(OS2009)
/*******************************************************************************/
GtkWidget *create_sensor_page(GtkWidget * config_window) {
    GtkWidget *main_widget = NULL,
        *use_sensor = NULL,
        *table = NULL,
        *display_at_station_name = NULL,
        *display_at_new_icon = NULL,
        *sensor_update_time = NULL, *apply_button = NULL;
    GSList *display_group = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->sensor_tab_start_state = 0;
    apply_button = lookup_widget(config_window, "apply_button");
    main_widget = gtk_vbox_new(FALSE, 0);
    use_sensor
        =
        gtk_check_button_new_with_label(_
                                        ("Use device temperature sensor"));
    GLADE_HOOKUP_OBJECT(config_window, use_sensor, "use_sensor");
    gtk_widget_set_name(use_sensor, "use_sensor");
    table = gtk_table_new(2, 3, FALSE);
    /* check if sensor is enable */
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(use_sensor),
                                 app->config->use_sensor);
    if (app->config->use_sensor) {
        gtk_widget_set_sensitive(table, TRUE);
        app->sensor_tab_start_state |= STATE_USE_SENSOR;
    } else
        gtk_widget_set_sensitive(table, FALSE);
    g_signal_connect(GTK_TOGGLE_BUTTON(use_sensor), "toggled",
                     G_CALLBACK(use_sensor_button_toggled_handler), table);
    if (apply_button)
        g_signal_connect(use_sensor, "toggled",
                         G_CALLBACK(check_buttons_changed_handler),
                         config_window);
    /* display sensor at */
    gtk_table_attach_defaults(GTK_TABLE(table),
                              gtk_label_new(_
                                            ("Show temperature at:")),
                              0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),
                              display_at_station_name =
                              gtk_radio_button_new_with_label(NULL,
                                                              _
                                                              ("station name")),
                              1, 2, 0, 1);
    GLADE_HOOKUP_OBJECT(config_window, display_at_station_name,
                        "display_at");
    gtk_widget_set_name(display_at_station_name, "display_at_name");
    if (apply_button)
        g_signal_connect(display_at_station_name, "toggled",
                         G_CALLBACK(check_buttons_changed_handler),
                         config_window);
    gtk_button_set_focus_on_click(GTK_BUTTON(display_at_station_name),
                                  FALSE);
    display_group =
        gtk_radio_button_get_group(GTK_RADIO_BUTTON
                                   (display_at_station_name));
    gtk_table_attach_defaults(GTK_TABLE(table), display_at_new_icon =
                              gtk_radio_button_new_with_label
                              (display_group, _("new icon")), 2, 3, 0, 1);
    gtk_widget_set_name(display_at_new_icon, "display_at_icon");
    gtk_button_set_focus_on_click(GTK_BUTTON(display_at_new_icon), FALSE);
    if (apply_button)
        g_signal_connect(display_at_new_icon, "toggled",
                         G_CALLBACK(check_buttons_changed_handler),
                         config_window);

    if (app->config->display_at == STATION_NAME) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON
                                     (display_at_station_name), TRUE);
        app->sensor_tab_start_state |= STATE_SHOW_AT_NAME;
    } else {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON
                                     (display_at_new_icon), TRUE);
        app->sensor_tab_start_state |= STATE_SHOW_AT_ICON;
    }
    /* update time label */
    gtk_table_attach_defaults(GTK_TABLE(table),
                              gtk_label_new(_("Read sensor every:")),
                              0, 1, 1, 2);
    /* update time entry */
    gtk_table_attach_defaults(GTK_TABLE(table),
                              sensor_update_time
                              = gtk_combo_box_new_text(), 1, 2, 1, 2);
    GLADE_HOOKUP_OBJECT(config_window, sensor_update_time,
                        "sensor_update_time");
    gtk_widget_set_name(sensor_update_time, "sensor_update_time");
    g_signal_connect(G_OBJECT(sensor_update_time), "changed",
                     G_CALLBACK(combo_boxs_changed_handler),
                     (gpointer) apply_button);

    gtk_combo_box_append_text(GTK_COMBO_BOX(sensor_update_time),
                              _("Never"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(sensor_update_time),
                              _("1 minute"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(sensor_update_time),
                              _("3 minutes"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(sensor_update_time),
                              _("5 minutes"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(sensor_update_time),
                              _("10 minutes"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(sensor_update_time),
                              _("15 minutes"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(sensor_update_time),
                              _("30 minutes"));

    switch (get_index_from_time(app->config->sensor_update_time)) {
    default:
    case 0:
        gtk_combo_box_set_active(GTK_COMBO_BOX(sensor_update_time), 0);
        break;
    case 1:
        gtk_combo_box_set_active(GTK_COMBO_BOX(sensor_update_time), 1);
        break;
    case 2:
        gtk_combo_box_set_active(GTK_COMBO_BOX(sensor_update_time), 2);
        break;
    case 3:
        gtk_combo_box_set_active(GTK_COMBO_BOX(sensor_update_time), 3);
        break;
    case 4:
        gtk_combo_box_set_active(GTK_COMBO_BOX(sensor_update_time), 4);
        break;
    case 5:
        gtk_combo_box_set_active(GTK_COMBO_BOX(sensor_update_time), 5);
        break;
    case 6:
        gtk_combo_box_set_active(GTK_COMBO_BOX(sensor_update_time), 6);
        break;
    }
    /* pack items to the main widget */
    gtk_box_pack_start(GTK_BOX(main_widget), use_sensor, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(main_widget), table, TRUE, TRUE, 0);
    gtk_widget_show_all(main_widget);

    app->sensor_tab_current_state = app->sensor_tab_start_state;
    return main_widget;
}

/*******************************************************************************/
void
use_sensor_button_toggled_handler(GtkToggleButton * togglebutton,
                                  gpointer user_data) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (gtk_toggle_button_get_active(togglebutton))
        gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
    else {
        gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
        if (app->sensor_timer > 0)
            g_source_remove(app->sensor_timer);
    }
}

/*******************************************************************************/
gboolean read_sensor(gboolean need_redraw) {
    FILE *file;
    gchar buffer[128];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!(file = fopen(SENSOR, "r")))
        return TRUE;
    memset(buffer, 0, sizeof(buffer));
    if (!fgets(buffer, sizeof(buffer) - 1, file)) {
        fclose(file);
        return TRUE;
    }
    fclose(file);
    app->sensor_data = atof(buffer) / 1000.0f;
    if (app->config->temperature_units == FAHRENHEIT)
        app->sensor_data = c2f(app->sensor_data);
    if (need_redraw)
        redraw_home_window(FALSE);
    return TRUE;
}

/*******************************************************************************/
WDB *create_sensor_icon_widget(gboolean transparency, gboolean draw_day_label,
                                GdkColor * color) {
    gchar buffer[256], buffer_icon[256];
    gint icon_size;

/* select image size */
    switch(app->config->icons_size){
        case GIANT:
            icon_size = GIANT_ICON_SIZE;
            break;
        default:
        case LARGE:
            icon_size = LARGE_ICON_SIZE;
            break;
        case MEDIUM:
            icon_size = MEDIUM_ICON_SIZE;
            break;
        case SMALL:
            icon_size = SMALL_ICON_SIZE;
            break;
        case TINY:
            icon_size = TINY_ICON_SIZE;
            break;
    }
#if defined(NONMAEMO) || defined (APPLICATION)
    icon_size = MEDIUM_ICON_SIZE;
#endif
    /* prepare temperature data */
    buffer[0] = 0;
    snprintf(buffer, sizeof(buffer) - 1,
             "%s%02x%02x%02x%s%s\n%.2f%s",
             "<span foreground='#",
             app->config->font_color.red >> 8,
             app->config->font_color.green >> 8,
             app->config->font_color.blue >> 8,
             "'>", _("Sensor"), app->sensor_data, "\302\260</span>");
    /* prepare icon */
    buffer_icon[0] = 0;
    snprintf(buffer_icon, sizeof(buffer_icon) - 1, "%ssensor.png",
             BUTTON_ICONS);
    return create_weather_day_button(buffer, buffer_icon, OTHER_BUTTON,
                                     transparency, draw_day_label, color,
                                     UNKNOWN_DIRECTION, -1);
}

/*******************************************************************************/
guint get_time_from_index(guint index) {
    guint result = 0;
    switch (index) {
    default:
    case 0:
        result = 0;
        break;
    case 1:
        result = 1;
        break;
    case 2:
        result = 3;
        break;
    case 3:
        result = 5;
        break;
    case 4:
        result = 10;
        break;
    case 5:
        result = 15;
        break;
    case 6:
        result = 30;
        break;
    }
    return result;
}

/*******************************************************************************/
guint get_index_from_time(guint time) {
    guint result = 0;
    switch (time) {
    default:
    case 0:
        result = 0;
        break;
    case 1:
        result = 1;
        break;
    case 3:
        result = 2;
        break;
    case 5:
        result = 3;
        break;
    case 10:
        result = 4;
        break;
    case 15:
        result = 5;
        break;
    case 30:
        result = 6;
        break;
    }
    return result;
}

/*******************************************************************************/
#endif
