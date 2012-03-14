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
#include "setting_window.h"
#define WIDTH  (480)
#define HEIGHT (800)
# define __UNUSED__
/*******************************************************************************/
static Evas_Object *list;

static void
close_setting_window(void *data, Evas *e, Evas_Object *o, void *event_info){
    struct _App *app = (struct _App*)data;
    fprintf(stderr,"Closing....\n");
   evas_object_del(app->setting_top_main_window);
   evas_object_del(app->setting_menu);
   create_main_window(app);
}

static void
close_setting_window2(void *data, Evas *e, Evas_Object *o, void *event_info){
    struct _App *app = (struct _App*)data;
    fprintf(stderr,"Closing2....\n");
   evas_object_del(app->setting_top_main_window2);
   evas_object_del(app->setting_menu2);
   create_setting_window(app);
}

static void
prepare_for_sources_window(void *data, Evas *e, Evas_Object *o, void *event_info){
    struct _App *app = (struct _App*)data;
    fprintf(stderr,"Closing2....\n");
   evas_object_del(app->setting_top_main_window2);
   evas_object_del(app->setting_menu2);
   create_sources_window(app);
}

static void
_close_dialog_cb(void *data, Evas *e, Evas_Object *o, void *event_info){
    struct _App *app = (struct _App*)data;
    fprintf(stderr,"Closing dailog....\n");
    evas_object_del(app->dialog);
}

static void
delete_station_cb(void *data, Evas *e, Evas_Object *o, void *event_info){
    struct _App *app = (struct _App*)data;
    fprintf(stderr,"Delete station....\n");
    app->config->stationsList().erase(app->config->stationsList().begin() + app->index_list - 1);
    if (app->config->stationsList().size() > 0)
       app->config->current_station_id(0);
    fprintf(stderr,"Delete station2....\n");
    app->config->saveConfig();
    app->index_list = INT_MAX; 
    evas_object_del(app->dialog);
    evas_object_del(app->list);
    evas_object_del(app->setting_top_main_window2);
    create_location_window(data);
}

static void
_sel_source_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;

    evas_object_del(app->list);
    evas_object_del(app->setting_top_main_window2);
    app->index_list = elm_genlist_item_index_get(elm_genlist_selected_item_get(app->list));
    create_countries_window(data);
}

static void
_sel_country_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;

    evas_object_del(app->list);
    evas_object_del(app->setting_top_main_window2);
    app->index_list = elm_genlist_item_index_get(elm_genlist_selected_item_get(app->list));
    create_regions_window(data);
}

static void
_sel_region_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;

    evas_object_del(app->list);
    evas_object_del(app->setting_top_main_window2);
    app->index_list = elm_genlist_item_index_get(elm_genlist_selected_item_get(app->list));
    create_stations_window(data);
}

