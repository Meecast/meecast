/**
 * Name        : MeecastDynamicBoxAppPopupProvider
 * Version     :
 * Vendor      :
 * Description :
 */

#include "MeecastDynamicBoxAppProvider.h"
#include "MeecastDynamicBoxAppPopupProvider.h"
#include <libintl.h>
#include <locale.h>

#define _(String) gettext(String)
#define STRONG_WIND 8 
#define SAFE_DELETE(x)  if (x) { delete x; x = null; }


using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui;
using namespace Tizen::Shell;
using namespace Core;
using namespace Tizen::Base::Collection;
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
	const float height = 380.0;

    char  buffer[4096]; 
    String str;
	pAppWidgetPopup->Construct(FloatDimension(width, height));

	// TODO:
	// Put your UI code here
	Label* pLabel = new Label();
	FloatRectangle popupClientAreaBounds = pAppWidgetPopup->GetClientAreaBounds();
	FloatRectangle rect(0, 20, popupClientAreaBounds.width, popupClientAreaBounds.height - 20);

	pLabel->Construct(rect, L"");
	pLabel->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	pLabel->SetTextHorizontalAlignment(ALIGNMENT_CENTER);
	//pLabel->SetBackgroundColor(Color::GetColor(COLOR_ID_GREY));
    pLabel->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/popup_window.png"));
	pAppWidgetPopup->AddControl(pLabel);

    /* Last Update */
    Label* __pLabelLastUpdate = new Label();
    __pLabelLastUpdate->Construct(FloatRectangle(width/6, (height - height/3.05), (width - width/6), height/5), L"");
	__pLabelLastUpdate->SetTextConfig(28, LABEL_TEXT_STYLE_BOLD);
	__pLabelLastUpdate->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	__pLabelLastUpdate->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	__pLabelLastUpdate->SetTextColor(Color::GetColor(COLOR_ID_WHITE));

    Label* __pLabelLastUpdateBG = new Label();
    __pLabelLastUpdateBG->Construct(FloatRectangle(width/6, (height - height/3.05)+1 , (width - width/6), height/5), L"");
	__pLabelLastUpdateBG->SetTextConfig(28, LABEL_TEXT_STYLE_BOLD);
	__pLabelLastUpdateBG->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	__pLabelLastUpdateBG->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	__pLabelLastUpdateBG->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
   
    /* Station name */
    Label* __pLabelTown = new Label();
    __pLabelTown->Construct(FloatRectangle(-10,  height/2.3, width - width/1.6, height/3.5), L"");
    __pLabelTown->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
    __pLabelTown->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelTown->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

    Label* __pLabelTownBG = new Label();
    __pLabelTownBG->Construct(FloatRectangle(-10 + 1, height/2.3 +1, width - width/1.6, height/3.5), L"");
    __pLabelTownBG->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    __pLabelTownBG->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelTownBG->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

    /* Main icon */
    Label* __pLabelMainIcon = new Label();
    __pLabelMainIcon->Construct(FloatRectangle(0, 0, 128, 128), L"");

    /* Temperature */
    Label* __pLabelMainTemperatureBackground = new Label();
    __pLabelMainTemperatureBackground->Construct(FloatRectangle((width/8.5 + 1), (height/5 + 1) , width/4, height/4), L"");
    __pLabelMainTemperatureBackground->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    
    Label* __pLabelMainTemperature = new Label();
    __pLabelMainTemperature->Construct(FloatRectangle((width/8.5), (height/5) , width/4, height/4), L"");
    __pLabelMainTemperature->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
     
    /* Wind Speed */
    Label* __pLabelMainWindSpeed = new Label();
    __pLabelMainWindSpeed->Construct(FloatRectangle((width/3.7) - 6, (height/15), 52+10, 52), L"");
    __pLabelMainWindSpeed->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);

    /* Wind icon */
    Label* __pLabelMainWindIcon = new Label();
    __pLabelMainWindIcon->Construct(FloatRectangle((width/3.7), (height/15), 52+10, 52), L"");

    Label* icon1;
    Label* icon2;
    Label* icon3;
    Label* icon4;
    Label* windicon1;
    Label* windicon2;
    Label* windicon3;
    Label* windicon4;
    Label* windspeed1;
    Label* windspeed2;
    Label* windspeed3;
    Label* windspeed4;
    Label* temperature_hi1;
    Label* temperature_hi2;
    Label* temperature_hi3;
    Label* temperature_hi4;
    Label* temperature_low1;
    Label* temperature_low2;
    Label* temperature_low3;
    Label* temperature_low4;
    Label* dayname1;
    Label* dayname2;
    Label* dayname3;
    Label* dayname4;

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
        		String customizedPattern = L" dd MMM ";
		        pDateFormatter->ApplyPattern(customizedPattern);
		        pDateFormatter->Format(dt, dateString);
                pTimeFormatter->Format(dt, timeString);
                dateString.Append(timeString);
                dateString.Insert(_("Last update:"), 0);
                __pLabelLastUpdate->SetText(dateString);
	            __pLabelLastUpdateBG->SetText(dateString);
	            pAppWidgetPopup->AddControl(__pLabelLastUpdateBG);
