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
#include "meecastLocationManager.h"
#include "meecastManageLocationsForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;
using namespace Tizen::Base::Collection;
using namespace Tizen::Locations;


meecastMainForm::meecastMainForm(void):
                 __pContextMenuText(null),
                 __pAnimation(null),
	             __pAnimationFrameList(null),
                 __updateTimer(null),
	             __pFlickGesture(null),
                 __daysmap(null),
	             __gestureDetected(false),
                 __pLocationManagerThread(null){
}

meecastMainForm::~meecastMainForm(void)
{
    if(__pAnimationFrameList != null){
		__pAnimationFrameList->RemoveAll(true);
		delete __pAnimationFrameList;
		__pAnimationFrameList = null;
	}

    if (__updateTimer)
        delete __updateTimer;

    if (__daysmap)
        __daysmap->RemoveAll(true);

	__pLocationManagerThread->Join();
	delete __pLocationManagerThread;

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
    __daysmap = new Tizen::Base::Collection::HashMap;
    __daysmap->Construct();

    // Setup back event listener
    SetFormBackEventListener(this);
    SetFormMenuEventListener(this);

    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    if (right_label != null)
		right_label->AddTouchEventListener(*this);
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    if (left_label != null)
		left_label->AddTouchEventListener(*this);
    Tizen::Ui::Controls::Label  *background_label = static_cast<Label*>(GetControl(L"IDC_BACKGROUND_LABEL"));
    if (background_label != null){
        background_label->AddTouchEventListener(*this);
    }

    /* Footer */
//    Footer* pFooter = GetFooter();
//    pFooter->SetStyle(FOOTER_STYLE_BUTTON_ICON);
    _config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");

    __updateButton = new Button();
    __updateButton->Construct(Rectangle(0, 0, 100, 100), ""); 
    __updateButton->SetActionId(ID_BUTTON_UPDATE);
    __updateButton->SetNormalBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("refresh_def.png"));
    __updateButton->SetPressedBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("refresh_sel.png"));
    __updateButton->AddActionEventListener(*this);

    Tizen::Ui::Controls::Label  *source_icon_label = static_cast<Label*>(GetControl(L"IDC_LABEL_SOURCE_ICON"));
	source_icon_label->AddTouchEventListener(*this);
    Tizen::Graphics::Point position = source_icon_label->GetPosition();

   // __updateButton->SetPosition(0 + 25, position.y - 60);
    __updateButton->SetPosition(0 + 25, position.y - 0);
    position.SetPosition(position.x + source_icon_label->GetWidth(), position.y + 50);
    CreateContextMenuList(position);
  //  pFooter->AddActionEventListener(*this);

  
	AppResource *pAppResource = Application::GetInstance()->GetAppResource();
	if (pAppResource != null){
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
		__pAnimation->Construct(Rectangle((35), position.y + 5, 80, 80), *__pAnimationFrameList);
		__pAnimation->SetRepeatCount(10000);
		AddControl(*__pAnimation);
        __pAnimation->SetShowState(false);
    }

    AddControl(__updateButton);
    __updateTimer = new (std::nothrow) Tizen::Base::Runtime::Timer;
    TryReturn(__updateTimer != null, E_FAILURE, "[E_FAILURE] Failed to create __updateTimer.");
    AppLog("updateTimer is created.");

    r = __updateTimer->Construct(*this);
    TryReturn(IsFailed(r) != true, r, "[%s] Failed to construct __pTimer", GetErrorMessage(r));
    AppLog("__updateTimer is constructed.");

    Tizen::Ui::Controls::Panel *pTouchArea = static_cast<Panel*>(GetControl(L"IDC_PANEL_TOUCH"));
	if (pTouchArea != null){
	    AddControl(*pTouchArea);
		pTouchArea->AddTouchEventListener(*this);
	}

    Tizen::Ui::Controls::ListView  *main_listview_forecast = static_cast<ListView*>(GetControl(L"IDC_LISTVIEW_FORECASTS"));
    main_listview_forecast->SetItemProvider(*this);
    main_listview_forecast->AddListViewItemEventListener(*this);
	main_listview_forecast->AddTouchEventListener(*this);


    Tizen::Ui::Controls::Label  *main_no_locations_label = static_cast<Label*>(GetControl(L"IDC_LABEL_NO_LOCATIONS"));
    main_no_locations_label->SetText(_("No locations are set up yet."));
    Tizen::Ui::Controls::Button *main_set_locations_button = static_cast<Button*>(GetControl(L"IDC_BUTTON_SET_LOCATIONS"));
    main_set_locations_button->SetText(_("Set locations"));
    Tizen::Ui::Controls::Label  *main_need_updating = static_cast<Label*>(GetControl(L"IDC_LABEL_NEED_UPDATING"));
    main_need_updating->SetText(_("Looks like there's no info for this location."));
    Tizen::Ui::Controls::Button  *main_set_try_update_button = static_cast<Button*>(GetControl(L"IDC_BUTTON_TRY_UPDATE"));
    main_set_try_update_button->SetText(_("Try to update"));

	__pFlickGesture = new (std::nothrow) TouchFlickGestureDetector;
	if (__pFlickGesture != null){
		__pFlickGesture->Construct();
        pTouchArea->AddGestureDetector(*__pFlickGesture);
        main_listview_forecast->AddGestureDetector(*__pFlickGesture);
	    source_icon_label->AddGestureDetector(*__pFlickGesture);
        main_need_updating->AddGestureDetector(*__pFlickGesture);
    	__pFlickGesture->AddFlickGestureEventListener(*this);
	}

	__pLocationManagerThread =  new (std::nothrow) meecastLocationManager();
	r = __pLocationManagerThread->Construct(*this);
	if (IsFailed(r)){
		AppLog("Thread Construct failed.");
		return r;
	}
    return r;
}

