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


#include "sourcelist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    SourceList::SourceList(const std::string path){
        Dirent *dp = 0;
        DIR *dir_fd = opendir(path.c_str());
        if(dir_fd){
            while((dp = readdir(dir_fd))){
                std::string name = dp->d_name;
                if(name == "." || name == "..")
                    continue;
                if(dp->d_type == DT_REG || dp->d_type == DT_LNK){
                    try{
                        std::string file = path + dp->d_name;
                        std::string schemafile = AbstractConfig::prefix;
                        schemafile += AbstractConfig::schemaPath;
                        schemafile += "source.xsd";
                        Source *s = new Source(file, schemafile);
                        this->push_back(s);
                    }
                    catch(std::string& err){
                        std::cerr << "error " << err << std::endl;
                        continue;
                    }
                    catch(const char *err){
                        std::cerr << "error " << err << std::endl;
                        continue;
                    }
                }
            }
            closedir(dir_fd);
        }
        _stations = new StationsList;
    }
////////////////////////////////////////////////////////////////////////////////
    SourceList::~SourceList(){
        for(unsigned i = 0; i < this->size(); i++){
            Source *s = this->at(i);
            if(s)
                delete s;
        }
        this->clear();
        _stations->clear();
        delete _stations;
    }
////////////////////////////////////////////////////////////////////////////////
    StationsList& SourceList::searchStation(const std::string& station){
        _stations->clear();
        for(unsigned i = 0; i < this->size(); i++){
            StationsList st = this->at(i)->search(station);
            for(unsigned j = 0; j < st.size(); j++)
                _stations->push_back(st.at(j));
        }
        return *_stations;
    }
////////////////////////////////////////////////////////////////////////////////
    Source* SourceList::source_by_name(const std::string& source_name){
        for(unsigned i = 0; i < this->size(); i++){
            if(this->at(i)->name() == source_name)
                return this->at(i);
        }
        return NULL;
    }
////////////////////////////////////////////////////////////////////////////////
    int SourceList::source_id_by_name(const std::string& source_name){
        for(unsigned i = 0; i < this->size(); i++){
            if(this->at(i)->name() == source_name)
                return i;
        }
        return -1;
    }

} // namespace Core
