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


#include "networkingcontrol.h"

int  NetworkingControl::omw_refCount = 0;
bool NetworkingControl::omw_isSessionActive = false;


NetworkingControl::NetworkingControl(QObject *parent): QObject(parent), omw_networkConfigManager(0), omw_networkSession(NULL), omw_isOnline(false){
//NetworkingControl::NetworkingControl(QObject *parent){
     omw_networkConfigManager = new QNetworkConfigurationManager();
     connect(omw_networkConfigManager, SIGNAL(onlineStateChanged(bool)),
             SLOT(slotOnlineStateChanged(bool)));
     omw_isOnline = omw_networkConfigManager->isOnline();
     std::cerr<<"Current online status:" << omw_isOnline<<std::endl;
}

NetworkingControl::~NetworkingControl(){
     if(omw_networkSession){
         delete omw_networkSession;
         omw_networkSession = NULL;
     }
     if(omw_networkConfigManager){
         delete omw_networkConfigManager;
         omw_networkConfigManager = NULL;
     }
}

bool
NetworkingControl::isOnline(){
    QNetworkConfigurationManager mgr;

    QList<QNetworkConfiguration> activeConfigs = mgr.allConfigurations(QNetworkConfiguration::Active);
    if (activeConfigs.count() > 0)
        return true;
    else
        return false;
}


void NetworkingControl::disconnectSession(){
    if(omw_refCount > 0){
        omw_refCount--;
    }
    if(omw_networkSession && 0 == omw_refCount){
        omw_networkSession->close();
    }
}


void NetworkingControl::slotSessionError(QNetworkSession::SessionError error){
    switch(error)
    {
        case QNetworkSession::UnknownSessionError:
            std::cerr<<"QNetworkSession::UnknownSessionError"<<std::endl;
            emit connectionError();
            break;
        case QNetworkSession::SessionAbortedError:
            std::cerr<<"QNetworkSession::SessionAbortedError"<<std::endl;
            emit connectionError();
            break;
        case QNetworkSession::RoamingError:
            std::cerr<<"QNetworkSession::RoamingError"<<std::endl;
            emit connectionError();
            break;
        case QNetworkSession::OperationNotSupportedError:
            std::cerr<<"QNetworkSession::OperationNotSupportedError"<<std::endl;
            emit connectionError();
            break;
        case QNetworkSession::InvalidConfigurationError:
            std::cerr<<"QNetworkSession::InvalidConfigurationError"<<std::endl;
            emit connectionError();
            break;
        default:
            std::cerr<<"QNetworkSession:: Invalid error code"<<std::endl;
            emit connectionError();
            break;
    }
}


void NetworkingControl::slotSessionState(QNetworkSession::State status)
{
    switch(status)
    {
        case QNetworkSession::Invalid:
            std::cerr<<"QNetworkSession::Invalid"<<std::endl;
            omw_isSessionActive = false;
            break;
        case QNetworkSession::NotAvailable:
            std::cerr<<"QNetworkSession::NotAvailable"<<std::endl;
            omw_isSessionActive = false;
            emit connectionError();
            break;
        case QNetworkSession::Connecting:
            std::cerr<<"QNetworkSession::Connecting"<<std::endl;
            omw_isSessionActive = false;
            break;
        case QNetworkSession::Connected:
            std::cerr<<"QNetworkSession::Connected"<<std::endl;
            if (omw_networkSession->isOpen() &&
                    omw_networkSession->state() == QNetworkSession::Connected){
                omw_isSessionActive = true;
                emit connectionSuccess();
            }else{
                emit connectionError();
            }
            break;
        case QNetworkSession::Closing:
            std::cerr<<"QNetworkSession::Closing"<<std::endl;
            omw_isSessionActive = false;
            break;
        case QNetworkSession::Disconnected:
            std::cerr<<"QNetworkSession::Disconnected"<<std::endl;
            omw_isSessionActive = false;
            break;
        case QNetworkSession::Roaming:
            std::cerr<<"QNetworkSession::Roaming"<<std::endl;
            omw_isSessionActive = false;
            break;
        default:
            std::cerr<<"QNetworkSession:: Unknown status change"<<std::endl;
            omw_isSessionActive = false;
            break;
    }
}


void NetworkingControl::slotOnlineStateChanged(bool isOnline){
    std::cerr<<"Online status changed, is online is now: "<< isOnline<<std::endl;
    if(omw_isOnline != isOnline){
        omw_isOnline = isOnline;
        emit valueChanged(omw_isOnline);
    }
}
