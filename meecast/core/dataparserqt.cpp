/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2006-2011 Pavel Fialko
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


#include "dataparserqt.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    DataParserQt::DataParserQt(const QString filename, const QUrl schema_filename) : ParserQt(filename, schema_filename)
    {
        _timezone = 0;
        _list = new DataList;

        QDomElement root = _doc.documentElement();
        Data* forecast_data;

        QDomNodeList nodelist = root.elementsByTagName("timezone");
        if (nodelist.count() == 1) {
            _timezone = nodelist.at(0).toElement().text().toInt();
            qDebug() << "timezone = " << _timezone;
        }
        nodelist = root.elementsByTagName("period");
        for (int i=0; i<nodelist.count(); i++){
            forecast_data = new Data();
            QDomElement e = nodelist.at(i).toElement();
            forecast_data->StartTime(e.attribute("start").toInt());
            forecast_data->EndTime(e.attribute("end").toInt());
            if (e.hasAttribute("current") && e.attribute("current") == "true")
                forecast_data->Current(1);
            if (e.hasAttribute("hour") && e.attribute("hour") == "true")
                forecast_data->Hour(1);
            QDomNode n = e.firstChild();
            while (!n.isNull()){
                QDomElement el = n.toElement();
                QString tag = el.tagName();

                if (tag == "temperature" || tag == "temperature_hi"){
                    forecast_data->temperature_hi().value(el.text().toFloat());
                }else if (tag == "temperature_low"){
                    forecast_data->temperature_low().value(el.text().toFloat());
                }else if (tag == "icon"){
                    forecast_data->Icon(el.text().toInt());
                }else if (tag == "description"){
                    forecast_data->Text(el.text().toStdString());
                }else if (tag == "humidity"){
                    forecast_data->Humidity(el.text().toInt());
                }else if (tag == "wind_speed"){
                    forecast_data->WindSpeed(el.text().toFloat());
                }else if (tag == "wind_direction"){
                    forecast_data->WindDirection(el.text().toStdString());
                }else if (tag == "flike"){
                    std::cout<<"flike 1 "<<std::sendl;
                    forecast_data->Flike().value(el.text().toFloat());
                }else if (tag == "pressure"){
                    forecast_data->Pressure(el.text().toInt());
                }else if (tag == "ppcp"){
                    forecast_data->Ppcp(el.text().toFloat());
                }else if (tag == "sunrise"){
                    std::cout<<"Sunrise 1 "<<std::sendl;
                    forecast_data->SunRiseTime(el.text().toInt());
                }else if (tag == "sunset")
                    forecast_data->SunSetTime(el.text().toInt());


                n = n.nextSibling();
            }
            _list->push_back(forecast_data);
        }

    }


    DataList&
    DataParserQt::data()
    {
        return *_list;
    }
    DataParserQt::~DataParserQt()
    {
        delete _list;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
