/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2014 Vlad Vasilyeu
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


#include "data.h"
#include <limits.h>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Data::Data(){
        _flike = new Temperature;
        //_windSpeed = INT_MAX;
        _windSpeed = new Windspeed(INT_MAX, "m/s");
        _visible = new Visible(INT_MAX, "m");
        _windGust = INT_MAX;
        _humidity = INT_MAX;
        //_pressure = INT_MAX;
        _pressure = new Pressure(INT_MAX, "mbar");
        _icon = INT_MAX;
        _windDirection = new std::string("N/A");
        _text = new std::string("N/A");
        _short_day_name = new std::string();
        _full_day_name = new std::string();
        _full_month_name = new std::string();
        _day_of_month_name = new std::string();
        _month_name = new std::string();
        _map_pattern = new std::string();
        _map_background_pattern = new std::string();
        _temperature_hi = new Temperature(INT_MAX, "C");
        _temperature_low = new Temperature(INT_MAX, "C");
        _temperature = new Temperature(INT_MAX, "C");
        _current = false;
        _hour = false;
        _start_time = 0;
        _end_time = 0;
        _sunrise = 0;
        _sunset = 0;
        _ppcp = INT_MAX;
        _uv_index = INT_MAX;
    }
    ////////////////////////////////////////////////////////////////////////////////
    Data::Data(const Data& data){

    }
    ////////////////////////////////////////////////////////////////////////////////
    Data::Data(const Data *data){
        /* fprintf(stderr," Data::Data(const Data *data) \n"); */
        //_flike = data->_flike;
        _windSpeed =  new Windspeed(data->_windSpeed->value(), data->_windSpeed->units());
        _visible = new Visible(data->_visible->value(), data->_visible->units());
        _windGust = INT_MAX;
        _humidity = data->_humidity;
        //_pressure = INT_MAX;
        _pressure = new Pressure(data->_pressure->value(), data->_pressure->units());
        _icon = data->_icon;
        _windDirection =  new std::string();
        _windDirection->assign(data->_windDirection->data());
        _text = new std::string;
        _text->assign(data->_text->data());
        _temperature_hi = new Temperature(data->_temperature_hi->value(), data->_temperature_hi->units());
        _temperature_low = new Temperature(data->_temperature_low->value(), data->_temperature_low->units());
        _temperature = new Temperature(data->_temperature->value(), data->_temperature->units());
        _current = data->_current;
        _hour = data->_hour;
        _start_time = data->_start_time;
        _end_time = data->_end_time;
        _flike = new Temperature(data->_flike->value(), data->_flike->units());
        _ppcp = data->_ppcp;
        _uv_index = data->_uv_index;
        _short_day_name = new std::string();
        _short_day_name->assign(data->_short_day_name->data());
        _full_day_name = new std::string();
        _full_day_name->assign(data->_full_day_name->data());
        _full_month_name = new std::string();
        _full_month_name->assign(data->_full_month_name->data()); 
        _day_of_month_name = new std::string();
        _day_of_month_name->assign(data->_day_of_month_name->data());
        _month_name = new std::string();
        _month_name->assign(data->_month_name->data());
        _map_pattern = new std::string();
        _map_pattern->assign(data->_map_pattern->data());
        _map_background_pattern = new std::string();
        _map_background_pattern->assign(data->_map_background_pattern->data());
        _sunrise = data->_sunrise;
        _sunset = data->_sunset;
    }
////////////////////////////////////////////////////////////////////////////////
    void
    Data::StartTime(time_t start_time){
        char buffer[2048];
        struct tm *tmp_time_date_struct;

        _start_time = start_time;

        delete _short_day_name;
        delete _full_day_name;
        delete _full_month_name;
        delete _day_of_month_name;
        delete _month_name;

        memset(buffer, 0, sizeof(buffer));
        tmp_time_date_struct = localtime(&start_time);
        strftime(buffer, sizeof(buffer), "%a", tmp_time_date_struct);
        _short_day_name = new std::string(buffer);
        strftime(buffer, sizeof(buffer), "%A", tmp_time_date_struct);
        _full_day_name = new std::string(buffer);
        strftime(buffer, sizeof(buffer), "%B", tmp_time_date_struct);
        _full_month_name = new std::string(buffer);
        strftime(buffer, sizeof(buffer), "%d", tmp_time_date_struct);
        _day_of_month_name = new std::string(buffer);
        strftime(buffer, sizeof(buffer), "%m", tmp_time_date_struct);
        _month_name = new std::string(buffer);
    }
    time_t
    Data::StartTime() const{
        return _start_time;
    }
////////////////////////////////////////////////////////////////////////////////
    void
    Data::EndTime(time_t end_time){
        _end_time = end_time;
    }
    time_t
    Data::EndTime() const{
        return _end_time;
    }
////////////////////////////////////////////////////////////////////////////////
    void
    Data::SunRiseTime(time_t sunrise_time){
        _sunrise = sunrise_time;

    }
    time_t
    Data::SunRiseTime() const{
        return _sunrise;
    }
