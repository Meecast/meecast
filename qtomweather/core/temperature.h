#ifndef TEMPERATURE_H
#define TEMPERATURE_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Temperature{
        std::string *_units;
        float _value;
        public:
            Temperature();
            Temperature(const float value, const std::string& units);
            Temperature(const Temperature& temperature);
            Temperature& operator=(const Temperature& temperature);
            virtual ~Temperature();
            void units(const std::string& name);
            std::string& units() const;
            void value(const float value);
            float value() const;
            float celciusValue() const;
            float fahrenheitValue() const;
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // TEMPERATURE_H