static void
_sel_station_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;
    Core::Station *station;

    evas_object_del(app->list);
    evas_object_del(app->setting_top_main_window2);

    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    path += app->source->c_str();
    path += ".db";
    Core::DatabaseSqlite *db;
    db = new Core::DatabaseSqlite("");
    if (db) 
        db->set_databasename(path.c_str());
    if (db->open_database()){
       Core::listdata * regionlist = db->create_region_list(app->country_id);
       app->region_id = atoi((char *)(regionlist->at(app->index_list-1).first.c_str()));
       app->region = new std::string (regionlist->at(app->index_list - 1).second.c_str());
       Core::listdata * stationlist = db->create_stations_list(app->region_id);

       app->index_list = elm_genlist_item_index_get(elm_genlist_selected_item_get(app->list));
       app->station_code = new std::string (stationlist->at(app->index_list-1).first.c_str());
       app->station = new std::string (stationlist->at(app->index_list - 1).second.c_str());

        std::string path(Core::AbstractConfig::prefix);
        path += Core::AbstractConfig::sourcesPath;
        Core::SourceList *sourcelist = new Core::SourceList(path);

        std::string url_template = sourcelist->at(app->source_id)->url_template();
        std::string url_detail_template = sourcelist->at(app->source_id)->url_detail_template();
        std::string url_for_view = sourcelist->at(app->source_id)->url_for_view();
        std::string cookie = sourcelist->at(app->source_id)->cookie();

        char forecast_url[4096];
        snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), app->station_code->c_str());
        std::string s_forecast_url = forecast_url;
        char forecast_detail_url[4096];
        snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, url_detail_template.c_str(), app->station_code->c_str());
        std::string s_forecast_detail_url = forecast_detail_url;
        char view_url[4096];
        snprintf(view_url, sizeof(view_url)-1, url_for_view.c_str(), app->station_code->c_str());
        std::string s_view_url = view_url;

        std::string source = app->source->c_str();
        std::string station_code = app->station_code->c_str();
        std::string station_name = app->station->c_str();
        std::string country = app->country->c_str();
        std::string region = app->region->c_str();
        station = new Core::Station(
                    source,
                    station_code,
                    station_name,
                    country,
                    region,
                    s_forecast_url,
                    s_forecast_detail_url,
                    s_view_url,
                    cookie,
                    false);
        std::string filename(Core::AbstractConfig::getConfigPath());
        filename += app->source->c_str();
        filename += "_";
        filename += app->station_code->c_str();
        station->fileName(filename);
        station->converter(sourcelist->at(app->source_id)->binary());

        app->config->stationsList().push_back(station);
        if (app->config->stationsList().size() > 0)
             app->config->current_station_id(0);
        //ConfigQml::Config::stationsList(*stationlist);
        app->config->saveConfig();

        std::cerr<<"ssssssssssssssssssssssssssssss"<<app->config->stationsList().size()<<std::endl;
    }

#if 0
    Core::Station *station;
    std::string code = city_id.toStdString();

    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sourcesPath;
    Core::SourceList *sourcelist = new Core::SourceList(path);

    std::string url_template = sourcelist->at(source_id)->url_template();
    std::string url_detail_template = sourcelist->at(source_id)->url_detail_template();
    std::string url_for_view = sourcelist->at(source_id)->url_for_view();
    std::string cookie = sourcelist->at(source_id)->cookie();

    char forecast_url[4096];
    snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), code.c_str());
    char forecast_detail_url[4096];
    snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, url_detail_template.c_str(), code.c_str());
    char view_url[4096];
    snprintf(view_url, sizeof(view_url)-1, url_for_view.c_str(), code.c_str());

    station = new Core::Station(
                source.toStdString(),
                code,
                city_name.toUtf8().data(),
                country.toStdString(),
                region.toStdString(),
                forecast_url,
                forecast_detail_url,
                view_url,
                cookie,
                gps);
    std::string filename(Core::AbstractConfig::getConfigPath());
    filename += source.toStdString();
    filename += "_";
    filename += code;
    station->fileName(filename);
    station->converter(sourcelist->at(source_id)->binary());

    stationsList().push_back(station);
    //ConfigQml::Config::stationsList(*stationlist);
    saveConfig();
#endif
}

static void
_delete_station_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;
    Evas_Object *edje_obj;
    Evas *evas;
    Evas_Object *temp_edje_obj = NULL;

    evas = ecore_evas_get(app->ee);

    edje_obj = edje_object_add(evas);

    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "dialog")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'dialog' from settingwindow.edj"
                        " %s\n", errmsg);
    }
    app->index_list = elm_genlist_item_index_get(elm_genlist_selected_item_get(app->list));
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "bt_yes");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, delete_station_cb, app); 
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "bt_no");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, _close_dialog_cb, app); 
    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, WIDTH, HEIGHT);
    evas_object_show(edje_obj);
    app->dialog = edje_obj;
}

static char *
_item_label_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   char buf[256];
   snprintf(buf, sizeof(buf), "%s", (char *)(char *)data);
   return strdup(buf);
}

