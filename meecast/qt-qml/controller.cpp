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
    std::cerr<<"End of creating Config class" <<std::endl;

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
  time_t current_hour;
  struct tm   *tm = NULL;
  DataItem *forecast_data = NULL;
  Core::Data *temp_data = NULL;
  int i = 0;
  
  if (_dp)
      delete _dp;
  _dp = NULL;
  if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0 &&
        _config->stationsList().at(_config->current_station_id()))
        _dp = current_data(_config->stationsList().at(_config->current_station_id())->fileName());

  _model = new DataModel(new DataItem, qApp);
  _current = new DataModel(new DataItem, qApp);
  _night_model = new DataModel(new DataItem, qApp);
  _current_night = new DataModel(new DataItem, qApp);
  _hours_model = new DataModel(new DataItem, qApp);

  /* set current day */ 
  current_day = time(NULL);
  tm = gmtime(&current_day);
  tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
  tm->tm_isdst = 1;
  current_day = mktime(tm); /* today 00:00:00 */

  /* fill current day */
  if  (_dp != NULL && (temp_data = _dp->data().GetDataForTime(time(NULL)))) {
      forecast_data = new DataItem(temp_data);
      forecast_data->Text(_(forecast_data->Text().c_str()));
      forecast_data->SunRiseTime(_dp->data().GetSunRiseForTime(current_day + 14 * 3600));
      forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 14 * 3600));
      forecast_data->LastUpdate(_dp->LastUpdate());
      forecast_data->temperatureunit = _config->temperatureunit();
      forecast_data->windunit = _config->windspeedunit();
      forecast_data->pressureunit = _config->pressureunit();
      _current->appendRow(forecast_data);
      MeecastIf* dbusclient = new MeecastIf("com.meecast.applet", "/com/meecast/applet", QDBusConnection::sessionBus(), 0);
      /* Preparing time for updateing */
      uint result_time = 0;
      if (_config->UpdatePeriod() != INT_MAX){
          if ((time(NULL) - _dp->LastUpdate()) > _config->UpdatePeriod())
              result_time = time(NULL) + 10;
          else
              if (_dp->LastUpdate() + _config->UpdatePeriod() < temp_data->EndTime())
                 result_time = _dp->LastUpdate() + _config->UpdatePeriod();  
              else
                 result_time = temp_data->EndTime();
      }else
          result_time = temp_data->EndTime();

      QDateTime t;
      t.setTime_t(_dp->LastUpdate());
      dbusclient->SetCurrentData( _config->stationname(), forecast_data->temperature(),
                                  forecast_data->temperature_high(),
                                  forecast_data->temperature_low(), 
                                  (_config->iconspath() + "/" + _config->iconset() + "/" + forecast_data->icon()), 
                                  result_time, 
                                  forecast_data->current(), 
                                  _config->Lockscreen(),
                                  t.toString("dd MMM h:mm"));
  }

  /* fill current night */
  if  (_dp != NULL && (temp_data = _dp->data().GetDataForTime(current_day + 3 * 3600))) {
      forecast_data = new DataItem(temp_data);
      forecast_data->Text(_(forecast_data->Text().c_str()));
      forecast_data->SunRiseTime(_dp->data().GetSunRiseForTime(current_day + 3 * 3600));
      forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 3 * 3600));
      forecast_data->LastUpdate(_dp->LastUpdate());
      forecast_data->temperatureunit = _config->temperatureunit();
      forecast_data->windunit = _config->windspeedunit();
      forecast_data->pressureunit = _config->pressureunit();
      _current_night->appendRow(forecast_data);
      /*MeecastIf* dbusclient = new MeecastIf("com.meecast.applet", "/com/meecast/applet", QDBusConnection::sessionBus(), 0);
      dbusclient->SetCurrentData( _config->stationname(), forecast_data->temperature(),
                                  forecast_data->temperature_high(), forecast_data->temperature_low(),
                                  (_config->iconspath() + "/" + _config->iconset() + "/" + forecast_data->icon()), forecast_data->Data::EndTime(), forecast_data->current());
      */
  }

  /* set next day */
  i = 3600*24;
  /* fill other days */
  while  (_dp != NULL && (temp_data = _dp->data().GetDataForTime( current_day + 14 * 3600  + i))) {
      forecast_data = new DataItem(temp_data);
      forecast_data->Text(_(forecast_data->Text().c_str()));
      forecast_data->SunRiseTime(_dp->data().GetSunRiseForTime(current_day + 14 * 3600  + i));
      forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 14 * 3600  + i));
      forecast_data->LastUpdate(_dp->LastUpdate());
      forecast_data->temperatureunit = _config->temperatureunit();
      forecast_data->windunit = _config->windspeedunit();
      forecast_data->pressureunit = _config->pressureunit();
      _model->appendRow(forecast_data);
      i = i + 3600*24;
  }
  /* set next night */
  i = 3600*24;
  /* fill other nights */
  while  (_dp != NULL && (temp_data = _dp->data().GetDataForTime( current_day + 3 * 3600  + i))) {
      forecast_data = new DataItem(temp_data);
      forecast_data->Text(_(forecast_data->Text().c_str()));
      forecast_data->SunRiseTime(_dp->data().GetSunRiseForTime(current_day + 3 * 3600  + i));
      forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 3 * 3600  + i));
      forecast_data->LastUpdate(_dp->LastUpdate());
      forecast_data->temperatureunit = _config->temperatureunit();
      forecast_data->windunit = _config->windspeedunit();
      forecast_data->pressureunit = _config->pressureunit();
      _night_model->appendRow(forecast_data);
      i = i + 3600*24;
  }
  /* set current hour */
  current_hour = time(NULL);
  tm = gmtime(&current_hour);
  tm->tm_sec = 0; tm->tm_min = 1; 
  tm->tm_isdst = 1;
  current_hour = mktime(tm); 
  i =0;
  
  /* fill hours */
  while  (_dp != NULL && i<24*3600) {
      if (temp_data = _dp->data().GetDataForTime(current_hour + i, true)){
          forecast_data = new DataItem(temp_data);
          forecast_data->Text(_(forecast_data->Text().c_str()));
          forecast_data->temperatureunit = _config->temperatureunit();
          forecast_data->windunit = _config->windspeedunit();
          forecast_data->pressureunit = _config->pressureunit();
          _hours_model->appendRow(forecast_data);
      }
      i = i + 3600;
  }

  _qview->rootContext()->setContextProperty("Current", _current);
  _qview->rootContext()->setContextProperty("Current_night", _current_night);
  _qview->rootContext()->setContextProperty("Forecast_model", _model);
  _qview->rootContext()->setContextProperty("Forecast_night_model", _night_model);
  _qview->rootContext()->setContextProperty("Forecast_hours_model", _hours_model);


  /* models for station selection */
  SelectModel* source_model = new SelectModel(qApp);
  CountryModel* country_model = new CountryModel(qApp);
  RegionModel* region_model = new RegionModel(qApp);
  CityModel* city_model = new CityModel(qApp);
  //country_model->populate("gismeteo.ru");
  QStringList sources = _config->Sources();
  for (int j=0; j<sources.size(); j++){
      QString str = sources.at(j);
      source_model->addData(new SelectData(str, "", str.left(1)));
      //qDebug() << countries.at(j) << str.left(1);
  }
  _qview->rootContext()->setContextProperty("source_model", source_model);
  _qview->rootContext()->setContextProperty("country_model", country_model);
  _qview->rootContext()->setContextProperty("region_model", region_model);
  _qview->rootContext()->setContextProperty("city_model", city_model);
  
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

