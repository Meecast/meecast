/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2012 Vlad Vasilyeu
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


#include "windspeed.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Windspeed::~Windspeed(){
        delete _units;
    }
////////////////////////////////////////////////////////////////////////////////
    Windspeed::Windspeed(const float value, const std::string& units){
        _units = new std::string;
        _units->assign(units);
        if(*_units == "km/h"){
             _value = value / 3.6f;
             return;
        }else if (*_units == "mi/h"){
                 _value = _value / 3.6f * 1.609344f;
                 return;
              }else if (*_units == "kn"){
                        _value = _value * 1.944;
                        return;
                    }else if (*_units == "Beaufort scale"){
                        _value = _value / 3.6f;
                        if(_value < 1){
                            _value = 0.0f;
                            return;
                        }
                        if(_value >= 1.1f && _value <= 5.5f){
                            _value = 1.0f;
                            return;  
                        }
                        if(_value >= 5.6f && _value <= 11.0f){
                            _value = 2.0f;
                            return;
                        }
                        if(_value >= 12.0f && _value <= 19.0f){
                            _value = 3.0f;
                            return;
                        }
                        if(_value >= 20.0f && _value <= 28.0f){
                            _value = 4.0f;
                            return;
                        }
                        if(_value >= 29.0f && _value <= 38.0f){
                            _value = 5.0f;
                            return;
                        }
                        if(_value >= 39.0f && _value <= 49.0f){
                            _value = 6.0f;
                            return;
                        }
                        if(_value >= 50.0f && _value <= 61.0f){
                            _value = 7.0f;
                            return;
                        }
                        if(_value >= 62.0f && _value <= 74.0f){
                            _value = 8.0f;
                            return;
                        }
                        if(_value >= 75.0f && _value <= 88.0f){
                            _value = 9.0f;
                            return;
                        }
                        if(_value >= 89.0f && _value <= 102.0f){
                            _value = 10.0f;
                            return;
                        }
                        if(_value >= 103.0f && _value <= 117.0f){
                            _value = 11.0f;
                            return;
                        }
                        if(_value >= 118.0f){
                            _value = 12.0f;
                            return;
                        }
                    }else
                       _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    Windspeed::Windspeed(const Windspeed& windspeed){
        _units = new std::string;
        _units->assign(*(windspeed._units));
        _value = windspeed._value;
    }
////////////////////////////////////////////////////////////////////////////////
    Windspeed& Windspeed::operator=(const Windspeed& windspeed){
        if(this != &windspeed){
            delete _units;
            _units = new std::string;
            _units->assign(*(windspeed._units));
            _value = windspeed._value;
        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
    void Windspeed::units(const std::string& units){
        _units->assign(units);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Windspeed::units() const{
        return *_units;
    }
////////////////////////////////////////////////////////////////////////////////
    void Windspeed::value(const float value){
        _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    float Windspeed::value(const bool absolute){
        if (absolute)
            return _value;
        if(*_units == "km/h")
             return(_value * 3.6f);
        else if (*_units == "mi/h")
                 return round(_value * 3.6f / 1.609344f);
             else if (*_units == "kn"){
                        return round(_value * 1.944);
                    }else if (*_units == "Beaufort scale"){
                        float value = _value / 3.6f;
                        if(value < 1){
                            value = 0.0f;
                            return value;
                        }
                        if(value >= 1.1f && value <= 5.5f){
                            value = 1.0f;
                            return value;  
                        }
                        if(value >= 5.6f && value <= 11.0f){
                            value = 2.0f;
                            return value;
                        }
                        if(value >= 12.0f && value <= 19.0f){
                            value = 3.0f;
                            return value;
                        }
                        if(value >= 20.0f && value <= 28.0f){
                            value = 4.0f;
                            return value;
                        }
                        if(value >= 29.0f && value <= 38.0f){
                            value = 5.0f;
                            return value;
                        }
                        if(value >= 39.0f && value <= 49.0f){
                            value = 6.0f;
                            return value;
                        }
                        if(value >= 50.0f && value <= 61.0f){
                            value = 7.0f;
                            return value;
                        }
                        if(value >= 62.0f && value <= 74.0f){
                            value = 8.0f;
                            return value;
                        }
                        if(value >= 75.0f && value <= 88.0f){
                            value = 9.0f;
                            return value;
                        }
                        if(value >= 89.0f && value <= 102.0f){
                            value = 10.0f;
                            return value;
                        }
                        if(value >= 103.0f && value <= 117.0f){
                            value = 11.0f;
                            return value;
                        }
                        if(value >= 118.0f){
                            value = 12.0f;
                            return value;
                        }
                    }else
                        return (_value);
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

