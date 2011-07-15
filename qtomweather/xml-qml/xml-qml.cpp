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
void
save_xml(Core::Data *data)
{
  char buffer[4096];
        QDomDocument doc;
        doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

        QDomElement root = doc.createElement("config");
        root.setAttribute("xmlns", "http://omweather.garage.maemo.org/schemas");
        doc.appendChild(root);

        QDomElement el = doc.createElement("icon");
        QDomText t = doc.createTextNode(QString::fromStdString(config->iconSet()) + "/" + QString::number(data->Icon()));
        el.appendChild(t);
        root.appendChild(el);


        QFile file(QString::fromStdString("/tmp/1.xml"));
        if (!file.open(QIODevice::WriteOnly)){
            std::cerr<<"error file open /tmp/1.xml"<<std::endl;
            throw("Invalid destination file");
            return;
        }

        QTextStream ts(&file);
        ts << doc.toString();
        //file.write(doc.toString());
        //std::cerr << doc.toString().toStdString() << std::endl;
        file.close();
}
QDomElement
make_item(QDomDocument doc, Core::Data *data, int num, bool current)
{
    QDomElement item = doc.createElement("item");
    item.setAttribute("id", num);
    if (current)
        item.setAttribute("current", "true");

    QDomElement el = doc.createElement("dayname");
    QDomText t = doc.createTextNode(QString::fromStdString(data->ShortDayName()));
    el.appendChild(t);
    item.appendChild(el);

    el = doc.createElement("icon");
    t = doc.createTextNode(QString::fromStdString(config->iconSet()) + "/" + QString::number(data->Icon()) + ".png");
    el.appendChild(t);
    item.appendChild(el);

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
  GSList      *l = NULL, *order = NULL;

  bindtextdomain(GETTEXT_PACKAGE, "/opt/com.meecast.omweather/share/locale");
  config = create_and_fill_config();
  translate_hash = hash_table_create();
  /* prepairing icon */
  if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0 &&
      config->stationsList().at(config->current_station_id())){
      dp = current_data(config->stationsList().at(config->current_station_id())->fileName());
      std::cerr << config->stationsList().at(config->current_station_id())->fileName() << std::endl;
  }
  if (dp){
      QDomDocument doc;
      QDomElement el;
      doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

      QDomElement root = doc.createElement("data");
      root.setAttribute("xmlns", "http://omweather.garage.maemo.org/schemas");
      doc.appendChild(root);

      current_day = time(NULL);
      tm = localtime(&current_day);
      year = 1900 + tm->tm_year;
      current_month = tm->tm_mon;
      tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
      tm->tm_isdst = 1;
      current_day = mktime(tm);
      /* fill current date */
      if  (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL) + i))) {
          std::cerr << temp_data->FullDayName() << std::endl;
          doc.appendChild(make_item(doc, temp_data, num, true));
      }

      /* set next day */
      i = 3600*24;
      num++;
      /* fill other days */
      while  (dp != NULL && (temp_data = dp->data().GetDataForTime( current_day + 12 * 3600  + i))) {
          i = i + 3600*24;
          std::cerr << temp_data->FullDayName() << std::endl;
          doc.appendChild(make_item(doc, temp_data, num, false));
          num++;
      }
      QFile file(QString::fromStdString("1.xml"));
      if (!file.open(QIODevice::WriteOnly)){
          std::cerr<<"error file open /tmp/1.xml"<<std::endl;
          throw("Invalid destination file");

      }

      QTextStream ts(&file);
      ts << doc.toString();
      //file.write(doc.toString());
      //std::cerr << doc.toString().toStdString() << std::endl;
      file.close();

  }
      temp_data = dp->data().GetDataForTime(time(NULL));
  if (temp_data)
      save_xml(temp_data);       
  if (dp){
      delete dp;
      dp = NULL;
  }

  return 0;
}
