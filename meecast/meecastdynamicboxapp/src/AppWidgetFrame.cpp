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
    //        ByteBuffer* pBuf = null;
//        String filepath = App::GetInstance()->GetAppDataPath();
//        filepath.Append("config.xml");
 //       pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(filepath);

    //    config = ConfigTizen::Instance( std::string((const char*)pBuf->GetPointer()),
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

	//Form *pForm = static_cast<Form*>(GetParent());
	//AppAssert(pForm);
//    RelativeLayout* pLayout = dynamic_cast< RelativeLayout* >(this->GetLayoutN());
//	AppAssert(pLayout);

	FloatRectangle bounds = this->GetBoundsF();

    RelativeLayout topPanelLayout;
    topPanelLayout.Construct();

    __pPanel = new Panel();
    __pPanel->Construct(topPanelLayout, bounds);

//	__pPanel->SetBackgroundColor(Color::GetColor(COLOR_ID_YELLOW));
    #define background_width1_1 140
    #define background_height1_1 134

    __pLabelBackground1 = new Label();
    __pLabelBackground1->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1), (bounds.y + bounds.height - background_height1_1), background_width1_1, background_height1_1), L"");

    __pLabelMainIcon = new Label();
    __pLabelMainIcon->Construct(FloatRectangle(bounds.x - 0, bounds.y - 0, 128, 128), L"");

    __pLabelMainWindIcon = new Label();
    __pLabelMainWindIcon->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1/2.8), bounds.y + background_height1_1/2.8, 52, 42), L"");

    __pLabelMainTemperatureBackground = new Label();
    __pLabelMainTemperatureBackground->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1 + 1), (bounds.height - bounds.height/3 + 1) , background_width1_1, bounds.height/3), L"");

    __pLabelMainTemperature = new Label();
    __pLabelMainTemperature->Construct(FloatRectangle((bounds.x + bounds.width - background_width1_1), (bounds.height - bounds.height/3) , background_width1_1, bounds.height/3), L"");

    __pLabelTown = new Label();
    __pLabelTown->Construct(FloatRectangle(bounds.x - 1, bounds.y+103, bounds.width, bounds.height - 104), _config->stationname().c_str());


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

        int t = INT_MAX;
        /* Temperature */
        if (temp_data->temperature().value(true) == INT_MAX){
          if ((temp_data->temperature_hi().value(true) == INT_MAX) &&
              (temp_data->temperature_low().value(true) == INT_MAX)){ 
            //main_description->SetText("N/A");
          } 
          if ((temp_data->temperature_hi().value(true) != INT_MAX) &&
              (temp_data->temperature_low().value(true) != INT_MAX)){ 
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°/ %0.f°", temp_data->temperature_low().value(),
                                                                 temp_data->temperature_hi().value());
            t = temp_data->temperature_hi().value();
          }  
          if (temp_data->temperature_hi().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_hi().value());
            t = temp_data->temperature_hi().value();
          }
          if (temp_data->temperature_low().value(true) != INT_MAX){
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature_low().value());
            t = temp_data->temperature_low().value();
          }
        }else{
            snprintf(buffer, sizeof(buffer) - 1, "%0.f°", temp_data->temperature().value());
            t = temp_data->temperature().value();
        }
//        Tizen::Graphics::Color*  color_of_temp = GetTemperatureColor(t);
//        backgroundPanel->SetBackgroundColor(*color_of_temp);
//        delete color_of_temp;
        __pLabelMainTemperature->SetShowState(true);
        Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
        __pLabelMainTemperature->SetText(str);
        __pLabelMainTemperature->SetTextConfig(42, LABEL_TEXT_STYLE_BOLD);
        __pLabelMainTemperature->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
        __pLabelMainTemperature->RequestRedraw();

        __pLabelMainTemperatureBackground->SetShowState(true);
        __pLabelMainTemperatureBackground->SetText(str);
        __pLabelMainTemperatureBackground->SetTextConfig(42, LABEL_TEXT_STYLE_BOLD);
        __pLabelMainTemperatureBackground->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
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

        /* Main wind direction */
        if (temp_data->WindDirection() != "N/A"){
            snprintf (buffer, sizeof(buffer) -1, "%s", temp_data->WindDirection().c_str());
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
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
#if 0
        /* Main wind speed */
        if (temp_data->WindSpeed().value() != INT_MAX){
            main_wind_speed_text->SetShowState(true);
            main_wind_speed_icon->SetShowState(true);
            snprintf (buffer, sizeof(buffer) -1, "%0.f %s", 
                                             temp_data->WindSpeed().value(), _(_config->WindSpeedUnit().c_str()));
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            main_wind_speed_text->SetText(str);
            main_wind_speed_text->RequestRedraw();
        }else{
            main_wind_speed_text->SetShowState(false);
            main_wind_speed_icon->SetShowState(false);
        }
        /* Main presssure */
        if (temp_data->pressure().value(true) != INT_MAX){
            snprintf (buffer, sizeof(buffer) -1, "%i %s", (int)temp_data->pressure().value(), _(_config->PressureUnit().c_str()));
            main_pressure_text->SetShowState(true);
            main_pressure_icon->SetShowState(true);
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer, str);
            main_pressure_text->SetText(str);
            main_pressure_text->RequestRedraw();
        }

