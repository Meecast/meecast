/**
 * Name        : MeecastDynamicBoxAppFrame
 * Version     :
 * Vendor      :
 * Description :
 */

#include "MeecastDynamicBoxAppProvider.h"
#include "MeecastDynamicBoxAppFrame.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::App;
using namespace Core;
#define SAFE_DELETE(x)  if (x) { delete x; x = null; }
#define STRONG_WIND 8 
#define background_width1_1 140
#define background_height1_1 134
#define background_width2_1 320
#define background_height2_1 134




MeecastDynamicBoxAppFrame::MeecastDynamicBoxAppFrame()
{

}

MeecastDynamicBoxAppFrame::~MeecastDynamicBoxAppFrame()
{
    _config->DeleteInstance();
}

result
MeecastDynamicBoxAppFrame::OnInitializing(void)
{ 
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


	FloatRectangle bounds = this->GetBoundsF();

    RelativeLayout topPanelLayout;
    topPanelLayout.Construct();

    __pPanel = new Panel();
    __pPanel->Construct(topPanelLayout, bounds);

//	__pPanel->SetBackgroundColor(Color::GetColor(COLOR_ID_YELLOW));
    __pLabelBackground1 = new Label();

    __pLabelMainIcon = new Label();
    __pLabelMainIcon->Construct(FloatRectangle(bounds.x - 0, bounds.y - 0, 128, 128), L"");

    __pLabelMainWindIcon = new Label();
    __pLabelMainWindIcon->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1/2.8), bounds.y + background_height1_1/2.8, 52, 42), L"");

    __pLabelMainWindSpeed = new Label();
    __pLabelMainWindSpeed->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1/2.8) - 6, (bounds.y + background_height1_1/2.8) - 1, 52+10, 52), L"");
    __pLabelMainWindSpeed->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);
    __pLabelMainWindSpeed->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    __pLabelMainWindSpeed->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelMainWindSpeed->SetTextHorizontalAlignment(ALIGNMENT_CENTER);


    __pLabelMainTemperatureBackground = new Label();
    __pLabelMainTemperatureBackground->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1 + 1 - 20), (bounds.height - bounds.height/3 + 1) , background_width1_1 + 40, bounds.height/3), L"");
    __pLabelMainTemperatureBackground->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    
    __pLabelMainTemperature = new Label();
    __pLabelMainTemperature->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1- 20), (bounds.height - bounds.height/3) , background_width1_1 + 40, bounds.height/3), L"");
    __pLabelMainTemperature->SetTextColor(Color::GetColor(COLOR_ID_WHITE));

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
    __pLabelMainDescription->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
    __pLabelMainDescription->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
    __pLabelMainDescription->SetTextHorizontalAlignment(ALIGNMENT_RIGHT);

    if (_dp)
        _dp->DeleteInstance();
    _dp = Core::DataParser::Instance(_config->stationsList().at(_config->current_station_id())->fileName().c_str(), "");

    Core::Data *temp_data = NULL;
    if (_dp != NULL && (temp_data = _dp->data().GetDataForTime(time(NULL)))){    

        AppLog ("_Config_dp inside");
        /* Preparing units */
        temp_data->temperature_low().units(_config->TemperatureUnit());
        temp_data->temperature_hi().units(_config->TemperatureUnit());
        temp_data->temperature().units(_config->TemperatureUnit());
        temp_data->WindSpeed().units(_config->WindSpeedUnit());
        temp_data->pressure().units(_config->PressureUnit());
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

            String icon_number = temp_data->Icon();

            mainIconBitmap = image->DecodeN(App::GetInstance()->GetAppResourcePath() + L"screen-density-xhigh/icons/Glance/" + icon_int.ToString() + ".png", BITMAP_PIXEL_FORMAT_ARGB8888);
            __pLabelMainIcon->SetBackgroundBitmap(*mainIconBitmap);
            __pLabelMainIcon->RequestRedraw();
            SAFE_DELETE(image);
            SAFE_DELETE(mainIconBitmap);
        }

        String str;
        Tizen::Base::Utility::StringUtil::Utf8ToString(temp_data->Text().c_str(), str);
        __pLabelMainDescription->SetText(str);
        __pLabelMainDescription->RequestRedraw();

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
        __pLabelMainTemperature->SetShowState(true);
        __pLabelMainTemperatureBackground->SetShowState(true);
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
            AppLog("Wind1 %S", str.GetPointer());
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
        }
    }
            //Adds the panel to the form
            //    AddControl(__pPanel);
            //
            //
    /* Prepared for 2x2 mode */
    if ((bounds.width <= (bounds.height*2 + 4)) && bounds.width>300){
        __pLabelBackground1->Construct(FloatRectangle((bounds.x + bounds.width - background_width2_1), (bounds.y + bounds.height - background_height2_1), background_width2_1, background_height2_1), L"");
        __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/single_now1_2.png"));
        __pLabelBackgroundTown->SetShowState(true);
        __pLabelBackgroundTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
        __pLabelTown->SetShowState(true);
        __pLabelTown->SetTextConfig(44, LABEL_TEXT_STYLE_NORMAL);
        __pLabelMainDescription->SetShowState(true);
        __pLabelMainTemperatureBackground->SetPosition((int)(bounds.x + bounds.width - background_width2_1/1.4 + 1), (int)(bounds.y + background_height2_1/3 +1));
        __pLabelMainTemperature->SetPosition((int)(bounds.x + bounds.width - background_width2_1/1.4), (int)(bounds.y + background_height2_1/3));

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

    }
    /* Prepared for 1x2 mode */
    if ((bounds.height == bounds.width) && bounds.height<200){
        __pLabelMainDescription->SetShowState(false);
        __pLabelBackground1->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1), (bounds.y + bounds.height - background_height1_1), background_width1_1, background_height1_1), L"");
       __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/single_now1_1.png"));

       __pLabelBackgroundTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
       __pLabelBackgroundTown->SetShowState(false);

       __pLabelTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
       __pLabelTown->SetShowState(false);
    }

    __pPanel->AddControl(__pLabelBackground1);
    __pPanel->AddControl(__pLabelMainIcon);
    __pPanel->AddControl(__pLabelMainWindIcon);
    __pPanel->AddControl(__pLabelMainWindSpeed);
    __pPanel->AddControl(__pLabelMainTemperatureBackground);
    __pPanel->AddControl(__pLabelMainTemperature);
    __pPanel->AddControl(__pLabelMainDescription);
    __pPanel->AddControl(__pLabelBackgroundTown);
    __pPanel->AddControl(__pLabelTown);

    this->AddControl(*__pPanel);

	return E_SUCCESS;
}

