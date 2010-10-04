#include <iostream>
#include <dlfcn.h>
#include <glib.h>
#include <sstream>
using namespace std;


typedef int (*parser_proto)(const gchar*, GHashTable*, gboolean);

void
test (void){
    void *handle;
    GHashTable  *station_data;
    parser_proto parser;
    int result;

    station_data = g_hash_table_new(g_str_hash, g_str_equal);

    handle = dlopen("/usr/lib/omweather/libomweather-weather-com-source.so", RTLD_NOW);
    if(handle){
        dlerror();
        parser = (parser_proto)dlsym(handle, "get_station_weather_data");
        if(!dlerror()){
            result = parser("/scratchbox/users/vlad/home/vlad/apps/omweather/BOXX0014.xml", station_data, FALSE);
            cout<<"result "<<result<<endl;
        }
    }
}

