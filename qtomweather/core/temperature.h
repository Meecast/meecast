#ifndef TEMPERATURE_H
#define TEMPERATURE_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <limits.h>
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Temperature{
        std::string *_units;
        float _value;
        public:
            Temperature(const float value = INT_MAX, const std::string& units = "C");
            Temperature(const Temperature& temperature);
            Temperature& operator=(const Temperature& temperature);
            virtual ~Temperature();
            void units(const std::string& name);
            std::string& units() const;
            void value(const float value);
            float value() const;
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // TEMPERATURE_H
