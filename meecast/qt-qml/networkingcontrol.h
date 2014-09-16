/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2014 Vlad Vasiliev
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


#ifndef NETWORKINGCONTROL_H
#define NETWORKINGCONTROL_H
#include <iostream>
#include <stdio.h>
#include <QObject>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>

class QNetworkConfigurationManager;

class NetworkingControl : public QObject
{
Q_OBJECT
public:
    NetworkingControl(QObject *parent = 0);
    ~NetworkingControl();
    bool isOnline();
    void connectSession(bool connectInBackground = false);
    void disconnectSession();
signals:
    void valueChanged(bool aConnected);
    void connectionSuccess();
    void connectionError();
private:
    QNetworkConfigurationManager *omw_networkConfigManager; 
    QNetworkSession *omw_networkSession; 
    static bool omw_isSessionActive; 
    bool omw_isOnline; 
    static int omw_refCount; 
private slots:
    void slotOnlineStateChanged(bool isOnline);
    void slotSessionState(QNetworkSession::State status);
    void slotSessionError(QNetworkSession::SessionError error);
};

#endif //NETWORKINGCONTROL_H 
