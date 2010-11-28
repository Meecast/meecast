#include "station.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Station::Station(){
        _name = new std::string;
        _id = new std::string;
        _country = new std::string;
        _region = new std::string;
        _timezone = 0;
        _data = new Data;
        _fileName = NULL;
    }
////////////////////////////////////////////////////////////////////////////////
    Station::~Station(){
        delete _name;
        delete _id;
        delete _country;
        delete _region;
        delete _data;
        if(_fileName)
            delete _fileName;
    }
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const Station& station){
        _name = new std::string;
        _name->assign(*(station._name));
        _id = new std::string;
        _id->assign(*(station._id));
        _country = new std::string;
        _country->assign(*(station._country));
        _region = new std::string;
        _region->assign(*(station._region));
    }
////////////////////////////////////////////////////////////////////////////////
    Station& Station::operator=(const Station& station){
        if(this != &station){
            delete _name;
            _name = new std::string;
            _name->assign(*(station._name));
            delete _id;
            _id = new std::string;
            _id->assign(*(station._id));
            delete _country;
            _country = new std::string;
            _country->assign(*(station._country));
            delete _region;
            _region = new std::string;
            _region->assign(*(station._region));
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
} // namespace Core
