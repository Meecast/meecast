/**
 * Name        : MeecastDynamicBoxAppFrame
 * Version     :
 * Vendor      :
 * Description :
 */

#include "MeecastDynamicBoxAppProvider.h"
#include "MeecastDynamicBoxAppFrame.h"

#include <libintl.h>
#include <locale.h>


using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::App;
using namespace Core;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

#define SAFE_DELETE(x)  if (x) { delete x; x = null; }
#define _(String) gettext(String)
#define STRONG_WIND 8 
#define background_width1_1 140
#define background_height1_1 134
#define background_width2_1 320
#define background_height2_1 134
#define background_width2_2 320
#define background_height2_2 320 


static const wchar_t* LOCAL_MESSAGE_PORT_NAME = L"SERVICE_PORT";

MeecastDynamicBoxAppFrame::MeecastDynamicBoxAppFrame(): __pMessagePort(null)
{

}

MeecastDynamicBoxAppFrame::~MeecastDynamicBoxAppFrame()
{
    _config->DeleteInstance();
}

result
MeecastDynamicBoxAppFrame::OnInitializing(void){ 
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
	// Initialize ServerChannel
	__pMessagePort = new (std::nothrow) MeecastDynamicMessagePort();
	TryReturn(__pMessagePort != null, false, "MeeCastDynamicBox : [E_FAILURE] Failed to create __pMessagePort.");
	/* AppLog("MeeCastDynamicBox : __pMessagePort is created."); */

    result r = E_SUCCESS;
	r = __pMessagePort->Construct(LOCAL_MESSAGE_PORT_NAME);
	TryReturn(IsFailed(r) != true, r, "MeeCastDynamicBox : [%s] Failed to construct __pMessagePort", GetErrorMessage(r));
	/* AppLog("MeeCastDynamicBox : __pMessagePort is constructed."); */

	FloatRectangle bounds = this->GetBoundsF();


    __pPanel = new Panel();
    __pPanel->Construct(bounds);
//	__pPanel->SetBackgroundColor(Color::GetColor(COLOR_ID_YELLOW));
    __pLabelBackground1 = new Label();

    __pLabelMainIcon = new Label();
    __pLabelMainIcon->Construct(FloatRectangle(bounds.x - 0, bounds.y - 0, 128, 128), L"");

    __pLabelMainWindIcon = new Label();
    __pLabelMainWindIcon->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1/2.8), bounds.y + background_height1_1/2.8, 52, 42), L"");

    __pLabelMainWindSpeed = new Label();
    __pLabelMainWindSpeed->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1/2.8) - 6, (bounds.y + background_height1_1/2.8) - 1, 52+10, 52), L"");
    __pLabelMainWindSpeed->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);
    if (_config->Mod() == "Digia"){
        __pLabelMainWindSpeed->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    }else if (_config->Mod() == "Marina"){
        __pLabelMainWindSpeed->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
    }else if (_config->Mod() == "ExtraCoffe"){
        __pLabelMainWindSpeed->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    }
    __pLabelMainWindSpeed->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelMainWindSpeed->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

    __pLabelMainTemperatureBackground = new Label();
    __pLabelMainTemperatureBackground->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1 + 1 - 20), (bounds.height - bounds.height/3 + 1) , background_width1_1 + 40, bounds.height/3), L"");
    __pLabelMainTemperatureBackground->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    
    __pLabelMainTemperature = new Label();
    __pLabelMainTemperature->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1- 20), (bounds.height - bounds.height/3) , background_width1_1 + 40, bounds.height/3), L"");
    __pLabelMainTemperature->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
    if (_config->Mod() == "Digia"){
        __pLabelMainTemperature->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
    }else if (_config->Mod() == "Marina"){
        __pLabelMainTemperature->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
    } if (_config->Mod() == "ExtraCoffe"){
        __pLabelMainTemperature->SetTextColor(Color::Color(0xff, 0xda, 0x9f));
    }

    __pLabelTown = new Label();
    __pLabelTown->Construct(FloatRectangle((bounds.x + bounds.width - background_width2_1) - 1, bounds.y+103, bounds.width, bounds.height - 104), _config->stationname().c_str());
    __pLabelTown->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
    __pLabelTown->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelTown->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

    __pLabelBackgroundTown = new Label();
    __pLabelBackgroundTown->Construct(FloatRectangle(bounds.x, bounds.y+104, bounds.width, bounds.height - 104), _config->stationname().c_str());
    __pLabelBackgroundTown->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    __pLabelBackgroundTown->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelBackgroundTown->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

    __pLabelMainDescription = new Label();
    __pLabelMainDescription->Construct(FloatRectangle(bounds.x, bounds.y+104, bounds.width, bounds.height - 104), L"");
    __pLabelMainDescription->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
    __pLabelMainDescription->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelMainDescription->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

    __pLabelMainDescriptionBackground = new Label();
    __pLabelMainDescriptionBackground->Construct(FloatRectangle(bounds.x, bounds.y+104, bounds.width, bounds.height - 104), L"");
    __pLabelMainDescriptionBackground->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    __pLabelMainDescriptionBackground->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelMainDescriptionBackground->SetTextHorizontalAlignment(ALIGNMENT_CENTER);


    if (_dp)
        _dp->DeleteInstance();
    if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0){
        _dp = Core::DataParser::Instance(_config->stationsList().at(_config->current_station_id())->fileName().c_str(), "");
    }else
        _dp = NULL;

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
        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/" + _config->iconSet().c_str() + "/" + icon_int.ToString() + ".png")){
            /* Main Icon */ 
            Tizen::Media::Image *image = null;
            Tizen::Graphics::Bitmap* mainIconBitmap = null;
            image = new (std::nothrow) Tizen::Media::Image();
            image->Construct();

            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/" + _config->iconSet().c_str() + "/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
            __pLabelMainIcon->SetBackgroundBitmap(*mainIconBitmap);
            __pLabelMainIcon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }

        /* Description */
        String str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_(temp_data->Text().c_str()), str);
        __pLabelMainDescription->SetText(str);
        __pLabelMainDescriptionBackground->SetText(str);

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


        if (str.GetLength()<8){
            __pLabelMainTemperature->SetTextConfig(44, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(44, LABEL_TEXT_STYLE_BOLD);
        }else{
            __pLabelMainTemperature->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
        }
        __pLabelMainTemperature->RequestRedraw();
        __pLabelMainTemperatureBackground->RequestRedraw();


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

        /* Main wind direction */
        if (temp_data->WindDirection() != "N/A"){
            snprintf (buffer, sizeof(buffer) -1, "%s", temp_data->WindDirection().c_str());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            if (temp_data->WindSpeed().value() > STRONG_WIND){
                str.Append("_warning");
            }
            /* AppLog("Wind1 %S", str.GetPointer()); */
            String temp_str;
            Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);

            if (str == "CALM" || Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/"+ temp_str +"/wind_" + str + ".png")){
                __pLabelMainWindIcon->SetShowState(true);
                /* Wind direction Icon */ 
                Tizen::Media::Image *image = null;
                Tizen::Graphics::Bitmap* windIconBitmap = null;
                image = new (std::nothrow) Tizen::Media::Image();
                image->Construct();
                
                if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/" + temp_str + "/wind_" + str + ".png")){
                    windIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"720x1280/" + temp_str + "/wind_" + str + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
                    AppLog("Wind %S", str.GetPointer());
                    __pLabelMainWindIcon->SetBackgroundBitmap(*windIconBitmap);
                    __pLabelMainWindIcon->SetSize(windIconBitmap->GetWidth(), windIconBitmap->GetHeight());
                    __pLabelMainWindIcon->RequestRedraw();
                    SAFE_DELETE(image);
                    SAFE_DELETE(windIconBitmap);
                }
            }
        }
    }else{
        __pLabelMainTemperature->SetText("MeeCast");
        __pLabelMainTemperatureBackground->SetText("MeeCast");
        __pLabelMainTemperature->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
        __pLabelMainTemperatureBackground->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);

        Tizen::Base::Integer icon_int = 49;
        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/" + _config->iconSet().c_str() + "/" + icon_int.ToString() + ".png")){
            /* Main Icon */ 

            Tizen::Media::Image *image = null;
            Tizen::Graphics::Bitmap* mainIconBitmap = null;

            image = new (std::nothrow) Tizen::Media::Image();
            image->Construct();

            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/" + _config->iconSet().c_str() + "/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
            __pLabelMainIcon->SetBackgroundBitmap(*mainIconBitmap);
            __pLabelMainIcon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }
    }
            //Adds the panel to the form
            //    AddControl(__pPanel);
            //
            //
    /* Prepared for 1x1 mode */
    if ((bounds.height == bounds.width) && bounds.height<200){
        /* AppLog("Init 1x1"); */
        __pLabelMainIcon->SetSize(Dimension(128, 128));
        __pLabelMainDescription->SetShowState(false);
        __pLabelMainDescriptionBackground->SetShowState(false);
        __pLabelBackground1->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1), (bounds.y + bounds.height - background_height1_1), background_width1_1, background_height1_1), L"");
        String temp_str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);
       __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN(L"Widget/" + temp_str + L"/single_now1_1.png"));

       __pLabelBackgroundTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
       __pLabelBackgroundTown->SetShowState(false);

       __pLabelTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
       __pLabelTown->SetShowState(false);
       __pLabelMainWindSpeed->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);
    }
    /* Prepared for 2x1 mode */
    if ((bounds.width != bounds.height) && bounds.height<200){
        /* AppLog("Init 2x1"); */
        __pLabelMainIcon->SetSize(Dimension(128, 128));
        __pLabelBackground1->Construct(FloatRectangle((bounds.x + bounds.width - background_width2_1), (bounds.y + bounds.height - background_height2_1), background_width2_1, background_height2_1), L"");

        String temp_str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);
        __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/" + temp_str + "/single_now1_2.png"));

        __pLabelBackgroundTown->SetShowState(true);
        __pLabelBackgroundTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
        __pLabelTown->SetShowState(true);
        __pLabelTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);

        __pLabelMainTemperatureBackground->SetPosition((int)(bounds.x + bounds.width - background_width2_1/1.4 + 1), (int)(bounds.y + background_height2_1/2.9 +1));
        __pLabelMainTemperature->SetPosition((int)(bounds.x + bounds.width - background_width2_1/1.4), (int)(bounds.y + background_height2_1/2.9));
        __pLabelMainTemperature->SetSize((background_width2_1/1.6), bounds.height/3);
        __pLabelMainTemperatureBackground->SetSize((background_width2_1/1.6), bounds.height/3);

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
        __pLabelMainTemperature->RequestRedraw();
        __pLabelMainTemperatureBackground->RequestRedraw();

        if (__pLabelTown->GetText().GetLength()>15){
            if (__pLabelTown->GetText().GetLength()>20){
                __pLabelTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
                __pLabelBackgroundTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
            }else{
                __pLabelTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
                __pLabelBackgroundTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
            }
        }else{
            __pLabelTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
            __pLabelBackgroundTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
        }
        __pLabelBackgroundTown->SetShowState(true);
        __pLabelBackgroundTown->SetSize(Dimension(bounds.width + 40, (bounds.height - bounds.height/2)));
        __pLabelBackgroundTown->SetPosition((bounds.x  + 1), (bounds.height - bounds.height/2.5 + 1));
        __pLabelTown->SetShowState(true);
        __pLabelTown->SetSize(Dimension(bounds.width + 40, (bounds.height - bounds.height/2)));
        __pLabelTown->SetPosition((bounds.x), (bounds.height - bounds.height/2.5));

        __pLabelMainDescription->SetShowState(false);
        __pLabelMainDescriptionBackground->SetShowState(false);
        __pLabelMainWindSpeed->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);
    }


    /* Prepared for 2x2 mode */
    if ((bounds.height == bounds.width) && bounds.height>200){
        /* AppLog("Init 2x2"); */
        __pLabelBackground1->Construct(FloatRectangle((bounds.x + bounds.width - background_width2_2), (bounds.y + bounds.height - background_height2_2), background_width2_2, background_height2_2), L"");

        String temp_str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);
        __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/" + temp_str + "/single_now2_2.png"));

        if (__pLabelTown->GetText().GetLength()>15){
            if (__pLabelTown->GetText().GetLength()>20){
                __pLabelTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
                __pLabelBackgroundTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
            }else{
                __pLabelTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
                __pLabelBackgroundTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
            }
        }else{
            __pLabelTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
            __pLabelBackgroundTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
        }
        __pLabelBackgroundTown->SetShowState(true);
        __pLabelBackgroundTown->SetSize(Dimension(bounds.width + 40, (bounds.height - bounds.height/2.6)));
        __pLabelBackgroundTown->SetPosition((bounds.x  + 1), (bounds.height - bounds.height/2.5 + 1));

        __pLabelTown->SetShowState(true);
        __pLabelTown->SetSize(Dimension(bounds.width + 40, (bounds.height - bounds.height/2.6)));
        __pLabelTown->SetPosition((bounds.x), (bounds.height - bounds.height/2.5));

        /* Main Description */
        __pLabelMainDescription->SetShowState(true);
        __pLabelMainDescription->SetTextConfig(28, LABEL_TEXT_STYLE_NORMAL);
        __pLabelMainDescription->SetPosition((bounds.x + 40), (bounds.height - bounds.height/2.3));
        __pLabelMainDescription->SetSize(Dimension(bounds.width - 40, (bounds.height - bounds.height/1.45)));
        __pLabelMainDescription->RequestRedraw();
        __pLabelMainDescriptionBackground->SetShowState(true);
        __pLabelMainDescriptionBackground->SetTextConfig(28, LABEL_TEXT_STYLE_NORMAL);

        __pLabelMainDescriptionBackground->SetPosition((bounds.x + 40 + 1), (bounds.height - bounds.height/2.3 + 1));
        __pLabelMainDescriptionBackground->SetSize(Dimension(bounds.width - 40, (bounds.height - bounds.height/1.45)));
        __pLabelMainDescriptionBackground->RequestRedraw();


        __pLabelMainIcon->SetSize(Dimension(210, 210));

        /* Main Temperature */
        __pLabelMainTemperatureBackground->SetPosition((int)(bounds.x + bounds.width - background_width2_2/1.4 + 1), (int)(bounds.y + background_height2_2/2.5 + 1));
        __pLabelMainTemperature->SetPosition((int)(bounds.x + bounds.width - background_width2_2/1.4), (int)(bounds.y + background_height2_2/2.5));
        __pLabelMainTemperature->SetSize((int)(background_width2_2/1.3), (int)(background_height2_2/2.5));
        __pLabelMainTemperatureBackground->SetSize((int)(background_width2_2/1.3), (int)(background_height2_2/2.5));
        if (__pLabelMainTemperature->GetText().GetLength()<6){
            __pLabelMainTemperature->SetTextConfig(90, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(90, LABEL_TEXT_STYLE_BOLD);
        }else{
            if (__pLabelMainTemperature->GetText().GetLength()<9){
                __pLabelMainTemperature->SetTextConfig(55, LABEL_TEXT_STYLE_BOLD);
                __pLabelMainTemperatureBackground->SetTextConfig(55, LABEL_TEXT_STYLE_BOLD);
            }else{
                __pLabelMainTemperature->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
                __pLabelMainTemperatureBackground->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
            }
        }
       __pLabelMainWindIcon->SetSize(__pLabelMainWindIcon->GetWidth()*2, __pLabelMainWindIcon->GetHeight()*2);
       __pLabelMainWindIcon->SetPosition((int)(bounds.x + bounds.width - background_width2_2/3.2), (int)(bounds.y + background_height2_2/6.5));
       __pLabelMainWindSpeed->SetPosition((int)(bounds.x + bounds.width - background_width2_2/3.55), (int)(bounds.y + background_height2_2/4.55));

       __pLabelMainWindSpeed->SetSize(1.30*(52+10), (62));
       __pLabelMainWindSpeed->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
    }
    __pPanel->AddControl(__pLabelBackground1);
    __pPanel->AddControl(__pLabelMainIcon);
    __pPanel->AddControl(__pLabelMainWindIcon);
    __pPanel->AddControl(__pLabelMainWindSpeed);
    __pPanel->AddControl(__pLabelMainTemperatureBackground);
    __pPanel->AddControl(__pLabelMainTemperature);
    __pPanel->AddControl(__pLabelMainDescriptionBackground);
    __pPanel->AddControl(__pLabelMainDescription);
    __pPanel->AddControl(__pLabelBackgroundTown);
    __pPanel->AddControl(__pLabelTown);
 
    this->AddControl(*__pPanel);

	return E_SUCCESS;
}

