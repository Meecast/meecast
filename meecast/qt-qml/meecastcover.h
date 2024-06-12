/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2014 Vlad Vasilyeu
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


#ifndef MEECASTCOVER_H
#define MEECASTCOVER_H

#include <QObject>
#include "configqml.h"

class MeeCastCover : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool status READ status WRITE setStatus NOTIFY statusChanged)
private:
   bool _status;
protected:
public:
   explicit MeeCastCover(QObject* parent = 0);
   virtual ~MeeCastCover();
   bool status();
signals:
   void statusChanged();
public Q_SLOTS:
   void setStatus(bool active);

};

#endif // MEECASTCOVER_H
