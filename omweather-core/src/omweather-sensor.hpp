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
#ifndef _omweather_sensor_hpp
#define _omweather_sensor_hpp 1
/*******************************************************************************/
#if defined(MAEMO4) || defined(MAEMO5) /* sensor path at Nokia N810 */
#define SENSOR "/sys/devices/platform/i2c_omap.1/i2c-1/1-0048/temp1_input"
#endif
#if defined(APPLICATION) || defined(NONMAEMO) /* sensor path at the PC */
#define SENSOR "/sys/devices/platform/w83627hf.656/temp2_input"
#endif
/*******************************************************************************/
#include <fstream>
/*******************************************************************************/
class Sensor{
    public:
        Sensor();
        float read();
};
/*******************************************************************************/
#endif
