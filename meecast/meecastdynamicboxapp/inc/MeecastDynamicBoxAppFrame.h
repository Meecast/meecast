#ifndef _MeecastDynamicBoxApp_FRAME_H_
#define _MeecastDynamicBoxApp_FRAME_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include "../../core/config.h"

#include <FShell.h>

class MeecastDynamicBoxAppFrame
	: public Tizen::Shell::AppWidgetFrame
{
public:
	MeecastDynamicBoxAppFrame();
	virtual ~MeecastDynamicBoxAppFrame();
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnBoundsChanged(const Tizen::Graphics::Rectangle& oldRect, const Tizen::Graphics::Rectangle& newRect);
	void OnAppWidgetUpdate(void);

private:
	Tizen::Ui::Controls::Label* __pLabel;
    Core::Config *_config;
};

#endif /* _MeecastDynamicBoxApp_FRAME_H_ */
