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
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include "configqml.h"
#include "datamodel.h"
#include "dataitem.h"
#include "selectmodel.h"
#include "countrymodel.h"
#include "regionmodel.h"
#include "citymodel.h"
#include <bb/cascades/QmlDocument>
#include <time.h>
#include "active.h"


#ifndef CONTROLLER_H
#define CONTROLLER_H
class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(const std::string& qml_filename ="asset:///qml/main.qml");
    ConfigQml* config();
    QmlDocument *qview();
    QmlDocument *active_frame_qview();
    ActiveFrame *active_frame;
    void active_frame_qview(QmlDocument *active_frame_view);
    virtual ~Controller();

public Q_SLOTS:
    void reload_config();
    void load_config();
    void load_data();

private:
    ConfigQml *_config;
    QmlDocument *_qview;
    QmlDocument *_active_frame_view;
    Core::DataParser *_dp;
    DataModelQt *_model; 
    DataModelQt *_hours_model; 
    DataModelQt *_current;
    DataModelQt *_night_model;
    DataModelQt *_current_night;
};

#endif // CONTROLLER_H
