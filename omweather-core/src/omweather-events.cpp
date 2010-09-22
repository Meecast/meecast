/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 *  for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *            az@pocketpcrussia.com 
 *  for default icon set (Glance)
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
#include "omweather-events.hpp"
#include <iostream>
/*******************************************************************************/
Timer::Timer(int number, SIGFUNC handler, long interval){
    struct sigaction sigv;
    error = false;
    this->interval = interval;
    sigemptyset(&sigv.sa_mask);
    sigv.sa_flags = SA_SIGINFO;
    sigv.sa_sigaction = handler;
    if(sigaction(SIGINT, &sigv, 0) == -1)
        error = true;
    else{
        struct sigevent sigx;
        sigx.sigev_notify = SIGEV_SIGNAL;
        sigx.sigev_signo = SIGINT;
        sigx.sigev_value.sival_int = number;
        if(timer_create(CLOCK_REALTIME, &sigx, &id) == -1)
            error = true;
    }
}
/*******************************************************************************/
Timer::~Timer(){
    if(!error){
        stop();
        if(timer_delete(id) == -1)
            std::cerr << "Error deleting timer.";
    }
}
/*******************************************************************************/
bool Timer::run(){
    if(error)
        return false;
    error = false;
    long t = (long)(interval / 1000);
    long m = (long)(interval % 1000);
    value.it_value.tv_sec = t;
    value.it_value.tv_nsec = m;
    value.it_interval.tv_sec = t;
    value.it_interval.tv_nsec = m;
    if(timer_settime(id, 0, &value, 0) == -1)
        error = true;
    return error;
}
/*******************************************************************************/
bool Timer::stop(){
    if(error)
        return false;
    error = false;
    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;
    if(timer_settime(id, 0, &value, 0) == -1)
        error = true;
    return error;
}
/*******************************************************************************/
bool Timer::operator!(){
    return error ? true : false;
}
/*******************************************************************************/
