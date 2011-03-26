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


#include "temperature.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Temperature::~Temperature(){
        delete _units;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature::Temperature(const float value, const std::string& units){
        _units = new std::string;
        _units->assign(units);
        if(*_units == "F")
             _value = (value - 32.0f ) / 1.8f;
        else
            _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature::Temperature(const Temperature& temperature){
        _units = new std::string;
        _units->assign(*(temperature._units));
        _value = temperature._value;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature& Temperature::operator=(const Temperature& temperature){
        if(this != &temperature){
            delete _units;
            _units = new std::string;
            _units->assign(*(temperature._units));
            _value = temperature._value;
        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
    void Temperature::units(const std::string& units){
        _units->assign(units);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Temperature::units() const{
        return *_units;
    }
////////////////////////////////////////////////////////////////////////////////
    void Temperature::value(const float value){
        _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    float Temperature::value(const bool absolute){
        if (absolute)
            return _value;
        if(*_units == "F")
            return (floor((_value * 9 / 5) + 32));
        return _value;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
