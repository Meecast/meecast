/**
 * Name        : MeecastDynamicBoxAppPopupProvider
 * Version     :
 * Vendor      :
 * Description :
 */

#include "MeecastDynamicBoxAppProvider.h"
#include "MeecastDynamicBoxAppPopupProvider.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Shell;

MeecastDynamicBoxAppPopupProvider::MeecastDynamicBoxAppPopupProvider()
{

}

MeecastDynamicBoxAppPopupProvider::~MeecastDynamicBoxAppPopupProvider()
{

}

bool
MeecastDynamicBoxAppPopupProvider::OnAppWidgetPopupProviderInitializing(const String& userInfo)
{
	// TODO:
	// Initialize AppWidgetFrame and AppWidget provider specific data.
	// The AppWidget provider permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the App will be terminated.
	AppWidgetPopup* pAppWidgetPopup = new AppWidgetPopup;
	TryReturn(pAppWidgetPopup, false, "Failed to allocate memory for AppWidgetPopup.");

	const float width = 720.0;
	const float height = 350.0;

	pAppWidgetPopup->Construct(FloatDimension(width, height));

	// TODO:
	// Put your UI code here
	Label* pLabel = new Label();
	FloatRectangle popupClientAreaBounds = pAppWidgetPopup->GetClientAreaBounds();
	FloatRectangle rect(0, 0, popupClientAreaBounds.width, popupClientAreaBounds.height);

	pLabel->Construct(rect, L"This is Drop View!");
	pLabel->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
	pLabel->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	pLabel->SetTextHorizontalAlignment(ALIGNMENT_CENTER);
	//pLabel->SetBackgroundColor(Color::GetColor(COLOR_ID_GREY));
    pLabel->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/popup_window4.png"));
	pAppWidgetPopup->AddControl(pLabel);

	pAppWidgetPopup->Show();

	SetAppWidgetPopup(pAppWidgetPopup);
	return true;
}

void
MeecastDynamicBoxAppPopupProvider::OnAppWidgetPopupProviderTerminating(void)
{
	// TODO:
	// Deallocate resources allocated by this AppWidget provider for termination.
	// The AppWidget provider's permanent data and context can be saved via appRegistry.
}
