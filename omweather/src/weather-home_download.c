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
  sleep (1);
  result = fwrite(buffer, size, nmemb, out->stream);
  fprintf(stderr,"End %s()\n", __PRETTY_FUNCTION__);
  return result;

}			  

int
download_html(GString *url, GString *full_filename)
{
CURL *curl_handle;
int result;

    struct HtmlFile html_file;
    
    fprintf(stderr,"%s()\n", __PRETTY_FUNCTION__);
    html_file.filename = full_filename->str;
    html_file.stream = NULL;
    
	    
    curl_handle = weather_curl_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url->str);
    fprintf (stderr,"URL: %s\n",url->str);    
    /* for debug */
//    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://127.0.0.1");
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,
                data_read);	
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_file);

    if(CURLE_OK != curl_easy_perform(curl_handle))
    {
	fprintf(stderr,"Not Ok\n");
	result = -1;
    }
    else
    {
	fprintf(stderr,"Ok\n");     
	result = 0;
    }	
  if (html_file.stream)
   fclose (html_file.stream);
  curl_easy_cleanup(curl_handle);
  return result;
}
