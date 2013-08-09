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
#include <FIo.h>
#include "meecastStationsForm.h"
#include "configtizen.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

static const int LIST_HEIGHT = 112;
static const int BUTTON_HEIGHT = 74;

meecastStationsForm::meecastStationsForm(void)
                    : __pListView(null)
{
}

meecastStationsForm::~meecastStationsForm(void)
{
    __map->RemoveAll(true);
}

bool
meecastStationsForm::Initialize(void)
{
    Construct(L"STATIONS_FORM");
    return true;
}

result
meecastStationsForm::OnInitializing(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your initialization code here

    //CreateContextMenuList();
    // Setup back event listener
    SetFormBackEventListener(this);

    Header* pHeader = GetHeader();
    pHeader->SetTitleText(_("Select location"));


    // Creates an instance of ListView
    __pListView = static_cast <ListView*> (GetControl(L"IDC_LISTVIEW"));
    __pListView->SetItemProvider(*this);
    __pListView->AddListViewItemEventListener(*this);
    __pListView->AddFastScrollListener(*this);
    __pListView->SetTextOfEmptyList(_("There are no stations to show in this view of the list"));

    // Adds the list view to the form
    AddControl(*__pListView);

    return r;
}

result
meecastStationsForm::OnTerminating(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastStationsForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
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
meecastStationsForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

void
meecastStationsForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    AppLog("OnSceneActivatedN STations");
    if (pArgs != null) {
        __db = static_cast < Core::DatabaseSqlite* > (pArgs->GetAt(0));
        __SourceId = *(dynamic_cast<String*> (pArgs->GetAt(1)));
        __CountryName = *(dynamic_cast<String*> (pArgs->GetAt(2)));
        __RegionName = *(dynamic_cast<String*> (pArgs->GetAt(3)));
        pArgs->RemoveAt(3);
        pArgs->RemoveAt(2);
        pArgs->RemoveAt(1);
        AppLog("SOurceID %S", __SourceId.GetPointer());
        AppLog("Region Name %S", __RegionName.GetPointer());
        delete pArgs;
        LoadList();
    }
}

void
meecastStationsForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}

int
meecastStationsForm::GetItemCount(void)
{
    int itemCount = 0;

    IList* pList = __map->GetKeysN();
    AppAssert(pList);
    itemCount = pList->GetCount();
    delete pList;
    return itemCount;
}

bool
meecastStationsForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}


Tizen::Ui::Controls::ListItemBase*
meecastStationsForm::CreateItem (int index, int itemWidth)
{
    CustomItem* pItem = new (std::nothrow) CustomItem();
    TryReturn(pItem != null, null, "Out of memory");

    pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_NORMAL);

    String* pStr = dynamic_cast< String* >(__map->GetValue(Integer(index)));
    pItem->AddElement(Tizen::Graphics::Rectangle(26, 32, 600, 50), 0, *pStr, false);
    return pItem;
}

void
meecastStationsForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
    ConfigTizen *config;

	if (status == LIST_ITEM_STATUS_SELECTED){
         __StationName = *(dynamic_cast<String*> (__map->GetValue(Integer(index))));
        AppLog("SELECTED %s ", __StationName.GetPointer());

        __StationCode = __db->get_station_code_by_name(__CountryName, __RegionName, __StationName);
        AppLog("S %s ", __StationCode.GetPointer());
        double lat = 0;
        double lon = 0;
        __db->get_station_coordinate(__StationCode, lat, lon);
        config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");
        config->saveStation1(
                __SourceId,
                __StationCode,
                __StationName,
                __CountryName,
                __RegionName,
                false, lat, lon);
        SceneManager* pSceneManager = SceneManager::GetInstance();
        AppAssert(pSceneManager);
        pSceneManager->GoBackward(BackwardSceneTransition(L"MainScene",
                                               SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

   	}
}
void
meecastStationsForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
meecastStationsForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state)
{
}

void
meecastStationsForm::OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex)
{
}

bool
meecastStationsForm::LoadList(void){
     __map = __db->create_stations_list_by_name(__CountryName, __RegionName);
    AppLog("Open DB success");
     String* pValue = null;
     String letter;
     __indexString = "";
	 for(int i = 0; i <  __map->GetCount(); i ++){
        pValue = static_cast< String* > (__map->GetValue(Integer(i)));
        pValue->SubString(0, 1, letter);
        if (!__indexString.Contains(letter))
            __indexString += letter;
     }
	__pListView->SetFastScrollIndex(__indexString, false);

    return true;
}

void
meecastStationsForm::OnFastScrollIndexSelected(Control& source, Tizen::Base::String& index)
{
	String compare(L"");
    String* pValue = null;
	for(int i = 0; i <  __map->GetCount(); i ++){
        pValue = static_cast< String* > (__map->GetValue(Integer(i)));
        pValue->SubString(0,1,compare);
		if(compare.CompareTo(index) == 0){
			__pListView->ScrollToItem(i, LIST_SCROLL_ITEM_ALIGNMENT_TOP);
            break;
		}
	}
    __pListView->Invalidate(false);
}

