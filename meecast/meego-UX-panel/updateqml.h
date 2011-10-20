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

#ifndef UPDATEQML_H
#define UPDATEQML_H

#include <QObject>
#include "core.h"
#include <QStringList>
#include <QtDBus/QtDBus>
#include <QDebug>
#include "updatethread.h"

#if defined (BSD) && !_POSIX_SOURCE
    #include <sys/dir.h>
    typedef struct dirent Dirent;
#else
    #include <dirent.h>
    #include <linux/fs.h>
    typedef struct dirent Dirent;
#endif

class UpdateQml : public QObject
{
    Q_OBJECT
public:
    explicit UpdateQml();
    ~UpdateQml();

signals:
    //void configChange();
    void reload();

public slots:
    void makeQmlData();
    void updateData();
private:
    Core::Config *config;
    UpdateThread *thread;
    QDomElement make_item(QDomDocument doc, Core::Data *data, int num, bool current);
    Core::DataParser* current_data(std::string& str);
private slots:
    void configChangeSlot();
    void downloadFinishedSlot();
};

#endif // UPDATEQML_H
