/**
 * Name        : meecast
 * Version     : 
 * Vendor      : 
 * Description : 
 */


#include "meecast.h"
#include "meecastFrame.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;




meecastApp::meecastApp(void)
{
}

meecastApp::~meecastApp(void)
{
}

UiApp*
meecastApp::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new meecastApp();
}

bool
meecastApp::OnAppInitializing(AppRegistry& appRegistry)
{
	// TODO:
	// Initialize App specific data.
	// The App's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the App will be terminated.

    AppLogDebug("Create Config class: ");

    try{
        config = ConfigTizen::Instance(Core::AbstractConfig::getConfigPath()+
                                       "config.xml",
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");
    }
    catch(const std::string &str){
        AppLogDebug("Create Config class:  ");
        config =  ConfigTizen::Instance();
    }
    catch(const char *str){
        AppLogDebug("Create Config class:  ");
        config =  ConfigTizen::Instance();
    }
    config->saveConfig();
//    save_station((char*)"gismeteo.ru.xml",(char*)"Afghanistan",(char*)"Afghanistan", (char*)"Herat",(char*) "5511");
 //   save_station((char*)"gismeteo.ru",(char*)"Afghanistan",(char*)"Afghanistan", (char*)"Herat",(char*) "5511");
 



	// Uncomment the following statement to listen to the screen on/off events.
	//PowerManager::SetScreenEventListener(*this);

	// Create a Frame
	meecastFrame* pmeecastFrame = new meecastFrame;
	pmeecastFrame->Construct();
	pmeecastFrame->SetName(L"meecast");
	AddFrame(*pmeecastFrame);

	// TODO: Add your initialization code here
	return true;
}

bool
meecastApp::OnAppInitialized(void)
{
	// TODO:
	// Comment.
    //
   // AppLogDebug("Create Config class1: ");
	return true;
}

bool
meecastApp::OnAppWillTerminate(void)
{
	// TODO:
	// Deallocate or release resources that are UI dependent.
	return true;
}

bool
meecastApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO:
	// Deallocate resources allocated by this App for termination.
	// The App's permanent data and context can be saved via appRegistry.
	return true;
}

void
meecastApp::OnForeground(void)
{
	// TODO:
	// Start or resume drawing when the application is moved to the foreground.
}

void
meecastApp::OnBackground(void)
{
	// TODO:
	// Stop drawing when the application is moved to the background.
}

void
meecastApp::OnLowMemory(void)
{
	// TODO:
	// Free unused resources or close the application.
}

void
meecastApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}

void
meecastApp::OnScreenOn(void)
{
	// TODO:
	// Get the released resources or resume the operations that were paused or stopped in OnScreenOff().
}

void
meecastApp::OnScreenOff(void)
{
	// TODO:
	// Unless there is a strong reason to do otherwise, release resources (such as 3D, media, and sensors) to allow the device
	// to enter the sleep mode to save the battery.
	// Invoking a lengthy asynchronous method within this listener method can be risky, because it is not guaranteed to invoke a
	// callback before the device enters the sleep mode.
	// Similarly, do not perform lengthy operations in this listener method. Any operation must be a quick one.
}