result
meecastMainForm::OnTerminating(void){
    result r = E_SUCCESS;
    
	if (__pFlickGesture != null){
		__pFlickGesture->RemoveFlickGestureEventListener(*this);
		delete __pFlickGesture;
	}

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastMainForm::NextStation(){
    if ((uint)(_config->current_station_id() + 1) < _config->stationsList().size())
        _config->current_station_id(_config->current_station_id() + 1);
    else
       _config->current_station_id(0);
    _config->saveConfig();
    ReInitElements(); 
}

void
meecastMainForm::PreviousStation(){
    if ((uint)(_config->current_station_id() - 1) >= 0)
        _config->current_station_id(_config->current_station_id() - 1);
    else
        _config->current_station_id(_config->stationsList().size());
    _config->saveConfig();
    ReInitElements(); 
}

void
meecastMainForm::OnTouchPressed(const Tizen::Ui::Control& source, 
                                const Tizen::Graphics::Point& currentPosition,
		                        const Tizen::Ui::TouchEventInfo& touchInfo) {

	__gestureDetected = false;
    AppLog("Touch Pressed");
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    if (source.Equals(*left_label)){
        PreviousStation();
        AppLog("Left Touch Screen");
    }
    if (source.Equals(*right_label)){
        NextStation();
        AppLog("Right Touch Screen");
	}


}

void
meecastMainForm::OnTouchReleased(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo)
{
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);
    AppLog("OnTouchReleased");
    Tizen::Ui::Controls::Panel *pTouchArea = static_cast<Panel*>(GetControl(L"IDC_PANEL_TOUCH"));
    Tizen::Ui::Controls::Label  *source_icon_label = static_cast<Label*>(GetControl(L"IDC_LABEL_SOURCE_ICON"));
	if (__gestureDetected == false){
        if (source.Equals(*pTouchArea)){
            AppLog("BackGround Touch Screen");
            Tizen::Base::Collection::ArrayList* pList = new (std::nothrow)Tizen::Base::Collection::ArrayList();
		    pList->Construct();
		    pList->Add(*new (std::nothrow) Integer(0));
            pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_FULLWEATHERSCENE"), pList);
	    }
        if (source.Equals(*source_icon_label)){
            if (_config->stationsList().size() > 0){
                AppControlBrowser(_config->stationsList().at(_config->current_station_id())->viewURL().c_str());
                AppLog("Source Touch Screen %s", _config->stationsList().at(_config->current_station_id())->viewURL().c_str());
            }
	    }
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
   // Footer* pFooter = GetFooter();
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
    case ID_MENU_ADD_LOCATION:
        AppLog("Add Location Button is clicked!");
        pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_SOURCESSCENE"));
        break;
    case ID_BUTTON_UPDATE: 
        AppLog("Settings Update is clicked!");
        UpdateWeatherForecast();
        break;
    case ID_SET_LOCATIONS:
        AppLog("Locations is clicked!");
        pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_SOURCESSCENE"));
        break;
    case ID_MENU_ABOUT:
        AppLog("About is clicked!");
        pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_ABOUTSCENE"));
        break;
    case ID_MENU_ADJUST_GPS:{
        AppLog("GpS is clicked!");

        if (_config->Gps()){
			result r = __pLocationManagerThread->Start();

        AppLog("GpS is clicked!1");
			if (IsFailed(r)){
				AppLog("Failed to fetch the current location");
			}
        }

//        UpdateGpsPosition();
        break;
    }    
    default:
        break;
    }
}
void
meecastMainForm::OnTimerExpired(Tizen::Base::Runtime::Timer& timer){
    if (!_config->isupdatingstations()){
        __updateTimer->Cancel();
        __pAnimation->Stop();
        __updateButton->SetShowState(true);
        __pAnimation->SetShowState(false);
        ReInitElements();
    }
}


