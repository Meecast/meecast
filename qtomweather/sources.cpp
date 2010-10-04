#include <iostream>
#include <dlfcn.h>
#include <glib.h>
#include <sstream>
using namespace std;

extern "C" {
typedef void (*) (*parser_proto)(const gchar*, GHashTable*, gboolean);

void
test (void){
    void *handle;
    GHashTable  *station_data;
    parser_proto parser;



    handle = dlopen("/usr/lib/omweather/libomweather-weather-com-source.so", RTLD_NOW);
    if(handle){
        dlerror();
        parser = dlsym(handle, "get_station_weather_data");
        if(!dlerror()){
            cout<<"parser "<< parser<<endl;
            parser("/tmp/test.xml", station_data, TRUE);
        }
    }
}
}
