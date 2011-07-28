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

#include <libintl.h>
#include <locale.h>

#ifdef LOCALDEBUG
    #define CONFIG_FILE "config.xml"
    #define CONFIG_XSD_PATH "../core/data/config.xsd"
#else
//    #define CONFIG_PATH "~/.config/omweather/config.xml"
    #define CONFIG_FILE "config.xml"
    #define CONFIG_XSD_PATH "/opt/com.meecast.omweather/share/xsd/config.xsd"
    #define DATA_XSD_PATH "/opt/com.meecast.omweather/share/xsd/data.xsd"
#endif


/*#define _(String) dgettext (GETTEXT_PACKAGE, String)*/
#define GETTEXT_PACKAGE "omweather"
#include <glib/gi18n-lib.h>


void init_omweather_core(void);
Core::DataParser *current_data(std::string& str);

Core::Config *create_and_fill_config(void);
GHashTable *hash_table_create(void);

/* Global section */
Core::Config *config;
Core::StationsList stationslist;
Core::DataParser* dp = NULL;
GHashTable           *translate_hash=NULL;
FILE *file;

/*******************************************************************************/
GHashTable *hash_table_create(void) {
    GHashTable *hash;
    hash = g_hash_table_new(g_str_hash, g_str_equal);
/*
 * WARNING!
 * Do not insert new lines to this file, use file hash.data
 * To add new reserved word from data (xml) file you can use
 * script get_reserved_word.pl like this:
 * cat BOXX0014.xml USCA0001.xml > |./get_reserved_word.pl
*/
#include "../netbook-UX/hash.data"
    return hash;
}

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

Core::DataParser*
current_data(std::string& str){
  Core::DataParser* dp;
  try{
        dp = new Core::DataParser(str, DATA_XSD_PATH);
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

QDomElement
make_item(QDomDocument doc, Core::Data *data, int num, bool current)
{
    QDomElement item = doc.createElement("item");
    item.setAttribute("id", num);
    if (current)
        item.setAttribute("current", "true");

    QDomElement el = doc.createElement("dayname");
    QString str;
    if (current){
        str = QString::fromStdString(data->FullDayName() +", " + data->DayOfMonthName() + " " + data->FullMonthName());
    }else {
        str = QString::fromStdString(data->FullDayName());
    }
    QDomText t = doc.createTextNode(str);
    el.appendChild(t);
    item.appendChild(el);

    el = doc.createElement("icon");
    t = doc.createTextNode(QString::fromStdString(config->prefix_path()) + "icons/" +
                           QString::fromStdString(config->iconSet()) + "/" +
                           QString::number(data->Icon()) + ".png");
    el.appendChild(t);
    item.appendChild(el);

    if (data->temperature_hi().value() != INT_MAX){
        QDomElement el = doc.createElement("temperature_hi");
        data->temperature_hi().units(config->TemperatureUnit());
        QDomText t = doc.createTextNode(QString::number(data->temperature_hi().value()) + "°" +
                                        QString::fromStdString(config->TemperatureUnit()));
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->temperature_low().value() != INT_MAX){
        QDomElement el = doc.createElement("temperature_low");
        data->temperature_low().units(config->TemperatureUnit());
        QDomText t = doc.createTextNode(QString::number(data->temperature_low().value()) + "°" +
                                        QString::fromStdString(config->TemperatureUnit()));
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->temperature().value() != INT_MAX){
        QDomElement el = doc.createElement("temperature");
        data->temperature().units(config->TemperatureUnit());
        QDomText t = doc.createTextNode(QString::number(data->temperature().value()) + "°" +
                                        QString::fromStdString(config->TemperatureUnit()));
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->Text().compare("N/A") != 0){
        QDomElement el = doc.createElement("description");
        QDomText t = doc.createTextNode(QString::fromStdString(data->Text()));
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->Pressure() != INT_MAX){
        QDomElement el = doc.createElement("pressure");
        QDomText t = doc.createTextNode(QString::number(data->Pressure()));
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->Humidity() != INT_MAX){
        QDomElement el = doc.createElement("humidity");
        QDomText t = doc.createTextNode(QString::number(data->Humidity()) + "%");
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->WindDirection().compare("N/A") != 0){
        QDomElement el = doc.createElement("wind_direction");
        QDomText t = doc.createTextNode(QString::fromStdString(data->WindDirection()));
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->WindSpeed().value() != INT_MAX){
        QDomElement el = doc.createElement("wind_speed");
        data->WindSpeed().units(config->WindSpeedUnit());
        QDomText t = doc.createTextNode(QString::number(data->WindSpeed().value()) +
                                        QString::fromStdString(config->WindSpeedUnit()));
        el.appendChild(t);
        item.appendChild(el);
    }

    return item;
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
  GSList      *l = NULL, *order = NULL;

  bindtextdomain(GETTEXT_PACKAGE, "/opt/com.meecast.omweather/share/locale");
  config = create_and_fill_config();
  translate_hash = hash_table_create();

  /*update weather forecast*/

  for (i=0; i < config->stationsList().size();i++){
      if (config->stationsList().at(i)->updateData(true)){
          success ++;
      }
  }

  QDomDocument doc;
  QDomElement station;
  doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
  QDomElement root = doc.createElement("data");
  doc.appendChild(root);

  QDomElement el1 = doc.createElement("update");
  QDomElement el = doc.createElement("period");
  QDomText t = doc.createTextNode(QString::number(config->UpdatePeriod()*1000));
  el.appendChild(t);
  el1.appendChild(el);
  root.appendChild(el1);


  Core::StationsList::iterator cur;
  for (cur=config->stationsList().begin(); cur<config->stationsList().end(); cur++){
      std::cerr << (*cur)->fileName() << " " << (*cur)->name() << std::endl;
      dp = current_data((*cur)->fileName());

      if (dp){
          station = doc.createElement("station");
          station.setAttribute("name", QString::fromStdString((*cur)->name()));
          station.setAttribute("id", QString::fromStdString((*cur)->id()));

          current_day = time(NULL);
          tm = localtime(&current_day);
          year = 1900 + tm->tm_year;
          current_month = tm->tm_mon;
          tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
          tm->tm_isdst = 1;
          current_day = mktime(tm);
          num = 0;
          i = 0;
          bool current = true;
          while  (dp != NULL && (temp_data = dp->data().GetDataForTime( current_day + 12 * 3600  + i))) {
              i = i + 3600*24;
              station.appendChild(make_item(doc, temp_data, num, current));
              if (current) current = false;
              num++;
          }
          root.appendChild(station);
      }

  }
  QFile file(QString::fromStdString(Core::AbstractConfig::getConfigPath() + "qmldata.xml"));
  if (!file.open(QIODevice::WriteOnly)){
      std::cerr<<"error file open 1.xml"<<std::endl;
      throw("Invalid destination file");
  }

  QTextStream ts(&file);
  ts << doc.toString();
  file.close();

  if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0 &&
      config->stationsList().at(config->current_station_id())){
      dp = current_data(config->stationsList().at(config->current_station_id())->fileName());
      std::cerr << config->stationsList().at(config->current_station_id())->fileName() << std::endl;
  }

  if (dp){
      delete dp;
      dp = NULL;
  }

  return 0;
}
