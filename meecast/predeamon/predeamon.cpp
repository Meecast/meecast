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
#include "core.h"

#include "dbusadaptor_applet.h"
#include <libintl.h>
#include <locale.h>
#define _(String) gettext(String)
#define DATA_XSD_PATH "/opt/com.meecast.omweather/share/xsd/data.xsd"
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
       config = Core::Config::Instance(Core::AbstractConfig::getConfigPath()+
                                              "config.xml",
                                              Core::AbstractConfig::prefix+
                                              Core::AbstractConfig::schemaPath+
                                              "config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config =  Core::Config::Instance();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config =  Core::Config::Instance();
    }
    std::cerr<<"End of creating Config class: " <<std::endl;

    return config;
}

Core::DataParser*
current_data(std::string& str){
  Core::DataParser* dp;
  try{
        dp =  Core::DataParser::Instance(str, DATA_XSD_PATH);
    }
    catch(const std::string &str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return NULL;
    }
    catch(const char *str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return NULL;
    }
    return dp;
}

int
main (int argc, char *argv[])
{
  time_t current_day;
  struct tm   *tm = NULL;
  int year, current_month;
  Core::Data *temp_data = NULL;
  Core::DataParser* dp = NULL;
  int success = 0;
  uint i = 0;
  QString temp;
  QString temp_high;
  QString temp_low;

    QCoreApplication a(argc, argv);
    textdomain("omweather");
    bindtextdomain("omweather", "/opt/com.meecast.omweather/share/locale");

    config = create_and_fill_config();
    /* Check time for previous updating */
    dp = current_data(config->stationsList().at(config->current_station_id())->fileName());

    /* 25*60 = 25 minutes - minimal time between updates */ 
    if ((!dp) || (dp && (abs(time(NULL) - dp->LastUpdate()) > 25*60))){
        /*update weather forecast*/
        if (config->UpdatePeriod() != INT_MAX || config->UpdatePeriod() != 0){
            if ((!dp) || (time(NULL) - dp->LastUpdate()) > config->UpdatePeriod()){
                for (i=0; i < config->stationsList().size();i++){
                    if (config->stationsList().at(i)->updateData(true)){
                        success ++;
                    }
                }
            }
        }
    }
    dp = current_data(config->stationsList().at(config->current_station_id())->fileName());
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
    if (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL)))) {
        QString icon_string =  config->iconspath().c_str();
        QString icon_number;
        icon_string.append("/") ;
        icon_string.append(config->iconSet().c_str());
        icon_string.append("/") ;
        icon_number = icon_number.number((temp_data->Icon()), 'i', 0) + ".png";
        icon_string.append(icon_number) ;
        temp_data->temperature_low().units(config->TemperatureUnit());
        temp_data->temperature_hi().units(config->TemperatureUnit());
        temp_data->temperature().units(config->TemperatureUnit());
        temp_data->Text(_(temp_data->Text().c_str()));
        if (temp_data->temperature().value(true) == INT_MAX){
            temp = "N/A";
        }else
            temp = temp.number((temp_data->temperature().value()),'f',0);
        if (temp_data->temperature_hi().value(true) == INT_MAX){
            temp_high = "N/A";
        }else
            temp_high = temp.number((temp_data->temperature_hi().value()),'f',0);

        if (temp_data->temperature_low().value(true) == INT_MAX){
            temp_low = "N/A";
        }else
            temp_low = temp.number((temp_data->temperature_low().value()),'f',0);

        /* Call DBUS */
        MeecastIf* dbusclient = new MeecastIf("com.meecast.applet", "/com/meecast/applet", QDBusConnection::sessionBus(), 0);
        /* Preparing time for updating */
        uint result_time = 0;
        if (config->UpdatePeriod() != INT_MAX || config->UpdatePeriod() != 0){
            if ((time(NULL) - dp->LastUpdate()) > config->UpdatePeriod())
                result_time = time(NULL) + 10;
            else
                if (dp->LastUpdate() + config->UpdatePeriod() < temp_data->EndTime())
                   result_time = dp->LastUpdate() + config->UpdatePeriod();  
                else
                   result_time = temp_data->EndTime();
        }else
            result_time = temp_data->EndTime();

        QString stationname = "";
        QDateTime t;
        t.setTime_t(dp->LastUpdate());
        QString description ="";
        dbusclient->SetCurrentData(stationname.fromUtf8(config->stationname().c_str()),
                                   temp, temp_high, temp_low, 
                                   icon_string, description.fromUtf8(temp_data->Text().c_str()),
                                   result_time, temp_data->Current(),
                                   config->Lockscreen(), 
                                   config->Standbyscreen(), 
                                   t.toString("dd MMM h:mm")); 
  }

  if (dp){
      dp->DeleteInstance();
      dp = NULL;
  }
  return 0;
}