void
meecastMainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source){
    UiApp* pApp = UiApp::GetInstance();
    AppAssert(pApp);
    pApp->Terminate();
}

void
meecastMainForm::OnFormMenuRequested(Tizen::Ui::Controls::Form& source){
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);
    __pContextMenuText->SetShowState(true);
    __pContextMenuText->Show();

//    pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_SETTINGSSCENE"));
}



void 
meecastMainForm::ReInitElements(void){

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

    if(__pContextMenuText){
        if (_config->Gps())
            __pContextMenuText->InsertItemAt(3, _("Adjust GPS station"), ID_MENU_ADJUST_GPS);
        else{
           __pContextMenuText->RemoveItemAt(__pContextMenuText->GetItemIndexFromActionId(ID_MENU_ADJUST_GPS));
        }
    }

    if (_config->stationsList().size()!=0){
        station_label->SetText(_config->stationname().c_str());
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background_main.png"));
        main_no_locations_label->SetShowState(false);
        main_set_locations_button->SetShowState(false);
        main_set_try_update_button->SetShowState(false);
        main_need_updating->SetShowState(false);
        __updateButton->SetShowState(true);
    }else{
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background.png"));
        station_label->SetText("MeeCast");
        main_no_locations_label->SetShowState(true);
        main_set_locations_button->SetShowState(true);
        main_set_locations_button->AddActionEventListener(*this);
        main_set_locations_button->SetActionId(ID_SET_LOCATIONS);
        backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(0xFF, 0xFF, 0xFF));
        main_set_try_update_button->SetShowState(false);
        main_need_updating->SetShowState(false);
        __updateButton->SetShowState(false);
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
    main_icon->SetDropEnabled(false);
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

        Tizen::Ui::Controls::Label  *pFooter = static_cast<Label*>(GetControl(L"IDC_LABEL_SOURCE_ICON"));
        //Footer* pFooter = GetFooter();
        if (_config->stationsList().at(_config->current_station_id())->sourceName() == "openweathermap.org")
            pFooter->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("openweathermap.org.png"));
        if (_config->stationsList().at(_config->current_station_id())->sourceName() == "gismeteo.ru")
            pFooter->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("gismeteo.ru.png"));
        if (_config->stationsList().at(_config->current_station_id())->sourceName() == "foreca.com")
            pFooter->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("foreca.com.png"));
        if (_config->stationsList().at(_config->current_station_id())->sourceName() == "hko.gov.hk")
            pFooter->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("hko.gov.hk.png"));
         if (_config->stationsList().at(_config->current_station_id())->sourceName() == "yr.no")
            pFooter->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("yr.no.png"));

         pFooter->RequestRedraw();
    }
    else 
        _config->dp = NULL;

     Core::Data *temp_data = NULL;
     AppLog ("DP %p", _config->dp);
     if (__daysmap)
        __daysmap->RemoveAll(true);

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
        Tizen::Base::Utility::StringUtil::Utf8ToString(_(temp_data->Text().c_str()), str);
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
            Tizen::Base::Utility::StringUtil::Utf8ToString(_("Now"), str);
        else
            Tizen::Base::Utility::StringUtil::Utf8ToString(_("Today"), str);
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

            if (str == "CALM" || Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/wind_direction_arrow_" + str + ".png")){
                main_background_wind_icon->SetShowState(true);
                main_wind_icon->SetShowState(true);

                /* Wind direction Icon */ 
                Tizen::Media::Image *image = null;
                Tizen::Graphics::Bitmap* windIconBitmap = null;
                image = new (std::nothrow) Tizen::Media::Image();
                image->Construct();
                
                if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/wind_direction_arrow_" + str + ".png")){
                    windIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"720x1280/wind_direction_arrow_" + str + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
                    main_wind_icon->SetBackgroundBitmap(*windIconBitmap);
                    main_wind_icon->RequestRedraw();
                    SAFE_DELETE(image);
                    SAFE_DELETE(windIconBitmap);
                }
                snprintf (buffer, sizeof(buffer) -1, "%s", _(temp_data->WindDirection().c_str()));
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                main_wind_text->SetShowState(true);
                main_wind_text->SetText(str);
                main_wind_text->RequestRedraw();
            }
        }
        /* Main wind speed */
        if (temp_data->WindSpeed().value() != INT_MAX){
            main_wind_speed_text->SetShowState(true);
            main_wind_speed_icon->SetShowState(true);
            snprintf (buffer, sizeof(buffer) -1, "%0.f %s", 
                                             temp_data->WindSpeed().value(), _(_config->WindSpeedUnit().c_str()));
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            main_wind_speed_text->SetText(str);
            main_wind_speed_text->RequestRedraw();
        }else{
            main_wind_speed_text->SetShowState(false);
            main_wind_speed_icon->SetShowState(false);
        }
        /* Main presssure */
        if (temp_data->pressure().value(true) != INT_MAX){
            snprintf (buffer, sizeof(buffer) -1, "%i %s", (int)temp_data->pressure().value(), _(_config->PressureUnit().c_str()));
            main_pressure_text->SetShowState(true);
            main_pressure_icon->SetShowState(true);
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            main_pressure_text->SetText(str);
            main_pressure_text->RequestRedraw();
        }

    }else{
        /* Main Icon  N/A*/ 
        Tizen::Media::Image *image = null;
        Tizen::Graphics::Bitmap* mainIconBitmap = null;
        image = new (std::nothrow) Tizen::Media::Image();
        image->Construct();

        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() +
                                            L"screen-density-xhigh/icons/Atmos/na.png")){
            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() +
                                            L"screen-density-xhigh/icons/Atmos/na.png", BITMAP_PIXEL_FORMAT_ARGB8888);
            main_icon->SetBackgroundBitmap(*mainIconBitmap);
            main_icon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background.png"));
        main_background_label->RequestRedraw();
    }

    main_listview_forecast->SetItemProvider(*this);
    main_listview_forecast->SetItemDividerColor(Tizen::Graphics::Color(0x1F, 0x1F, 0x1F)); 
    /* Fill list of days with weather forecast */


    time_t current_day;
    struct tm   *tm = NULL;
    current_day = time(NULL);

    /* Set localtimezone */
    struct tm time_tm1;
    struct tm time_tm2;
    gmtime_r(&current_day, &time_tm1);
    localtime_r(&current_day, &time_tm2);
    int localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 



    /* set current day */ 
    current_day = time(NULL);




    tm = gmtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    current_day = mktime(tm) + 3600*localtimezone; /* today 00:00:00 */
    AppLog("Local TimeZone %i", localtimezone);

    /* fill other days */
    int i = 0;
    int j = 0;
    int timezone = 0;
    /* Timezone */
    if (_config->dp != NULL){
        timezone = _config->dp->timezone();
        AppLog("TimeZone %i", timezone);
    }

    _dayCount = 0;
    while  (_config->dp != NULL && i < 3600*24*14) {
        if (_config->dp->data().GetDataForTime( current_day + 15*3600 + i - 3600*timezone)){
            __daysmap->Add(*(new (std::nothrow) Integer(_dayCount)), *(new (std::nothrow) Long(current_day + 15*3600 + i - 3600*timezone)));
            AppLog ("Result1 %li", current_day);
            AppLog ("Result %li", current_day + 15*3600 + i - 3600*timezone);
            _dayCount ++;
        }
        i = i + 3600*24;
    }
    /* Check nil weather data */
    if (_config->stationsList().size()!=0 && _dayCount == 0){
        main_set_try_update_button->SetShowState(true);
        main_need_updating->SetShowState(true);
        main_set_try_update_button->AddActionEventListener(*this);
        main_set_try_update_button->SetActionId(ID_BUTTON_UPDATE);
        backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(0xFF, 0xFF, 0xFF));
    }

    if (_dayCount >0)
        main_icon->SetShowState(true);
    main_listview_forecast->UpdateList();
    backgroundPanel->RequestRedraw();
}

