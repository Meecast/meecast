/* vim: set sw=4 ts=4 et: */
/* This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * Copyright (C) 2008 Andrew Olmsted
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 /Now* modify it under the terms of the GNU Lesser General Public License
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
#ifndef _weather_presets_h
#define _weather_presets_h 1

#include "weather-presets.h"

void
composition_central_vertical_day_button(WDB *new_day_button)
{
    gchar       buffer[2048];
    gchar       tmp_buffer[2048];
    gchar       *day_name;
    gchar       *begin_of_string;
    gchar       *begin_of_string2;
    GtkWidget   *day = NULL;
    GtkWidget   *temperature = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    /* Packing all to the box */
    /* create day box to contain icon, label and wind image */
    /* background */
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_LEFT_VERTICAL_BACKGROUND);
    new_day_button->background = gtk_image_new_from_file (buffer);

    gtk_widget_set_size_request(new_day_button->box, 70, 213);
    if (new_day_button->background)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->background, 0, 0);
    if (new_day_button->icon_image)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->icon_image, 0+3, 2);
    if (new_day_button->wind)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind, 0-5+14, 66-4);
    if (new_day_button->wind_text)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind_text, 0+14+6, 66+5);


    day_name = (gchar*)gtk_label_get_text(GTK_LABEL(new_day_button->label));


    begin_of_string = strstr(day_name, "\n");
    if (begin_of_string){
        memset(buffer, 0, sizeof(buffer));
        memset(tmp_buffer, 0, sizeof(tmp_buffer));
        memcpy(tmp_buffer, day_name , strlen(day_name) - strlen(begin_of_string));
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_BACK, tmp_buffer);
        day = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(day), buffer);
        gtk_label_set_justify(GTK_LABEL(day), GTK_JUSTIFY_CENTER);
        set_font(day, PRESET_DAY_FONT, -1);
        gtk_widget_set_size_request(day, 60, 25);

        memset(buffer, 0, sizeof(buffer));
        memset(tmp_buffer, 0, sizeof(tmp_buffer));
        begin_of_string2 = strstr(begin_of_string + 1, "\n"); 
        memcpy(tmp_buffer, begin_of_string + 1 , strlen(begin_of_string + 1) - strlen(begin_of_string2));
        temperature = gtk_label_new(NULL);
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' weight=\"bold\" foreground='%s'>%s</span><span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_FRONT, tmp_buffer, PRESET_FONT_COLOR_LOW_TEMP, 
                                        begin_of_string2);
    }else
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_BACK,
                                        (gchar*)gtk_label_get_text(GTK_LABEL(new_day_button->label)));
    gtk_label_set_markup(GTK_LABEL(temperature), buffer);
    gtk_label_set_justify(GTK_LABEL(temperature), GTK_JUSTIFY_CENTER);
    set_font(temperature, PRESET_TEMPERATURE_FONT, -1);
    gtk_widget_set_size_request(temperature, 66, 60);

    if (day)
            gtk_fixed_put(GTK_FIXED(new_day_button->box), day, 0, 60+37+17);
    if (temperature)
            gtk_fixed_put(GTK_FIXED(new_day_button->box), temperature, 0+2, 60+37+16+30);

    gtk_widget_destroy(new_day_button->label);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/**********************************************************************************************************/
