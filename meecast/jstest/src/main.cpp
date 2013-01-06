#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "main.h"
extern "C" {
    extern void my_js();
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
    fprintf(stderr,"Enf of creating Config class");
    return config;
}

int 
main(int argc, char *argv[])
{
    ConfigEfl *config;
    my_js();
    fprintf(stderr, "11111111\n");
    config = create_and_fill_config();
    fprintf(stderr, "22222222\n");
    //my_js();
	return 0;
}
