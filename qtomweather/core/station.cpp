#include "station.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const std::string& source_name, const std::string& id,
                     const std::string& name, const std::string& country,
                     const std::string& region){
        _id = new std::string(id);
        _name = new std::string(name);
        _country = new std::string(country);
        _region = new std::string(region);
        _timezone = 0;
        _fileName = NULL;
        _source = NULL;
    }
////////////////////////////////////////////////////////////////////////////////
    Station::~Station(){
        delete _name;
        delete _id;
        delete _country;
        delete _region;
        if(_data)
            delete _data;
        if(_fileName)
            delete _fileName;
    }
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const Station& station){
        _id = new std::string(*(station._id));
        _name = new std::string(*(station._name));
        _country = new std::string(*(station._country));
        _region = new std::string(*(station._region));
        _source = station._source;
    }
////////////////////////////////////////////////////////////////////////////////
    Station& Station::operator=(const Station& station){
        if(this != &station){
            delete _name;
            _name = new std::string(*(station._name));
            delete _id;
            _id = new std::string(*(station._id));
            delete _country;
            _country = new std::string(*(station._country));
            delete _region;
            _region = new std::string(*(station._region));
        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
    void Station::name(const std::string& name){
        _name->assign(name);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Station::name() const{
        return *_name;
    }
////////////////////////////////////////////////////////////////////////////////
    void Station::id(const std::string& id){
        _id->assign(id);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Station::id() const{
        return *_id;
    }
////////////////////////////////////////////////////////////////////////////////
    void Station::timezone(const int timezone){
        _timezone = timezone;
    }
////////////////////////////////////////////////////////////////////////////////
    int Station::timezone() const{
        return _timezone;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Station::dataValid(){
        return true;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Station::prepareFile(){
        return true;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Station::updateData(bool force){
        if(!force || dataValid())
            return true;
        return true;
    }
////////////////////////////////////////////////////////////////////////////////
    void Station::source(Source *source){
           _source = source;
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Station::forecastURL() const{
        std::cout<<"uuuuuuuuu"<<std::endl;
        return _source->forecastURL();
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
