#include <iostream>
#include <dlfcn.h>
#include <sstream>
using namespace std;

void
test (){
    void *handle;
    void *parser;
    handle = dlopen("/usr/lib/omweather/libomweather-weather-com-source.so", RTLD_NOW);
    if(handle){
        dlerror();
        parser = dlsym(handle, "get_station_weather_data");
        if(!dlerror()){
            cout<<"parser "<< parser<<endl;
        }
    }
}