//	            pAppWidgetPopup->AddControl(__pLabelLastUpdate);
            }
    }else
        _dp = NULL;

    if (_config->stationname().c_str()){
       __pLabelTown->SetText(_config->stationname().c_str());
       __pLabelTownBG->SetText(_config->stationname().c_str());
       if (__pLabelTown->GetText().GetLength()>15){
            if (__pLabelTown->GetText().GetLength()>20){
                __pLabelTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
                __pLabelTownBG->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
            }else{
                __pLabelTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
                __pLabelTownBG->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
            }
        }else{
            __pLabelTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
            __pLabelTownBG->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
        }

        pAppWidgetPopup->AddControl(__pLabelTownBG);
        pAppWidgetPopup->AddControl(__pLabelTown);
    }

    Core::Data *temp_data = NULL;
    if (_dp != NULL && (temp_data = _dp->data().GetDataForTime(time(NULL)))){

        /* AppLog ("_Config_dp inside"); */
        /* Preparing units */
        temp_data->temperature_low().units(_config->TemperatureUnit());
        temp_data->temperature_hi().units(_config->TemperatureUnit());
        temp_data->temperature().units(_config->TemperatureUnit());
        temp_data->WindSpeed().units(_config->WindSpeedUnit());
        /* Main Icon */
//        snprintf(buffer, sizeof(buffer) - 1, "%s/%s/%i.png", 
//                                       app->config->iconspath().c_str(), 
//                                       app->config->iconSet().c_str(), 
//                                       temp_data->Icon());

        Tizen::Base::Integer icon_int =  temp_data->Icon();
        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Glance/" + icon_int.ToString() + ".png")){
            /* Main Icon */ 
            Tizen::Media::Image *image = null;
            Tizen::Graphics::Bitmap* mainIconBitmap = null;
            image = new (std::nothrow) Tizen::Media::Image();
            image->Construct();

            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Glance/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
            __pLabelMainIcon->SetBackgroundBitmap(*mainIconBitmap);
            __pLabelMainIcon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }

        pAppWidgetPopup->AddControl(__pLabelMainIcon);
        /* Description */
        /*
        String str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_(temp_data->Text().c_str()), str);
        __pLabelMainDescription->SetText(str);
        __pLabelMainDescriptionBackground->SetText(str);
        */
        int t = INT_MAX;
        /* Temperature */
        if (temp_data->temperature().value(true) == INT_MAX){
          if ((temp_data->temperature_hi().value(true) == INT_MAX) &&
              (temp_data->temperature_low().value(true) == INT_MAX)){ 
            //__pLabelMainDescription->SetText("N/A");
          } 
          if ((temp_data->temperature_hi().value(true) != INT_MAX) &&
              (temp_data->temperature_low().value(true) != INT_MAX)){ 
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°/ %0.f°", temp_data->temperature_low().value(),
                                                                 temp_data->temperature_hi().value());
            t = temp_data->temperature_hi().value();
          }else{  
              if (temp_data->temperature_hi().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
                t = temp_data->temperature_hi().value();
              }
              if (temp_data->temperature_low().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
                t = temp_data->temperature_low().value();
              }
          }
        }else{
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature().value());
            t = temp_data->temperature().value();
        }


