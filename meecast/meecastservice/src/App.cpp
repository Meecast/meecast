/**
 * Name        : MeecastServiceApp
 * Version     : 
 * Vendor      : 
 * Description : 
 */

#include "MeecastService.h"
#include <libintl.h>
#include <locale.h>


#define _(String) gettext(String)

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::System;
using namespace Core;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Media;
using namespace Tizen::Locales;

static const wchar_t* LOCAL_MESSAGE_PORT_NAME = L"SERVICE_PORT";


MeecastServiceApp::MeecastServiceApp(void):
                 __pBitmapOriginal(null),
                 __updateTimer(null),
                 __checkupdatingTimer(null){

    result r = E_SUCCESS;
    __updateTimer = new (std::nothrow) Tizen::Base::Runtime::Timer;
    /* TryReturn(__updateTimer != null, E_FAILURE, "[E_FAILURE] Failed to create __updateTimer."); */
    AppLog("updateTimer is created.");

    r = __updateTimer->Construct(*this);
    /* TryReturn(IsFailed(r) != true, r, "[%s] Failed to construct __pTimer", GetErrorMessage(r)); */
    AppLog("__updateTimer is constructed.");

    __checkupdatingTimer = new (std::nothrow) Tizen::Base::Runtime::Timer;
    /* TryReturn(__checkupdatingTimer != null, E_FAILURE, "[E_FAILURE] Failed to create __updateTimer."); */
    AppLog("__checkupdatingTimer is created.");

    r = __checkupdatingTimer->Construct(*this);
/*    TryReturn(IsFailed(r) != true, r, "[%s] Failed to construct __pTimer", GetErrorMessage(r)); */
    AppLog("__checkupdatingTimer is constructed.");


}

MeecastServiceApp::~MeecastServiceApp(void){
    if (__checkupdatingTimer)
        delete __checkupdatingTimer;
    if (__updateTimer)
        delete __updateTimer;
    __updateTimer = NULL;
    _config->DeleteInstance();
    _config = NULL;
    String originalWallpaperFilePath(L"");

    result r = E_SUCCESS;
    AppRegistry* appRegistry = Application::GetInstance()->GetAppRegistry();
    r = appRegistry->Get(L"OriginalWallpaperPath", originalWallpaperFilePath);
    if (r == E_SUCCESS && originalWallpaperFilePath != L""){
        AppLog("set wallpaper %S", originalWallpaperFilePath.GetPointer());
        r = SettingInfo::SetValue(L"http://tizen.org/setting/screen.wallpaper.lock", originalWallpaperFilePath);
    }
}

ServiceApp*
MeecastServiceApp::CreateInstance(void){
	// Create the instance through the constructor.
	return new MeecastServiceApp();
}

bool
MeecastServiceApp::OnAppInitializing(AppRegistry& appRegistry){
	// TODO:
	// Initialize App specific data.
	// The App's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the App will be terminated.

	// TODO: Add your initialization code here
	// Initialize ServerChannel
	__pMessagePort = new (std::nothrow) MeecastServiceMessagePort();
	TryReturn(__pMessagePort != null, false, "MeeCastService : [E_FAILURE] Failed to create __pMessagePort.");
	AppLog("MeeCastService : __pMessagePort is created.");

    result r = E_SUCCESS;
	r = __pMessagePort->Construct(LOCAL_MESSAGE_PORT_NAME);
	TryReturn(IsFailed(r) != true, r, "MeeCastService : [%s] Failed to construct __pMessagePort", GetErrorMessage(r));
	AppLog("MeeCastService : __pMessagePort is constructed.");

    char  buffer[4096]; 
    String str;
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

    AppLog("Update Period %i", _config->UpdatePeriod());
    if (_config->UpdatePeriod() != INT_MAX){
        __updateTimer->Start(_config->UpdatePeriod()*1000);
    }


    /* AppLog ("Lang: %s", getenv ("LANG")); */
    setlocale(LC_ALL, getenv ("LANG"));
    setlocale (LC_TIME, getenv ("LANG"));
    ByteBuffer* pBuf = null;
    String dataPath= App::GetInstance()->GetAppResourcePath() + "locales";
    pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(dataPath);

    textdomain("omweather");
    bindtextdomain("omweather", (const char*)pBuf->GetPointer());
    delete pBuf;
    if (_config->Language() != "System"){
        std::string str;
        str = _config->Language()+ ".UTF8";
        setlocale (LC_ALL, str.c_str());
        setlocale (LC_MESSAGES, str.c_str());
        setlocale (LC_TIME, str.c_str());
    }

    /* TODO Check config */
    SettingInfo::AddSettingEventListener(*this);
    Tizen::Base::String str1;
    str1 = L"http://tizen.org/setting/screen.wallpaper.lock";
    OnSettingChanged(str1);
	return true;
}

