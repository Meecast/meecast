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
#ifndef _omweather_sources_hpp
#define _omweather_sources_hpp 1
/*******************************************************************************/
#include <string>
#include <vector>

#include "omweather-common.hpp"
/*******************************************************************************/
#define SOURCESPATH "/usr/share/omweather/sources/"
#define SOURCES_LIB "/usr/lib/omweather/"
#define DATABASEPATH "/usr/share/omweather/db/"
/*******************************************************************************/
class Source{
    std::string         source_name;
    std::string         source_logo;
    std::string         source_forecast_url;
    std::string         source_detail_url;
    std::string         source_search_url;
    std::string         source_database_name;
    std::string         source_library_name;
    void*               source_handle;
    void*               source_parser;
    void parse_children(xmlNode *node);
    bool load_library();
    bool check_database();
    bool                source_is_invalid;
    public:
        Source(const std::string);
        ~Source();
        bool is_valid() const;
};
/*******************************************************************************/
class SourcesList{
    std::vector<Source> list;
    public:
        SourcesList();
        int size() const;
};
/*******************************************************************************/
#endif
