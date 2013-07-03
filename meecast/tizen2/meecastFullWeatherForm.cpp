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
#include "meecastFullWeatherForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;


meecastFullWeatherForm::meecastFullWeatherForm(void):
                                  __nowButton(null),
                                  __dayButton(null),
                                  __nightButton(null),
                                  __hourlyButton(null){
    _dayNumber = 0;
}

meecastFullWeatherForm::~meecastFullWeatherForm(void)
{
    SAFE_DELETE(__nowButton);
    SAFE_DELETE(__dayButton);
    SAFE_DELETE(__nightButton);
    SAFE_DELETE(__hourlyButton);
}

bool
meecastFullWeatherForm::Initialize(void)
{
    Construct(L"FULL_WEATHER_FORM");
    return true;
}

result
meecastFullWeatherForm::OnInitializing(void)
{
    result r = E_SUCCESS;


    /* Footer */
    Footer* pFooter = GetFooter();
    pFooter->SetStyle(FOOTER_STYLE_SEGMENTED_ICON_TEXT);

    __nowButton = new Tizen::Ui::Controls::FooterItem(); 
    __nowButton->Construct(ID_BUTTON_NOW);
    __nowButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("now_def.png"));
    __nowButton->SetIcon(FOOTER_ITEM_STATUS_SELECTED, Application::GetInstance()->GetAppResource()->GetBitmapN("now_sel.png"));
    __nowButton->SetText(L"Now");

    __dayButton = new Tizen::Ui::Controls::FooterItem(); 
    __dayButton->Construct(ID_BUTTON_DAY);
    __dayButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("day_def.png"));
    __dayButton->SetIcon(FOOTER_ITEM_STATUS_SELECTED, Application::GetInstance()->GetAppResource()->GetBitmapN("day_sel.png"));
    __dayButton->SetText(L"Day");

    __nightButton = new Tizen::Ui::Controls::FooterItem(); 
    __nightButton->Construct(ID_BUTTON_NIGHT);
    __nightButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("night_def.png"));
    __nightButton->SetIcon(FOOTER_ITEM_STATUS_SELECTED, Application::GetInstance()->GetAppResource()->GetBitmapN("night_sel.png"));
    __nightButton->SetText(L"Night");

    __hourlyButton = new Tizen::Ui::Controls::FooterItem(); 
    __hourlyButton->Construct(ID_BUTTON_HOURLY);
    __hourlyButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("hourly_def.png"));
    __hourlyButton->SetIcon(FOOTER_ITEM_STATUS_SELECTED, Application::GetInstance()->GetAppResource()->GetBitmapN("hourly_sel.png"));
    __hourlyButton->SetText(L"Hourly");


    pFooter->AddItem(*__nowButton);
    pFooter->AddItem(*__dayButton);
    pFooter->AddItem(*__nightButton);
    pFooter->AddItem(*__hourlyButton);


    pFooter->SetBackButton();

    // TODO:
    // Add your initialization code here

    // Setup back event listener
    SetFormBackEventListener(this);

    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    if (right_label != null)
		right_label->AddTouchEventListener(*this);
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    if (left_label != null)
		left_label->AddTouchEventListener(*this);
    Tizen::Ui::Controls::Label  *background_label = static_cast<Label*>(GetControl(L"IDC_BACKGROUND_LABEL"));
    if (background_label != null)
        background_label->AddTouchEventListener(*this);

    _config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");


    return r;
}

