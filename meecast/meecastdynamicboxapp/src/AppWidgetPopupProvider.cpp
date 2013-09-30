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
using namespace Core;
using namespace Tizen::Locales;

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

	pLabel->Construct(rect, L"");
	pLabel->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
	pLabel->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	pLabel->SetTextHorizontalAlignment(ALIGNMENT_CENTER);
	//pLabel->SetBackgroundColor(Color::GetColor(COLOR_ID_GREY));
    pLabel->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/popup_window.png"));
	pAppWidgetPopup->AddControl(pLabel);
    /* Last Update */
    Label* __pLabelLastUpdate = new Label();
    __pLabelLastUpdate->Construct(FloatRectangle(width/2.8, height/2.8, (width - width/2.8), (height - height/2.8)), L"");
	__pLabelLastUpdate->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
	__pLabelLastUpdate->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	__pLabelLastUpdate->SetTextHorizontalAlignment(ALIGNMENT_CENTER);




    try{
        _config = Config::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");
    }
    catch(const std::string &str){
        AppLogDebug("Create Config class:  ");
        _config =  Config::Instance();
    }
    catch(const char *str){
        AppLogDebug("Create Config class:  ");
        _config =  Config::Instance();
    }
    if (_dp)
        _dp->DeleteInstance();
    if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0){
        _dp = Core::DataParser::Instance(_config->stationsList().at(_config->current_station_id())->fileName().c_str(), "");

            if (_dp->LastUpdate()>0){
                time_t last_update = _dp->LastUpdate();
                struct tm   tm1;
                localtime_r(&last_update, &tm1);

                /* Convert date and time */
                DateTime dt;
                dt.SetValue(tm1.tm_year, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min);
                String dateString;
                String timeString;
                LocaleManager localeManager;
                localeManager.Construct();
                Locale  systemLocale = localeManager.GetSystemLocale();
                String countryCodeString = systemLocale.GetCountryCodeString();
                String languageCodeString = systemLocale.GetLanguageCodeString();
                Tizen::Locales::DateTimeFormatter* pDateFormatter = DateTimeFormatter::CreateDateFormatterN(systemLocale, DATE_TIME_STYLE_DEFAULT);
                Tizen::Locales::DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(systemLocale, DATE_TIME_STYLE_SHORT);
        		String customizedPattern = L"dd MMM ";
		        pDateFormatter->ApplyPattern(customizedPattern);
		        pDateFormatter->Format(dt, dateString);
                pTimeFormatter->Format(dt, timeString);
                dateString.Append(timeString);
                dateString.Insert(L"Last update: ",0);
	            __pLabelLastUpdate->SetText(dateString);
	            pAppWidgetPopup->AddControl(__pLabelLastUpdate);
            }
    }else
        _dp = NULL;



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
