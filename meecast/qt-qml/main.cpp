/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2024 Vlad Vasilyeu
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

//#include <sailfishapp.h>
#include <auroraapp.h>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
//#include <QtQml/qqml.h>


//#include <QtCore/QtGlobal>
//#include <QApplication>
//#include <QGraphicsView>
//#include <QGraphicsScene>
//#include <QGraphicsWidget>
//#include <QGraphicsLinearLayout>
//#include <QDeclarativeComponent>
//#include <QDeclarativeContext>
//#include <QDeclarativeEngine>
//#include <QDeclarativeView>
#include <QLocale>
#include <QHash>
#include <QtQml>
//#include <QGraphicsGridLayout>
#include <exception>
#include <iostream>

#include "core.h"
#include "dataqml.h"
#include "configqml.h"
//#include "qmllayoutitem.h"
#include "dataitem.h"
#include "datamodel.h"
#include "parserqt.h"
#include "databasesqlite.h"
#include "abstractconfig.h"
#include "dbusadaptor.h" 
#include "controller.h" 
#include "meecastcover.h"

//#include <QtDebug>

#include <libintl.h>
#include <locale.h>

#include "selectmodel.h"
#include "networkingcontrol.h"

//#define _(String) gettext(String)


#define _(String)  QObject::trUtf8(String).toStdString().c_str()

using namespace Aurora;
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
/////////////////////////////////////////////////////////////////////////////

Q_DECL_EXPORT int main(int argc, char* argv[])
{
     
    //QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QGuiApplication* app(Aurora::Application::application(argc, argv));
    app->setOrganizationName(QStringLiteral("org.meecast"));
    app->setApplicationName(QStringLiteral("MeeCast"));

    qmlRegisterType<MeeCastCover>("harbour.meecast.meecastcover", 1, 0, "MeeCastCover");
    //app->setProperty("NoMStyle", true);
    QDir::setCurrent(app->applicationDirPath());

    
    std::cerr<<app->applicationDirPath().toStdString().c_str()<<std::endl;
//    std::cerr<<Core::AbstractConfig::prefix.c_str()<<std::endl;

    QString str = QDir::currentPath();
    std::cerr<<str.toStdString().c_str()<<std::endl;

    str.truncate(str.length()-4);

    Core::AbstractConfig::prefix = str.toStdString();

/*
    std::cerr<<Core::AbstractConfig::prefix.c_str()<<std::endl;
    QDir::setCurrent(QDir::currentPath());
*/


    //Add the QML snippet into the layout

    
    //ConfigQml *config;
    //
    Controller *controller;
    /*
    textdomain("omweather");
    bindtextdomain("omweather", localepath.toStdString().c_str());
    std::cerr<<" Path for tanslate "<<localepath.toStdString().c_str()<<std::endl;
*/
    controller = new Controller(app);

    /* D-BUS */
    DbusAdaptor* dadapt = new DbusAdaptor(controller);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("org.meego.omweather");
    connection.registerObject("/org/meego/omweather", controller);
    QQuickView *qview;
    qview = controller->qview();
    //QUrl qmlPath(Aurora::Application::pathTo("qml/main.qml"));
    /* Hack for working in FingerTerminal for direct exec */
    QString myurl = Core::AbstractConfig::prefix.c_str();
    myurl.append(Core::AbstractConfig::sharePath.c_str());
    myurl.append(Core::AbstractConfig::layoutqml.c_str());
    std::cerr<<"MyURL "<<myurl.toStdString()<<std::endl;
    QUrl qmlPath(myurl);

    qview->setSource(qmlPath);
    qview->show();


    int result = app->exec();
    std::cerr<<"This is the End "<<std::endl;
    delete controller;
    return result;
//    return app->exec();
//    return SailfishApp::main(argc, argv);
   
}
