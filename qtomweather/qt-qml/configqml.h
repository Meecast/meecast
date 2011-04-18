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


#ifndef CONFIGQML_H
#define CONFIGQML_H

#include "core.h"
#include <QObject>
#include <QColor>
#include "datamodel.h"
#include "dataitem.h"

class ConfigQml : public QObject, public Core::Config

{
    Q_OBJECT
    Q_PROPERTY(QString iconset READ iconset NOTIFY iconsetChanged)
    Q_PROPERTY(QString iconspath READ iconspath NOTIFY iconspathChanged)
    Q_PROPERTY(QString imagespath READ imagespath NOTIFY imagespathChanged)
    Q_PROPERTY(QString iconsbutton READ iconsbutton NOTIFY iconsbuttonChanged)
    Q_PROPERTY(QString temperatureunit READ temperatureunit NOTIFY temperatureunitChanged)
    Q_PROPERTY(QString windspeedunit READ windspeedunit NOTIFY windspeedunitChanged)
    Q_PROPERTY(QColor fontcolor READ fontcolor NOTIFY fontcolorChanged)
    Q_PROPERTY(QString stationname READ stationname NOTIFY stationnameChanged)
    Q_PROPERTY(QString filename READ filename NOTIFY filenameChanged)
    public:
    ConfigQml();
    ConfigQml(const std::string& filename, const std::string& schema_filename = "/usr/" + schemaPath + "config.xsd");
    QString iconset();
    QString iconspath();
    QString imagespath();
    QString iconsbutton();
    QString temperatureunit();
    QString windspeedunit();
    QColor fontcolor();
    QString stationname();
    QString filename();
    Q_INVOKABLE void changestation();
    Q_INVOKABLE void updatestations();
    Q_INVOKABLE void runsetting();
    Q_INVOKABLE void closeapplication();
    void refreshconfig();
    virtual ~ConfigQml(){};
    signals:
    void iconsetChanged();
    void iconspathChanged();
    void imagespathChanged();
    void iconsbuttonChanged();
    void temperatureunitChanged();
    void windspeedunitChanged();
    void fontcolorChanged();
    void stationnameChanged();
    void filenameChanged();
    public Q_SLOTS:
    void reload_config();

       };

#endif // CONFIGQML_H
