/**
 * Name        : meecast
 * Version     : 
 * Vendor      : 
 * Description : 
 */


#include "meecast.h"
#include "meecastFrame.h"
#include <FLocations.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Locations;
using namespace Tizen::Base::Runtime;



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
    if (config->dp){
        config->dp->DeleteInstance();
    }
    config->DeleteInstance(); 
    xmlCleanupParser();
//    delete config;
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
    AppLog("Before saveConfig()");
    config->saveConfig();
//    save_station((char*)"gismeteo.ru.xml",(char*)"Afghanistan",(char*)"Afghanistan", (char*)"Herat",(char*) "5511");
 //   save_station((char*)"gismeteo.ru",(char*)"Afghanistan",(char*)"Afghanistan", (char*)"Herat",(char*) "5511");
 
    config->ReLoadConfig();
    /*
    if ((config->stationsList().size() > 0) && config->current_station_id() > config->stationsList().size()) 
       config->dp = current_data(config->stationsList().at(config->current_station_id())->fileName());
    else 
        config->dp = NULL;
    */

    /* AppLog ("Lang: %s", getenv ("LANG")); */
    setlocale(LC_ALL, getenv ("LANG"));
    setlocale (LC_TIME, getenv ("LANG"));
    ByteBuffer* pBuf = null;
    String dataPath= App::GetInstance()->GetAppResourcePath() + "locales";
    pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(dataPath);

    textdomain("omweather");
    bindtextdomain("omweather", (const char*)pBuf->GetPointer());
    delete pBuf;
    if (config->Language() != "System"){
        std::string str;
        str = config->Language()+ ".UTF8";
        setlocale (LC_ALL, str.c_str());
        setlocale (LC_MESSAGES, str.c_str());
        setlocale (LC_TIME, str.c_str());
    }
//#if 0
    /* Add GPS station */
    if (config->Gps() && config->getGpsStation() == -1){
        Location location = LocationProvider::GetLastKnownLocation();
        String dbPath;
        dbPath.Append(App::GetInstance()->GetAppResourcePath());
        dbPath.Append("db/openweathermap.org.db");
        if (Database::Exists(dbPath) == true){
            Core::DatabaseSqlite *__db;
            __db = new Core::DatabaseSqlite(dbPath);
            if (__db->open_database() == true){
                std::string country,  region, code, name;
                double latitude, longitude;
                if (!((Double::ToString(location.GetCoordinates().GetLatitude())== "NaN") || 
                    (Double::ToString(location.GetCoordinates().GetLongitude()) == "NaN"))){
                    __db->get_nearest_station(location.GetCoordinates().GetLatitude(), location.GetCoordinates().GetLongitude(), country, region, code,  name, latitude, longitude);
                    if (latitude != INT_MAX && longitude != INT_MAX){
                        String Name;
                        Name = name.c_str();
                        Name.Append(" (GPS)");
                        config->saveStation1("openweathermap.org", String(code.c_str()), Name, String(country.c_str()), String(region.c_str()), true, latitude, longitude);
                    }
                }
            }
            delete __db;
        }
    }
//#endif

    /*
     String title;
     String description;
     result r = E_SUCCESS;
     title = L"http://tizen.org/setting/screen.wallpaper"; 
     r = SettingInfo::GetValue(title, description);
     if(r == E_UNSUPPORTED_OPERATION){
        description = "Unsupported";
     }
     AppLog (" Result %S", description.GetPointer());
*/
	// Uncomment the following statement to listen to the screen on/off events.
	//PowerManager::SetScreenEventListener(*this);

    String repAppId(15);
	repAppId = L"ctLjIIgCCj";

	String serviceName(L".meecastservice");
	AppId serviceId(repAppId+serviceName);

	AppLog("SampleUiApp : Service Id is %ls", serviceId.GetPointer());

	AppManager* pAppManager = AppManager::GetInstance();
    AppLog("SampleUiApp : Checking service status for time out 5 sec...");

	result r = E_FAILURE;
	for (int i=0; i < 5; ++i)
	{
		if (pAppManager->IsRunning(serviceId))
		{
			AppLog("SampleUiApp : Service is ready !!!");
			break;
		}
		else
		{
			AppLog("SampleUiApp : Service is not ready !!! try to launch !!! ");
			r = pAppManager->LaunchApplication(serviceId, null);
			Thread::Sleep(1000);
		}
	}

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
    /*
    if (config->dp){
        config->dp->DeleteInstance();
    }

    */
    AppLog("qqqqqqqqqqqqqqqqqqq1111");
    AppLog("qqqqqqqqqqqqqqqqqqq");
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