void
composition_left_vertical_day_button(WDB *new_day_button, gint layout)
{
    gchar       buffer[2048];
    gchar       tmp_buffer[2048];
    gchar       *day_name;
    gchar       *begin_of_string;
    gchar       *begin_of_string2;
    GtkWidget   *day = NULL;
    GtkWidget   *temperature = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    /* Packing all to the box */
    /* create day box to contain icon, label and wind image */
    /* background */
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_LEFT_VERTICAL_BACKGROUND);
    new_day_button->background = gtk_image_new_from_file (buffer);

    day_name = (gchar*)gtk_label_get_text(GTK_LABEL(new_day_button->label));

    begin_of_string = strstr(day_name, "\n");
    if (begin_of_string){
        memset(buffer, 0, sizeof(buffer));
        memset(tmp_buffer, 0, sizeof(tmp_buffer));
        memcpy(tmp_buffer, day_name , strlen(day_name) - strlen(begin_of_string));
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_BACK, tmp_buffer);
        day = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(day), buffer);
        gtk_label_set_justify(GTK_LABEL(day), GTK_JUSTIFY_CENTER);
        set_font(day, PRESET_DAY_FONT, -1);
        gtk_widget_set_size_request(day, 60, 25);

        memset(buffer, 0, sizeof(buffer));
        memset(tmp_buffer, 0, sizeof(tmp_buffer));
        begin_of_string2 = strstr(begin_of_string + 1, "\n"); 
        memcpy(tmp_buffer, begin_of_string + 1 , strlen(begin_of_string + 1) - strlen(begin_of_string2));
        temperature = gtk_label_new(NULL);
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' weight=\"bold\" foreground='%s'>%s</span><span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_FRONT, tmp_buffer, PRESET_FONT_COLOR_LOW_TEMP, 
                                        begin_of_string2);
    }else
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_BACK,
                                        (gchar*)gtk_label_get_text(GTK_LABEL(new_day_button->label)));
    gtk_label_set_markup(GTK_LABEL(temperature), buffer);
    gtk_label_set_justify(GTK_LABEL(temperature), GTK_JUSTIFY_CENTER);
    set_font(temperature, PRESET_TEMPERATURE_FONT, -1);
    gtk_widget_set_size_request(temperature, 66, 60);


    switch (layout){
        case PRESET_NOW_PLUS_TWO:
            gtk_widget_set_size_request(new_day_button->box, 70, 213);
            if (new_day_button->background)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->background, 12+15-6, 0);
            if (new_day_button->icon_image)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->icon_image, 12+15-6+3, 2);
            if (new_day_button->wind)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind, 12+15-10+14, 66-4);
            if (new_day_button->wind_text)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind_text, 12+15-6+14+6, 66+5);
            if (day)
                    gtk_fixed_put(GTK_FIXED(new_day_button->box), day, 12+15, 60+37+17);
            if (temperature)
                    gtk_fixed_put(GTK_FIXED(new_day_button->box), temperature, 12+15+2, 60+37+16+30);
            break;
        default:
        case PRESET_NOW_PLUS_THREE_V:
            gtk_widget_set_size_request(new_day_button->box, 70, 213);
            if (new_day_button->background)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->background, 0, 0);
            if (new_day_button->icon_image)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->icon_image, 0+3, 2);
            if (new_day_button->wind)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind, 0-5+14, 66-4);
            if (new_day_button->wind_text)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind_text, 0+14+6, 66+5);
            if (day)
                    gtk_fixed_put(GTK_FIXED(new_day_button->box), day, 0, 60+37+17);
            if (temperature)
                    gtk_fixed_put(GTK_FIXED(new_day_button->box), temperature, 0+2, 60+37+16+30);
   
            break;
    }
 
    gtk_widget_destroy(new_day_button->label);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/**********************************************************************************************************/
