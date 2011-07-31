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

#include "updateqml.h"
#include "omweatherintf.h"
#include "omweatheradaptor.h"

#define DATA_XSD_PATH "/opt/com.meecast.omweather/share/xsd/data.xsd"

UpdateQml::UpdateQml() : QObject()
{
    config = NULL;

    new OmweatherAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/", this);

    com::meecast::omweather *iface;
    iface = new com::meecast::omweather(QString(), QString(), QDBusConnection::sessionBus(), this);
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.meecast.omweather", "configChange", this, SLOT(configChangeSlot()));

    thread = new UpdateThread();
    connect(thread, SIGNAL(finished()), this, SLOT(downloadFinishedSlot()));
}
UpdateQml::~UpdateQml()
{
    if (config) delete config;
    if (thread) delete thread;
}

void
UpdateQml::configChangeSlot()
{
    makeQmlData();
    //emit reload();
}
void
UpdateQml::downloadFinishedSlot()
{
    makeQmlData();
}

void
UpdateQml::updateData()
{
    thread->start();
}

/*from config and downloaded files makes qmldata.xml, then emit reload signal */
void
UpdateQml::makeQmlData()
{
    Core::DataParser *dp = NULL;
    Core::Data *temp_data = NULL;
    //char buffer[4096];
    time_t current_day;
    struct tm   *tm = NULL;
    int year, current_month;
    int i, num = 0;

    //config->ReLoadConfig();
    //config->saveConfig();
    if (config) delete config;
    try{
        config = new Core::Config(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
    }
    catch(const std::string &str){
        config = new Core::Config();
        config->saveConfig();
    }
    catch(const char *str){
        config = new Core::Config();
        config->saveConfig();
    }

    QDomDocument doc;
    QDomElement station;
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
    QDomElement root = doc.createElement("data");
    doc.appendChild(root);

    QDomElement el1 = doc.createElement("update");
    QDomElement el = doc.createElement("period");
    QDomText t;
    if (config->UpdatePeriod() != INT_MAX)
        t = doc.createTextNode(QString::number(config->UpdatePeriod()*1000));
    else
        t = doc.createTextNode("0");
    el.appendChild(t);
    el1.appendChild(el);
    root.appendChild(el1);

    Core::StationsList::iterator cur;
    for (cur=config->stationsList().begin(); cur<config->stationsList().end(); cur++){
        qDebug() <<"111 add station "<< QString::fromStdString((*cur)->fileName()) << " " << QString::fromStdString((*cur)->name());
        station = doc.createElement("station");
        station.setAttribute("name", QString::fromStdString((*cur)->name()));
        station.setAttribute("id", QString::fromStdString((*cur)->id()));
        dp = current_data((*cur)->fileName());
        if (dp){
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
            

        }
        root.appendChild(station);
    }
    
    QFile file(QString::fromStdString(Core::AbstractConfig::getConfigPath() + "qmldata.xml"));
    if (!file.open(QIODevice::WriteOnly)){
        std::cerr<<"error file open 1.xml"<<std::endl;
        throw("Invalid destination file");
    }

    QTextStream ts(&file);
    ts << doc.toString();
    file.close();

    if (dp){
        delete dp;
        dp = NULL;
    }

    emit reload();
}
QDomElement
UpdateQml::make_item(QDomDocument doc, Core::Data *data, int num, bool current)
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
        QDomText t = doc.createTextNode(QString::number(data->temperature_hi().value()) +
                                        QString::fromStdString(config->TemperatureUnit()));
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->temperature_low().value() != INT_MAX){
        QDomElement el = doc.createElement("temperature_low");
        data->temperature_low().units(config->TemperatureUnit());
        QDomText t = doc.createTextNode(QString::number(data->temperature_low().value()) +
                                        QString::fromStdString(config->TemperatureUnit()));
        el.appendChild(t);
        item.appendChild(el);
    }
    if (data->temperature().value() != INT_MAX){
        QDomElement el = doc.createElement("temperature");
        data->temperature().units(config->TemperatureUnit());
        QDomText t = doc.createTextNode(QString::number(data->temperature().value()) +
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
Core::DataParser*
UpdateQml::current_data(std::string& str){
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
