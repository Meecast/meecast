#include "abstractconfig.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
#ifdef LOCALDEBUG
    std::string AbstractConfig::prefix = "";
    std::string AbstractConfig::sharePath = "../test/";
    std::string AbstractConfig::schemaPath = "../test/schemas/";
    std::string AbstractConfig::iconsPath = "../test/iconsets/";
    std::string AbstractConfig::libPath = "lib/omweather/";
    std::string AbstractConfig::sourcesPath = "../test/sources/";
    std::string AbstractConfig::layoutqml = "weatherlayoutitem.qml";
    //std::string AbstractConfig::configPath = "../test/";
#else
    std::string AbstractConfig::prefix = "/usr/";
    std::string AbstractConfig::sharePath = "share/omweather/";
    std::string AbstractConfig::schemaPath = "share/omweather/xsd/";
    std::string AbstractConfig::iconsPath = "share/omweather/iconsets/";
    std::string AbstractConfig::libPath = "lib/omweather/";
    std::string AbstractConfig::sourcesPath = "share/omweather/sources/";
    std::string AbstractConfig::layoutqml = "/usr/share/omweather/qml/weatherlayoutitem.qml";
    //std::string AbstractConfig::configPath = "~/.config/omweather/";
#endif
    std::string AbstractConfig::getConfigPath()
    {
#ifdef LOCALDEBUG
        return "../test/";
#else
        char filepath[4096];
        struct passwd *pw = getpwuid(getuid());
        std::string path(pw->pw_dir);
        path += "/.config";
        /* TODO check directory exist */
        mkdir(path.c_str(), 0755);
        path += "/omweather/";
        mkdir(path.c_str(), 0755);
        return path;
#endif
    }
} // namespace Core
