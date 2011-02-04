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
#include "stationlist.h"
#include "abstractconfig.h"
#include "databasesqlite.h"
#include "databaseabstract.h"
#include "downloader.h"

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
    class DatabaseSqlite;
    class DatabaseAbstract;
    class Downloader;
};
////////////////////////////////////////////////////////////////////////////////
#endif // CORE_H
