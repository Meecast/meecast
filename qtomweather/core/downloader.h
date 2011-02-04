#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "curl/curl.h"
#include <stdio.h>
#include <string>
#include <iostream>

namespace Core{
class Downloader
{
public:
    Downloader();
    static size_t writedata(void *ptr, size_t size, size_t nmemb, FILE *stream);
    static int progress(void *ptr, double totald, double nowd, double totalu, double nowu);
    static bool downloadData(const std::string &filename, const std::string &url);
};
}
#endif // DOWNLOADER_H
