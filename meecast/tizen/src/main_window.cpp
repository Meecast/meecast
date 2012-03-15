/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen
 *
 * Copyright (C) 2012 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "main_window.h"
#define WIDTH  (480)
#define HEIGHT (800)

/*******************************************************************************/

static void
on_done(void *data, Evas_Object *obj, void *event_info)
{
      // quit the mainloop (elm_run function will return)
      elm_exit();
}

static void
set_color_by_temp(Evas_Object *obj, int t)
{
    int c1, c2, c3;
//    if (Config.temperatureunit == "F"){
//        t = (t - 32) * 5 / 9;
//    }
    if (t >= 30){
        c2 = (t - 50)*(246-60)/(30-50) + 60;
        evas_object_color_set(obj, 255, c2, 0, 255);
        return;
    }else if (t < 30 && t >= 15){
        c1 = (t - 30)*(114-1)/(15-30) + 1;
        c2 = (t - 30)*(1-246)/(15-30) + 246;
        evas_object_color_set(obj, c1, c2, 0, 255);
        return; 
    }else if (t < 15 && t >= 0){
        c1 = (t - 15)*(1-114)/(0-15) + 144;
        c3 = (t - 15)*(1-0)/(0-15) + 0;
        evas_object_color_set(obj, c1, 255, c3, 255);
        return; 
    }else if (t < 0 && t >= -15){
        c1 = (t - 0)*(0-1)/(-15-0) + 1;
        c2 = (t - 0)*(216)/(-15-0) + 1;
        evas_object_color_set(obj, c1, c2, 255, 255);
        return;
    }
    else if (t < -15 && t >= -30){
        c2 = (t - (-15))*(66-216)/(-30+15) + 216;
        evas_object_color_set(obj, 0, c2, 255, 255);
    }else if (t < -30){
        c1 = (t - (-30))*(132-0)/(-30+15) + 0;
        c2 = (t - (-30))*(0-66)/(-30+15) + 66;
        evas_object_color_set(obj, c1, c2, 255, 255);
    }
}

/*******************************************************************************/
static void
download_forecast(void *data, Evas *e, Evas_Object *o, void *event_info){
    struct _App *app = (struct _App*)data;
    fprintf(stderr,"Updating....\n");
    for (short i=0; i < app->config->stationsList().size();i++){
        app->config->stationsList().at(i)->updateData(true);
    }
    create_main_window(data);
}
/*******************************************************************************/
static void
menu(void *data, Evas *e, Evas_Object *o, void *event_info){
    struct _App *app = (struct _App*)data;
    create_setting_window(data);
}


/*******************************************************************************/
static void
left_arrow_down(void *data, Evas *e, Evas_Object *o, void *event_info){

    struct _App *app = (struct _App*)data;
    if ((uint)(app->config->current_station_id() - 1) >= 0)
        app->config->current_station_id(app->config->current_station_id() - 1);
    else 
        app->config->current_station_id(app->stationslist->size());
    app->config->saveConfig();
    create_main_window(data);
}
/*******************************************************************************/
static void
right_arrow_down(void *data, Evas *e, Evas_Object *o, void *event_info){

    struct _App *app = (struct _App*)data;
    if ((uint)(app->config->current_station_id() + 1) < app->stationslist->size())
        app->config->current_station_id(app->config->current_station_id() + 1);
    else 
        app->config->current_station_id(0);
    app->config->saveConfig();
    create_main_window(data);
}

