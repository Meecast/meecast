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
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////

struct _request
{
    long size;
};


Downloader::Downloader()
{

}

size_t
Downloader::writedata(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    ecore_main_loop_iterate();
    return fwrite(ptr, size, nmemb, stream);
}


static Eina_Bool
_url_progress_cb(void *data, int type, void *event_info)
{
       Ecore_Con_Event_Url_Progress *url_progress = (Ecore_Con_Event_Url_Progress* )event_info;
          float percent;

        if (url_progress->down.total > 0) {
                struct _request *req = (Core::_request* )ecore_con_url_data_get(url_progress->url_con);
                req->size = url_progress->down.now;

                percent = (url_progress->down.now / url_progress->down.total) * 100;
                printf("Total of download complete: %0.1f (%0.0f)%%\n",
                percent, url_progress->down.now);
        }
    return EINA_TRUE;
}

static Eina_Bool
_url_complete_cb(void *data, int type, void *event_info)
{
       Ecore_Con_Event_Url_Complete *url_complete = (Ecore_Con_Event_Url_Complete* )event_info;

        struct _request *req = (Core::_request* )ecore_con_url_data_get(url_complete->url_con);
        int nbytes = ecore_con_url_received_bytes_get(url_complete->url_con);

         printf("\n");
         printf("download completed with status code: %d\n", url_complete->status);
         printf("Total size of downloaded file: %ld bytes\n", req->size);
         printf("Total size of downloaded file: %ld bytes "
                                      "(from received_bytes_get)\n", nbytes);

         ecore_con_url_shutdown();
         ecore_con_shutdown();
         return EINA_TRUE;
}

bool
Downloader::downloadData(const std::string &filename, const std::string &url, const std::string &cookie)
{
    Ecore_Con_Url *ec_url = NULL;
    int fd;
    struct _request *req;
   
    fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1){
        std::cerr << "error open file " << filename << std::endl;
        return false;
    }
 

    ecore_con_init();
    ecore_con_url_init();
    ec_url = ecore_con_url_new(url.c_str());


    if (!ec_url){
        return false; 
    }

    req = (Core::_request* )malloc(sizeof(*req));
    req->size = 0;
    ecore_con_url_data_set(ec_url, req);
    ecore_con_url_fd_set(ec_url, fd);
    ecore_event_handler_add(ECORE_CON_EVENT_URL_PROGRESS, _url_progress_cb, NULL);
    ecore_event_handler_add(ECORE_CON_EVENT_URL_COMPLETE, _url_complete_cb, NULL);
    curl_easy_setopt(ec_url->curl_easy, CURLOPT_COOKIE, cookie.c_str()); 
    if (!ecore_con_url_get(ec_url)){
        printf("could not realize request.\n");
        return false;
    }
    return true;
   #if 0
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
        res = curl_easy_perform(curl);
        std::cerr << "curl result = " << res << std::endl;
        curl_easy_cleanup(curl);
        fclose(fp);
        return true;
    }else return false;
#endif
}
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