void
MeecastDynamicBoxAppFrame::OnAppWidgetUpdate(void)
{
	// TODO:
	// Add your code to update AppWidget here
    AppLog("OnAppWidgetUpdate");
	Invalidate(true);
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
   AppLog(" bounds.height %i bounds.width %i", bounds.height, bounds.width);

    __pPanel->SetSize(Dimension(bounds.width, bounds.height));
    /* For 2x1 mode */
    if (bounds.width <= (bounds.height*2 + 4)){
        __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/single_now1_2.png"));
         __pLabelBackground1->SetSize(Dimension(background_width2_1, background_height2_1));

         __pLabelMainWindIcon->SetPosition((int)(bounds.x + bounds.width - background_width2_1/6), (int)(bounds.y + background_height2_1/2.8));
         __pLabelMainWindSpeed->SetPosition((int)(bounds.x + bounds.width - background_width2_1/6) - 6, (bounds.y + background_height2_1/2.8) - 1);

         __pLabelMainTemperatureBackground->SetPosition((int)(bounds.x + bounds.width - background_width2_1/1.4 + 1), (int)(bounds.y + background_height2_1/3 +1));
         __pLabelMainTemperature->SetPosition((int)(bounds.x + bounds.width - background_width2_1/1.4), (int)(bounds.y + background_height2_1/3));

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
/*
        __pLabelMainDescription->SetShowState(true);
        __pLabelMainDescription->SetTextConfig(20, LABEL_TEXT_STYLE_NORMAL);
       __pLabelMainDescription->SetSize(Dimension((bounds.width - bounds.width/3 + 10) , (bounds.height - bounds.height/2)));
       __pLabelMainDescription->SetPosition((bounds.x + bounds.width/3), (bounds.height - bounds.height/1.55));
*/
    }

    
    if ((bounds.height == bounds.width) && bounds.height<200){
        AppLog("i1111 bounds.height %i bounds.width %i", bounds.height, bounds.width);
        __pLabelBackground1->SetSize(Dimension(background_width1_1, background_height1_1));
        __pLabelBackground1->SetPosition((bounds.x + bounds.width - background_width1_1), (bounds.y + bounds.height - background_height1_1));
        __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/single_now1_1.png"));
        __pLabelMainDescription->SetShowState(false);

        __pLabelMainTemperatureBackground->SetPosition((int)(bounds.x + bounds.width - background_width1_1 + 1 - 20), (int)(bounds.y + bounds.height/2 +1));
        __pLabelMainTemperature->SetPosition((int)(bounds.x + bounds.width - background_width1_1 - 20), (int)(bounds.y + bounds.height/2));
        if (__pLabelMainTemperature->GetText().GetLength()<8){
            __pLabelMainTemperature->SetTextConfig(44, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(44, LABEL_TEXT_STYLE_BOLD);
        }else{
            __pLabelMainTemperature->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
            __pLabelMainTemperatureBackground->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
        }
        __pLabelMainTemperature->RequestRedraw();
        __pLabelMainTemperatureBackground->RequestRedraw();

        __pLabelTown->SetShowState(false);
        __pLabelBackgroundTown->SetShowState(false);

         __pLabelMainWindIcon->SetPosition((int)(bounds.x + bounds.width - background_width1_1/2.8), (int)(bounds.y + background_height1_1/2.8));

        __pLabelMainWindIcon->RequestRedraw();

        __pLabelMainWindSpeed->SetPosition((int)(bounds.x + bounds.width - background_width1_1/2.8) - 6, (int)(bounds.y + background_height1_1/2.8));

    }
}

