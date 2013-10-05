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
using namespace Core;
using namespace Tizen::Base::Collection;
static const wchar_t* LOCAL_MESSAGE_PORT_NAME = L"SERVICE_PORT";

//STUBS
int convert_station_yrno_data(char const* a, char const* b, char const* c){return 0;}
int convert_station_bomgovau_data(char const* a, char const* b, char const* c){ return 0;}
int convert_station_hkogovhk_data(char const* a, char const* b, char const* c){return 0;}
int convert_station_gismeteo_data(char const* a, char const* b, char const* c){return 0;}
int convert_station_forecacom_data(char const* a, char const* b, char const* c){return 0;}
int convert_station_openweathermaporg_data(char const* a, char const* b, char const* c, char const* d){return 0;}




MeecastServiceApp::MeecastServiceApp(void):
                 __updateTimer(null),
                 __checkupdatingTimer(null)
{

    result r = E_SUCCESS;
    __updateTimer = new (std::nothrow) Tizen::Base::Runtime::Timer;
    /* TryReturn(__updateTimer != null, E_FAILURE, "[E_FAILURE] Failed to create __updateTimer."); */
    AppLog("updateTimer is created.");

    r = __updateTimer->Construct(*this);
    /* TryReturn(IsFailed(r) != true, r, "[%s] Failed to construct __pTimer", GetErrorMessage(r)); */
    AppLog("__updateTimer is constructed.");

    __checkupdatingTimer = new (std::nothrow) Tizen::Base::Runtime::Timer;
    /* TryReturn(__checkupdatingTimer != null, E_FAILURE, "[E_FAILURE] Failed to create __updateTimer."); */
    AppLog("__checkupdatingTimer is created.");

    r = __checkupdatingTimer->Construct(*this);
/*    TryReturn(IsFailed(r) != true, r, "[%s] Failed to construct __pTimer", GetErrorMessage(r)); */
    AppLog("__checkupdatingTimer is constructed.");


}

MeecastServiceApp::~MeecastServiceApp(void){
    if (__checkupdatingTimer)
        delete __checkupdatingTimer;
    if (__updateTimer)
        delete __updateTimer;
    _config->DeleteInstance();
}

ServiceApp*
MeecastServiceApp::CreateInstance(void){
	// Create the instance through the constructor.
	return new MeecastServiceApp();
}

bool
MeecastServiceApp::OnAppInitializing(AppRegistry& appRegistry){
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

    char  buffer[4096]; 
    String str;
    try{
        _config = Config::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");
    }
    catch(const std::string &str){
        AppLogDebug("Create Config class:  ");
        _config =  Config::Instance();
    }
    catch(const char *str){
        AppLogDebug("Create Config class:  ");
        _config =  Config::Instance();
    }

    AppLog("Update Period %i", _config->UpdatePeriod());
    if (_config->UpdatePeriod() != INT_MAX){
        __updateTimer->StartAsRepeatable(_config->UpdatePeriod()*1000);
    }

	return true;
}

bool
MeecastServiceApp::OnAppInitialized(void){
	// TODO:
	// Comment.

	return true;
}

bool
MeecastServiceApp::OnAppWillTerminate(void){
	// TODO:
	// Comment.

	return true;
}

bool
MeecastServiceApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination){
	// TODO:
	// Deallocate resources allocated by this App for termination.
	// The App's permanent data and context can be saved via appRegistry.

	// TODO: Add your termination code here

	return true;
}

void
MeecastServiceApp::OnLowMemory(void){
	// TODO:
	// Free unused resources or close the App.
}

void
MeecastServiceApp::OnBatteryLevelChanged(BatteryLevel batteryLevel){
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}


void
MeecastServiceApp::OnUserEventReceivedN(RequestId requestId, IList* pArgs){
	AppLog("MeeCastService::OnUserEventReceivedN is called. requestId is %d", requestId);

	switch (requestId)
	{
	case RELOAD_CONFIG :
//        OnAppWidgetUpdate();
//        _mdbaProviderFactory->Update(); 
        _config->ReLoadConfig();
		break;
	default:
		break;
	}
}

void
MeecastServiceApp::OnTimerExpired(Tizen::Base::Runtime::Timer& timer){
    AppLog("OnTimerExpired");
    for (short i=0; i < _config->stationsList().size();i++){
        _config->stationsList().at(i)->updateData(true);
    }
}


