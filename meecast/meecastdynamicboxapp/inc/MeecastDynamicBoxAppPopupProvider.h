#ifndef _MeecastDynamicBoxApp_POPUP_PROVIDER_H_
#define _MeecastDynamicBoxApp_POPUP_PROVIDER_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>


#include <FShell.h>

/**
 * [MeecastDynamicBoxAppAppWidgetPopupProvider] AppWidget popup provider must inherit from AppWidgetPopupProvider class
 * which provides basic features necessary to define a AppWidget popup.
 */
class MeecastDynamicBoxAppPopupProvider
	: public Tizen::Shell::AppWidgetPopupProvider
{
public:
	MeecastDynamicBoxAppPopupProvider();
	virtual ~MeecastDynamicBoxAppPopupProvider();

protected:
	virtual bool OnAppWidgetPopupProviderInitializing(const Tizen::Base::String& userInfo);
	virtual void OnAppWidgetPopupProviderTerminating(void);
};

#endif /* _MeecastDynamicBoxApp_POPUP_PROVIDER_H_ */
