/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2010 Vlad Vasiliev
 * Copyright (C) 2006-2010 Pavel Fialko
 * 	for the code
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
#include "weather-common.h"
#include "weather-download.h"
#include "weather-config.h"
#include "weather-home.h"
#include "weather-sources.h"
#include "weather-popup.h"
#include <curl/multi.h>
/*******************************************************************************/
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
int
data_read(void *buffer, size_t size, size_t nmemb, void *stream){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifndef RELEASE
    fprintf(stderr, "\nFILE %p\n", stream);
    fprintf(stderr, "SIZE %i %i\n", size, nmemb);
#endif
    return fwrite(buffer, size, nmemb, (FILE*)stream);
}
/*******************************************************************************/
void*
download_url(void *user_data){
    CURL                *handle = NULL;
    struct curl_slist   *headers = NULL;
    struct download_params *params = (struct download_params*)user_data;
    FILE                *file = NULL;
    int                 max_repeats = 3,
                        repeats = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    handle = curl_easy_init();
    if(handle){
        file = fopen(params->filename, "wb");
        if(!file){
            curl_easy_cleanup(handle);
            return NULL;
        }
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(handle, CURLOPT_FAILONERROR, 1);
        curl_easy_setopt(handle, CURLOPT_USERAGENT,
                            "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1.1) "
                            "Gecko/20061205 Iceweasel/2.0.0.1");
        curl_easy_setopt(handle, CURLOPT_URL, params->url);
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, 60);
        curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 10);
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        headers = curl_slist_append(headers, "Cache-Control: no-cache");
        headers = curl_slist_append(headers, "Pragma: no-cache");
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
        if(params->proxy_host){
            curl_easy_setopt(handle, CURLOPT_PROXY, params->proxy_host);
            if(params->proxy_port > -1)
                curl_easy_setopt(handle, CURLOPT_PROXYPORT, params->proxy_port);
        }
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, data_read);
        /* process downloading */
        while(repeats < max_repeats){
            params->status = curl_easy_perform(handle);
            /* don't repeat in next cases */
#ifdef OS2008
            if(params->status == CURLE_OK || 
#else
            if(params->status == CURLE_OK || params->status == CURLE_REMOTE_ACCESS_DENIED ||
#endif
                    params->status == CURLE_HTTP_RETURNED_ERROR)
                break;
            /* reopen and truncate file */
            fclose(file);
            file = fopen(params->filename, "wb");
            if(!file)
                break;
            /* update file handler */
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, file);
            repeats++;
#ifndef RELEASE
            fprintf(stderr, "\n>>>>>>>>>>>>>>>>>>>>>>>>Repeat N%d download station %s\n", repeats, params->filename);
#endif
            DEBUG_FUNCTION("Repeat download.");
        }
        if(file)
            fclose(file);
        curl_slist_free_all(headers);
        /* always cleanup */
        curl_easy_cleanup(handle);
    }
    return NULL;
}
/*******************************************************************************/
gboolean
download_html(void *user_data){
    char        *url = NULL,
                *hour_url = NULL,
                *filename = NULL,
                *filename_hour = NULL,
                buffer[256],
                buffer1[256];
    static GSList      *list = NULL;
    static GtkWidget    *update_window = NULL;
    static int download_total = 0,
               download_ok = 0,
               download_status = 0,
               i = 0;
    pthread_t               tid;
    struct download_params  params;
    struct download_data    *data = NULL;
    int         error = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* If not connected and it autoupdate do go away */
    if(!app->show_update_window && !app->iap_connected){
        app->phase = ZERO_PHASE;
        return FALSE;
    }

#ifndef RELEASE
    fprintf(stderr, "\n>>>>>>>>>>>>>>>>>>>>>>>>Current phase: %d\n", app->phase);
#endif
    switch(app->phase){
        default:
        case ZERO_PHASE:
            DEBUG_FUNCTION("Zero phase");
            app->phase = ZERO_PHASE;
            if(update_window)
                gtk_widget_destroy(update_window);
            return FALSE;
        break;
        case FIRST_PHASE:
            DEBUG_FUNCTION("First phase");
            update_window = NULL;
            list = NULL;
            data = NULL;
            download_total = download_ok = download_status = 0;
            /* destroy popup window */
            if(app->popup_window)
                destroy_popup_window();
            /* show info for user */
            if(!update_window && app->show_update_window)
                update_window = hildon_banner_show_animation(app->main_window, NULL, _("Updating weather"));
            app->phase = SECOND_PHASE;
            return TRUE;
        break;
        case SECOND_PHASE:
            DEBUG_FUNCTION("Second phase");
            #ifdef DEBUGCONNECT
                app->phase = FOURTH_PHASE;
                return TRUE;
            #endif

            /* check connection status */
            if(app->iap_connected){
                app->phase = FOURTH_PHASE;
                return TRUE;
            }
            if(!app->iap_connecting){
                DEBUG_FUNCTION("Wakeup connection");
#ifndef RELEASE
                fprintf(stderr, "\n>>>>>>>>>>>Wakeup connection...\n");
#endif
                if(!wakeup_connection()){
#ifndef RELESASE
                    fprintf(stderr, "\n>>>>>>>>>>>Connection wakeup failed.\n");
#endif
                    DEBUG_FUNCTION("Wakeup connection failed");
                    app->phase = ZERO_PHASE;
                    if(update_window)
                        gtk_widget_destroy(update_window);
                    return FALSE;   /* connection wakeup error, terminate update process */
                }
            }
            else
                app->iap_connecting_timer = 0;
            app->phase = THIRD_PHASE;
            return TRUE;
        break;
        case THIRD_PHASE:
            DEBUG_FUNCTION("Third phase");
            /* if allready connected go next */
            if(app->iap_connected && !app->iap_connecting){
                app->phase = FOURTH_PHASE;
                return TRUE;
            }
            /* else wait timeout */
            if(app->iap_connecting_timer > 150){
                if(app->show_update_window){
                    if(update_window){
                        gtk_widget_destroy(update_window);
                        update_window = NULL;
                    }
                    hildon_banner_show_information(app->main_window, NULL,
                                               _("Could not connecting to Internet.\nConnection timeout is expired."));
                    app->iap_connecting_timer = 0;
                }
                app->phase = ZERO_PHASE;
                return FALSE;
            }
            else{
                app->iap_connecting_timer++;
                DEBUG_FUNCTION("Wait connection up");
                return TRUE;
            }
        break;
        case FOURTH_PHASE:
            DEBUG_FUNCTION("Fourth phase");
            curl_global_init(CURL_GLOBAL_ALL);
            /* if connection is established start download data */
            memset(&params, 0, sizeof(struct download_params));
            /* preapre proxy data */
            if(app->config->iap_http_proxy_host)
                params.proxy_host = app->config->iap_http_proxy_host;
            else
                params.proxy_host = NULL;
            if(app->config->iap_http_proxy_port)
                params.proxy_port = app->config->iap_http_proxy_port;
            else
                params.proxy_port = -1;
            /* get first url and filename */
            if(!get_station_url(&url, &filename, &hour_url, &filename_hour, TRUE)){
                download_ok = 0;
                download_total = 0;
                download_status = DOWNLOAD_UNCKNOWN_ERROR; /* The strange error */
                app->phase = SIXTH_PHASE;
                return TRUE;
            }
            else{
                params.url = url;
                params.filename = filename;
                params.hour_data = FALSE;
                data = g_new0(struct download_data, 1);
                data->params = params;
                error = pthread_create(&tid, NULL, download_url, (void*)&data->params);
                if(0 != error){
#ifndef RELEASE
                    fprintf(stderr, "\n>>>>>>>>>>>>>>>>>Couldn't run thread, errno %d\n", error);
#endif
                    g_free(data);
                }
                else{
                    data->tid = tid;
                    list = g_slist_append(list, (void*)data);
                    i++;
                    download_total++;
                }
                /* hour data */
                if(app->config->show_weather_for_two_hours){
                    params.url = hour_url;
                    params.filename = filename_hour;
                    params.hour_data = TRUE;
                    data = g_new0(struct download_data, 1);
                    data->params = params;
                    error = pthread_create(&tid, NULL, download_url, (void*)&data->params);
                    if(0 != error){
#ifndef RELEASE
                        fprintf(stderr, "\n>>>>>>>>>>>>>>>>>Couldn't run thread, errno %d\n", error);
#endif
                        g_free(data);
                    }
                    else{
                        DEBUG_FUNCTION("Thread created");
                        data->tid = tid;
                        list = g_slist_append(list, (void*)data);
                    i++;
                    }
                }
            }
            /* get others urls and file names */
            while(get_station_url(&url, &filename, &hour_url, &filename_hour, FALSE)){
                data = g_new0(struct download_data, 1);
                params.url = url;
                params.filename = filename;
                params.hour_data = FALSE;
                data->params = params;
                error = pthread_create(&tid, NULL, download_url, (void*)&data->params);
                if(0 != error){
#ifndef RELEASE
                    fprintf(stderr, "\n>>>>>>>>>>>>>>>>>Couldn't run thread number %d, errno %d\n", i, error);
#endif
                    g_free(data);
                }
                else{
                    DEBUG_FUNCTION("Thread created");
                    data->tid = tid;
                    list = g_slist_append(list, (void*)data);
                    i++;
                    download_total++;
                }
                /* hour data */
                if(app->config->show_weather_for_two_hours){
                    params.url = hour_url;
                    params.filename = filename_hour;
                    params.hour_data = TRUE;
                    data = g_new0(struct download_data, 1);
                    data->params = params;
                    error = pthread_create(&tid, NULL, download_url, (void*)&data->params);
                    if(0 != error){
#ifndef RELEASE
                        fprintf(stderr, "\n>>>>>>>>>>>>>>>>>Couldn't run thread number %d, errno %d\n", i, error);
#endif
                        g_free(data);
                    }
                    else{
                        data->tid = tid;
                        list = g_slist_append(list, (void*)data);
                        i++;
                    }
                }
            }
            app->phase = FIFTH_PHASE;
            i = 0;
            return TRUE;
        break;
        case FIFTH_PHASE:
            DEBUG_FUNCTION("Fifth phase");
            /* now wait for all threads to terminate */
            if(list){
                data = (struct download_data*)list->data;
                tid = data->tid;
                error = pthread_tryjoin_np(tid, NULL);
                if(error == EBUSY){/* thread does not finished */
#ifndef RELEASE
                    fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>>>Thread does not finished\n");
#endif
                    return TRUE;
                }
                if(!error){
#ifndef RELEASE
                    fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>>>Thread %d finished with status %d\n", i, data->params.status);
#endif
                    DEBUG_FUNCTION("Thread finished");
                    if(!data->params.status && !data->params.hour_data)
                        download_ok++;
                    /* store thread status */
                    download_status = data->params.status;
                }
                g_free(data->params.url);
                g_free(data->params.filename);
                g_free(data);
                list = g_slist_next(list);
                i++;
                return TRUE;
            }
            if(list)
                g_slist_free(list);
            /* prepare returning result */
            download_ok = download_ok;
            download_total = download_total;
            app->phase = SIXTH_PHASE;
            return TRUE;
        break;
        case SIXTH_PHASE:
            DEBUG_FUNCTION("Sixth phase");
            if(update_window)
                gtk_widget_destroy(update_window);
            *buffer = 0;
            *buffer1 = 0;
            snprintf(buffer1, sizeof(buffer1) - 1, "%s: %d/%d",
                        _("Successfully loaded"), download_ok, download_total);
            switch(download_status){
                default:
                case DOWNLOAD_UNCKNOWN_ERROR:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                                _("An unknown error has occurred."), buffer1);
                break;
                case DOWNLOAD_OK:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                                _("The weather has been updated."), buffer1);
                break;
                case DOWNLOAD_PROXY_ERROR:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                                _("Couldn't resolve proxy."), buffer1);
                break;
                case DOWNLOAD_RESOLVE_ERROR:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                                _("Couldn't resolve host."), buffer1);
                break;
                case DOWNLOAD_CONNECT_ERROR:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                                _("Failed to connect to host or proxy."), buffer1);
                break;
                case DOWNLOAD_ACCESS_DENIED:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                                _("Access denied to the given URL."), buffer1);
                break;
                case DOWNLOAD_HTTP_ERROR:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                                _("HTTP error."), buffer1);
                break;
                case DOWNLOAD_NOW_WORKING:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                            _("Update already running."), buffer1);
                break;
                case DOWNLOAD_TIMEOUT:
                    snprintf(buffer, sizeof(buffer) - 1, "%s\n%s",
                                _("Download timed out."), buffer1);
                break;
            }
            if(app->show_update_window)
                hildon_banner_show_information(app->main_window, NULL, buffer);
            app->phase = ZERO_PHASE;
            DEBUG_FUNCTION("End update");
            redraw_home_window(FALSE);
            return FALSE;/* termainate process */
        break;
    }
    return TRUE;
}
/*******************************************************************************/
/* Create URL and filename for xml file.
 * Returns TRUE if all right otherwise return FLASE.
*/
gboolean
get_station_url(gchar **url, gchar **filename, gchar **hour_url,
                                        gchar **filename_hour, gboolean first){
    gboolean            valid = FALSE;
    static GtkTreeIter  iter;
    gchar               *station_code = NULL,
                        *new_station_code = NULL,
                        *station_source = NULL,
                        buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(first)
        valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list), &iter);
    else
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list), &iter);
    if(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list), &iter,
                                ID0_COLUMN, &station_code,3, &station_source, -1);
        /* Skip Empty station */
        while(1){
            if(station_code && (!strcmp(station_code," ") || !strcmp(station_code,_("Unknown"))) ){
                if(station_code){
                    g_free(station_code);
                    station_code = NULL;
                }
                if(station_source){
                    g_free(station_source);
                    station_source = NULL;
                }
                valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list), &iter);
                if(valid){
                    gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter,
                                        ID0_COLUMN, &station_code,
                                        3, &station_source, -1);
                }
                else
                    break;
            }
            else
                break;
        }
    }
    if(valid){
        if(station_source){
/* prepare forecast url */
            if(get_source_forecast_url(app->sources_list, station_source)){
                *buffer = 0;
/* TO DO this part of code will move to sources code */
                if(station_source && (!strcmp(station_source, "gismeteo.ru"))){
                    new_station_code = get_new_gismeteo_code(station_code, station_source);
                    snprintf(buffer, sizeof(buffer) - 1,
                                get_source_forecast_url(app->sources_list, station_source),
                                                        new_station_code);
                }else
                    snprintf(buffer, sizeof(buffer) - 1,
                                get_source_forecast_url(app->sources_list, station_source),
                                                        station_code);
                *url = g_strdup(buffer);
            }
/* prepare detail url */
            if(get_source_detail_url(app->sources_list, station_source)){
                *buffer = 0;
/* TO DO move this code to sources libs */
                if(!strcmp(station_source, "gismeteo.ru")){
                    new_station_code = get_new_gismeteo_code(station_code, station_source);
                    snprintf(buffer, sizeof(buffer) - 1,
                                get_source_detail_url(app->sources_list, station_source),
                                                        new_station_code);
                    g_free(new_station_code);
                }
                else
                    snprintf(buffer, sizeof(buffer) - 1,
                                get_source_detail_url(app->sources_list, station_source),
                                                        station_code);
                *hour_url = g_strdup(buffer);
            }
        }
#ifndef RELEASE
/*
        fprintf(stderr, "\n>>>>>>>>>>URL %s", *url);
        fprintf(stderr, "\n>>>>>>>>>>Hour URL %s\n", *hour_url);
*/
#endif
/* preapare filename */
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer) - 1, "%s/%s.xml.new",
                    app->config->cache_dir_name, station_code);
        *filename = g_strdup(buffer);
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer) - 1, "%s/%s_hour.xml.new",
                    app->config->cache_dir_name, station_code);
        *filename_hour = g_strdup(buffer);
#ifndef RELEASE
/*
        fprintf(stderr, "\n>>>>>>>>>NAME %s", *filename);
        fprintf(stderr, "\n>>>>>>>>>Hour NAME %s\n", *filename_hour);
*/
#endif
        g_free(station_code);
        g_free(station_source);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return valid;
}
/*******************************************************************************/
gboolean
wakeup_connection(void){
#ifdef USE_CONIC
    if(app->connection){
        if(!con_ic_connection_connect(app->connection, CON_IC_CONNECT_FLAG_NONE))
            return FALSE;
        else
            return TRUE;
    }
    else
        return FALSE;
#else
#ifndef NONMAEMO
    if(osso_iap_connect(OSSO_IAP_ANY, OSSO_IAP_REQUESTED_CONNECT, NULL) != OSSO_OK)
        return FALSE;
#endif
#endif
    return TRUE;
}
/*******************************************************************************/
