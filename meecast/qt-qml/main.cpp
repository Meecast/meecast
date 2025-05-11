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

#include <sailfishapp.h>
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
    QGuiApplication* app(SailfishApp::application(argc, argv));
    qmlRegisterType<MeeCastCover>("harbour.meecast.meecastcover", 1, 0, "MeeCastCover");
    //app->setProperty("NoMStyle", true);
    QDir::setCurrent(app->applicationDirPath());

    
    std::cerr<<"application binary path "<< app->applicationDirPath().toStdString().c_str()<<std::endl;
    std::cerr<<"application config path "<< QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).toStdString().c_str()<<std::endl;

    /* For Sailfishos only. Remove after versiom 1.12.4 */
    /* Move config from old versions path. For SailfishOS only */
    const QString old_config_path_directory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-meecast/";
    const QString old_config_path = old_config_path_directory + "config.xml";
    const QString new_config_path_directory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/org.meecast/MeeCast/";
    const QString new_config_path = new_config_path_directory + "config.xml";

    if (!(QFile::exists(new_config_path))){
        qDebug()<<"New config is not present";
        QDir pathDir(new_config_path_directory);
        bool new_pathdir_exist = pathDir.exists();
        if (!new_pathdir_exist){
            new_pathdir_exist = QDir().mkpath(new_config_path_directory);
        }
        if (new_pathdir_exist && (QFile::exists(old_config_path))){
            qDebug()<<"Old config present";
            QFile old_file(old_config_path);
            QFile new_file(new_config_path);
            if (old_file.open(QIODevice::ReadOnly | QIODevice::Text)){
                qDebug()<<"Open old config file";
                if (new_file.open(QIODevice::WriteOnly | QIODevice::Text)){
                    qDebug()<<"copy old config to new file";
                    QTextStream in(&old_file);
                    QTextStream out(&new_file);
                    while(!in.atEnd()) {
                        QString line = in.readLine();
                        line.replace(old_config_path_directory, new_config_path_directory);
                        out<<line<<endl;
                        qDebug()<<line;
                    }
                    new_file.close();
                }else{
                    qDebug()<<"Can't copy old config to new file1";
                }
                old_file.close();
            }else{
                qDebug()<<"Can't open old config file "<<old_config_path;
            }
        }
    }else{
        qDebug()<<"New config file is present";
    }

/*
    QString str = QDir::currentPath();
    std::cerr<<str.toStdString().c_str()<<std::endl;

    str.truncate(str.length()-4);

    Core::AbstractConfig::prefix = str.toStdString();
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
    QUrl qmlPath(SailfishApp::pathTo("qml/main.qml"));
    qview->setSource(qmlPath);
    qview->show();


    int result = app->exec();
    std::cerr<<"This is the End "<<std::endl;
    delete controller;
    return result;
//    return app->exec();
//    return SailfishApp::main(argc, argv);
   
}
