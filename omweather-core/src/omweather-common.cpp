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
#include "omweather-common.hpp"
/*******************************************************************************/
void Color::set(const char *str){
    std::string c = str;
    r = (unsigned)(strtol(c.substr(1, 2).c_str(), NULL, 16) << 8);
    g = (unsigned)(strtol(c.substr(3, 2).c_str(), NULL, 16) << 8);
    b = (unsigned)(strtol(c.substr(5, 2).c_str(), NULL, 16) << 8);
}
/*******************************************************************************/
const char* Color::get(){
    std::string c;
    char buffer[128];
    *buffer = 0;
    c.clear();
    snprintf(buffer, sizeof(buffer) - 1, "#%02X%02X%02X", r >> 8, g >> 8, b >> 8);
    c = buffer;
    return c.c_str();
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
int Set::toInt() const{
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
SettingsMode::SettingsMode(){
    string_list.insert( std::pair<std::string,int>("SIMPLE", SIMPLE) );
    string_list.insert( std::pair<std::string,int>("EXTENDED", EXTENDED) );
    int_list.insert( std::pair<int,std::string>(SIMPLE, "SIMPLE") );
    int_list.insert( std::pair<int,std::string>(EXTENDED, "EXTENDED") );
    /* default value */
    current_number = string_list["SIMPLE"];
    current_name = int_list[SIMPLE];
}
/*******************************************************************************/
TextPosition::TextPosition(){
    string_list.insert( std::pair<std::string,int>("LEFT", LEFT) );
    string_list.insert( std::pair<std::string,int>("RIGHT", RIGHT) );
    string_list.insert( std::pair<std::string,int>("TOP", TOP) );
    string_list.insert( std::pair<std::string,int>("BOTTOM", BOTTOM) );
    int_list.insert( std::pair<int,std::string>(LEFT, "LEFT") );
    int_list.insert( std::pair<int,std::string>(RIGHT, "RIGHT") );
    int_list.insert( std::pair<int,std::string>(TOP, "TOP") );
    int_list.insert( std::pair<int,std::string>(BOTTOM, "BOTTOM") );
    /* default value */
    current_number = string_list["RIGHT"];
    current_name = int_list[RIGHT];
}
/*******************************************************************************/
WindUnits::WindUnits(){
    string_list.insert( std::pair<std::string,int>("MpS", MpS) );
    string_list.insert( std::pair<std::string,int>("KMpH", KMpH) );
    string_list.insert( std::pair<std::string,int>("MIpH", MIpH) );
    int_list.insert( std::pair<int,std::string>(MpS, "MpS") );
    int_list.insert( std::pair<int,std::string>(KMpH, "KMpH") );
    int_list.insert( std::pair<int,std::string>(MIpH, "MIpH") );
    /* default value */
    current_number = string_list["MpS"];
    current_name = int_list[MpS];
}
/*******************************************************************************/
PressureUnits::PressureUnits(){
    string_list.insert( std::pair<std::string,int>("MB", MB) );
    string_list.insert( std::pair<std::string,int>("INCH", INCH) );
    string_list.insert( std::pair<std::string,int>("MM", MM) );
    int_list.insert( std::pair<int,std::string>(MB, "MB") );
    int_list.insert( std::pair<int,std::string>(INCH, "INCH") );
    int_list.insert( std::pair<int,std::string>(MM, "MM") );
    /* default value */
    current_number = string_list["MM"];
    current_name = int_list[MM];
}
/*******************************************************************************/
DistanceUnits::DistanceUnits(){
    string_list.insert( std::pair<std::string,int>("M", M) );
    string_list.insert( std::pair<std::string,int>("KM", KM) );
    string_list.insert( std::pair<std::string,int>("MI", MI) );
    string_list.insert( std::pair<std::string,int>("sMI", sMI) );
    int_list.insert( std::pair<int,std::string>(M, "M") );
    int_list.insert( std::pair<int,std::string>(KM, "KM") );
    int_list.insert( std::pair<int,std::string>(MI, "MI") );
    int_list.insert( std::pair<int,std::string>(sMI, "sMI") );
    /* default value */
    current_number = string_list["KM"];
    current_name = int_list[KM];
}
/*******************************************************************************/
TemperatureUnits::TemperatureUnits(){
    string_list.insert( std::pair<std::string,int>("CELCIUS", CELCIUS) );
    string_list.insert( std::pair<std::string,int>("F", FAHRENHEIT) );
    int_list.insert( std::pair<int,std::string>(CELCIUS, "CELCIUS") );
    int_list.insert( std::pair<int,std::string>(FAHRENHEIT, "FAHRENHEIT") );
    /* default value */
    current_number = string_list["CELCIUS"];
    current_name = int_list[CELCIUS];
}
/*******************************************************************************/
ClickType::ClickType(){
    string_list.insert( std::pair<std::string,int>("SHORT", SHORT) );
    string_list.insert( std::pair<std::string,int>("LONG", LONG) );
    int_list.insert( std::pair<int,std::string>(SHORT, "SHORT") );
    int_list.insert( std::pair<int,std::string>(LONG, "LONG") );
    /* default value */
    current_number = string_list["SHORT"];
    current_name = int_list[SHORT];
}
/*******************************************************************************/
SensorPosition::SensorPosition(){
    string_list.insert( std::pair<std::string,int>("ICON", ICON) );
    string_list.insert( std::pair<std::string,int>("STATION", STATION) );
    int_list.insert( std::pair<int,std::string>(ICON, "ICON") );
    int_list.insert( std::pair<int,std::string>(STATION, "STATION") );
    /* default value */
    current_number = string_list["ICON"];
    current_name = int_list[ICON];
}
/*******************************************************************************/
