#ifndef DOWNLOADER_H
#define DOWNLOADER_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <curl/multi.h>
#include "connection.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    int dataRead(void *buffer, size_t size, size_t nmemb, void *stream);
    class Downloader{
#define USERAGENT "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1.1) Gecko/20061205 Iceweasel/2.0.0.1"
        std::string *_fileName;
        std::string *_url;
        Connection *_connection;
        FILE *_file;
        CURL *_handle;
        struct curl_slist *_headers;
        bool init();
        int _status;
        long _dataTimeout;
        public:
            Downloader();
            virtual ~Downloader();
            bool get(const std::string& url, const std::string& filename);
            int status() const;
            void dataTimeout(long timeout = 60);
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DOWNLOADER_H
