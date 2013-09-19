/**
 * Name        : MeecastDynamicBoxAppPopupProviderFactory
 * Version     :
 * Vendor      :
 * Description :
 */

#include "MeecastDynamicBoxAppPopupProvider.h"
#include "MeecastDynamicBoxAppPopupProviderFactory.h"


using namespace Tizen::Base;
using namespace Tizen::Shell;

MeecastDynamicBoxAppPopupProviderFactory::MeecastDynamicBoxAppPopupProviderFactory()
{

}

MeecastDynamicBoxAppPopupProviderFactory::~MeecastDynamicBoxAppPopupProviderFactory()
{

}

AppWidgetPopupProvider*
MeecastDynamicBoxAppPopupProviderFactory::CreateInstance(const String& name, const String& userInfo)
{
	// TODO:
	// Create the AppWidgetPopupProvider instance through the constructor.
	return new MeecastDynamicBoxAppPopupProvider();
}