#endif
    }
            //Adds the panel to the form
            //    AddControl(__pPanel);
            //
    if ((bounds.height == bounds.width) && bounds.height<200){
       __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/single_now.png"));
       __pPanel->AddControl(__pLabelBackground1);
/*
	   __pLabelBackground2 = new Label();
	   __pLabelBackground2->Construct(FloatRectangle(bounds.x, bounds.y+104, bounds.width, bounds.height - 104), L"");
       __pLabelBackground2->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/OMW_widget_button_single_town.png"));
       __pPanel->AddControl(__pLabelBackground2);
*/
       __pLabelBackgroundTown = new Label();
	   __pLabelBackgroundTown->Construct(FloatRectangle(bounds.x, bounds.y+104, bounds.width, bounds.height - 104), _config->stationname().c_str());
       __pLabelBackgroundTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
       __pLabelBackgroundTown->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
	   __pLabelBackgroundTown->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	   __pLabelBackgroundTown->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

       __pPanel->AddControl(__pLabelBackgroundTown);
       __pLabelBackgroundTown->SetShowState(false);

       __pLabelTown->SetTextConfig(24, LABEL_TEXT_STYLE_NORMAL);
       __pLabelTown->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
	   __pLabelTown->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	   __pLabelTown->SetTextHorizontalAlignment(ALIGNMENT_CENTER);
       __pPanel->AddControl(__pLabelTown);
       __pLabelTown->SetShowState(false);



    //    AddControl(*__pLabelBackground);
    
//    topPanelLayout.SetHorizontalFitPolicy(*__pLabelBackground1, FIT_POLICY_PARENT);
//    topPanelLayout.SetRelation(*__pLabelBackground1, *this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
//    topPanelLayout.SetRelation(*__pLabelBackground1, *this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
//    topPanelLayout.SetRelation(*__pLabelBackground1, *this, RECT_EDGE_RELATION_TOP_TO_TOP);
  //  topPanelLayout.SetRelation(*__pLabelBackground1, __pPanel, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);
  //  topPanelLayout.SetMargin(*__pLabelBackground1, 0, 0, 0, 70);
    //topPanelLayout.SetRelation(*__pLabelBackground1, *__pLabelBackground2, RECT_EDGE_RELATION_BOTTOM_TO_TOP);


//    topPanelLayout.SetHorizontalFitPolicy(*__pLabelBackground2, FIT_POLICY_PARENT);
//    topPanelLayout.SetRelation(*__pLabelBackground2, *this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
 //   topPanelLayout.SetRelation(*__pLabelBackground2, *this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
 //   topPanelLayout.SetRelation(*__pLabelBackground2, *__pLabelBackground1, RECT_EDGE_RELATION_TOP_TO_BOTTOM);
 //   topPanelLayout.SetRelation(*__pLabelBackground2, *this, RECT_EDGE_RELATION_BOTTOM_TO_TOP);

    }
    __pPanel->AddControl(__pLabelMainIcon);
    __pPanel->AddControl(__pLabelMainWindIcon);
    __pPanel->AddControl(__pLabelMainTemperatureBackground);
    __pPanel->AddControl(__pLabelMainTemperature);

    AppLog ("33333");

    this->AddControl(*__pPanel);
	// TODO:
	// Add your initialization code here
	__pLabel = new Label();
	__pLabel->Construct(bounds, L"Hello Dynamic Box!");
	__pLabel->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
	__pLabel->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	__pLabel->SetTextHorizontalAlignment(ALIGNMENT_CENTER);
	__pLabel->SetBackgroundColor(Color::GetColor(COLOR_ID_YELLOW));
//	this->AddControl(__pLabel);

    Tizen::Base::ByteBuffer* pBuf = null;
    Tizen::Base::String filepath = App::GetInstance()->GetAppDataPath() + "config.xml";
        pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(filepath);
        AppLogDebug("Config file %s", pBuf->GetPointer());

    AppLog ("44444");
	return E_SUCCESS;
}

void
MeecastDynamicBoxAppFrame::OnAppWidgetUpdate(void)
{
	// TODO:
	// Add your code to update AppWidget here

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
	// TODO:
	// Add your code to resize AppWidget here
	if (__pLabel)
	{
		__pLabel->SetBounds(newRect);
	}

}

