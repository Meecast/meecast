#ifndef _MeecastDynamicBoxApp_PROVIDER_FACTORY_H_
#define _MeecastDynamicBoxApp_PROVIDER_FACTORY_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>


#include <FShell.h>

class MeecastDynamicBoxAppProviderFactory
: public Tizen::Shell::IAppWidgetProviderFactory
{
public:
	MeecastDynamicBoxAppProviderFactory();
	virtual ~MeecastDynamicBoxAppProviderFactory();

	virtual Tizen::Shell::AppWidgetProvider* CreateInstance(const Tizen::Base::String& name, float width, float height, const Tizen::Base::String& userInfo);
};

#endif /* _MeecastDynamicBoxApp_PROVIDER_FACTORY_H_ */
