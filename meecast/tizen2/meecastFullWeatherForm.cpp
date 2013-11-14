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
#include <time.h>

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;
using namespace Tizen::Locales;
using namespace Tizen::Uix::Speech;
using namespace Tizen::System;

meecastFullWeatherForm::meecastFullWeatherForm(void):
                                  _speaking(false),
                                  __pContextMenuText(null),
                                  __nowButton(null),
                                  __dayButton(null),
                                  __nightButton(null),
                                  __hourlyButton(null),
                                  _current_selected_tab(NOW),
                                  _pKeyList(null),
                                  _pValueList(null),
                                  __pFlickGesture(null),
                                  __gestureDetected(false),
                                  __pTts(null),
                                  __updateTimer()
                                  {
    _dayNumber = 0;
    result r = E_SUCCESS;
    __updateTimer = new (std::nothrow) Tizen::Base::Runtime::Timer;
    if (__updateTimer){
        AppLog("updateTimer is created.");
        r = __updateTimer->Construct(*this);
    }
    AppLog("Initializea meecastFullWeatherForm");
    __pTts = new Tizen::Uix::Speech::TextToSpeech();
	if (__pTts != null){
		r = __pTts->Construct(*this);
		TryLog(!IsFailed(r), "[%s] Construct() error", GetErrorMessage(r));
		r = __pTts->Initialize();
		TryLog(!IsFailed(r), "[%s] Initialize() error", GetErrorMessage(r));
        AppLog("Initializea meecastFullWeatherForm2");
	}

}

meecastFullWeatherForm::~meecastFullWeatherForm(void)
{
    SAFE_DELETE(__nowButton);
    SAFE_DELETE(__dayButton);
    SAFE_DELETE(__nightButton);
    SAFE_DELETE(__hourlyButton);
    if (_pKeyList->GetCount() > 0){
        _pKeyList->RemoveAll(true);
    }
    if (__updateTimer)
        delete __updateTimer;
    delete _pKeyList;
    if (_pValueList->GetCount() > 0){
        _pValueList->RemoveAll(true);
    }
    delete _pValueList;
    if (__pContextMenuText)
        delete __pContextMenuText;
    if(__pTts)
        delete __pTts;
}

bool
meecastFullWeatherForm::Initialize(void){
    Construct(L"FULL_WEATHER_FORM");
    return true;
}