static Evas_Object *
_item_content_add_get(void *data, Evas_Object *obj, const char *part)
{

   Evas_Object *ic = elm_icon_add(obj);
   if (!strcmp(part, "elm.swallow.end")){
        elm_icon_standard_set(ic, "new");
   }
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static Evas_Object *
_item_content_del_get(void *data, Evas_Object *obj, const char *part)
{

   Evas_Object *ic = elm_icon_add(obj);
   if (!strcmp(part, "elm.swallow.end")){
        elm_icon_standard_set(ic, "delete");
   }
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

void 
create_countries_window(void *data){
    struct _App *app = (struct _App*)data;
    Evas_Object *edje_obj;
    Evas_Object *edje_obj_menu;
    Evas *evas;
    Evas_Object *temp_edje_obj = NULL;
    Elm_Object_Item *item;

    int i;

    evas = ecore_evas_get(app->ee);

    edje_obj = edje_object_add(evas);
    fprintf(stderr,"Create countries window....\n");
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "managelistwindow")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    edje_object_part_text_set(edje_obj, "settings_label", "Select country");
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "list_rect");
    list = elm_genlist_add(temp_edje_obj); 
                 _itc.item_style = "default";
                 _itc.func.text_get = _item_label_get;
                 _itc.func.content_get = _item_content_add_get;
                 _itc.func.del = NULL;
    std::string path1(Core::AbstractConfig::prefix);
    path1 += Core::AbstractConfig::sourcesPath;

    Core::SourceList *sourcelist = new Core::SourceList(path1);
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    path += sourcelist->at(app->index_list - 1)->name();
    app->source = new std::string (sourcelist->at(app->index_list - 1)->name());
    path += ".db";
    app->source_id = app->index_list - 1;
    Core::DatabaseSqlite *db;
    db = new Core::DatabaseSqlite("");
    std::cerr<<"Path "<<path.c_str()<<std::endl;
    if (db) 
        db->set_databasename(path.c_str());
    if (db->open_database()){
//       Core::listdata * countrylist = db->create_regions_list(app->index_list);
       Core::listdata * countrylist = db->create_countries_list();
       for (short i=0; i < countrylist->size();i++){
          item = elm_genlist_item_append(list, &_itc,
                        (void *)(char *)(countrylist->at(i).second.c_str()), NULL,
                        ELM_GENLIST_ITEM_NONE,
                        _sel_country_cb, app);
        }
    }
    evas_object_move(list, 0, 100);
    evas_object_resize(list, WIDTH, HEIGHT-120);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, WIDTH, HEIGHT);
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    edje_obj_menu = edje_object_add(evas);
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj_menu, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "menu_managelocations")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj_menu);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'menu_managelocations' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "back");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, close_setting_window2, app); 
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "add");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, prepare_for_sources_window, app); 

    evas_object_move(edje_obj_menu, 0, 740);
    evas_object_resize(edje_obj_menu, WIDTH, 60);
    evas_object_show(edje_obj_menu);
    app->setting_menu2 = edje_obj_menu;


}

void 
create_regions_window(void *data){
    struct _App *app = (struct _App*)data;
    Evas_Object *edje_obj;
    Evas_Object *edje_obj_menu;
    Evas *evas;
    Evas_Object *temp_edje_obj = NULL;
    Elm_Object_Item *item;

    int i;

    evas = ecore_evas_get(app->ee);

    edje_obj = edje_object_add(evas);
    fprintf(stderr,"Create regions window....\n");
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "managelistwindow")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    edje_object_part_text_set(edje_obj, "settings_label", "Select region");
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "list_rect");
    list = elm_genlist_add(temp_edje_obj); 
                 _itc.item_style = "default";
                 _itc.func.text_get = _item_label_get;
                 _itc.func.content_get = _item_content_add_get;
                 _itc.func.del = NULL;

    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    path += app->source->c_str();
    path += ".db";
    Core::DatabaseSqlite *db;
    db = new Core::DatabaseSqlite("");
    if (db) 
        db->set_databasename(path.c_str());
    if (db->open_database()){
       Core::listdata * countrylist = db->create_countries_list();
       app->country_id = atoi((char *)(countrylist->at(app->index_list-1).first.c_str()));
       app->country = new std::string (countrylist->at(app->index_list - 1).second.c_str());
       Core::listdata * regionlist = db->create_region_list(app->country_id);
       for (short i=0; i < regionlist->size();i++){
          item = elm_genlist_item_append(list, &_itc,
                        (void *)(char *)(regionlist->at(i).second.c_str()), NULL,
                        ELM_GENLIST_ITEM_NONE,
                        _sel_region_cb, app);
        }
    }
    evas_object_move(list, 0, 100);
    evas_object_resize(list, WIDTH, HEIGHT-120);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, WIDTH, HEIGHT);
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    edje_obj_menu = edje_object_add(evas);
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj_menu, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "menu_managelocations")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj_menu);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'menu_managelocations' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "back");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, close_setting_window2, app); 
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "add");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, prepare_for_sources_window, app); 

    evas_object_move(edje_obj_menu, 0, 740);
    evas_object_resize(edje_obj_menu, WIDTH, 60);
    evas_object_show(edje_obj_menu);
    app->setting_menu2 = edje_obj_menu;

}

