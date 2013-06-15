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