void
composition_right_vertical_day_button(WDB *new_day_button, gint layout)
{
    gchar       buffer[2048];
    gchar       tmp_buffer[2048];
    gchar       *day_name;
    gchar       *begin_of_string;
    gchar       *begin_of_string2;
    GtkWidget   *day = NULL;
    GtkWidget   *temperature = NULL;


    /* Packing all to the box */
    /* create day box to contain icon, label and wind image */
    /* background */
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_RIGHT_VERTICAL_BACKGROUND);
    new_day_button->background = gtk_image_new_from_file (buffer);

    day_name = (gchar*)gtk_label_get_text(GTK_LABEL(new_day_button->label));

    begin_of_string = strstr(day_name, "\n");
    if (begin_of_string){ 
        memset(buffer, 0, sizeof(buffer));
        memset(tmp_buffer, 0, sizeof(tmp_buffer));
        memcpy(tmp_buffer, day_name , strlen(day_name) - strlen(begin_of_string));
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_BACK, tmp_buffer);
        day = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(day), buffer);
        gtk_label_set_justify(GTK_LABEL(day), GTK_JUSTIFY_CENTER);
        set_font(day, PRESET_DAY_FONT, -1);
        gtk_widget_set_size_request(day, 60, 25);

        memset(buffer, 0, sizeof(buffer));
        memset(tmp_buffer, 0, sizeof(tmp_buffer));
        begin_of_string2 = strstr(begin_of_string + 1, "\n"); 
        memcpy(tmp_buffer, begin_of_string + 1 , strlen(begin_of_string + 1) - strlen(begin_of_string2));
        temperature = gtk_label_new(NULL);
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' weight=\"bold\" foreground='%s'>%s</span><span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_FRONT, tmp_buffer, PRESET_FONT_COLOR_LOW_TEMP, 
                                        begin_of_string2);
    }else
        snprintf(buffer,sizeof(buffer) - 1,
                                       "<span stretch='ultracondensed' foreground='%s'>%s</span>",
                                        PRESET_BIG_FONT_COLOR_BACK,
                                        (gchar*)gtk_label_get_text(GTK_LABEL(new_day_button->label)));
    gtk_label_set_markup(GTK_LABEL(temperature), buffer);
    gtk_label_set_justify(GTK_LABEL(temperature), GTK_JUSTIFY_CENTER);
    set_font(temperature, PRESET_TEMPERATURE_FONT, -1);
    gtk_widget_set_size_request(temperature, 66, 60);

 
    gtk_widget_set_size_request(new_day_button->box, 70, 213);
    switch (layout){
        case PRESET_NOW_PLUS_TWO:
            if (new_day_button->background)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->background, 7, 0);
            if (new_day_button->icon_image)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->icon_image, 7+3, 2);
            if (new_day_button->wind)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind, 7-5+14, 66-4);
            if (new_day_button->wind_text)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind_text, 7+14+6, 66+5); 
            if (day)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), day, 0+8, 60+37+17);
            if (temperature)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), temperature, 0+8+2, 60+37+16+30);
            break;
        default:
        case PRESET_NOW_PLUS_THREE_V:
            if (new_day_button->background)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->background, 0, 0);
            if (new_day_button->icon_image)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->icon_image, 0+3, 2);
            if (new_day_button->wind)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind, 0-5+14, 66-4);
            if (new_day_button->wind_text)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind_text, 0+14+6, 66+5); 
            if (day)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), day, 0+1, 60+37+17);
            if (temperature)
                gtk_fixed_put(GTK_FIXED(new_day_button->box), temperature, 0+1+2, 60+37+16+30);
            break;
    }

    gtk_widget_destroy(new_day_button->label);

}
/*******************************************************************************/
void
composition_now(WDB *new_day_button, gint layout)
{
    gchar       buffer[2048];
    gint        xoffset = 0;
    gint        image_xoffset = 0;
    gint        wind_xoffset = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    /* create day box to contain icon, label and wind image */
    memset(buffer, 0, sizeof(buffer));
    switch (layout){
        case PRESET_NOW_PLUS_THREE_V:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_NOW_BACKGROUND_PLUS_THREE_V);
            gtk_widget_set_size_request(new_day_button->box, 210, 37+15+106);
            xoffset = 0;
            image_xoffset = 13;
            wind_xoffset = 53;
            gtk_widget_set_size_request(new_day_button->label, 208, 55);
            gtk_widget_set_size_request(new_day_button->shadow_label, 208, 55);
            break;
        case PRESET_NOW_PLUS_TWO:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_NOW_BACKGROUND_PLUS_TWO);
            xoffset = 12 + 15;
            gtk_widget_set_size_request(new_day_button->box, xoffset+140+1, 37+15+106);
            break;
        case PRESET_NOW_PLUS_THREE_H:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_NOW_BACKGROUND_HORIZONTAL);
            xoffset = 12 + 15;
            gtk_widget_set_size_request(new_day_button->box, xoffset+138, 37+15+106);
            break;
 
        default:
        case PRESET_NOW:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_NOW_BACKGROUND);
            xoffset = 12 + 15;
            gtk_widget_set_size_request(new_day_button->box, xoffset+140+1, 37+15+106);
            break;
    }
    new_day_button->background = gtk_image_new_from_file (buffer);