result
meecastFullWeatherForm::OnTerminating(void){
    result r = E_SUCCESS;

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastFullWeatherForm::OnTouchPressed(const Tizen::Ui::Control& source, 
                                const Tizen::Graphics::Point& currentPosition,
		                        const Tizen::Ui::TouchEventInfo& touchInfo) {
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    Tizen::Ui::Controls::Label  *background_label = static_cast<Label*>(GetControl(L"IDC_BACKGROUND_LABEL"));
	if (source.Equals(*left_label)){
        if ((uint)(_config->current_station_id() - 1) >= 0)
            _config->current_station_id(_config->current_station_id() - 1);
        else
            _config->current_station_id(_config->stationsList().size());
        _config->saveConfig();

        ReInitElements(); 
        AppLog("Left Touch Screen");
	}
    if (source.Equals(*right_label)){
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
meecastFullWeatherForm::GetTemperatureColor(int t){
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
meecastFullWeatherForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);

    AppLog("Check Action");
    switch(actionId)
    {
    default:
        break;
    }
}

void
meecastFullWeatherForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
    SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}


void 
meecastFullWeatherForm::ReInitElements(void){

    AppLog("REINIT ELEMENTS");
    char  buffer[4096];
    Tizen::Ui::Controls::Label  *station_label = static_cast<Label*>(GetControl(L"IDC_LABEL_STATION_NAME"));
    Tizen::Ui::Controls::Label  *main_background_label = static_cast<Label*>(GetControl(L"IDC_BACKGROUND_LABEL"));
    Tizen::Ui::Controls::Label  *main_no_locations_label = static_cast<Label*>(GetControl(L"IDC_LABEL_NO_LOCATIONS"));
    Tizen::Ui::Controls::Button *main_set_locations_button = static_cast<Button*>(GetControl(L"IDC_BUTTON_SET_LOCATIONS"));
    Tizen::Ui::Controls::Label  *main_need_updating = static_cast<Label*>(GetControl(L"IDC_LABEL_NEED_UPDATING"));
    Tizen::Ui::Controls::Button *main_set_try_update_button = static_cast<Button*>(GetControl(L"IDC_BUTTON_TRY_UPDATE"));
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

    if (_config->stationsList().size()!=0){
        station_label->SetText(_config->stationname().c_str());
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background_main.png"));
    }else{
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background.png"));
        station_label->SetText("MeeCast");
    }
    main_background_label->RequestRedraw();
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
    time_t current_day;
    struct tm   *tm = NULL;
    current_day = time(NULL);
    tm = gmtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    current_day = mktime(tm); /* today 00:00:00 */


    int i = 3600*24;
 
    /* Preparing data */
    if ((temp_data = _config->dp->data().GetDataForTime(current_day + 14 * 3600 + _dayNumber*3600*24))) {

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
        Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(t);
        backgroundPanel->SetBackgroundColor(*color_of_temp);
        delete color_of_temp;
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
        
    AppLog("REINIT ELEMENTS333");
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
        main_listview_forecast->SetItemDividerColor(Tizen::Graphics::Color(0x1F, 0x1F, 0x1F)); 
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
        while  ( (temp_data = _config->dp->data().GetDataForTime( current_day + 14*3600 + i))) {
            _dayCount ++;
            i = i + 3600*24;
        }
    }else{
        /* Main Icon  N/A*/ 
        Tizen::Media::Image *image = null;
        Tizen::Graphics::Bitmap* mainIconBitmap = null;
        image = new (std::nothrow) Tizen::Media::Image();
        image->Construct();

        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() +
                                            L"screen-size-normal/icons/Atmos/na.png")){
            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() +
                                            L"screen-size-normal/icons/Atmos/na.png", BITMAP_PIXEL_FORMAT_ARGB8888);
            main_icon->SetBackgroundBitmap(*mainIconBitmap);
            main_icon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }
        _dayCount = 0;
    }
    if (_dayCount == 0){
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background.png"));
        main_background_label->RequestRedraw();
        if (_config->stationsList().size()!=0){
            main_set_try_update_button->SetShowState(true);
            main_need_updating->SetShowState(true);
            main_set_try_update_button->AddActionEventListener(*this);
            backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(0xFF, 0xFF, 0xFF));
        }
    }
    main_listview_forecast->UpdateList();
    backgroundPanel->RequestRedraw();
}

void
meecastFullWeatherForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    // TODO:
    // Add your scene activate code here
    //AppLog("OnSceneActivatedNi %i", _config->current_station_id());
    //AppLog("OnSceneActivatedNi %s", _config->stationname().c_str());
    if (pArgs != null) {
        Integer* pReqId = static_cast<Integer*> (pArgs->GetAt(0));
        _dayNumber = pReqId->ToInt();
        pArgs->RemoveAll(true);
        delete pArgs;
    }

    AppLog("OnSceneActivatedNi %i", _dayNumber);
    ReInitElements(); 
}

void
meecastFullWeatherForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}


int
meecastFullWeatherForm::GetItemCount(void)
{
    return _dayCount;
}



bool
meecastFullWeatherForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}


Tizen::Ui::Controls::ListItemBase*
meecastFullWeatherForm::CreateItem (int index, int itemWidth)
{
    char  buffer[4096];
    CustomItem* pItem = new (std::nothrow) CustomItem();
    TryReturn(pItem != null, null, "Out of memory");

    pItem->Construct(Tizen::Graphics::Dimension(itemWidth, 100), LIST_ANNEX_STYLE_NORMAL);

    //Tizen::Ui::Controls::ListView  *main_listview_forecast = static_cast<ListView*>(GetControl(L"IDC_LISTVIEW_FORECASTS"));

    if ((index-1) %2 != 0 ){
        pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, Tizen::Graphics::Color(0x00, 0x00, 0x00));
    }else{
        pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, Tizen::Graphics::Color(0x1F, 0x1F, 0x1F));
    }
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

        pItem->AddElement(Tizen::Graphics::Rectangle(10, 20, 220, 50), 0, temp_data->DayOfMonthName().c_str(), false);
        if (index != 0)
            pItem->AddElement(Tizen::Graphics::Rectangle(90, 20, 220, 50), 4, temp_data->ShortDayName().c_str(), false);
        else
            pItem->AddElement(Tizen::Graphics::Rectangle(90, 20, 220, 50), 4, L"Today", false);
        /* Icon */
        snprintf(buffer, sizeof(buffer) - 1, "icons/Atmos/%i.png", temp_data->Icon());
        pItem->AddElement(Tizen::Graphics::Rectangle(320, 0, 100, 100), 1, *Application::GetInstance()->GetAppResource()->GetBitmapN(buffer), null, null);

        if (temp_data->temperature_low().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
            Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(10);
            pItem->AddElement(Tizen::Graphics::Rectangle(600, 20, 100, 60), 2, buffer, false);
            delete color_of_temp;
            pItem->SetElementTextHorizontalAlignment(2, ALIGNMENT_RIGHT);
        }
        if (temp_data->temperature_hi().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
            Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(temp_data->temperature_hi().value());
            pItem->AddElement(Tizen::Graphics::Rectangle(500, 20, 100, 60), 3, buffer, 40, *color_of_temp, *color_of_temp, *color_of_temp);
            pItem->SetElementTextHorizontalAlignment(3, ALIGNMENT_RIGHT);
            delete color_of_temp;
        }
    }
    return pItem;
}

void
meecastFullWeatherForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
    ConfigTizen *config;

	if (status == LIST_ITEM_STATUS_SELECTED){

   	}
}
void
meecastFullWeatherForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
meecastFullWeatherForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state)
{
}

void
meecastFullWeatherForm::OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex)
{
}


