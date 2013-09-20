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

	__pPanel->SetBackgroundColor(Color::GetColor(COLOR_ID_YELLOW));

            //Adds the panel to the form
            //    AddControl(__pPanel);
            //
    if ((bounds.height == bounds.width) && bounds.height<200){
	   __pLabelBackground1 = new Label();
	   __pLabelBackground1->Construct(FloatRectangle(bounds.x, bounds.y, bounds.width, 104), L"Hello Dynamic Box!");
       __pLabelBackground1->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/OMW_widget_background_single_now.png"));
       __pPanel->AddControl(__pLabelBackground1);

	   __pLabelBackground2 = new Label();
	   __pLabelBackground2->Construct(FloatRectangle(bounds.x, bounds.y+104, bounds.width, bounds.height - 104), L"");
       __pLabelBackground2->SetBackgroundBitmap(*Application::GetInstance()->GetAppResource()->GetBitmapN("Widget/Digia/OMW_widget_button_single_town.png"));
       __pPanel->AddControl(__pLabelBackground2);

       __pLabelBackgroundTown = new Label();
	   __pLabelBackgroundTown->Construct(FloatRectangle(bounds.x, bounds.y+104, bounds.width, bounds.height - 104), _config->stationname().c_str());
       __pLabelBackgroundTown->SetTextConfig(22, LABEL_TEXT_STYLE_NORMAL);
       __pLabelBackgroundTown->SetTextColor(Color::GetColor(COLOR_ID_BLACK));
	   __pLabelBackgroundTown->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	   __pLabelBackgroundTown->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

       __pPanel->AddControl(__pLabelBackgroundTown);

       __pLabelTown = new Label();
	   __pLabelTown->Construct(FloatRectangle(bounds.x - 1, bounds.y+103, bounds.width + 1, bounds.height - 105), _config->stationname().c_str());
       __pLabelTown->SetTextConfig(22, LABEL_TEXT_STYLE_NORMAL);
       __pLabelTown->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
	   __pLabelTown->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	   __pLabelTown->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

       __pPanel->AddControl(__pLabelTown);

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

