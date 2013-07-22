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



Core::DataParser*
current_data(std::string& str){
  Core::DataParser* dp;
  try{
        //dp = Core::DataParser::Instance(str, DATA_XSD_PATH);
        dp = Core::DataParser::Instance(str, "");
    }
    catch(const std::string &str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return NULL;
    }
    catch(const char *str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return NULL;
    }
    return dp;
}



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
//        ByteBuffer* pBuf = null;
//        String filepath = App::GetInstance()->GetAppDataPath();
//        filepath.Append("config.xml");
 //       pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(filepath);

    //    config = ConfigTizen::Instance( std::string((const char*)pBuf->GetPointer()),
        config = ConfigTizen::Instance( std::string("config.xml"),
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
 
    config->ReLoadConfig();
    if ((config->stationsList().size() > 0) && config->current_station_id() > config->stationsList().size()) 
       config->dp = current_data(config->stationsList().at(config->current_station_id())->fileName());
    else 
        config->dp = NULL;


    ByteBuffer* pBuf = null;
    String dataPath= App::GetInstance()->GetAppResourcePath() + "locales";
    pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(dataPath);

    textdomain("omweather");
    bindtextdomain("omweather", (const char*)pBuf->GetPointer());
    if (config->Language() != "System"){
        std::string str;
        str = config->Language()+ ".UTF8";
        setlocale (LC_ALL, str.c_str());
        setlocale (LC_MESSAGES, str.c_str());
    }

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