void
meecastMainForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs){

    ReInitElements(); 

    Tizen::Ui::Controls::Label  *source_icon_label = static_cast<Label*>(GetControl(L"IDC_LABEL_SOURCE_ICON"));
    Tizen::Ui::Controls::Button  *main_set_try_update_button = static_cast<Button*>(GetControl(L"IDC_BUTTON_TRY_UPDATE"));
    Tizen::Graphics::Point position = source_icon_label->GetPosition();
    __updateButton->SetPosition(0 + 25, position.y - 0);
    __updateButton->RequestRedraw();
    __pAnimation->SetPosition(0 + 35, position.y + 10);
    __pAnimation->RequestRedraw();
    
    if(_config->Gps() && main_set_try_update_button->GetShowState()){
        UpdateWeatherForecast();
    }
}

void
meecastMainForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId){
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}

void
meecastMainForm::CreateContextMenuList(Tizen::Graphics::Point Corner_Point){
    __pContextMenuText = new (std::nothrow) ContextMenu();
    __pContextMenuText->Construct(Corner_Point, CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
    __pContextMenuText->AddItem(_("Add Station"), ID_MENU_ADD_LOCATION);
    __pContextMenuText->AddItem(_("Settings"), ID_MENU_SETTINGS);
    __pContextMenuText->AddItem(_("Update"), ID_BUTTON_UPDATE);
    __pContextMenuText->AddItem(_("About"), ID_MENU_ABOUT);
    __pContextMenuText->AddActionEventListener(*this);
    __pContextMenuText->SetMaxVisibleItemsCount(5);
}


int
meecastMainForm::GetItemCount(void){
    int itemCount = 0;
    if (__daysmap){
        IList* pList = __daysmap->GetKeysN();
        AppAssert(pList);
        itemCount = pList->GetCount();
        delete pList;
    }
    return itemCount;
}



bool
meecastMainForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth){
	delete pItem;
	return true;
}


