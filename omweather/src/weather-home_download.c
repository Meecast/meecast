/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) Superkaramba's Liquid Weather ++ team
 *	for ALL the artwork (icons)
 *        The maintainer (also the main author I believe)
 *        is Matthew <dmbkiwi@yahoo.com>
 *  http://liquidweather.net/icons.html#iconsets
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
#include "weather-home_download.h"

CURL 
*weather_curl_init(void)
{
CURL *curl_handle;
    curl_handle  = curl_easy_init(); 
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1); 
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, 
            "Mozilla/5.0 (X11; U; Linux i686; en-US; " 
            "rv:1.8.1.1) Gecko/20061205 Iceweasel/2.0.0.1"); 
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 30); 
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10); 
    
//    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://www.ru");
    
return curl_handle;    
}

int data_read(void *buffer, size_t size, size_t nmemb, void *stream)
{
  int result;
  struct HtmlFile *out=(struct HtmlFile *)stream;
  fprintf(stderr,"%s()\n", __PRETTY_FUNCTION__);
  if(out && !out->stream) {
  /* open file for writing */
  out->stream=fopen(out->filename, "wb");
  if(!out->stream)
  {
      return -1; /* failure, can't open file to write */      
  }      
  }
  result = fwrite(buffer, size, nmemb, out->stream);
  fprintf(stderr,"End %s()\n", __PRETTY_FUNCTION__);
  return result;

}			  

gboolean
download_html(gpointer data)
{

CURLMsg *msg;
int result;

    gint num_transfers = 0, num_msgs = 0;
    
    fprintf(stderr,"%s()\n", __PRETTY_FUNCTION__);
    
    if(curl_multi && CURLM_CALL_MULTI_PERFORM
            == curl_multi_perform(curl_multi, &num_transfers))
    {	
    	fprintf (stderr,"RETURN TRUE\n");
        return TRUE; /* Give UI a chance first. */
    }

    
    if (!curl_handle)
    {
        html_file.filename = full_filename->str;
        html_file.stream = NULL;
	curl_handle = weather_curl_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, url->str);
	if(!curl_multi)
	{
    	    curl_multi = curl_multi_init();
	}
       curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_file);		
       curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, data_read);	
       curl_multi_add_handle(curl_multi, curl_handle);	
    /* for debug */
//    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://127.0.0.1");
       fprintf (stderr,"RETURN INIT\n");
    return TRUE;
    }
    else
    {
        fprintf (stderr,"ELSE\n");
	while(curl_multi && (msg = curl_multi_info_read(curl_multi, &num_msgs)))
	{
	 if(msg->msg == CURLMSG_DONE)
         {
	  if(update_window)
	    gtk_widget_destroy(update_window);
	  if(weather_window_popup)
	    gtk_widget_destroy(weather_window_popup);	    
	  if (html_file.stream)
          fclose (html_file.stream);
	  curl_multi_cleanup(curl_multi);
	  curl_multi = NULL;
	  curl_handle = NULL;
	  curl_easy_cleanup(curl_handle);
	  if (url)
	    g_string_free(url, TRUE);    
	  if (full_filename)
	      g_string_free(full_filename, TRUE);    
  	  hildon_banner_show_information(box,NULL, _("Weather updated"));	      
	  return FALSE;
	 }
	}
       return TRUE;
    }
  return FALSE;
}
