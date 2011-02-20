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


#include "connection.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Connection::Connection(long timeout){
        _proxyHost = new std::string;
        _proxyPort = -1;
        _hasProxy = false;
        _connected = false;
        _timeout = timeout;
    }
////////////////////////////////////////////////////////////////////////////////
    Connection::~Connection(){
        delete _proxyHost;
    }
////////////////////////////////////////////////////////////////////////////////
    void Connection::wakeup(){
    }
////////////////////////////////////////////////////////////////////////////////
    bool Connection::connected() const{
        return _connected;
    }
////////////////////////////////////////////////////////////////////////////////
    void Connection::close(){
    }
////////////////////////////////////////////////////////////////////////////////
    bool Connection::hasProxy() const{
        return _hasProxy;
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Connection::proxyHost() const{
        return *_proxyHost;
    }
////////////////////////////////////////////////////////////////////////////////
    int Connection::proxyPort() const{
        return _proxyPort;
    }
////////////////////////////////////////////////////////////////////////////////
    long Connection::timeout() const{
        return _timeout;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
