/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen
 *
 * Copyright (C) 2012 - 2013 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
#ifndef _MeecastDynamicBoxApp_FRAME_H_
#define _MeecastDynamicBoxApp_FRAME_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FMedia.h>
#include <FUi.h>
#include "../../core/core.h"
#include "../../core/config.h"
#include "MeecastDynamicMessagePort.h"

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
//	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
	void OnAppWidgetUpdate(void);

private:
    Tizen::Ui::Controls::Panel* __pPanel;
	Tizen::Ui::Controls::Label* __pLabel;
    Tizen::Ui::Controls::Label* __pLabelBackground1;
    Tizen::Ui::Controls::Label* __pLabelBackground2;
    Tizen::Ui::Controls::Label* __pLabelBackgroundTown;
    Tizen::Ui::Controls::Label* __pLabelTown;
    Tizen::Ui::Controls::Label* __pLabelMainIcon;
    Tizen::Ui::Controls::Label* __pLabelMainWindIcon;
    Tizen::Ui::Controls::Label* __pLabelMainWindSpeed;
    Tizen::Ui::Controls::Label* __pLabelMainTemperature;
    Tizen::Ui::Controls::Label* __pLabelMainTemperatureBackground;
    Tizen::Ui::Controls::Label* __pLabelMainDescription;
    Tizen::Ui::Controls::Label* __pLabelMainDescriptionBackground;
    Core::Config *_config;
    Core::DataParser* _dp;
    MeecastDynamicMessagePort* __pMessagePort;
};

#endif /* _MeecastDynamicBoxApp_FRAME_H_ */
