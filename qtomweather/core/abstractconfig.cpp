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

#include "abstractconfig.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
#ifdef LOCALDEBUG
    std::string AbstractConfig::prefix = "";
    std::string AbstractConfig::sharePath = "../core/data/";
    std::string AbstractConfig::schemaPath = "../test/schemas/";
    std::string AbstractConfig::iconsPath = "../core/data/icons/";
    std::string AbstractConfig::libPath = "lib/omweather/";
    std::string AbstractConfig::sourcesPath = "../test/sources/";
    std::string AbstractConfig::layoutqml = "weatherlayoutitem.qml";
    //std::string AbstractConfig::configPath = "../test/";
#else
/*
    std::string AbstractConfig::prefix = "/usr/";
    std::string AbstractConfig::sharePath = "share/omweather/";
    std::string AbstractConfig::schemaPath = "share/omweather/xsd/";
    std::string AbstractConfig::iconsPath = "share/omweather/icons/";
    std::string AbstractConfig::libPath = "lib/omweather/";
    std::string AbstractConfig::sourcesPath = "share/omweather/sources/";
    std::string AbstractConfig::layoutqml = "/usr/share/omweather/qml/weatherlayoutitem.qml";
    //std::string AbstractConfig::configPath = "~/.config/omweather/";
    */
    std::string AbstractConfig::prefix = "/opt/com.meecast.omweather";
    std::string AbstractConfig::sharePath = "/share/";
    std::string AbstractConfig::schemaPath = "/share/xsd/";
    std::string AbstractConfig::iconsPath = "/share/icons/";
    std::string AbstractConfig::libPath = "/lib/";
    std::string AbstractConfig::sourcesPath = "/share/sources/";
    std::string AbstractConfig::layoutqml = "/qml/weatherlayoutitem.qml";
    //std::string AbstractConfig::configPath = "~/.config/omweather/";

#endif
    std::string AbstractConfig::getConfigPath()
    {
#ifdef LOCALDEBUG
        return "../test/";
#else
        char filepath[4096];
        struct passwd *pw = getpwuid(getuid());
        std::string path(pw->pw_dir);
        path += "/.config";
        /* TODO check directory exist */
        mkdir(path.c_str(), 0755);
        path += "/com.meecast.omweather/";
        mkdir(path.c_str(), 0755);
        return path;
#endif
    }
} // namespace Core
