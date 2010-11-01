#ifndef CONFIG_H
#define CONFIG_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Config : public Parser{
            bool parse(const std::string& filename);
        public:
            Config();
            Config(const Config& config);
            Config& operator=(const Config& config);
            virtual ~Config();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // CONFIG_H
