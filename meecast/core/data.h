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


#ifndef DATA_H
#define DATA_H
////////////////////////////////////////////////////////////////////////////////
#include <time.h>
#include <vector>
#include <string>
#include "temperature.h"
#include "windspeed.h"

////////////////////////////////////////////////////////////////////////////////
namespace Core{
    /* Weather data for interval from _start_time to _end_time */
    class Data {
        time_t _start_time;
        time_t _end_time;
        time_t _sunrise;
        time_t _sunset;
        Temperature *_flike;
        //float _windSpeed;
        Windspeed *_windSpeed;
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
        Temperature *_temperature;
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
            void  SunRiseTime(time_t sunrise_time);
            time_t SunRiseTime(void) const;
            void  SunSetTime(time_t sunset_time);
            time_t SunSetTime(void) const;
            time_t DayLength(void) const;
            //void  WindSpeed(float windspeed);
            //float WindSpeed(void) const;
            Windspeed& WindSpeed();
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
            Temperature& temperature();
            Temperature& Flike();
            void  Pressure(float pressure);
            int Pressure(void) const;
            void  Ppcp(float ppcp);
            float Ppcp(void) const;
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATA_H
