/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2013 Vlad Vasilyeu
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

#include "updatethread.h"
#include "configqml.h"

UpdateThread::UpdateThread(QObject *parent) :
    QThread(parent){
}

void
UpdateThread::run(){
    std::cerr<<"UpdateThread::run()"<<std::endl;
    ConfigQml *config = NULL;
    try{
        config = ConfigQml::Instance(Core::AbstractConfig::getConfigPath()+
                                   "config.xml",
                                   Core::AbstractConfig::prefix+
                                   Core::AbstractConfig::schemaPath+
                                   "config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config =  ConfigQml::Instance();
        config->saveConfig();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config =  ConfigQml::Instance();
        config->saveConfig();
    }
    if (!config->isOnline()){
        std::cerr<<"Not connect to Internet"<<std::endl;
        //config->connectSession(false);
        config->connectSession(true);
        config->need_updating = true;
        return;
    }
    /* std::cerr<<"Size of StationList "<< config->stationsList().size()<<std::endl; */
    for (short i=0; i < config->stationsList().size();i++){
        config->stationsList().at(i)->updateData(true);
    }
    config->need_updating = false;
}
