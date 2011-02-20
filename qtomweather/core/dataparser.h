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


#ifndef DATAPARSER_H
#define DATAPARSER_H
////////////////////////////////////////////////////////////////////////////////
#include "parser.h"
#include "data.h"
#include "datalist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class DataParser : public Parser {
        DataList *_list;
        int _timezone;
        #ifdef LIBXML
        void processNode(const xmlpp::Node* node);
        #endif
        public:
            DataParser(const std::string& filename, const std::string& schema_filename = "/usr/share/omweather/schemas/data.xsd");
            DataParser();
            virtual ~DataParser();
            DataList& data();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATAPARSER_H
