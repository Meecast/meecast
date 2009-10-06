/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include "omweather-sensor.hpp"
#include <stdlib.h>
/*******************************************************************************/
Sensor::Sensor(const std::string sensor_path){
    path = sensor_path;
}
/*******************************************************************************/
float Sensor::read(){
    char buffer[128];
    float value = 0.0f;
    std::ifstream file(path.c_str());
    if(file.is_open()){
        *buffer = 0;
        file.getline(buffer, sizeof(buffer) - 1);
        file.close();
        value = atof(buffer) / 1000.0f;
    }
    return value;
}
/*******************************************************************************/