////////////////////////////////////////////////////////////////////////////////
    time_t
    Data::DayLength() const{
        time_t t = 0;
        t = _sunset - _sunrise;
        if (t < 0)
            t = 0;
        return t;
    }
////////////////////////////////////////////////////////////////////////////////
    void
    Data::SunSetTime(time_t sunset_time){
        _sunset = sunset_time;
    }
    time_t
    Data::SunSetTime() const{
        return _sunset;
    }
////////////////////////////////////////////////////////////////////////////////
     Temperature&
     Data::Flike(){
         /* need to check type (Celsius Farenheit ) */
         return *_flike;
     }
////////////////////////////////////////////////////////////////////////////////
     /*
     void
     Data::WindSpeed(float windspeed){
         _windSpeed = windspeed;
     }
     float
     Data::WindSpeed() const{
         // need to check type ( )
         return _windSpeed;
     }*/
     Windspeed& 
     Data::WindSpeed(){
         return *_windSpeed;
     }
////////////////////////////////////////////////////////////////////////////////
     Visible& 
     Data::ViSible(){
         return *_visible;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::WindDirection(const std::string& text){
         /* Check possible direction (N,NW,NNW,NE,NEE,S,SW,SWW,SE,SEE, E, W) */
         if (text !="")
            _windDirection->assign(text);
         else
            _windDirection->assign("N/A");
     }
     std::string&
     Data::WindDirection(){
         return *_windDirection;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::MapPattern(const std::string& text){
         _map_pattern->assign(text);
     }
     std::string&
     Data::MapPattern(){
         return *_map_pattern;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::MapBackgroundPattern(const std::string& text){
         _map_background_pattern->assign(text);
     }
     std::string&
     Data::MapBackgroundPattern(){
         return *_map_background_pattern;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::WindGust(float windgust){
         _windGust = windgust;
     }
     float
     Data::WindGust() const{
         /* need to check type ( ) */
         return _windGust;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::Humidity(int humidity){
         _humidity = humidity;
     }
     int
     Data::Humidity() const{
         /* need to check type ( ) */
         return _humidity;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::Current(bool current){
         _current = current;
     }
     bool
     Data::Current() const{
         return _current;
     }
///////////////////////////////////////////////////////////////////////////////
     void
     Data::Hour(bool hour){
         _hour = hour;
     }
     bool
     Data::Hour() const{
         return _hour;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::Icon(int icon){
         _icon = icon;
     }
     int
     Data::Icon() const{
         if (_icon >49|| _icon <0)
             return 49;
         return _icon;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::UVindex(int index){
         _uv_index = index;
     }
     int
     Data::UVindex() const{
         return _uv_index;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::Text(const std::string& text){
         /* Check possible direction (N,NW,NNW,NE,NEE,S,SW,SWW,SE,SEE, E, W) */
         _text->assign(text);
     }
////////////////////////////////////////////////////////////////////////////////
     std::string&
     Data::Text(){
         return *_text;
     }
////////////////////////////////////////////////////////////////////////////////
     std::string&
     Data::ShortDayName(){
         return *_short_day_name;
     }
////////////////////////////////////////////////////////////////////////////////
     std::string&
     Data::FullDayName(){
         return *_full_day_name;
     }
////////////////////////////////////////////////////////////////////////////////
     std::string&
     Data::FullMonthName(){
         return *_full_month_name;
     }
////////////////////////////////////////////////////////////////////////////////
     std::string&
     Data::DayOfMonthName(){
         return *_day_of_month_name;
     }
////////////////////////////////////////////////////////////////////////////////
    std::string&
    Data::MonthName(){
        return *_month_name;
    }
////////////////////////////////////////////////////////////////////////////////
    time_t
    Data::GetTimeDistance(time_t need_time){
        if(_end_time >= need_time && _start_time <= need_time)
            return (_end_time - _start_time);
        else
            return -1;
    }
////////////////////////////////////////////////////////////////////////////////
    Data::~Data(){
       /* fprintf(stderr, " Data::~Data()\n"); */
       delete _windDirection;
       delete _windSpeed;
       delete _visible;
       delete _text;
       delete _temperature_hi;
       delete _temperature_low;
       delete _temperature;
       delete _pressure;
       delete _flike;
       delete _short_day_name;
       delete _full_day_name;
       delete _full_month_name;
       delete _day_of_month_name;
       delete _month_name;
       delete _map_pattern;
       delete _map_background_pattern;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature& Data::temperature_hi(){
        return *_temperature_hi;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature& Data::temperature_low(){
        return *_temperature_low;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature& Data::temperature(){
        return *_temperature;
    }
////////////////////////////////////////////////////////////////////////////////
    /*void
    Data::Pressure(float pressure){
        _pressure =(int)pressure;
    }

    int
    Data::Pressure() const{
        return _pressure;
    }*/
    Pressure& Data::pressure(){
        return *_pressure;
    }
////////////////////////////////////////////////////////////////////////////////
    void
    Data::Ppcp(float ppcp){
        _ppcp = ppcp;
    }

    float
    Data::Ppcp() const{
        /* need to check type ( ) */
        return _ppcp;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
