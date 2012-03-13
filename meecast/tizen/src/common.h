/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen 
 *
 * Copyright (C) 2012 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _main_h
#define _main_h 1
#endif
#include "core.h"
/*******************************************************************************/
struct _App {
    Ecore_Evas *ee;
    Evas_Object *top_main_window;
    Evas_Object *setting_top_main_window;
    Evas_Object *setting_menu;
    Evas_Object *setting_top_main_window2;
    Evas_Object *setting_menu2;
    Evas_Object *day_list_main_window;
    Evas_Object *menu;
    Core::DataParser* dp;
    Core::Config *config;
    Core::StationsList *stationslist;
};
/*******************************************************************************/
