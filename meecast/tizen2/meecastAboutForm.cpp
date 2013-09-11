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
        case ID_BUTTON_DONATION:
            AppControlBrowser(Tizen::Base::String(L""));
            break;
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

    Label* support = new Label();
    support->Construct(Tizen::Graphics::Rectangle(0, 165, clientRect.width, 50), _("Support"));
    support->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    support->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* fillsupport = new Label();
    fillsupport->Construct(Tizen::Graphics::Rectangle(0,200, clientRect.width, 150), _("support@meecast.org"));
    fillsupport->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    fillsupport->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_BLUE));
    fillsupport->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
    fillsupport->SetTextVerticalAlignment(ALIGNMENT_TOP);
    fillsupport->AddTouchEventListener(*this);

    Label* admin = new Label();
    admin->Construct(Tizen::Graphics::Rectangle(0, 250, clientRect.width, 50), _("Project administrator"));
    admin->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    admin->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filladmin = new Label();
    filladmin->Construct(Tizen::Graphics::Rectangle(0,300, clientRect.width, 150), _("Vlad Vailyeu, Oksana Kalinkevich"));
    filladmin->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filladmin->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* develop = new Label();
    develop->Construct(Tizen::Graphics::Rectangle(0, 340, clientRect.width, 50), _("Programmers"));
    develop->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    develop->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filldevelop = new Label();
    filldevelop->Construct(Tizen::Graphics::Rectangle(0,385, clientRect.width, 150), _("Vlad Vailyeu"));
    filldevelop->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filldevelop->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* designer = new Label();
    designer->Construct(Tizen::Graphics::Rectangle(0, 430, clientRect.width, 50), _("Lead designer"));
    designer->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    designer->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filldesigner = new Label();
    filldesigner->Construct(Tizen::Graphics::Rectangle(0,475, clientRect.width, 150), _("Andrew Zhilin"));
    filldesigner->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filldesigner->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* manager = new Label();
    manager->Construct(Tizen::Graphics::Rectangle(0, 520, clientRect.width, 50), _("Project manager"));
    manager->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    manager->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* fillmanager = new Label();
    fillmanager->Construct(Tizen::Graphics::Rectangle(0,565, clientRect.width, 150), _("Ludmila Lisovskaya"));
    fillmanager->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    fillmanager->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* platform = new Label();
    platform->Construct(Tizen::Graphics::Rectangle(0, 610, clientRect.width, 50), _("Localization platform"));
    platform->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    platform->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));

    __fillplatform = new Label();
    __fillplatform->Construct(Tizen::Graphics::Rectangle(0,650, clientRect.width, 150), "http://transifex.net");
    __fillplatform->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    __fillplatform->SetTextVerticalAlignment(ALIGNMENT_TOP);
    __fillplatform->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_BLUE));
    __fillplatform->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
    __fillplatform->AddTouchEventListener(*this);

    Label* translators = new Label();
    translators->Construct(Tizen::Graphics::Rectangle(0, 700, clientRect.width, 50), _("Translators"));
    translators->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    translators->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filltranslators = new Label();
    filltranslators->Construct(Tizen::Graphics::Rectangle(0,750, clientRect.width, 350), _("\
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
Spanish - Alejandro Lopez, Agustí Clara\n \
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
Chinese(Taiwan) - osk2,\n \
Vietnamese - Hieu Nguyen\n \
\tAnh Dương and truongnx, \n \
Swedish - Angela Nennebrandt,\n \
\tNiklas Åkerström, \n \
\tTomislav Veleckovik"));
    filltranslators->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filltranslators->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* license = new Label();
    license->Construct(Tizen::Graphics::Rectangle(0, 1060, clientRect.width, 50), _("License"));
    license->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    license->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    Label* filllicense = new Label();
    filllicense->Construct(Tizen::Graphics::Rectangle(0,1090, clientRect.width, 350), _("\
This software is free software; you can\n \
redistribute it and/or modify it under \n \
the terms of the GNU General Public\n \
License as published by the Free Software \n \
Foundation; either version 2.1 of the \n \
License, or (at your option) any \n \
later version."));
    filllicense->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    filllicense->SetTextVerticalAlignment(ALIGNMENT_TOP);

    Label* source = new Label();
    source->Construct(Tizen::Graphics::Rectangle(0, 1405, clientRect.width, 50), _("Source code in github"));
    source->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    source->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_GREY));
    __fillsource = new Label();
    __fillsource->Construct(Tizen::Graphics::Rectangle(0,1455, clientRect.width, 350), "http://github.com/Meecast/meecast");
    __fillsource->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    __fillsource->SetTextVerticalAlignment(ALIGNMENT_TOP);
    __fillsource->SetTextColor(Tizen::Graphics::Color::GetColor(Tizen::Graphics::COLOR_ID_BLUE));
    __fillsource->AddTouchEventListener(*this);


    Button* donationButton = new Button();
    donationButton->Construct(Tizen::Graphics::Rectangle(clientRect.width - 184 - 20, 190, 184, 52), ""); 
    donationButton->SetNormalBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("btn_donate_LG.png"));
    donationButton->SetPressedBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("btn_donate_LG.png"));

    donationButton->SetActionId(ID_BUTTON_DONATION);
    donationButton->AddActionEventListener(*this);

    scrollPanel->AddControl(about);
    scrollPanel->AddControl(fillabout);
    scrollPanel->AddControl(support);
    scrollPanel->AddControl(fillsupport);
    scrollPanel->AddControl(admin);
    scrollPanel->AddControl(filladmin);
    scrollPanel->AddControl(develop);
    scrollPanel->AddControl(filldevelop);
    scrollPanel->AddControl(designer);
    scrollPanel->AddControl(filldesigner);
    scrollPanel->AddControl(manager);
    scrollPanel->AddControl(fillmanager);
    scrollPanel->AddControl(platform);
    scrollPanel->AddControl(__fillplatform);
    scrollPanel->AddControl(translators);
    scrollPanel->AddControl(filltranslators);
    scrollPanel->AddControl(license);
    scrollPanel->AddControl(filllicense);
    scrollPanel->AddControl(source);
    scrollPanel->AddControl(__fillsource);
    scrollPanel->AddControl(donationButton);
}

