/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen2
 *
 * Copyright (C) 2013 Vlad Vasilyeu
 *     for the code
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
#include <FApp.h>
#include "meecastMainForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;

meecastMainForm::meecastMainForm(void):
                 __pContextMenuText(null)
{
}

meecastMainForm::~meecastMainForm(void)
{
}

bool
meecastMainForm::Initialize(void)
{
    Construct(L"IDF_FORM");
    return true;
}

result
meecastMainForm::OnInitializing(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your initialization code here

    CreateContextMenuList();
    // Setup back event listener
    SetFormBackEventListener(this);

    // Get a button via resource ID
    Tizen::Ui::Controls::Button *pButtonOk = static_cast<Button*>(GetControl(L"IDC_BUTTON_OK"));
    if (pButtonOk != null)
    {
        pButtonOk->SetActionId(ID_BUTTON_OK);
        pButtonOk->AddActionEventListener(*this);
    }

    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    if (right_label != null)
		right_label->AddTouchEventListener(*this);
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    if (left_label != null)
		left_label->AddTouchEventListener(*this);

    /* Footer */
    Footer* pFooter = GetFooter();
    pFooter->SetStyle(FOOTER_STYLE_BUTTON_ICON_TEXT);

    FooterItem menuButton;
    menuButton.Construct(ID_BUTTON_MENU);
    menuButton.SetText("Menu");

    FooterItem updateButton;
    updateButton.Construct(ID_BUTTON_UPDATE);
    updateButton.SetText("Update");

    pFooter->AddItem(menuButton);
    pFooter->AddItem(updateButton);

    pFooter->AddActionEventListener(*this);

    _config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");
    // Get a button via resource ID
    Tizen::Ui::Controls::Panel  *backgroundPanel = static_cast<Panel*>(GetControl(L"IDC_PANEL_BACKGROUND"));
    backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(0x50, 0xFF, 0x38));

  //  Tizen::Media::Image image;
//	image.Construct();
 //   __pImage = image.DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-size-normal/images/mask_background_main.png", BITMAP_PIXEL_FORMAT_ARGB8888);
  //  backgroundPanel->SetBackgroundBitmap(*__pImage);

    return r;
}

result
meecastMainForm::OnTerminating(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastMainForm::OnTouchPressed(const Tizen::Ui::Control& source, 
                                const Tizen::Graphics::Point& currentPosition,
		                        const Tizen::Ui::TouchEventInfo& touchInfo) {

    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
	if (source.Equals(*left_label))
	{
        if ((uint)(_config->current_station_id() - 1) >= 0)
            _config->current_station_id(_config->current_station_id() - 1);
        else
            _config->current_station_id(_config->stationsList().size());
        _config->saveConfig();

        ReInitElements(); 
        AppLog("Left Touch Screen");
	}
    if (source.Equals(*right_label))
	{
        if ((uint)(_config->current_station_id() + 1) < _config->stationsList().size())
            _config->current_station_id(_config->current_station_id() + 1);
        else
            _config->current_station_id(0);
        _config->saveConfig();

        ReInitElements(); 

        AppLog("Right Touch Screen");
	}
}


void
meecastMainForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);

    AppLog("Check Action");
    switch(actionId)
    {
    case ID_BUTTON_OK:
        AppLog("OK Button is clicked!");
        break;
    case ID_BUTTON_MENU:
        AppLog("Menu Button is clicked!");
        __pContextMenuText->SetShowState(true);
        __pContextMenuText->Show();
        break;
    case ID_MENU_SETTINGS:
        AppLog("Settings Button is clicked!");
        pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_SETTINGSSCENE"));
        break;
    case ID_BUTTON_UPDATE: 
        AppLog("Settings Update is clicked!");
        _config->updatestations();
        break;
    default:
        break;
    }
}

void
meecastMainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
    UiApp* pApp = UiApp::GetInstance();
    AppAssert(pApp);
    pApp->Terminate();
}


