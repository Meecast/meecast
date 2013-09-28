/**
 * Name        : MeecastDynamicBoxAppApp
 * Version     :
 * Vendor      :
 * Description :
 */

#include <unique_ptr.h>

#include "MeecastDynamicBoxApp.h"
#include "MeecastDynamicBoxAppProviderFactory.h"
#include "MeecastDynamicBoxAppPopupProviderFactory.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::System;
using namespace Tizen::Graphics;
using namespace Tizen::Shell;


//STUBS
int convert_station_yrno_data(char const* a, char const* b, char const* c){return 0;}
int convert_station_bomgovau_data(char const* a, char const* b, char const* c){ return 0;}
int convert_station_hkogovhk_data(char const* a, char const* b, char const* c){return 0;}
int convert_station_gismeteo_data(char const* a, char const* b, char const* c){return 0;}
int convert_station_forecacom_data(char const* a, char const* b, char const* c){return 0;}
int convert_station_openweathermaporg_data(char const* a, char const* b, char const* c, char const* d){return 0;}




MeecastDynamicBoxAppApp::MeecastDynamicBoxAppApp(void)
{
}

MeecastDynamicBoxAppApp::~MeecastDynamicBoxAppApp(void)
{
}

ServiceApp*
MeecastDynamicBoxAppApp::CreateInstance(void)
{
	return new MeecastDynamicBoxAppApp();
}

bool
MeecastDynamicBoxAppApp::OnAppInitializing(AppRegistry& appRegistry)
{
	AppWidgetProviderManager* pAppWidgetProviderManager = AppWidgetProviderManager::GetInstance();
	AppAssertf(pAppWidgetProviderManager, "AppWidgetProviderManager creation failed.");

    _mdbaProviderFactory = new MeecastDynamicBoxAppProviderFactory;
//	pAppWidgetProviderManager->SetAppWidgetProviderFactory(*(new MeecastDynamicBoxAppProviderFactory));
	pAppWidgetProviderManager->SetAppWidgetProviderFactory(*(_mdbaProviderFactory));
	pAppWidgetProviderManager->SetAppWidgetPopupProviderFactory(*(new MeecastDynamicBoxAppPopupProviderFactory));
  	return true;
}

bool
MeecastDynamicBoxAppApp::OnAppInitialized(void)
{
	return true;
}

bool
MeecastDynamicBoxAppApp::OnAppWillTerminate(void)
{
	return true;
}

bool
MeecastDynamicBoxAppApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO:
	// Deallocate resources allocated by this App for termination.
	// The App's permanent data and context can be saved via appRegistry.

	// TODO: Add your termination code here

	return true;
}

void
MeecastDynamicBoxAppApp::OnLowMemory(void)
{
	// TODO:
	// Free unused resources or close the App.
}

void
MeecastDynamicBoxAppApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(mp3 etc.) if the battery level is CRITICAL.
}

void
MeecastDynamicBoxAppApp::OnUserEventReceivedN(RequestId requestId, IList* pArgs)
{
	AppLog("MeeCastDynamicBox::OnUserEventReceivedN is called. requestId is %d", requestId);

	switch (requestId)
	{
	case RELOAD_CONFIG :
//        OnAppWidgetUpdate();
        _mdbaProviderFactory->Update();
		break;
	default:
		break;
	}
}