bool
MeecastServiceApp::OnAppInitialized(void){
	// TODO:
	// Comment.

	return true;
}

bool
MeecastServiceApp::OnAppWillTerminate(void){
	// TODO:
	// Comment.

	return true;
}

bool
MeecastServiceApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination){
	// TODO:
	// Deallocate resources allocated by this App for termination.
	// The App's permanent data and context can be saved via appRegistry.

	// TODO: Add your termination code here

	return true;
}

void
MeecastServiceApp::OnLowMemory(void){
	// TODO:
	// Free unused resources or close the App.
}

void
MeecastServiceApp::OnBatteryLevelChanged(BatteryLevel batteryLevel){
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}


void
MeecastServiceApp::OnUserEventReceivedN(RequestId requestId, IList* pArgs){
	AppLog("MeeCastService::OnUserEventReceivedN is called. requestId is %d", requestId);

	switch (requestId)
	{
	case RELOAD_CONFIG :
//        OnAppWidgetUpdate();
//        _mdbaProviderFactory->Update(); 
        _config->ReLoadConfig();
        UpdateLockScreen();
        __updateTimer->Cancel();
        if (_config->UpdatePeriod() != INT_MAX){
            __updateTimer->Start(_config->UpdatePeriod()*1000);
        }
        /* TODO Check config */

		break;
	default:
		break;
	}
}

void
MeecastServiceApp::OnTimerExpired(Tizen::Base::Runtime::Timer& timer){
    AppLog("OnTimerExpired");
    if (&timer ==  __updateTimer){
        AppLog("In  __updateTimer");
        for (short i=0; i < _config->stationsList().size();i++){
            _config->stationsList().at(i)->updateData(true);
        }
        __checkupdatingTimer->StartAsRepeatable(10*1000);
    }
    if (&timer == __checkupdatingTimer){
        AppLog("In __checkupdatingTimer");
        for (short i=0; i < _config->stationsList().size();i++){
            if (_config->stationsList().at(i)->isupdating())
                return;
        }
        AppLog("Out __checkupdatingTimer");
        __checkupdatingTimer->Cancel();
        for (short i=0; i < _config->stationsList().size();i++){
            _config->stationsList().at(i)->run_converter();
        }
        String repAppId(15);
        String widgetName(L".meecastdynamicboxapp");
        repAppId = L"ctLjIIgCCj";
        AppId widgetId(repAppId+widgetName);
        Tizen::Shell::AppWidgetProviderManager* pAppWidgetProviderManager = Tizen::Shell::AppWidgetProviderManager::GetInstance();
        result r = E_FAILURE;
        /*  AppLog("Reload Widget"); */
        pAppWidgetProviderManager->RequestUpdate(widgetId, "MeecastDynamicBoxAppProvider", L"");

        UpdateLockScreen();
        AppLog("Out __checkupdatingTimer1");
        __updateTimer->Start(_config->UpdatePeriod()*1000);
        AppLog("Out __checkupdatingTimer2");
    }
}

