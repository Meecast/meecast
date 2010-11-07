#ifndef DATA_H
#define DATA_H
////////////////////////////////////////////////////////////////////////////////
#include <time.h>
#include <vector>
#include <string>
#include "temperature.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    /* Weather data for interval from _start_time to _end_time */
    class Data {
        time_t _start_time;
        time_t _end_time;
        float _flike;
        float _windSpeed;
        std::string *_windDirection;
        float _windGust;
        int _humidity;
        int _pressure;
        std::string *_text;
        int _icon;
        Temperature *_temperature_hi;
        Temperature *_temperature_low;
        public:
            Data();
            Data(const Data& data);
            Data& operator=(const Data& data);
            void  StartTime(time_t start_time);
            void  EndTime(time_t end_time);
            void  Flike(float flike);
            float Flike(void) const;
            void  WindSpeed(float windspeed);
            float WindSpeed(void) const;
            void  WindDirection(const std::string& text);
            std::string& WindDirection(void);
            void  WindGust(float windgust);
            float WindGust(void) const;
            void  Humidity(int humidity);
            int   Humidity(void) const;
            void  Icon(int icon);
            int   Icon(void) const;
            void  Text(const std::string& text);
            std::string& Text(void);
            time_t GetTimeDistance(time_t need_time); /* Checking for  occurrence of this time (need_time) to Data interval and return the interval if  this time are include */
            virtual ~Data();
            Temperature& temperature_hi();
            Temperature& temperature_low();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATA_H
