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
        if(*_units == "km/h")
             _value = value / 3.6f;
        else if (*_units == "mi/h")
                 _value = _value / 3.6f * 1.609344f;
             else
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
    float Windspeed::value() const{
        if(*_units == "km/h")
             return(_value * 3.6f);
        else if (*_units == "mi/h")
                 return round(_value * 3.6f / 1.609344f);
             else
                 return (_value);
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

