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
# define __UNUSED__
/*******************************************************************************/
static Evas_Object *list;

Evas_Object *eo_radiogroup_repeat;


Elm_Object_Item *item_repeat;

list_item_data
*list_item_create(int index, char *text1, char *text2,
		   Evas_Object *icon, void *ad)
{
	list_item_data *ret = (list_item_data *)calloc(1, sizeof(list_item_data));
	ret->index = index;
	if (text1) {
		snprintf(ret->text1, sizeof(ret->text1), "%s", text1);
	}
	if (text2) {
		snprintf(ret->text2, sizeof(ret->text2), "%s", text2);
	}
	ret->icon = icon;
	ret->app = ad;
	return ret;
}

static void
close_setting_window (void *data, Evas_Object *obj, const char *emission, const char *source){
    struct _App *app = (struct _App*)data;
    fprintf(stderr,"Closing....\n");
    evas_object_del(app->setting_top_main_window);
    evas_object_del(app->setting_menu);
    create_main_window(app);
}

static void
close_setting_window2(void *data, Evas_Object *obj, const char *emission, const char *source){
   struct _App *app = (struct _App*)data;
   fprintf(stderr,"Closing2....\n");
   evas_object_del(app->setting_top_main_window2);
   evas_object_del(app->setting_menu2);
   create_setting_window(app);
}

static void
prepare_for_sources_window(void *data, Evas_Object *obj, const char *emission, const char *source){
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

    evas_object_del(app->setting_top_main_window2);
    evas_object_del(app->setting_menu2);
    app->index_list = elm_genlist_item_index_get(elm_genlist_selected_item_get(app->list));
    evas_object_del(app->list);
    create_countries_window(data);
}

static void
_sel_country_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;

    evas_object_del(app->list);
    evas_object_del(app->setting_top_main_window2);
    evas_object_del(app->setting_menu2);
    app->index_list = elm_genlist_item_index_get(elm_genlist_selected_item_get(app->list));
    create_regions_window(data);
}

static void
_sel_region_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;

    evas_object_del(app->list);
    evas_object_del(app->setting_top_main_window2);
    evas_object_del(app->setting_menu2);
    app->index_list = elm_genlist_item_index_get(elm_genlist_selected_item_get(app->list));
    create_stations_window(data);
}

static char *
_item_label_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   char buf[256];
   snprintf(buf, sizeof(buf), "%s", (char *)(char *)data);
   return strdup(buf);
}

static void
_sel_station_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;
    Core::Station *station;

    evas_object_del(app->list);
    evas_object_del(app->setting_top_main_window2);
    evas_object_del(app->setting_top_main_window);

    evas_object_del(app->setting_menu);
    evas_object_del(app->setting_menu2);

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
    }
    
    fprintf(stderr,"Updating....\n");
    for (short i=0; i < app->config->stationsList().size();i++){
        app->config->stationsList().at(i)->updateData(true);
    }

    //delete app->config;
    app->config = ConfigEfl::Instance(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");

    create_main_window(data);
}

static void
_delete_station_cb(void *data, Evas_Object *obj, void *event_info)
{
    struct _App *app = (struct _App*)data;
    Evas_Object *edje_obj;
    Evas *evas;
    Evas_Object *temp_edje_obj = NULL;

    evas = evas_object_evas_get(app->win);	
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
    evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());
    evas_object_show(edje_obj);
    app->dialog = edje_obj;
}

