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
#include "meecastCountriesForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

static const int LIST_HEIGHT = 112;
static const int BUTTON_HEIGHT = 74;

meecastCountriesForm::meecastCountriesForm(void)
                    : __pListView(null)
                    , __pSearchBar(null){
}

meecastCountriesForm::~meecastCountriesForm(void){
    AppLog(" delete meecastCountriesForm");
    __map->RemoveAll(true);
}

bool
meecastCountriesForm::Initialize(void){
    Construct(L"COUNTRIES_FORM");
    return true;
}

result
meecastCountriesForm::OnInitializing(void){
    result r = E_SUCCESS;

    Tizen::Graphics::Rectangle rect = GetClientAreaBounds();
    // TODO:
    // Add your initialization code here

    //CreateContextMenuList();
    // Setup back event listener
    SetFormBackEventListener(this);

    Header* pHeader = GetHeader();
    pHeader->SetTitleText(_("Select country"));


    // Creates an instance of ListView
    __pListView = static_cast <ListView*> (GetControl(L"IDC_LISTVIEW"));
    __pListView->SetItemProvider(*this);
    __pListView->AddListViewItemEventListener(*this);
    __pListView->AddFastScrollListener(*this);

	__pSearchBar = static_cast<SearchBar*>(GetControl("IDC_SEARCHBAR"));
	__pSearchBar->SetText(_("Click here!"));
	__pSearchBar->AddSearchBarEventListener(*this);
	__pSearchBar->AddTextEventListener(*this);
	__pSearchBar->AddKeypadEventListener(*this);

// 	__pSearchBar->SetContent(__pListView);

    // Adds the list view to the form
    AddControl(*__pListView);
    AddControl(*__pSearchBar);
 	FloatRectangle clientRect = GetClientAreaBoundsF();
 	__pListView->SetSize(FloatDimension(clientRect.width, clientRect.height - __pSearchBar->GetHeightF()));


    return r;
}

void
meecastCountriesForm::OnKeypadActionPerformed(Control &source, Tizen::Ui::KeypadAction keypadAction){
	__pSearchBar->HideKeypad();
	Invalidate(true);
}

void
meecastCountriesForm::OnKeypadClosed(Control &source){
	FloatRectangle clientRect = GetClientAreaBoundsF();
	__pSearchBar->SetContentAreaSize(FloatDimension(clientRect.width, clientRect.height - __pSearchBar->GetHeightF()));
	__pListView->SetSize(FloatDimension(clientRect.width, clientRect.height - __pSearchBar->GetHeightF()));
	Invalidate(true);
}

void
meecastCountriesForm::OnKeypadOpened(Control &source)
{
	FloatRectangle clientRect = GetClientAreaBoundsF();
	FloatRectangle searchBarBounds = CoordinateSystem::AlignToDevice(FloatRectangle(__pSearchBar->GetBoundsF()));
	__pSearchBar->SetContentAreaSize(FloatDimension(clientRect.width, clientRect.height - searchBarBounds.height));
	__pListView->SetSize(FloatDimension(clientRect.width, clientRect.height - __pSearchBar->GetHeightF()));
	Invalidate(true);
}


void
meecastCountriesForm::OnKeypadWillOpen(Control &source){
	Invalidate(true);
}

void
meecastCountriesForm::OnKeypadBoundsChanged(Tizen::Ui::Control& source)
{
	FloatRectangle clientRect = GetClientAreaBoundsF();
	FloatRectangle searchBarBounds = CoordinateSystem::AlignToDevice(FloatRectangle(__pSearchBar->GetBoundsF()));
	__pSearchBar->SetContentAreaSize(FloatDimension(clientRect.width, clientRect.height - searchBarBounds.height));
	__pListView->SetSize(FloatDimension(clientRect.width, clientRect.height - __pSearchBar->GetHeightF()));
	__pListView->SetEnabled(false);
	Invalidate(true);
}

void
meecastCountriesForm::OnSearchBarModeChanged(SearchBar& source, SearchBarMode mode){

    AppLog("OnSearchBarModeChanged");
	FloatRectangle clientRect = GetClientAreaBoundsF();
	__pSearchBar->SetText(L"");
	if(mode == SEARCH_BAR_MODE_INPUT)
	{
		__pSearchBar->SetContentAreaSize(FloatDimension(clientRect.width, clientRect.height - __pSearchBar->GetHeightF()));
		__pListView->SetSize(FloatDimension(clientRect.width, clientRect.height - __pSearchBar->GetHeightF()));
	}
	else
	{
		__pSearchBar->SetText(L"Click here!");
		__pListView->SetSize(FloatDimension(clientRect.width, clientRect.height - __pSearchBar->GetHeightF()));
		__pListView->SetEnabled(true);
        LoadList();
		__pListView->UpdateList();

	}
	__pListView->ScrollToItem(0);
	Invalidate(true);
}

