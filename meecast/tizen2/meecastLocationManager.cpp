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
#include <FBase.h>
#include <FLocations.h>
#include "meecastLocationManager.h"
#include "meecastMainForm.h"
#include "meecastManageLocationsForm.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;
using namespace Tizen::Locations;
using namespace Tizen::Ui::Controls;

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
    _config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");


	AppLog("Requested for current location in new thread.");
	result lastResult = E_SUCCESS;
	LocationCriteria locCriteria;

	locCriteria.SetAccuracy(LOC_ACCURACY_ANY);

	Location location = LocationProvider::GetLocation(locCriteria);

    AppLog ("Latitude1 %d",location.GetCoordinates().GetLatitude());
    AppLog ("Longitude1 %d",location.GetCoordinates().GetLongitude());

	lastResult = GetLastResult();

	if (lastResult == E_USER_NOT_CONSENTED)
	{
        AppLog("Problem");
   		__pUiControl->SendUserEvent(meecastMainForm::LOC_MGR_NOTIFY_ERROR, null);
 		
	}
    
	if (lastResult == E_SUCCESS){
            AppLog("Yes!!!");
            AppLog ("Latitude %d",location.GetCoordinates().GetLatitude());
            AppLog ("Longitude %d",location.GetCoordinates().GetLongitude());
            String dbPath;
            dbPath.Append(App::GetInstance()->GetAppResourcePath());
            dbPath.Append("db/openweathermap.org.db");
            if (Database::Exists(dbPath) == true){
                Core::DatabaseSqlite *__db;
                __db = new Core::DatabaseSqlite(dbPath);
                if (__db->open_database() == true){
                    std::string country,  region, code, name;
                    double latitude, longitude;
                    if ((Double::ToString(location.GetCoordinates().GetLatitude())== "NaN") || 
                        (Double::ToString(location.GetCoordinates().GetLongitude()) == "NaN")){
                        int doModal;
                        MessageBox messageBox;
                        messageBox.Construct(L"Error", "Data for GPS is not available", MSGBOX_STYLE_OK, 0);
                        messageBox.ShowAndWait(doModal);
                    }else{ 
                        __db->get_nearest_station(location.GetCoordinates().GetLatitude(), location.GetCoordinates().GetLongitude(), country, region, code,  name, latitude, longitude);
                        if (latitude != INT_MAX && longitude != INT_MAX){
                            /* find exist gps station */
                            int index = _config->getGpsStation();
                            if (index > -1){
                                /* delete gps station */
                                _config->removeStation(index);
                            }
                            String Name;
                            Name = name.c_str();
                            Name.Append(" (GPS)");
                            _config->saveStation1("openweathermap.org", String(code.c_str()), Name, String(country.c_str()), String(region.c_str()), true, latitude, longitude);
                        }
                    }
                }
                delete __db;
            }

       __pUiControl->SendUserEvent(meecastMainForm::LOC_MGR_DRAW_SYNC_LOC_UPDATE, null);
      __pUiControl->SendUserEvent(meecastManageLocationsForm::UPDATE_LIST, null);
    }

	return null;
}