void 
meecastMainForm::ReInitElements(void){

    AppLog("REINIT ELEMENTS");
    Tizen::Ui::Controls::Label  *station_label = static_cast<Label*>(GetControl(L"IDC_LABEL_STATION_NAME"));
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    Tizen::Ui::Controls::Label  *main_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_MAIN_ICON"));
    station_label->SetText(_config->stationname().c_str());
    station_label->RequestRedraw();
    if (_config->nextstationname().length() < 1)
        right_label->SetShowState(false);
    else{
        right_label->SetShowState(true);
    }
    if (_config->prevstationname().length() < 1)
        left_label->SetShowState(false);
    else
        left_label->SetShowState(true);

    AppLog("_config->current_station_id() %i", _config->current_station_id());
    AppLog("_config->stationsList().size() %i", _config->stationsList().size());
    //if ((_config->stationsList().size() > 0) && _config->current_station_id() > _config->stationsList().size()) {
    if ((_config->stationsList().size() > 0)) {
        String temp = (App::GetInstance()->GetAppDataPath());
        temp.Append(_config->stationsList().at(_config->current_station_id())->fileName().c_str());
        std::string temp_string = (const char*) (Tizen::Base::Utility::StringUtil::StringToUtf8N(temp)->GetPointer());
       // AppLog("Filename %s", temp_string.c_str());
       //_config->dp = Core::DataParser::Instance(temp_string, "");
       _config->dp = Core::DataParser::Instance(_config->stationsList().at(_config->current_station_id())->fileName().c_str(), "");
    }
    else 
        _config->dp = NULL;

     Core::Data *temp_data = NULL;
     AppLog ("DP %p", _config->dp);
    /* Preparing data */
    if (_config->dp != NULL && (temp_data = _config->dp->data().GetDataForTime(time(NULL)))){    

     AppLog ("_Config_dp inside");
     /* Preparing units */
        temp_data->temperature_low().units(_config->TemperatureUnit());
        temp_data->temperature_hi().units(_config->TemperatureUnit());
        temp_data->temperature().units(_config->TemperatureUnit());
        temp_data->WindSpeed().units(_config->WindSpeedUnit());
        temp_data->pressure().units(_config->PressureUnit());
        /* Main Icon */
//        snprintf(buffer, sizeof(buffer) - 1, "%s/%s/%i.png", 
//                                       app->config->iconspath().c_str(), 
//                                       app->config->iconSet().c_str(), 
//                                       temp_data->Icon());
        /* Main Icon */ 
        Tizen::Media::Image *image = null;
        Tizen::Graphics::Bitmap* mainIconBitmap = null;
        image = new (std::nothrow) Tizen::Media::Image();
        image->Construct();

        String icon_number = temp_data->Icon();
        Tizen::Base::Integer icon_int =  temp_data->Icon();
        mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-size-normal/icons/Atmos/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
        main_icon->SetBackgroundBitmap(*mainIconBitmap);
        main_icon->RequestRedraw();
        SAFE_DELETE(image);
        SAFE_DELETE(mainIconBitmap);

#if 0 
        if (!edje_object_file_set(edje_obj, "/opt/apps/com.meecast.omweather/share/edje/mainwindow.edj", "mainwindow")){
            Edje_Load_Error err = edje_object_load_error_get(edje_obj);
            const char *errmsg = edje_load_error_str(err);
            fprintf(stderr, "Could not load 'mainwindow' from mainwindow.edj:"
                            " %s\n", errmsg);
        }
        evas_object_show(app->top_main_window);
        /* Preparing units */
        temp_data->temperature_low().units(app->config->TemperatureUnit());
        temp_data->temperature_hi().units(app->config->TemperatureUnit());
        temp_data->temperature().units(app->config->TemperatureUnit());
        temp_data->WindSpeed().units(app->config->WindSpeedUnit());
        temp_data->pressure().units(app->config->PressureUnit());

        /* Filling window */
        /* Station name */
        edje_object_part_text_set(edje_obj, "station_name", app->config->stationname().c_str());
        /* Current or not current period */
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
        snprintf(buffer, sizeof(buffer) - 1, "%s/%s/%i.png", 
                                       app->config->iconspath().c_str(), 
                                       app->config->iconSet().c_str(), 
                                       temp_data->Icon());
        param.type = EDJE_EXTERNAL_PARAM_TYPE_STRING;
        param.name = "icon";
        param.s = buffer;
        edje_object_part_external_param_set (edje_obj, "main_icon", &param);
        
        /* Main humidity */
        if (temp_data->Humidity() != INT_MAX){
            snprintf (buffer, sizeof(buffer) -1, "%i%%", temp_data->Humidity());
            edje_object_part_text_set(edje_obj, "humidity_label", buffer);
        }

        /* Main presssure */
        if (temp_data->pressure().value(true) == INT_MAX)
            snprintf (buffer, sizeof(buffer) -1, "N/A");
        else
            snprintf (buffer, sizeof(buffer) -1, "%i", (int)temp_data->pressure().value());
        edje_object_part_text_set(edje_obj, "pressure_label", buffer);
        
        /* Main wind direction */
        snprintf (buffer, sizeof(buffer) -1, "%s", temp_data->WindDirection().c_str());
        edje_object_part_text_set(edje_obj, "wind_direction_label", buffer);
        /* Main wind speed */
        snprintf (buffer, sizeof(buffer) -1, "%0.f %s", 
                                             temp_data->WindSpeed().value(), app->config->WindSpeedUnit().c_str());
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
        evas_object_resize(edje_obj, app->config->get_screen_width(), app->config->get_screen_height());

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
        list_box = evas_object_box_add(app->evass); 
        app->day_list_main_window = list_box;
        evas_object_box_layout_set(
                      list_box, evas_object_box_layout_vertical, NULL, NULL);
        while  (app->dp != NULL && (temp_data = app->dp->data().GetDataForTime( current_day + 14 * 3600  + i))) {
            edje_obj_block = edje_object_add(app->evass);
            if (!edje_object_file_set(edje_obj_block, "/opt/apps/com.meecast.omweather/share/edje/mainwindow.edj", "dayblock")){
                Edje_Load_Error err = edje_object_load_error_get(edje_obj_block);
                const char *errmsg = edje_load_error_str(err);
                fprintf(stderr, "Could not load 'dayblock' from mainwindow.edj:"
                                " %s\n", errmsg);
            }
            temp_data->temperature_low().units(app->config->TemperatureUnit());
            temp_data->temperature_hi().units(app->config->TemperatureUnit());
            temp_data->temperature().units(app->config->TemperatureUnit());

            evas_object_resize(edje_obj_block, app->config->get_screen_width(),  DAY_BLOCK_HEIGHT);
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
        
        /* Insert box to scroller */
        scroller = elm_scroller_add(app->win);
        evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_resize(scroller, app->config->get_screen_width(), app->config->get_screen_height()*0.53);
        evas_object_move(scroller, 0, app->config->get_screen_height()*0.4); 
        elm_object_content_set(scroller, list_box);
        evas_object_show(scroller);
#endif
    }


}

void
meecastMainForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    // TODO:
    // Add your scene activate code here
    AppLog("OnSceneActivatedNi %i", _config->current_station_id());
    AppLog("OnSceneActivatedNi %s", _config->stationname().c_str());
    ReInitElements(); 
}

void
meecastMainForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}

void
meecastMainForm::CreateContextMenuList(void)
{
    __pContextMenuText = new (std::nothrow) ContextMenu();
    __pContextMenuText->Construct(Tizen::Graphics::Point(140, 415), CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
    __pContextMenuText->AddItem("Settings", ID_MENU_SETTINGS);
    __pContextMenuText->AddItem("About", ID_MENU_ABOUT);
    __pContextMenuText->AddActionEventListener(*this);
}


