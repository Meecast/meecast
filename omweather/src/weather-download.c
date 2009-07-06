/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
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
#include "weather-download.h"
/*******************************************************************************/
#ifdef USE_CONIC
#include <conic/conic.h>
#define USER_DATA_MAGIC 0xaadcaadc
#endif
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
static gchar *url = NULL;
static gchar *hour_url = NULL;
static gboolean second_attempt = FALSE;
static CURL *curl_handle = NULL;
static CURL *curl_handle_hour = NULL;
static CURL *curl_multi = NULL;
static struct curl_slist *headers=NULL;
static struct HtmlFile html_file, html_file_hour;
static GtkWidget *update_window = NULL;
/*******************************************************************************/
/* Create standard Hildon animation small window */
GtkWidget *create_window_update(void) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    return hildon_banner_show_animation(app->main_window,
                                        NULL, _("Update weather"));
}
/*******************************************************************************/
#if !defined OS2008 && !defined OS2009
static DBusHandlerResult
get_connection_status_signal_cb(DBusConnection * connection,
                                DBusMessage * message, void *user_data) {

    gchar *iap_name = NULL, *iap_nw_type = NULL, *iap_state = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* check signal */
    if (!dbus_message_is_signal(message,
                                ICD_DBUS_INTERFACE,
                                ICD_STATUS_CHANGED_SIG)) {
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    if (!dbus_message_get_args(message, NULL,
                               DBUS_TYPE_STRING, &iap_name,
                               DBUS_TYPE_STRING, &iap_nw_type,
                               DBUS_TYPE_STRING, &iap_state,
                               DBUS_TYPE_INVALID)) {
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

/*    fprintf(stderr,"OMWeather  - iap_state = %s\n", iap_state);*/
    if (!strcmp(iap_state, "CONNECTED")) {
        if (!app->iap_connected) {
            app->iap_connected = TRUE;
            app->iap_connecting = FALSE;
            app->iap_connecting_timer = 0;
            if (app->config->downloading_after_connecting)
                add_current_time_event();
        }
    } else if (!strcmp(iap_state, "CONNECTING")) {
        app->iap_connected = FALSE;
        app->iap_connecting = TRUE;
        app->iap_connecting_timer = 0;
    } else if (app->iap_connected) {
        app->iap_connected = FALSE;     /* !!!!!!!!! Need Remove download */
        app->iap_connecting = FALSE;
        app->iap_connecting_timer = 0;
    } else {
        app->iap_connecting = FALSE;
    }

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}
#endif
/*******************************************************************************/
/* Callback function for request  connection to Internet */
#ifdef USE_CONIC
#define OSSO_CON_IC_CONNECTING             0x05
void
connection_cb(ConIcConnection * connection,
              ConIcConnectionEvent * event, gpointer user_data) {
    const gchar *iap_id, *bearer;
    ConIcConnectionStatus status;
    ConIcConnectionError error;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    status = con_ic_connection_event_get_status(event);
    error = con_ic_connection_event_get_error(event);
    iap_id = con_ic_event_get_iap_id(CON_IC_EVENT(event));
    bearer = con_ic_event_get_bearer_type(CON_IC_EVENT(event));

    switch (status) {
    case CON_IC_STATUS_CONNECTED:
#ifdef DEBUGFUNCTIONCALL
        second_attempt = TRUE;
        update_weather(FALSE);
#endif
        app->iap_connecting = FALSE;
        app->iap_connected = TRUE;
        app->iap_connecting_timer = 0;
        if (app->config->downloading_after_connecting)
            add_current_time_event();
        break;

    case CON_IC_STATUS_DISCONNECTED:
        app->iap_connected = FALSE;
        app->iap_connecting = FALSE;
        app->iap_connecting_timer = 0;
        break;
    case CON_IC_STATUS_DISCONNECTING:
        app->iap_connected = FALSE;
        app->iap_connecting = FALSE;
        app->iap_connecting_timer = 0;
        break;
/*        default:
    	    app->iap_connected = FALSE;
	    app->iap_connecting = FALSE;
            break;
*/
    }
}
#else
/*******************************************************************************/
#ifndef NONMAEMO
void iap_callback(struct iap_event_t *event, void *arg) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->iap_connecting = FALSE;
    switch (event->type) {
    case OSSO_IAP_CONNECTED:
#if defined (DEBUGFUNCTIONCALL) || defined (NONMAEMO)
        second_attempt = TRUE;
        update_weather(FALSE);
#endif
        app->iap_connected = TRUE;
        break;
    case OSSO_IAP_DISCONNECTED:
        app->iap_connected = FALSE;
        break;
    case OSSO_IAP_ERROR:
        app->iap_connected = FALSE;
        hildon_banner_show_information(app->main_window,
                                       NULL,
                                       _("Not connected to Internet"));

        break;
    }
}
#endif
#endif
/* Init easy curl */
CURL *weather_curl_init(CURL * my_curl_handle) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    my_curl_handle = curl_easy_init();
    
    curl_easy_setopt(my_curl_handle, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(my_curl_handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(my_curl_handle, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(my_curl_handle, CURLOPT_USERAGENT,
                     "Mozilla/5.0 (X11; U; Linux i686; en-US; "
                     "rv:1.8.1.1) Gecko/20061205 Iceweasel/2.0.0.1");
    if (app->config->show_weather_for_two_hours)
       curl_easy_setopt(my_curl_handle, CURLOPT_TIMEOUT, 60);
    else
       curl_easy_setopt(my_curl_handle, CURLOPT_TIMEOUT, 30);
    curl_easy_setopt(my_curl_handle, CURLOPT_CONNECTTIMEOUT, 10);
    /* For disabled cache */
    headers = curl_slist_append(headers, "Cache-Control: no-cache");
    headers = curl_slist_append(headers, "Pragma: no-cache");
    curl_easy_setopt(my_curl_handle, CURLOPT_HTTPHEADER, headers);

    config_update_proxy();
    /* Set Proxy option */
    if (app->config->iap_http_proxy_host) {
        curl_easy_setopt(my_curl_handle, CURLOPT_PROXY,
                         app->config->iap_http_proxy_host);
        if (app->config->iap_http_proxy_port)
            curl_easy_setopt(my_curl_handle, CURLOPT_PROXYPORT,
                             app->config->iap_http_proxy_port);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return my_curl_handle;
}
/*******************************************************************************/
void
free_curl(void)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    curl_slist_free_all(headers);
    headers = NULL;
    curl_multi = NULL;
    curl_handle_hour = NULL;
    curl_handle = NULL;
    app->flag_updating = 0;
}
/*******************************************************************************/
static int data_read(void *buffer, size_t size, size_t nmemb, void *stream) {
    int result;
    struct HtmlFile *out = (struct HtmlFile *)stream;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (out && !out->stream) {
        /* open file for writing */
        out->stream = fopen(out->filename, "wb");
        if (!out->stream)
            return -1;          /* failure, can't open file to write */
    }
#ifndef RELEASE
    fprintf(stderr, "SIZE %i %i\n", size, nmemb);
#endif
    result = fwrite(buffer, size, nmemb, out->stream);
    return result;
}
/*******************************************************************************/
gboolean download_html(gpointer data) {
    CURLMsg *msg;
    CURLMcode mret;
    fd_set rs, ws, es;
    int max;
    gint num_transfers = 0, num_msgs = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (app->popup_window && app->show_update_window) {
        destroy_popup_window();
    }
    /* If not connected and it autoupdate do go away */
    if (!app->show_update_window && !app->iap_connected) {
        app->flag_updating = 0;
        return FALSE;
    }
    if (app->iap_connected)
        second_attempt = TRUE;

/* Connection wake up */
    if (app->show_update_window && (!second_attempt)
        && (!app->iap_connecting)) {
        app->iap_connecting = TRUE;
#ifdef USE_CONIC
        if (app->connection)
            con_ic_connection_connect(app->connection,
                                      CON_IC_CONNECT_FLAG_NONE);
        else{
            app->flag_updating = 0;
            return FALSE;
        }
#else
    #ifndef NONMAEMO
        if (osso_iap_connect
            (OSSO_IAP_ANY, OSSO_IAP_REQUESTED_CONNECT, NULL) != OSSO_OK) {
            fprintf(stderr,
                    "after 1 osso_iap_connect(OSSO_IAP_ANY, OSSO_IAP_REQUESTED_CONNECT, NULL) != OSSO_OK)\n");
        }
     #endif

#endif
        app->flag_updating = 0;
        second_attempt = TRUE;
        return TRUE;
    }
    if (app->iap_connecting) {
        /* Check timeout */
        if (app->iap_connecting_timer > 150) {
            if (app->show_update_window) {
                if (update_window) {
                    gtk_widget_destroy(update_window);
                    update_window = NULL;
                }
                hildon_banner_show_information(app->main_window,
                                               NULL,
                                               _
                                               ("Not connected to Internet\nConnection time is expired"));
                app->iap_connecting_timer = 0;
            }
            app->iap_connecting = FALSE;
            app->flag_updating = 0;
            return FALSE;
        } else {
            app->iap_connecting_timer++;
            return TRUE;
        }
    }

    second_attempt = FALSE;
    /* The second stage */
    /* call curl_multi_perform for read weather data from Inet */
    if (curl_multi && CURLM_CALL_MULTI_PERFORM ==
        curl_multi_perform(curl_multi, &num_transfers)){
        return TRUE;            /* return to UI */
        }
    /* The first stage */
    if (!curl_handle && !curl_handle_hour) {
#ifndef RELEASE
        fprintf(stderr, "\n>>>>>>>>>>>First stage\n");
#endif
//        DEBUG_FUNCTION("\n>>>>>>>>>>>First stage\n");
        if (app->show_update_window)
            update_window = create_window_update();     /* Window with update information */
        /* get first station */
        if (!get_station_url
            (&url, &html_file, &hour_url, &html_file_hour, TRUE)) {
            app->flag_updating = 0;
            return FALSE;       /* The strange error */
        }
#ifndef RELEASE
        fprintf(stderr, "\n>>>>>Url - %s, File - %s\n", url,
                html_file.filename);
#endif
        /* Init easy_curl */
        curl_handle = weather_curl_init(curl_handle);
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        /* Init curl_mult */
        if (!curl_multi)
            curl_multi = curl_multi_init();

        max = 0;
        FD_ZERO(&rs);
        FD_ZERO(&ws);
        FD_ZERO(&es);
        mret = curl_multi_fdset(curl_multi, &rs, &ws, &es, &max);
        if (mret != CURLM_OK) {
            fprintf(stderr, "Error CURL\n");
        }

        /* set options for the curl easy handle */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_file);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, data_read);
        /* for debug */
        /*    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://127.0.0.1"); */
        /* add the easy handle to a multi session */

        if (app->config->show_weather_for_two_hours) {
            curl_handle_hour = weather_curl_init(curl_handle_hour);
            curl_easy_setopt(curl_handle_hour, CURLOPT_URL, hour_url);
            curl_easy_setopt(curl_handle_hour, CURLOPT_WRITEDATA,
                             &html_file_hour);
            curl_easy_setopt(curl_handle_hour, CURLOPT_WRITEFUNCTION,
                             data_read);
        }

        curl_multi_add_handle(curl_multi, curl_handle);
        if (app->config->show_weather_for_two_hours)
            curl_multi_add_handle(curl_multi, curl_handle_hour);
        return TRUE;            /* return to UI */
    } else {
#ifndef RELEASE
        fprintf(stderr, "\n>>>>>>>>>>>Third stage\n");
#endif
//        DEBUG_FUNCTION("\n>>>>>>>>>>>Third stage\n");
        /* The third stage */
        num_msgs = 0;
        while (curl_multi
               && (msg = curl_multi_info_read(curl_multi, &num_msgs))) {
            if (msg->msg == CURLMSG_DONE) {
                if (msg->easy_handle == curl_handle){
                    /* Clean */
                    mret = curl_multi_remove_handle(curl_multi, curl_handle);       /* Delete curl_handle from curl_multi */
                    if (mret != CURLM_OK)
                       fprintf(stderr, " Error remove handle %p\n",
                            curl_handle);
                    curl_easy_cleanup(curl_handle);
                    curl_handle = NULL;
                    if (url) {
                       g_free(url);
                       url = NULL;
                    }
                    if (html_file.stream) {
                        fclose(html_file.stream);
                        html_file.stream = NULL;
                    }

                    if (html_file.filename) {
                        g_free(html_file.filename);
                        html_file.filename = NULL;
                    }
                }
                if (curl_handle_hour && msg->easy_handle == curl_handle_hour && app->config->show_weather_for_two_hours){

                    mret = curl_multi_remove_handle(curl_multi, curl_handle_hour);  /* Delete curl_handle from curl_multi */
                    if (mret != CURLM_OK)
                        fprintf(stderr, " Error remove handle %p\n",curl_handle);
                    curl_easy_cleanup(curl_handle_hour);
                    curl_handle_hour = NULL;
                    if (hour_url) {
                        g_free(hour_url);
                        hour_url = NULL;
                    }
                    if (html_file_hour.stream) {
                            fclose(html_file_hour.stream);
                            html_file_hour.stream = NULL;
                    }
                    if (html_file_hour.filename) {
                            g_free(html_file_hour.filename);
                            html_file_hour.filename = NULL;
                    }
                }
                if (msg->data.result != CURLE_OK) {     /* Not success of the download */
                    if (app->show_update_window)
                        hildon_banner_show_information
                            (app->main_window, NULL,
                             _("Did not download weather"));
                } else {

                    if (curl_handle_hour || curl_handle)
                        return TRUE; /* Continue downloading */
                    /* get next station url */
                    if (!get_station_url(&url, &html_file, &hour_url, &html_file_hour, FALSE)) {        /* Success - all is downloaded */
                        if (app->show_update_window)
                            hildon_banner_show_information
                                (app->main_window, NULL,
                                 _("Weather updated"));
                        redraw_home_window(FALSE);
#ifndef RELEASE
                        fprintf(stderr,
                                "\n>>>>>>>>>>>>>>End of update cycle\n");

#endif
//                        DEBUG_FUNCTION("End of update cycle");
                    } else {
#ifndef RELEASE
                        fprintf(stderr,
                                "\n>>>>>Url - %s, File - %s\n",
                                url, html_file.filename);
#endif
//                        DEBUG_FUNCTION(html_file.filename);
                        /* set options for the curl easy handle */
                        curl_handle = weather_curl_init(curl_handle);
                        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
                        curl_easy_setopt(curl_handle,
                                         CURLOPT_WRITEDATA, &html_file);
                        curl_easy_setopt(curl_handle,
                                         CURLOPT_WRITEFUNCTION, data_read);
                        if (app->config->show_weather_for_two_hours) {
                            curl_handle_hour =
                                weather_curl_init(curl_handle_hour);
                            curl_easy_setopt(curl_handle_hour,
                                             CURLOPT_URL, hour_url);
                            curl_easy_setopt(curl_handle_hour,
                                             CURLOPT_WRITEDATA,
                                             &html_file_hour);
                            curl_easy_setopt(curl_handle_hour,
                                             CURLOPT_WRITEFUNCTION,
                                             data_read);
                        }
                        /* add the easy handle to a multi session */
                        curl_multi_add_handle(curl_multi, curl_handle);
                        if (app->config->show_weather_for_two_hours)
                            curl_multi_add_handle(curl_multi,
                                                  curl_handle_hour);
                        return TRUE;    /* Download next station */
                    }
                }
                /* Clean all */
                if (update_window) {
                    gtk_widget_destroy(update_window);
                    update_window = NULL;
                }
                curl_multi_cleanup(curl_multi);
                free_curl();
//                DEBUG_FUNCTION("This is the end");
                return FALSE;   /* This is the end */
            }
        }
        if (!curl_multi){
            /* Clean all */
            if (update_window) {
                gtk_widget_destroy(update_window);
                update_window = NULL;
            }
            free_curl();
            return FALSE;
        }
        return TRUE;
    }
    app->flag_updating = 0;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return FALSE;
}
/*******************************************************************************/
void clean_download(void) {
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (curl_multi)
        curl_multi_cleanup(curl_multi);
    curl_multi = NULL;
    curl_handle = NULL;
    curl_handle_hour = NULL;
    if (update_window) {
        gtk_widget_destroy(update_window);
        update_window = NULL;
    }
}
/*******************************************************************************/
/* Create URL and filename for xml file. 
 * Returns TRUE if all right otherwise return FLASE.
*/
gboolean
get_station_url(gchar ** url, struct HtmlFile *html_file,
                gchar ** hour_url, struct HtmlFile *html_file_hour,
                gboolean first) {
    gboolean valid = FALSE;
    static GtkTreeIter iter;
    gchar	*station_code = NULL,
		*station_source = NULL;
    gint	source = 0;
    gchar buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (first)
        valid =
            gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                          (app->user_stations_list),
                                          &iter);
    else
        valid =
            gtk_tree_model_iter_next(GTK_TREE_MODEL
                                     (app->user_stations_list), &iter);
    if (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                           &iter,
                           ID0_COLUMN, &station_code,
                           3, &station_source,
                           -1);
/* !!! TODO: recode this */
        /* Default weather.com */
        if (station_source){
            if(!strcmp(station_source, "weather.com"))
                source = 0;
            if(!strcmp(station_source, "rp5.ru"))
                source = 1;
        }else
                source = 0;
        /* prepare url */
        if(weather_sources[source].url){
	    *buffer = 0;
	    snprintf(buffer, sizeof(buffer) - 1,
			weather_sources[source].url, station_code);
	    *url = g_strdup(buffer);
	}
        if(weather_sources[source].hour_url){
	    *buffer = 0;
	    snprintf(buffer, sizeof(buffer) - 1,
			weather_sources[source].hour_url, station_code);
	    *hour_url = g_strdup(buffer);
	}
#ifndef RELEASE
        fprintf(stderr, "\n>>>>>>>>>>URL %s\n", *url);
#endif
        /* preapare filename */
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer) - 1,
                 "%s/%s.xml.new", app->config->cache_dir_name,
                 station_code);
        html_file->filename = g_strdup(buffer);
        html_file->stream = NULL;
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer) - 1,
                 "%s/%s_hour.xml.new",
                 app->config->cache_dir_name, station_code);
        html_file_hour->filename = g_strdup(buffer);
        html_file_hour->stream = NULL;

#ifndef RELEASE
        fprintf(stderr, "\n>>>>>>>>>NAME %s\n", html_file->filename);
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
get_file(const gchar *full_url, const gchar *full_storing_name){
    if(!full_url || !full_storing_name)
        return FALSE;

    return TRUE;
}
/*******************************************************************************/
