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
Core::DatabaseSqlite *db = NULL;

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
    create_countries_list(char *dbase){
        fprintf(stderr,"prepare_databases is done %p\n", db);
        std::string buf;
        memset(global_temp_buffer, 0, sizeof(global_temp_buffer));
        if (db){
            if (dbase && db->get_databasename().c_str() && strcmp(dbase,db->get_databasename().c_str())){
                fprintf(stderr, "file :%s\n", db->get_databasename().c_str()); 
                remove (db->get_databasename().c_str());
            }
            delete db;
            db = NULL;
        }
        db = new Core::DatabaseSqlite(dbase);
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

    char* EMSCRIPTEN_KEEPALIVE
    create_regions_list(char *country_name){
        std::string buf;
        memset(global_temp_buffer, 0, sizeof(global_temp_buffer));
        //app->country_id = atoi((char *)(countrylist->at(app->index_list-1).first.c_str()));
        //app->country = new std::string (countrylist->at(app->index_list - 1).second.c_str());
        Core::listdata * regionlist = db->create_region_list_by_name(std::string(country_name));
        buf = "[";
        for (short i=0; i < regionlist->size();i++){
          if (i !=0)
              buf = buf + ",";
          buf = buf + "\""+ (char *)regionlist->at(i).second.c_str() +"\"";
        }
        buf = buf + "]";
        snprintf(global_temp_buffer, sizeof(global_temp_buffer) -1, "%s", buf.c_str());
        return global_temp_buffer;
    }

    char* EMSCRIPTEN_KEEPALIVE
    create_stations_list(char* country_name, char *region_name){
        std::string buf;
        memset(global_temp_buffer, 0, sizeof(global_temp_buffer));
        //app->country_id = atoi((char *)(countrylist->at(app->index_list-1).first.c_str()));
        //app->country = new std::string (countrylist->at(app->index_list - 1).second.c_str());
        fprintf(stderr,"Country name %s Region name %s\n",country_name, region_name);
        Core::listdata * stationlist = db->create_stations_list_by_name(std::string(country_name), std::string(region_name));
        buf = "[";
        for (short i=0; i < stationlist->size();i++){
          if (i !=0)
              buf = buf + ",";
          buf = buf + "\""+ (char *)stationlist->at(i).second.c_str() +"\"";
        }
        buf = buf + "]";
        snprintf(global_temp_buffer, sizeof(global_temp_buffer) -1, "%s", buf.c_str());
        return global_temp_buffer;
    }

    char* EMSCRIPTEN_KEEPALIVE
    station_code(char* country_name, char *region_name, char *station_name){
        std::string buf;
        memset(global_temp_buffer, 0, sizeof(global_temp_buffer));
        //app->country_id = atoi((char *)(countrylist->at(app->index_list-1).first.c_str()));
        //app->country = new std::string (countrylist->at(app->index_list - 1).second.c_str());
        fprintf(stderr,"Country name %s Region name %s Station name %s\n",country_name, region_name, station_name);
         ;
        snprintf(global_temp_buffer, sizeof(global_temp_buffer) -1, "%s", db->get_station_code_by_name(std::string(country_name), std::string(region_name), std::string(station_name)).c_str());
        fprintf(stderr, "Code: %s\n", global_temp_buffer);
        return global_temp_buffer;
    }
    char* EMSCRIPTEN_KEEPALIVE
    save_station( char* source_name, char* country_name, char *region_name, char *station_name, char *station_code){
        Core::Station *station;
        fprintf(stderr," Source database %s\n", source_name);
        Core::Source *s = new Core::Source(std::string(source_name), "source.xsd");
        std::string url_template = s->url_template();
        std::string url_detail_template = s->url_detail_template();
        std::string url_for_view = s->url_for_view();
        std::string url_for_map = s->url_for_map();
        std::string cookie = s->cookie();

        char forecast_url[4096];
        snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), station_code);
        std::string s_forecast_url = forecast_url;
        char forecast_detail_url[4096];
        snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, url_detail_template.c_str(), station_code);
        std::string s_forecast_detail_url = forecast_detail_url;
        char view_url[4096];
        snprintf(view_url, sizeof(view_url)-1, url_for_view.c_str(), station_code);
        std::string s_view_url = view_url;
        char map_url[4096];
        snprintf(map_url, sizeof(map_url)-1, url_for_map.c_str(), station_code);
        std::string s_map_url = map_url;



        station = new Core::Station(
                    std::string(source_name),
                    std::string (station_code),
                    std::string (station_name),
                    std::string (country_name),
                    std::string (region_name),
                    s_forecast_url,
                    s_forecast_detail_url,
                    s_view_url,
                    s_map_url,
                    cookie,
                    false, 0.0, 0.0);
        std::string filename(Core::AbstractConfig::getConfigPath());
        filename += source_name;
        filename += "_";
        filename += station_code;
        station->fileName(filename);
        station->converter(s->binary());

        config->stationsList().push_back(station);
        if (config->stationsList().size() > 0)
            config->current_station_id(0);
        config->saveConfig();

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

