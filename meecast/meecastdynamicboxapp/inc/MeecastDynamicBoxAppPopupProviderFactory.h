#ifndef MeecastDynamicBoxApp_POPUP_PROVIDER_FACTORY_H_
#define MeecastDynamicBoxApp_POPUP_PROVIDER_FACTORY_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>


#include <FShell.h>

class MeecastDynamicBoxAppPopupProviderFactory
: public Tizen::Shell::IAppWidgetPopupProviderFactory
{
public:
	MeecastDynamicBoxAppPopupProviderFactory();
	virtual ~MeecastDynamicBoxAppPopupProviderFactory();

	virtual Tizen::Shell::AppWidgetPopupProvider* CreateInstance(const Tizen::Base::String& name, const Tizen::Base::String& userInfo);
};

#endif /* MeecastDynamicBoxApp_POPUP_PROVIDER_FACTORY_H_ */
