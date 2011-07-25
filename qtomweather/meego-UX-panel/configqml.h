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

#include <QObject>
#include "core.h"
#include <QStringList>

#if defined (BSD) && !_POSIX_SOURCE
    #include <sys/dir.h>
    typedef struct dirent Dirent;
#else
    #include <dirent.h>
    #include <linux/fs.h>
    typedef struct dirent Dirent;
#endif


class ConfigQml : public QObject, public Core::Config
{
    Q_OBJECT
public:
    explicit ConfigQml();
    //explicit ConfigQml(const std::string& filename, const std::string& schema_filename = "/usr/" + schemaPath + "config.xsd");
public slots:
    QStringList icons();
    QString iconSet();
    void iconSet(QString c);
    QStringList UpdatePeriods();
    void UpdatePeriod(QString str);
    QString UpdatePeriod();
    void UpdateConnect(bool uc);
    bool UpdateConnect();
    QStringList TemperatureUnits();
    void TemperatureUnit(QString text);
    QString TemperatureUnit();
    QStringList WindSpeedUnits();
    void WindSpeedUnit(QString text);
    QString WindSpeedUnit();
    void saveConfig();
private:
    QHash<QString, int> period_hash;
};

#endif // CONFIGQML_H
