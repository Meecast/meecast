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


#include "datalist.h"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
     Data*
     DataList::GetDataForTime(time_t _time){  
         std::vector<Data*>::const_iterator next_it, it;
         struct tm   *begin_day = NULL;
         struct tm   *end_day = NULL;
         time_t temp_time = -1, result_time;
         time_t begin_day_time, end_day_time;
         Data* temp_data;
         Data* temp_data_result = NULL;
         int hi_day_temp = INT_MAX, low_day_temp = INT_MAX;
         begin_day = localtime(&_time); begin_day->tm_hour = 0; begin_day->tm_min = 0; begin_day->tm_sec = 0;
         begin_day_time = mktime(begin_day); 
         end_day = localtime(&_time); begin_day->tm_hour = 23; begin_day->tm_min = 59; begin_day->tm_sec = 59;
         end_day_time = mktime(end_day); 

         for(it=this->begin(); it!=this->end(); ++it) {
            ++(next_it = it);
            temp_data = *it;
            result_time = temp_data->GetTimeDistance(_time);
            if (temp_time == -1)
                temp_time = result_time;
            /* select min period including _time */
            if ((result_time >0) && result_time <= temp_time){
                temp_time = result_time;
                temp_data_result = temp_data;
            }
         }
         return temp_data_result;
       }

////////////////////////////////////////////////////////////////////////////////
} // namespace Core
