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
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _omweather_stations_hpp
#define _omweather_stations_hpp 1
/*******************************************************************************/
#include "omweather-common.hpp"
/*******************************************************************************/
class Country{
        int country_id;
        std::string country_name;
    public:
        Country(int, const std::string);
        Country(int, const char*);
        Country(const Country&);
        std::string name() const;
        int id() const;
};
/*******************************************************************************/
class Region : public Country{
        float longitude_max, latitude_max, longitude_min, latitude_min;
    public:
        Region(int, const std::string, float, float, float, float);
        Region(int, const char*, float, float, float, float);
        float longitudemax() const;
        float latitudemax() const;
        float longitudemin() const;
        float latitudemin() const;
};
/*******************************************************************************/
class Station : public Country{
    std::string station_code;
    std::string station_source;
    bool        is_gps_station;
    public:
        Station(const std::string name, const std::string code,
                const std::string source, bool is_gps = false);
        Station(const Station&);
        std::string code() const;
        std::string source() const;
        bool is_gps() const;
};
/*******************************************************************************/
typedef std::vector<Country> CountriesList;
typedef std::vector<Region> RegionsList;
typedef std::vector<Station> StationsList;
/*******************************************************************************/
#endif
