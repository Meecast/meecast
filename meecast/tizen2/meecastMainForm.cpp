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
                 __pContextMenuText(null),
                 __pAnimation(null),
	             __pAnimationFrameList(null),
                 __updateTimer(null)
{
}

meecastMainForm::~meecastMainForm(void)
{
    if(__pAnimationFrameList != null){
		__pAnimationFrameList->RemoveAll(true);
		delete __pAnimationFrameList;
		__pAnimationFrameList = null;
	}
    delete __updateButton;
    __updateButton = null;
    delete __updateTimer;
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

    // Setup back event listener
    SetFormBackEventListener(this);

    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    if (right_label != null)
		right_label->AddTouchEventListener(*this);
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    if (left_label != null)
		left_label->AddTouchEventListener(*this);

    /* Footer */
    Footer* pFooter = GetFooter();
    pFooter->SetStyle(FOOTER_STYLE_BUTTON_ICON);

    FooterItem menuButton;
    menuButton.Construct(ID_BUTTON_MENU);
    menuButton.SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("icon_more_ef.png"));

    FooterItem fake1menuButton;
    fake1menuButton.Construct(10000);
    fake1menuButton.SetBackgroundBitmap(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("null.png"));
    FooterItem fake2menuButton;
    fake2menuButton.Construct(100011);
    fake2menuButton.SetBackgroundBitmap(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("null.png"));
    FooterItem fake3menuButton;
    fake3menuButton.Construct(1000033);
    fake3menuButton.SetBackgroundBitmap(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("null.png"));



    __updateButton = new Tizen::Ui::Controls::FooterItem(); 
    __updateButton->Construct(ID_BUTTON_UPDATE);
    __updateButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("refresh_def.png"));
    __updateButton->SetIcon(FOOTER_ITEM_STATUS_PRESSED, Application::GetInstance()->GetAppResource()->GetBitmapN("refresh_sel.png"));

    pFooter->AddItem(*__updateButton);
    pFooter->AddItem(fake1menuButton);
    pFooter->AddItem(fake2menuButton);
    pFooter->AddItem(fake3menuButton);
    pFooter->AddItem(menuButton);
    Tizen::Graphics::Point position = pFooter->GetPosition();
    position.SetPosition(position.x + pFooter->GetWidth(), position.y);
    CreateContextMenuList(position);
    pFooter->AddActionEventListener(*this);

    _config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");




	AppResource *pAppResource = Application::GetInstance()->GetAppResource();
	if (pAppResource != null)
	{
		Bitmap *pBitmap0 = pAppResource->GetBitmapN("animations/00_list_process_01.png");
		Bitmap *pBitmap1 = pAppResource->GetBitmapN("animations/00_list_process_02.png");
		Bitmap *pBitmap2 = pAppResource->GetBitmapN("animations/00_list_process_03.png");
		Bitmap *pBitmap3 = pAppResource->GetBitmapN("animations/00_list_process_04.png");
		Bitmap *pBitmap4 = pAppResource->GetBitmapN("animations/00_list_process_05.png");
		Bitmap *pBitmap5 = pAppResource->GetBitmapN("animations/00_list_process_06.png");
		Bitmap *pBitmap6 = pAppResource->GetBitmapN("animations/00_list_process_07.png");
		Bitmap *pBitmap7 = pAppResource->GetBitmapN("animations/00_list_process_08.png");
		Bitmap *pBitmap8 = pAppResource->GetBitmapN("animations/00_list_process_09.png");
		Bitmap *pBitmap9 = pAppResource->GetBitmapN("animations/00_list_process_10.png");
		Bitmap *pBitmap10 = pAppResource->GetBitmapN("animations/00_list_process_11.png");
		Bitmap *pBitmap11 = pAppResource->GetBitmapN("animations/00_list_process_12.png");
		Bitmap *pBitmap12 = pAppResource->GetBitmapN("animations/00_list_process_13.png");
		Bitmap *pBitmap13 = pAppResource->GetBitmapN("animations/00_list_process_14.png");
		Bitmap *pBitmap14 = pAppResource->GetBitmapN("animations/00_list_process_15.png");
		Bitmap *pBitmap15 = pAppResource->GetBitmapN("animations/00_list_process_16.png");
		Bitmap *pBitmap16 = pAppResource->GetBitmapN("animations/00_list_process_17.png");
		Bitmap *pBitmap17 = pAppResource->GetBitmapN("animations/00_list_process_18.png");
		Bitmap *pBitmap18 = pAppResource->GetBitmapN("animations/00_list_process_19.png");
		Bitmap *pBitmap19 = pAppResource->GetBitmapN("animations/00_list_process_20.png");
		Bitmap *pBitmap20 = pAppResource->GetBitmapN("animations/00_list_process_21.png");
		Bitmap *pBitmap21 = pAppResource->GetBitmapN("animations/00_list_process_22.png");
		Bitmap *pBitmap22 = pAppResource->GetBitmapN("animations/00_list_process_23.png");
		Bitmap *pBitmap23 = pAppResource->GetBitmapN("animations/00_list_process_24.png");
		Bitmap *pBitmap24 = pAppResource->GetBitmapN("animations/00_list_process_25.png");
		Bitmap *pBitmap25 = pAppResource->GetBitmapN("animations/00_list_process_26.png");
		Bitmap *pBitmap26 = pAppResource->GetBitmapN("animations/00_list_process_27.png");
		Bitmap *pBitmap27 = pAppResource->GetBitmapN("animations/00_list_process_28.png");
		Bitmap *pBitmap28 = pAppResource->GetBitmapN("animations/00_list_process_29.png");
		Bitmap *pBitmap29 = pAppResource->GetBitmapN("animations/00_list_process_30.png");

		// Create AnimationFrames
		long duration = DURATION / PROGRESS_COUNT;
		AnimationFrame *pAniFrame1 = new (std::nothrow) AnimationFrame(*pBitmap0, duration);
		AnimationFrame *pAniFrame2 = new (std::nothrow) AnimationFrame(*pBitmap1, duration);
		AnimationFrame *pAniFrame3 = new (std::nothrow) AnimationFrame(*pBitmap2, duration);
		AnimationFrame *pAniFrame4 = new (std::nothrow) AnimationFrame(*pBitmap3, duration);
		AnimationFrame *pAniFrame5 = new (std::nothrow) AnimationFrame(*pBitmap4, duration);
		AnimationFrame *pAniFrame6 = new (std::nothrow) AnimationFrame(*pBitmap5, duration);
		AnimationFrame *pAniFrame7 = new (std::nothrow) AnimationFrame(*pBitmap6, duration);
		AnimationFrame *pAniFrame8 = new (std::nothrow) AnimationFrame(*pBitmap7, duration);
		AnimationFrame *pAniFrame9 = new (std::nothrow) AnimationFrame(*pBitmap8, duration);
		AnimationFrame *pAniFrame10 = new (std::nothrow) AnimationFrame(*pBitmap9, duration);
		AnimationFrame *pAniFrame11 = new (std::nothrow) AnimationFrame(*pBitmap10, duration);
		AnimationFrame *pAniFrame12 = new (std::nothrow) AnimationFrame(*pBitmap11, duration);
		AnimationFrame *pAniFrame13 = new (std::nothrow) AnimationFrame(*pBitmap12, duration);
		AnimationFrame *pAniFrame14 = new (std::nothrow) AnimationFrame(*pBitmap13, duration);
		AnimationFrame *pAniFrame15 = new (std::nothrow) AnimationFrame(*pBitmap14, duration);
		AnimationFrame *pAniFrame16 = new (std::nothrow) AnimationFrame(*pBitmap15, duration);
		AnimationFrame *pAniFrame17 = new (std::nothrow) AnimationFrame(*pBitmap16, duration);
		AnimationFrame *pAniFrame18 = new (std::nothrow) AnimationFrame(*pBitmap17, duration);
		AnimationFrame *pAniFrame19 = new (std::nothrow) AnimationFrame(*pBitmap18, duration);
		AnimationFrame *pAniFrame20 = new (std::nothrow) AnimationFrame(*pBitmap19, duration);
		AnimationFrame *pAniFrame21 = new (std::nothrow) AnimationFrame(*pBitmap20, duration);
		AnimationFrame *pAniFrame22 = new (std::nothrow) AnimationFrame(*pBitmap21, duration);
		AnimationFrame *pAniFrame23 = new (std::nothrow) AnimationFrame(*pBitmap22, duration);
		AnimationFrame *pAniFrame24 = new (std::nothrow) AnimationFrame(*pBitmap23, duration);
		AnimationFrame *pAniFrame25 = new (std::nothrow) AnimationFrame(*pBitmap24, duration);
		AnimationFrame *pAniFrame26 = new (std::nothrow) AnimationFrame(*pBitmap25, duration);
		AnimationFrame *pAniFrame27 = new (std::nothrow) AnimationFrame(*pBitmap26, duration);
		AnimationFrame *pAniFrame28 = new (std::nothrow) AnimationFrame(*pBitmap27, duration);
		AnimationFrame *pAniFrame29 = new (std::nothrow) AnimationFrame(*pBitmap28, duration);
		AnimationFrame *pAniFrame30 = new (std::nothrow) AnimationFrame(*pBitmap29, duration);

		// Create AnimationList
		__pAnimationFrameList = new (std::nothrow)Tizen::Base::Collection::ArrayList();
		__pAnimationFrameList->Construct();
		__pAnimationFrameList->Add(*pAniFrame1);
		__pAnimationFrameList->Add(*pAniFrame2);
		__pAnimationFrameList->Add(*pAniFrame3);
		__pAnimationFrameList->Add(*pAniFrame4);
		__pAnimationFrameList->Add(*pAniFrame5);
		__pAnimationFrameList->Add(*pAniFrame6);
		__pAnimationFrameList->Add(*pAniFrame7);
		__pAnimationFrameList->Add(*pAniFrame8);
		__pAnimationFrameList->Add(*pAniFrame9);
		__pAnimationFrameList->Add(*pAniFrame10);
		__pAnimationFrameList->Add(*pAniFrame11);
		__pAnimationFrameList->Add(*pAniFrame12);
		__pAnimationFrameList->Add(*pAniFrame13);
		__pAnimationFrameList->Add(*pAniFrame14);
		__pAnimationFrameList->Add(*pAniFrame15);
		__pAnimationFrameList->Add(*pAniFrame16);
		__pAnimationFrameList->Add(*pAniFrame17);
		__pAnimationFrameList->Add(*pAniFrame18);
		__pAnimationFrameList->Add(*pAniFrame19);
		__pAnimationFrameList->Add(*pAniFrame20);
		__pAnimationFrameList->Add(*pAniFrame21);
		__pAnimationFrameList->Add(*pAniFrame22);
		__pAnimationFrameList->Add(*pAniFrame23);
		__pAnimationFrameList->Add(*pAniFrame24);
		__pAnimationFrameList->Add(*pAniFrame25);
		__pAnimationFrameList->Add(*pAniFrame26);
		__pAnimationFrameList->Add(*pAniFrame27);
		__pAnimationFrameList->Add(*pAniFrame28);
		__pAnimationFrameList->Add(*pAniFrame29);
		__pAnimationFrameList->Add(*pAniFrame30);

		// Deallocate a Bitmap.
		delete pBitmap0;
		delete pBitmap1;
		delete pBitmap2;
		delete pBitmap3;
		delete pBitmap4;
		delete pBitmap5;
		delete pBitmap6;
		delete pBitmap7;
		delete pBitmap8;
		delete pBitmap9;
		delete pBitmap10;
		delete pBitmap11;
		delete pBitmap12;
		delete pBitmap13;
		delete pBitmap14;
		delete pBitmap15;
		delete pBitmap16;
		delete pBitmap17;
		delete pBitmap18;
		delete pBitmap19;
		delete pBitmap20;
		delete pBitmap21;
		delete pBitmap22;
		delete pBitmap23;
		delete pBitmap24;
		delete pBitmap25;
		delete pBitmap26;
		delete pBitmap27;
		delete pBitmap28;
		delete pBitmap29;

		// Create Animation
		__pAnimation = new (std::nothrow) Animation();
		__pAnimation->Construct(Rectangle((50), (1140), 60, 60), *__pAnimationFrameList);
		__pAnimation->SetRepeatCount(10000);
		AddControl(*__pAnimation);
       }
    __updateTimer = new (std::nothrow) Tizen::Base::Runtime::Timer;
    TryReturn(__updateTimer != null, E_FAILURE, "[E_FAILURE] Failed to create __updateTimer.");
    AppLog("SampleServiceApp : updateTimer is created.");

    r = __updateTimer->Construct(*this);
    TryReturn(IsFailed(r) != true, r, "[%s] Failed to construct __pTimer", GetErrorMessage(r));
    AppLog("SampleServiceApp : __updateTimer is constructed.");

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
    Footer* pFooter = GetFooter();
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
        if (_config->stationsList().size() > 0){
            pFooter->RemoveItemAt(0);
            __pAnimation->Play();
	        __updateTimer->StartAsRepeatable(1000);
            _config->updatestations();
        }

        break;
    default:
        break;
    }
}
void
meecastMainForm::OnTimerExpired(Tizen::Base::Runtime::Timer& timer)
{
    if (!_config->isupdatingstations()){
        __updateTimer->Cancel();
        __pAnimation->Stop();
        Footer* pFooter = GetFooter();
        pFooter->InsertItemAt(0, *__updateButton);
        ReInitElements();
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
    Tizen::Ui::Controls::Label  *main_background_label = static_cast<Label*>(GetControl(L"IDC_BACKGROUND_LABEL"));
    Tizen::Ui::Controls::Label  *main_no_locations_label = static_cast<Label*>(GetControl(L"IDC_LABEL_NO_LOCATIONS"));
    Tizen::Ui::Controls::Button  *main_set_locations_button = static_cast<Button*>(GetControl(L"IDC_BUTTON_SET_LOCATIONS"));
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
        main_no_locations_label->SetShowState(false);
        main_set_locations_button->SetShowState(false);
    }else{
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background.png"));
        station_label->SetText("MeeCast");
        main_no_locations_label->SetShowState(true);
        main_set_locations_button->SetShowState(true);
        backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(0xFF, 0xFF, 0xFF));
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

        /* Footer */
        Footer* pFooter = GetFooter();
        if (_config->stationsList().at(_config->current_station_id())->sourceName() == "openweathermap.org")
            pFooter->SetBackgroundBitmap(Application::GetInstance()->GetAppResource()->GetBitmapN("openweathermap.org.png"));
        if (_config->stationsList().at(_config->current_station_id())->sourceName() == "gismeteo.ru")
            pFooter->SetBackgroundBitmap(Application::GetInstance()->GetAppResource()->GetBitmapN("gismeteo.ru.png"));

         pFooter->RequestRedraw();
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
        Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(t);
        backgroundPanel->SetBackgroundColor(*color_of_temp);
        delete color_of_temp;
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
    while  ( (temp_data = _config->dp->data().GetDataForTime( current_day + 14 * 3600  + i))) {
        _dayCount ++;
        i = i + 3600*24;
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
        _dayCount = 0;
    }

   main_listview_forecast->UpdateList();
   //main_listview_forecast->RequestRedraw();
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
meecastMainForm::CreateContextMenuList(Tizen::Graphics::Point Corner_Point)
{
    __pContextMenuText = new (std::nothrow) ContextMenu();
    __pContextMenuText->Construct(Corner_Point, CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
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
        pItem->AddElement(Tizen::Graphics::Rectangle(90, 20, 220, 50), 4, temp_data->ShortDayName().c_str(), false);
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


