/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2025 Vlad Vasilyeu
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
// Debug

#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>


#define _(String)  QObject::trUtf8(String).toStdString().c_str()
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
        return NULL;
//        config =  Core::Config::Instance();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
//        config =  Core::Config::Instance();
        return NULL;
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
create_current_xml_file(Core::DataParser* dp){
    time_t current_day;
    struct tm   *tm = NULL;
    QString temp;
    QString temp_high;
    QString temp_low;
    int localtimezone = 0;
    int timezone = 0;
    struct tm time_tm1;
    struct tm time_tm2;
    Core::Data *temp_data = NULL;
    uint i = 0;

    if (dp){
        fprintf (stderr, "timezone!!!!! %i\n",dp->timezone());
        timezone = dp->timezone();
    }

    /* Set localtimezone */

    current_day = time(NULL);
    gmtime_r(&current_day, &time_tm1);
    localtime_r(&current_day, &time_tm2);
    time_tm1.tm_isdst = 0;
    time_tm2.tm_isdst = 0;

    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 

    /* set current day */ 
    current_day = time(NULL);
    fprintf(stderr,"Current time %i\n", current_day);

    current_day = current_day + 3600*timezone;
    fprintf(stderr,"Current time for time zone %i\n", current_day);
    //tm = localtime(&current_day);
    tm = gmtime(&current_day);
    current_day = mktime(tm)  ; /* today 00:00:00 */
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    //  tm->tm_isdst = 1;
    tm->tm_isdst = 0;
    //  current_day = mktime(tm) - 3600*timezone ; /* today 00:00:00 */
    current_day = mktime(tm) - 3600*timezone + 3600*localtimezone ; /* today 00:00:00 */

  
    if (dp)
        temp_data = dp->data().GetDataForTime(time(NULL));
#if 0
      // Debug begin
	//QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "Predeamon "<<"\n";
	    file.close();
	}
	// Debug end
#endif

    /* fill current date */
    if (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL)))) {
#if 0
      // Debug begin
//	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "Predeamon2 "<<"\n";
	    file.close();
	}
	// Debug end
#endif

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
        if (temp_data->Text() != "")
            temp_data->Text(_(temp_data->Text().c_str()));
        else
            temp_data->Text("");
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
            if ((time(NULL) - dp->LastUpdate()) > config->UpdatePeriod()){
                result_time = time(NULL) + 10;
            }else
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

#if 0
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "Predeamon3 Send DBUS messages"<<"\n";
	    file.close();
	}
