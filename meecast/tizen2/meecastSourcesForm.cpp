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
#include "meecastSourcesForm.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::System;
using namespace Tizen::Io;

static const int LIST_HEIGHT = 112;
static const int BUTTON_HEIGHT = 74;

meecastSourcesForm::meecastSourcesForm(void)
                    : __pListView(null)
{
}

meecastSourcesForm::~meecastSourcesForm(void)
{
	if (__fileList.GetCount() > 0){
		__fileList.RemoveAll(true);
	}
}

bool
meecastSourcesForm::Initialize(void)
{
    Construct(L"SOURCES_FORM");
    return true;
}

result
meecastSourcesForm::OnInitializing(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your initialization code here

    //CreateContextMenuList();
    // Setup back event listener
    SetFormBackEventListener(this);

    Header* pHeader = GetHeader();
    pHeader->SetTitleText(_("Select the weather source"));



    // Creates an instance of ListView
    __pListView = static_cast <ListView*> (GetControl(L"IDC_LISTVIEW"));
    __pListView->SetItemProvider(*this);
    __pListView->AddListViewItemEventListener(*this);
    AppLog ("__pListView %p", __pListView);
    // Adds the list view to the form
    AddControl(*__pListView);
	r = GetFilesList();
	TryReturn(r == E_SUCCESS, r, "Could not get sources files present in socrces directory");

    return r;
}

result
meecastSourcesForm::OnTerminating(void)
{
    result r = E_SUCCESS;

    // TODO:
    // Add your termination code here
    return r;
}

void
meecastSourcesForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
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
meecastSourcesForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

void
meecastSourcesForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                          const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    // TODO:
    // Add your scene activate code here
    AppLog("OnSceneActivatedN Sources");
}

void
meecastSourcesForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                           const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    // TODO:
    // Add your scene deactivate code here
    AppLog("OnSceneDeactivated");
}

int
meecastSourcesForm::GetItemCount(void)
{
    AppLog("Count %i",  __fileList.GetCount());
    return __fileList.GetCount();
}

bool
meecastSourcesForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}


Tizen::Ui::Controls::ListItemBase*
meecastSourcesForm::CreateItem (int index, int itemWidth)
{
    CustomItem* pItem = new (std::nothrow) CustomItem();
    TryReturn(pItem != null, null, "Out of memory");

    pItem->Construct(Tizen::Graphics::Dimension(itemWidth, LIST_HEIGHT), LIST_ANNEX_STYLE_DETAILED);

    String* pStr = dynamic_cast< String* >(__fileList.GetAt(index));
    pItem->AddElement(Tizen::Graphics::Rectangle(26, 32, 600, 50), 0, *pStr, false);
    return pItem;

}

void
meecastSourcesForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
	if (status == LIST_ITEM_STATUS_SELECTED || status == LIST_ITEM_STATUS_MORE){
        SceneManager* pSceneManager = SceneManager::GetInstance();
        AppAssert(pSceneManager);

	    AppLog("LIST_ITEM_STATUS_SELECTED ");
        Tizen::Base::Collection::ArrayList* pList = new Tizen::Base::Collection::ArrayList();
        AppAssert(pList);
        pList->Construct();
        String* pStr = dynamic_cast< String* >(__fileList.GetAt(index));
        pList->Add(*(new String(*pStr)));

        /* Select 'Source location' */
        if (index == 0){
	        AppLog("i111LIST_ITEM_STATUS_SELECTED ");
//            pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_MAINSCENE"));
         }

        pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_COUNTRIESSCENE"), pList);
        /*
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);

		CalTodo* pTodo = static_cast<CalTodo*>(__pTodosList->GetAt(index));
		AppAssert(pTodo);

		ArrayList* pList = new (std::nothrow) ArrayList();
		pList->Construct();
		pList->Add(*new (std::nothrow) Integer(pTodo->GetRecordId()));

		pSceneManager->GoForward(ForwardSceneTransition(SCENE_DETAIL), pList);
        */
	}
}
void
meecastSourcesForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
meecastSourcesForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state)
{
}

void
meecastSourcesForm::OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex)
{
}

result
meecastSourcesForm::GetFilesList(void)
{
	Directory* pDir = null;
	DirEnumerator* pDirEnum = null;
	StringComparer strComparer;
	result r = E_SUCCESS;

    AppLog("meecastSourcesForm::GetFilesList");
	__fileList.Construct();

	pDir = new (std::nothrow) Directory();

	r = pDir->Construct(App::GetInstance()->GetAppResourcePath() + "/sources");

	TryCatch(r == E_SUCCESS, delete pDir ,"[%s] Failed to construct directory %S", GetErrorMessage(r), ((App::GetInstance()->GetAppResourcePath() + "/sources").GetPointer()));

	pDirEnum = pDir->ReadN();

	TryCatch(pDirEnum != null, delete pDir ,"[%s] Failed to read entries from directory %S", GetErrorMessage(GetLastResult()), (App::GetInstance()->GetAppResourcePath() + "/sources").GetPointer());

	while (pDirEnum->MoveNext() == E_SUCCESS)
	{
		DirEntry dirEntry = pDirEnum->GetCurrentDirEntry();
		if ((dirEntry.IsDirectory() == false) && (dirEntry.IsHidden() == false))
		{
			String* fullFileName = new (std::nothrow) String;
			//fullFileName->Append(__appRootPath + __dirName);
			//fullFileName->Append('/');
            String fileName(dirEntry.GetName());
            fileName.Remove(fileName.GetLength()-4, 4);
			fullFileName->Append(fileName);
            /* TODO load data from xml */
			__fileList.Add(fullFileName);
		}
	}

//	__fileList.Sort(strComparer);

	delete pDir;
	delete pDirEnum;

	if (__fileList.GetCount() >= 0)
	{
		return E_SUCCESS;
	}
	else
	{
		return E_FAILURE;
	}
CATCH:
	return r;

}

