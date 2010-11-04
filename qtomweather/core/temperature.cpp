#include "temperature.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Temperature::~Temperature(){
        delete _units;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature::Temperature(const float value, const std::string& units){
        _units = new std::string;
        _units->assign(units);
        if(*_units == "F")
             _value = (value - 32.0f ) / 1.8f;
        else
            _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature::Temperature(const Temperature& temperature){
        _units = new std::string;
        _units->assign(*(temperature._units));
        _value = temperature._value;
    }
////////////////////////////////////////////////////////////////////////////////
    Temperature& Temperature::operator=(const Temperature& temperature){
        if(this != &temperature){
            delete _units;
            _units = new std::string;
            _units->assign(*(temperature._units));
            _value = temperature._value;
        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
    void Temperature::units(const std::string& units){
        _units->assign(units);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Temperature::units() const{
        return *_units;
    }
////////////////////////////////////////////////////////////////////////////////
    void Temperature::value(const float value){
        _value = value;
    }
////////////////////////////////////////////////////////////////////////////////
    float Temperature::value() const{
        if(*_units == "F")
            return ((_value - 32.0f ) / 1.8f);
        return _value;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
