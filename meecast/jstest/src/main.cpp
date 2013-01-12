#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "main.h"

// Optimizations might wipe out our functions without this
#define KEEPALIVE __attribute__((used))

/* Common */
ConfigEfl *config;
int config_is_ready = 0;
int stub = 1;
Core::DatabaseSqlite *db;

char  global_temp_buffer[16384];

extern "C" {
    extern void prepare_config_js();
    extern void prepare_database_js();
    extern char* currentstationname_js();
    extern char* create_sources_list_js();
    extern char* create_countries_list_js();

    void EMSCRIPTEN_KEEPALIVE 
    prepareconfig() {
        config_is_ready = 1;
    }
    char* EMSCRIPTEN_KEEPALIVE 
    create_countries_list(){
        fprintf(stderr,"prepare_databases is done\n");
        std::string buf;
        memset(global_temp_buffer, 0, sizeof(global_temp_buffer));
        db = new Core::DatabaseSqlite("weather.com.db");
        if (db->open_database()){
            Core::listdata * countrylist = db->create_countries_list();
            fprintf(stderr,"prepare_databases is donei2\n");
            buf = "[";
            for (short i=0; i < countrylist->size();i++){
                if (i !=0)
                    buf = buf + ",";
                buf = buf + "\""+ (char *)(countrylist->at(i).second.c_str()) +"\"";
            } 
            buf = buf + "]";
            snprintf(global_temp_buffer, sizeof(global_temp_buffer) -1, "%s", buf.c_str());
            fprintf(stderr,"Success DB!!!!\n");
            return global_temp_buffer;
        }else{
             fprintf(stderr," NOT Success DB!!!!\n");
             return NULL;
        }
    }

    char* EMSCRIPTEN_KEEPALIVE 
    current_station_name() {
        if (config->stationname().c_str())
            return (char*)config->stationname().c_str();
        else
            return ""; 
    }
    char* EMSCRIPTEN_KEEPALIVE
    create_sources_list(void){
        std::string buf;
        memset(global_temp_buffer, 0, sizeof(global_temp_buffer));
        std::string path(Core::AbstractConfig::sourcesPath);
        Core::SourceList *sourcelist = new Core::SourceList(path);
        buf = "[";
        for (short i=0; i < sourcelist->size();i++){
          if (i !=0)
              buf = buf + ",";
          buf = buf + "\""+ (char *)sourcelist->at(i)->name().c_str() +"\"";
        }
        buf = buf + "]";
        delete sourcelist;
        snprintf(global_temp_buffer, sizeof(global_temp_buffer) -1, "%s", buf.c_str());
        return global_temp_buffer;
    }

}

ConfigEfl *
create_and_fill_config(){
    ConfigEfl *config;

    std::cerr<<"Create Config class: " << std::endl;
    try{
        config = ConfigEfl::Instance(Core::AbstractConfig::getConfigPath()+
                                       "config.xml",
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config =  ConfigEfl::Instance();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config =  ConfigEfl::Instance();
    }
    config->saveConfig();
    fprintf(stderr,"End of creating Config class");
    return config;
    
}

void mainLoop(void)
{
        //printf("%d\n", config_is_ready);
        if (config_is_ready){
            config_is_ready = 0;
            config = create_and_fill_config();
        }
}

int 
main(int argc, char *argv[])
{
   // prepare_config_js();

    if (stub >1){
         fprintf(stderr, "Current station %s\n",currentstationname_js());
         prepare_config_js();
         prepareconfig();
         create_sources_list_js();
         create_countries_list_js();
         prepare_database_js();
    }
    fprintf(stderr, "11111111\n");
    emscripten_set_main_loop(mainLoop, 0, 1);
    fprintf(stderr, "22222222\n");
    //my_js();
	return 0;
}