#endif
        std::cerr<<"Create file with current weather or forecast information"<< std::endl;
        /*
        dbusclient->SetCurrentData(stationname.fromUtf8(config->stationname().c_str()),
                                   temp, temp_high, temp_low, 
                                   icon_string, description.fromUtf8(temp_data->Text().c_str()),
                                   result_time, temp_data->Current(),
                                   config->Lockscreen(), 
                                   config->Standbyscreen(), 
                                   t.toString("dd MMM h:mm")); 
        */
        QFile current_file(QDir::homePath() + "/.cache/harbour-meecast/current.xml");

        if (current_file.open(QIODevice::WriteOnly)){
            QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
            xmlWriter->setAutoFormatting(true);
            xmlWriter->setDevice(&current_file);
            xmlWriter->writeStartDocument();
            xmlWriter->writeStartElement("meecast");
            if (config->Lockscreen())
                xmlWriter->writeTextElement("lockscreen", "true");
            else
                xmlWriter->writeTextElement("lockscreen", "false");

            xmlWriter->writeTextElement("temperatureunit", config->TemperatureUnit().c_str());
            xmlWriter->writeTextElement("last_update_text", (_("Last update:")));
            xmlWriter->writeStartElement("station");
            xmlWriter->writeAttribute("name", stationname.fromUtf8(config->stationname().c_str()));
            xmlWriter->writeTextElement("last_update", t.toString("dd MMM h:mm"));

            xmlWriter->writeStartElement("period");
            xmlWriter->writeAttribute("itemnumber", "1");
            xmlWriter->writeAttribute("start", QString::number(t.toTime_t()));
            xmlWriter->writeAttribute("end", QString::number(result_time));
            if (temp_data->Current())
                xmlWriter->writeAttribute("current", "true");
            xmlWriter->writeTextElement("temperature", temp);
            xmlWriter->writeTextElement("temperature_hi", temp_high);
            xmlWriter->writeTextElement("temperature_low", temp_low);
            xmlWriter->writeTextElement("icon", icon_string);
            xmlWriter->writeTextElement("description", description.fromUtf8(_(temp_data->Text().c_str())));
            QDateTime t;
            t.setTime_t(temp_data->StartTime());
            xmlWriter->writeTextElement("short_day_name", QLocale().toString(t, "ddd"));

            xmlWriter->writeEndElement();

            /* fill other days for 5 days */
            i = 1; /* plus 1 second  */ 
            int itemnumber = 2;
            //std::cerr<<" Time for check "<< current_day + 15 * 3600  + i<<std::endl;
            while  (dp != NULL && ((temp_data = dp->data().GetDataForTime(current_day + 15 * 3600  + i )) && (i < 7*3600*24)) || (i < 7*3600*24)) {
                if (temp_data){
                    //std::cerr << "Success " << current_day + 15 * 3600 + i << std::endl;
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
         
                    xmlWriter->writeStartElement("period");
                    xmlWriter->writeAttribute("itemnumber", QString::number(itemnumber));
                    xmlWriter->writeAttribute("start", QString::number(temp_data->StartTime()));
                    xmlWriter->writeAttribute("end", QString::number(temp_data->EndTime()));

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

                    xmlWriter->writeTextElement("temperature", temp);
                    xmlWriter->writeTextElement("temperature_hi", temp_high);
                    xmlWriter->writeTextElement("temperature_low", temp_low);
                    xmlWriter->writeTextElement("icon", icon_string);
                    xmlWriter->writeTextElement("description", description.fromUtf8(_(temp_data->Text().c_str())));
                    QDateTime t;
                    t.setTime_t(temp_data->StartTime());
                    xmlWriter->writeTextElement("short_day_name", QLocale().toString(t, "ddd"));
                    xmlWriter->writeEndElement();
                    itemnumber++;
                }
              i = i + 3600*24;
            }

            xmlWriter->writeEndElement();
            xmlWriter->writeEndElement();
            xmlWriter->writeEndDocument();
            current_file.close();
        }else{
            std::cerr<<"Problem with current.xml file"<< std::endl;
            return -2;
        }
  }else{
      if (dp){
          dp->DeleteInstance();
          dp = NULL;
      }
      return -1;
  }

  if (dp){
      dp->DeleteInstance();
      dp = NULL;
  }
  return 0;

}

int
main (int argc, char *argv[]){
  Core::DataParser* dp = NULL;
  int success = 0;


#if 0
      // Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "Predeamon beginn";
	    file.close();
	}
	// Debug end
#endif

    QCoreApplication a(argc, argv);

    config = create_and_fill_config();
    if (!config){
        std::cerr<<"Problem with config file"<<std::endl;
        exit (-1);
    }


    /* Locale */
    // Set up the translator.
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString("omweather_%1").arg(locale_string);
/*    std::cerr<<filename.toStdString().c_str()<<std::endl; */

    QString localepath =QString::fromStdString(Core::AbstractConfig::prefix + "/share/harbour-meecast/locale");
    if (translator.load(filename, localepath)) {
        /* std::cerr<<"Success TR"<<std::endl; */
        a.installTranslator(&translator);
    }

//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    for (unsigned int i=1; i<config->languagesList().size(); i++){
        if (config->languagesList().at(i).first == config->Language()){
            QLocale::setDefault(QLocale(config->languagesList().at(i).second.c_str()));
            filename = QString("omweather_%1").arg(config->languagesList().at(i).second.c_str());
            std::cerr<<filename.toStdString().c_str()<<std::endl; 
            QString localepath = QString::fromStdString(Core::AbstractConfig::prefix + "/share/harbour-meecast/locale");
            if (translator.load(filename, localepath)) {
                    std::cerr<<"Success TR"<<std::endl;
                    a.installTranslator(&translator);
            }
        }
    }



    /* Check time for previous updating */
    dp = current_data(config->stationsList().at(config->current_station_id())->fileName());

    /* 10*60 = 10 minutes - minimal time between updates */
    if ((!dp) || (dp && (abs(time(NULL) - dp->LastUpdate()) > 10*60))){
        /*update weather forecast*/
        if (config->UpdatePeriod() != INT_MAX || config->UpdatePeriod() != 0){
            if ((!dp) || (time(NULL) - dp->LastUpdate()) > config->UpdatePeriod()){
                for (unsigned int i = 0; i < config->stationsList().size();i++){
                    if (config->stationsList().at(i)->updateData(true)){
                        success ++;
                    }
                }
            }
        }
    }

    dp = current_data(config->stationsList().at(config->current_station_id())->fileName());

    int result = create_current_xml_file(dp);
  return result;
}
