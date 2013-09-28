/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen
 *
 * Copyright (C) 2012 - 2013 Vlad Vasilyeu
 * 	for the code
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
#ifndef _MeecastDynamicBoxApp_PROVIDER_APP_H_
#define _MeecastDynamicBoxApp_PROVIDER_APP_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include "MeecastDynamicType.h"
#include "MeecastDynamicBoxAppProviderFactory.h"

/**
 * [MeecastDynamicBoxAppApp] ServiceApp must inherit from ServiceApp class
 * which provides basic features necessary to define an ServiceApp.
 */
class MeecastDynamicBoxAppApp
	: public Tizen::App::ServiceApp
{
public:

	/**
	 * [MeecastDynamicBoxAppApp] ServiceApp must have a factory method that creates an instance of itself.
	 */
	static Tizen::App::ServiceApp* CreateInstance(void);

public:

	MeecastDynamicBoxAppApp();
	~MeecastDynamicBoxAppApp();

private:
    MeecastDynamicBoxAppProviderFactory* _mdbaProviderFactory;
public:

	// Called when the ServiceApp is initializing.
	bool OnAppInitializing(Tizen::App::AppRegistry& appRegistry);

	// Called when the ServiceApp initializing is finished.
	bool OnAppInitialized(void); 

	// Called when the ServiceApp is requested to terminate.
	bool OnAppWillTerminate(void);

	// Called when the ServiceApp is terminating.
	bool OnAppTerminating(Tizen::App::AppRegistry& appRegistry, bool forcedTermination = false);

	// Called when the system memory is not sufficient to run the ServiceApp any further.
	void OnLowMemory(void);

	// Called when the battery level changes.
	void OnBatteryLevelChanged(Tizen::System::BatteryLevel batteryLevel);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
};

#endif // _MeecastDynamicBoxApp_PROVIDER_APP_H_