Tizen::Ui::Controls::ListItemBase*
meecastMainForm::CreateItem (int index, int itemWidth){
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
    String* pStr;
    Core::Data *temp_data = NULL;

    Long* pLong = static_cast< Long* >(__daysmap->GetValue(Integer(index)));

    if ((temp_data = _config->dp->data().GetDataForTime( (time_t)pLong->ToLong()))) {
        temp_data->temperature_low().units(_config->TemperatureUnit());
        temp_data->temperature_hi().units(_config->TemperatureUnit());
        temp_data->temperature().units(_config->TemperatureUnit());

        pItem->AddElement(Tizen::Graphics::Rectangle(10, 24, 220, 50), 0, temp_data->DayOfMonthName().c_str(), 38,
                Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)), Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)),
                Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)));
        if (index != 0)
            pItem->AddElement(Tizen::Graphics::Rectangle(70, 20, 220, 50), 4, temp_data->ShortDayName().c_str(), false);
        else
            pItem->AddElement(Tizen::Graphics::Rectangle(70, 20, 220, 50), 4, _("Today"), false);
        /* Icon */
        snprintf(buffer, sizeof(buffer) - 1, "icons/Atmos/%i.png", temp_data->Icon());
        pItem->AddElement(Tizen::Graphics::Rectangle(320, 0, 100, 100), 1, *Application::GetInstance()->GetAppResource()->GetBitmapN(buffer), null, null);

        if (temp_data->temperature_low().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
            pItem->AddElement(Tizen::Graphics::Rectangle(600, 20, 100, 60), 2, buffer, 40, Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)), Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)),
                Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)));
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

    AppLog("LIST_ITEM_STATUS_SELECTED init");
	if (status == LIST_ITEM_STATUS_SELECTED){
        SceneManager* pSceneManager = SceneManager::GetInstance();
        AppAssert(pSceneManager);
        AppLog("LIST_ITEM_STATUS_SELECTED");
        Tizen::Base::Collection::ArrayList* pList = new (std::nothrow)Tizen::Base::Collection::ArrayList();
		pList->Construct();
        pList->Add(*new (std::nothrow) Integer(index));
        pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_FULLWEATHERSCENE"), pList);
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


void
meecastMainForm::OnFlickGestureDetected(TouchFlickGestureDetector& gestureDetector)
{
    AppLog("Flick detected!");
	Rectangle rc(0, 0, 0, 0);
	Point point(0, 0);


    /*
    Tizen::Ui::Controls::Panel 
        *pTouchArea = static_cast<Panel*>(GetControl(L"IDC_PANEL_TOUCH"));
    Tizen::Ui::Controls::ListView  
        *main_listview_forecast = static_cast<ListView*>(GetControl(L"IDC_LISTVIEW_FORECASTS"));
	if (pTouchArea != null) {
		rc = pTouchArea->GetBounds();

		Touch touch;
		point = touch.GetPosition(*pTouchArea);
	}

	if (point.y < 0 || point.y > rc.height)
	{
        AppLog("Problem with Flick");
		    return;
	}
*/
//	pTouchArea->Invalidate(false);

	FlickDirection direction = gestureDetector.GetDirection();
	switch(direction){
        case FLICK_DIRECTION_RIGHT:
            PreviousStation();
            AppLog("Flick detected RIGHT");
            break;
        case FLICK_DIRECTION_LEFT:
            NextStation();
            AppLog("Flick detected LEFT");
            break;
        case FLICK_DIRECTION_UP:
            AppLog("Flick detected UP");
            break;
        case FLICK_DIRECTION_DOWN:
            AppLog("Flick detected DOWN");
            break;
        case FLICK_DIRECTION_NONE:
        default:
            AppLog("Flick detected NONE");
            break;
	}
    AppLog("Flick detected");
	__gestureDetected = true;
 //   Invalidate(false);
}

void
meecastMainForm::OnFlickGestureCanceled(TouchFlickGestureDetector& gestureDetector)
{
    AppLog("Flick canceled!");
}


void
meecastMainForm::AppControlBrowser(Tizen::Base::String uri){
    AppControl* pAc = AppManager::FindAppControlN(L"tizen.internet",
                                                     L"http://tizen.org/appcontrol/operation/view");
    if (pAc){
        pAc->Start(&uri, null, null, null);
        delete pAc;
    }
}

void
meecastMainForm::UpdateWeatherForecast(){
    if (_config->stationsList().size() > 0){
        Tizen::Ui::Controls::Label  *main_need_updating = static_cast<Label*>(GetControl(L"IDC_LABEL_NEED_UPDATING"));
        Tizen::Ui::Controls::Button  *main_set_try_update_button = static_cast<Button*>(GetControl(L"IDC_BUTTON_TRY_UPDATE"));
        main_set_try_update_button->SetShowState(false);
        main_need_updating->SetShowState(false);
        __pAnimation->SetShowState(true);
        __updateButton->SetShowState(false);
        __pAnimation->Play();
        __updateTimer->StartAsRepeatable(1000);
        _config->updatestations();
    }

}
void
meecastMainForm::UpdateGpsPosition(){
    AppLog("Gps is changed ");
        result lastResult = E_SUCCESS;
        LocationCriteria locCriteria;

        locCriteria.SetAccuracy(LOC_ACCURACY_ANY);

        Location location = LocationProvider::GetLocation(locCriteria);

        lastResult = GetLastResult();

        if (lastResult == E_USER_NOT_CONSENTED){
            int doModal;
            MessageBox messageBox;
            messageBox.Construct(L"Error", "The user has disabled the required settings.", MSGBOX_STYLE_OK, 0);
            messageBox.ShowAndWait(doModal);
        }else{
            AppLog("Yes!!!");
            AppLog ("Latitude %d",location.GetCoordinates().GetLatitude());
            AppLog ("Longitude %d",location.GetCoordinates().GetLongitude());
            String dbPath;
            dbPath.Append(App::GetInstance()->GetAppResourcePath());
            dbPath.Append("db/openweathermap.org.db");
            if (Database::Exists(dbPath) == true){
                Core::DatabaseSqlite *__db;
                __db = new Core::DatabaseSqlite(dbPath);
                if (__db->open_database() == true){
                    std::string country,  region, code, name;
                    double latitude, longitude;
                    if ((Double::ToString(location.GetCoordinates().GetLatitude())== "NaN") || 
                        (Double::ToString(location.GetCoordinates().GetLongitude()) == "NaN")){
                        int doModal;
                        MessageBox messageBox;
                        messageBox.Construct(L"Error", "Data for GPS is not available", MSGBOX_STYLE_OK, 0);
                        messageBox.ShowAndWait(doModal);
                    }else{ 
                        __db->get_nearest_station(location.GetCoordinates().GetLatitude(), location.GetCoordinates().GetLongitude(), country, region, code,  name, latitude, longitude);
                        if (latitude != INT_MAX && longitude != INT_MAX){
                            /* find exist gps station */
                            int index = _config->getGpsStation();
                            if (index > -1){
                                /* delete gps station */
                                _config->removeStation(index);
                            }
                            String Name;
                            Name = name.c_str();
                            Name.Append(" (GPS)");
                            _config->saveStation1("openweathermap.org", String(code.c_str()), Name, String(country.c_str()), String(region.c_str()), true, latitude, longitude);
                        }
                    }
                }
                delete __db;
            }
        }
        ReInitElements(); 
}

void
meecastMainForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs){
	if (requestId == LOC_MGR_DRAW_SYNC_LOC_UPDATE)
	{
        ReInitElements(); 
	}else if(requestId == LOC_MGR_NOTIFY_ERROR){
		bool isSettingEnabled = CheckLocationSetting();
		if (!isSettingEnabled){
			LaunchLocationSettings();
		}
		else{
            int doModal;
            MessageBox messageBox;
            messageBox.Construct(_("Privacy protection"), _("Allow the MeeCast to use your location. You can change settings at Settings->Privacy."), MSGBOX_STYLE_OK, 0);
            messageBox.ShowAndWait(doModal);
		}
	}

	if(pArgs)
	{
		pArgs->RemoveAll(true);
		delete pArgs;
	}
}