void 
create_stations_window(void *data){
    struct _App *app = (struct _App*)data;
    Evas_Object *edje_obj;
    Evas_Object *edje_obj_menu;
    Evas *evas;
    Evas_Object *temp_edje_obj = NULL;
    Elm_Object_Item *item;

    int i;

    evas = ecore_evas_get(app->ee);

    edje_obj = edje_object_add(evas);
    fprintf(stderr,"Create stations window....\n");
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "managelistwindow")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    edje_object_part_text_set(edje_obj, "settings_label", "Select location");
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "list_rect");
    list = elm_genlist_add(temp_edje_obj); 
                 _itc.item_style = "default";
                 _itc.func.text_get = _item_label_get;
                 _itc.func.content_get = _item_content_add_get;
                 _itc.func.del = NULL;

    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    path += app->source->c_str();
    path += ".db";
    Core::DatabaseSqlite *db;
    db = new Core::DatabaseSqlite("");
    if (db) 
        db->set_databasename(path.c_str());
    if (db->open_database()){
       Core::listdata * regionlist = db->create_region_list(app->country_id);
       app->region_id = atoi((char *)(regionlist->at(app->index_list-1).first.c_str()));
       app->region = new std::string (regionlist->at(app->index_list - 1).second.c_str());
       Core::listdata * stationlist = db->create_stations_list(app->region_id);
       for (short i=0; i < stationlist->size();i++){
          item = elm_genlist_item_append(list, &_itc,
                        (void *)(char *)(stationlist->at(i).second.c_str()), NULL,
                        ELM_GENLIST_ITEM_NONE,
                        _sel_station_cb, app);
        }
    }
    evas_object_move(list, 0, 100);
    evas_object_resize(list, WIDTH, HEIGHT-120);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, WIDTH, HEIGHT);
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    edje_obj_menu = edje_object_add(evas);
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj_menu, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "menu_managelocations")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj_menu);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'menu_managelocations' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "back");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, close_setting_window2, app); 
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "add");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, prepare_for_sources_window, app); 

    evas_object_move(edje_obj_menu, 0, 740);
    evas_object_resize(edje_obj_menu, WIDTH, 60);
    evas_object_show(edje_obj_menu);
    app->setting_menu2 = edje_obj_menu;


}

void 
create_sources_window(void *data){
    struct _App *app = (struct _App*)data;
    Evas_Object *edje_obj;
    Evas_Object *edje_obj_menu;
    Evas *evas;
    Evas_Object *temp_edje_obj = NULL;
    Elm_Object_Item *item;

    int i;

    evas = ecore_evas_get(app->ee);

    edje_obj = edje_object_add(evas);
    fprintf(stderr,"Create source window....\n");
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "managelistwindow")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    edje_object_part_text_set(edje_obj, "settings_label", "Select the weather source");
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "list_rect");
    list = elm_genlist_add(temp_edje_obj); 
                 _itc.item_style = "default";
                 _itc.func.text_get = _item_label_get;
                 _itc.func.content_get = _item_content_add_get;
                 //_itc.func.state_get = _item_content_get;
                 //_itc.func.del = NULL;
   std::string path(Core::AbstractConfig::prefix);
   path += Core::AbstractConfig::sourcesPath;
   Core::SourceList *sourcelist = new Core::SourceList(path);
   for (short i=0; i < sourcelist->size();i++){
      item = elm_genlist_item_append(list, &_itc,
                    (void *)(char *)sourcelist->at(i)->name().c_str(), NULL,
                    ELM_GENLIST_ITEM_NONE,
                    _sel_source_cb, app);
    }

    evas_object_move(list, 0, 100);
    evas_object_resize(list, WIDTH, HEIGHT-120);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, WIDTH, HEIGHT);
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    edje_obj_menu = edje_object_add(evas);
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj_menu, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "menu_managelocations")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj_menu);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'menu_managelocations' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "back");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, close_setting_window2, app); 
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "add");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, prepare_for_sources_window, app); 

    evas_object_move(edje_obj_menu, 0, 740);
    evas_object_resize(edje_obj_menu, WIDTH, 60);
    evas_object_show(edje_obj_menu);
    app->setting_menu2 = edje_obj_menu;


}

