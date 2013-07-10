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
#include "meecastMeasurementUnitsForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

static const int LIST_HEIGHT = 112;
static const int BUTTON_HEIGHT = 74;

meecastMeasurementUnits::meecastMeasurementUnits(void)
                            : __pList(null)
                            , __pItemContext(null)
{
}

meecastMeasurementUnits::~meecastMeasurementUnits(void)
{
}

bool
meecastMeasurementUnits::Initialize(void)
{
    Construct(L"MEASUREMENT_UNITS_FORM");
    return true;
}

result
meecastMeasurementUnits::OnInitializing(void)
{
    result r = E_SUCCESS;
	CreateGroupedListView();

	// Create Custom Element
//	__pCustomGroupedListElement = new (std::nothrow) CustomGroupedListElement();

	__pItemContext = new (std::nothrow) ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(1, L"Button 1");
	__pItemContext->AddElement(2, L"Button 2");


    return r;
}


void
meecastMeasurementUnits::CreateGroupedListView(void)
{
	__pList = static_cast <GroupedListView*> (GetControl("IDC_GROUPEDLISTVIEW"));
	__pList->SetItemProvider(*this);
	__pList->AddGroupedListViewItemEventListener(*this);
	AddControl(*__pList);
}

result
meecastMeasurementUnits::OnTerminating(void)
{
    result r = E_SUCCESS;
	// Deallocate bitmaps
	delete __pItemContext;
    // TODO:
    // Add your termination code here
    return r;
}

void
meecastMeasurementUnits::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
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
meecastMeasurementUnits::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

void
meecastMeasurementUnits::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    // TODO:
    // Add your scene activate code here
    AppLog("OnSceneActivatedN Settings");
}

void
meecastMeasurementUnits::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}


ListItemBase*
meecastMeasurementUnits::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{
//	ListAnnexStyle style =  LIST_ANNEX_STYLE_NORMAL;
	ListAnnexStyle style =  LIST_ANNEX_STYLE_RADIO;
	CustomItem* pItem = new (std::nothrow) CustomItem();
	pItem->Construct(Tizen::Graphics::Dimension(GetClientAreaBounds().width, 90),style);

    Tizen::Graphics::FloatRectangle firstElementRect(26.0f, 27.0f, 60.0f, 60.0f);
	Tizen::Graphics::FloatRectangle secondElementRect(150.0f, 32.0f, 150.0f, 50.0f);
    switch (groupIndex){
        case 0:
            switch (itemIndex % 4){
            case 0:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"Celsius", true);
                break;
            case 1:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"Fahrenheit", true);
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (itemIndex % 4){
            case 0:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"m/s", true);
                break;
            case 1:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"km/h", true);
                break;
            case 2:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"mi/h", true);
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (itemIndex % 4){
            case 0:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"mbar", true);
                break;
            case 1:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"Pa", true);
                break;
            case 2:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"mmHg", true);
                break;
            default:
                break;
            }
            break;
        case 3:
            switch (itemIndex % 4){
            case 0:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"m", true);
                break;
            case 1:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"km", true);
                break;
            case 2:
                pItem->AddElement(Tizen::Graphics::Rectangle(0, 0, GetClientAreaBounds().width, 100), 1, L"mi", true);
                break;
            default:
                break;
            }
            break;
    }
//	pItem->SetContextItem(__pItemContext);

	return pItem;
}

int
meecastMeasurementUnits::GetGroupCount(void)
{
	return 4;
}

int
meecastMeasurementUnits::GetItemCount(int groupIndex)
{
	int itemCount = 0;
	switch(groupIndex)
	{
	case 0:
		itemCount = 2;
		break;
	case 1:
		itemCount = 3;
		break;
	case 2:
		itemCount = 3;
		break;
	case 3:
		itemCount = 3;
		break;
	default:
		break;
	}
	return itemCount;
}


bool
meecastMeasurementUnits::DeleteItem(int groupIndex, int itemIndex, Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

bool
meecastMeasurementUnits::DeleteGroupItem(int groupIndex, Controls::GroupItem* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

GroupItem*
meecastMeasurementUnits::CreateGroupItem(int groupIndex, int itemWidth)
{
	GroupItem* pItem = new (std::nothrow) GroupItem();

	pItem->Construct(Tizen::Graphics::Dimension(GetClientAreaBounds().width, 80));
	String text(L"");
	switch (groupIndex)
	{
	case 0:
		text = L"Temperature units";
		break;
	case 1:
		 text = L"Wind speed units";
		break;
	case 2:
		 text = L"Pressure units";
		break;
	case 3:
		 text = L"Visible units";
		break;
	}
	pItem->SetElement(text, null);
	return pItem;
}


void
meecastMeasurementUnits::OnGroupedListViewItemStateChanged(GroupedListView& listView, int groupIndex, int itemIndex, int elementId, ListItemStatus state)
{
    /*
    if (state == LIST_ITEM_STATUS_SELECTED && __requestId == FormFactory::REQUEST_SCAN_RESULT_DEVICE_LIST_FORM)
    {
    	WifiDirectDeviceInfo* pDeviceInfo = null;

    	if (groupIndex == INDEX_GROUP_OWNER)
    	{
    		pDeviceInfo = new WifiDirectDeviceInfo(*static_cast<WifiDirectDeviceInfo*>(__groupOwnerInfoList.GetAt(itemIndex)));
    	}
    	else
		{
    		pDeviceInfo = new WifiDirectDeviceInfo(*static_cast<WifiDirectDeviceInfo*>(__othersInfoList.GetAt(itemIndex)));
		}

        ArrayList* pArgs = new ArrayList(SingleObjectDeleter);
        pArgs->Add(*pDeviceInfo);

        SceneManager* pSceneManager = SceneManager::GetInstance();
        AppAssert(pSceneManager);
        pSceneManager->GoBackward(BackwardSceneTransition(), pArgs);
    }

	if (state == LIST_ITEM_STATUS_CHECKED)
	{
		// uncheck the previously selected item
		__pGroupedListView->SetItemChecked(__selectedGroupIndex, __selectedItemIndex, false);
		__pGroupedListView->RequestRedraw();
		__selectedGroupIndex = groupIndex;
		__selectedItemIndex = itemIndex;
	}
	else if (state == LIST_ITEM_STATUS_UNCHECKED)
	{
		__selectedGroupIndex = -1;
		__selectedItemIndex = -1;
	}
    */
}


