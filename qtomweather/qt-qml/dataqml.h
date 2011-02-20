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


#ifndef DATAQML_H
#define DATAQML_H
#include "core.h"
#include <QObject>
class DataQml : public QObject, public Core::Data

{
    Q_OBJECT
    Q_PROPERTY(QString temperature_low READ temperature_low NOTIFY temperature_lowChanged)
    Q_PROPERTY(QString temperature_high READ temperature_high NOTIFY temperature_highChanged)
    Q_PROPERTY(QString text READ text NOTIFY textChanged)
    Q_PROPERTY(int icon READ icon NOTIFY iconChanged)

    public:
    DataQml();
    DataQml(const Core::Data *data);
    QString temperature_high();
    QString temperature_low();
    QString text();
    int icon();
    void refreshview();
    virtual ~DataQml(){};
    signals:
    void temperature_highChanged();
    void temperature_lowChanged();
    void iconChanged();
    void textChanged();
};

#endif // DATAQML_H
