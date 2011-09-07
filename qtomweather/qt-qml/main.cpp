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

#include "selectmodel.h"

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

int main(int argc, char* argv[])
{
    //QApplication::setGraphicsSystem("native");
    QApplication app(argc, argv);

    app.setProperty("NoMStyle", true);

    QDir::setCurrent(app.applicationDirPath());

    textdomain("omweather");
    bindtextdomain("omweather", "/opt/com.meecast.omweather/share/locale");
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

    //QString locale = QLocale::system().name();
    //std::cerr<<"locale: "<<locale.toStdString()<<std::endl;
    
    //ConfigQml *config;
    Controller *controller;

    QTranslator translator;
    translator.load("ru.qml", "i18n");
    app.installTranslator(&translator);

    controller = new Controller(); 
    DbusAdaptor* dadapt = new DbusAdaptor(controller);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("org.meego.omweather");
    connection.registerObject("/org/meego/omweather", controller);
    


    //config = controller->config();
    //std::cerr<<"iconpath = "<<config->imagespath().toStdString() << std::endl;
    //update_weather_forecast(config);
    
    QDeclarativeView *qview;
    qview = controller->qview();

    SelectModel model;
    model.addData(new SelectData("C", "", "a"));
    model.addData(new SelectData("F", "", "a"));
    qview->rootContext()->setContextProperty("temp_model", &model);
 //   qview->rootContext()->setContextProperty("Forecast_model", model);
    //qview->rootContext()->setContextProperty("Config", config);
    std::cerr << "qml path = " << Core::AbstractConfig::layoutqml << std::endl;
    /*
    qview->setSource(QUrl::fromLocalFile(QString::fromStdString(Core::AbstractConfig::prefix + 
                                                                Core::AbstractConfig::sharePath + 
                                                                Core::AbstractConfig::layoutqml)));
    */
    //qview->setSource(QUrl::fromLocalFile(QString::fromStdString(Core::AbstractConfig::layoutqml)));
    qview->setSource(QUrl::fromLocalFile(QString::fromStdString(Core::AbstractConfig::prefix +
                                                                Core::AbstractConfig::sharePath +
                                                                Core::AbstractConfig::layoutqml)));
    /* Trying to make fullscreen http://wiki.meego.com/QML/Full-screen_applications */
    qview->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    //qview->setGeometry(QRect(0,0,854,480));
    qview->showFullScreen();
    //qview->show();
    /*This code provides Segmantation fault
    delete dadapt;
    delete controller;
    */
    return app.exec();
   
}
