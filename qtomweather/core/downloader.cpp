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
int
Downloader::progress(void *ptr, double totald, double nowd, double totalu, double nowu)
{
    int d = 40;
    double fract = nowd / totald;
    int dotz = (fract * d < 0) ? (fract * d - 0.5) : (fract * d -0.5);

    int ii = 0;
    printf("%3.0f%% [",fract*100);

    for ( ; ii < dotz; ii++){
        printf("=");
    }
    for ( ; ii < d; ii ++){
        printf(" ");
    }
    printf("]\r");
    fflush(stdout);

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
        //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        //curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, Downloader::progress);
        res = curl_easy_perform(curl);
        std::cerr << "curl result = " << res << std::endl;
        curl_easy_cleanup(curl);
        fclose(fp);
        return true;
    }else return false;
}
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
