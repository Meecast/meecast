#include "source.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Source::Source() : Parser(){
        _name = new std::string;
        _logo = new std::string;
        _forecastUrl = new std::string;
        _detailUrl = new std::string;
        _searchUrl = new std::string;
        _databaseName = new std::string;
        _binaryName = new std::string;
    }
////////////////////////////////////////////////////////////////////////////////
    Source::~Source(){
        delete _name;
        delete _logo;
        delete _forecastUrl;
        delete _detailUrl;
        delete _searchUrl;
        delete _databaseName;
        delete _binaryName;
    }
////////////////////////////////////////////////////////////////////////////////
    void Source::parse(const std::string& filename){

    }
////////////////////////////////////////////////////////////////////////////////
    Source::Source(const Source& source){
        _name = new std::string;
        _name->assign(*(source._name));
        _logo = new std::string;
        _logo->assign(*(source._logo));
        _forecastUrl = new std::string;
        _forecastUrl->assign(*(source._forecastUrl));
        _detailUrl = new std::string;
        _detailUrl->assign(*(source._detailUrl));
        _searchUrl = new std::string;
        _searchUrl->assign(*(source._searchUrl));
        _databaseName = new std::string;
        _databaseName->assign(*(source._databaseName));
        _binaryName = new std::string;
        _binaryName->assign(*(source._binaryName));
    }
////////////////////////////////////////////////////////////////////////////////
    Source& Source::operator=(const Source& source){
        if(this != &source){
            delete _name;
            _name = new std::string;
            _name->assign(*(source._name));
            delete _logo;
            _logo = new std::string;
            _logo->assign(*(source._logo));
            delete _forecastUrl;
            _forecastUrl = new std::string;
            _forecastUrl->assign(*(source._forecastUrl));
            delete _detailUrl;
            _detailUrl = new std::string;
            _detailUrl->assign(*(source._detailUrl));
            delete _searchUrl;
            _searchUrl = new std::string;
            _searchUrl->assign(*(source._searchUrl));
            delete _databaseName;
            _databaseName = new std::string;
            _databaseName->assign(*(source._databaseName));
            delete _binaryName;
            _binaryName = new std::string;
            _binaryName->assign(*(source._binaryName));
        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
