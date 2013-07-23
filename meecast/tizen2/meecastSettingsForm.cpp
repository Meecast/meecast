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
#include "meecastSettingsForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

static const int LIST_HEIGHT = 112;
static const int BUTTON_HEIGHT = 74;

meecastSettingsForm::meecastSettingsForm(void)
                    : __pListView(null)
{
}

meecastSettingsForm::~meecastSettingsForm(void)
{
}

bool
meecastSettingsForm::Initialize(void)
{
    Construct(L"SETTINGS_FORM");
    return true;
}

result
meecastSettingsForm::OnInitializing(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your initialization code here

    //CreateContextMenuList();
    // Setup back event listener
    SetFormBackEventListener(this);

    Header* pHeader = GetHeader();
    pHeader->SetTitleText(_("Settings"));

    // Creates an instance of ListView
    __pListView = new ListView();
    __pListView->Construct(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, GetClientAreaBounds().height), true, false);
    __pListView->SetItemProvider(*this);
    __pListView->AddListViewItemEventListener(*this);

    // Adds the list view to the form
    AddControl(*__pListView);

    // Get a button via resource ID
//    Tizen::Ui::Controls::Button *pButtonOk = static_cast<Button*>(GetControl(L"IDC_BUTTON_OK"));
//    if (pButtonOk != null)
//    {
//        pButtonOk->SetActionId(ID_BUTTON_OK);
//        pButtonOk->AddActionEventListener(*this);
//    }
    /* Footer */
  //  Footer* pFooter = GetFooter();
  //  pFooter->SetStyle(FOOTER_STYLE_BUTTON_ICON_TEXT);

  //  FooterItem menuButton;
   // menuButton.Construct(ID_BUTTON_MENU);
  //  menuButton.SetText("Menu");
  //  pFooter->AddItem(menuButton);
  //  pFooter->AddActionEventListener(*this);

    return r;
}

result
meecastSettingsForm::OnTerminating(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastSettingsForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
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
meecastSettingsForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

void
meecastSettingsForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    // TODO:
    // Add your scene activate code here
    AppLog("OnSceneActivatedN Settings");
}

void
meecastSettingsForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}

int
meecastSettingsForm::GetItemCount(void)
{
    return 2;
}

bool
meecastSettingsForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}


Tizen::Ui::Controls::ListItemBase*
meecastSettingsForm::CreateItem (int index, int itemWidth)
{
	SimpleItem* pItem = new SimpleItem();
	AppAssert(pItem);

    pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_DETAILED);

	String listItemString;
	String subject = "";
    switch (index){
        case 0: subject = _("Manage locations"); break;
        case 1: subject = _("Measurement units"); break;
    }
	listItemString.Append(subject);
	pItem->SetElement(listItemString);

	return pItem;
}

void
meecastSettingsForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
	if (status == LIST_ITEM_STATUS_SELECTED || status == LIST_ITEM_STATUS_MORE){
        SceneManager* pSceneManager = SceneManager::GetInstance();
        AppAssert(pSceneManager);
	    AppLog("LIST_ITEM_STATUS_SELECTED ");
        if (index == 0){
	        AppLog("i111LIST_ITEM_STATUS_SELECTED ");
            pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_MANAGELOCATIONSSCENE"));
        }
        if (index == 1){
	        AppLog("LIST_ITEM_STATUS_SELECTED ");
            pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_MEASUREMENT_UNITSSCENE"));
        }
	}
}
void
meecastSettingsForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
meecastSettingsForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state)
{
}

void
meecastSettingsForm::OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex)
{
}


