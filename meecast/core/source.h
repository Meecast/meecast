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


#ifndef SOURCE_H
#define SOURCE_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <dlfcn.h>
class StationsList;
#include "stationlist.h"
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Source : public Parser {
        #ifdef LIBXML
        void processNode(const xmlpp::Node* node);
        #endif
        std::string *_name;
        std::string *_logo;
        std::string *_library;
        std::string *_binary;
        std::string *_url_template;
        std::string *_url_detail_template;
        std::string *_url_for_view;
        void *_libraryHandler;
        bool _hasForecast;
        bool _hasDetail;
        bool _hasSearch;
        // source init function
        void (*_sourceInit)();
        // source destroy function
        void (*_sourceDestroy)();
        // source search station function
        StationsList& (*_sourceSearch)(const std::string& station);
        // source get forecast url function
        bool (*_sourceGetForecast)(const std::string& id, const std::string& filename);
        // source get detail url function
        bool (*_sourceGetDetail)(const std::string& id, const std::string& filename);
        public:
            Source(const std::string& filename,
                   const std::string& schema_filename = prefix + schemaPath + "source.xsd");
            Source& operator=(const Source& source);
            std::string& name() const;
            std::string& logo() const;
            std::string& url_template() const;
            std::string& url_detail_template() const;
            std::string& url_for_view() const;
            std::string& binary() const;
            virtual ~Source();
            StationsList& search(const std::string& station);
    };
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // SOURCE_H
