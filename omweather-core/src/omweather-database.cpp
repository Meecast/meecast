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
#include "omweather-database.hpp"
#include <fstream>
/*******************************************************************************/
#define DATABASEPATH        "/usr/share/omweather/db/"
/*******************************************************************************/
Database::Database(){
    db = NULL;
}
/*******************************************************************************/
Database::~Database(){
    close();
}
/*******************************************************************************/
bool Database::open(const std::string name){
    if(name.empty())
        return false;
    std::string full_name = DATABASEPATH + name;
    std::fstream file(full_name.c_str());
    if(!file.is_open())
        return false;
    if(sqlite3_open_v2(full_name.c_str(), &db, SQLITE_OPEN_READONLY, NULL))
        return false;
}
/*******************************************************************************/
void Database::close(){
    if(db)
        sqlite3_close(db);
}
/*******************************************************************************/
