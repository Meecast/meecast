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

    CURLcode status;
    long val;
    
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
       num_msgs++;
    /* for debug */
//    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://127.0.0.1");
       fprintf (stderr,"RETURN INIT\n");
    return TRUE;
    }
    else
    {
        fprintf (stderr,"ELSE %i\n",num_msgs);
	while(curl_multi && (msg = curl_multi_info_read(curl_multi, &num_msgs)))
	{
	 if(msg->msg == CURLMSG_DONE)
         {
	  fprintf(stderr,"test after ELSE  CURLMSG_DONE\n");
	  fprintf(stderr,"URL INFO: %s\n",curl_easy_getinfo(msg->easy_handle,CURLINFO_EFFECTIVE_URL,&val));
	  status = curl_easy_getinfo(msg->easy_handle,CURLINFO_HTTP_CODE,&val);
	  if (status != CURLE_OK)
	  {
	   fprintf(stderr,"NOT CURL_OK\n");
	   hildon_banner_show_information(app->main_window, 
			    NULL, _("Did not download weather"));
	  }
	  else
	  {
           fprintf(stderr,"CURL_OK\n");
	   hildon_banner_show_information(app->main_window,
			    NULL, _("Weather updated"));	      
	  } 
	  if(update_window)
	    gtk_widget_destroy(update_window);
	  if(app->popup_window)
	    gtk_widget_destroy(app->popup_window);
	  if (html_file.stream)
          fclose (html_file.stream);
	  curl_multi_remove_handle(curl_multi,msg->easy_handle);
	  curl_multi_cleanup(curl_multi);
	  curl_multi = NULL;
	  curl_handle = NULL;
	  curl_easy_cleanup(curl_handle);
	  if (url)
	    g_string_free(url, TRUE);    
	  if (full_filename)
	      g_string_free(full_filename, TRUE);    

	  return FALSE;
	 }
	}
       return TRUE;
    }
  return FALSE;
}
