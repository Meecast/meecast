#ifndef _MeecastDynamicBoxApp_PROVIDER_APP_H_
#define _MeecastDynamicBoxApp_PROVIDER_APP_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>

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
};

#endif // _MeecastDynamicBoxApp_PROVIDER_APP_H_
