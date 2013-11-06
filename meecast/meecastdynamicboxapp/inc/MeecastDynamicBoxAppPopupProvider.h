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


#include <FUix.h>
#include <FShell.h>

/**
 * [MeecastDynamicBoxAppAppWidgetPopupProvider] AppWidget popup provider must inherit from AppWidgetPopupProvider class
 * which provides basic features necessary to define a AppWidget popup.
 */
class MeecastDynamicBoxAppPopupProvider
	: public Tizen::Shell::AppWidgetPopupProvider
	, public Tizen::Ui::IActionEventListener
    , public Tizen::Ui::ITouchFlickGestureEventListener
    , public Tizen::Ui::ITouchEventListener
    , public Tizen::Uix::Speech::ITextToSpeechEventListener
{
public:
	MeecastDynamicBoxAppPopupProvider();
	virtual ~MeecastDynamicBoxAppPopupProvider();
    void Update();
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
// from ITouchEventListener
	virtual void OnTouchPressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchDoublePressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchFocusIn(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchFocusOut(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchLongPressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchMoved(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchReleased(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo);
    //Gesture
	virtual void OnFlickGestureDetected(Tizen::Ui::TouchFlickGestureDetector& gestureDetector);
	virtual void OnFlickGestureCanceled(Tizen::Ui::TouchFlickGestureDetector& gestureDetector);
    // Speech
	virtual void OnTextToSpeechInitialized(void);
	virtual void OnTextToSpeechStatusChanged(Tizen::Uix::Speech::TextToSpeechStatus status);
	virtual void OnTextToSpeechErrorOccurred(Tizen::Uix::Speech::TextToSpeechError error);
	virtual void OnTextToSpeechCompleted(void);

    void NextStation();
    void PreviousStation();
    void ReInitElements();



protected:
	virtual bool OnAppWidgetPopupProviderInitializing(const Tizen::Base::String& userInfo);
	virtual void OnAppWidgetPopupProviderTerminating(void);
	bool __gestureDetected;
	Tizen::Ui::TouchFlickGestureDetector* __pFlickGesture;
    static const int ID_BUTTON_LAUNCHER = 101;
	Tizen::Uix::Speech::TextToSpeech* __pTts;
private:
    Core::Config *_config;
    Core::DataParser* _dp;
    Tizen::Shell::AppWidgetPopup::AppWidgetPopup* __pAppWidgetPopup;
    static const float width =  720.0;
	static const float height =  380.0;
    Tizen::Ui::Controls::Label* __pLabelLastUpdate;
    Tizen::Ui::Controls::Label* __pLabelLastUpdateBG;
    Tizen::Ui::Controls::Label* __pLabelTown;
    Tizen::Ui::Controls::Label* __pLabelTownBG;
    Tizen::Ui::Controls::Label* __pLabelMainIcon;
    Tizen::Ui::Controls::Label* __pLabelMainTemperature;
    Tizen::Ui::Controls::Label* __pLabelMainTemperatureBG;
    Tizen::Ui::Controls::Label* __pLabelMainWindSpeed;
    Tizen::Ui::Controls::Label* __pLabelMainWindIcon;

    Tizen::Ui::Controls::Label* icon1;
    Tizen::Ui::Controls::Label* icon2;
    Tizen::Ui::Controls::Label* icon3;
    Tizen::Ui::Controls::Label* icon4;
    Tizen::Ui::Controls::Label* windicon1;
    Tizen::Ui::Controls::Label* windicon2;
    Tizen::Ui::Controls::Label* windicon3;
    Tizen::Ui::Controls::Label* windicon4;
    Tizen::Ui::Controls::Label* windspeed1;
    Tizen::Ui::Controls::Label* windspeed2;
    Tizen::Ui::Controls::Label* windspeed3;
    Tizen::Ui::Controls::Label* windspeed4;
    Tizen::Ui::Controls::Label* temperature_hi1;
    Tizen::Ui::Controls::Label* temperature_hi2;
    Tizen::Ui::Controls::Label* temperature_hi3;
    Tizen::Ui::Controls::Label* temperature_hi4;
    Tizen::Ui::Controls::Label* temperature_low1;
    Tizen::Ui::Controls::Label* temperature_low2;
    Tizen::Ui::Controls::Label* temperature_low3;
    Tizen::Ui::Controls::Label* temperature_low4;
    Tizen::Ui::Controls::Label* dayname1;
    Tizen::Ui::Controls::Label* dayname2;
    Tizen::Ui::Controls::Label* dayname3;
    Tizen::Ui::Controls::Label* dayname4;

};

#endif /* _MeecastDynamicBoxApp_POPUP_PROVIDER_H_ */
