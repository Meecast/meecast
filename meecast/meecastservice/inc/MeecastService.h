#ifndef _MeecastService_H_
#define _MeecastService_H_


#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include "MeecastServiceMessagePort.h"

/**
 * [MeecastServiceApp] ServiceApp must inherit from ServiceApp class
 * which provides basic features necessary to define an ServiceApp.
 */
class MeecastServiceApp
	: public Tizen::App::ServiceApp
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

#endif // _MeecastService_H_