void
MeecastServiceApp::UpdateLockScreen(){
    AppLog("Update LockScreen");

    int  Width = 720;
    int  Height = 1280;
    char buffer[4096]; 
    
    result r = E_SUCCESS;
/*
    r = SettingInfo::GetValue(L"http://tizen.org/feature/screen.height", Height);
    r = SettingInfo::GetValue(L"http://tizen.org/feature/screen.width", Width);
*/
    Image img;
    img.Construct();


    AppLog("11111111");
    String originalWallpaperFilePath(App::GetInstance()->GetAppDataPath() + "wallpaper.original.jpg");
    String meecastWallpaperFilePath(App::GetInstance()->GetAppSharedPath() + "data/" +  "wallpaper.meecast.png");
    if (__pBitmapOriginal){
        delete __pBitmapOriginal;
    }
    __pBitmapOriginal = img.DecodeN(originalWallpaperFilePath, BITMAP_PIXEL_FORMAT_ARGB8888);
    if (__pBitmapOriginal)
        __pBitmapOriginal->Scale(Dimension(Width, Height));

    AppLog("111111111222");
	Canvas *pCanvas = new Canvas();
    /*
    if (__pBitmapOriginal){
        Width = __pBitmapOriginal->GetWidth();
        Height = __pBitmapOriginal->GetHeight();
    }
    */
    pCanvas->Construct(Rectangle(0, 0, Width, Height));

//	if(!pCanvas)
//		return GetLastResult();

    pCanvas->Clear();
	if (__pBitmapOriginal)
		pCanvas->DrawBitmap(Rectangle(0, 0, Width, Height), *__pBitmapOriginal);
    else{
        pCanvas->SetForegroundColor(Color::GetColor(COLOR_ID_BLACK));
        pCanvas->FillRectangle(Color::GetColor(COLOR_ID_BLACK), Rectangle(0, 0, Width, Height));
    }

    AppLog("222222");
    /* Create background rectangle */
    Rectangle rect;
    Dimension round;
    int PositionX = 60;
    int PositionY = 400;
    rect.x = PositionX;
    rect.y = PositionY;
    rect.width = 600;
    rect.height = 300;

    round.width = 9;
    round.height = 9;

    pCanvas->SetForegroundColor(Color::GetColor(COLOR_ID_BLACK));
    pCanvas->SetLineWidth(1);
    pCanvas->SetLineStyle(LINE_STYLE_SOLID);
    pCanvas->DrawRoundRectangle(Rectangle(PositionX - 1, PositionY - 1, rect.width + 2, rect.height + 2), round);

    pCanvas->SetForegroundColor(Color::GetColor(COLOR_ID_WHITE));
    pCanvas->SetLineWidth(3);
    pCanvas->SetLineStyle(LINE_STYLE_SOLID);
    pCanvas->DrawRoundRectangle(rect, round);


    byte red   =  0x10;
    byte green =  0x10;
    byte blue  =  0x10;
    byte alpha =  0x40;

    Color color(red, green, blue, alpha);
    rect.x = rect.x + 3;
    rect.y = rect.y + 3;
    rect.width = rect.width - 6;
    rect.height = rect.height - 6;

    pCanvas->FillRoundRectangle(color, rect, round);


    AppLog("333333");
    if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0){
        _dp = Core::DataParser::Instance(_config->stationsList().at(_config->current_station_id())->fileName().c_str(), "");
    }else
        _dp = NULL;

    Core::Data *temp_data = NULL;
    if (_dp != NULL && (temp_data = _dp->data().GetDataForTime(time(NULL)))){ 
        /* Main Icon */
        Tizen::Base::Integer icon_int =  temp_data->Icon();
        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Atmos/" + icon_int.ToString() + ".png")){
            /* Main Icon */ 
            Tizen::Media::Image *image = null;
            Tizen::Graphics::Bitmap* mainIconBitmap = null;
            image = new (std::nothrow) Tizen::Media::Image();
            image->Construct();
            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Atmos/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);

		    pCanvas->DrawBitmap(Rectangle(PositionX - 70, PositionY - 90, 350, 350), *mainIconBitmap);
            if (mainIconBitmap)
                delete mainIconBitmap;
            if (image)
                delete image;
        }
        /* Temperature */
        EnrichedText* pEnrichedText = null;
        TextElement* pTextElement = null;
        pEnrichedText = new EnrichedText();
        r = pEnrichedText->Construct(Dimension(280, 100));
        pEnrichedText->SetHorizontalAlignment(TEXT_ALIGNMENT_CENTER);
        pEnrichedText->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
        pEnrichedText->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
        pEnrichedText->SetTextAbbreviationEnabled(true);

        int t = INT_MAX;
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
        pTextElement = new TextElement();
        r = pTextElement->Construct(buffer);

        pTextElement->SetTextColor(Color::GetColor(COLOR_ID_BLACK));{
            Font font;
            font.Construct(FONT_STYLE_BOLD, 80);
            pTextElement->SetFont(font);
        }
        pEnrichedText->Add(*pTextElement);
        pCanvas->DrawText(Point(PositionX + 220 + 2, PositionY + 90 + 2), *pEnrichedText);

        pTextElement->SetTextColor(Color::GetColor(COLOR_ID_WHITE));{
            Font font;
            font.Construct(FONT_STYLE_BOLD, 80);
            pTextElement->SetFont(font);
        }
        pEnrichedText->Add(*pTextElement);
        pCanvas->DrawText(Point(PositionX + 220, PositionY + 90), *pEnrichedText);


        delete pTextElement;
        delete pEnrichedText;

        /* Station name */
        pEnrichedText = new EnrichedText();
        r = pEnrichedText->Construct(Dimension(350, 70));
        pEnrichedText->SetHorizontalAlignment(TEXT_ALIGNMENT_CENTER);
        pEnrichedText->SetVerticalAlignment(TEXT_ALIGNMENT_TOP);
        pEnrichedText->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
        pEnrichedText->SetTextAbbreviationEnabled(true);
        pTextElement = new TextElement();

        if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0){
            String str;
            Tizen::Base::Utility::StringUtil::Utf8ToString(_config->stationname().c_str(), str);
            r = pTextElement->Construct(str);
        }else{
            r = pTextElement->Construct(L"MeeCast");
        }

        pTextElement->SetTextColor(Color::GetColor(COLOR_ID_BLACK));{
            Font font;
            font.Construct(FONT_STYLE_BOLD, 45);
            pTextElement->SetFont(font);
        }
        pEnrichedText->Add(*pTextElement);
        pCanvas->DrawText(Point(PositionX + 250 + 2 , PositionY + 10 + 2), *pEnrichedText);

        pTextElement->SetTextColor(Color::GetColor(COLOR_ID_WHITE));{
            Font font;
            font.Construct(FONT_STYLE_BOLD, 45);
            pTextElement->SetFont(font);
        }
        pEnrichedText->Add(*pTextElement);
        pCanvas->DrawText(Point(PositionX + 250, PositionY + 10), *pEnrichedText);

        delete pTextElement;
        delete pEnrichedText;

        /* Description */
        pEnrichedText = new EnrichedText();
        r = pEnrichedText->Construct(Dimension(550, 60));
        pEnrichedText->SetHorizontalAlignment(TEXT_ALIGNMENT_CENTER);
        pEnrichedText->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
        pEnrichedText->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
        pEnrichedText->SetTextAbbreviationEnabled(true);
        pTextElement = new TextElement();

        if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0){
            String str;
            str = L"";
            Tizen::Base::Utility::StringUtil::Utf8ToString(_(temp_data->Text().c_str()), str);
            r = pTextElement->Construct(str);
        }else{
            r = pTextElement->Construct(L"");
        }

        pTextElement->SetTextColor(Color::GetColor(COLOR_ID_BLACK));{
            Font font;
            font.Construct(FONT_STYLE_BOLD, 40);
            pTextElement->SetFont(font);
        }
        pEnrichedText->Add(*pTextElement);
        pCanvas->DrawText(Point(PositionX + 5 + 2 , PositionY + 190 + 2), *pEnrichedText);

        pTextElement->SetTextColor(Color::GetColor(COLOR_ID_WHITE));{
            Font font;
            font.Construct(FONT_STYLE_BOLD, 40);
            pTextElement->SetFont(font);
        }
        pEnrichedText->Add(*pTextElement);
        pCanvas->DrawText(Point(PositionX + 5, PositionY + 190), *pEnrichedText);

        delete pTextElement;
        delete pEnrichedText;

        /* Last Update */
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

            pEnrichedText = new EnrichedText();
            r = pEnrichedText->Construct(Dimension(570, 100));
            pEnrichedText->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
            pEnrichedText->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
            pEnrichedText->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
            pEnrichedText->SetTextAbbreviationEnabled(true);
            pTextElement = new TextElement();

            r = pTextElement->Construct(dateString);

            pTextElement->SetTextColor(Color::GetColor(COLOR_ID_BLACK));{
                Font font;
                font.Construct(FONT_STYLE_BOLD, 30);
                pTextElement->SetFont(font);
            }
            pEnrichedText->Add(*pTextElement);
            pCanvas->DrawText(Point(PositionX + 5 + 2 , PositionY + 220 + 2), *pEnrichedText);

            pTextElement->SetTextColor(Color::GetColor(COLOR_ID_WHITE));{
                Font font;
                font.Construct(FONT_STYLE_BOLD, 30);
                pTextElement->SetFont(font);
            }
            pEnrichedText->Add(*pTextElement);
            pCanvas->DrawText(Point(PositionX + 5, PositionY + 220), *pEnrichedText);

            delete pTextElement;
            delete pEnrichedText;

        }

    }
	//make bitmap using canvas
	Bitmap* pBitmap = new Bitmap();
	pBitmap->Construct(*pCanvas, Rectangle(0, 0, Width, Height));
