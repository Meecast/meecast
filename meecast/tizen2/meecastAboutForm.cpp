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
    about->Construct(Tizen::Graphics::Rectangle(0, 0, clientRect.width, 50), _("About"));
    about->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    about->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* fillabout = new Label();
    fillabout->Construct(Tizen::Graphics::Rectangle(0,50, clientRect.width, 150), _("MeeCast - multiplatform highly customizable open source weather forecast client based on OMWeather code. Copyright (C) 2006-2013"));
    fillabout->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    fillabout->SetTextVerticalAlignment(ALIGNMENT_TOP);
    Label* admin = new Label();
    admin->Construct(Tizen::Graphics::Rectangle(0, 200, clientRect.width, 50), _("Project administrator"));
    admin->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    admin->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filladmin = new Label();
    filladmin->Construct(Tizen::Graphics::Rectangle(0,250, clientRect.width, 150), _("Vlad Vailyeu, Oksana Kalinkevich"));
    filladmin->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filladmin->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* develop = new Label();
    develop->Construct(Tizen::Graphics::Rectangle(0, 300, clientRect.width, 50), _("Programmers"));
    develop->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    develop->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filldevelop = new Label();
    filldevelop->Construct(Tizen::Graphics::Rectangle(0,350, clientRect.width, 150), _("Vlad Vailyeu"));
    filldevelop->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filldevelop->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* designer = new Label();
    designer->Construct(Tizen::Graphics::Rectangle(0, 400, clientRect.width, 50), _("Lead designer"));
    designer->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    designer->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filldesigner = new Label();
    filldesigner->Construct(Tizen::Graphics::Rectangle(0,450, clientRect.width, 150), _("Andrew Zhilin"));
    filldesigner->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filldesigner->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* manager = new Label();
    manager->Construct(Tizen::Graphics::Rectangle(0, 500, clientRect.width, 50), _("Project manager"));
    manager->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    manager->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* fillmanager = new Label();
    fillmanager->Construct(Tizen::Graphics::Rectangle(0,550, clientRect.width, 150), _("Ludmila Lisovskaya"));
    fillmanager->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    fillmanager->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* translators = new Label();
    translators->Construct(Tizen::Graphics::Rectangle(0, 600, clientRect.width, 50), _("Translators"));
    translators->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    translators->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filltranslators = new Label();
    filltranslators->Construct(Tizen::Graphics::Rectangle(0,650, clientRect.width, 350), _("\
French -  Nicolas Granziano, \n \
\tPhilippe, Benot Knecht \n \
Russian - Pavel Fialko, Vlad Vasiliev, \n \
\tEd Bartosh\n \
Finnish - Marko Vertainen\n \
German -  Claudius Henrichs\n \
Italian - Lorenzo Facca,\n \
\tPavel Fialko, Alessandro Pasotti,\n \
\tSalvatore Fiorenzo, Ilir Gjika,\n \
\tLuca Onnis\n \
Spanish - Alejandro Lopez\n \
Polish - Tomasz Dominikowski\n \
Slovak - Roman Moravčík\n \
Dutch - Tom Leenders, Michiel Pater\n \
Bulgarian - Zlatko Enikov\n \
Albanian - Ilir Gjika\n \
Chinese - Wong Man Kong, Michael\n \
Portuguese - Jose Narciso,\n \
Turkish - Burak Kaan Kose,\n \
Catalan - Agustí Clara,\n \
Arabic - Tarad Alfandi,\n \
Serbian - Aleksandar Zaric,\n \
Norwegian - Peer-Atle Motland,\n \
Hungarian - Salmi,\n \
Vietnamese - Hieu Nguyen\n \
\tAnh Dương and truongnx, \n \
Swedish - Angela Nennebrandt,\n \
\tNiklas Åkerström, \n \
\tTomislav Veleckovik"));
    filltranslators->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filltranslators->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* license = new Label();
    license->Construct(Tizen::Graphics::Rectangle(0, 1000, clientRect.width, 50), _("License"));
    license->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    license->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filllicense = new Label();
    filllicense->Construct(Tizen::Graphics::Rectangle(0,1050, clientRect.width, 350), _("\
This software is free software; you can\n \
redistribute it and/or modify it under \n \
the terms of the GNU General Public\n \
License as published by the Free Software \n \
Foundation; either version 2.1 of the \n \
License, or (at your option) any \n \
later version."));
    filllicense->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filllicense->SetTextVerticalAlignment(ALIGNMENT_TOP);



    scrollPanel->AddControl(about);
    scrollPanel->AddControl(fillabout);
    scrollPanel->AddControl(admin);
    scrollPanel->AddControl(filladmin);
    scrollPanel->AddControl(develop);
    scrollPanel->AddControl(filldevelop);
    scrollPanel->AddControl(designer);
    scrollPanel->AddControl(filldesigner);
    scrollPanel->AddControl(manager);
    scrollPanel->AddControl(fillmanager);
    scrollPanel->AddControl(translators);
    scrollPanel->AddControl(filltranslators);
    scrollPanel->AddControl(license);
    scrollPanel->AddControl(filllicense);

}

void
meecastAboutForm::AppControlBrowser(void){
    String uri = L"http://www.tizen.org";
    AppControl* pAc = AppManager::FindAppControlN(L"tizen.internet",
                                                                 L"http://tizen.org/appcontrol/operation/view");
    if (pAc){
        pAc->Start(&uri, null, null, null);
        delete pAc;
    }
}

void
meecastAboutForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}


