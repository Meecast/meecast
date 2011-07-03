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
#include "controller.h"
ConfigQml *
create_and_fill_config(){
    ConfigQml *config;

    std::cerr<<"Create Config class: " << Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd"<< std::endl;
    try{
        config = new ConfigQml(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
        std::cerr << config->stationsList().size() << std::endl;
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config = new ConfigQml();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config = new ConfigQml();
    }
    //std::cerr<<"End of creating Config class: " <<std::endl;
    config->saveConfig();
    std::cerr<<"End of creating Config class: " <<std::endl;

    return config;
}

Core::DataParser*
current_data(std::string& str){
  Core::DataParser* dp;
  try{
        dp = new Core::DataParser(str, Core::AbstractConfig::prefix+Core::AbstractConfig::schemaPath+"data.xsd");
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


Controller::Controller() : QObject()
{
  _qview = new QDeclarativeView();
  _dp = NULL;
  this->load_config();
  this->load_data();
}

Controller::~Controller()
{
  if (_dp) 
      delete _dp;
}

QDeclarativeView* 
Controller::qview()
{
    return _qview;
}

void
Controller::load_data()
{
  time_t current_day;
  struct tm   *tm = NULL;
  int year, current_month;
  DataItem *forecast_data = NULL;
  Core::Data *temp_data = NULL;
  int i = 0;
  
  if (_dp)
      delete _dp;
  _dp = NULL;
  std::cout<<"Data"<<std::endl;
  if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0 &&
        _config->stationsList().at(_config->current_station_id()))
        _dp = current_data(_config->stationsList().at(_config->current_station_id())->fileName());
  _model = new DataModel(new DataItem, qApp);
  /* set current day */ 
  current_day = time(NULL);
  tm = localtime(&current_day);
  year = 1900 + tm->tm_year;
  current_month = tm->tm_mon;
  tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
  tm->tm_isdst = 1;
  current_day = mktime(tm);
  /* fill current date */
  if  (_dp != NULL && (temp_data = _dp->data().GetDataForTime(time(NULL) + i))) {
      forecast_data = new DataItem(temp_data);
      forecast_data->Text(_(forecast_data->Text().c_str()));
      _model->appendRow(forecast_data);
  }

  /* set next day */
  i = 3600*24;
  /* fill other days */
  while  (_dp != NULL && (temp_data = _dp->data().GetDataForTime( current_day + 12 * 3600  + i))) {
      i = i + 3600*24;
      forecast_data = new DataItem(temp_data);
      forecast_data->Text(_(forecast_data->Text().c_str()));
      _model->appendRow(forecast_data);
  }
  _qview->rootContext()->setContextProperty("Forecast_model", _model);
}

void
Controller::load_config()
{
   std::cout<<"Load";
  _config = create_and_fill_config();   
  _qview->rootContext()->setContextProperty("Config", _config);
}
void
Controller::reload_config()
{
  std::cout<<"Reload";
  delete _config;
  this->load_config();
  delete _model;
  this->load_data();
}

ConfigQml*
Controller::config()
{
   return _config;
}

