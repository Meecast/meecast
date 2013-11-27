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
#include "meecastAppereanceForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;

static const int LIST_HEIGHT = 112;
static const int BUTTON_HEIGHT = 74;
static const RequestId SEND_RELOAD_CONFIG = 200;

meecastAppereanceForm::meecastAppereanceForm(void)
                            : __pList(null)
                            ,__pSliderX(null)
                            ,__pSliderY(null)
{
    _config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");

}

meecastAppereanceForm::~meecastAppereanceForm(void)
{
    _config->DeleteInstance();
}

bool
meecastAppereanceForm::Initialize(void)
{
    Construct(L"APPEARANCE_FORM");
    return true;
}

result
meecastAppereanceForm::OnInitializing(void)
{
    result r = E_SUCCESS;

    SetFormBackEventListener(this);
	CreateGroupedListView();

    __pSliderX = static_cast<Slider*>(GetControl(L"IDC_SLIDERX"));
    __pSliderY = static_cast<Slider*>(GetControl(L"IDC_SLIDERY"));
    __pSliderX->SetTitleText(_("Select the X-position for left corner of widget"));
    __pSliderY->SetTitleText(_("Select the Y-position for left corner of widget"));
    __pSliderX->SetValue(_config->Xleft_corner_of_lockscreen_widget());
    __pSliderY->SetValue(_config->Yleft_corner_of_lockscreen_widget());
    __pSliderX->SetRange(40, 130);
    __pSliderY->SetRange(110, 960);
    __pSliderX->AddAdjustmentEventListener(*this);
    __pSliderY->AddAdjustmentEventListener(*this);

    Header* pHeader = GetHeader();
    pHeader->SetTitleText(_("Appearance"));

    __pList->SetItemChecked(1,1, true);
    __pList->RequestRedraw();

    return r;
}


void
meecastAppereanceForm::CreateGroupedListView(void)
{
	__pList = static_cast <GroupedListView*> (GetControl("IDC_GROUPEDLISTVIEW"));
	__pList->SetItemProvider(*this);
	__pList->AddGroupedListViewItemEventListener(*this);
	AddControl(*__pList);
}

result
meecastAppereanceForm::OnTerminating(void)
{
    result r = E_SUCCESS;
	// Deallocate bitmaps
    // TODO:
    // Add your termination code here
    return r;
}

void
meecastAppereanceForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
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
meecastAppereanceForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

    _config->saveConfig();
    App* pApp = App::GetInstance();
    pApp->SendUserEvent(SEND_RELOAD_CONFIG, null);

	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

void
meecastAppereanceForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    // TODO:
    // Add your scene activate code here
    AppLog("OnSceneActivatedN Settings");
    __pList->UpdateList();
}

void
meecastAppereanceForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}


ListItemBase*
meecastAppereanceForm::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{


    CustomItem* pItem = new (std::nothrow) CustomItem();
    TryReturn(pItem != null, null, "Out of memory");

    String* pStr;

    Tizen::Media::Image *image = null;
    Tizen::Graphics::Bitmap* mainIconBitmap = null;
    switch (groupIndex){
         case 0: 
            switch (itemIndex % 8){
            case 0:
                pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_DETAILED);
                pStr = new String (_("Iconset"));
                pStr->Append(" - ");
                pStr->Append(_config->iconSet().c_str());
                pItem->AddElement(Tizen::Graphics::Rectangle(16, 32, 700, 50), 0, *pStr, 36,
                                  Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)),
                                  Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)),
                                  Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)), true);

                image = new (std::nothrow) Tizen::Media::Image();
                image->Construct();


                /* AppLog("Iconset %s", config->iconSet().c_str()); */
                mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/" + _config->iconSet().c_str() + "/30.png", BITMAP_PIXEL_FORMAT_ARGB8888);
                pItem->AddElement(Rectangle(480, 10, 100, 100), 502, *mainIconBitmap, null, null);
                SAFE_DELETE(image);
                SAFE_DELETE(mainIconBitmap);

                break;
            case 1:
                pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_DETAILED);
                pStr = new String (_("Modification"));
                pStr->Append(" - ");
                pStr->Append(_config->Mod().c_str());
                pItem->AddElement(Tizen::Graphics::Rectangle(16, 32, 700, 50), 0, *pStr, 36,
                                  Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)),
                                  Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)),
                                  Tizen::Graphics::Color(Color::GetColor(COLOR_ID_GREY)), true);

                break;
            case 2:
                pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_ONOFF_SLIDING);
                pStr = new String (_("Voice forecast button on a widget"));
                pItem->AddElement(Tizen::Graphics::Rectangle(16, 32, 700, 50), 0, *pStr, 36,
                                  Tizen::Graphics::Color(Tizen::Graphics::Color::GetColor(COLOR_ID_GREY)),
                                  Tizen::Graphics::Color(Tizen::Graphics::Color::GetColor(COLOR_ID_GREY)),
                                  Tizen::Graphics::Color(Tizen::Graphics::Color::GetColor(COLOR_ID_GREY)), true);
                if (_config->SpeechWidget()){
                   __pList->SetItemChecked(0, 2, true);
                }else{
                   __pList->SetItemChecked(0, 2, false);
                }
                break;

            default:
                pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_ONOFF_SLIDING);
                break;
            }
            break;
            case 1:
            switch (itemIndex % 8){

            case 0:
              pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_ONOFF_SLIDING);
                pStr = new String (_("Widget in LockScreen"));
                pItem->AddElement(Tizen::Graphics::Rectangle(16, 32, 700, 50), 0, *pStr, 36,
                                  Tizen::Graphics::Color(Tizen::Graphics::Color::GetColor(COLOR_ID_GREY)),
                                  Tizen::Graphics::Color(Tizen::Graphics::Color::GetColor(COLOR_ID_GREY)),
                                  Tizen::Graphics::Color(Tizen::Graphics::Color::GetColor(COLOR_ID_GREY)), true);
                if (_config->Lockscreen()){
                   __pList->SetItemChecked(0, 0, true);
                   __pSliderX->SetShowState(true);
                   __pSliderY->SetShowState(true);
                }else{
                   __pList->SetItemChecked(0, 0, false);
                   __pSliderX->SetShowState(false);
                   __pSliderY->SetShowState(false);
                }
                break;

            default:
                break;
            }
            break;

    }

	return pItem;
}