result
meecastFullWeatherForm::OnInitializing(void){


    result r = E_SUCCESS;
    /* Footer */
    Footer* pFooter = GetFooter();
    pFooter->SetStyle(FOOTER_STYLE_SEGMENTED_ICON_TEXT);
    pFooter->AddActionEventListener(*this);
    pFooter->SetBackButton();

    // TODO:
    // Add your initialization code here

    // Setup back event listener
    SetFormBackEventListener(this);
    SetFormMenuEventListener(this);

    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    if (right_label != null)
        right_label->AddTouchEventListener(*this);
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    if (left_label != null)
        left_label->AddTouchEventListener(*this);

    _config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");
    Rectangle clientRect;
    Form *pForm = static_cast<Form*>(GetParent());
    AppAssert(pForm);
    RelativeLayout* pLayout = dynamic_cast< RelativeLayout* >(this->GetLayoutN());
    AppAssert(pLayout);
    clientRect = pForm->GetClientAreaBounds();
    __clientWidth = clientRect.width;
    __clientHeight = clientRect.height;
    SetBounds(Rectangle(0, 0, clientRect.width, clientRect.height));

    __pTableView = new (std::nothrow) TableView();
    AppLogExceptionIf(__pTableView == null, "Table view creation is failed");

    Tizen::Ui::Controls::Label  *main_background_label = static_cast<Label*>(GetControl(L"IDC_BACKGROUND_LABEL"));
    __pTableView->Construct(Rectangle(0, 0, clientRect.width, clientRect.height - main_background_label->GetHeight() - pFooter->GetHeight() - INDICATE_HEIGHT), true, TABLE_VIEW_SCROLL_BAR_STYLE_FADE_OUT);
    __pTableView->SetItemProvider(this);
    __pTableView->AddTableViewItemEventListener(*this);

    __pTableView->SetBackgroundColor(Tizen::Graphics::Color(0x00, 0x00, 0x00));

    AddControl(*__pTableView);
    pLayout->SetHorizontalFitPolicy(*__pTableView, FIT_POLICY_PARENT);
    pLayout->SetRelation(*__pTableView, *this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
    pLayout->SetRelation(*__pTableView, *this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
    pLayout->SetRelation(*__pTableView, *main_background_label, RECT_EDGE_RELATION_TOP_TO_BOTTOM);
    pLayout->SetRelation(*__pTableView, *pFooter, RECT_EDGE_RELATION_BOTTOM_TO_TOP);

    _pValueList = new (std::nothrow) Tizen::Base::Collection::ArrayList();
    _pValueList->Construct();
    _pKeyList = new (std::nothrow) Tizen::Base::Collection::ArrayList();
    _pKeyList->Construct();
    Tizen::Ui::Controls::Panel *pTouchArea = static_cast<Panel*>(GetControl(L"IDC_PANEL_TOUCH"));
    if (pTouchArea != null){
        AddControl(*pTouchArea);
        pTouchArea->AddTouchEventListener(*this);
    }
    __pFlickGesture = new (std::nothrow) TouchFlickGestureDetector;
    if (__pFlickGesture != null){
        __pFlickGesture->Construct();
        pTouchArea->AddGestureDetector(*__pFlickGesture);
        __pTableView->AddGestureDetector(*__pFlickGesture);
//	    source_icon_label->AddGestureDetector(*__pFlickGesture);
//        main_need_updating->AddGestureDetector(*__pFlickGesture);
        __pFlickGesture->AddFlickGestureEventListener(*this);
    }

    Tizen::Graphics::Point position = pFooter->GetPosition();
    position.SetPosition(position.x + pFooter->GetWidth(), position.y + 10);
    CreateContextMenuList(position);


    return r;
}

void
meecastFullWeatherForm::CreateContextMenuList(Tizen::Graphics::Point Corner_Point){


	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
    __pContextMenuText = new (std::nothrow) ContextMenu();
    //__pContextMenuText->Construct(Corner_Point, CONTEXT_MENU_STYLE_LIST);
    __pContextMenuText->Construct(Corner_Point, CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);

	Bitmap* pNormalBitmap0 = pAppResource->GetBitmapN("null.png");
    __pContextMenuText->AddItem(_("Settings"), ID_MENU_SETTINGS, *pNormalBitmap0, pNormalBitmap0);
	Bitmap* pNormalBitmap1; 
    if (_config->SpeechFullWindow())
	    pNormalBitmap1 = pAppResource->GetBitmapN("din.png");
    else
	    pNormalBitmap1 = pAppResource->GetBitmapN("din_off.png");
    __pContextMenuText->AddItem(_("Speak weather forecast"), ID_MENU_SPEAK, *pNormalBitmap1, pNormalBitmap1);
    __pContextMenuText->AddItem(_("About"), ID_MENU_ABOUT, *pNormalBitmap0, pNormalBitmap0);
    delete pNormalBitmap0;
    delete pNormalBitmap1;
	__pContextMenuText->SetFocusable(true);
    __pContextMenuText->AddActionEventListener(*this);
    __pContextMenuText->SetMaxVisibleItemsCount(5);
}

result
meecastFullWeatherForm::OnTerminating(void){
    result r = E_SUCCESS;
	if (__pFlickGesture != null){
		__pFlickGesture->RemoveFlickGestureEventListener(*this);
		delete __pFlickGesture;
	}
    return r;
}

void
meecastFullWeatherForm::OnTouchPressed(const Tizen::Ui::Control& source, 
                                const Tizen::Graphics::Point& currentPosition,
		                        const Tizen::Ui::TouchEventInfo& touchInfo) {

	__gestureDetected = false;
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
	if (source.Equals(*left_label)){
        if (_dayNumber>0)
            _dayNumber--;
        ReInitElements(); 
        AppLog("Left Touch Screen");
	}
    if (source.Equals(*right_label)){
        _dayNumber++;
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

    AppResource* pAppResource = Application::GetInstance()->GetAppResource();
    AppLog("Check Action");
    switch(actionId){
        case ID_BUTTON_NOW:
            _current_selected_tab = NOW; 
            ReInitElements(); 
            break;
        case ID_BUTTON_DAY:
            _current_selected_tab = DAY; 
            ReInitElements(); 
            break;
        case ID_BUTTON_NIGHT:
            _current_selected_tab = NIGHT; 
            ReInitElements(); 
            break;
        case ID_BUTTON_HOURLY:
            _current_selected_tab = HOURLY; 
            ReInitElements(); 
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
        case ID_MENU_ABOUT:
            AppLog("About is clicked!");
            pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_ABOUTSCENE"));
            break;
        case ID_MENU_SPEAK:
            AppLog("Speak is clicked!");
            Bitmap* pNormalBitmap1; 
            __pContextMenuText->RemoveItemAt(1);
            if (_config->SpeechFullWindow()){
                _config->SpeechFullWindow(false);
                pNormalBitmap1 = pAppResource->GetBitmapN("din_off.png");
            }else{
                _config->SpeechFullWindow(true);
                pNormalBitmap1 = pAppResource->GetBitmapN("din.png");
            }
            _config->saveConfig();
            __pContextMenuText->InsertItemAt(1, _("Speak weather forecast"), ID_MENU_SPEAK, *pNormalBitmap1, pNormalBitmap1);
            delete pNormalBitmap1;

            __pContextMenuText->SetShowState(true);
            __pContextMenuText->Show();

            break;

        default:
        break;
    }
}

void
meecastFullWeatherForm::OnFormMenuRequested(Tizen::Ui::Controls::Form& source){
    AppLog("OnFormMenuRequested");
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);
    __pContextMenuText->SetShowState(true);
    __pContextMenuText->Show();

//    pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_SETTINGSSCENE"));
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
    int localtimezone = 0;
    int timezone = 0;

    result r = E_FAILURE;

    Tizen::Ui::Controls::Label  *day_name_label = static_cast<Label*>(GetControl(L"IDC_LABEL_DAY_NAME"));
    Tizen::Ui::Controls::Label  *main_background_label = static_cast<Label*>(GetControl(L"IDC_BACKGROUND_LABEL"));
    Tizen::Ui::Controls::Label  *left_label = static_cast<Label*>(GetControl(L"IDC_LABEL_LEFT_BUTTON"));
    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));
    Tizen::Ui::Controls::Label  *main_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_MAIN_ICON"));
    Tizen::Ui::Controls::Label  *main_description = static_cast<Label*>(GetControl(L"IDC_LABEL_MAIN_DESCRIPTION"));
    Tizen::Ui::Controls::Label  *main_temperature = static_cast<Label*>(GetControl(L"IDC_LABEL_MAIN_TEMPERATURE"));
    Tizen::Ui::Controls::Label  *main_current_state = static_cast<Label*>(GetControl(L"IDC_LABEL_CURRENT_STATE"));
    Tizen::Ui::Controls::Panel  *backgroundPanel = static_cast<Panel*>(GetControl(L"IDC_PANEL_BACKGROUND"));

    if (_config->stationsList().size()!=0){
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background_main.png"));
    }else{
        main_background_label->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mask_background.png"));
    }
    main_background_label->RequestRedraw();
    main_current_state->SetShowState(false);
    main_icon->SetShowState(false);
    main_temperature->SetShowState(false);
    main_description->SetShowState(false);

    /* AppLog("_config->current_station_id() %i", _config->current_station_id());
    AppLog("_config->stationsList().size() %i", _config->stationsList().size());
    */
    //if ((_config->stationsList().size() > 0) && _config->current_station_id() > _config->stationsList().size()) {
    if ((_config->stationsList().size() > 0)) {
        String temp = (App::GetInstance()->GetAppDataPath());
        temp.Append(_config->stationsList().at(_config->current_station_id())->fileName().c_str());
        std::string temp_string = (const char*) (Tizen::Base::Utility::StringUtil::StringToUtf8N(temp)->GetPointer());
        // AppLog("Filename %s", temp_string.c_str());
        //_config->dp = Core::DataParser::Instance(temp_string, "");
        _config->dp = Core::DataParser::Instance(_config->stationsList().at(_config->current_station_id())->fileName().c_str(), "");

    }else 
        _config->dp = NULL;

    Core::Data *temp_data = NULL;
    Core::Data *temp_data2 = NULL;
    /* AppLog ("DP %p", _config->dp); */
    time_t current_day;
    struct tm   *tm = NULL;
    /* Timezone */
    if (_config->dp){
        timezone = _config->dp->timezone();
     /*   AppLog("TimeZone %i", timezone); */
    }

    current_day = time(NULL);
    /* AppLog ("Current day: !! %li", current_day); */


    /* Set localtimezone */
    struct tm time_tm1;
    struct tm time_tm2;
    gmtime_r(&current_day, &time_tm1);
    localtime_r(&current_day, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 

    current_day = current_day + 3600*timezone; 
    /* AppLog("Current day0 %li", current_day); */

    tm = gmtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    current_day = mktime(tm); /* today 00:00:00 */
    current_day = current_day + 3600*localtimezone - 3600*timezone; 
    /* AppLog("Current day %li", current_day); */
    /* Footer */
    Footer* pFooter = GetFooter();

    pFooter->RemoveAllItems();

    if (_config->dp && (temp_data = _config->dp->data().GetDataForTime(current_day + 15 * 3600 + _dayNumber*24*3600))) {
        __dayButton = new Tizen::Ui::Controls::FooterItem(); 
        __dayButton->Construct(ID_BUTTON_DAY);
        __dayButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("day_def.png"));
        __dayButton->SetIcon(FOOTER_ITEM_STATUS_SELECTED, Application::GetInstance()->GetAppResource()->GetBitmapN("day_sel.png"));
        __dayButton->SetText(_("Day"));
    }
    /* Check Night */
    __nightButton = NULL;

    if (_config->dp && (temp_data = _config->dp->data().GetDataForTime(current_day + 3 * 3600 + _dayNumber*24*3600))) {
        /* Compare day and night weather */
        if ((temp_data2 = _config->dp->data().GetDataForTime(current_day + 15 * 3600 + _dayNumber*24*3600)) != NULL){
            if (!((temp_data->Icon() == temp_data2->Icon()) &&
                (temp_data->temperature().value(true) == temp_data2->temperature().value(true))&&
                (temp_data->temperature_hi().value(true) == temp_data2->temperature_hi().value(true))&&
                (temp_data->temperature_low().value(true) == temp_data2->temperature_low().value(true))&&
                (temp_data->WindDirection() == temp_data2->WindDirection())&&
                (temp_data->WindSpeed().value() == temp_data2->WindSpeed().value())&&
                (temp_data->Humidity() == temp_data2->Humidity())&&
                (temp_data->pressure().value() == temp_data2->pressure().value()))){
                __nightButton = new Tizen::Ui::Controls::FooterItem(); 
            }
        }else{
            __nightButton = new Tizen::Ui::Controls::FooterItem(); 
        }
        if (__nightButton){
            __nightButton->Construct(ID_BUTTON_NIGHT);
            __nightButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("night_def.png"));
            __nightButton->SetIcon(FOOTER_ITEM_STATUS_SELECTED, Application::GetInstance()->GetAppResource()->GetBitmapN("night_sel.png"));
            __nightButton->SetText(_("Night"));
        }
    }

    /* Check Now */
    __nowButton = NULL;
    if (_dayNumber == 0 && _config->dp != NULL && (temp_data = _config->dp->data().GetDataForTime(time(NULL))) && temp_data->Current()){    
        __nowButton = new Tizen::Ui::Controls::FooterItem(); 
        __nowButton->Construct(ID_BUTTON_NOW);
        __nowButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("now_def.png"));
        __nowButton->SetIcon(FOOTER_ITEM_STATUS_SELECTED, Application::GetInstance()->GetAppResource()->GetBitmapN("now_sel.png"));
        __nowButton->SetText(_("Now"));
    }else
        if (_current_selected_tab == NOW)
            _current_selected_tab = DAY;

    /* Check Hourly */
    for (unsigned int i=1; i<24; i++){ 
        if (_config->dp &&
            (temp_data = _config->dp->data().GetDataForTime(current_day + i * 3600 + _dayNumber*24*3600, true)) != NULL) {    
            __hourlyButton = new Tizen::Ui::Controls::FooterItem(); 
            __hourlyButton->Construct(ID_BUTTON_HOURLY);
            __hourlyButton->SetIcon(FOOTER_ITEM_STATUS_NORMAL, Application::GetInstance()->GetAppResource()->GetBitmapN("hourly_def.png"));
            __hourlyButton->SetIcon(FOOTER_ITEM_STATUS_SELECTED, Application::GetInstance()->GetAppResource()->GetBitmapN("hourly_sel.png"));
            __hourlyButton->SetText(_("Hourly"));
            break;
        }
    }
    pFooter->SetBackButton();
    int tab_count = 0;
    AppLog("11111111");
    if (__nowButton){
        pFooter->AddItem(*__nowButton);
        if (_current_selected_tab == NOW){
            pFooter->SetItemSelected(tab_count);
            AppLog("Set tAB NOW!!!!");
        }
        tab_count++;
    }
    if (__dayButton){
        pFooter->AddItem(*__dayButton);
        if (_current_selected_tab == DAY){
            pFooter->SetItemSelected(tab_count);
            AppLog("Set tAB DAY!!!!");
        }
        tab_count++;
    }
    if (__nightButton){
        pFooter->AddItem(*__nightButton);
        if (_current_selected_tab == NIGHT){
            pFooter->SetItemSelected(tab_count);
            AppLog("Set tAB Night!!!!");
        }
        tab_count++;
    }
    if (__hourlyButton){
        pFooter->AddItem(*__hourlyButton);
        if (_current_selected_tab == HOURLY){
            pFooter->SetItemSelected(tab_count);
           AppLog("Set tAB Hourly!!!!");
        }
    }
    pFooter->RequestRedraw();
    RelativeLayout* pLayout = dynamic_cast< RelativeLayout* >(this->GetLayoutN());
    AppAssert(pLayout);

    if (_pKeyList->GetCount() > 0){
        _pKeyList->RemoveAll(true);
    }
    if (_pValueList->GetCount() > 0){
        _pValueList->RemoveAll(true);
    }


    if (_current_selected_tab == HOURLY){
        /* set current hour */
        time_t current_hour;
        current_hour = time(NULL);
        tm = localtime(&current_hour);
        tm->tm_sec = 0; tm->tm_min = 1; 
        tm->tm_isdst = 1;
        current_hour = mktime(tm) + localtimezone*3600 - 3*3600; 
        int i =0;
        _count_of_hours = 0; 
        /* fill hours */
        while  (_config->dp != NULL && i<5*24*3600) {
            if ((temp_data = _config->dp->data().GetDataForTime(current_hour + i, true)) != NULL){
                if (temp_data->StartTime() + 60 == current_hour + i){
                    _pValueList->Add(new Long(current_hour + i));
                    _pKeyList->Add(new Integer(_count_of_hours));
                    _count_of_hours ++; 
                 }
            }
            i = i + 3600;
        }
        pLayout->SetRelation(*__pTableView, *day_name_label, RECT_EDGE_RELATION_TOP_TO_TOP);
        __pTableView->SetSize(__clientWidth, __clientHeight - pFooter->GetHeight() - INDICATE_HEIGHT);

        __pTableView->RequestRedraw();
        AppLog("Set tAB Hourly relation");
    }else{
        /* Select time for showimg */ 
        time_t time_for_show = 0;
        switch (_current_selected_tab){
            case NOW: 
                time_for_show = time(NULL);
                break;
            case DAY:
                time_for_show = current_day + 15 * 3600 + _dayNumber*24*3600;
                AppLog("Time_for_show %li %li %i", time_for_show, current_day, timezone);
                break;
            case NIGHT:
                time_for_show = current_day + 3 * 3600 + _dayNumber*24*3600;
                break;
        }
        AppLog("Time for Show %li", time_for_show);
        /* Preparing data */
        if (_config->dp && (temp_data = _config->dp->data().GetDataForTime(time_for_show))) {

            day_name_label->SetText(temp_data->FullDayName().c_str());
            day_name_label->RequestRedraw();

            TextToSpeech = "Weather forecast for   ";
            TextToSpeech.Append( _config->stationname().c_str());        
            TextToSpeech.Append( " on "); 

            DateTime dt;
            time_t day_and_time;
            struct tm   *tm1 = NULL;
            day_and_time = temp_data->StartTime() + _config->dp->timezone()*3600;
            tm1 = gmtime(&(day_and_time));
            dt.SetValue(1900 + tm1->tm_year, tm1->tm_mon + 1, tm1->tm_mday, tm1->tm_hour, tm1->tm_min);
            String dateString;
            String timeString;
            LocaleManager localeManager;
            localeManager.Construct();
            Locale  systemLocale = Locale(LANGUAGE_ENG, COUNTRY_US);
            String countryCodeString = systemLocale.GetCountryCodeString();
            String languageCodeString = systemLocale.GetLanguageCodeString();
            Tizen::Locales::DateTimeFormatter* pDateFormatter = DateTimeFormatter::CreateDateFormatterN(systemLocale, DATE_TIME_STYLE_SHORT);
            String customizedPattern = L"EEEE";
            pDateFormatter->ApplyPattern(customizedPattern);
            pDateFormatter->Format(dt, dateString);
            TextToSpeech.Append( "."); 

            TextToSpeech.Append(dateString);
 
            /* Next day */
            if (_config->dp->data().GetDataForTime(time_for_show + 24*3600 ))
                right_label->SetShowState(true);
            else{
                right_label->SetShowState(false);
            }
            right_label->RequestRedraw();

            /* Previous day */
            if ((_dayNumber > 0) && _config->dp->data().GetDataForTime(time_for_show - 24*3600 ))
                left_label->SetShowState(true);
            else
                left_label->SetShowState(false);
            left_label->RequestRedraw();

             AppLog ("_Config_dp inside");
            /* Preparing units */
            temp_data->temperature_low().units(_config->TemperatureUnit());
            temp_data->temperature_hi().units(_config->TemperatureUnit());
            temp_data->temperature().units(_config->TemperatureUnit());
            temp_data->Flike().units(_config->TemperatureUnit());
            temp_data->WindSpeed().units(_config->WindSpeedUnit());
            temp_data->pressure().units(_config->PressureUnit());
            temp_data->ViSible().units(_config->VisibleUnit());
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
              if (temp_data->temperature_hi().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
                t = temp_data->temperature_hi().value();
              }
              if (temp_data->temperature_low().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
                t = temp_data->temperature_low().value();
              }

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
            }else{
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature().value());
                t = temp_data->temperature().value();
            }
            Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(t);
            backgroundPanel->SetBackgroundColor(*color_of_temp);
            delete color_of_temp;
            main_temperature->SetShowState(true);
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);

            if (str.GetLength()<6)
                main_temperature->SetTextConfig(95, LABEL_TEXT_STYLE_NORMAL); 
            else
                if (str.GetLength()<8)
                    main_temperature->SetTextConfig(75, LABEL_TEXT_STYLE_NORMAL); 
                else
                    main_temperature->SetTextConfig(55, LABEL_TEXT_STYLE_NORMAL); 
            main_temperature->SetText(str);
            main_temperature->RequestRedraw();
            /* Current or not current period */
            switch(_current_selected_tab){
                case NOW:
                    Tizen::Base::Utility::StringUtil::Utf8ToString(_("Now"), str);
                    break;
                case DAY:
                    Tizen::Base::Utility::StringUtil::Utf8ToString(_("Day"), str);
                    break;
                case NIGHT:
                    Tizen::Base::Utility::StringUtil::Utf8ToString(_("Night"), str);
                    break;
                case ID_BUTTON_HOURLY:
                    Tizen::Base::Utility::StringUtil::Utf8ToString("", str);
                    break;
                default:
                break;
            }

            main_current_state->SetShowState(true);
            main_current_state->SetText(str);
            main_current_state->RequestRedraw();
            /* Flike Temperature */
            if (temp_data->Flike().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->Flike().value());
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                String *temp_String;
                temp_String =  new String(_("Flike"));
                temp_String->Append(":");
                _pKeyList->Add(temp_String);
            }

            /* Presssure */
            if (temp_data->pressure().value(true) != INT_MAX){
                snprintf (buffer, sizeof(buffer) -1, "%i %s", (int)temp_data->pressure().value(), _(_config->PressureUnit().c_str()));
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                _pKeyList->Add(new String(_("Pressure:")));
            }

            /* Wind direction */
            if (temp_data->WindDirection() != "N/A"){
                snprintf (buffer, sizeof(buffer) -1, "%s", _(temp_data->WindDirection().c_str()));
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                _pKeyList->Add(new String(_("Wind direction:")));
            }
            /* Wind Speed */
            if (temp_data->WindSpeed().value() != INT_MAX){

                if (_config->WindSpeedUnit() == "Beaufort scale"){
                    snprintf (buffer, sizeof(buffer) -1, "%0.f (%s)", 
                        temp_data->WindSpeed().value(), _(_config->WindSpeedUnit().c_str()));
                }else{
                    snprintf (buffer, sizeof(buffer) -1, "%0.f %s", 
                        temp_data->WindSpeed().value(), _(_config->WindSpeedUnit().c_str()));
                }
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                String *temp_String;
                temp_String =  new String(_("Wind speed"));
                temp_String->Append(":");
                _pKeyList->Add(temp_String);
            };
            /* Humidity */
            if (temp_data->Humidity() != INT_MAX){
                snprintf (buffer, sizeof(buffer) -1, "%i%%", temp_data->Humidity());
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                _pKeyList->Add(new String(_("Humidity:")));
            }
            /* Ppcp */
            if (temp_data->Ppcp() != INT_MAX){
                snprintf (buffer, sizeof(buffer) -1, "%0.f", temp_data->Ppcp());
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                _pKeyList->Add(new String(_("Ppcp:")));
            }
            /* UV index */
            if (temp_data->UVindex() != INT_MAX){
                snprintf (buffer, sizeof(buffer) -1, "%0.i", temp_data->UVindex());
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                _pKeyList->Add(new String(_("UV index:")));
            }
            /* Visible */
            if (temp_data->ViSible().value(true) != INT_MAX){
                snprintf (buffer, sizeof(buffer) -1, "%0.f %s", temp_data->ViSible().value(), _(_config->VisibleUnit().c_str()));
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                _pKeyList->Add(new String(_("Visible:")));
            }

              /*  AppLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %li", current_day + 15 * 3600 + _dayNumber*24*3600); */
            /* Sun Rise */
            if (_config->dp->data().GetSunRiseForTime(current_day + 15 * 3600 + _dayNumber*24*3600) > 0){
            /*    AppLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %i %i", timezone, localtimezone); */
                //time_t sun_rise_time =  _config->dp->data().GetSunRiseForTime(current_day + 15 * 3600) + 3600*timezone -3600*localtimezone;
                time_t sun_rise_time =  _config->dp->data().GetSunRiseForTime(current_day + 15 * 3600) + 3600*timezone;
                struct tm   tm1;
               // tm = gmtime(&current_day);
                gmtime_r(&sun_rise_time, &tm1);
                /* Convert date and time */
                DateTime dt;
                dt.SetValue(1900 + tm1.tm_year, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min);
                String dateString;
                String timeString;
                LocaleManager localeManager;
                localeManager.Construct();
                Locale  systemLocale = localeManager.GetSystemLocale();
                String countryCodeString = systemLocale.GetCountryCodeString();
                String languageCodeString = systemLocale.GetLanguageCodeString();
                Tizen::Locales::DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(systemLocale, DATE_TIME_STYLE_SHORT);
                pTimeFormatter->Format(dt, timeString);
                _pValueList->Add(new String(timeString));
                _pKeyList->Add(new String(_("Sunrise:")));
            }
            /* Sun Set */
            if (_config->dp->data().GetSunSetForTime(current_day + 15 * 3600 + _dayNumber*24*3600) > 0){
                /* AppLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %i %i", timezone, localtimezone); */
                time_t sun_set_time =  _config->dp->data().GetSunSetForTime(current_day + 15 * 3600) + 3600*timezone;
                struct tm   tm1;
               // tm = gmtime(&current_day);
                gmtime_r(&sun_set_time, &tm1);
                /* Convert date and time */
                DateTime dt;
                dt.SetValue(1900 + tm1.tm_year, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min);
                String dateString;
                String timeString;
                LocaleManager localeManager;
                localeManager.Construct();
                Locale  systemLocale = localeManager.GetSystemLocale();
                String countryCodeString = systemLocale.GetCountryCodeString();
                String languageCodeString = systemLocale.GetLanguageCodeString();
                Tizen::Locales::DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(systemLocale, DATE_TIME_STYLE_SHORT);
                pTimeFormatter->Format(dt, timeString);
                _pValueList->Add(new String(timeString));
                _pKeyList->Add(new String(_("Sunset:")));
                time_t sun_rise_time =  _config->dp->data().GetSunRiseForTime(current_day + 15 * 3600) + 3600*timezone;
                time_t day_length_time =  sun_set_time - sun_rise_time;
                gmtime_r(&day_length_time, &tm1);
                snprintf (buffer, sizeof(buffer) -1, "%i:%i", tm1.tm_hour, tm1.tm_min);
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                _pValueList->Add(new String(str));
                _pKeyList->Add(new String(_("Day length:")));
            }

            if (_config->dp->LastUpdate()>0){
                time_t last_update = _config->dp->LastUpdate();
                struct tm   tm1;
                localtime_r(&last_update, &tm1);

                /* Convert date and time */
                DateTime dt;
                dt.SetValue(1900 + tm1.tm_year, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min);
                String dateString;
                String timeString;
                LocaleManager localeManager;
                localeManager.Construct();
                Locale  systemLocale = localeManager.GetSystemLocale();
                String countryCodeString = systemLocale.GetCountryCodeString();
                String languageCodeString = systemLocale.GetLanguageCodeString();
                Tizen::Locales::DateTimeFormatter* pDateFormatter = DateTimeFormatter::CreateDateFormatterN(systemLocale, DATE_TIME_STYLE_DEFAULT);
                Tizen::Locales::DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(systemLocale, DATE_TIME_STYLE_SHORT);
                String customizedPattern = L"dd MMM ";
                pDateFormatter->ApplyPattern(customizedPattern);
                pDateFormatter->Format(dt, dateString);
                pTimeFormatter->Format(dt, timeString);
                dateString.Append(timeString);
                _pValueList->Add(new String(dateString));
                _pKeyList->Add(new String(_("Last update:")));
            }

            /*
                  if (_dp->data().GetSunSetForTime(current_day + 15 * 3600) >0)
                              forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 15 * 3600) + 3600*timezone - 3600*localtimezone);
                        forecast_data->LastUpdate(_dp->LastUpdate());
             */

        }else{
            AppLog("Nothing");
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
        }
        pLayout->SetRelation(*__pTableView, *main_background_label, RECT_EDGE_RELATION_TOP_TO_BOTTOM);
        __pTableView->SetSize(__clientWidth, __clientHeight - pFooter->GetHeight() - main_background_label->GetHeight() - INDICATE_HEIGHT);
    }
    __pTableView->UpdateTableView();
    backgroundPanel->RequestRedraw();

    pFooter->SetColor(pFooter->GetButtonColor(BUTTON_ITEM_STATUS_NORMAL));
    pFooter->RequestRedraw();
    __updateTimer->StartAsRepeatable(500);



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
    AppLog("meecastFullWeatherForm::GetItemCount");
    AppLog("meecastFullWeatherForm::GetItemCount %i", _pValueList->GetCount()/2);
    /*
	if (sectionIndex == SECTION_TYPE_REPEAT_TYPE)
	{
		return SECTION_ITEM_COUNT_TYPE;
	}
	else //(sectionIndex == SECTION_TYPE_UNTILE_TYPE)
	{
		return SECTION_ITEM_COUNT_UNTIL;
	}
    */
    if (_current_selected_tab != HOURLY){
        if (_pValueList->GetCount() % 2 > 0)
            return (_pValueList->GetCount() / 2) + 1;
        else    
            return _pValueList->GetCount() / 2;
    }else{
        return _pValueList->GetCount();
    }
}

