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
#include "meecastManageLocationsForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

static const int LIST_HEIGHT = 112;
static const int BUTTON_HEIGHT = 74;

meecastManageLocationsForm::meecastManageLocationsForm(void)
                    : __pListView(null)
{
}

meecastManageLocationsForm::~meecastManageLocationsForm(void)
{
}

bool
meecastManageLocationsForm::Initialize(void)
{
    Construct(L"MANAGE_LOCATIONS_FORM");
    return true;
}

result
meecastManageLocationsForm::OnInitializing(void)
{
    result r = E_SUCCESS;

    _config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");

    // Setup back event listener
    SetFormBackEventListener(this);



    // Creates an instance of ListView
    __pListView = static_cast <ListView*> (GetControl(L"IDC_LISTVIEW"));
    __pListView->SetItemProvider(*this);
    __pListView->AddListViewItemEventListener(*this);
    __pListView->SetTextOfEmptyList(_("No locations are set up yet."));

    // Adds the list view to the form
    AddControl(*__pListView);

	GetStationsList();
    /* Footer */
    Footer* pFooter = GetFooter();
    pFooter->AddActionEventListener(*this);

    return r;
}

result
meecastManageLocationsForm::OnTerminating(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastManageLocationsForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);

    AppLog("Check Action");
            
    switch(actionId)
    {
    case 0:  
        pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_SOURCESSCENE"));
        break;
    default:
        break;
    }
}

void
meecastManageLocationsForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

void
meecastManageLocationsForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    // TODO:
    // Add your scene activate code here
    AppLog("OnSceneActivatedN ManageLocations");
}

void
meecastManageLocationsForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}

int
meecastManageLocationsForm::GetItemCount(void)
{
    return _config->stationsList().size();
}

bool
meecastManageLocationsForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}


Tizen::Ui::Controls::ListItemBase*
meecastManageLocationsForm::CreateItem (int index, int itemWidth)
{
    CustomItem* pItem = new (std::nothrow) CustomItem();
    TryReturn(pItem != null, null, "Out of memory");

//    pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_DETAILED);
    pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_ONOFF_SLIDING);
    String* pStr = new String (_config->stationsList().at(index)->name().c_str()); 
    pItem->AddElement(Tizen::Graphics::Rectangle(26, 32, 600, 50), 0, *pStr, false);

    __pListView->SetItemChecked(index, true);
	return pItem;
}


void
meecastManageLocationsForm::DeleteMessageBox(const Tizen::Base::String& Station, int index)
{
    MessageBox messageBox;
    messageBox.Construct(Station, L"Delete location?", MSGBOX_STYLE_YESNO, 30000);

    int modalResult = 0;

    // Calls ShowAndWait() : Draws and Shows itself and processes events
    messageBox.ShowAndWait(modalResult);
    switch (modalResult) {
        case MSGBOX_RESULT_YES: {
            _config->removeStation(index);
        }
        break;
        default:
            break;
    }
}
                                                                                     


void
meecastManageLocationsForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
    if (status ==  LIST_ITEM_STATUS_UNCHECKED){
        DeleteMessageBox(_config->stationsList().at(index)->name().c_str(), index);
        listView.UpdateList();
   	}
}
void
meecastManageLocationsForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
meecastManageLocationsForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state)
{
}

void
meecastManageLocationsForm::OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex)
{
}

void
meecastManageLocationsForm::GetStationsList(void)
{
}



