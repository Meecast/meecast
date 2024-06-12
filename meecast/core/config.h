/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2013 Vlad Vasilyeu
 * Copyright (C) 2006-2011 Pavel Fialko
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
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


#ifndef CONFIG_H
#define CONFIG_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <QTextStream>
#include "parser.h"
#include "stationlist.h"
typedef std::vector< std::pair<std::string, std::string> > languages;
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Config : public Parser{
            #ifdef LIBXML
            void processNode(const xmlpp::Node* node);
            #endif
            std::string *_filename;
            std::string *_pathPrefix;
            std::string *_iconspath;
            std::string *_iconset;
            std::string *_temperature_unit;
            std::string *_wind_speed_unit;
            std::string *_pressure_unit;
            std::string *_visible_unit;
            std::string *_language;
            std::string *_nullname;
            std::string *_mod;
            bool         _update_connect;
            bool         _fullscreen;
            bool         _lockscreen;
            bool         _standbyscreen;
            bool         _gps;
            bool         _splash;
            int          _update_period;
            std::string *_font_color;
            int          _current_station_id;
            StationsList *_stations;
            languages    *_languages_list;
            int         _Xleft_corner_of_lockscreen_widget;
            int         _Yleft_corner_of_lockscreen_widget;
        protected:
            static Config* _self;
            static int _refcount;
            Config(const std::string& filename, const std::string& schema_filename = prefix + schemaPath + "config.xsd");
            Config();
            Config(const Config& config);
            Config& operator=(const Config& config);
            virtual ~Config();
           
        public:
            static Config* Instance();
            static Config* Instance(const std::string& filename, const std::string& schema_filename);
            static bool DeleteInstance();
            void LoadConfig();
            void ReLoadConfig();
            void InitLanguagesList();
            std::string& prefix_path(void);
            void iconSet(const std::string& text);
            std::string& iconSet(void);
            void current_station_id(int id_station);
            int current_station_id(void);
            void TemperatureUnit(const std::string& text);
            std::string& TemperatureUnit(void);
            void WindSpeedUnit(const std::string& text);
            std::string& WindSpeedUnit(void);
            void PressureUnit(const std::string& text);
            std::string& PressureUnit(void);
            void VisibleUnit(const std::string& text);
            std::string& VisibleUnit(void);
            std::string& Language(void);
            void Language(const std::string& lang);
            std::string& Mod(void);
            void Mod(const std::string& mod);

            void UpdateConnect(const bool uc);
            bool UpdateConnect(void);
            void Fullscreen(const bool uc);
            bool Fullscreen(void);
            void Lockscreen(const bool uc);
            bool Lockscreen(void);
            void Standbyscreen(const bool uc);
            bool Standbyscreen(void);
            void Gps(const bool uc);
            bool Gps(void);
            void Splash(const bool uc);
            bool Splash(void);
            void UpdatePeriod(const int period);
            int UpdatePeriod(void);
            void FontColor(const std::string& text);
            std::string& FontColor(void);
            StationsList& stationsList();
            std::string& stationname();
            std::string& stationname(int i);
            std::string& nextstationname();
            std::string& prevstationname();
            std::string& iconspath();
            //void stationsList(std::vector<Core::Station*> list);
            void stationsList(StationsList list);
            languages& languagesList();
            void Xleft_corner_of_lockscreen_widget(const int X);
            int Xleft_corner_of_lockscreen_widget(void);
            void Yleft_corner_of_lockscreen_widget(const int Y);
            int Yleft_corner_of_lockscreen_widget(void);
            void saveConfig();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // CONFIG_H