bool
meecastMainForm::CheckLocationSetting(void){
	bool hasPrivilege = false;
	bool gpsEnabled = true;
	bool wpsEnabled = true;

	result gps = Tizen::System::SettingInfo::GetValue(L"http://tizen.org/setting/location.gps", gpsEnabled);
	result wps = Tizen::System::SettingInfo::GetValue(L"http://tizen.org/setting/location.wps", wpsEnabled);

	hasPrivilege = gpsEnabled | wpsEnabled;
	if (gps != E_SUCCESS || wps != E_SUCCESS || hasPrivilege == false)
	{
		return false;
	}

	return true;
}

void
meecastMainForm::LaunchLocationSettings(void)
{
	int res;

	MessageBox messageBox;
	messageBox.Construct(L"Information", L"Location services are disabled. Enable them in location settings?", MSGBOX_STYLE_YESNO);
	messageBox.ShowAndWait(res);

	if (res == MSGBOX_RESULT_YES)
	{
		HashMap extraData;
		extraData.Construct();
		String categoryKey = L"category";
		String categoryVal = L"Location";
		extraData.Add(&categoryKey, &categoryVal);

		AppControl* pAc = AppManager::FindAppControlN(L"tizen.settings", L"http://tizen.org/appcontrol/operation/configure");

		if (pAc)
		{
			pAc->Start(null, null, &extraData, this);
			delete pAc;
		}
	}
	return;
}

