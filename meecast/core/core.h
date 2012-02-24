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

#ifndef CORE_H
#define CORE_H
////////////////////////////////////////////////////////////////////////////////
#include "connection.h"
#include "config.h"
#include "station.h"
#include "source.h"
#include "data.h"
#include "parser.h"
#include "source.h"
#include "sourcelist.h"
#include "datalist.h"
#include "dataparser.h"
#include "temperature.h"
#include "pressure.h"
#include "windspeed.h"
#include "stationlist.h"
#include "abstractconfig.h"
#include "databasesqlite.h"
#include "databaseabstract.h"
#include "downloader.h"

#define MEECAST_VERSION "0.5.2"
namespace Core{
    class Connection;
    class AbstractConfig;
    class Config;
    class Data;
    class Source;
    class SourceList;
    class Station;
    class StationList;
    class DataParser;
    class Temperature;
    class Pressure;
    class Windspeed;
    class DatabaseSqlite;
    class DatabaseAbstract;
    class Downloader;
};
////////////////////////////////////////////////////////////////////////////////
#endif // CORE_H
