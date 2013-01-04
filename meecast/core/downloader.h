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


#ifndef DOWNLOADER_H
#define DOWNLOADER_H

//#include "curl/curl.h"
#include <stdio.h>
#include <string>
#include <iostream>

#ifdef TIZEN
#include <Ecore.h>
#include <Ecore_Con.h>
#include <fcntl.h>
#include "config.h"

typedef unsigned int              Ecore_Magic;
struct _Ecore_Con_Url
{
   Ecore_Magic  __magic;
   CURL *curl_easy;
   struct curl_slist *headers;
   Eina_List *additional_headers;
   Eina_List *response_headers;
   char *url;

   Ecore_Con_Url_Time time_condition;
   double timestamp;
   void *data;

   Ecore_Fd_Handler *fd_handler;
   int fd;
   int flags;

   int received;
   int write_fd;

   Eina_Bool active : 1;
};
#endif

namespace Core{
class Downloader
{
public:
    Downloader();
    static size_t writedata(void *ptr, size_t size, size_t nmemb, FILE *stream);
    static bool downloadData(const std::string &filename, const std::string &url, 
    const std::string &cookie, const std::string &converter_command);
};
}
#endif // DOWNLOADER_H