static Evas_Object *
_item_content_add_get(void *data, Evas_Object *obj, const char *part)
{

   Evas_Object *ic = elm_icon_add(obj);
   if (!strcmp(part, "elm.swallow.end")){
        elm_icon_standard_set(ic, "save");
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

    evas = evas_object_evas_get(app->win);	

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
    evas_object_move(list, 0, app->config->get_screen_height()*0.1);
    evas_object_resize(list, app->config->get_screen_width(), app->config->get_screen_height() - app->config->get_screen_height()*0.12);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    edje_obj_menu = edje_object_add(evas);

    /* Set layout Edje File */
    Evas_Object *layout, *ed, *pg ;
    layout = elm_layout_add(app->win);
   
    pg = elm_naviframe_add(app->win);
    evas_object_size_hint_weight_set(pg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(pg, 0.0,0.0);
    evas_object_show(pg);


    if(elm_layout_file_set(layout, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj","menu")){
       ed = elm_layout_edje_get(layout);
       /* Set callback functions */
       edje_object_signal_callback_add(ed, "clicked", "back", close_setting_window2, app);
       elm_naviframe_item_simple_push(pg, layout);
       evas_object_show(layout);
       evas_object_move(pg, 0, app->config->get_screen_height() - app->config->get_screen_height()*0.075);
       evas_object_resize(pg, app->config->get_screen_width(),  app->config->get_screen_height()*0.075);
       app->setting_menu = layout;
    }

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

    evas = evas_object_evas_get(app->win);	

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
    evas_object_move(list, 0, app->config->get_screen_height()*0.1);
    evas_object_resize(list, app->config->get_screen_width(), app->config->get_screen_height() - app->config->get_screen_height()*0.12);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    edje_obj_menu = edje_object_add(evas);


  /* Set layout Edje File */
    Evas_Object *layout, *ed, *pg ;
    layout = elm_layout_add(app->win);
   
    pg = elm_naviframe_add(app->win);
    evas_object_size_hint_weight_set(pg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(pg, 0.0,0.0);
    evas_object_show(pg);


    if(elm_layout_file_set(layout, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj","menu")){
       ed = elm_layout_edje_get(layout);
       /* Set callback functions */
       edje_object_signal_callback_add(ed, "clicked", "back", close_setting_window2, app);
       elm_naviframe_item_simple_push(pg, layout);
       evas_object_show(layout);
       evas_object_move(pg, 0, app->config->get_screen_height() - app->config->get_screen_height()*0.075);
       evas_object_resize(pg, app->config->get_screen_width(),  app->config->get_screen_height()*0.075);
       app->setting_menu = layout;
    }

    app->setting_menu2 = edje_obj_menu;

}

static void _gl_sel_repeat_sub(void *data, Evas_Object * obj, void *event_info)
{
	Elm_Object_Item *gli = (Elm_Object_Item *) (event_info);
	elm_genlist_item_selected_set(gli, 0);
	Evas_Event_Mouse_Up *ev = (Evas_Event_Mouse_Up *) event_info;

	list_item_data *item_data = (list_item_data *) data;
	struct _App *app =  (struct _App*)item_data->app;

	int cur_focuse = elm_radio_value_get(eo_radiogroup_repeat);
	if (item_data->index == cur_focuse) 
		return;
	elm_radio_value_set(eo_radiogroup_repeat, item_data->index);
//	_alarm_view_snooze_save_repeat(ad);
	elm_genlist_item_update(item_repeat);
}

static char *
_item_list_label_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{	

	char *ret = NULL;
	list_item_data *item_data = (list_item_data *) data;

	if (!strcmp(part, "elm.text.1")) {
		ret = strdup(item_data->text1);
	} else if (!strcmp(part,"elm.text.2")) {
		char temp[32] = { 0 };
        /*
		snprintf(temp, sizeof(temp), "%s", 
	            _(title_duration
			   [_DURATION_GET_KEY(g_snooze_duration_cur)]));
        */
        snprintf(temp, sizeof(temp), "%s", 
	            title_temperature[g_temperature_cur]);

		char *text2 = temp;
		snprintf(item_data->text2, sizeof(item_data->text2), "%s",
			 text2);
		ret = strdup(item_data->text2);
	}
	return ret;
}


static char *_gl_label_get_repeat_sub(void *data, Evas_Object *obj,
                                              const char *part)
{

std::cerr<<"*_gl_label_get_repeat_sub"<<std::endl;
    char *ret = NULL;
    list_item_data *item_data = (list_item_data *)data;

    if (!strcmp(part, "elm.text")) {
                   ret = strdup(item_data->text1);
    std::cerr<<"*_gl_label_get_repeat_sub "<< item_data->text1<<std::endl;
    }
    return ret;
}
static Evas_Object *_gl_icon_get_repeat_sub(void *data, Evas_Object * obj,
					    const char *part)
{
//	retvm_if(!data, NULL, "data null");
std::cerr<<"*_gl_icon_get_repeat_sub"<<std::endl;
	Evas_Object *ret = NULL;
	list_item_data *item_data = (list_item_data *) data;
//	struct appdata *ad = item_data->ad;
	//snooze_view *view = ad->view_snooze;

    struct _App *app = (struct _App*)data;

	if (!strcmp(part, "elm.icon")) {
        std::cerr<<"*_gl_icon_get_repeat_sub icon"<<std::endl;
		ret = elm_radio_add(obj);
		elm_radio_state_value_set(ret, item_data->index);
		elm_radio_group_add(ret, eo_radiogroup_repeat);
		item_data->icon = ret;
	}
//  evas_object_propagate_events_set(ret, EINA_FALSE);
	return ret;
}

static void 
_set_expand_cb(void *data, Evas_Object * obj, void *event_info)
{
	Elm_Object_Item *gli = (Elm_Object_Item *) (event_info);
	elm_genlist_item_selected_set(gli, 0);
	Evas_Event_Mouse_Up *ev = (Evas_Event_Mouse_Up *) event_info;
	Eina_Bool status = elm_genlist_item_expanded_get(gli);
	elm_genlist_item_expanded_set(gli, !status);
}


static void _units_gl_exp(void *data, Evas_Object * obj,
                                              void *event_info){
    Elm_Genlist_Item_Class _itc;
	int i = 0;
	char temp[32] = { 0 };
	list_item_data *item_data = NULL;

    struct _App *app = (struct _App*)data;
    std::cerr<<"_units_gl_exp "<<list << " " << item_repeat<<std::endl;

    _itc_sub.item_style  = "dialogue/1text.1icon/expandable2";
    _itc_sub.func.text_get = _gl_label_get_repeat_sub;
    _itc_sub.func.content_get = _gl_icon_get_repeat_sub;
    _itc_sub.func.state_get = NULL;
    _itc_sub.func.del = NULL; 
    //_itc.func.del = default_gl_dg_item_dell
    
	eo_radiogroup_repeat = elm_radio_add(list);
	elm_radio_value_set(eo_radiogroup_repeat, -1);
    for (i = 0; i != MAX_TEMPERATURE_ITEM_NUM; ++i) {
       // snprintf(temp, sizeof(temp), "%s", _(title_duration[i]));
       snprintf(temp, sizeof(temp), "%s", title_temperature[i]);
       item_data = list_item_create(i, temp, NULL, NULL, app);
       item_data->item =
		    elm_genlist_item_append(list, &_itc_sub,
					    (void *)item_data,
					    item_repeat,
					    ELM_GENLIST_ITEM_NONE,
					    _gl_sel_repeat_sub, item_data);
    }

	elm_radio_value_set(eo_radiogroup_repeat, 0);
    evas_object_show(eo_radiogroup_repeat);
}

static void _units_gl_con(void *data, Evas_Object * obj,
				      void *event_info)
{
    std::cerr<<"_units_gl_con"<<std::endl;
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;
	elm_genlist_item_subitems_clear(item);
    /* Save Config */
}



void 
create_units_window(void *data){
    struct _App *app = (struct _App*)data;
    Evas_Object *edje_obj;
    Evas_Object *edje_obj_menu;
    Evas *evas;
    Evas_Object *temp_edje_obj = NULL;
    Elm_Object_Item *item;

    list_item_data *item_data;
    int i;

    evas = evas_object_evas_get(app->win);	

    edje_obj = edje_object_add(evas);
    fprintf(stderr,"Create units window....\n");
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "manageunitswindow")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'manageunitswindow' from settingwindow.edj"
                        " %s\n", errmsg);
    }
    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "temperature_list");
    list = elm_genlist_add(temp_edje_obj); 
    evas_object_smart_callback_add(list,
                                   "expanded",
                                   _units_gl_exp, app);
    evas_object_smart_callback_add(list,
                                   "contracted",
                                   _units_gl_con, app);

	elm_genlist_tree_effect_enabled_set(list, EINA_FALSE);
                 _itc.item_style = "dialogue/2text/expandable";
                 _itc.func.text_get = _item_list_label_get;
                 _itc.func.content_get = NULL;
                 _itc.func.del = NULL;

    item_data = (list_item_data *)calloc(1, sizeof(list_item_data));
    item_data->index = 0;
    item_data->icon = NULL;
    item_data->app = app;
    snprintf(item_data->text1, sizeof(item_data->text1), "Temperature");

    item_data->item = elm_genlist_item_append(list, &_itc,
                    (void *)item_data, NULL,
                    ELM_GENLIST_ITEM_TREE,
                    _set_expand_cb, item_data);
    item_repeat = item_data->item;
	elm_genlist_item_expanded_set(item_data->item, true);

    
    evas_object_show(list);

 evas_object_move(list, 0, app->config->get_screen_height()*0.1);

    evas_object_resize(list, app->config->get_screen_width(), app->config->get_screen_height() - app->config->get_screen_height()*0.12);

    edje_obj_menu = edje_object_add(evas);


  /* Set layout Edje File */
    Evas_Object *layout, *ed, *pg ;
    layout = elm_layout_add(app->win);
   
    pg = elm_naviframe_add(app->win);
    evas_object_size_hint_weight_set(pg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(pg, 0.0,0.0);
    evas_object_show(pg);


    if(elm_layout_file_set(layout, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj","menu")){
       ed = elm_layout_edje_get(layout);
       /* Set callback functions */
       edje_object_signal_callback_add(ed, "clicked", "back", close_setting_window2, app);
       elm_naviframe_item_simple_push(pg, layout);
       evas_object_show(layout);
       evas_object_move(pg, 0, app->config->get_screen_height() - app->config->get_screen_height()*0.075);
       evas_object_resize(pg, app->config->get_screen_width(),  app->config->get_screen_height()*0.075);
       app->setting_menu = layout;
    }

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

    evas = evas_object_evas_get(app->win);	

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

    evas_object_move(list, 0, app->config->get_screen_height()*0.1);
    evas_object_resize(list, app->config->get_screen_width(), app->config->get_screen_height() - app->config->get_screen_height()*0.12);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    edje_obj_menu = edje_object_add(evas);
    /* Set layout Edje File */
    Evas_Object *layout, *ed, *pg ;
    layout = elm_layout_add(app->win);
   
    pg = elm_naviframe_add(app->win);
    evas_object_size_hint_weight_set(pg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(pg, 0.0,0.0);
    evas_object_show(pg);

    if(elm_layout_file_set(layout, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj","menu")){
       ed = elm_layout_edje_get(layout);
       /* Set callback functions */
       edje_object_signal_callback_add(ed, "clicked", "back", close_setting_window2, app);
       elm_naviframe_item_simple_push(pg, layout);
       evas_object_show(layout);
       evas_object_move(pg, 0, app->config->get_screen_height() - app->config->get_screen_height()*0.075);
       evas_object_resize(pg, app->config->get_screen_width(),  app->config->get_screen_height()*0.075);
       app->setting_menu = layout;
    }
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

    evas = evas_object_evas_get(app->win);	

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

    evas_object_move(list, 0, app->config->get_screen_height()*0.1);
    evas_object_resize(list, app->config->get_screen_width(), app->config->get_screen_height() - app->config->get_screen_height()*0.15);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

 /* Set layout Edje File */
    Evas_Object *layout, *ed, *pg ;
    layout = elm_layout_add(app->win);
   
    pg = elm_naviframe_add(app->win);
    evas_object_size_hint_weight_set(pg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(pg, 0.0,0.0);
    evas_object_show(pg);

    if(elm_layout_file_set(layout, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj","menu")){
       ed = elm_layout_edje_get(layout);
       /* Set callback functions */
       edje_object_signal_callback_add(ed, "clicked", "back", close_setting_window2, app);
       elm_naviframe_item_simple_push(pg, layout);
       evas_object_show(layout);
       evas_object_move(pg, 0, app->config->get_screen_height() - app->config->get_screen_height()*0.075);
       evas_object_resize(pg, app->config->get_screen_width(),  app->config->get_screen_height()*0.075);
       app->setting_menu2 = layout;
    }
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
    evas = evas_object_evas_get(app->win);	
    edje_obj = edje_object_add(evas);
    fprintf(stderr,"Mange location window....\n");
    /* exercising Edje loading error, on purpose */
    if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj", "managelistwindow")){
        Edje_Load_Error err = edje_object_load_error_get(edje_obj);
        const char *errmsg = edje_load_error_str(err);
        fprintf(stderr, "Could not load 'managelistwindow' from settingwindow.edj"
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

    evas_object_move(list, 0, app->config->get_screen_height()*0.1);
    evas_object_resize(list, app->config->get_screen_width(), app->config->get_screen_height() - app->config->get_screen_height()*0.15);
    evas_object_show(list);
    app->list = list;

    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());
    evas_object_show(edje_obj);
    app->setting_top_main_window2 = edje_obj;

    /* Set layout Edje File */
    Evas_Object *layout, *ed, *pg ;
    layout = elm_layout_add(app->win);
   
    pg = elm_naviframe_add(app->win);
    evas_object_size_hint_weight_set(pg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(pg, 0.0,0.0);
    evas_object_show(pg);

    if(elm_layout_file_set(layout, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj","menu_managelocations")){
       ed = elm_layout_edje_get(layout);
       /* Set callback functions */
       edje_object_signal_callback_add(ed, "clicked", "back", close_setting_window2, app);
       edje_object_signal_callback_add(ed, "clicked", "add", prepare_for_sources_window, app);
       elm_naviframe_item_simple_push(pg, layout);
       evas_object_show(layout);
       evas_object_move(pg, 0, app->config->get_screen_height() - app->config->get_screen_height()*0.075);
       evas_object_resize(pg, app->config->get_screen_width(),  app->config->get_screen_height()*0.075);
       app->setting_menu2 = layout;
    }


}
static void
manage_location_window(void *data, Evas *e, Evas_Object *o, void *event_info){
    create_location_window(data);
}

static void
manage_units_window(void *data, Evas *e, Evas_Object *o, void *event_info){
    create_units_window(data);
}

static void
_signal_cb(void *data, Evas_Object * o, const char *emission, const
           char *source)
           {
   std::cerr<<"ddddddddddddddddddd"<<std::endl;
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


    evas = evas_object_evas_get(app->win);	

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
    temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj, "measurement_units_box");
    evas_object_event_callback_add(temp_edje_obj, EVAS_CALLBACK_MOUSE_DOWN, manage_units_window, app); 



    evas_object_move(edje_obj, 0, 0);
    evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());
    evas_object_show(edje_obj);
    app->setting_top_main_window = edje_obj;

    /* Set layout Edje File */
    Evas_Object *layout, *ed, *pg ;
    layout = elm_layout_add(app->win);
   
    pg = elm_naviframe_add(app->win);
    evas_object_size_hint_weight_set(pg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(pg, 0.0,0.0);
    evas_object_show(pg);

    if(elm_layout_file_set(layout, "/opt/apps/com.meecast.omweather/share/edje/settingwindow.edj","menu")){
       ed = elm_layout_edje_get(layout);
       /* Set callback functions */
       edje_object_signal_callback_add(ed, "clicked", "back", close_setting_window, app);
       elm_naviframe_item_simple_push(pg, layout);
       evas_object_show(layout);
       evas_object_move(pg, 0, app->config->get_screen_height() - app->config->get_screen_height()*0.075);
       evas_object_resize(pg, app->config->get_screen_width(),  app->config->get_screen_height()*0.075);
       app->setting_menu = layout;
    }
}