//        Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(t);
//        backgroundPanel->SetBackgroundColor(*color_of_temp);
//        delete color_of_temp;
//        __pLabelMainTemperature->SetShowState(true);
//        __pLabelMainTemperatureBackground->SetShowState(true);
        Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
        __pLabelMainTemperature->SetText(str);
        __pLabelMainTemperatureBackground->SetText(str);
        __pLabelMainTemperature->RequestRedraw();
        __pLabelMainTemperatureBackground->RequestRedraw();


        if (__pLabelMainTemperature->GetText().GetLength()<4){
            __pLabelMainTemperature->SetTextConfig(68, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(68, LABEL_TEXT_STYLE_BOLD);
        }else{
            if (__pLabelMainTemperature->GetText().GetLength()<6){
                __pLabelMainTemperature->SetTextConfig(58, LABEL_TEXT_STYLE_BOLD);
                __pLabelMainTemperatureBackground->SetTextConfig(58, LABEL_TEXT_STYLE_BOLD);
            }else{
                if (__pLabelMainTemperature->GetText().GetLength()<9){
                    __pLabelMainTemperature->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
                    __pLabelMainTemperatureBackground->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
                }else{
                    __pLabelMainTemperature->SetTextConfig(32, LABEL_TEXT_STYLE_BOLD);
                    __pLabelMainTemperatureBackground->SetTextConfig(32, LABEL_TEXT_STYLE_BOLD);
                }
            }
        }


#if 0        
        /* Current or not current period */
        if (temp_data->Current())
            Tizen::Base::Utility::StringUtil::Utf8ToString(_("Now"), str);
        else
            Tizen::Base::Utility::StringUtil::Utf8ToString(_("Today"), str);
        main_current_state->SetShowState(true);
        main_current_state->SetText(str);
        main_current_state->RequestRedraw();
#endif

        /* Main wind direction */
        if (temp_data->WindDirection() != "N/A"){
            snprintf (buffer, sizeof(buffer) -1, "%s", temp_data->WindDirection().c_str());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            if (temp_data->WindSpeed().value() > STRONG_WIND){
                str.Append("_warning");
            }
            /* AppLog("Wind1 %S", str.GetPointer()); */
            if (str == "CALM" || Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/Digia/wind_" + str + ".png")){
                __pLabelMainWindIcon->SetShowState(true);
                /* Wind direction Icon */ 
                Tizen::Media::Image *image = null;
                Tizen::Graphics::Bitmap* windIconBitmap = null;
                image = new (std::nothrow) Tizen::Media::Image();
                image->Construct();
                
                if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/Digia/wind_" + str + ".png")){
                    windIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"720x1280/Digia/wind_" + str + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
                    AppLog("Wind %S", str.GetPointer());
                    __pLabelMainWindIcon->SetBackgroundBitmap(*windIconBitmap);
                    __pLabelMainWindIcon->SetSize(windIconBitmap->GetWidth(), windIconBitmap->GetHeight());
                    __pLabelMainWindIcon->RequestRedraw();
                    SAFE_DELETE(image);
                    SAFE_DELETE(windIconBitmap);
                }
            }
            pAppWidgetPopup->AddControl(__pLabelMainWindIcon);
        }
        /* Main wind speed */
        if (temp_data->WindSpeed().value() != INT_MAX){
            __pLabelMainWindSpeed->SetShowState(true);
            snprintf (buffer, sizeof(buffer) -1, "%0.f", 
                                             temp_data->WindSpeed().value());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            __pLabelMainWindSpeed->SetText(str);
            __pLabelMainWindSpeed->RequestRedraw();
        }else{
            __pLabelMainWindSpeed->SetShowState(false);
        }

        pAppWidgetPopup->AddControl(__pLabelMainWindSpeed);

    }else{
        __pLabelMainTemperature->SetText("MeeCast");
        __pLabelMainTemperatureBackground->SetText("MeeCast");
    }
    pAppWidgetPopup->AddControl(__pLabelMainTemperatureBackground);
    pAppWidgetPopup->AddControl(__pLabelMainTemperature);

    time_t current_day;
    struct tm   *tm = NULL;

    int localtimezone = 0;
    int timezone = 0;
    /* Timezone */
    if (_dp != NULL){
        timezone = _dp->timezone();
        /* AppLog("TimeZone %i", timezone); */
    }


    current_day = time(NULL);

    /*  AppLog("First Current day %li", current_day); */
    /* Set localtimezone */
    struct tm time_tm1;
    struct tm time_tm2;
    gmtime_r(&current_day, &time_tm1);
    localtime_r(&current_day, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 

    /* set current day */ 
    current_day = current_day + 3600*timezone; 

    /* AppLog("Current day0 %li", current_day); */

    tm = gmtime(&current_day);
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    current_day = mktime(tm); /* today 00:00:00 */
    /* AppLog("Current day before %li", current_day); */
    current_day = current_day + 3600*localtimezone - 3600*timezone; 

    /* AppLog("Current day %li", current_day); */
    /* AppLog("Local TimeZone %i Remote timezone %i", localtimezone, timezone); */

    /* fill other days */
    int i = 3600*24;
    int _dayCount = 1;
    while  (_dp != NULL && i < 3600*24*5) {
       /* AppLog ("Result0 %li", current_day + 15*3600 + i - 3600*timezone); */
        if ((temp_data = _dp->data().GetDataForTime(current_day + 15*3600 + i)) != null){
            Tizen::Base::Integer icon_int = temp_data->Icon();
            Label* icon = new Label();
            icon->Construct(FloatRectangle(260 + (_dayCount -1)*110 , 0, 80, 80), L"");

            if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Glance/" + icon_int.ToString() + ".png")){
                /* Main Icon */ 
                Tizen::Media::Image *image = null;
                Tizen::Graphics::Bitmap* mainIconBitmap = null;
                image = new (std::nothrow) Tizen::Media::Image();
                image->Construct();

                mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Glance/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
                icon->SetBackgroundBitmap(*mainIconBitmap);
                icon->RequestRedraw();
                SAFE_DELETE(image);
                SAFE_DELETE(mainIconBitmap);
            }

            Label* windicon = new Label();
            windicon->Construct(FloatRectangle(275 + (_dayCount -1)*110 , 75, 52 + 10, 52), L"");
            /* Main wind direction */
            if (temp_data->WindDirection() != "N/A"){
                snprintf (buffer, sizeof(buffer) -1, "%s", temp_data->WindDirection().c_str());
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                if (temp_data->WindSpeed().value() > STRONG_WIND){
                    str.Append("_warning");
                }
                /* AppLog("Wind1 %S", str.GetPointer()); */
                if (str == "CALM" || Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/Digia/wind_" + str + ".png")){
                    /* Wind direction Icon */ 
                    Tizen::Media::Image *image = null;
                    Tizen::Graphics::Bitmap* windIconBitmap = null;
                    image = new (std::nothrow) Tizen::Media::Image();
                    image->Construct();
                    
                    if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/Digia/wind_" + str + ".png")){
                        windIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"720x1280/Digia/wind_" + str + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
                        windicon->SetBackgroundBitmap(*windIconBitmap);
                        windicon->SetSize(windIconBitmap->GetWidth(), windIconBitmap->GetHeight());
                        SAFE_DELETE(image);
                        SAFE_DELETE(windIconBitmap);
                    }
                }
            }
            Label* windspeed = new Label();
            windspeed->Construct(FloatRectangle(275 + (_dayCount -1)*110 - 6 , 75, 52 + 10, 52), L"");
            windspeed->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);

            /* Wind speed */
            if (temp_data->WindSpeed().value() != INT_MAX){
                windspeed->SetShowState(true);
                snprintf (buffer, sizeof(buffer) -1, "%0.f", 
                                                 temp_data->WindSpeed().value());
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                windspeed->SetText(str);
            }else{
                windspeed->SetShowState(false);
            }
            /* Day name */
            Label* day_name = new Label();
            day_name->Construct(FloatRectangle(245 + (_dayCount -1)*110, 115, 110, 70), L"");
            day_name->SetTextConfig(36, LABEL_TEXT_STYLE_NORMAL);
            /* Convert date and time */
            DateTime dt;
            time_t day_and_time;
            struct tm   *tm1 = NULL;
            day_and_time = temp_data->StartTime() + _dp->timezone()*3600;
            tm1 = gmtime(&(day_and_time));
            dt.SetValue(1900 + tm1->tm_year, tm1->tm_mon + 1, tm1->tm_mday, tm1->tm_hour, tm1->tm_min);
            String dateString;
            String timeString;
            LocaleManager localeManager;
            localeManager.Construct();
            Locale  systemLocale = localeManager.GetSystemLocale();
            String countryCodeString = systemLocale.GetCountryCodeString();
            String languageCodeString = systemLocale.GetLanguageCodeString();
            Tizen::Locales::DateTimeFormatter* pDateFormatter = DateTimeFormatter::CreateDateFormatterN(systemLocale, DATE_TIME_STYLE_SHORT);
            String customizedPattern = L"E";
            pDateFormatter->ApplyPattern(customizedPattern);
            pDateFormatter->Format(dt, dateString);

