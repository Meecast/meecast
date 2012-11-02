/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2012 Vlad Vasilyeu
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


#include "visible.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Visible::~Visible(){
        delete _units;
    }
////////////////////////////////////////////////////////////////////////////////
    Visible::Visible(const float value, const std::string& units){
        _units = new std::string;
        _units->assign(units);
        if(*_units == "km")
             _value = value * 1000;
        else if (*_units == "mi")
                 _value = _value * 1609.344f;
             else
                 _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    Visible::Visible(const Visible& visible){
        _units = new std::string;
        _units->assign(*(visible._units));
        _value = visible._value;
    }
////////////////////////////////////////////////////////////////////////////////
    Visible& Visible::operator=(const Visible& visible){
        if(this != &visible){
            delete _units;
            _units = new std::string;
            _units->assign(*(visible._units));
            _value = visible._value;
        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
    void Visible::units(const std::string& units){
        _units->assign(units);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Visible::units() const{
        return *_units;
    }
////////////////////////////////////////////////////////////////////////////////
    void Visible::value(const float value){
        _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    float Visible::value(const bool absolute){
        if (absolute)
            return _value;
        if(*_units == "km")
             return(_value / 1000);
        else if (*_units == "mi")
                 return round(_value / 1.609344f);
             else
                 return (_value);
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