void
MeecastDynamicBoxAppFrame::OnAppWidgetUpdate(void)
{

    char  buffer[4096]; 
	// TODO:
	// Add your code to update AppWidget here
    /* AppLog("OnAppWidgetUpdate"); */

    _config->ReLoadConfig();
    Rectangle bounds = GetBounds();
    if (_dp)
        _dp->DeleteInstance();
    _dp = Core::DataParser::Instance(_config->stationsList().at(_config->current_station_id())->fileName().c_str(), "");

    __pLabelMainWindIcon->SetShowState(false);
    __pLabelMainWindSpeed->SetShowState(false);
    Core::Data *temp_data = NULL;
    if (_dp != NULL && (temp_data = _dp->data().GetDataForTime(time(NULL)))){ 
        temp_data->temperature_low().units(_config->TemperatureUnit());
        temp_data->temperature_hi().units(_config->TemperatureUnit());
        temp_data->temperature().units(_config->TemperatureUnit());
        temp_data->WindSpeed().units(_config->WindSpeedUnit());

        /* Main Icon */
        Tizen::Base::Integer icon_int =  temp_data->Icon();
        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/" + _config->iconSet().c_str() + "/" + icon_int.ToString() + ".png")){
            /* Main Icon */ 
            Tizen::Media::Image *image = null;
            Tizen::Graphics::Bitmap* mainIconBitmap = null;
            image = new (std::nothrow) Tizen::Media::Image();
            image->Construct();

            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/"  + _config->iconSet().c_str() + "/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
            __pLabelMainIcon->SetBackgroundBitmap(*mainIconBitmap);
            __pLabelMainIcon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }

        /* Description */
        String str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_(temp_data->Text().c_str()), str);
        __pLabelMainDescription->SetText(str);
        __pLabelMainDescriptionBackground->SetText(str);

        /* Temperature */
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
//        Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(t);
//        backgroundPanel->SetBackgroundColor(*color_of_temp);
//        delete color_of_temp;
//        __pLabelMainTemperature->SetShowState(true);
//        __pLabelMainTemperatureBackground->SetShowState(true);
        Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
        __pLabelMainTemperature->SetText(str);
        __pLabelMainTemperatureBackground->SetText(str);

        /* Prepare Mod name */
        String temp_str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);

        /* For 1x1 mode */
        if ((bounds.height == bounds.width) && bounds.height<200){
            __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN(L"Widget/" + temp_str + L"/single_now1_1.png"));
            __pLabelBackground1->RequestRedraw();
            if (str.GetLength()<8){
                __pLabelMainTemperature->SetTextConfig(44, LABEL_TEXT_STYLE_BOLD);
                __pLabelMainTemperatureBackground->SetTextConfig(44, LABEL_TEXT_STYLE_BOLD);
            }else{
                __pLabelMainTemperature->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
                __pLabelMainTemperatureBackground->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
            }
            __pLabelMainTemperature->RequestRedraw();
            __pLabelMainTemperatureBackground->RequestRedraw();
        }

        /* Prepared for 2x1 mode */
        if ((bounds.height != bounds.width) && bounds.height<200){
            __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN(L"Widget/" + temp_str + L"/single_now1_2.png"));
     
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

        /* Prepared for 2x2 mode */
        if ((bounds.height == bounds.width) && bounds.height>200){
            __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN(L"Widget/" + temp_str + L"/single_now2_2.png"));
            if (__pLabelMainTemperature->GetText().GetLength()<6){
                __pLabelMainTemperature->SetTextConfig(90, LABEL_TEXT_STYLE_BOLD);
                __pLabelMainTemperatureBackground->SetTextConfig(90, LABEL_TEXT_STYLE_BOLD);
            }else{
                if (__pLabelMainTemperature->GetText().GetLength()<9){
                    __pLabelMainTemperature->SetTextConfig(55, LABEL_TEXT_STYLE_BOLD);
                    __pLabelMainTemperatureBackground->SetTextConfig(55, LABEL_TEXT_STYLE_BOLD);
                }else{
                    __pLabelMainTemperature->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
                    __pLabelMainTemperatureBackground->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
                }
            }
        }

        /* Main wind direction */
        if (temp_data->WindDirection() != "N/A"){
            snprintf (buffer, sizeof(buffer) -1, "%s", temp_data->WindDirection().c_str());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            if (temp_data->WindSpeed().value() > STRONG_WIND){
                str.Append("_warning");
            }
            /* AppLog("Wind1 %S", str.GetPointer()); */
            String temp_str;
            Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);
            if (str == "CALM" || Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/" + temp_str + "/wind_" + str + ".png")){
                __pLabelMainWindIcon->SetShowState(true);
                /* Wind direction Icon */ 
                Tizen::Media::Image *image = null;
                Tizen::Graphics::Bitmap* windIconBitmap = null;
                image = new (std::nothrow) Tizen::Media::Image();
                image->Construct();
                
                if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"720x1280/" + temp_str + "/wind_" + str + ".png")){
                    windIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"720x1280/" + temp_str + "/wind_" + str + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
                    AppLog("Wind %S", str.GetPointer());
                    __pLabelMainWindIcon->SetBackgroundBitmap(*windIconBitmap);
                    __pLabelMainWindIcon->RequestRedraw();
                    SAFE_DELETE(image);
                    SAFE_DELETE(windIconBitmap);
                }
            }
        }

        /* Main wind speed */
        if (_config->Mod() == "Digia"){
            __pLabelMainWindSpeed->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
        }else if (_config->Mod() == "Marina"){
            __pLabelMainWindSpeed->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
        }else if (_config->Mod() == "ExtraCoffe"){
            __pLabelMainWindSpeed->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
        }
        if (temp_data->WindSpeed().value() != INT_MAX){
            __pLabelMainWindSpeed->SetShowState(true);
            snprintf (buffer, sizeof(buffer) -1, "%0.f", 
                                             temp_data->WindSpeed().value());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            /* AppLog("Wind Speed %S", str.GetPointer()); */
            __pLabelMainWindSpeed->SetText(str);
            __pLabelMainWindSpeed->RequestRedraw();
        }else{
            __pLabelMainWindSpeed->SetShowState(false);
        }
    }else{
        __pLabelMainTemperature->SetText("MeeCast");
        __pLabelMainTemperatureBackground->SetText("MeeCast");
        __pLabelMainTemperature->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
        __pLabelMainTemperatureBackground->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);

        Tizen::Base::Integer icon_int = 49;
        if (Tizen::Io::File::IsFileExist(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/" + _config->iconSet().c_str() + "/" + icon_int.ToString() + ".png")){
            /* Main Icon */ 
            Tizen::Media::Image *image = null;
            Tizen::Graphics::Bitmap* mainIconBitmap = null;

            image = new (std::nothrow) Tizen::Media::Image();
            image->Construct();

            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/" + _config->iconSet().c_str() + "/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
            __pLabelMainIcon->SetBackgroundBitmap(*mainIconBitmap);
            __pLabelMainIcon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }
    }

    /* Town */
    String str;
    Tizen::Base::Utility::StringUtil::Utf8ToString(_config->stationname().c_str(), str);
    __pLabelTown->SetText(str);
    __pLabelBackgroundTown->SetText(str);

	Invalidate(true);
  /* AppLog("OnAppWidgetUpdate Done"); */
}

