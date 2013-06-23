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
#include <FUi.h>
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
    //Tizen::Ui::Controls::Panel  *backgroundPanel = static_cast<Panel*>(GetControl(L"IDC_PANEL_BACKGROUND"));
   // backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(0x50, 0xFF, 0x38));

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

Tizen::Graphics::Color*
meecastMainForm::GetTemperatureColor(int t){
        Tizen::Graphics::Color* color = new (std::nothrow)Tizen::Graphics::Color();
        int c1, c2, c3;
        if (_config->TemperatureUnit() == "F"){
             t = (t - 32) * 5 / 9;
        }

        if (t >= 30){
            c2 = ((t - 50.0)*(246.0/255.0-60.0/255.0)/(30.0-50.0) + 60.0/255.0)*255.0;
            color->SetColorComponents(255, c2, 0);
        }else if (t < 30 && t >= 15){
            c1 = (((t - 30.0)*(114.0/255.0-1.0)/(15.0-30.0) + 1.0))*255.0;
            c2 = ((t - 30.0)*(1.0-246.0/255.0)/(15.0-30.0) + 246.0/255.0)*255.0;
            color->SetColorComponents(c1, c2, 0);
        }else if (t < 15 && t >= 0){
            c1 = ((t - 15.0)*(1.0-114.0/255.0)/(0.0-15.0) + 144.0/255.0)*255.0;
            c3 = ((t - 15.0)*(1.0-0.0)/(0.0-15.0) + 0.0)*255.0;
            color->SetColorComponents(c1, 255, c3);
        }else if (t < 0 && t >= -15){
            c1 = ((t - 0.0)*(0.0-1.0)/(-15.0-0.0) + 1.0)*255.0;
            c2 = ((t - 0.0)*(216.0/255.0-1.0)/(-15.0-0.0) + 1.0)*255.0;
            color->SetColorComponents(c1, c2, 255);
        }
        else if (t < -15 && t >= -30){
            c2 = ((t - (-15.0))*(66/255.0-216.0/255.0)/(-30.0+15.0) + 216.0/255.0)*255.0;
            color->SetColorComponents(0, c2, 255);
        }else if (t < -30){
            c1 = ((t - (-30.0))*(132.0/255.0-0.0)/(-30.0+15.0) + 0.0)*255.0;
            c2 = ((t - (-30.0))*(0.0-66.0/255.0)/(-30.0+15.0) + 66.0/255.0)*255.0;
            color->SetColorComponents(c1, c2, 255);
        }
   return(color);
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
    char  buffer[4096];
    Tizen::Ui::Controls::Label  *station_label = static_cast<Label*>(GetControl(L"IDC_LABEL_STATION_NAME"));
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    Tizen::Ui::Controls::Label  *main_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_MAIN_ICON"));
    Tizen::Ui::Controls::Label  *main_description = static_cast<Label*>(GetControl(L"IDC_LABEL_MAIN_DESCRIPTION"));
    Tizen::Ui::Controls::Label  *main_temperature = static_cast<Label*>(GetControl(L"IDC_LABEL_MAIN_TEMPERATURE"));
    Tizen::Ui::Controls::Label  *main_current_state = static_cast<Label*>(GetControl(L"IDC_LABEL_CURRENT_STATE"));
    Tizen::Ui::Controls::Label  *main_humidity_text = static_cast<Label*>(GetControl(L"IDC_LABEL_HUMIDITY_TEXT"));
    Tizen::Ui::Controls::Label  *main_humidity_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_HUMIDITY_ICON"));
    Tizen::Ui::Controls::Label  *main_background_wind_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_WIND_BACKGROUND"));
    Tizen::Ui::Controls::Label  *main_wind_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_WIND_DIRECTION"));
    Tizen::Ui::Controls::Label  *main_wind_text = static_cast<Label*>(GetControl(L"IDC_LABEL_WINDDIRECTION_TEXT"));
    Tizen::Ui::Controls::Label  *main_wind_speed_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_WIND_SPEED_ICON"));
    Tizen::Ui::Controls::Label  *main_wind_speed_text = static_cast<Label*>(GetControl(L"IDC_LABEL_WINDSPEED_TEXT"));
    Tizen::Ui::Controls::Label  *main_pressure_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_PRESSURE_ICON"));
    Tizen::Ui::Controls::Label  *main_pressure_text = static_cast<Label*>(GetControl(L"IDC_LABEL_PRESSURE_TEXT"));
    Tizen::Ui::Controls::Panel  *backgroundPanel = static_cast<Panel*>(GetControl(L"IDC_PANEL_BACKGROUND"));

    Tizen::Ui::Controls::ListView  *main_listview_forecast = static_cast<ListView*>(GetControl(L"IDC_LISTVIEW_FORECASTS"));

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

    main_humidity_icon->SetShowState(false);
    main_humidity_text->SetShowState(false);
    main_current_state->SetShowState(false);
    main_icon->SetShowState(false);
    main_temperature->SetShowState(false);
    main_description->SetShowState(false);
    main_background_wind_icon->SetShowState(false);
    main_wind_icon->SetShowState(false);
    main_wind_text->SetShowState(false);
    main_wind_speed_icon->SetShowState(false);
    main_wind_speed_text->SetShowState(false);
    main_pressure_icon->SetShowState(false);
    main_pressure_text->SetShowState(false);


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

        Tizen::Base::Integer icon_int =  temp_data->Icon();
        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Atmos/" + icon_int.ToString() + ".png")){
            /* Main Icon */ 
            Tizen::Media::Image *image = null;
            Tizen::Graphics::Bitmap* mainIconBitmap = null;
            image = new (std::nothrow) Tizen::Media::Image();
            image->Construct();

            String icon_number = temp_data->Icon();

            main_icon->SetShowState(true);
            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Atmos/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
            main_icon->SetBackgroundBitmap(*mainIconBitmap);
            main_icon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }

        /* Description for current weather forecast */
        String str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(temp_data->Text().c_str(), str);
        main_description->SetText(str);
        main_description->RequestRedraw();
        main_description->SetShowState(true);
        
        int t = INT_MAX;
        /* Temperature */
        if (temp_data->temperature().value(true) == INT_MAX){
          if ((temp_data->temperature_hi().value(true) == INT_MAX) &&
              (temp_data->temperature_low().value(true) == INT_MAX)){ 
            main_description->SetText("N/A");
          } 
          if ((temp_data->temperature_hi().value(true) != INT_MAX) &&
              (temp_data->temperature_low().value(true) != INT_MAX)){ 
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°/ %0.f°", temp_data->temperature_low().value(),
                                                                 temp_data->temperature_hi().value());
            t = temp_data->temperature_hi().value();
          }  
          if (temp_data->temperature_hi().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
            t = temp_data->temperature_hi().value();
          }
          if (temp_data->temperature_low().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
            t = temp_data->temperature_low().value();
          }
        }else{
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature().value());
            t = temp_data->temperature().value();
        }
        int c1, c2, c3;
        if (_config->TemperatureUnit() == "F"){
             t = (t - 32) * 5 / 9;
        }

        if (t >= 30){
            c2 = ((t - 50.0)*(246.0/255.0-60.0/255.0)/(30.0-50.0) + 60.0/255.0)*255.0;
            backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(255, c2, 0));
        }else if (t < 30 && t >= 15){
            c1 = (((t - 30.0)*(114.0/255.0-1.0)/(15.0-30.0) + 1.0))*255.0;
            c2 = ((t - 30.0)*(1.0-246.0/255.0)/(15.0-30.0) + 246.0/255.0)*255.0;
            backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(c1, c2, 0));
        }else if (t < 15 && t >= 0){
            c1 = ((t - 15.0)*(1.0-114.0/255.0)/(0.0-15.0) + 144.0/255.0)*255.0;
            c3 = ((t - 15.0)*(1.0-0.0)/(0.0-15.0) + 0.0)*255.0;
            backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(c1, 255, c3));
        }else if (t < 0 && t >= -15){
            c1 = ((t - 0.0)*(0.0-1.0)/(-15.0-0.0) + 1.0)*255.0;
            c2 = ((t - 0.0)*(216.0/255.0-1.0)/(-15.0-0.0) + 1.0)*255.0;
            backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(c1, c2, 255));
        }
        else if (t < -15 && t >= -30){
            c2 = ((t - (-15.0))*(66/255.0-216.0/255.0)/(-30.0+15.0) + 216.0/255.0)*255.0;
            backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(0, c2, 255));
        }else if (t < -30){
            c1 = ((t - (-30.0))*(132.0/255.0-0.0)/(-30.0+15.0) + 0.0)*255.0;
            c2 = ((t - (-30.0))*(0.0-66.0/255.0)/(-30.0+15.0) + 66.0/255.0)*255.0;
 
            backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(c1, c2, 255));
        }

        backgroundPanel->RequestRedraw();
        main_temperature->SetShowState(true);
        Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
        main_temperature->SetText(str);
        main_temperature->RequestRedraw();
        /* Current or not current period */
        if (temp_data->Current())
            Tizen::Base::Utility::StringUtil::Utf8ToString("Now", str);
        else
            Tizen::Base::Utility::StringUtil::Utf8ToString("Today", str);
        main_current_state->SetShowState(true);
        main_current_state->SetText(str);
        main_current_state->RequestRedraw();

        /* Main humidity */
        if (temp_data->Humidity() != INT_MAX){
            main_humidity_text->SetShowState(true);
            main_humidity_icon->SetShowState(true);

            snprintf (buffer, sizeof(buffer) -1, "%i%%", temp_data->Humidity());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            main_humidity_text->SetText(str);
            main_humidity_text->RequestRedraw();
        }else{
            main_humidity_text->SetShowState(false);
            main_humidity_icon->SetShowState(false);
        }
        
        /* Main wind direction */
        if (temp_data->WindDirection() != "N/A"){
            snprintf (buffer, sizeof(buffer) -1, "%s", temp_data->WindDirection().c_str());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);

            if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/wind_direction_arrow_" + str + ".png")){
                main_background_wind_icon->SetShowState(true);
                main_wind_icon->SetShowState(true);
                main_wind_text->SetShowState(true);
                main_wind_text->SetText(str);
                main_wind_text->RequestRedraw();

                /* Wind direction Icon */ 
                Tizen::Media::Image *image = null;
                Tizen::Graphics::Bitmap* windIconBitmap = null;
                image = new (std::nothrow) Tizen::Media::Image();
                image->Construct();

                windIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"720x1280/wind_direction_arrow_" + str + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
                main_wind_icon->SetBackgroundBitmap(*windIconBitmap);
                main_wind_icon->RequestRedraw();
                SAFE_DELETE(image);
                SAFE_DELETE(windIconBitmap);
            }
        }
        /* Main wind speed */
        if (temp_data->WindSpeed().value() != INT_MAX){
            main_wind_speed_text->SetShowState(true);
            main_wind_speed_icon->SetShowState(true);
            snprintf (buffer, sizeof(buffer) -1, "%0.f %s", 
                                             temp_data->WindSpeed().value(), _config->WindSpeedUnit().c_str());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            main_wind_speed_text->SetText(str);
            main_wind_speed_text->RequestRedraw();
        }else{
            main_wind_speed_text->SetShowState(false);
            main_wind_speed_icon->SetShowState(false);
        }
        /* Main presssure */
        if (temp_data->pressure().value(true) != INT_MAX){
            snprintf (buffer, sizeof(buffer) -1, "%i %s", (int)temp_data->pressure().value(), _config->PressureUnit().c_str());
            main_pressure_text->SetShowState(true);
            main_pressure_icon->SetShowState(true);
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            main_pressure_text->SetText(str);
            main_pressure_text->RequestRedraw();
       }



    main_listview_forecast->SetItemProvider(*this);
    /* Fill list of days with weather forecast */
    /* set current day */ 
    time_t current_day;
    struct tm   *tm = NULL;
    current_day = time(NULL);
    tm = gmtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    current_day = mktime(tm); /* today 00:00:00 */

    /* fill other days */
    int i = 3600*24;
    int j = 0;

    _dayCount = 0;
    while  ( (temp_data = _config->dp->data().GetDataForTime( current_day + 14 * 3600  + i))) {
            temp_data->temperature_low().units(_config->TemperatureUnit());
            temp_data->temperature_hi().units(_config->TemperatureUnit());
            temp_data->temperature().units(_config->TemperatureUnit());

            //edje_object_part_text_set(edje_obj_block, "full_day_name", temp_data->FullDayName().c_str());
            /* Icon */
            snprintf(buffer, sizeof(buffer) - 1, "%s/%s/%i.png", _config->iconspath().c_str(), 
                                                                 _config->iconSet().c_str(), temp_data->Icon());
//            param.type = EDJE_EXTERNAL_PARAM_TYPE_STRING;
//            param.name = "icon";
//            param.s = buffer;
           // edje_object_part_external_param_set (edje_obj_block, "icon", &param);


            if (temp_data->temperature_low().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
             //   edje_object_part_text_set(edje_obj_block, "min_temp", buffer);
            }
            if (temp_data->temperature_hi().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
             //   edje_object_part_text_set(edje_obj_block, "max_temp", buffer);
              //  temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_block, "max_temp");
               // set_color_by_temp(temp_edje_obj, (int)temp_data->temperature_hi().value(true));
               // temp_edje_obj = NULL;
            }
           // evas_object_box_append(list_box, edje_obj_block);
           // evas_object_show(edje_obj_block);
            if (j % 2 == 0 ){
             //   temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_block, "bg_rect");
              //  evas_object_color_set(temp_edje_obj, 16, 16, 16, 255);
              //  temp_edje_obj = NULL;
            }
            i = i + 3600*24;
            j++;
            _dayCount ++;
        }

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
    }else{
        /* Main Icon  N/A*/ 
        Tizen::Media::Image *image = null;
        Tizen::Graphics::Bitmap* mainIconBitmap = null;
        image = new (std::nothrow) Tizen::Media::Image();
        image->Construct();

        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-size-normal/icons/Atmos/na.png")){
            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-size-normal/icons/Atmos/na.png", BITMAP_PIXEL_FORMAT_ARGB8888);
            main_icon->SetBackgroundBitmap(*mainIconBitmap);
            main_icon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }

    }

   main_listview_forecast->UpdateList();

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


