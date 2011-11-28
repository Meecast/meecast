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


#include "pressure.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Pressure::~Pressure(){
        delete _units;
    }
////////////////////////////////////////////////////////////////////////////////
    Pressure::Pressure(const float value, const std::string& units){
        _units = new std::string;
        _units->assign(units);
        if(*_units == "Pa")
             _value = value * 10;
        else if (*_units == "mmHg")
                 _value = _value / 0.75006f;
             else
                 _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    Pressure::Pressure(const Pressure& pressure){
        _units = new std::string;
        _units->assign(*(pressure._units));
        _value = pressure._value;
    }
////////////////////////////////////////////////////////////////////////////////
    Pressure& Pressure::operator=(const Pressure& pressure){
        if(this != &pressure){
            delete _units;
            _units = new std::string;
            _units->assign(*(pressure._units));
            _value = pressure._value;
        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
    void Pressure::units(const std::string& units){
        _units->assign(units);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Pressure::units() const{
        return *_units;
    }
////////////////////////////////////////////////////////////////////////////////
    void Pressure::value(const float value){
        _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    float Pressure::value(const bool absolute){
        if (absolute)
            return _value;
        if(*_units == "Pa")
             return(_value / 10);
        else if (*_units == "mmHg")
                 return round(_value * 0.75006f);
             else
                 return (_value);
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

