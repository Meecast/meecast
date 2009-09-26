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
#include "omweather-common.hpp"
/*******************************************************************************/
void Color::set(const char *str){
    std::string c = str;
    r = atoi(c.substr(1, 2).c_str());
    g = atoi(c.substr(3, 2).c_str());
    b = atoi(c.substr(5, 2).c_str());
}
/*******************************************************************************/
Set::Set(){
    string_list.clear();
    int_list.clear();
    current_name.clear();
    current_number = -1;
}
/*******************************************************************************/
void Set::current(int n){
    std::map<int,std::string>::iterator it = int_list.find(n);
    if(it != int_list.end()){
        current_number = it->first;
        current_name = it->second;
    }
}
/*******************************************************************************/
void Set::current(const std::string n){
    std::map<std::string,int>::iterator it = string_list.find(n);
    if(it != string_list.end()){
        current_number = it->second;
        current_name = it->first;
    }
}
/*******************************************************************************/
void Set::current(const char* n){
    std::string str = n;
    std::map<std::string,int>::iterator it = string_list.find(str);
    if(it != string_list.end()){
        current_number = it->second;
        current_name = it->first;
    }
}
/*******************************************************************************/
int Set::toInt(){
    return current_number;
}
/*******************************************************************************/
std::string Set::toString(){
    return current_name;
}
/*******************************************************************************/
IconsPreset::IconsPreset(){
    string_list.insert( std::pair<std::string,int>("ONE_ROW", ONE_ROW) );
    string_list.insert( std::pair<std::string,int>("ONE_COLUMN", ONE_COLUMN) );
    string_list.insert( std::pair<std::string,int>("TWO_ROWS", TWO_ROWS) );
    string_list.insert( std::pair<std::string,int>("TWO_COLUMNS", TWO_COLUMNS) );
    string_list.insert( std::pair<std::string,int>("COMBINATION", COMBINATION) );
    string_list.insert( std::pair<std::string,int>("NOW", NOW) );
    string_list.insert( std::pair<std::string,int>("NOW_PLUS_TWO", NOW_PLUS_TWO) );
    string_list.insert( std::pair<std::string,int>("NOW_PLUS_THREE_V", NOW_PLUS_THREE_V) );
    string_list.insert( std::pair<std::string,int>("NOW_PLUS_THREE_H", NOW_PLUS_THREE_H) );
    string_list.insert( std::pair<std::string,int>("NOW_PLUS_SEVEN", NOW_PLUS_SEVEN) );
    string_list.insert( std::pair<std::string,int>("APPLICATION_MODE", APPLICATION_MODE) );
    int_list.insert( std::pair<int,std::string>(ONE_ROW, "ONE_ROW") );
    int_list.insert( std::pair<int,std::string>(ONE_COLUMN, "ONE_COLUMN") );
    int_list.insert( std::pair<int,std::string>(TWO_ROWS, "TWO_ROWS") );
    int_list.insert( std::pair<int,std::string>(TWO_COLUMNS, "TWO_COLUMNS") );
    int_list.insert( std::pair<int,std::string>(COMBINATION, "COMBINATION") );
    int_list.insert( std::pair<int,std::string>(NOW, "NOW") );
    int_list.insert( std::pair<int,std::string>(NOW_PLUS_TWO, "NOW_PLUS_TWO") );
    int_list.insert( std::pair<int,std::string>(NOW_PLUS_THREE_V, "NOW_PLUS_THREE_V") );
    int_list.insert( std::pair<int,std::string>(NOW_PLUS_THREE_H, "NOW_PLUS_THREE_H") );
    int_list.insert( std::pair<int,std::string>(NOW_PLUS_SEVEN, "NOW_PLUS_SEVEN") );
    int_list.insert( std::pair<int,std::string>(APPLICATION_MODE, "APPLICATION_MODE") );
    /* default value */
    current_number = string_list["NOW"];
    current_name = int_list[NOW];
}
/*******************************************************************************/
IconsSize::IconsSize(){
    string_list.insert( std::pair<std::string,int>("TINY", TINY) );
    string_list.insert( std::pair<std::string,int>("SMALL", SMALL) );
    string_list.insert( std::pair<std::string,int>("MEDIUM", MEDIUM) );
    string_list.insert( std::pair<std::string,int>("BIG", BIG) );
    string_list.insert( std::pair<std::string,int>("LARGE", LARGE) );
    string_list.insert( std::pair<std::string,int>("GIANT", GIANT) );
    string_list.insert( std::pair<std::string,int>("SUPER_GIANT", SUPER_GIANT) );
    int_list.insert( std::pair<int,std::string>(TINY, "TINY") );
    int_list.insert( std::pair<int,std::string>(SMALL, "SMALL") );
    int_list.insert( std::pair<int,std::string>(MEDIUM, "MEDIUM") );
    int_list.insert( std::pair<int,std::string>(BIG, "BIG") );
    int_list.insert( std::pair<int,std::string>(LARGE, "LARGE") );
    int_list.insert( std::pair<int,std::string>(GIANT, "GIANT") );
    int_list.insert( std::pair<int,std::string>(SUPER_GIANT, "SUPER_GIANT") );
    /* default value */
    current_number = string_list["LARGE"];
    current_name = int_list[LARGE];
}
/*******************************************************************************/
ViewMode::ViewMode(){
    string_list.insert( std::pair<std::string,int>("COLLAPSED", COLLAPSED) );
    string_list.insert( std::pair<std::string,int>("EXPANDED", EXPANDED) );
    int_list.insert( std::pair<int,std::string>(COLLAPSED, "COLLAPSED") );
    int_list.insert( std::pair<int,std::string>(EXPANDED, "EXPANDED") );
    /* default value */
    current_number = string_list["COLLAPSED"];
    current_name = int_list[COLLAPSED];
}
/*******************************************************************************/
/*******************************************************************************/