int
meecastMainForm::GetItemCount(void)
{
    return _dayCount;
}



bool
meecastMainForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}


Tizen::Ui::Controls::ListItemBase*
meecastMainForm::CreateItem (int index, int itemWidth)
{
    char  buffer[4096];
    CustomItem* pItem = new (std::nothrow) CustomItem();
    TryReturn(pItem != null, null, "Out of memory");

    pItem->Construct(Tizen::Graphics::Dimension(itemWidth, 100), LIST_ANNEX_STYLE_NORMAL);

//    String* pStr = dynamic_cast< String* >(__map->GetValue(Integer(index)));
    /* set current day */ 
    time_t current_day;
    struct tm   *tm = NULL;
    current_day = time(NULL);
    tm = gmtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    current_day = mktime(tm); /* today 00:00:00 */

    /* fill other days */
    int i = 3600*24;
    String* pStr;
    Core::Data *temp_data = NULL;
    if ((temp_data = _config->dp->data().GetDataForTime( current_day + 14 * 3600  + index*3600*24))) {
            temp_data->temperature_low().units(_config->TemperatureUnit());
            temp_data->temperature_hi().units(_config->TemperatureUnit());
            temp_data->temperature().units(_config->TemperatureUnit());

            //edje_object_part_text_set(edje_obj_block, "full_day_name", temp_data->FullDayName().c_str());
            pItem->AddElement(Tizen::Graphics::Rectangle(10, 32, 220, 50), 0, temp_data->FullDayName().c_str(), false);
            /* Icon */
            snprintf(buffer, sizeof(buffer) - 1, "icons/Atmos/%i.png", temp_data->Icon());
            pItem->AddElement(Tizen::Graphics::Rectangle(320, 0, 100, 100), 1, *Application::GetInstance()->GetAppResource()->GetBitmapN(buffer), null, null);

            if (temp_data->temperature_low().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
                Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(10);
                pItem->AddElement(Tizen::Graphics::Rectangle(500, 30, 100, 60), 2, buffer, false);
                delete color_of_temp;
            }
            if (temp_data->temperature_hi().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
                Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(temp_data->temperature_hi().value());
                pItem->AddElement(Tizen::Graphics::Rectangle(620, 30, 100, 60), 3, buffer, 40, *color_of_temp, *color_of_temp, *color_of_temp);
                delete color_of_temp;
            }
           // evas_object_box_append(list_box, edje_obj_block);
           // evas_object_show(edje_obj_block);
        //    if (j % 2 == 0 ){
             //   temp_edje_obj = (Evas_Object*)edje_object_part_object_get(edje_obj_block, "bg_rect");
              //  evas_object_color_set(temp_edje_obj, 16, 16, 16, 255);
              //  temp_edje_obj = NULL;
        //    }
        }

    return pItem;
}

void
meecastMainForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
    ConfigTizen *config;

	if (status == LIST_ITEM_STATUS_SELECTED){

   	}
}
void
meecastMainForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
meecastMainForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state)
{
}

void
meecastMainForm::OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex)
{
}


