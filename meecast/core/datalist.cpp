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
     DataList::GetDataForTime(time_t _time, bool is_hour){  
         std::vector<Data*>::const_iterator next_it, it;
         struct tm   *begin_day = NULL;
         struct tm   *end_day = NULL;
         time_t temp_time = -1, result_time;
         time_t begin_day_time, end_day_time;
         Data* temp_data;
         Data* temp_data_result = NULL;
         float hi_day_temp = INT_MAX, low_day_temp = INT_MAX;
         begin_day = localtime(&_time); begin_day->tm_hour = 0; begin_day->tm_min = 0; begin_day->tm_sec = 0;
         begin_day_time = mktime(begin_day); 
         end_day = localtime(&_time); begin_day->tm_hour = 23; begin_day->tm_min = 59; begin_day->tm_sec = 59;
         end_day_time = mktime(end_day); 

         for(it=this->begin(); it!=this->end(); ++it) {
            ++(next_it = it);
            temp_data = *it;
           // std::cerr<<"Is_hour "<<is_hour<<"Hour "<< temp_data->Hour()<<std::endl;
            if (is_hour && !temp_data->Hour())
                continue;
	 //   else
	 //	if (is_hour)
         //   		std::cerr<<"yyyyyyyyyyyyyyyy "<<temp_data->StartTime() <<" "<<temp_data->EndTime() <<std::endl;
            if (temp_data->StartTime() >= begin_day_time && temp_data->StartTime() < end_day_time){  
                if (temp_data->temperature_hi().value(true) != INT_MAX){
                    if (hi_day_temp != INT_MAX){
                       if (temp_data->temperature_hi().value(true) > hi_day_temp)
                           hi_day_temp = temp_data->temperature_hi().value(true);
                    }else
                        hi_day_temp = temp_data->temperature_hi().value(true);
                }
                if (temp_data->temperature_low().value(true) != INT_MAX){
                    if (low_day_temp != INT_MAX){
                       if (temp_data->temperature_low().value(true) < low_day_temp)
                           low_day_temp = temp_data->temperature_low().value(true);
                    }else
                        low_day_temp = temp_data->temperature_low().value(true);
                }

                if (temp_data->temperature().value(true) != INT_MAX){ 
                   if (hi_day_temp == INT_MAX)
                       hi_day_temp = temp_data->temperature().value(true);
                   if (temp_data->temperature().value(true) > hi_day_temp)
                       hi_day_temp = temp_data->temperature().value(true);
                }

                if (temp_data->temperature().value(true) != INT_MAX){  
                    if (low_day_temp == INT_MAX)
                        low_day_temp = temp_data->temperature().value(true); 
                    if (temp_data->temperature().value(true) < low_day_temp){

                        low_day_temp = temp_data->temperature().value(true); 
                    }
                }
            }
            result_time = temp_data->GetTimeDistance(_time);
            if (temp_time == -1)
                temp_time = result_time;
	    // if (is_hour)
            //     std::cerr<<"111111111111 "<<_time<<" "<<temp_time<<" "<< result_time<<std::endl;
            /* select min period including _time */
            if ((result_time > 0) && result_time <= temp_time){
                temp_time = result_time; 
                temp_data_result = temp_data;
	     //if (is_hour)
             //   std::cerr<<"success"<<std::endl;
            }
         }
         if (temp_data_result){
             if (hi_day_temp != INT_MAX)
                temp_data_result->temperature_hi().value(hi_day_temp);
             if (low_day_temp != INT_MAX)
                temp_data_result->temperature_low().value(low_day_temp);
         }
         return temp_data_result;
       }
////////////////////////////////////////////////////////////////////////////////
       time_t 
       DataList::GetSunRiseForTime(time_t _time){
         std::vector<Data*>::const_iterator next_it, it;
         struct tm   *begin_day = NULL;
         struct tm   *end_day = NULL;
         time_t result_time = 0;
         time_t begin_day_time, end_day_time;
         Data* temp_data;

         begin_day = localtime(&_time); begin_day->tm_hour = 0; begin_day->tm_min = 0; begin_day->tm_sec = 0;
         begin_day_time = mktime(begin_day); 
         end_day = localtime(&_time); begin_day->tm_hour = 23; begin_day->tm_min = 59; begin_day->tm_sec = 59;
         end_day_time = mktime(end_day); 

         for(it=this->begin(); it!=this->end(); ++it) {
            ++(next_it = it);
            temp_data = *it;
            if (temp_data->StartTime() <= _time && temp_data->EndTime() > _time){
                if (temp_data->SunRiseTime() > 0){
                    result_time = temp_data->SunRiseTime();
                }
            }
         }
         return result_time;
       }
////////////////////////////////////////////////////////////////////////////////
       time_t 
       DataList::GetSunSetForTime(time_t _time){
         std::vector<Data*>::const_iterator next_it, it;
         struct tm   *begin_day = NULL;
         struct tm   *end_day = NULL;
         time_t result_time = 0;
         time_t begin_day_time, end_day_time;
         Data* temp_data;

         begin_day = localtime(&_time); begin_day->tm_hour = 0; begin_day->tm_min = 0; begin_day->tm_sec = 0;
         begin_day_time = mktime(begin_day); 
         end_day = localtime(&_time); begin_day->tm_hour = 23; begin_day->tm_min = 59; begin_day->tm_sec = 59;
         end_day_time = mktime(end_day); 

         for(it=this->begin(); it!=this->end(); ++it) {
            ++(next_it = it);
            temp_data = *it;
            if (temp_data->StartTime() >= begin_day_time && temp_data->StartTime() < end_day_time){
                if (temp_data->SunSetTime()>0)
                    result_time = temp_data->SunSetTime();
            }
         }
         return result_time;
       }

////////////////////////////////////////////////////////////////////////////////
} // namespace Core
