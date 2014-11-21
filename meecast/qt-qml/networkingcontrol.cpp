/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
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


#include "networkingcontrol.h"

int  NetworkingControl::omw_refCount = 0;
bool NetworkingControl::omw_isSessionActive = false;


NetworkingControl::NetworkingControl(QObject *parent): QObject(parent), omw_networkConfigManager(0), omw_networkSession(NULL), omw_isOnline(false){
//NetworkingControl::NetworkingControl(QObject *parent){
     omw_networkConfigManager = new QNetworkConfigurationManager();
     
     QNetworkConfiguration netConf  = omw_networkConfigManager->defaultConfiguration(); 

     switch (netConf.type()) {
         case QNetworkConfiguration::InternetAccessPoint:
             std::cerr<<"QNetworkConfiguration::InternetAccessPoint"<<std::endl;
             break;
         case QNetworkConfiguration::ServiceNetwork:
             std::cerr<<"QNetworkConfiguration::ServiceNetwork"<<std::endl;
             break;
         case QNetworkConfiguration::UserChoice:
             std::cerr<<"QNetworkConfiguration::UserChoice"<<std::endl;
             break;
         case QNetworkConfiguration::Invalid:
             std::cerr<<"QNetworkConfiguration::Invalid:"<<std::endl;
             break;
     }

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
    std::cerr<<"activeConfigs for QNetworkConfiguration" << activeConfigs.count()<<std::endl;
    if (activeConfigs.count() > 0)
        return true;
    else
        return false;
}

void NetworkingControl::connectSession(bool background ){
    if(omw_isSessionActive){
        std::cerr<<"Network session already active, ignoring connect call"<<std::endl;
        omw_refCount++;
        emit connectionSuccess();
        return;
    }else if(!omw_networkSession){
        QNetworkConfiguration netConfig = omw_networkConfigManager->defaultConfiguration();
        omw_networkSession = new QNetworkSession(netConfig);
        connect(omw_networkSession, SIGNAL(error(QNetworkSession::SessionError)),
                SLOT(slotSessionError(QNetworkSession::SessionError)));
        connect(omw_networkSession, SIGNAL(stateChanged(QNetworkSession::State)),
                SLOT(slotSessionState(QNetworkSession::State)));
        /*
         * connect(m_networkSession, SIGNAL(opened()),
         * SLOT(slotSessionOpened()));
         * */

 //       std::cerr<<"netConfig.isValid() "<<netConfig.isValid()<<std::endl;
    }
    omw_networkSession->setSessionProperty("ConnectInBackground", background);
//    const bool canStartIAP = (omw_networkConfigManager->capabilities()
//                                           & QNetworkConfigurationManager::CanStartAndStopInterfaces);

//    std::cerr<<"canStartIAP "<<canStartIAP<<std::endl;
    omw_networkSession->open();
    omw_networkSession->waitForOpened(60000);
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
    switch(status){
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
        if (!omw_isOnline)
           omw_isSessionActive = false; 
        emit valueChanged(omw_isOnline);
    }
}