//    pBitmap->Scale(Dimension(Width, Height));

	delete pCanvas;

    img.EncodeToFile(*pBitmap, IMG_FORMAT_PNG, meecastWallpaperFilePath, true);
    delete pBitmap;
    AppLog(" New Path %S", meecastWallpaperFilePath.GetPointer());
    r = SettingInfo::SetValue(L"http://tizen.org/setting/screen.wallpaper.lock", meecastWallpaperFilePath);

}

void
MeecastServiceApp::OnSettingChanged(Tizen::Base::String& key){
    if (key == L"http://tizen.org/setting/screen.wallpaper.lock"){

        if ( __updateTimer == NULL)
            return;
        result r = E_SUCCESS;
        AppLog("Changed %S", key.GetPointer());
        String orig_filename;
        r = SettingInfo::GetValue(L"http://tizen.org/setting/screen.wallpaper.lock", orig_filename);
        AppLog (" Result of wallpaper %S", orig_filename.GetPointer());
        
        String meecastWallpaperFilePath(App::GetInstance()->GetAppSharedPath() + "data/" +  "wallpaper.meecast.png");
        String originalWallpaperFilePath(App::GetInstance()->GetAppDataPath() + "wallpaper.original.jpg");
        if (orig_filename != meecastWallpaperFilePath){
            AppRegistry* appRegistry = Application::GetInstance()->GetAppRegistry();
            AppLog (" Set wallpaper %S", orig_filename.GetPointer());
            String temp_name;
            r = appRegistry->Get(L"OriginalWallpaperPath", temp_name);
            if (r == E_KEY_NOT_FOUND){
                r = appRegistry->Add(L"OriginalWallpaperPath", orig_filename);
                if (IsFailed(r)){
                    AppLog("Error in Registry Add");
                }
            }else
                r = appRegistry->Set(L"OriginalWallpaperPath", orig_filename);
            if (IsFailed(r)){
                AppLog("Error in Registry Set");
            }
            r = appRegistry->Save();
            if (IsFailed(r)){
                AppLog("Error in Registry Save");
            }
            AppLog("test111111");
            /* Save original wallpaper */
            Tizen::Io::File::Copy(orig_filename, originalWallpaperFilePath, false);
        }else
            return;
        UpdateLockScreen();
    }
}