/*******************************************************************************/
void
create_main_window(void *data)
{

    Evas_Object *bg;
    Evas_Object *edje_obj;
    Evas_Object *list_box;
    Evas_Object *edje_obj_menu;
    Evas_Object *temp_edje_obj = NULL;
    Evas_Object *edje_obj_block;
    Evas *evas;
    struct _App *app = (struct _App*)data;
    int i, j;
    char  buffer[4096];
    Core::Data *temp_data = NULL;
    Evas_Object *my_obj = NULL;
    Edje_External_Param param;;

    time_t current_day;
    struct tm   *tm = NULL;

    fprintf(stderr,"Load!!!!!!!! yy %i %i\n",app->config->current_station_id(),  app->config->stationsList().size() );
    evas = ecore_evas_get(app->ee);

    bg = evas_object_rectangle_add(evas);
    evas_object_color_set(bg, 0, 0, 0, 255); /* red bg */
    evas_object_move(bg, 0, 0); /* at canvas' origin */
    evas_object_resize(bg, WIDTH, HEIGHT); /* covers full canvas */
    evas_object_show(bg);
    ecore_evas_object_associate(app->ee, bg, ECORE_EVAS_OBJECT_ASSOCIATE_BASE);

    evas_object_focus_set(bg, EINA_TRUE);


    if ((app->config->stationsList().size() > 0) && (app->config->current_station_id() < app->config->stationsList().size())){
        fprintf(stderr,"ttttttttttttttttttttttttttttttt\n");
        app->dp = current_data(app->config->stationsList().at(app->config->current_station_id())->fileName());
    }
    else
        app->dp = NULL;

    evas_object_del(app->top_main_window);

    edje_obj = edje_object_add(evas);
    app->top_main_window = edje_obj; 
     if (app->dp)
    fprintf(stderr,"Load!!!!!!!! yy %p %p\n", app->dp, app->dp->data().GetDataForTime(time(NULL)));
    /* Preparing data */
    if (app->dp != NULL && (temp_data = app->dp->data().GetDataForTime(time(NULL)))){    
    
        if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/mainwindow.edj", "mainwindow")){
            Edje_Load_Error err = edje_object_load_error_get(edje_obj);
            const char *errmsg = edje_load_error_str(err);
            fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                            " %s\n", errmsg);
        }

        temp_data->temperature_low().units(app->config->TemperatureUnit());
        temp_data->temperature_hi().units(app->config->TemperatureUnit());
        temp_data->temperature().units(app->config->TemperatureUnit());

        /* Filling window */
        /*Station name */
        edje_object_part_text_set(edje_obj, "station_name", app->config->stationname().c_str());
        /* Current o not current period */
        if (temp_data->Current())
            edje_object_part_text_set(edje_obj, "period_name", "Now");
        else
            edje_object_part_text_set(edje_obj, "period_name", "Today");

        /* Temperature */
        if (temp_data->temperature().value(true) == INT_MAX){
          if ((temp_data->temperature_hi().value(true) == INT_MAX) &&
              (temp_data->temperature_low().value(true) == INT_MAX)){ 
            edje_object_part_text_set(edje_obj, "main_temperature", "N/A");
          } 
          if ((temp_data->temperature_hi().value(true) != INT_MAX) &&
              (temp_data->temperature_low().value(true) != INT_MAX)){ 
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°/ %0.f°", temp_data->temperature_low().value(),
                                                                 temp_data->temperature_hi().value());
            edje_object_part_text_set(edje_obj, "main_temperature", buffer);
          }  
          if (temp_data->temperature_hi().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
            edje_object_part_text_set(edje_obj, "main_temperature", buffer);
          }
          if (temp_data->temperature_low().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
            edje_object_part_text_set(edje_obj, "main_temperature", buffer);
          }
        }else{
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature().value());
            edje_object_part_text_set(edje_obj, "main_temperature", buffer);
        }

        /* Description for current weather forecast */
        edje_object_part_text_set(edje_obj, "main_description", temp_data->Text().c_str());

        /* Main Icon */
        snprintf(buffer, sizeof(buffer) - 1, "%s/%s/%i.png", app->config->iconspath().c_str(), app->config->iconSet().c_str(), temp_data->Icon());
        param.type = EDJE_EXTERNAL_PARAM_TYPE_STRING;
        param.name = "icon";
        param.s = buffer;
        edje_object_part_external_param_set (edje_obj, "main_icon", &param);
        
        /* Main humidity */
        if (temp_data->Humidity() != INT_MAX){
            snprintf (buffer, sizeof(buffer) -1, "%i", temp_data->Humidity());
            edje_object_part_text_set(edje_obj, "humidity_label", buffer);
        }

        /* Main presssure */
        snprintf (buffer, sizeof(buffer) -1, "%i", temp_data->pressure().value());
        edje_object_part_text_set(edje_obj, "pressure_label", buffer);
        
        /* Main wind direction */
        snprintf (buffer, sizeof(buffer) -1, "%s", temp_data->WindDirection().c_str());
        edje_object_part_text_set(edje_obj, "wind_direction_label", buffer);
        /* Main wind speed */
        snprintf (buffer, sizeof(buffer) -1, "%0.f", temp_data->WindSpeed().value());
        edje_object_part_text_set(edje_obj, "wind_speed_label", buffer);

        /* Change color for background */
        temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "main_background_rect");
        if (temp_data->temperature().value(true) != INT_MAX)
            set_color_by_temp(temp_edje_obj, (int)temp_data->temperature().value(true));
        else{
            if (temp_data->temperature_hi().value(true) != INT_MAX){
                set_color_by_temp(temp_edje_obj, (int)temp_data->temperature_hi().value(true));
            }else
            if (temp_data->temperature_low().value(true) != INT_MAX){
                set_color_by_temp(temp_edje_obj, (int)temp_data->temperature_low().value(true));
            }
        }
        temp_edje_obj = NULL;

        evas_object_move(edje_obj, 0, 0);
        evas_object_resize(edje_obj, WIDTH, HEIGHT);
        evas_object_show(edje_obj);


        /* show or hide arrows */
        temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "left_arrow");
        if (app->config->prevstationname().length() < 1)
            evas_object_hide(temp_edje_obj);
        else
            evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, left_arrow_down, app); 

        temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "right_arrow");
        if (app->config->nextstationname().length() < 1)
            evas_object_hide(temp_edje_obj);
        else
            evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, right_arrow_down, app); 
        temp_edje_obj = NULL;


        /* Fill list of days with weather forecast */
        /* set current day */ 
        current_day = time(NULL);
        tm = gmtime(&current_day);
        tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
        tm->tm_isdst = 1;
        current_day = mktime(tm); /* today 00:00:00 */


        /* fill other days */
        i = 3600*24;
        j = 0;
        list_box = evas_object_box_add(evas); 
        app->day_list_main_window = list_box;
        evas_object_box_layout_set(
                      list_box, evas_object_box_layout_vertical, NULL, NULL);
        while  (app->dp != NULL && (temp_data = app->dp->data().GetDataForTime( current_day + 14 * 3600  + i))) {
            edje_obj_block = edje_object_add(evas);
            if (!edje_object_file_set(edje_obj_block, "/opt/apps/com.meecast.omweather/share/edje/mainwindow.edj", "dayblock")){
                Edje_Load_Error err = edje_object_load_error_get(edje_obj_block);
                const char *errmsg = edje_load_error_str(err);
                fprintf(stderr, "Could not load 'dayblock' from mainwindow.edj:"
                                " %s\n", errmsg);
            }
            evas_object_resize(edje_obj_block, WIDTH, 80);
            edje_object_part_text_set(edje_obj_block, "full_day_name", temp_data->FullDayName().c_str());
            /* Icon */
            snprintf(buffer, sizeof(buffer) - 1, "%s/%s/%i.png", app->config->iconspath().c_str(), 
                                                                 app->config->iconSet().c_str(), temp_data->Icon());
            param.type = EDJE_EXTERNAL_PARAM_TYPE_STRING;
            param.name = "icon";
            param.s = buffer;
            edje_object_part_external_param_set (edje_obj_block, "icon", &param);

            if (temp_data->temperature_low().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
                edje_object_part_text_set(edje_obj_block, "min_temp", buffer);
            }
            if (temp_data->temperature_hi().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
                edje_object_part_text_set(edje_obj_block, "max_temp", buffer);
                temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_block, "max_temp");
                set_color_by_temp(temp_edje_obj, (int)temp_data->temperature_hi().value(true));
                temp_edje_obj = NULL;
            }
            evas_object_box_append(list_box, edje_obj_block);
            evas_object_show(edje_obj_block);
            if (j % 2 == 0 ){
                temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_block, "bg_rect");
                evas_object_color_set(temp_edje_obj, 16, 16, 16, 255);
                temp_edje_obj = NULL;
            }
            i = i + 3600*24;
            j++;
        }
        evas_object_move(list_box, 0, 320);
        evas_object_resize(list_box, WIDTH, 80*j);
        evas_object_show(list_box);

    }else{
        if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/mainwindow.edj", "nullwindow")){
            Edje_Load_Error err = edje_object_load_error_get(edje_obj);
            const char *errmsg = edje_load_error_str(err);
            fprintf(stderr, "Could not load 'nullwindow' from mainwindow.edj:"
                            " %s\n", errmsg);
        }
        if (app->config->stationsList().size() > 0){

            edje_object_part_text_set(edje_obj, "Message1", "Looks like there's no info" );
            edje_object_part_text_set(edje_obj, "Message2", "for this location.");
        }

        evas_object_move(edje_obj, 0, 0);
        evas_object_resize(edje_obj, WIDTH, HEIGHT);
        evas_object_show(edje_obj);
    }

    evas_object_del(app->menu);

    /* Fill menu */
    edje_obj_menu = edje_object_add(evas);
    if (!edje_object_file_set(edje_obj_menu, "/opt/apps/com.meecast.omweather/share/edje/mainwindow.edj", "menu")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj_menu);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'menu' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    if (app->dp){
        /* Source Logo */
        snprintf(buffer, sizeof(buffer) - 1, "/opt/apps/com.meecast.omweather/share/images/%s.png",
        app->config->stationsList().at(app->config->current_station_id())->sourceName().c_str());
        param.type = EDJE_EXTERNAL_PARAM_TYPE_STRING;
        param.name = "icon";
        param.s = buffer;
        edje_object_part_external_param_set (edje_obj_menu, "source_logo", &param);

        temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "refresh_button");
        evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, download_forecast, app); 
    }
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "menu_button");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, menu, app); 

    evas_object_move(edje_obj_menu, 0, 740);
    evas_object_resize(edje_obj_menu, WIDTH, 60);
    evas_object_show(edje_obj_menu);
    app->menu = edje_obj_menu;

}


