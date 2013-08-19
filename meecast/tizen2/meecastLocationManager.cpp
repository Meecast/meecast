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
#include <FUi.h>
#include <FLocations.h>
#include "meecastLocationManager.h"
#include "meecastMainForm.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;
using namespace Tizen::Locations;

meecastLocationManager::meecastLocationManager(void)
	: Tizen::Base::Runtime::Thread()
	, __pUiControl(null){
}

meecastLocationManager::~meecastLocationManager(void){
}

result
meecastLocationManager::Construct(const Tizen::Ui::Control& uiControl)
{
	__pUiControl = &uiControl;
	return Thread::Construct();
}

Object*
meecastLocationManager::Run(void)
{
	result lastResult = E_SUCCESS;
	LocationCriteria locCriteria;

	locCriteria.SetAccuracy(LOC_ACCURACY_HUNDRED_METERS);

	Location location = LocationProvider::GetLocation(locCriteria);

	AppLog("Requested for current location in new thread.");

	lastResult = GetLastResult();

	if (lastResult == E_USER_NOT_CONSENTED)
	{
		__pUiControl->SendUserEvent(meecastMainForm::LOC_MGR_NOTIFY_ERROR, null);
	}

	ArrayList* pList = new (std::nothrow) ArrayList();
	Location* pLocation = new (std::nothrow) Location(location);

	pList->Construct();
	pList->Add(*pLocation);
	__pUiControl->SendUserEvent(meecastMainForm::LOC_MGR_DRAW_SYNC_LOC_UPDATE, pList);

	return null;
}
