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
#include "core.h"
#include <QObject>
//#include <QDeclarativeView>
//#include <QDeclarativeContext>
#include <QQmlContext>
#include <QtQuick/QQuickView>
#include "configqml.h"
#include "datamodel.h"
#include "dataitem.h"
#include "selectmodel.h"
#include "countrymodel.h"
#include "regionmodel.h"
#include "citymodel.h"
#include "dbusadaptor_applet.h"
#include <time.h>
#include <QtGui/QGuiApplication>


#ifndef CONTROLLER_H
#define CONTROLLER_H
class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QGuiApplication *app);
    ConfigQml* config();
    QQuickView *qview();
    virtual ~Controller();

public Q_SLOTS:
    void reload_config();
    void load_config();
    void load_data();

private:
    QGuiApplication *_app;
    ConfigQml *_config;
    QQuickView *_qview;
    QQmlContext *context;
    Core::DataParser *_dp;
    DataModel *_model; 
    DataModel *_hours_model; 
    DataModel *_current;
    DataModel *_night_model;
    DataModel *_current_night;
};

#endif // CONTROLLER_H
