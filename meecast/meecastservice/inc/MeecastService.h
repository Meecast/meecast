#ifndef _MeecastService_H_
#define _MeecastService_H_


#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FIo.h>
#include <FShell.h>
#include <FMedia.h> 
#include <FGraphics.h>
#include "MeecastDynamicType.h"
#include "MeecastServiceMessagePort.h"
#include "../../core/core.h"
#include "../../core/config.h"
#include <FLocales.h>



/**
 * [MeecastServiceApp] ServiceApp must inherit from ServiceApp class
 * which provides basic features necessary to define an ServiceApp.
 */
class MeecastServiceApp
	: public Tizen::App::ServiceApp
    , public Tizen::Base::Runtime::ITimerEventListener
    , public Tizen::System::ISettingEventListener
{
public:

	/**
	 * [MeecastServiceApp] ServiceApp must have a factory method that creates an instance of itself.
	 */
	static Tizen::App::ServiceApp* CreateInstance(void);

public:

	MeecastServiceApp(void);
	~MeecastServiceApp(void);

private:
    MeecastServiceMessagePort* __pMessagePort;
    Core::Config *_config;
    Core::DataParser* _dp;
    Tizen::Base::Runtime::Timer* __updateTimer;
    Tizen::Base::Runtime::Timer* __checkupdatingTimer;
    Tizen::Graphics::Bitmap * __pBitmapOriginal;

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

    virtual void OnTimerExpired(Tizen::Base::Runtime::Timer& timer);
    void UpdateLockScreen();
//ISettingEventLisetener
    virtual void OnSettingChanged(Tizen::Base::String& key);

};

#endif // _MeecastService_H_
