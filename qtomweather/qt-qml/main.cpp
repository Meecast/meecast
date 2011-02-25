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


#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QTranslator>
#include <QHash>
#include <QGraphicsGridLayout>
#include <exception>
#include <iostream>

#include "core.h"
#include "dataqml.h"
#include "configqml.h"
#include "qmllayoutitem.h"
#include "dataitem.h"
#include "datamodel.h"
#include "parserqt.h"
#include "databasesqlite.h"
#include "abstractconfig.h"
#include "dbusadaptor.h" 
#include "controller.h" 

#include <QtDebug>

#include <libintl.h>
#include <locale.h>

#define _(String) gettext(String)



//////////////////////////////////////////////////////////////////////////////
bool
update_weather_forecast(Core::Config *config){
    uint i;
    int success = 0;
    Core::Station* station;

    for (i = 0; i < config->stationsList().size(); i++){
        station = config->stationsList().at(i);
        if (station->updateData(true)){
            success ++;
        }
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////////
Core::DataParser*
current_data(std::string& str){
  Core::DataParser* dp;
  try{
        dp = new Core::DataParser(str, Core::AbstractConfig::prefix+Core::AbstractConfig::schemaPath+"data.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return NULL;
    }
    catch(const char *str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return NULL;
    }
    return dp;
}

int main(int argc, char* argv[])
{
    QApplication::setGraphicsSystem("native");
    QApplication app(argc, argv);
    time_t current_day;
    struct tm   *tm = NULL;
    int year, current_month;

     
    textdomain("omweather");
/*
    //Set up a graphics scene with a QGraphicsWidget and Layout
    QGraphicsView view;
    QGraphicsScene scene;
    QGraphicsWidget *widget = new QGraphicsWidget();
    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    layout->setSpacing(0);
    widget->setLayout(layout);
    scene.addItem(widget);
    view.setScene(&scene);
*/

    //Add the QML snippet into the layout

    ConfigQml *config;
    Controller *controller;
    DataItem *forecast_data = NULL;

    Core::DataParser* dp = NULL;
    Core::Data *temp_data = NULL;
    int i = 0;


    QTranslator translator;
    translator.load("ru.qml", "i18n");
    app.installTranslator(&translator);

    controller = new Controller(); 
    new DbusAdaptor(controller);
//    new DbusAdaptor(config);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("org.meego.omweather");
    connection.registerObject("/org/meego/omweather", controller);
//    connection.registerObject("/org/meego/omweather", config);
    


    config = controller->config();
    std::cerr<<"iconpath = "<<config->prefix_path()<<std::endl;
    //update_weather_forecast(config);
    
        if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0 &&
        config->stationsList().at(config->current_station_id()))
        dp = current_data(config->stationsList().at(config->current_station_id())->fileName());
    DataModel *model = new DataModel(new DataItem, qApp);

    /* set current day */ 
    current_day = time(NULL);
    tm = localtime(&current_day);
    year = 1900 + tm->tm_year;
    current_month = tm->tm_mon;
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    current_day = mktime(tm);
    /* fill current date */
    if  (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL) + i))) {
        forecast_data = new DataItem(temp_data);
        forecast_data->Text(_(forecast_data->Text().c_str()));
        model->appendRow(forecast_data);
    }

    /* set next day */
    i = 3600*24;
    /* fill other days */
    while  (dp != NULL && (temp_data = dp->data().GetDataForTime( current_day + 12 * 3600  + i))) {
        i = i + 3600*24;
        forecast_data = new DataItem(temp_data);
        forecast_data->Text(_(forecast_data->Text().c_str()));
        model->appendRow(forecast_data);

    }
    QDeclarativeView qview;
    qview.rootContext()->setContextProperty("Forecast_model", model);
    qview.rootContext()->setContextProperty("Config", config);
    qview.setSource(QUrl::fromLocalFile(QString::fromStdString(Core::AbstractConfig::layoutqml)));
    qview.show();

    return app.exec();
}
