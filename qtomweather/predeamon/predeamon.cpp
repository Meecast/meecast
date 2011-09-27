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
#include "core.h"

#include "dbusadaptor_applet.h"



void init_omweather_core(void);

Core::Config *create_and_fill_config(void);

/* Global section */
Core::Config *config;
Core::StationsList stationslist;
Core::DataParser* dp = NULL;
FILE *file;


Core::Config *
create_and_fill_config(){
    Core::Config *config;
    std::cerr<<"Create Config class: " << Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd"<< std::endl;
    try{
        config = new Core::Config(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
        std::cerr << "count station:" << config->stationsList().size() << std::endl;
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config = new Core::Config();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config = new Core::Config();
    }
    //std::cerr<<"End of creating Config class: " <<std::endl;
    config->saveConfig();
    std::cerr<<"End of creating Config class: " <<std::endl;

    return config;
}

int
main (int argc, char *argv[])
{
  char buffer[4096];
  time_t current_day;
  struct tm   *tm = NULL;
  int year, current_month;
  Core::Data *temp_data = NULL;
  Core::DataParser* dp = NULL;
  int i, num = 0;
  int success = 0;
  Core::Data *forecast_data = NULL;

  config = create_and_fill_config();

  /*update weather forecast*/

  for (i=0; i < config->stationsList().size();i++){
      if (config->stationsList().at(i)->updateData(true)){
          success ++;
      }
  }

  /* set current day */ 
    current_day = time(NULL);
    //tm = localtime(&current_day);
    tm = gmtime(&current_day);
    year = 1900 + tm->tm_year;
    current_month = tm->tm_mon;
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;

    if (dp)
        temp_data = dp->data().GetDataForTime(time(NULL));
//    if (temp_data)
//        current_day = current_day + 3600*dp->timezone();

    current_day = mktime(tm);
    /* fill current date */
    i = 0;
    if (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL) + i))) {
        MeecastIf* dbusclient = new MeecastIf("com.meecast.applet", "/com/meecast/applet", QDBusConnection::sessionBus(), 0);
        QString icon_string =  config->iconspath().c_str();
        icon_string.append("/") ;
        icon_string.append(config->iconSet().c_str());
        icon_string.append("/") ;
        icon_string.append(temp_data->Icon()) ;
        QString temp;
        DataItem::Data::temperature_low().units(temperatureunit.toStdString());
        if (DataItem::Data::temperature_low().value(TRUE) == INT_MAX){
            temp = "N/A";
        }
         temp.number((DataItem::Data::temperature_low().value()),'f',0);

        dbusclient->SetCurrentData(config->stationname().c_str(), temp.number((DataItem::Data::temperature_low().value()),'f',0);

                                   temp_data->temperature_hi().c_str(), temp_data->temperature_low().c_str(), 
                                   icon_string, 0, temp_data->Current()); 
    }

  if (dp){
      delete dp;
      dp = NULL;
  }

  return 0;
}
