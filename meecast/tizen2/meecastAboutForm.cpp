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
#include "meecastAboutForm.h"
#include "../core/core.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::System;
using namespace Tizen::Io;


meecastAboutForm::meecastAboutForm(void)
{
}

meecastAboutForm::~meecastAboutForm(void)
{
}

bool
meecastAboutForm::Initialize(void)
{
    Construct(L"ABOUT_FORM");
    return true;
}

result
meecastAboutForm::OnInitializing(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your initialization code here

    //CreateContextMenuList();
    // Setup back event listener
    SetFormBackEventListener(this);

    return r;
}

result
meecastAboutForm::OnTerminating(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastAboutForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);

    AppLog("Check Action");
    switch(actionId){

    default:
        break;
    }
}

void
meecastAboutForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

void
meecastAboutForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    AppLog("OnSceneActivatedN Sources");
    Tizen::Ui::Controls::Label  *main_icon = static_cast<Label*>(GetControl(L"IDC_LABEL_MAIN_ICON"));
    if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Atmos/30.png")){
        /* Main Icon */ 
        Tizen::Media::Image *image = null;
        Tizen::Graphics::Bitmap* mainIconBitmap = null;
        image = new (std::nothrow) Tizen::Media::Image();
        image->Construct();
        mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Atmos/30.png",Tizen::Graphics::BITMAP_PIXEL_FORMAT_ARGB8888);
        main_icon->SetBackgroundBitmap(*mainIconBitmap);
        main_icon->RequestRedraw();
        SAFE_DELETE(image);
        SAFE_DELETE(mainIconBitmap);
    }
    Tizen::Ui::Controls::Panel  *backgroundPanel = static_cast<Panel*>(GetControl(L"IDC_PANEL_BACKGROUND"));
    backgroundPanel->SetBackgroundColor(Tizen::Graphics::Color(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_VIOLET)));
    Tizen::Ui::Controls::Label  *title_label = static_cast<Label*>(GetControl(L"IDC_LABEL_TITLE"));
    title_label->SetText(_("MeeCast for Tizen"));
    Tizen::Ui::Controls::Label  *version_label = static_cast<Label*>(GetControl(L"IDC_LABEL_VERSION"));

    String temp = (_("Version"));
    temp.Append(" ");
    temp.Append(MEECAST_VERSION);
    version_label->SetText(temp);
    Tizen::Ui::Controls::ScrollPanel  *scrollPanel = static_cast<ScrollPanel*>(GetControl(L"IDC_SCROLLPANEL"));
    Tizen::Graphics::Rectangle clientRect;
    Form *pForm = static_cast<Form*>(GetParent());
    AppAssert(pForm);
    clientRect = pForm->GetClientAreaBounds();
    Label* about = new Label();
    about->Construct(Tizen::Graphics::Rectangle(0, 0, clientRect.width, 200), _("About:"));
    about->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    Label* fillabout = new Label();
    fillabout->Construct(Tizen::Graphics::Rectangle(0,50, 200, 150), _("Aboutmy:"));

    EditField* pEdit = new EditField();
    pEdit->Construct(Tizen::Graphics::Rectangle(0, 250, 200, 150));
    pEdit->SetText(L"Edit");

    scrollPanel->AddControl(about);
    scrollPanel->AddControl(fillabout);
    scrollPanel->AddControl(pEdit);

}

void
meecastAboutForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}


