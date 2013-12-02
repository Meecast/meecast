/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2012 Vlad Vasilyeu
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
    //setRoleNames(_prototype->roleNames());
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(_prototype->roleNames());
#endif
    try{
       _config =  ConfigQml::Instance(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        _config =  ConfigQml::Instance();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        _config =  ConfigQml::Instance();
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
    //this->reset();
    this->endResetModel();
}

void
DataModel::remove(const int row)
{
    this->_list.removeAt(row);
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
DataModel::reload_data(QString filename)
{
   Core::DataParser* dp = NULL;
   if (!filename.isEmpty()){
        try{
            dp = Core::DataParser::Instance(filename.toStdString(),
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

}
void
DataModel::update_model(int period)
{
    this->clear();
    DataItem *forecast_data = NULL;

    int i;
    Core::DataParser* dp = NULL;
    Core::Data *temp_data = NULL;
    time_t current_day;
    time_t current_hour;
    struct tm   *tm = NULL;
    int year, current_month;
    int timezone = 0;
    int localtimezone = 0;
    struct tm time_tm1;
    struct tm time_tm2;

    /* std::cerr<<"Update model"<<std::endl; */

    dp = Core::DataParser::Instance();
    if (dp){
        timezone = dp->timezone();
    }

    /* Set localtimezone */

    current_day = time(NULL);
    gmtime_r(&current_day, &time_tm1);
    localtime_r(&current_day, &time_tm2);
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 

    /* set current day */ 
    current_day = time(NULL);

    //tm = localtime(&current_day);
    tm = gmtime(&current_day);
    year = 1900 + tm->tm_year;
    current_month = tm->tm_mon;
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 0;
    current_day = mktime(tm) - 3600*timezone + 3600*localtimezone;

    std::string mapfilename(Core::AbstractConfig::getCachePath());
    if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0 &&
        _config->stationsList().at(_config->current_station_id())){

        mapfilename += _config->stationsList().at(_config->current_station_id())->sourceName().c_str();
        mapfilename += "_";
        mapfilename += _config->stationsList().at(_config->current_station_id())->id().c_str();
        mapfilename += "_map_";
        mapfilename += "%s.png";
    }
    std::string basemapfilename(Core::AbstractConfig::getCachePath());
    if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0 &&
        _config->stationsList().at(_config->current_station_id())){

        basemapfilename += _config->stationsList().at(_config->current_station_id())->sourceName().c_str();
        basemapfilename += "_";
        basemapfilename += _config->stationsList().at(_config->current_station_id())->id().c_str();
        basemapfilename += "_basemap_";
        basemapfilename += "%s.png";
    }

    if (dp)
        temp_data = dp->data().GetDataForTime(time(NULL));
//    if (temp_data)
//        current_day = current_day + 3600*dp->timezone();

    /* fill current date */
    switch (period) {
        case current_period:
            i = 0;
            if (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL) + i))) {
                forecast_data = new DataItem(temp_data);

                /* Prepare */
                if (forecast_data->Text() != "")
                    forecast_data->Text(_(forecast_data->Text().c_str()));

                if (dp->data().GetSunRiseForTime(time(NULL) + i) > 0)
                    forecast_data->SunRiseTime(dp->data().GetSunRiseForTime(time(NULL)  + i) + 3600*timezone - 3600*localtimezone);
                if (dp->data().GetSunSetForTime(time(NULL) + i) > 0)
                    forecast_data->SunSetTime(dp->data().GetSunSetForTime(time(NULL)  + i) + 3600*timezone - 3600*localtimezone);
                forecast_data->LastUpdate(dp->LastUpdate());
                forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                forecast_data->windunit = _config->WindSpeedUnit().c_str();
                forecast_data->pressureunit = _config->PressureUnit().c_str();
                forecast_data->visibleunit = _config->VisibleUnit().c_str();
                /* Add Map */
                forecast_data->MapPattern(mapfilename);
                forecast_data->MapBackgroundPattern(basemapfilename);

                /* Add to list */
                this->appendRow(forecast_data);
                MeecastIf* dbusclient = new MeecastIf("com.meecast.applet", "/com/meecast/applet", QDBusConnection::sessionBus(), 0);
                QString icon_string =  _config->iconspath();
                icon_string.append("/") ;
                icon_string.append(_config->iconSet().c_str());
                icon_string.append("/") ;
                icon_string.append(forecast_data->icon());
                QString stationname = "";
                /* Preparing time for updateing */
                uint result_time = 0;
                if (_config->UpdatePeriod() != INT_MAX){
                    if ((time(NULL) - dp->LastUpdate()) > _config->UpdatePeriod())
                        result_time = time(NULL) + 10;
                    else
                        if (dp->LastUpdate() + _config->UpdatePeriod() < temp_data->EndTime())
                           result_time = dp->LastUpdate() + _config->UpdatePeriod();  
                        else
                           result_time = temp_data->EndTime();
                }else
                    result_time = temp_data->EndTime();
                QDateTime t;
                t.setTime_t(dp->LastUpdate());
                QString description = forecast_data->Text().c_str();
                dbusclient->SetCurrentData(_config->stationname(), 
                                           forecast_data->temperature(), 
                                           forecast_data->temperature_high(), 
                                           forecast_data->temperature_low(), 
                                           icon_string,
                                           description.fromUtf8(temp_data->Text().c_str()),
                                           result_time, forecast_data->current(), 
                                           _config->Lockscreen(), _config->Standbyscreen(),
                                           t.toString("dd MMM h:mm")); 
           }
            break;
        case current_night_period:
            if (dp != NULL && (temp_data = dp->data().GetDataForTime(current_day + 3*3600))) {
                forecast_data = new DataItem(temp_data);
                if (forecast_data->Text() != "")
                    forecast_data->Text(forecast_data->Text().c_str());
                if (dp->data().GetSunRiseForTime(current_day + 3*3600) > 0)
                    forecast_data->SunRiseTime(dp->data().GetSunRiseForTime(current_day + 3*3600) + 3600*timezone - 3600*localtimezone);
                if (dp->data().GetSunSetForTime(current_day + 3*3600) > 0)
                    forecast_data->SunSetTime(dp->data().GetSunSetForTime(current_day + 3*3600) + 3600*timezone - 3600*localtimezone);
                forecast_data->LastUpdate(dp->LastUpdate());
                forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                forecast_data->windunit = _config->WindSpeedUnit().c_str();
                forecast_data->pressureunit = _config->PressureUnit().c_str();
                forecast_data->visibleunit = _config->VisibleUnit().c_str();
                /* Add Map */
                forecast_data->MapPattern(mapfilename);
                forecast_data->MapBackgroundPattern(basemapfilename);

                this->appendRow(forecast_data);
            }
            break;
        case day_period:
          //  i = 3600*24;
           // fprintf(stderr,"First day in datamodel %i\n", current_day + 14 * 3600);
            i = 1;
            while  (dp != NULL && ((temp_data = dp->data().GetDataForTime(current_day + 15*3600 + i)) || (i < 7*3600*24))) {
                if (temp_data){
                    forecast_data = new DataItem(temp_data);
                    if (forecast_data->Text() != "")
                        forecast_data->Text(forecast_data->Text().c_str());
                    if (dp->data().GetSunRiseForTime(current_day + 15*3600 + i) > 0)
                        forecast_data->SunRiseTime(dp->data().GetSunRiseForTime(current_day + 15*3600 + i) + 3600*timezone - 3600*localtimezone);
                    if (dp->data().GetSunSetForTime(current_day + 15*3600 + i) > 0)
                        forecast_data->SunSetTime(dp->data().GetSunSetForTime(current_day + 15*3600  + i) + 3600*timezone - 3600*localtimezone);
                    forecast_data->LastUpdate(dp->LastUpdate());
                    forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                    forecast_data->windunit = _config->WindSpeedUnit().c_str();
                    forecast_data->pressureunit = _config->PressureUnit().c_str();
                    forecast_data->visibleunit = _config->VisibleUnit().c_str();
                    /* Add Map */
                    if (i == 0){
                        forecast_data->MapPattern(mapfilename);
                        forecast_data->MapBackgroundPattern(basemapfilename);
                    }

                    this->appendRow(forecast_data);
                }
                i = i + 3600*24;
            }
            break;
        case night_period:
         //   i = 3600*24;
            i = 0;
            while  (dp != NULL && ((temp_data = dp->data().GetDataForTime(current_day + 3*3600 + i)) || (i < 7*3600*24))) {
                if (temp_data){
                    forecast_data = new DataItem(temp_data);

                    if (forecast_data->Text() != "")
                        forecast_data->Text(forecast_data->Text().c_str());
                    if (dp->data().GetSunRiseForTime(current_day + 3*3600 + i) > 0)
                        forecast_data->SunRiseTime(dp->data().GetSunRiseForTime(current_day + 3*3600 + i) + 3600*timezone - 3600*localtimezone);
                    if (dp->data().GetSunSetForTime(current_day + 3*3600 + i) > 0)
                        forecast_data->SunSetTime(dp->data().GetSunSetForTime(current_day + 3*3600  + i) + 3600*timezone - 3600*localtimezone);
                    forecast_data->LastUpdate(dp->LastUpdate());
                    forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                    forecast_data->windunit = _config->WindSpeedUnit().c_str();
                    forecast_data->pressureunit = _config->PressureUnit().c_str();
                    forecast_data->visibleunit = _config->VisibleUnit().c_str();
                    /* Add Map */
                    if (i == 0){
                        forecast_data->MapPattern(mapfilename);
                        forecast_data->MapBackgroundPattern(basemapfilename);
                    }

                    this->appendRow(forecast_data);
                }
                i = i + 3600*24;
            }
            break;
        case hours_period:
	        /* set current hour */
            current_hour = time(NULL);
            //tm = gmtime(&current_hour);
            tm = localtime(&current_hour);
            tm->tm_sec = 0; tm->tm_min = 1; 
            tm->tm_isdst = 1;
            current_hour = mktime(tm); 
            i =0;
            
            /* fill hours */
            while  (dp != NULL && i<5*24*3600) {
                if (temp_data = dp->data().GetDataForTime(current_hour + i, true)){
                    if (temp_data->StartTime() + 60 == current_hour + i){
                        forecast_data = new DataItem(temp_data);
                        if (forecast_data->Text() != "")
                            forecast_data->Text(_(forecast_data->Text().c_str()));
                        forecast_data->temperatureunit = _config->TemperatureUnit().c_str();
                        forecast_data->windunit = _config->WindSpeedUnit().c_str();
                        forecast_data->pressureunit = _config->PressureUnit().c_str();
                        this->appendRow(forecast_data);
                    }
                }
                i = i + 3600;
            }
            break;
    }
    dp->DeleteInstance();
    //this->reset();
    this->endResetModel();
    
}
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
QHash<int, QByteArray> DataModel::roleNames() const {
    return _prototype->roleNames();
}

#endif