//          Tizen::Base::Utility::StringUtil::Utf8ToString(temp_data->ShortDayName().c_str(), str);
            day_name->SetText(dateString);

            /* Temperatures */
            Label* temperature_hi = new Label();
            temperature_hi->Construct(FloatRectangle(250 + (_dayCount -1)*110, 165, 110, 70), L"");
            temperature_hi->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);

            Label* temperature_low = new Label();
            temperature_low->Construct(FloatRectangle(250 + (_dayCount -1)*110, 215, 110, 70), L"");
            temperature_low->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);


            if (temp_data->temperature_low().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                temperature_low->SetText(str);
	            temperature_low->SetTextColor(Color::GetColor(COLOR_ID_GREY));
                temperature_low->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
                temperature_low->SetTextHorizontalAlignment(ALIGNMENT_CENTER);
            }
            if (temp_data->temperature_hi().value(true) != INT_MAX){
                snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
                Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
                temperature_hi->SetText(str);
	            temperature_hi->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
                temperature_hi->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
                temperature_hi->SetTextHorizontalAlignment(ALIGNMENT_CENTER);
            }

            pAppWidgetPopup->AddControl(windicon);
            pAppWidgetPopup->AddControl(windspeed);
            pAppWidgetPopup->AddControl(icon);
            pAppWidgetPopup->AddControl(temperature_low);
            pAppWidgetPopup->AddControl(temperature_hi);
            pAppWidgetPopup->AddControl(day_name);
            switch (_dayCount){
                case 1:
                    icon1 = icon;
                    windicon1 = windicon;
                    windspeed1 = windspeed;
                    temperature_hi1 = temperature_hi;
                    temperature_low1 = temperature_low;
                    dayname1 = day_name;
                    break;  
                case 2:
                    icon2=icon;
                    windicon2=windicon;
                    windspeed2=windspeed;
                    temperature_hi2 = temperature_hi;
                    temperature_low2 = temperature_low;
                    dayname2 = day_name;
                    break;
                case 3:
                    icon3=icon;
                    windicon3=windicon;
                    windspeed3=windspeed;
                    temperature_hi3 = temperature_hi;
                    temperature_low3 = temperature_low;
                    dayname3 = day_name;
                    break;
                case 4:
                    icon4=icon;
                    windicon4=windicon;
                    windspeed4=windspeed;
                    temperature_hi4 = temperature_hi;
                    temperature_low4 = temperature_low;
                    dayname4 = day_name;
                    break;
            }
    //        __daysmap->Add(*(new (std::nothrow) Integer(_dayCount)), *(new (std::nothrow) Long(current_day + 15*3600 + i)));
             /* AppLog ("Result1 %li", current_day); */
            /* AppLog ("Result %li", current_day + 15*3600 + i); */
            _dayCount ++;
        }
        i = i + 3600*24;
    }

    /* Launcher Button */
    Button* __pLauncherButton = new Button();
    __pLauncherButton->Construct(Tizen::Graphics::Rectangle((width/1.12), (height/1.4), 45, 45), ""); 
    __pLauncherButton->SetNormalBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mainmenu.png"));
    __pLauncherButton->SetPressedBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("mainmenu.png"));

    __pLauncherButton->SetActionId(ID_BUTTON_LAUNCHER);
    __pLauncherButton->AddActionEventListener(*this);

    pAppWidgetPopup->AddControl(__pLauncherButton);

	pAppWidgetPopup->Show();

	SetAppWidgetPopup(pAppWidgetPopup);
	return true;
}

void
MeecastDynamicBoxAppPopupProvider::OnAppWidgetPopupProviderTerminating(void){
	// TODO:
	// Deallocate resources allocated by this AppWidget provider for termination.
	// The AppWidget provider's permanent data and context can be saved via appRegistry.
}

void
MeecastDynamicBoxAppPopupProvider::OnActionPerformed(const Tizen::Ui::Control& source, int actionId){
    AppLog("Check Action");
    String repAppId(15);
    repAppId = L"ctLjIIgCCj.meecast";
    AppId appId(repAppId);
    AppManager* pAppManager = AppManager::GetInstance();
    result r = E_FAILURE;
    switch(actionId){
        case ID_BUTTON_LAUNCHER:
            AppLog("sssssssssssssssssssssss");

            r = pAppManager->LaunchApplication(repAppId, null);

            break;
        default:
            break;
    }
}