void 
create_location_window(void *data){
    struct _App *app = (struct _App*)data;
    Evas_Object *edje_obj;
    Evas_Object *edje_obj_menu;
    Evas *evas;
    Evas_Object *temp_edje_obj = NULL;
    Elm_Object_Item *item;

    int i;

    evas = ecore_evas_get(app->ee);

    edje_obj = edje_object_add(evas);
    fprintf(stderr,"Mange location window....\n");
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "managelistwindow")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    edje_object_part_text_set(edje_obj, "settings_label", "Manage locations");
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "list_rect");
    list = elm_genlist_add(temp_edje_obj); 
                 _itc.item_style = "default";
                 _itc.func.text_get = _item_label_get;
                 _itc.func.content_get = _item_content_del_get;
                 //_itc.func.state_get = _item_content_get;
                 //_itc.func.del = NULL;

    for (short i=0; i < app->config->stationsList().size();i++){
      item = elm_genlist_item_append(list, &_itc,
                    (void *)(char *)app->config->stationsList().at(i)->name().c_str(), NULL,
                    ELM_GENLIST_ITEM_NONE,
                    _delete_station_cb, app);
    }

    evas_object_move(list, 0, 100);
    evas_object_resize(list, WIDTH, HEIGHT-120);
    evas_object_show(list);
    app->list = list;
    fprintf(stderr,"1111d.... %p\n", app->list);

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, WIDTH, HEIGHT);
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    edje_obj_menu = edje_object_add(evas);
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj_menu, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "menu_managelocations")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj_menu);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'menu_managelocations' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "back");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, close_setting_window2, app); 
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "add");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, prepare_for_sources_window, app); 

    evas_object_move(edje_obj_menu, 0, 740);
    evas_object_resize(edje_obj_menu, WIDTH, 60);
    evas_object_show(edje_obj_menu);
    app->setting_menu2 = edje_obj_menu;


}
static void
manage_location_window(void *data, Evas *e, Evas_Object *o, void *event_info){
    create_location_window(data);
}

void
create_setting_window(void *data)
{

    Evas_Object *bg;
    Evas_Object *edje_obj;
    Evas_Object *list_box;
    Evas_Object *edje_obj_menu;
    Evas_Object *temp_edje_obj = NULL;
    Evas_Object *edje_obj_block;
    Evas *evas;
    struct _App *app = (struct _App*)data;
    struct _Setting_window win;
    int i, j;
    char  buffer[4096];
    Core::Data *temp_data = NULL;
    Evas_Object *my_obj = NULL;
    Edje_External_Param param;;

    time_t current_day;
    struct tm   *tm = NULL;

    evas = ecore_evas_get(app->ee);

    evas_object_del(app->setting_top_main_window);
    evas_object_del(app->setting_menu);

    edje_obj = edje_object_add(evas);
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "settingwindow")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "manage_location_box");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, manage_location_window, app); 

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, WIDTH, HEIGHT);
    evas_object_show(edje_obj);
    app->setting_top_main_window = edje_obj;

    edje_obj_menu = edje_object_add(evas);
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj_menu, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "menu")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj_menu);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                        " %s\n", errmsg);
    }
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_menu, "back");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, close_setting_window, app); 

    evas_object_move(edje_obj_menu, 0, 740);
    evas_object_resize(edje_obj_menu, WIDTH, 60);
    evas_object_show(edje_obj_menu);
    app->setting_menu = edje_obj_menu;
}


