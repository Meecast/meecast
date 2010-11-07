#ifndef CONFIG_H
#define CONFIG_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Config : public Parser{
            bool parse(const std::string& filename);
            std::string *_base_icons_path;
        public:
            Config();
            Config(const Config& config);
            Config& operator=(const Config& config);
            virtual ~Config();
            void  Base_Icons_Path(const std::string& text);
            std::string& Base_Icons_Path(void);
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // CONFIG_H
