/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
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
#define GCONF_KEY_CURRENT_CONNECTIVITY	"/system/osso/connectivity/IAP/current"
#define URL "http://xoap.weather.com/weather/local/%s?cc=*&prod=xoap&link=xoap&par=1004517364&key=a29796f587f206b2&unit=m&dayf=%d"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
static GString *url = NULL;
static gboolean second_attempt = FALSE;
static CURL *curl_handle = NULL;
static CURL *curl_multi = NULL;
static struct HtmlFile html_file;
static GtkWidget *update_window = NULL;
static GString *full_filename_new_xml = NULL;
static gboolean	valid;
static GtkTreeIter	iter;
static     gchar	*station_name = NULL,
 			*station_code = NULL;
/*******************************************************************************/
/* Create standard Hildon animation small window */
static void create_window_update(void){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    update_window = hildon_banner_show_animation(app->main_window,
						    NULL,
						    _("Update weather"));
}
/*******************************************************************************/
#ifdef USE_DBUS
static DBusHandlerResult
get_connection_status_signal_cb(DBusConnection *connection,
        DBusMessage *message, void *user_data){

    gchar *iap_name = NULL, *iap_nw_type = NULL, *iap_state = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* check signal */
    if(!dbus_message_is_signal(message,
                ICD_DBUS_INTERFACE,
                ICD_STATUS_CHANGED_SIG)){
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    if(!dbus_message_get_args(message, NULL,
                DBUS_TYPE_STRING, &iap_name,
                DBUS_TYPE_STRING, &iap_nw_type,
                DBUS_TYPE_STRING, &iap_state,
                DBUS_TYPE_INVALID)){
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

/*    fprintf(stderr,"OMWeather  - iap_state = %s\n", iap_state);*/
    if(!strcmp(iap_state, "CONNECTED")){
        if(!app->iap_connected){
            app->iap_connected = TRUE;
	    app->iap_connecting = FALSE;
	    app->iap_connecting_timer = 0;
	    if (app->config->downloading_after_connecting)
		add_current_time_event();
        }
    }
    else if (!strcmp(iap_state, "CONNECTING")){
	    app->iap_connected = FALSE;
	    app->iap_connecting = TRUE;
	    app->iap_connecting_timer = 0;
	 }
    else if(app->iap_connected){
    	    app->iap_connected = FALSE; /* !!!!!!!!! Need Remove download */
	    app->iap_connecting = FALSE;
	    app->iap_connecting_timer = 0;
	 }else
	 {
	    app->iap_connecting = FALSE;
	 } 

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}
#endif
/*******************************************************************************/
/* Callback function for request  connection to Internet */
#ifdef USE_CONIC
#define OSSO_CON_IC_CONNECTING             0x05
static void connection_cb(ConIcConnection *connection,
                          ConIcConnectionEvent *event,
                          gpointer user_data)
{
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
	    break ;

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

void iap_callback(struct iap_event_t *event, void *arg){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->iap_connecting = FALSE;
    switch(event->type){
	case OSSO_IAP_CONNECTED:
#ifdef DEBUGFUNCTIONCALL
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
/*******************************************************************************/
void weather_initialize_dbus(void){

    gchar		*filter_string,
			*tmp;
    GConfClient		*gconf_client = NULL;
    DBusConnection	*dbus_conn;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif    
    if(!app->dbus_is_initialize){   
	/* Reseting values */
        app->iap_connecting = FALSE;
	app->iap_connected = FALSE;
	app->iap_connecting_timer = 0;
	
	/* Check connection */
	gconf_client = gconf_client_get_default();
	if(gconf_client){
	    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_CURRENT_CONNECTIVITY, NULL);
	    if(tmp){
    		app->iap_connected = TRUE;
		g_free(tmp);
	    }	
	    else
		app->iap_connected = FALSE;
	    gconf_client_clear_cache(gconf_client);
	    g_object_unref(gconf_client);		
	} 
#ifdef USE_CONIC
	app->connection = con_ic_connection_new();
	if(app->connection != NULL){
	    g_object_set(app->connection, "automatic-connection-events", TRUE, NULL);
	    g_signal_connect(G_OBJECT(app->connection), "connection-event",
                    	     G_CALLBACK(connection_cb),
                	     GINT_TO_POINTER(USER_DATA_MAGIC));
	}
#else	
    	osso_iap_cb(iap_callback);
#endif

#ifdef USE_DBUS	   
	/* Add D-BUS signal handler for 'status_changed' */
        dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
        filter_string = g_strdup_printf("interface=%s", ICD_DBUS_INTERFACE);
        /* add match */
        dbus_bus_add_match(dbus_conn, filter_string, NULL);
        g_free(filter_string);
	/* add the callback */
        dbus_connection_add_filter(dbus_conn,
                		    get_connection_status_signal_cb,
                		    NULL, NULL);	     
				    
#endif /* USE_DBUS */
    /* For Debug on i386 */
#ifndef RELEASE
	app->iap_connected = TRUE; 
#endif
	app->dbus_is_initialize = TRUE;
    }
    
}
/*******************************************************************************/
/* Init easy curl */
CURL* weather_curl_init(CURL *curl_handle){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    curl_handle = curl_easy_init(); 
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, 
            "Mozilla/5.0 (X11; U; Linux i686; en-US; " 
            "rv:1.8.1.1) Gecko/20061205 Iceweasel/2.0.0.1"); 
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 30); 
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10); 
    config_update_proxy();
    /* Set Proxy option */
    if(app->config->iap_http_proxy_host){ 
        curl_easy_setopt(curl_handle, CURLOPT_PROXY, app->config->iap_http_proxy_host); 
        if(app->config->iap_http_proxy_port) 
            curl_easy_setopt(curl_handle, CURLOPT_PROXYPORT, app->config->iap_http_proxy_port); 
    } 
    return curl_handle;    
}
/*******************************************************************************/
static int data_read(void *buffer, size_t size, size_t nmemb, void *stream){
    int result;
    struct HtmlFile *out = (struct HtmlFile *)stream;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(out && !out->stream){
    /* open file for writing */
	out->stream = fopen(out->filename, "wb");
	if(!out->stream)
	    return -1; /* failure, can't open file to write */      
    }
    result = fwrite(buffer, size, nmemb, out->stream);
    return result;
}			  
/*******************************************************************************/
/* Form URL and filename for  write xml file. 
   Returns TRUE if the station is taken from the list
   Else return FLASE. This the end list
*/
static gboolean form_url_and_filename(gchar *station_code){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(station_code == NULL)
	return FALSE;
    if(url){
	    g_string_free(url, TRUE);    
	    url = NULL;
    } 
    if(full_filename_new_xml){
        g_string_free(full_filename_new_xml, TRUE);    
        full_filename_new_xml = NULL;
    } 
    url = g_string_new(NULL);        
    g_string_append_printf(url, URL,
				    station_code, Max_count_weather_day);
    full_filename_new_xml = g_string_new(NULL);        
    g_string_append_printf(full_filename_new_xml,"%s/%s.xml.new",
				    app->config->cache_dir_name,station_code);
    /* Forming structure for download data of weather */
    html_file.filename = full_filename_new_xml->str;
    html_file.stream = NULL;
    return TRUE;
}
/*******************************************************************************/
/* Download html/xml file. Call every 100 ms after begin download */
gboolean download_html(gpointer data){
    CURLMsg	*msg;
    CURLMcode	mret;
    fd_set	rs, ws, es;
    int		max;
    gint	num_transfers = 0,
		num_msgs = 0;		
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->popup_window && app->show_update_window){
	gtk_widget_destroy(app->popup_window);
	app->popup_window = NULL;
    }
    /* If not connected and it autoupdate do go away */
    if(!app->show_update_window && !app->iap_connected){
    	app->flag_updating = 0;
	return FALSE;
    }
    if(app->iap_connected) 
	second_attempt = TRUE;

/* Connection wake up */    
    if( app->show_update_window && (!second_attempt) && (!app->iap_connecting) ){    
        app->iap_connecting = TRUE;
#ifdef USE_CONIC
	if (app->connection)
    	    con_ic_connection_connect(app->connection, CON_IC_CONNECT_FLAG_NONE);
	else
	    return FALSE;
#else
        if(osso_iap_connect(OSSO_IAP_ANY, OSSO_IAP_REQUESTED_CONNECT, NULL) != OSSO_OK){
    	    fprintf(stderr,"after 1 osso_iap_connect(OSSO_IAP_ANY, OSSO_IAP_REQUESTED_CONNECT, NULL) != OSSO_OK){\n");		

	}  	
#endif
    	app->flag_updating = 0; 
	second_attempt = TRUE;
        return TRUE;
    }

    if(app->iap_connecting){
	/* Check buggy */
	if (app->iap_connecting_timer > 150){
	    if(app->show_update_window){
		if(update_window){
		    gtk_widget_destroy(update_window);
		    update_window = NULL;
		}
	    	hildon_banner_show_information(app->main_window,
					    NULL,
					    _("Not connected to Internet\nConnection time is expired"));
		app->iap_connecting_timer = 0;
	    }				    
	    app->iap_connecting = FALSE;
	    return FALSE;
	}else{
    	    app->iap_connecting_timer++;
	    return TRUE;
	}    
    }
	
    second_attempt = FALSE;
    /* The second stage */
    /* call curl_multi_perform for read weather data from Inet */
    if(curl_multi && CURLM_CALL_MULTI_PERFORM ==
            curl_multi_perform(curl_multi, &num_transfers))
        return TRUE; /* return to UI */
    /* The first stage */
    if(!curl_handle){
	if(app->show_update_window)
    	    create_window_update(); /* Window with update information */
        /* Initialize list */
	valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
					    &iter);
	if (valid){
	    if (station_name)
		g_free(station_name);
	    if (station_code)
		g_free(station_code);
	    gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
				&iter, 
                    		0, &station_name,
                    		1, &station_code,
                		-1);
	}			    
	if(!valid || !form_url_and_filename(station_code)){
	    if(url){
		g_string_free(url, TRUE);    
		url = NULL;
	    }	 
	    if(full_filename_new_xml){
	        g_string_free(full_filename_new_xml, TRUE);    
	    	full_filename_new_xml = NULL;
	    }
	    app->flag_updating = 0;	 
	    return FALSE; /* The strange error */		
	} 
	valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
					    &iter);
	if (valid){
	    if (station_name)
		g_free(station_name);
	    if (station_code)
		g_free(station_code);
	    gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
				&iter, 
                    		0, &station_name,
                    		1, &station_code,
                		-1);			
	}
	/* Init easy_curl */
	curl_handle = weather_curl_init(curl_handle);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url->str);
	/* Init curl_mult */
	if(!curl_multi)
    	    curl_multi = curl_multi_init();
	max = 0;
	FD_ZERO(&rs);
	FD_ZERO(&ws);
	FD_ZERO(&es);
	mret = curl_multi_fdset(curl_multi,&rs,&ws,&es,&max);
	if(mret != CURLM_OK){
	    fprintf (stderr,"Error CURL\n");
	}
	/* set options for the curl easy handle */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_file);		
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, data_read);	
        /* for debug */
        /*    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://127.0.0.1"); */
        /* add the easy handle to a multi session */
        curl_multi_add_handle(curl_multi, curl_handle);	
        return TRUE; /* return to UI */
    }
    else{
        /* The third stage */
	num_msgs = 0;
	while(curl_multi && (msg = curl_multi_info_read(curl_multi, &num_msgs))){
	    if(msg->msg == CURLMSG_DONE){	  
		if(msg->data.result != CURLE_OK){ /* Not success of the download */
		    if(app->show_update_window)
			hildon_banner_show_information(app->main_window, 
							NULL,
							_("Did not download weather"));
		}
		else{ /* Clean */
		    mret = curl_multi_remove_handle(curl_multi,curl_handle); /* Delete curl_handle from curl_multi */
		    if (mret != CURLM_OK)
			fprintf(stderr," Error remove handle %p\n",curl_handle);
			
		    curl_easy_cleanup(curl_handle); 
		    curl_handle = NULL;

		    if(html_file.stream)
        		fclose(html_file.stream);
		    
		    if(!valid || !form_url_and_filename(station_code) ){ /* Success - all is downloaded */
			if(app->show_update_window)
			    hildon_banner_show_information(app->main_window,
							    NULL,
							    _("Weather updated"));
        		redraw_home_window();	
		    }
		    else{
			valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
					    &iter);
			if (valid){
			    if (station_name)
				g_free(station_name);
			    if (station_code)
				g_free(station_code);			
			    gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
				&iter, 
                    		0, &station_name,
                    		1, &station_code,
                		-1);
			}	
			/* set options for the curl easy handle */
			curl_handle = weather_curl_init(curl_handle);
			curl_easy_setopt(curl_handle, CURLOPT_URL, url->str);
        		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_file);		
        		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, data_read);	
    			/* add the easy handle to a multi session */
    			curl_multi_add_handle(curl_multi, curl_handle);	
			return TRUE;/* Download next station */
		    }		    
		}

		if(update_window){
		    gtk_widget_destroy(update_window);
		    update_window = NULL;
		}
	  /* Clean all */    
	  /*
		if (msg->easy_handle) {
    		    curl_multi_remove_handle(curl_multi,msg->easy_handle);
		    curl_easy_cleanup(msg->easy_handle);
		    msg->easy_handle = NULL;
		}
		*/
		if(curl_handle){
		    curl_easy_cleanup(curl_handle); 
		    curl_handle = NULL;
		}
		curl_multi_cleanup(curl_multi);
		curl_multi = NULL;
		curl_handle = NULL;
		if(url){
		    g_string_free(url, TRUE);    
		    url = NULL;
		}	 
		if(full_filename_new_xml){
		    g_string_free(full_filename_new_xml, TRUE);    	  
		    full_filename_new_xml = NULL;
		}
		app->flag_updating = 0;     
		return FALSE; /* This is the end */
	    }
	}
	return TRUE;
    }
    app->flag_updating = 0;
    return FALSE;
}
/*******************************************************************************/
void clean_download(void){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(curl_multi)
	curl_multi_cleanup(curl_multi);
    curl_multi = NULL;
    curl_handle = NULL;
    if(update_window){
        gtk_widget_destroy(update_window);
        update_window = NULL;
    }
}
/*******************************************************************************/
/*
void pre_update_weather(void){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!app->dbus_is_initialize)
	weather_initialize_dbus();
    update_weather(TRUE);
}
*/
/*******************************************************************************/
