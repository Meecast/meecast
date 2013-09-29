/**
 * Name        : MeecastDynamicBoxAppProvider
 * Version     :
 * Vendor      :
 * Description :
 */


#include "MeecastDynamicBoxAppFrame.h"
#include "MeecastDynamicBoxAppProvider.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::System;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Shell;

MeecastDynamicBoxAppProvider::MeecastDynamicBoxAppProvider()
{
}

MeecastDynamicBoxAppProvider::~MeecastDynamicBoxAppProvider()
{
}


bool
MeecastDynamicBoxAppProvider::OnAppWidgetProviderInitializing(float width, float height, const Tizen::Base::String& userInfo)
{
	AppLog("%f, %f", width, height);

	// TODO:
	// Initialize AppWidgetFrame and AppWidget provider specific data.
	// The AppWidget provider permanent data and context can be obtained from persistent storage.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the AppWidget provider will be terminated.

	AppWidgetFrame* pFrame = new MeecastDynamicBoxAppFrame();
	
	pFrame->Construct(FloatDimension(width, height));
	this->SetAppWidgetFrame(pFrame);

	// REMARK:
	// MeecastDynamicBoxAppFrame::OnInitializing() will be invoked after MeecastDynamicBoxAppFrame::Show() is called first.
	pFrame->Show();

	return true;
}

void
MeecastDynamicBoxAppProvider::OnAppWidgetProviderTerminating()
{
	// TODO:
	// Deallocate resources allocated by this AppWidget provider for termination.
	// The AppWidget provider's permanent data and context can be saved via appRegistry.
}

bool
MeecastDynamicBoxAppProvider::OnAppWidgetProviderUpdating(const Tizen::Base::String& argument)
{
    AppLog("!!!!!!!!!!!!!!!!!!!!!!!!!!");
	MeecastDynamicBoxAppFrame* pAppWidgetFrame = dynamic_cast<MeecastDynamicBoxAppFrame*>(GetAppWidgetFrame());

	// TODO:
	// Add your code to update AppWidget here.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the AppWidget provider will be terminated.

	pAppWidgetFrame->OnAppWidgetUpdate();

	return true;
}

void
MeecastDynamicBoxAppProvider::OnAppWidgetProviderRemoved()
{
	// TODO:
	// The AppWidget provider's permanent data and context can be remove at here
}
void
MeecastDynamicBoxAppProvider::Update(){
	MeecastDynamicBoxAppFrame* pAppWidgetFrame = dynamic_cast<MeecastDynamicBoxAppFrame*>(GetAppWidgetFrame());

	// TODO:
	// Add your code to update AppWidget here.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the AppWidget provider will be terminated.

	pAppWidgetFrame->OnAppWidgetUpdate();

}


