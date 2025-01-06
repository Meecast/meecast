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


#include "downloader.h"
#ifndef LIBCURL
    #include <QtCore>
    #include <QEventLoop>
    #include <QtNetwork>
    #include <QObject>
#endif
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
Downloader::Downloader()
{

}
size_t
Downloader::writedata(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}
bool
Downloader::downloadData(const std::string &filename, const std::string &url, const std::string &cookie, const std::string &user_agent)
{

#ifdef LIBCURL
    CURL *curl;
    CURLcode res;
    FILE *fp;

    curl = curl_easy_init();
    if (curl){
        fp = fopen(filename.c_str(), "w");
        if (!fp){
            std::cerr << "error open file " << filename << std::endl;
            return false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Downloader::writedata);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str()); 
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true); 
        if (user_agent != ""){
            curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
        }
        res = curl_easy_perform(curl);
        std::cerr << "curl result = " << res << std::endl;
        curl_easy_cleanup(curl);
        fclose(fp);
        return true;
    }else return false;
#else
    //qDebug()<<"----"<<QSslSocket::sslLibraryBuildVersionString();
    //qDebug()<<"----"<<QSslSocket::sslLibraryVersionString();
    QEventLoop loop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QUrl _url(url.c_str());
    qDebug()<<url.c_str();
    QNetworkRequest request(_url);
    if (user_agent != ""){
        qDebug()<<"Set User-Agent";
        request.setRawHeader("User-Agent", user_agent.c_str());
    }
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    auto reply = mgr.get(request);
    loop.exec();
    QFile localFile(filename.c_str());
    if (!localFile.open(QIODevice::WriteOnly))
        return false;
    localFile.write(reply->readAll());
    localFile.close();
    delete reply;
    return true;
#endif
}
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