void
meecastAboutForm::AppControlBrowser(Tizen::Base::String uri){
    if (uri == ""){
        uri = "file://";
        uri.Append(App::GetInstance()->App::GetAppSharedPath());
        uri.Append(L"res/donation.html");
    }
    AppControl* pAc = AppManager::FindAppControlN(L"tizen.internet",
                                                     L"http://tizen.org/appcontrol/operation/view");
    if (pAc){
        pAc->Start(&uri, null, null, null);
        delete pAc;
    }
}

void
meecastAboutForm::AppControlEmail(void){

   Tizen::Base::Collection::HashMap extraData;
   extraData.Construct();
   String subjectKey = L"http://tizen.org/appcontrol/data/subject";
   String subjectVal = L"Problem in the MeeCast";
   String toKey = L"http://tizen.org/appcontrol/data/to";
   String toVal = L"support@meecast.org";
   extraData.Add(&subjectKey, &subjectVal);
   extraData.Add(&toKey, &toVal);
   AppControl* pAc = AppManager::FindAppControlN(L"tizen.email",
                                                  L"http://tizen.org/appcontrol/operation/compose");
   if (pAc){
       pAc->Start(null, null, &extraData, null);
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

void
meecastAboutForm::OnTouchPressed(const Tizen::Ui::Control& source, 
                                const Tizen::Graphics::Point& currentPosition,
		                        const Tizen::Ui::TouchEventInfo& touchInfo) {
    if (source.Equals(*__fillsource)){
        AppControlBrowser(Tizen::Base::String(L"https://github.com/Meecast/meecast"));
    }else{
        if (source.Equals(*__fillplatform)){
            AppControlBrowser(Tizen::Base::String(L"https://www.transifex.com/projects/p/meecast/"));
        }else
            AppControlEmail();
    }
}

void
meecastAboutForm::OnTouchReleased(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo)
{
}


