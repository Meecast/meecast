#include "downloader.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
Downloader::Downloader()
{

}
size_t
Downloader::writedata(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}
bool
Downloader::downloadData(const std::string &filename, const std::string &url)
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
        std::cerr << "open file " << filename << std::endl;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Downloader::writedata);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        std::cerr << "curl result = " << res << std::endl;
        curl_easy_cleanup(curl);
        fclose(fp);
        return true;
    }else return false;
}
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
