/**
 * Name        : MeecastDynamicBoxAppProviderFactory
 * Version     :
 * Vendor      :
 * Description :
 */

#include "MeecastDynamicBoxAppProvider.h"
#include "MeecastDynamicBoxAppProviderFactory.h"

using namespace Tizen::Base;
using namespace Tizen::Shell;

MeecastDynamicBoxAppProviderFactory::MeecastDynamicBoxAppProviderFactory()
{

}

MeecastDynamicBoxAppProviderFactory::~MeecastDynamicBoxAppProviderFactory()
{

}

AppWidgetProvider*
MeecastDynamicBoxAppProviderFactory::CreateInstance(const String& name, float width, float height, const String& userInfo)
{
	// TODO:
	// Create the AppWidgetProvider instance through the constructor.
    _mdbaprovider = new MeecastDynamicBoxAppProvider();
	return _mdbaprovider;
}
void
MeecastDynamicBoxAppProviderFactory::Update(){
    _mdbaprovider->Update();
}
