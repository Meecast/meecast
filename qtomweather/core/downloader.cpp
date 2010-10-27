#include "downloader.h"
#include <cstdio>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Downloader::Downloader(){
        _fileName = new std::string;
        _url = new std::string;
        _file = NULL;
        _handle = NULL;
        _headers = NULL;
        _connection = NULL;
        _status = 0;
    }
////////////////////////////////////////////////////////////////////////////////
    Downloader::~Downloader(){
        delete _fileName;
        delete _url;
        if(_file)
            fclose(_file);
        if(_handle)
            curl_easy_cleanup(_handle);
        if(_headers)
            curl_slist_free_all(_headers);
        if(_connection)
            delete _connection;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Downloader::get(const std::string& url, const std::string& filename){
        if(!url.length() || !filename.length())
            return false;
        _url->assign(url);
        _fileName->assign(filename);
        if(_connection)
            delete _connection;
        _connection = new Connection;
        if(!_connection)
            return false;
        if(!init())
            return false;
        _status = curl_easy_perform(_handle);
        return true;
    }
////////////////////////////////////////////////////////////////////////////////
    void Downloader::dataTimeout(long timeout){
        _dataTimeout = timeout;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Downloader::init(){
        if(_handle)
            curl_easy_cleanup(_handle);
        _handle = curl_easy_init();
        if(!_handle)
            return false;
        if(_file)
            fclose(_file);
        _file = fopen(_fileName->c_str(), "wb");
        if(!_file){
            curl_easy_cleanup(_handle);
            return false;
        }
        curl_easy_setopt(_handle, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(_handle, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(_handle, CURLOPT_FAILONERROR, 1);
        curl_easy_setopt(_handle, CURLOPT_USERAGENT, USERAGENT);
        curl_easy_setopt(_handle, CURLOPT_URL, _url->c_str());
        curl_easy_setopt(_handle, CURLOPT_TIMEOUT, _dataTimeout);
        curl_easy_setopt(_handle, CURLOPT_CONNECTTIMEOUT, _connection->timeout());
        curl_easy_setopt(_handle, CURLOPT_NOSIGNAL, 1);
        if(_headers)
            curl_slist_free_all(_headers);
        _headers = curl_slist_append(_headers, "Cache-Control: no-cache");
        _headers = curl_slist_append(_headers, "Pragma: no-cache");
        curl_easy_setopt(_handle, CURLOPT_HTTPHEADER, _headers);
        if(_connection->hasProxy()){
            curl_easy_setopt(_handle, CURLOPT_PROXY, _connection->proxyHost().c_str());
            curl_easy_setopt(_handle, CURLOPT_PROXYPORT, _connection->proxyPort());
        }
        curl_easy_setopt(_handle, CURLOPT_WRITEDATA, _file);
        curl_easy_setopt(_handle, CURLOPT_WRITEFUNCTION, dataRead);
        return true;
    }
////////////////////////////////////////////////////////////////////////////////
    int Downloader::status() const{
        return _status;
    }
////////////////////////////////////////////////////////////////////////////////
    int dataRead(void *buffer, size_t size, size_t nmemb, void *stream){
        return fwrite(buffer, size, nmemb, (FILE*)stream);
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