void
meecastFullWeatherForm::UpdateItem(int index, TableViewItem* pItem)
{
    return;
}

bool
meecastFullWeatherForm::DeleteItem(int index, TableViewItem* pItem)
{
//	pItem->RemoveAllControls();
    delete pItem;
    pItem = null;

   // pItem->Destroy();
    return true;
}




int
meecastFullWeatherForm::GetDefaultItemHeight(void){
    return 120;
}

TableViewItem*
meecastFullWeatherForm::CreateItem(int index, int itemWidth){

    String title;
    String description;
    char buffer[4096];

    AppLog("meecastFullWeatherForm::CreateItem");
    TableViewAnnexStyle style = TABLE_VIEW_ANNEX_STYLE_NORMAL;

    TableViewItem* pItem = new TableViewItem();

    AppLogExceptionIf(pItem == null, "Table item creation is failed");
    pItem->Construct(Dimension(itemWidth, 100), style);

    if (_current_selected_tab != HOURLY){

        pItem->SetBackgroundColor(Tizen::Graphics::Color(0x00, 0x00, 0x00), TABLE_VIEW_ITEM_DRAWING_STATUS_NORMAL);
        if (_pKeyList->GetAt(2*index)){
            Label* pKeyTitleLabel = new Label();
            pKeyTitleLabel->Construct(Rectangle(0, 0, __clientWidth/2, 50), *static_cast< String* >(_pKeyList->GetAt(2*index)));
            pKeyTitleLabel->SetTextColor(Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)));
            pKeyTitleLabel->SetTextConfig(28, LABEL_TEXT_STYLE_NORMAL);
            pKeyTitleLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
            pItem->AddControl(*pKeyTitleLabel);

            if (_pValueList->GetAt(2*index)){
                Label* pKeyDescriptionLabel = new Label();
                pKeyDescriptionLabel->Construct(Rectangle(0, 20, __clientWidth/2, 100), *static_cast< String* >(_pValueList->GetAt(2*index)));
                pKeyDescriptionLabel->SetTextConfig(36, LABEL_TEXT_STYLE_NORMAL);
                pKeyDescriptionLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
                pItem->AddControl(*pKeyDescriptionLabel);
            }
        }

        if(_pKeyList->GetCount()/2 + 1 > index + 1){
            Label* pKeyTitleLabel = new Label();
            pKeyTitleLabel->Construct(Rectangle(__clientWidth/2, 0, __clientWidth/2, 50), *static_cast< String* >(_pKeyList->GetAt(2*index + 1)));
            pKeyTitleLabel->SetTextConfig(28, LABEL_TEXT_STYLE_NORMAL);
            pKeyTitleLabel->SetTextColor(Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)));
            pKeyTitleLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
            pItem->AddControl(*pKeyTitleLabel);

            if (_pValueList->GetAt(2*index + 1)){
                Label* pKeyDescriptionLabel = new Label();
                pKeyDescriptionLabel->Construct(Rectangle(__clientWidth/2, 20, __clientWidth/2, 100), *static_cast< String* >(_pValueList->GetAt(2*index + 1)));
                pKeyDescriptionLabel->SetTextConfig(36, LABEL_TEXT_STYLE_NORMAL);
                pKeyDescriptionLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
                pItem->AddControl(*pKeyDescriptionLabel);
            }
        }
    }
    if (_current_selected_tab == HOURLY){
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

        String str;
        /* fill hours */
        if (_config->dp != NULL) {
            Long* pLong = static_cast< Long* >(_pValueList->GetAt(index));
            if (pLong){
                temp_data = _config->dp->data().GetDataForTime((time_t)pLong->ToLong(), true);
            }else
                temp_data = NULL;

            if ( temp_data){
                /* Preparing units */
                temp_data->temperature().units(_config->TemperatureUnit());
                temp_data->WindSpeed().units(_config->WindSpeedUnit());
                temp_data->pressure().units(_config->PressureUnit());

                /* Convert date and time */
                DateTime dt;
                time_t day_and_time;
                struct tm   *tm1 = NULL;
                day_and_time = temp_data->StartTime() + _config->dp->timezone()*3600;
                tm1 = gmtime(&(day_and_time));
                dt.SetValue(1900 + tm1->tm_year, tm1->tm_mon + 1, tm1->tm_mday, tm1->tm_hour, tm1->tm_min);
                String dateString;
                String timeString;
                LocaleManager localeManager;
                localeManager.Construct();
                Locale  systemLocale = localeManager.GetSystemLocale();
                String countryCodeString = systemLocale.GetCountryCodeString();
                String languageCodeString = systemLocale.GetLanguageCodeString();
                Tizen::Locales::DateTimeFormatter* pDateFormatter = DateTimeFormatter::CreateDateFormatterN(systemLocale, DATE_TIME_STYLE_DEFAULT);
                Tizen::Locales::DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(systemLocale, DATE_TIME_STYLE_SHORT);
//                timeString = pTimeFormatter->GetPattern();
                String customizedPattern = L"dd MMM";
                pDateFormatter->ApplyPattern(customizedPattern);
                pDateFormatter->Format(dt, dateString);
                pTimeFormatter->Format(dt, timeString);
                if (timeString.StartsWith(":", 1))
                    timeString.Insert("0", 0);

                if ((index-1) %2 != 0 ){
                    pItem->SetBackgroundColor(Tizen::Graphics::Color(0x00, 0x00, 0x00), TABLE_VIEW_ITEM_DRAWING_STATUS_NORMAL);
                }else{
                    pItem->SetBackgroundColor(Tizen::Graphics::Color(0x1F, 0x1F, 0x1F), TABLE_VIEW_ITEM_DRAWING_STATUS_NORMAL);
                }

                Label* pDateLabel = new Label();
                pDateLabel->Construct(Rectangle(5, 20, 220, 50), dateString);
                pDateLabel->SetTextConfig(38, LABEL_TEXT_STYLE_NORMAL);

                pDateLabel->SetTextColor(Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)));
                pDateLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
                pItem->AddControl(*pDateLabel);

                Label* pHourLabel = new Label();
/*

               struct tm   *tm = NULL;
                time_t current_day;
                current_day = temp_data->StartTime() + _config->dp->timezone()*3600;
               // tm = localtime(&(current_day));
                tm = gmtime(&(current_day));

                snprintf(buffer, sizeof(buffer) - 1, "%02d:00", tm->tm_hour);
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
//                pHourLabel->Construct(Rectangle(150, 20, 140, 50), str);
*/
                pHourLabel->Construct(Rectangle(130, 20, 220, 50), timeString);
                pHourLabel->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
                pHourLabel->SetTextColor(Tizen::Graphics::Color(Color::GetColor(COLOR_ID_WHITE)));
  //              pHourLabel->SetTextHorizontalAlignment(ALIGNMENT_RIGHT);
                pHourLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
                pItem->AddControl(*pHourLabel);

                Label* main_icon = new Label();
                main_icon->Construct(Rectangle(350, 0, 90, 90), "");
                Tizen::Base::Integer icon_int =  temp_data->Icon();
                if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Atmos/" + icon_int.ToString() + ".png")){
                    /* Main Icon */ 
                    Tizen::Media::Image *image = null;
                    Tizen::Graphics::Bitmap* mainIconBitmap = null;
                    image = new (std::nothrow) Tizen::Media::Image();
                    image->Construct();

                    String icon_number = temp_data->Icon();

                    mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Atmos/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
                    main_icon->SetBackgroundBitmap(*mainIconBitmap);
                    main_icon->RequestRedraw();
                    SAFE_DELETE(image);
                    SAFE_DELETE(mainIconBitmap);
                }
                pItem->AddControl(*main_icon);

                if (temp_data->temperature().value(true) != INT_MAX){
                    snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature().value());
                    Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                    Label* pKeyTitleLabel = new Label();
                    Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(temp_data->temperature().value());
                    pKeyTitleLabel->Construct(Rectangle( __clientWidth - 140, 20, 130, 50), str);
                    pKeyTitleLabel->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
                    pKeyTitleLabel->SetTextColor(*color_of_temp);
                    pKeyTitleLabel->SetTextHorizontalAlignment(ALIGNMENT_RIGHT);
                    pItem->AddControl(*pKeyTitleLabel);
                }
            }
        }
    }

    return pItem;
}


