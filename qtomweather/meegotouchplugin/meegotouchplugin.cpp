/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include <MButton>
#include <MContainer>
#include "meegotouchplugin.h"
#include <MLibrary>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include "dbusadaptor.h"
#include "eventfeedif.h"



M_LIBRARY
Q_EXPORT_PLUGIN2(weatherextension, WeatherApplicationExtension)



WeatherApplicationExtension::WeatherApplicationExtension() : box(0){
}

WeatherApplicationExtension::~WeatherApplicationExtension(){
    delete box;
}

void 
WeatherApplicationExtension::weatherExtensionSpecificOperation(){
    // do something specific to the demo extension interface
}


bool 
WeatherApplicationExtension::initialize(const QString &){
   QGraphicsObject* mWidget;

   /* QDeclarative QML */
   QDeclarativeView* view = new QDeclarativeView();
   view->setSource(QUrl::fromLocalFile("/opt/com.meecast.omweather/share/omweather/qml/applet.qml"));
   box = new MyMWidget();
   mWidget = qobject_cast<QGraphicsObject*>(view->rootObject());
   mWidget->setParentItem(box);
   view->rootContext()->setContextProperty("Applet", box);

   /* D-BUS */
   new MeecastIf(box);
   QDBusConnection connection = QDBusConnection::sessionBus();
   bool ret = connection.registerService("com.meecast.applet");
   ret = connection.registerObject("/com/meecast/applet", box);
   EventFeedIf* client =  new EventFeedIf("com.nokia.home.EventFeed", "/eventfeed", QDBusConnection::sessionBus(), 0); 
   QObject::connect(client, SIGNAL(refreshRequested()), box, SLOT(refreshRequested1()));  
   box->startpredeamon();
   return true;
}

MWidget *WeatherApplicationExtension::widget(){
    return box;
}

void 
MyMWidget::SetCurrentData(const QString &station, const QString &temperature, const QString &temperature_high, const QString &temperature_low,  const QString &icon, const uint until_valid_time, bool current){

   QDateTime utc_time;
   utc_time = QDateTime::currentDateTimeUtc();

   this->temperature(temperature);
   this->temperature_high(temperature_high);
   this->temperature_low(temperature_low);
   this->station(station);
   this->icon(icon);
   this->current(current);
   this->refreshview();
   if ((until_valid_time - utc_time.toTime_t()) >0 && (until_valid_time - utc_time.toTime_t()) < 12* 3600)
        QTimer::singleShot((until_valid_time - utc_time.toTime_t() + 600000), this, SLOT(update_data()));
   else
        QTimer::singleShot(36000000, this, SLOT(update_data()));


}
void 
MyMWidget::refreshRequested1(){
    this->startpredeamon();
}
void MyMWidget::update_data(){
    this->startpredeamon();
}

