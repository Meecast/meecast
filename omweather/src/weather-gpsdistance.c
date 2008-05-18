/*
 * Copyright (C) 2006, 2007 John Costigan.
 *
 * POI and GPS-Info code originally written by Cezary Jackiewicz.
 * Adapting for OMWeather by Vlad Vasiliev
 *
 * Default map data provided by http://www.openstreetmap.org/
 *
 * This file is part of Maemo Mapper.
 *
 * Maemo Mapper is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Maemo Mapper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Maemo Mapper.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Calculate the distance between two lat/lon pairs.  The distance is returned
 * in nautical miles and should be converted using UNITS_CONVERT[_units].
 */
/*******************************************************************************/
#include "weather-gpsdistance.h"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#ifdef HILDON

#define PI   (3.14159265358979323846)
#define EARTH_RADIUS (3443.91847)
#define deg2rad(deg) ((deg) * (PI / 180.0)) 
/*******************************************************************************/ 
gdouble
calculate_distance(gdouble lat1, gdouble lon1, gdouble lat2, gdouble lon2)
{
    gdouble dlat, dlon, slat, slon, a;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* Convert to radians. */
    lat1 = deg2rad(lat1);
    lon1 = deg2rad(lon1);
    lat2 = deg2rad(lat2);
    lon2 = deg2rad(lon2);

    dlat = lat2 - lat1;
    dlon = lon2 - lon1;

    slat = sin(dlat / 2.0F);
    slon = sin(dlon / 2.0F);
    a = (slat * slat) + (cos(lat1) * cos(lat2) * slon * slon);
    return ((2.0F * atan2(sqrt(a), sqrt(1.0F - a))) * EARTH_RADIUS);
}
/*******************************************************************************/
#endif
