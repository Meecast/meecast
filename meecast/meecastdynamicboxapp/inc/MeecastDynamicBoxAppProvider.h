#ifndef _MeecastDynamicBoxApp_PROVIDER_H_
#define _MeecastDynamicBoxApp_PROVIDER_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>


#include <FShell.h>

/**
 * [MeecastDynamicBoxAppAppWidgetProvider] AppWidget provider must inherit from AppWidgetProvider class
 * which provides basic features necessary to define a AppWidget.
 */
class MeecastDynamicBoxAppProvider
: public Tizen::Shell::AppWidgetProvider
 {
public:
	MeecastDynamicBoxAppProvider();
	virtual ~MeecastDynamicBoxAppProvider();

protected:
	virtual bool OnAppWidgetProviderInitializing(float width, float height, const Tizen::Base::String& userInfo);
	virtual void OnAppWidgetProviderTerminating();
	virtual void OnAppWidgetProviderRemoved();
	virtual bool OnAppWidgetProviderUpdating(const Tizen::Base::String& argument);
};


#endif /* _MeecastDynamicBoxApp_PROVIDER_H_ */
