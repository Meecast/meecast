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
#ifndef _MeecastDynamicBoxApp_POPUP_PROVIDER_H_
#define _MeecastDynamicBoxApp_POPUP_PROVIDER_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FMedia.h>
#include "../../core/core.h"
#include "../../core/config.h"
#include <FSystem.h>
#include <FLocales.h>



#include <FShell.h>

/**
 * [MeecastDynamicBoxAppAppWidgetPopupProvider] AppWidget popup provider must inherit from AppWidgetPopupProvider class
 * which provides basic features necessary to define a AppWidget popup.
 */
class MeecastDynamicBoxAppPopupProvider
	: public Tizen::Shell::AppWidgetPopupProvider
{
public:
	MeecastDynamicBoxAppPopupProvider();
	virtual ~MeecastDynamicBoxAppPopupProvider();
    void Update();

protected:
	virtual bool OnAppWidgetPopupProviderInitializing(const Tizen::Base::String& userInfo);
	virtual void OnAppWidgetPopupProviderTerminating(void);
private:
    Core::Config *_config;
    Core::DataParser* _dp;
};

#endif /* _MeecastDynamicBoxApp_POPUP_PROVIDER_H_ */
