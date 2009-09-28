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
#include "omweather-stations.hpp"
/*******************************************************************************/
Station::Station(){
    station_name.clear();
    station_code.clear();
    station_source.clear();
    is_gps_station = false;
}
/*******************************************************************************/
Station::Station(const std::string name, const std::string code,
                                        const std::string source, bool is_gps){
    station_name = name;
    station_code = code;
    station_source = source;
    is_gps_station = is_gps;
}
/*******************************************************************************/
Station::Station(const Station& src){
    this->station_name = src.station_name;
    this->station_code = src.station_code;
    this->station_source = src.station_source;
    this->is_gps_station = src.is_gps_station;
}
/*******************************************************************************/
std::string Station::name() const{
    return station_name;
}
/*******************************************************************************/
std::string Station::code() const{
    return station_code;
}
/*******************************************************************************/
std::string Station::source() const{
    return station_source;
}
/*******************************************************************************/
bool Station::is_gps() const{
    return is_gps_station;
}
/*******************************************************************************/
StationsList::StationsList(){
    list.clear();
}
/*******************************************************************************/
StationsList::StationsList(const StationsList& src){
    for(int i = 0; i < src.list.size(); i++)
        this->list.push_back(src.list[i]);
}
/*******************************************************************************/
void StationsList::add(const Station s){
    list.push_back(s);
}
/*******************************************************************************/
