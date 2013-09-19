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