result
MeecastDynamicBoxAppFrame::OnTerminating(void)
{
	// TODO:
	// Add your termination code here

	return E_SUCCESS;
}

void
MeecastDynamicBoxAppFrame::OnBoundsChanged(const Tizen::Graphics::Rectangle& oldRect, const Tizen::Graphics::Rectangle& newRect)
{
    Rectangle bounds = GetBounds();
    /* AppLog(" bounds.height %i bounds.width %i", bounds.height, bounds.width); */
    __pPanel->SetSize(Dimension(bounds.width, bounds.height));

    
    /* For 1x1 mode */
    if ((bounds.height == bounds.width) && bounds.height < 200){
        /* AppLog("1x1"); */
        if ((oldRect.height == oldRect.width) && oldRect.height > 300){
            __pLabelMainWindIcon->SetSize(__pLabelMainWindIcon->GetWidth()/2,
                                          __pLabelMainWindIcon->GetHeight()/2);
        }

        __pLabelMainIcon->SetSize(Dimension(128, 128));
        __pLabelBackground1->SetSize(Dimension(background_width1_1, background_height1_1));
        __pLabelBackground1->SetPosition((bounds.x + bounds.width - background_width1_1),
                                         (bounds.y + bounds.height - background_height1_1));

        String temp_str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);
        __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN(L"Widget/" + temp_str + L"/single_now1_1.png"));
        __pLabelBackground1->RequestRedraw();

        __pLabelMainDescription->SetShowState(false);

        __pLabelMainTemperatureBackground->SetPosition((int)(bounds.x + bounds.width - background_width1_1 + 1 - 20), (int)(bounds.y + bounds.height/2 +1));
        __pLabelMainTemperature->SetPosition((int)(bounds.x + bounds.width - background_width1_1 - 20), (int)(bounds.y + bounds.height/2));
        __pLabelMainTemperatureBackground->SetSize(background_width1_1 + 40, bounds.height/3);
        __pLabelMainTemperature->SetSize(background_width1_1 + 40, bounds.height/3);
        if (__pLabelMainTemperature->GetText().GetLength()<8){
            __pLabelMainTemperature->SetTextConfig(44, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(44, LABEL_TEXT_STYLE_BOLD);
        }else{
            __pLabelMainTemperature->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
        }
        __pLabelMainTemperature->SetSize(background_width1_1 + 40, bounds.height/3);
        __pLabelMainTemperatureBackground->SetSize(background_width1_1 + 40, bounds.height/3);
        __pLabelMainTemperatureBackground->SetPosition((bounds.x + bounds.width - background_width1_1 + 1 - 20), (bounds.height - bounds.height/3 + 1));
         __pLabelMainTemperature->SetPosition((bounds.x + bounds.width - background_width1_1  - 20), (bounds.height - bounds.height/3 ));
        __pLabelMainTemperature->RequestRedraw();
        __pLabelMainTemperatureBackground->RequestRedraw();

        __pLabelTown->SetShowState(false);
        __pLabelBackgroundTown->SetShowState(false);

        __pLabelMainWindIcon->SetPosition((int)(bounds.x + bounds.width - background_width1_1/2.8), (int)(bounds.y + background_height1_1/2.8));

        __pLabelMainWindIcon->RequestRedraw();

        __pLabelMainWindSpeed->SetPosition((int)(bounds.x + bounds.width - background_width1_1/2.8) - 6, (int)(bounds.y + background_height1_1/2.8));
        __pLabelMainWindSpeed->SetSize(52+10, 52);
 
        __pLabelMainWindSpeed->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);
        __pLabelMainWindSpeed->RequestRedraw();
    }

    /* For 2x1 mode */
    if ((bounds.height != bounds.width) && bounds.height<200){
        AppLog("2x1");
        __pLabelMainIcon->SetSize(Dimension(128, 128));
        if ((oldRect.height == oldRect.width) && oldRect.height >300){
            __pLabelMainWindIcon->SetSize(__pLabelMainWindIcon->GetWidth()/2, __pLabelMainWindIcon->GetHeight()/2);
        }
        String temp_str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);
        __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN(L"Widget/" + temp_str + L"/single_now1_2.png"));
        __pLabelBackground1->SetSize(Dimension(background_width2_1, background_height2_1));

         __pLabelMainWindIcon->SetPosition((int)(bounds.x + bounds.width - background_width2_1/6), (int)(bounds.y + background_height2_1/2.8));
         __pLabelMainWindSpeed->SetPosition((int)(bounds.x + bounds.width - background_width2_1/6) - 6, (bounds.y + background_height2_1/2.8) - 1);

         __pLabelMainTemperatureBackground->SetPosition((int)(bounds.x + bounds.width - background_width2_1/1.4 + 1), (int)(bounds.y + background_height2_1/3 +1));
         __pLabelMainTemperature->SetPosition((int)(bounds.x + bounds.width - background_width2_1/1.4), (int)(bounds.y + background_height2_1/3));

        __pLabelMainTemperature->SetSize((background_width2_1/1.6), bounds.height/3);
        __pLabelMainTemperatureBackground->SetSize((background_width2_1/1.6), bounds.height/3);

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
       __pLabelMainTemperature->RequestRedraw();
       __pLabelMainTemperatureBackground->RequestRedraw();
       

       if (__pLabelTown->GetText().GetLength()>15){
            if (__pLabelTown->GetText().GetLength()>20){
                __pLabelTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
                __pLabelBackgroundTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
            }else{
                __pLabelTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
                __pLabelBackgroundTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
            }
       }else{
            __pLabelTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
            __pLabelBackgroundTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
       }
       __pLabelBackgroundTown->SetShowState(true);
       __pLabelBackgroundTown->SetSize(Dimension(bounds.width + 40, (bounds.height - bounds.height/2)));
       __pLabelBackgroundTown->SetPosition((bounds.x  + 1), (bounds.height - bounds.height/2.5 + 1));
       __pLabelTown->SetShowState(true);
       __pLabelTown->SetSize(Dimension(bounds.width + 40, (bounds.height - bounds.height/2)));
       __pLabelTown->SetPosition((bounds.x), (bounds.height - bounds.height/2.5));
       __pLabelTown->RequestRedraw();

       __pLabelMainDescription->SetShowState(false);
       __pLabelMainWindSpeed->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);
       __pLabelMainWindSpeed->SetSize((52+10), (52));
       __pLabelMainWindSpeed->RequestRedraw();
    }

    /* Prepared for 2x2 mode */
    if ((bounds.height == bounds.width) && bounds.height>200){
        AppLog("2x2");
        String temp_str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(_config->Mod().c_str(), temp_str);
        __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN(L"Widget/" + temp_str + L"/single_now2_2.png"));
        __pLabelBackground1->SetSize(Dimension(background_width2_2, background_height2_2));
        __pLabelBackground1->SetPosition((bounds.x + bounds.width - background_width2_2), (bounds.y + bounds.height - background_height2_2));
        if (__pLabelTown->GetText().GetLength()>15){
            if (__pLabelTown->GetText().GetLength()>20){
                __pLabelTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
                __pLabelBackgroundTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
            }else{
                __pLabelTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
                __pLabelBackgroundTown->SetTextConfig(40, LABEL_TEXT_STYLE_NORMAL);
            }
        }else{
            __pLabelTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
            __pLabelBackgroundTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
        }
        __pLabelBackgroundTown->SetShowState(true);
        __pLabelBackgroundTown->SetSize(Dimension(bounds.width + 40, (bounds.height - bounds.height/2)));
        __pLabelBackgroundTown->SetPosition((bounds.x  + 1), (bounds.height - bounds.height/3 + 1));

        __pLabelTown->SetShowState(true);
        __pLabelTown->SetSize(Dimension(bounds.width + 40, (bounds.height - bounds.height/2)));
        __pLabelTown->SetPosition((bounds.x), (bounds.height - bounds.height/3));

        __pLabelMainDescription->SetShowState(true);
        __pLabelMainDescription->SetTextConfig(28, LABEL_TEXT_STYLE_NORMAL);

        __pLabelMainDescription->SetPosition((bounds.x + 40), (bounds.height - bounds.height/2.3));
        __pLabelMainDescription->SetSize(Dimension(bounds.width - 40, (bounds.height - bounds.height/1.45)));
        __pLabelMainDescription->RequestRedraw();

        __pLabelMainDescriptionBackground->SetShowState(true);
        __pLabelMainDescriptionBackground->SetTextConfig(28, LABEL_TEXT_STYLE_NORMAL);

        __pLabelMainDescriptionBackground->SetPosition((bounds.x + 40 + 1), (bounds.height - bounds.height/2.3 + 1));
        __pLabelMainDescriptionBackground->SetSize(Dimension(bounds.width - 40, (bounds.height - bounds.height/1.45)));
        __pLabelMainDescriptionBackground->RequestRedraw();


        __pLabelMainIcon->SetSize(Dimension(210, 210));
        __pLabelMainTemperatureBackground->SetPosition((int)(bounds.x + bounds.width - background_width2_2/1.4 + 1), (int)(bounds.y + background_height2_2/2.5 + 1));
        __pLabelMainTemperature->SetPosition((int)(bounds.x + bounds.width - background_width2_2/1.4), (int)(bounds.y + background_height2_2/2.5));
        __pLabelMainTemperature->SetSize((int)(background_width2_2/1.3), (int)(background_height2_2/2.5));
        __pLabelMainTemperatureBackground->SetSize((int)(background_width2_2/1.3), (int)(background_height2_2/2.5));
        if (__pLabelMainTemperature->GetText().GetLength()<6){
            __pLabelMainTemperature->SetTextConfig(90, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(90, LABEL_TEXT_STYLE_BOLD);
        }else{
            if (__pLabelMainTemperature->GetText().GetLength()<9){
                __pLabelMainTemperature->SetTextConfig(55, LABEL_TEXT_STYLE_BOLD);
                __pLabelMainTemperatureBackground->SetTextConfig(55, LABEL_TEXT_STYLE_BOLD);
            }else{
                __pLabelMainTemperature->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
                __pLabelMainTemperatureBackground->SetTextConfig(40, LABEL_TEXT_STYLE_BOLD);
            }
        }
        /* Wind Icon */
       __pLabelMainWindIcon->SetSize(__pLabelMainWindIcon->GetWidth()*2, __pLabelMainWindIcon->GetHeight()*2);
       __pLabelMainWindIcon->SetPosition((int)(bounds.x + bounds.width - background_width2_2/3.2), (int)(bounds.y + background_height2_2/6.5));

       /* Wind Speed */
       __pLabelMainWindSpeed->SetPosition((int)(bounds.x + bounds.width - background_width2_2/3.55), (int)(bounds.y + background_height2_2/4.55));

       __pLabelMainWindSpeed->SetSize(1.30*(52+10), (62));
       __pLabelMainWindSpeed->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);

    }
}

