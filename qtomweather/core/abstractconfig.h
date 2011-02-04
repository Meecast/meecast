#ifndef ABSTRACTCONFIG_H
#define ABSTRACTCONFIG_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    class AbstractConfig{
        public:
            static std::string prefix;
            static std::string schemaPath;
            static std::string iconsPath;
            static std::string sharePath;
            static std::string libPath;
            static std::string sourcesPath;
            static std::string getConfigPath();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // ABSTRACTCONFIG_H
