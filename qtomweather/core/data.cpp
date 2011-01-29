#include "data.h"
#include <limits.h>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Data::Data(){
        _flike = new Temperature;
        _windSpeed = INT_MAX;
        _windGust = INT_MAX;
        _humidity = INT_MAX;
        _pressure = INT_MAX;
        _icon = INT_MAX;
        _windDirection = new std::string("N/A");
        _text = new std::string("N/A");
        _short_day_name = new std::string();
        _temperature_hi = new Temperature;
        _temperature_low = new Temperature;
        _current = 0;
        _start_time = 0;
        _end_time = 0;
        _ppcp = INT_MAX;
    }
    ////////////////////////////////////////////////////////////////////////////////
    Data::Data(const Data& data){

    }
    ////////////////////////////////////////////////////////////////////////////////
    Data::Data(const Data *data){
        _flike = data->_flike;
        _windSpeed = data->_windSpeed;
        _windGust = INT_MAX;
        _humidity = data->_humidity;
        _pressure = INT_MAX;
        _icon = data->_icon;
        _windDirection = data->_windDirection;
        _text = data->_text;
        _temperature_hi = data->_temperature_hi;
        _temperature_low = data->_temperature_low;
        _current = data->_current;
        _start_time = data->_start_time;
        _end_time = data->_end_time;
        _pressure = data->_pressure;
        _flike = data->_flike;
        _ppcp = data->_ppcp;
        _short_day_name = data->_short_day_name;
    }
////////////////////////////////////////////////////////////////////////////////
    void
    Data::StartTime(time_t start_time){
        char buffer[2048];
        struct tm *tmp_time_date_struct;

        _start_time = start_time;

        delete _short_day_name;
        tmp_time_date_struct = gmtime(&start_time);
        strftime(buffer, sizeof(buffer), "%a", tmp_time_date_struct);
        _short_day_name = new std::string(buffer);
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
     Temperature&
     Data::Flike(){
         /* need to check type (Celsius Farenheit ) */
         return *_flike;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::WindSpeed(float windspeed){
         _windSpeed = windspeed;
     }
     float
     Data::WindSpeed() const{
         /* need to check type ( ) */
         return _windSpeed;
     }
////////////////////////////////////////////////////////////////////////////////
     void
     Data::WindDirection(const std::string& text){
         /* Check possible direction (N,NW,NNW,NE,NEE,S,SW,SWW,SE,SEE, E, W) */
         _windDirection->assign(text);
     }
     std::string&
     Data::WindDirection(){
         return *_windDirection;
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
////////////////////////////////////////////////////////////////////////////////
     void
     Data::Icon(int icon){
         _icon = icon;
     }
     int
     Data::Icon() const{
         /* need to check range (0 - 49) */
         return _icon;
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
    time_t
    Data::GetTimeDistance(time_t need_time){
        if(_end_time > need_time && _start_time < need_time)
            return (_end_time - _start_time);
        else
            return -1;
    }
////////////////////////////////////////////////////////////////////////////////
    Data::~Data(){
       delete _windDirection;
       delete _text;
       delete _temperature_hi;
       delete _temperature_low;
       delete _flike;
       delete _short_day_name;
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
    void
    Data::Pressure(int pressure){
        _pressure = pressure;
    }

    int
    Data::Pressure() const{
        /* need to check type ( ) */
        return _pressure;
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