void
meecastFullWeatherForm::OnTableViewItemStateChanged(TableView& tableView, int itemIndex, TableViewItem* pItem, TableViewItemStatus status)
{
}

void
meecastFullWeatherForm::OnTableViewContextItemActivationStateChanged(TableView& tableView, int itemIndex, TableViewContextItem* pContextItem, bool activated)
{
}

void
meecastFullWeatherForm::OnTableViewItemReordered(TableView& tableView, int itemIndexFrom, int itemIndexTo)
{
}

void
meecastFullWeatherForm::OnFlickGestureDetected(TouchFlickGestureDetector& gestureDetector)
{
    AppLog("Flick detected!");
    Rectangle rc(0, 0, 0, 0);
    Point point(0, 0);
    Tizen::Ui::Controls::Label  *right_label = static_cast<Label*>(GetControl(L"IDC_LABEL_RIGHT_BUTTON"));


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
            if (_dayNumber>0)
                _dayNumber--;
            ReInitElements(); 
            AppLog("Flick detected RIGHT");
            break;
        case FLICK_DIRECTION_LEFT:
           if (right_label->GetShowState()){
                _dayNumber++;
                ReInitElements(); 
            }
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
meecastFullWeatherForm::OnFlickGestureCanceled(TouchFlickGestureDetector& gestureDetector)
{
    AppLog("Flick canceled!");
}




void
meecastFullWeatherForm::OnTextToSpeechInitialized(void)
{
    AppLog("OnTextToSpeechInitialized");
     __pTts->SetSpeechRate(TEXT_TO_SPEECH_SPEECH_RATE_SYSTEM_SETTING);
	result r = E_SUCCESS;
    r = __pTts->SetLocale(Locale(LANGUAGE_ENG, COUNTRY_US));
	TryLog(!IsFailed(r), "[%s] setlocale() error", GetErrorMessage(r));


}

void
meecastFullWeatherForm::OnTextToSpeechStatusChanged(Tizen::Uix::Speech::TextToSpeechStatus status){
	if (status == TEXT_TO_SPEECH_STATUS_READY){
        _speaking = false;
	}
	if (status == TEXT_TO_SPEECH_STATUS_PLAYING){
        _speaking = true;
	}

	if(status == TEXT_TO_SPEECH_STATUS_PAUSED) {
	}


}

void
meecastFullWeatherForm::OnTextToSpeechErrorOccurred(Tizen::Uix::Speech::TextToSpeechError error){
	if (error == TEXT_TO_SPEECH_ERROR_UNSUPPORTED_SERVICE){
//		this->SendUserEvent(REQUEST_ID_EXIT, null);
	}
}

void
meecastFullWeatherForm::OnTextToSpeechCompleted(void){

    _speaking = false;
}

void
meecastFullWeatherForm::OnTimerExpired(Tizen::Base::Runtime::Timer& timer){
    result r = E_FAILURE;
    if (_config->SpeechFullWindow()){
        AppLog("test");
        r = __pTts->Speak(TextToSpeech, TEXT_TO_SPEECH_REQUEST_MODE_REPLACE);
        AppLog("Text %S", TextToSpeech.GetPointer());
        if (IsFailed(r)){
            AppLog("[%s] Speak() error", GetErrorMessage(r));
        }else{
            __updateTimer->Cancel();
        }
    }else{
        __updateTimer->Cancel();
    }
}

