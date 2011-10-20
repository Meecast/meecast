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


#ifndef DATABASEABSTRACT_H
#define DATABASEABSTRACT_H

#include <vector>
#include <string>

namespace Core{

typedef std::vector< std::pair<std::string, std::string> > listdata;

class DatabaseAbstract
{

public:
    virtual void set_databasename(const std::string& filename) = 0;
    virtual bool open_database() = 0;
    virtual listdata* create_countries_list() = 0;
    virtual listdata* create_region_list(int country_id) = 0;
    virtual listdata* create_stations_list(int region_id) = 0;
};
} // namespace Core
#endif // DATABASEABSTRACT_H
