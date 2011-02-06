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
        Temperature *_flike;
        float _windSpeed;
        std::string *_windDirection;
        float _windGust;
        int _humidity;
        int _pressure;
        std::string *_text;
        std::string *_short_day_name;
        std::string *_full_day_name;
        std::string *_full_month_name;
        std::string *_day_of_month_name;
        int _icon;
        Temperature *_temperature_hi;
        Temperature *_temperature_low;
        bool _current;
        float _ppcp;
        public:
            Data();
            Data(const Data& data);
            Data(const Data *data);
            Data& operator=(const Data& data);
            void  StartTime(time_t start_time);
            time_t StartTime(void) const;
            void  EndTime(time_t end_time);
            time_t EndTime(void) const;
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
            void  Current(bool current);
            bool  Current(void) const;
            void  Text(const std::string& text);
            std::string& Text(void);
            std::string& ShortDayName(void);
            std::string& FullDayName(void);
            std::string& FullMonthName(void);
            std::string& DayOfMonthName(void);
            time_t GetTimeDistance(time_t need_time); /* Checking for  occurrence of this time (need_time) to Data interval and return the interval if  this time are include */
            virtual ~Data();
            Temperature& temperature_hi();
            Temperature& temperature_low();
            Temperature& Flike();
            void  Pressure(int pressure);
            int Pressure(void) const;
            void  Ppcp(float ppcp);
            float Ppcp(void) const;
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATA_H