/* Packing all to the box */
    if (new_day_button->background)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->background, xoffset, 37+15);
    if (new_day_button->icon_image)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->icon_image, 0+image_xoffset, 0);
    if (new_day_button->wind)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind, (87+14) + xoffset + wind_xoffset - 11, 5+37+10);
    if (new_day_button->wind_text)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->wind_text, (87+14) + xoffset + wind_xoffset, 7+37+18+1);
    if (new_day_button->shadow_label)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->shadow_label, xoffset+2, 60+37+15+2);
    if (new_day_button->label)
        gtk_fixed_put(GTK_FIXED(new_day_button->box), new_day_button->label, xoffset, 60+37+15);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
 
}
/*******************************************************************************/
GtkWidget *
next_station_preset_now(gint layout)
{
    GtkWidget       *widget = NULL,
                    *station_text = NULL,
                    *shadow_station_text = NULL,
                    *background_town = NULL,
                    *station_name_btn = NULL;
    gchar           *begin_of_string;
    gchar           buffer[2048];
    gint            x_width = 0;
    gint            x_offset = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    widget = gtk_fixed_new();
    memset(buffer, 0, sizeof(buffer));
    switch (layout){
        case PRESET_NOW_PLUS_THREE_H:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_NOW_BACKGROUND_TOWN_HORIZONTAL);
            background_town = gtk_image_new_from_file (buffer);
            if (background_town)
                gtk_fixed_put(GTK_FIXED(widget), background_town, 12+15, 0);
            x_width = 138;
            x_offset = 12+15;
            break; 
        case PRESET_NOW_PLUS_TWO:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_NOW_BACKGROUND_TOWN_PLUS_TWO);
            background_town = gtk_image_new_from_file (buffer);
            if (background_town)
                gtk_fixed_put(GTK_FIXED(widget), background_town, 12+15, 0);
            x_width = 140;
            x_offset = 12+15;
            break;
        case PRESET_NOW_PLUS_THREE_V:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_NOW_BACKGROUND_TOWN_PLUS_THREE_V);
            background_town = gtk_image_new_from_file (buffer);
            if (background_town)
                gtk_fixed_put(GTK_FIXED(widget), background_town, 0, 0);
            x_width = 210;
            x_offset = 0;
            break;
        default:
        case PRESET_NOW:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_NOW_BACKGROUND_TOWN);
            background_town = gtk_image_new_from_file (buffer);
            if (background_town)
                gtk_fixed_put(GTK_FIXED(widget), background_town, 12+15, 0);
            x_width = 140;
            x_offset = 12+15;
            break;
    }

    /* Create next station event */
    station_name_btn = gtk_event_box_new();
    if(station_name_btn){
        gtk_widget_set_events(station_name_btn, GDK_BUTTON_PRESS_MASK);
        gtk_event_box_set_visible_window(GTK_EVENT_BOX(station_name_btn), FALSE);
        gtk_widget_set_size_request(station_name_btn, x_width, 71-2);
        g_signal_connect(station_name_btn, "button-press-event",
                    G_CALLBACK(change_station_next), NULL);
    }
    /* Create station name */
    if(app->config->current_station_id){
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer,"<span stretch='ultracondensed' foreground='%s'>%s</span>",
                            PRESET_BIG_FONT_COLOR_FRONT, app->config->current_station_name); 
        station_text = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(station_text), buffer);
        gtk_label_set_justify(GTK_LABEL(station_text), GTK_JUSTIFY_CENTER);
        set_font(station_text, PRESET_STATION_FONT, -1);
        gtk_widget_set_size_request(station_text, x_width-4, 30);
        /* Create shadow station name */
        if ((strlen(PRESET_BIG_FONT_COLOR_FRONT) == strlen(PRESET_BIG_FONT_COLOR_BACK))&&
            (begin_of_string = strstr(buffer,PRESET_BIG_FONT_COLOR_FRONT))){

            shadow_station_text = gtk_label_new(NULL);
            memcpy(begin_of_string, PRESET_BIG_FONT_COLOR_BACK,7);
            gtk_label_set_markup(GTK_LABEL(shadow_station_text), buffer);
            gtk_label_set_justify(GTK_LABEL(shadow_station_text), GTK_JUSTIFY_CENTER);
            set_font(shadow_station_text, PRESET_STATION_FONT, -1);
            gtk_widget_set_size_request(shadow_station_text, x_width-5, 30);
        }else
            shadow_station_text = NULL;
    }else{
        station_text = NULL;
        shadow_station_text = NULL;
    }

    if (station_name_btn)
        gtk_fixed_put(GTK_FIXED(widget), station_name_btn, x_offset, 0);
    if (shadow_station_text)
        gtk_fixed_put(GTK_FIXED(widget), shadow_station_text, x_offset+2+1, 17+1);
    if (station_text)
        gtk_fixed_put(GTK_FIXED(widget), station_text, x_offset+1+1,17);

    return widget;
}