void
meecastCountriesForm::OnTextValueChanged(const Control& source){
	UpdateSearchResult();
}


void
meecastCountriesForm::UpdateSearchResult(void){
	String inputText = __pSearchBar->GetText();

	if(inputText.CompareTo(L"") != 0){
        LoadList();
		__pListView->UpdateList();
	}else{
		__pSearchBar->SetText(L"");
        LoadList();
		__pListView->UpdateList();
	}
	Invalidate(true);
}



result
meecastCountriesForm::OnTerminating(void){
    result r = E_SUCCESS;

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastCountriesForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
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
meecastCountriesForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

void
meecastCountriesForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    // TODO:
    // Add your scene activate code here
    AppLog("OnSceneActivatedN ManageLocations");
    if (pArgs != null) {
        __SourceId = *(dynamic_cast<String*> (pArgs->GetAt(0)));
        pArgs->RemoveAll(true);
        delete pArgs;
        __dbPath.Append(App::GetInstance()->GetAppResourcePath());
        __dbPath.Append("db/");
        __dbPath.Append(__SourceId);
        __dbPath.Append(".db");
        LoadList();
    }
}

void
meecastCountriesForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}

int
meecastCountriesForm::GetItemCount(void)
{
    int itemCount = 0;

    IList* pList = __map->GetKeysN();
    AppAssert(pList);
    itemCount = pList->GetCount();
    delete pList;
    return itemCount;
}

bool
meecastCountriesForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}


Tizen::Ui::Controls::ListItemBase*
meecastCountriesForm::CreateItem (int index, int itemWidth)
{
    CustomItem* pItem = new (std::nothrow) CustomItem();
    TryReturn(pItem != null, null, "Out of memory");

    pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_DETAILED);

    String* pStr = dynamic_cast< String* >(__map->GetValue(Integer(index)));
    pItem->AddElement(Tizen::Graphics::Rectangle(26, 32, 600, 50), 0, *pStr, false);
    return pItem;
}

void
meecastCountriesForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
	if (status == LIST_ITEM_STATUS_SELECTED || status == LIST_ITEM_STATUS_MORE){
        SceneManager* pSceneManager = SceneManager::GetInstance();
        AppAssert(pSceneManager);

        Tizen::Base::Collection::ArrayList* pList = new Tizen::Base::Collection::ArrayList();
        AppAssert(pList);
        pList->Construct();
        pList->Add(*__db);

        String* tempS = new String("");
        tempS->Append(__SourceId);
        String* pStr0 = dynamic_cast< String* >(tempS);
        pList->Add(*(new String(*pStr0)));
        delete tempS;

        String* pStr = dynamic_cast< String* >(__map->GetValue(Integer(index)));
        pList->Add(*(new String(*pStr)));
        
        pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_REGIONSSCENE"), pList);
	    AppLog("LIST_ITEM_STATUS_SELECTED ");
	}
}
void
meecastCountriesForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
meecastCountriesForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state)
{
}

void
meecastCountriesForm::OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex)
{
}

bool
meecastCountriesForm::LoadList(void){

    AppLog("meecastCountriesForm::LoadList");

    if (Database::Exists(__dbPath) == true){
        AppLog("LoadList DB success");
    }else{
        AppLog("LoadList DB not success");
        return false;
    }
    String inputText;
    if (__pSearchBar)
	    inputText = __pSearchBar->GetText();
    else
	    inputText = "";
    __db = new Core::DatabaseSqlite(__dbPath);
    if (__db->open_database() == true){
        __map = __db->create_countries_list();
        String* pValue = null;
	    if(inputText != "" && inputText != _("Click here!")){
            int size_of_array = __map->GetCount();
            for(int i = 0; i < size_of_array ; i ++){
                pValue = static_cast< String* > (__map->GetValue(Integer(i)));
                result r = E_SUCCESS;
	            int Indexof = -1;
                r = pValue->IndexOf(inputText, 0, Indexof);
                if(Indexof < 0)
                    __map->Remove((Integer(i)));
            }
            Tizen::Base::Collection::HashMap *map; 
            map = new Tizen::Base::Collection::HashMap;
            map->Construct();
            int j = 0;
            for(int i = 0; i < size_of_array; i ++){
                pValue = static_cast< String* > (__map->GetValue(Integer(i)));
                if (pValue){
                    map->Add(*(new (std::nothrow) Integer(j++)), *(new (std::nothrow) String(pValue->GetPointer())));
                }
            }
            __map->RemoveAll();
            delete __map;
            __map = map;
        }
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
    }else{
        AppLog("Open DB not success");
        return false;
    }
}
void
meecastCountriesForm::OnFastScrollIndexSelected(Control& source, Tizen::Base::String& index)
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

void
meecastCountriesForm::OnTouchPressed (const Control& source, const Point& currentPosition, const TouchEventInfo &touchInfo) {
    Invalidate(true);
}
