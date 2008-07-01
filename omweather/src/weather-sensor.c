/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
#include <string.h>
#include <stdlib.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#if defined(OS2008) || defined(DEBUGTEMP)
#ifdef OS2008	/* sensor path at Nokia N810 */
    #define SENSOR "/sys/devices/platform/i2c_omap.1/i2c-1/1-0048/temp1_input"
#else	/* sensor path at the PC */
    #define SENSOR "/sys/devices/platform/w83627hf.656/temp2_input"
#endif
#endif
#if defined(OS2008) || defined(DEBUGTEMP)
/*******************************************************************************/
GtkWidget* create_sensor_page(GtkWidget *config_window){
    GtkWidget	*main_widget = NULL,
		*use_sensor = NULL,
		*table = NULL,
		*display_at_station_name = NULL,
		*display_at_new_icon = NULL,
		*update_time_entry = NULL,
		*apply_button = NULL;
    GSList	*display_group = NULL;
    gchar	buffer[10];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    apply_button = lookup_widget(config_window, "apply_button");
    main_widget = gtk_vbox_new(FALSE, 0);
    use_sensor
	= gtk_check_button_new_with_label(_("Use device temperature sensor"));
    GLADE_HOOKUP_OBJECT(config_window, use_sensor, "use_sensor");
    table = gtk_table_new(2, 3, FALSE);
    /* check if sensor is enable */
    if(app->config->use_sensor){
	gtk_widget_set_sensitive(table, TRUE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(use_sensor), TRUE);
    }
    else{
        gtk_widget_set_sensitive(table, FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(use_sensor), FALSE);
    }
    g_signal_connect(GTK_TOGGLE_BUTTON(use_sensor), "toggled",
            		G_CALLBACK(use_sensor_button_toggled_handler), table);
    gtk_widget_set_name(use_sensor, "use_sensor");
    if(apply_button)
	g_signal_connect(use_sensor, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			config_window);
    /* display sensor at */
    gtk_table_attach_defaults(GTK_TABLE(table), 
				gtk_label_new(_("Show temperature at:")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), 
				display_at_station_name
				    = gtk_radio_button_new_with_label(NULL,
									_("station name")),
				1, 2, 0, 1);
    GLADE_HOOKUP_OBJECT(config_window, display_at_station_name, "display_at");
    gtk_widget_set_name(display_at_station_name, "display_at");
    if(apply_button)
	g_signal_connect(display_at_station_name, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			config_window);
    gtk_button_set_focus_on_click(GTK_BUTTON(display_at_station_name), FALSE);
    display_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(display_at_station_name));
    gtk_table_attach_defaults(GTK_TABLE(table), 
				display_at_new_icon
				    = gtk_radio_button_new_with_label(display_group,
									_("new icon")),
				2, 3, 0, 1);
    gtk_button_set_focus_on_click(GTK_BUTTON(display_at_new_icon), FALSE);
    if(app->config->display_at == STATION_NAME)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display_at_station_name), TRUE);
    else
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display_at_new_icon), TRUE);
    /* update time label */
    gtk_table_attach_defaults(GTK_TABLE(table), 
				gtk_label_new(_("Read sensor every:")),
        			0, 1, 1, 2);
    /* update time entry */
    gtk_table_attach_defaults(GTK_TABLE(table), 
				update_time_entry
				    = gtk_entry_new_with_max_length(4),
				1, 2, 1, 2);
    g_signal_connect(G_OBJECT(update_time_entry), "changed",
			G_CALLBACK(check_buttons_changed_handler),
			(gpointer)config_window);
    GLADE_HOOKUP_OBJECT(config_window, update_time_entry, "update_time_entry");
    gtk_widget_set_name(update_time_entry, "update_time_entry");
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%u", app->config->sensor_update_time);
    gtk_entry_set_text(GTK_ENTRY(update_time_entry), buffer);
    gtk_table_attach_defaults(GTK_TABLE(table), 
				gtk_label_new(_("minutes")),
        			2, 3, 1, 2);
    /* pack items to the main widget */
    gtk_box_pack_start(GTK_BOX(main_widget), use_sensor, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(main_widget), table, TRUE, TRUE, 0);
    gtk_widget_show_all(main_widget);
    
    return main_widget;
}
/*******************************************************************************/
void use_sensor_button_toggled_handler(GtkToggleButton *togglebutton,
                                        		    gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(gtk_toggle_button_get_active(togglebutton))
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
    else{
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
	g_source_remove(app->sensor_timer);
    }
}
/*******************************************************************************/
int check_entry_text(GtkEntry *entry){
    const gchar	*text = gtk_entry_get_text(entry);
    gint	i;
    gboolean	error = FALSE;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(text && strlen(text) < 1)
	error = TRUE;
    for(i = 0; !error && i < strlen(text); i++)
	if(text[i] < '0' || text[i] > '9'){
	    error = TRUE;
	    break;
	}
    if(error)
	hildon_banner_show_information(app->main_window, NULL,
					_("Invalid symbol in field or field is empty."));
    return error;
}
/*******************************************************************************/
void read_sensor(gint need_redraw){
    FILE	*file;
    gchar	buffer[128];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!(file = fopen(SENSOR, "r")))
	return;
    memset(buffer, 0, sizeof(buffer));
    if(!fgets(buffer, sizeof(buffer) - 1, file)){
	fclose(file);
	return;
    }
    fclose(file);
    app->sensor_data = atof(buffer) / 1000.0f;
    if(app->config->temperature_units == FAHRENHEIT)
	app->sensor_data = c2f(app->sensor_data);
    if(need_redraw)
	redraw_home_window(FALSE);
}
/*******************************************************************************/
WDB* create_sensor_icon_widget(const int icon_size, gboolean transparency,
						char font_size, GdkColor *color){
    gchar	buffer[256],
		buffer_icon[256];
    
    /* prepare temperature data */
    buffer[0] = 0;
    snprintf(buffer, sizeof(buffer) - 1,
		"%s%02x%02x%02x%s%s\n%.2f%s",
		"<span foreground='#",
		app->config->font_color.red >> 8,
		app->config->font_color.green >> 8,
		app->config->font_color.blue >> 8,
		"'>",
		_("Sensor"),
		app->sensor_data,
		"\302\260</span>");
    /* prepare icon */
    buffer_icon[0] = 0;
    snprintf(buffer_icon, sizeof(buffer_icon) - 1, "%ssensor.png", BUTTON_ICONS);
    return create_weather_day_button(buffer, buffer_icon, icon_size,
						font_size, transparency, color);
}
/*******************************************************************************/
#endif