/*******************************************************************************/
void
fill_weather_day_button_presets(WDB *new_day_button, const char *text, const char *icon,
                const gint icon_size, gboolean transparency,
                gboolean draw_day_label, gint wind_direction, gfloat wind_speed)
{
    gchar       buffer[2048];
    GdkPixbuf   *icon_buffer;
    gchar       *begin_of_string;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* create day label */
    new_day_button->label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(new_day_button->label), text);
    gtk_label_set_justify(GTK_LABEL(new_day_button->label), GTK_JUSTIFY_CENTER);
    /* Set font size for label */
    if ( strlen(text)>65 )
        set_font(new_day_button->label, PRESET_MEDIUM_FONT, -1);
    else
        set_font(new_day_button->label, PRESET_BIG_FONT, -1);
    gtk_widget_set_size_request(new_day_button->label, 140, 52);


    /* create wind text */
    new_day_button->wind_text = gtk_label_new(NULL);
    memset(buffer, 0, sizeof(buffer));
    if (wind_speed != -1)
        sprintf(buffer,"<span stretch='ultracondensed' foreground='%s'>%.0f</span>",
                               PRESET_WIND_FONT_COLOR, wind_speed);

    gtk_label_set_markup(GTK_LABEL(new_day_button->wind_text), buffer);
    gtk_label_set_justify(GTK_LABEL(new_day_button->wind_text), GTK_JUSTIFY_CENTER);
    /* Set font size for label */
    set_font(new_day_button->wind_text, PRESET_WIND_FONT, -1);
    gtk_widget_set_size_request(new_day_button->wind_text, 30, 30);

   /* create day icon buffer */
    icon_buffer =
          gdk_pixbuf_new_from_file_at_size(icon,
                        PRESET_NORMAL_IMAGE_SIZE,
                        PRESET_NORMAL_IMAGE_SIZE,
                        NULL);
    if (icon_buffer)
        /* create day icon image from buffer */
        new_day_button->icon_image = create_icon_widget(icon_buffer, icon, icon_size, &app->clutter_objects_in_main_form);
    else
        new_day_button->icon_image = NULL;

    memset(buffer, 0, sizeof(buffer));
    switch (wind_direction){
       case TO_SOUTH:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_SOUTH);
            break;
       case TO_EAST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_EAST);
            break;
       case TO_WEST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_WEST);
            break;
       case TO_NORTH:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_NORTH);
            break;
       case TO_SOUTH_EAST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_SOUTH_EAST);
            break;
       case TO_SOUTH_WEST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_SOUTH_WEST);
            break;
       case TO_NORTH_EAST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_NORTH_EAST);
            break;
       case TO_NORTH_WEST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_NORTH_WEST);
            break;
       default:
            memset(buffer, 0, sizeof(buffer));
    }
    if (buffer[0] != 0){

        if (wind_speed > STRONG_WIND){
           begin_of_string = strstr(buffer,".png"); 
           snprintf(begin_of_string, sizeof(buffer) - strlen(buffer) - 1, "%s","_warning.png");
        }

        /* create day icon buffer */
        icon_buffer =
          gdk_pixbuf_new_from_file(buffer,
                        NULL);
        if (icon_buffer){
            /* create day icon image from buffer */
            new_day_button->wind = create_icon_widget(icon_buffer, icon, icon_size, &app->clutter_objects_in_main_form);
        }
        else
            new_day_button->wind = NULL;
    }
    new_day_button->box  = gtk_fixed_new();
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/**********************************************************************************************/
void
fill_weather_day_button_preset_now(WDB *new_day_button, const char *text, const char *icon,
                const gint icon_size, gboolean transparency,
                gboolean draw_day_label, gint wind_direction, gfloat wind_speed)
{
    gchar       buffer[2048];
    GdkPixbuf   *icon_buffer;
    gchar       *begin_of_string;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* create day label */
    new_day_button->label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(new_day_button->label), text);
    gtk_label_set_justify(GTK_LABEL(new_day_button->label), GTK_JUSTIFY_CENTER);
    /* Set font size for label */
    if ( strlen(text)>65 )
        set_font(new_day_button->label, PRESET_MEDIUM_FONT, -1);
    else
        set_font(new_day_button->label, PRESET_BIG_FONT, -1);
    gtk_widget_set_size_request(new_day_button->label, 140, 52);

