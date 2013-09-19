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
    delete _config;
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

	FloatRectangle bounds = this->GetBoundsF();
    if ((bounds.height == bounds.width) && bounds.height<200){

    }
	// TODO:
	// Add your initialization code here
	__pLabel = new Label();
	__pLabel->Construct(bounds, L"Hello Dynamic Box!");
	__pLabel->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
	__pLabel->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
	__pLabel->SetTextHorizontalAlignment(ALIGNMENT_CENTER);
	__pLabel->SetBackgroundColor(Color::GetColor(COLOR_ID_YELLOW));
	this->AddControl(__pLabel);

    Tizen::Base::ByteBuffer* pBuf = null;
    Tizen::Base::String filepath = App::GetInstance()->GetAppDataPath() + "config.xml";
        pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(filepath);
        AppLogDebug("Config file %s", pBuf->GetPointer());

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

