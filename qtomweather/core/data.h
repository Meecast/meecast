#ifndef DATA_H
#define DATA_H
////////////////////////////////////////////////////////////////////////////////
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Data : public Parser{
            void parse(const std::string& filename);
            void wallkTree(const xmlpp::Node* node);
        public:
            Data();
            Data(const Data& data);
            Data& operator=(const Data& data);
            virtual ~Data();
            void clear();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATA_H
