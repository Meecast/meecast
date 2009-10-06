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
Country::Country(int id, const std::string name){
    country_id = id;
    country_name = name;
}
/*******************************************************************************/
Country::Country(int id, const char *name){
    country_id = id;
    country_name = name;
}
/*******************************************************************************/
Country::Country(const Country& src){
    country_id = src.country_id;
    country_name = src.country_name;
}
/*******************************************************************************/
std::string Country::name() const{
    return country_name;
}
/*******************************************************************************/
int Country::id() const{
    return country_id;
}
/*******************************************************************************/
Region::Region(int id, const std::string name, float longitudemax = 0.0f,
                float latitudemax = 0.0f, float longitudemin = 0.0f,
                                float latitudemin = 0.0f) : Country(id, name){
    longitude_max = longitudemax;
    latitude_max = latitudemax;
    longitude_min = longitudemin;
    latitude_min = latitudemin;
}
/*******************************************************************************/
Region::Region(int id, const char *name, float longitudemax = 0.0f,
                float latitudemax = 0.0f, float longitudemin = 0.0f,
                                float latitudemin = 0.0f) : Country(id, name){
    longitude_max = longitudemax;
    latitude_max = latitudemax;
    longitude_min = longitudemin;
    latitude_min = latitudemin;
}
/*******************************************************************************/
float Region::longitudemax() const{
    return longitude_max;
}
/*******************************************************************************/
float Region::latitudemax() const{
    return latitude_max;
}
/*******************************************************************************/
float Region::longitudemin() const{
    return longitude_min;
}
/*******************************************************************************/
float Region::latitudemin() const{
    return latitude_min;
}
/*******************************************************************************/
Station::Station(const std::string name, const std::string code,
                    const std::string source, bool is_gps) : Country(-1, name){
    station_code = code;
    station_source = source;
    is_gps_station = is_gps;
}
/*******************************************************************************/
Station::Station(const Station& src) : Country(-1, src.name()){
    this->station_code = src.station_code;
    this->station_source = src.station_source;
    this->is_gps_station = src.is_gps_station;
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
