#include "station.h"
#include <iostream> 
#include <cstdlib>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const std::string& source_name, const std::string& id, const std::string& name,
                     const std::string& country, const std::string& region, const std::string& forecastURL){
        _sourceName = new std::string(source_name);
        _id = new std::string(id);
        _name = new std::string(name);
        _country = new std::string(country);
        _region = new std::string(region);
        _forecastURL = new std::string(forecastURL);
        _timezone = 0;
        _fileName = new std::string();
        _source = this->getSourceByName();
        _converter = new std::string();
    }
////////////////////////////////////////////////////////////////////////////////
    Station::~Station(){
        delete _sourceName;
        delete _id;
        delete _name;
        delete _country;
        delete _region;
        delete _forecastURL;
        if(_data)
            delete _data;
        if(_fileName)
            delete _fileName;
        if(_converter)
            delete _converter;
        if (_source)
            delete _source;
    }
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const Station& station){
        _sourceName = new std::string(*(station._sourceName));
        _id = new std::string(*(station._id));
        _name = new std::string(*(station._name));
        _country = new std::string(*(station._country));
        _region = new std::string(*(station._region));
        _forecastURL = new std::string(*(station._forecastURL));
        _fileName = new std::string(*(station._fileName));
        _converter = new std::string(*(station._converter));
    }
////////////////////////////////////////////////////////////////////////////////
    Station& Station::operator=(const Station& station){
        if(this != &station){
            delete _sourceName;
            _sourceName = new std::string(*(station._sourceName));
            delete _id;
            _id = new std::string(*(station._id));
            delete _name;
            _name = new std::string(*(station._name));
            delete _country;
            _country = new std::string(*(station._country));
            delete _region;
            _region = new std::string(*(station._region));
            delete _forecastURL;
            _forecastURL = new std::string(*(station._forecastURL));
            delete _fileName;
            _fileName = new std::string(*(station._fileName));
            delete _converter;
            _converter = new std::string(*(station._converter));
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
    void Station::sourceName(const std::string& sourceName){
        _sourceName->assign(sourceName);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::sourceName() const{
        return *_sourceName;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::forecastURL(const std::string& forecastURL){
        _sourceName->assign(forecastURL);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::forecastURL() const{
        return *_forecastURL;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::country(const std::string& country){
        _country->assign(country);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::country() const{
        return *_country;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::region(const std::string& region){
        _region->assign(region);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::region() const{
        return *_region;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::fileName(const std::string& fileName){
        _fileName->assign(fileName);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::fileName() const{
        return *_fileName;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::converter(const std::string &converter)
    {
        _converter->assign(converter);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::converter() const{
        return *_converter;
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
//        if(!force || dataValid())
//            return true;
        std::string command;
        //std::string command = "wget -O ";
        //command = command + this->fileName() + ".orig '" + this->forecastURL() + "'";
        //std::cerr<<" URL "<<command<<std::endl;
        //if (system(command.c_str()) == 0){
        if (Downloader::downloadData(this->fileName()+".orig", this->forecastURL())) {
            command = this->converter()+ " " +  this->fileName() + ".orig " + this->fileName();
            std::cerr<<" EXEC "<<command<<std::endl;
            if (system(command.c_str()) == 0)
                return true;
            else
               return false;
        }else{
            std::cerr<<" EXEC22 "<<command<<std::endl;
            return false;
        }
    }
////////////////////////////////////////////////////////////////////////////////
    void Station::updateSource(const Source* source){
        _source = const_cast<Source*>(source);
    }
////////////////////////////////////////////////////////////////////////////////
    Source* Station::getSourceByName()
    {
        std::string path(Core::AbstractConfig::prefix);
        path += Core::AbstractConfig::sourcesPath;

        SourceList *sourcelist = new Core::SourceList(path);

        for (int i=0; i<sourcelist->size(); i++){
            if (_sourceName->compare(sourcelist->at(i)->name()) == 0)
                return sourcelist->at(i);
        }
        return 0;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
