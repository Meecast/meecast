/**
 * Name        : MeecastServiceApp
 * Version     : 
 * Vendor      : 
 * Description : 
 */

#include "MeecastService.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::System;
static const wchar_t* LOCAL_MESSAGE_PORT_NAME = L"SERVICE_PORT";

MeecastServiceApp::MeecastServiceApp(void)
{
}

MeecastServiceApp::~MeecastServiceApp(void)
{
}

ServiceApp*
MeecastServiceApp::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new MeecastServiceApp();
}

bool
MeecastServiceApp::OnAppInitializing(AppRegistry& appRegistry)
{
	// TODO:
	// Initialize App specific data.
	// The App's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the App will be terminated.

	// TODO: Add your initialization code here
	// Initialize ServerChannel
	__pMessagePort = new (std::nothrow) MeecastServiceMessagePort();
	TryReturn(__pMessagePort != null, false, "MeeCastService : [E_FAILURE] Failed to create __pMessagePort.");
	AppLog("MeeCastService : __pMessagePort is created.");

    result r = E_SUCCESS;
	r = __pMessagePort->Construct(LOCAL_MESSAGE_PORT_NAME);
	TryReturn(IsFailed(r) != true, r, "MeeCastService : [%s] Failed to construct __pMessagePort", GetErrorMessage(r));
	AppLog("MeeCastService : __pMessagePort is constructed.");


	return true;
}

bool
MeecastServiceApp::OnAppInitialized(void)
{
	// TODO:
	// Comment.

	return true;
}

bool
MeecastServiceApp::OnAppWillTerminate(void)
{
	// TODO:
	// Comment.

	return true;
}

bool
MeecastServiceApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO:
	// Deallocate resources allocated by this App for termination.
	// The App's permanent data and context can be saved via appRegistry.

	// TODO: Add your termination code here

	return true;
}

void
MeecastServiceApp::OnLowMemory(void)
{
	// TODO:
	// Free unused resources or close the App.
}

void
MeecastServiceApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}