//        gtk_widget_set_name(new_day_button->label, "day_label");
    /* create shadow of day label */
    /* check for memcpy operation */
    if ((strlen(PRESET_BIG_FONT_COLOR_FRONT) == strlen(PRESET_BIG_FONT_COLOR_BACK))&&
        (begin_of_string = strstr(text, PRESET_BIG_FONT_COLOR_FRONT))){

        new_day_button->shadow_label = gtk_label_new(NULL);
        memcpy(begin_of_string, PRESET_BIG_FONT_COLOR_BACK,7);
        gtk_label_set_markup(GTK_LABEL(new_day_button->shadow_label), text);
        gtk_label_set_justify(GTK_LABEL(new_day_button->shadow_label), GTK_JUSTIFY_CENTER);
        /* Set font size for label */
        if ( strlen(text)>65 )
            set_font(new_day_button->shadow_label, PRESET_MEDIUM_FONT, -1);
        else
            set_font(new_day_button->shadow_label, PRESET_BIG_FONT, -1);
        gtk_widget_set_size_request(new_day_button->shadow_label, 140, 52);

     }else
        new_day_button->shadow_label = NULL;

    /* create wind text */
    new_day_button->wind_text = gtk_label_new(NULL);
    memset(buffer, 0, sizeof(buffer));
    if (wind_speed != -1)
        sprintf(buffer,"<span stretch='ultracondensed' foreground='%s'>%.0f</span>",
                               PRESET_WIND_FONT_COLOR, wind_speed);

    gtk_label_set_markup(GTK_LABEL(new_day_button->wind_text), buffer);
    gtk_label_set_justify(GTK_LABEL(new_day_button->wind_text), GTK_JUSTIFY_CENTER);
    /* Set font size for label */
    set_font(new_day_button->wind_text, PRESET_WIND_FONT, -1);
    gtk_widget_set_size_request(new_day_button->wind_text, 30, 30);

   /* create day icon buffer */
    icon_buffer =
          gdk_pixbuf_new_from_file_at_size(icon,
                        PRESET_BIG_IMAGE_SIZE,
                        PRESET_BIG_IMAGE_SIZE,
                        NULL);
    if (icon_buffer)
        /* create day icon image from buffer */
        new_day_button->icon_image = create_icon_widget(icon_buffer, icon, icon_size, &app->clutter_objects_in_main_form);
    else
        new_day_button->icon_image = NULL;

    memset(buffer, 0, sizeof(buffer));
    switch (wind_direction){
       case TO_SOUTH:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_SOUTH);
            break;
       case TO_EAST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_EAST);
            break;
       case TO_WEST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_WEST);
            break;
       case TO_NORTH:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_NORTH);
            break;
       case TO_SOUTH_EAST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_SOUTH_EAST);
            break;
       case TO_SOUTH_WEST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_SOUTH_WEST);
            break;
       case TO_NORTH_EAST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_NORTH_EAST);
            break;
       case TO_NORTH_WEST:
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", IMAGES_PATH, PRESET_WIND_NORTH_WEST);
            break;
       default:
            memset(buffer, 0, sizeof(buffer));
    }
    if (buffer[0] != 0){
        
        if (wind_speed < STRONG_WIND)
            new_day_button->wind = gtk_image_new_from_file (buffer);
        else{
           begin_of_string = strstr(buffer,".png"); 
           snprintf(begin_of_string, sizeof(buffer) - strlen(buffer) - 1, "%s","_warning.png");
           new_day_button->wind = gtk_image_new_from_file (buffer);
        }
    }
    new_day_button->box  = gtk_fixed_new();
}
/*******************************************************************************/
#endif
