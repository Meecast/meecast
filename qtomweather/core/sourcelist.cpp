#include "sourcelist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    SourceList::SourceList(const std::string path){
        Dirent *dp = 0;
        DIR *dir_fd = opendir(path.c_str());
        if(dir_fd){
            while((dp = readdir(dir_fd))){
                std::string name = dp->d_name;
                if(name == "." || name == "..")
                    continue;
                if(dp->d_type == DT_REG || dp->d_type == DT_LNK){
                    try{
                        std::string file = path + dp->d_name;
                        std::string schemafile = AbstractConfig::prefix;
                        schemafile += AbstractConfig::schemaPath;
                        schemafile += "source.xsd";
                        std::cerr << "schema = " << schemafile << std::endl;
                        Source *s = new Source(file, schemafile);
                        this->push_back(s);
                    }
                    catch(std::string& err){
                        std::cerr << "error " << err << std::endl;
                        continue;
                    }
                    catch(const char *err){
                        std::cerr << "error " << err << std::endl;
                        continue;
                    }
                }
            }
            closedir(dir_fd);
        }
        _stations = new StationsList;
    }
////////////////////////////////////////////////////////////////////////////////
    SourceList::~SourceList(){
        for(unsigned i = 0; i < this->size(); i++){
            Source *s = this->at(i);
            if(s)
                delete s;
        }
        this->clear();
        _stations->clear();
        delete _stations;
    }
////////////////////////////////////////////////////////////////////////////////
    StationsList& SourceList::searchStation(const std::string& station){
        _stations->clear();
        for(unsigned i = 0; i < this->size(); i++){
            StationsList st = this->at(i)->search(station);
            for(unsigned j = 0; j < st.size(); j++)
                _stations->push_back(st.at(j));
        }
        return *_stations;
    }
////////////////////////////////////////////////////////////////////////////////
    Source* SourceList::source_by_name(const std::string& source_name){
        for(unsigned i = 0; i < this->size(); i++){
            if(this->at(i)->name() == source_name)
                return this->at(i);
        }
        return NULL;
    }
} // namespace Core