int
meecastAppereanceForm::GetGroupCount(void)
{
	return 2;
}

int
meecastAppereanceForm::GetItemCount(int groupIndex)
{
	int itemCount = 0;
	switch(groupIndex)
	{
	case 0:
		itemCount = 3;
		break;
	case 1:
		itemCount = 1;
		break;

	default:
		break;
	}
	return itemCount;
}


bool
meecastAppereanceForm::DeleteItem(int groupIndex, int itemIndex, Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

bool
meecastAppereanceForm::DeleteGroupItem(int groupIndex, Controls::GroupItem* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

GroupItem*
meecastAppereanceForm::CreateGroupItem(int groupIndex, int itemWidth)
{
	GroupItem* pItem = new (std::nothrow) GroupItem();

	pItem->Construct(Tizen::Graphics::Dimension(GetClientAreaBounds().width, 80));
	String text(L"");
	switch (groupIndex)
	{
        case 0:
            text = _("Widget style and Iconset");
            text.Append(" ");
            text.Append(_("(Home screen)"));
            break;

        case 1:
            text = _("Settings for Lockscreen widget");
            break;
	}
	pItem->SetElement(text, null);
	return pItem;
}


void
meecastAppereanceForm::OnGroupedListViewItemStateChanged(GroupedListView& listView, int groupIndex, int itemIndex, int elementId, ListItemStatus state)
{

    MessageBox messageBox;
    int modalResult = 0;
    if (state == LIST_ITEM_STATUS_CHECKED){
/*        AppLog(" LIST_ITEM_STATUS_CHECKED"); */
        switch (groupIndex){
            case 0:
                switch (itemIndex % 8){
                    case 2:
                        _config->SpeechWidget(true);
                        _config->saveConfig();
                        messageBox.Construct(_("Warning"), _("This is experimental feature. Only English is supported."), MSGBOX_STYLE_OK, 30000);
                        messageBox.ShowAndWait(modalResult);
                        break;
                    default:
                        break;
                }
                break;
            case 1:
                switch (itemIndex % 8){
                    case 0:
                       _config->Lockscreen(true);
                       __pSliderX->SetShowState(true);
                       __pSliderY->SetShowState(true);
                       _config->saveConfig();
                       break;
                    default:
                        break;
                }
                break;
        }
    } 
    if (state == LIST_ITEM_STATUS_UNCHECKED){
        AppLog(" LIST_ITEM_STATUS_UNCHECKED");
        switch (groupIndex){
            case 0:
                switch (itemIndex % 8){
                    case 2:
                        _config->SpeechWidget(false);
                        _config->saveConfig();
                        break;
                    default:
                        break;
                }
                break;
            case 1:
                switch (itemIndex % 8){
                case 0:
                   _config->Lockscreen(false);
                   __pSliderX->SetShowState(false);
                   __pSliderY->SetShowState(false);
                   _config->saveConfig();
                   break;

                default:
                    break;
                }
                break;
       }
    } 

	if (state == LIST_ITEM_STATUS_SELECTED || state == LIST_ITEM_STATUS_MORE){
        SceneManager* pSceneManager = SceneManager::GetInstance();
        AppAssert(pSceneManager);
	    AppLog("LIST_ITEM_STATUS_SELECTED ");
        switch (groupIndex){
            case 0:
                switch (itemIndex % 8){
                case 0:
                   pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_ICONSETSSCENE"), null);
                   break;
                case 1:
                   pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_MODSSCENE"), null);
                   break;

                default:
                    break;
                }
                break;
       }
	}
    __pList->UpdateList();
    App* pApp = App::GetInstance();
    pApp->SendUserEvent(SEND_RELOAD_CONFIG, null);
}


// IAdjustmentEventListener implementation
void
meecastAppereanceForm::OnAdjustmentValueChanged(const Control& source, int adjustment){
     // ....

    _config->Xleft_corner_of_lockscreen_widget(__pSliderX->GetValue());
    _config->Yleft_corner_of_lockscreen_widget(__pSliderY->GetValue());
    _config->saveConfig();
    App* pApp = App::GetInstance();
    pApp->SendUserEvent(SEND_RELOAD_CONFIG, null);
}
