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


#ifndef STATION_H
#define STATION_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
class Source;
#include "source.h"
#include "data.h"
#include "sourcelist.h"
#include "downloader.h"

////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Station{
        std::string *_sourceName;
        std::string *_id;
        std::string *_name;
        std::string *_country;
        std::string *_region;
        std::string *_fileName;
        std::string *_forecastURL;
        std::string *_converter;
        int _timezone;
        Data *_data; /* Is it really needed ? Vlad */
        bool dataValid();
        bool prepareFile();
        Source *getSourceByName();
        public:
        Station(const std::string& source_name, const std::string& id, const std::string& name,
                    const std::string& country = 0, const std::string& region = 0, 
                    const std::string& forecastURL = 0);
            Station(const Station& station);
            Station& operator=(const Station& station);
            Source *_source;
            virtual ~Station();
            void name(const std::string& name);
            std::string& name() const;
            void id(const std::string& id);
            std::string& id() const;
            void sourceName(const std::string& sourceName);
            std::string& sourceName() const;
            void country(const std::string& country);
            std::string& country() const;
            void region(const std::string& region);
            std::string& region() const;
            void forecastURL(const std::string& region);
            std::string& forecastURL() const;
            void timezone(const int timezone = 0);
            int timezone() const;
            bool updateData(bool force);
            void updateSource(const Source* source);
            void fileName(const std::string& fileName);
            std::string& fileName() const;
            void converter(const std::string& converter);
            std::string& converter() const;


    };
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // STATION_H
