#ifndef _meecast_H_
#define _meecast_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include "configtizen.h"

/**
 * [meecast] UiApp must inherit from UiApp class
 * which provides basic features necessary to define an UiApp.
 */
class meecastApp
	: public Tizen::App::UiApp
	, public Tizen::System::IScreenEventListener
{
public:
	/**
	 * [Test] UiApp must have a factory method that creates an instance of itself.
	 */
	static Tizen::App::UiApp* CreateInstance(void);

public:
	meecastApp(void);
	virtual ~meecastApp(void);
    ConfigTizen *config;

public:
	// Called when the UiApp is initializing.
	virtual bool OnAppInitializing(Tizen::App::AppRegistry& appRegistry);

	// Called when the UiApp initializing is finished. 
	virtual bool OnAppInitialized(void); 

	// Called when the UiApp is requested to terminate. 
	virtual bool OnAppWillTerminate(void); 

	// Called when the UiApp is terminating.
	virtual bool OnAppTerminating(Tizen::App::AppRegistry& appRegistry, bool forcedTermination = false);

	// Called when the UiApp's frame moves to the top of the screen.
	virtual void OnForeground(void);

	// Called when this UiApp's frame is moved from top of the screen to the background.
	virtual void OnBackground(void);

	// Called when the system memory is not sufficient to run the UiApp any further.
	virtual void OnLowMemory(void);

	// Called when the battery level changes.
	virtual void OnBatteryLevelChanged(Tizen::System::BatteryLevel batteryLevel);

	// Called when the screen turns on.
	virtual void OnScreenOn(void);

	// Called when the screen turns off.
	virtual void OnScreenOff(void);
};

#endif // _meecast_H_
