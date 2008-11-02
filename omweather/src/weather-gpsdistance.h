/* vim: set sw=4 ts=4 et: */
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
#ifndef _weather_gpsdistance_h
#define _weather_gpsdistance_h 1
/*******************************************************************************/
#include "weather-common.h"
#include <math.h>
/*******************************************************************************/
#ifdef OS2008
gdouble
calculate_distance(gdouble lat1, gdouble lon1, gdouble lat2, gdouble lon2);
#endif
/*******************************************************************************/
#endif
