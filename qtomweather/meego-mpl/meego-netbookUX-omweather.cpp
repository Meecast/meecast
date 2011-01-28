/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2006-2011 Pavel Fialko
 * Copyright (C) 2010-2011 Tanya Makova
 * 	for the code
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

#include "meego-netbookUX-omweather.h"

#ifdef LOCALDEBUG
    #define CONFIG_PATH "config.xml"
    #define CONFIG_XSD_PATH "../core/data/config.xsd"
#else
    #define CONFIG_PATH "~/.config/omweather/config.xml"
    #define CONFIG_XSD_PATH "/usr/share/omweather/xsd/config.xsd"
#endif
Core::Config *
create_and_fill_config(){
    Core::Config *config;
    std::cerr<<"Create ConfigQML class: " <<std::endl;
    try{
        config = new Core::Config(CONFIG_PATH, CONFIG_XSD_PATH);
    }
    catch(const std::string &str){
        std::cerr<<"Error in ConfigQML class: "<< str <<std::endl;
        config = new Core::Config();
    }
    catch(const char *str){
        std::cerr<<"Error in ConfigQML class: "<< str <<std::endl;
        config = new Core::Config();
    }
    std::cerr<<"End of creating ConfigQML class: " <<std::endl;
    config->saveConfig("newconfig.xml");
    return config;
}
