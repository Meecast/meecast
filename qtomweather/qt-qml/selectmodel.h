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

#ifndef SELECTMODEL_H
#define SELECTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class SelectData: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
public:
    SelectData();
    SelectData(const QString &name, const QString &key, const QString &category);

    QString name();
    QString key();
    QString category();
    void setName(QString name);
    void setKey(QString key);
    void setCategory(QString category);
signals:
    void nameChanged(QString);
    void keyChanged(QString);
    void categoryChanged(QString);
private:
    QString _name;
    QString _key;
    QString _category;
};

class SelectModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    enum SelectModelRoles {
        NameRole = Qt::UserRole + 1,
        KeyRole = Qt::UserRole + 2,
        CategoryRole = Qt::UserRole + 3
    };
    SelectModel(QObject *parent = 0);

    Q_INVOKABLE SelectData* get(int index);
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;
    void addData(SelectData* data);
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
private:
    QList<SelectData*> _list;
};

#endif // SELECTMODEL_H
