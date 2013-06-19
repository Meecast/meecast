/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2013 Vlad Vasilyeu
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


#include "downloader.h"

extern std::vector<RequestId> _reqIdList;
using namespace Tizen::App;
////////////////////////////////////////////////////////////////////////////////


Downloader::Downloader()
{
    DownloadManager* pManager = DownloadManager::GetInstance();
    AppLog("Downloader Manager %p", pManager);
    pManager->SetDownloadListener(this);
}

void
Downloader::OnDownloadCompleted(RequestId reqId, const Tizen::Base::String& path){
   AppLog("Download is completed. %S", path.GetPointer());
    if (_reqIdList.size() == 0)
        return;
    int i;
    AppLog("Download is completed. first stage %i", _reqIdList.size());
    for(i=0; i<_reqIdList.size(); i++){
    AppLog("Download is completed. Second stage %i", _reqIdList[i]);
        if (_reqIdList[i] == reqId){
            _reqIdList.erase(_reqIdList.begin() + i);
            AppLog("Download is completed. %S", path.GetPointer());
            if (_reqIdList.size() <= 0){
                AppLog(" last stage %i", _reqIdList.size());
                ConfigTizen *config;
                config = ConfigTizen::Instance( std::string("config.xml"),
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");

                config->convertstations();
            }
        }
    }

}

void
Downloader::OnDownloadFailed(RequestId reqId, result r, const Tizen::Base::String& errorCode){
    AppLog("Download failed. %S %d", errorCode.GetPointer(), r);
    if (_reqIdList.size() == 0)
        return;
    switch (r){
        case E_INVALID_URL: AppLog ("The specified URL is invalid. ");break;
        case E_CONNECTION_FAILED: AppLog ("The server connection fails.");break;
        case E_TIMEOUT: AppLog ("   The connection has timed out. ");break;
        case E_MAX_EXCEEDED: AppLog ("  The request has exceeded the limit.  ");break;
        case E_STORAGE_FULL: AppLog ("The storage is full. ");break;
        case E_OUT_OF_MEMORY: AppLog ("The memory is insufficient");break;
        case E_SYSTEM: AppLog ("The method cannot proceed due to a severe system error. ");break;


    }

    int i;
    for(i=0; i<_reqIdList.size(); i++){
        if (_reqIdList[i] == reqId){
            AppLog("Download failed. %S", errorCode.GetPointer());
            if (_reqIdList.size() <= 0){
                AppLog(" last stage %i",  _reqIdList.size());
            }
            _reqIdList.erase(_reqIdList.begin() + i);
        }
    }

}


////////////////////////////////////////////////////////////////////////////////
