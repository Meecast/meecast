/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen
 *
 * Copyright (C) 2012 - 2013 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
/*******************************************************************************/
#include "configtizen.h"

ConfigTizen* ConfigTizen::_self;
int ConfigTizen::_refcount;

ConfigTizen::ConfigTizen(const std::string& filename, const std::string& schema_filename):Core::Config(filename, schema_filename)
{
}

ConfigTizen::ConfigTizen():Core::Config()
{
}

ConfigTizen::~ConfigTizen()
{
}


ConfigTizen* 
ConfigTizen::Instance()
{
    if (!_self)
        _self = new ConfigTizen();
    _refcount++;
    return _self;
}

ConfigTizen* 
ConfigTizen::Instance(const std::string& filename, const std::string& schema_filename)
{
    if (!_self)
        _self = new ConfigTizen(filename, schema_filename);
    _refcount++;
    return _self;
}

void 
ConfigTizen::set_screen_width(int width){
    _screen_width = width;
}

int  
ConfigTizen::get_screen_width(){
    return _screen_width;
}

void 
ConfigTizen::set_screen_height(int height){
    _screen_height = height;
}

int  
ConfigTizen::get_screen_height(){
    return _screen_height;
}


void 
saveStation1(String city_id, String city_name, String region,
                      String country, String source, int source_id, bool gps, double latitude, double longitude)
{
}

