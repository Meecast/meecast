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
//#include "downloader.h"
#include <sys/stat.h>

#include <FApp.h>
#include <FContent.h>
#include <FNet.h>

using namespace Tizen::Base;
using namespace Tizen::Content;
using namespace Tizen::App;



////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Station: public Tizen::Net::Http::IHttpTransactionEventListener {
        enum Type_Of_Downloading{
            NONE = 0,
            FORECAST = 1,
            FORECAST_DONE = 2,
            DETAIL_FORECAST = 3,
            DETAIL_FORECAST_DONE = 4,
            HOURS_FORECAST = 5,
            HOURS_FORECAST_DONE = 6,
            TIMEZONE = 7,
            TIMEZONE_DONE = 8
        };

        std::string *_sourceName;
        std::string *_id;
        std::string *_name;
        std::string *_country;
        std::string *_region;
        std::string *_fileName;
        std::string *_forecastURL;
        std::string *_detailURL;
        std::string *_hoursURL;
        std::string *_cookie;
        std::string *_viewURL;
        std::string *_mapURL;
        std::string *_basemapURL;
        std::string *_converter;
        double _latitude;
        double _longitude;
        int _timezone;
        bool _gps;
        bool dataValid();
        bool prepareFile();
        Source *getSourceByName();
        int _downloading;
        public:
            Station(const std::string& source_name, const std::string& id, 
                    const std::string& name,
                    const std::string& country = 0, const std::string& region = 0, 
                    const std::string& forecastURL = 0,
		            const std::string& detailURL = 0,
		            const std::string& hoursURL = 0,
		            const std::string& viewURL = 0, 
		            const std::string& mapURL = 0, 
		            const std::string& basemapURL = 0, 
                    const std::string& cookie = 0,
                    const bool gps = false, double latitude = 0.0, double longitude = 0.0);
            Station(const std::string& source_name, const std::string& id, 
                    const std::string& name,
                    const std::string& country = 0, const std::string& region = 0, 
		            const bool gps = false, double latitude = 0.0, double longitude = 0.0);
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
            void forecastURL(const std::string& ForeastURL);
            std::string& forecastURL() const;
            void cookie(const std::string& Cookie);
            std::string& cookie() const;
            void detailURL(const std::string& region);
            void hoursURL(const std::string& hours);
            std::string& mapURL() const;
            void mapURL(const std::string& MapURL);
            std::string& basemapURL() const;
            void basemapURL(const std::string& MapURL);
            std::string& detailURL() const;
            std::string& hoursURL() const;
            void viewURL(const std::string& ViewURL);
            std::string& viewURL() const;
            void timezone(const int timezone = 0);
            int station_timezone() const;
            bool updateData(bool force);
            bool convertData(bool force);
            void updateSource(const Source* source);
            void fileName(const std::string& fileName);
            std::string& fileName() const;
            void converter(const std::string& converter);
            std::string& converter() const;
            void gps(const bool gps);
            bool gps() const;
            void latitude(const double latitude);
            double latitude() const;
            void longitude(const double longtitude);
            double longitude() const;

            void run_converter();
            bool isupdating();

 // IHttpTransactionEventListener
	virtual void OnTransactionReadyToRead(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	virtual void OnTransactionAborted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, result r);
	virtual void OnTransactionReadyToWrite(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	virtual void OnTransactionHeaderCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool authRequired);
	virtual void OnTransactionCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction);
	virtual void OnTransactionCertVerificationRequiredN(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, Tizen::Base::String* pCert);

           
private:
	result RequestHttpGet(void);

private:
	Tizen::Net::Http::HttpSession* __pHttpSession;


    };
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // STATION_H
