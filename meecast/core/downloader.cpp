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
int downloading_count = 0;

#ifdef TIZEN
EAPI Ecore_Event_Handler *complete_handler = NULL; 
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
    ecore_main_loop_iterate();
    return fwrite(ptr, size, nmemb, stream);
}


#ifdef TIZEN
static void
exe_complete(void *data, const Ecore_Exe *exe){

    downloading_count--;
    if (downloading_count <= 0){
        ecore_event_handler_del (complete_handler);
        ecore_con_url_shutdown();
        ecore_con_shutdown();
    }
}

static Eina_Bool
_url_complete_cb(void *data, int type, void *event_info)
{  
    Ecore_Exe *exe; 
    Ecore_Con_Event_Url_Complete *url_complete = (Ecore_Con_Event_Url_Complete* )event_info;
    std::string *command = (std::string *)ecore_con_url_data_get(url_complete->url_con);
/* Debug
    int nbytes = ecore_con_url_received_bytes_get(url_complete->url_con);

    printf("\n");
    printf("download completed with status code: %d\n", url_complete->status);
    printf("Total size of downloaded file: %ld bytes "
                                      "(from received_bytes_get)\n", nbytes);

    printf("Command %s\n", command->c_str());
*/
    if (strcmp(command->c_str(),"")){
        exe =  ecore_exe_run(command->c_str(), NULL);
        ecore_exe_callback_pre_free_set(exe, exe_complete);   
    }else{
        downloading_count--;
        if (downloading_count <= 0){
            ecore_event_handler_del (complete_handler);
            ecore_con_url_shutdown();
            ecore_con_shutdown();
        }
    }

    delete command;

    return EINA_TRUE;
}
bool
Downloader::downloadData(const std::string &filename, const std::string &url, 
                         const std::string &cookie, const std::string &converter_command)
{
    Ecore_Con_Url *ec_url = NULL;
    int fd;

    fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1){
        std::cerr << "error open file " << filename << std::endl;
        return false;
    }
    if (downloading_count == 0){
        ecore_con_init();
        ecore_con_url_init();
        complete_handler = ecore_event_handler_add(ECORE_CON_EVENT_URL_COMPLETE, _url_complete_cb, NULL);
    }

    ec_url = ecore_con_url_new(url.c_str());

    if (!ec_url)
        return false; 

    ecore_con_url_fd_set(ec_url, fd);

    curl_easy_setopt(ec_url->curl_easy, CURLOPT_COOKIE, cookie.c_str()); 
    std::string *command_string = new std::string();
    command_string->assign(converter_command);
    ecore_con_url_data_set(ec_url, command_string);


    if (!ecore_con_url_get(ec_url)){
        printf("could not realize request.\n");
        return false;
    }
    downloading_count ++;
    return true;
#endif

#ifndef TIZEN
bool
Downloader::downloadData(const std::string &filename, const std::string &url, 
                         const std::string &cookie, const std::string &converter_command)
{

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
