#ifndef DATA_H
#define DATA_H
////////////////////////////////////////////////////////////////////////////////
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Data {
        float _temperature;
        float _flike;
        float _windSpeed;
        std::string _windDirection;
        float _windGust;
        int _humidity;
        int _pressure;
        std::string _text;
        int _icon;
        public:
            Data();
            Data(const Data& data);
            Data& operator=(const Data& data);
            virtual ~Data();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATA_H
