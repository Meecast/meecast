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


#include "datamodel.h"

DataModel::DataModel(DataItem* prototype, QObject *parent) :
    QAbstractListModel(parent), _prototype(prototype)
{
    setRoleNames(_prototype->roleNames());
    try{
        _config = new Core::Config(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        _config = new Core::Config();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        _config = new Core::Config();
    }
}

int
DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list.size();
}

QVariant
DataModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list.size())
        return QVariant();
    return _list.at(index.row())->data(role);
}
QVariant
DataModel::getdata(const int index, QString role)
{
    if (index < 0 || index >= _list.size())
        return QVariant();
    DataItem *item = _list.at(index);
    return item->data(item->getRole(role));
}
DataItem*
DataModel::find(const int row)
{
    qDebug() << "222 " << row << _list.at(row)->description();
    return _list.at(row);
}

DataModel::~DataModel()
{
    delete _prototype;
}

void
DataModel::clear()
{
    //qDeleteAll(this->_list);
    this->_list.clear();
    //emit dataChanged(this->createIndex(0, 0), this->createIndex(count, 0));
    this->reset();
}

void
DataModel::appendRow(DataItem *item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _list.append(item);
    endInsertRows();
    //return (_list.size() - 1);
}
void
DataModel::update(QString filename, int  period)
{
    this->clear();
    DataItem *forecast_data = NULL;

    int i;
    Core::DataParser* dp = NULL;
    Core::Data *temp_data = NULL;
    time_t current_day;
    struct tm   *tm = NULL;
    int year, current_month;

    if (_config) delete _config;
    try{
        _config = new Core::Config(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        _config = new Core::Config();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        _config = new Core::Config();
    }

    if (!filename.isEmpty()){
        try{
            dp = new Core::DataParser(filename.toStdString(),
                                      Core::AbstractConfig::prefix+Core::AbstractConfig::schemaPath+"data.xsd");
        }
        catch(const std::string &str){
            std::cerr<<"Error in DataParser class: "<< str << std::endl;
            //return NULL;
        }
        catch(const char *str){
            std::cerr<<"Error in DataParser class: "<< str << std::endl;
            //return NULL;
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
    switch (period) {
        case current_period:
            i = 0;
            if (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL) + i))) {
                forecast_data = new DataItem(temp_data);
                forecast_data->Text(forecast_data->Text().c_str());
                forecast_data->SunRiseTime(dp->data().GetSunRiseForTime(time(NULL)  + i));
                forecast_data->SunSetTime(dp->data().GetSunSetForTime(time(NULL)  + i));
                forecast_data->LastUpdate(dp->LastUpdate());
                forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                forecast_data->windunit = _config->WindSpeedUnit().c_str();
                forecast_data->pressureunit = _config->PressureUnit().c_str();
                this->appendRow(forecast_data);
                MeecastIf* dbusclient = new MeecastIf("com.meecast.applet", "/com/meecast/applet", QDBusConnection::sessionBus(), 0);
                QString icon_string =  _config->iconspath().c_str();
                icon_string.append("/") ;
                icon_string.append(_config->iconSet().c_str());
                icon_string.append("/") ;
                icon_string.append(forecast_data->icon());
                QString stationname = "";
                dbusclient->SetCurrentData(stationname.fromUtf8(_config->stationname().c_str()), forecast_data->temperature(), 
                                           forecast_data->temperature_high(), forecast_data->temperature_low(), 
                                           icon_string, forecast_data->EndTime(), forecast_data->current()); 
            }
            break;
        case current_night_period:
            if (dp != NULL && (temp_data = dp->data().GetDataForTime(current_day + 3*3600))) {
                forecast_data = new DataItem(temp_data);
                forecast_data->Text(forecast_data->Text().c_str());
                forecast_data->SunRiseTime(dp->data().GetSunRiseForTime(current_day + 3*3600));
                forecast_data->SunSetTime(dp->data().GetSunSetForTime(current_day + 3*3600));
                forecast_data->LastUpdate(dp->LastUpdate());
                forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                forecast_data->windunit = _config->WindSpeedUnit().c_str();
                forecast_data->pressureunit = _config->PressureUnit().c_str();
                this->appendRow(forecast_data);
            }
            break;
        case day_period:
          //  i = 3600*24;
            i = 0;
            while  (dp != NULL && (temp_data = dp->data().GetDataForTime(current_day + 14*3600 + i))) {
                forecast_data = new DataItem(temp_data);
                forecast_data->Text(forecast_data->Text().c_str());
                forecast_data->SunRiseTime(dp->data().GetSunRiseForTime(current_day + 14*3600 + i));
                forecast_data->SunSetTime(dp->data().GetSunSetForTime(current_day + 14*3600  + i));
                forecast_data->LastUpdate(dp->LastUpdate());
                forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                forecast_data->windunit = _config->WindSpeedUnit().c_str();
                forecast_data->pressureunit = _config->PressureUnit().c_str();
                this->appendRow(forecast_data);
                i = i + 3600*24;
            }
            break;
        case night_period:
         //   i = 3600*24;
            i = 0;
            while  (dp != NULL && (temp_data = dp->data().GetDataForTime(current_day + 3*3600 + i))) {
                forecast_data = new DataItem(temp_data);
                forecast_data->Text(forecast_data->Text().c_str());
                forecast_data->SunRiseTime(dp->data().GetSunRiseForTime(current_day + 3*3600 + i));
                forecast_data->SunSetTime(dp->data().GetSunSetForTime(current_day + 3*3600  + i));
                forecast_data->LastUpdate(dp->LastUpdate());
                forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                forecast_data->windunit = _config->WindSpeedUnit().c_str();
                forecast_data->pressureunit = _config->PressureUnit().c_str();
                this->appendRow(forecast_data);
                i = i + 3600*24;
            }
            break;

    }
    this->reset();
}
