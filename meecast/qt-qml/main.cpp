/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2013 Vlad Vasilyeu
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/

#include <QtCore/QtGlobal>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QTranslator>
#include <QHash>
#include <QGraphicsGridLayout>
#include <exception>
#include <iostream>

#include "core.h"
#include "dataqml.h"
#include "configqml.h"
#include "dataitem.h"
#include "datamodel.h"
#include "parserqt.h"
#include "databasesqlite.h"
#include "abstractconfig.h"
#include "dbusadaptor.h" 
#include "controller.h" 

#include <QtDebug>

#ifndef Q_OS_ANDROID
    #include <libintl.h>
    #include <locale.h>
#endif

#include "selectmodel.h"
#include "networkingcontrol.h"

#ifndef Q_OS_ANDROID
    #define _(String) gettext(String)
#endif

#ifdef Q_OS_ANDROID
    #define _(String) String
#endif


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

Q_DECL_EXPORT int main(int argc, char* argv[])
{
    //QApplication::setGraphicsSystem("native");
    //QApplication app(argc, argv);
    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Material"));
    qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", QByteArray("Dark"));

    QGuiApplication* app = new QGuiApplication(argc, argv);
    QQmlApplicationEngine engine;
    app->setOrganizationName(QStringLiteral("org.meecast"));
    app->setApplicationName(QStringLiteral("MeeCast"));



    QDir::setCurrent(app->applicationDirPath());

    

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

  //  QString locale = QLocale::system().name();

   // std::cerr<<"locale: "<<locale.toStdString()<<std::endl;
    
    //ConfigQml *config;
    //
    Controller *controller;
/*
    QTranslator translator;
    translator.load("ru.qml", "i18n");
    app.installTranslator(&translator);
*/
    controller = new Controller(app, &engine);
    
    /* Locale */
    for (unsigned int i=1; i<controller->config()->languagesList().size(); i++){
        if (controller->config()->languagesList().at(i).first == controller->config()->Language()){
            setlocale (LC_ALL, controller->config()->languagesList().at(i).second.c_str());
            setlocale (LC_MESSAGES, controller->config()->languagesList().at(i).second.c_str());
            QLocale::setDefault(QLocale(controller->config()->languagesList().at(i).second.c_str()));
        }
    }

#ifndef Q_OS_ANDROID
    textdomain("omweather");
    bindtextdomain("omweather", "/opt/com.meecast.omweather/share/locale");
#endif
#ifdef QDBUS    
    /* D-BUS */
    DbusAdaptor* dadapt = new DbusAdaptor(controller);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("org.meego.omweather");
    connection.registerObject("/org/meego/omweather", controller);
#endif    

    //config = controller->config();
    //std::cerr<<"iconpath = "<<config->imagespath().toStdString() << std::endl;
    //update_weather_forecast(config);
    

    std::cerr << "qml path = " << Core::AbstractConfig::layoutqml << std::endl;

//    qview->setSource(QUrl(QStringLiteral("assets:/qml/main.qml")));
#ifdef ANDROID
    engine.load(QUrl(QString::fromStdString(std::string("assets:") + Core::AbstractConfig::layoutqml)));
#else
//    QObject::connect((QObject*)qview->engine(), SIGNAL(quit()), app, SLOT(quit()));
//    qview->showFullScreen();
    engine.load(QUrl::fromLocalFile(QString::fromStdString(Core::AbstractConfig::prefix +
                                                                Core::AbstractConfig::sharePath +
                                                                Core::AbstractConfig::layoutqml)));
#endif    
    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    window->showFullScreen();
    //window->show();
    /*This code provides Segmantation fault
    delete dadapt;
    delete controller;
    */
    qDebug()<<"sssssssssssssssssssssssss";
    return app->exec();
   
}
