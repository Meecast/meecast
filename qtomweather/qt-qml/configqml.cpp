#include "configqml.h"

//ConfigQml::ConfigQml():QObject(),Core::Config("config.xml", "../core/data/config.xsd"){}
ConfigQml::ConfigQml(const std::string& filename, const std::string& schema_filename):QObject(),Core::Config(filename, schema_filename)
{

}

ConfigQml::ConfigQml():QObject(),Core::Config(){}

QString
ConfigQml::iconset(){
    QString c;
    c = ConfigQml::Config::iconSet().c_str();
    return c;
}

QString
ConfigQml::iconspath(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("/icons");
    return c;
}

QString
ConfigQml::iconsbutton(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("/button_icons");
    return c;
}

QString
ConfigQml::temperatureunit(){
    QString c;
    c = ConfigQml::Config::TemperatureUnit().c_str();
    return c;
}
QColor
ConfigQml::fontcolor(){
    QColor c;
    c.setNamedColor(ConfigQml::Config::FontColor().c_str());
    return c;
}
QString
ConfigQml::stationname()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->name().c_str();
    else
        return "Unknown";
}
void ConfigQml::changestation()
{
    if (this->current_station_id() + 1 < this->stationsList().size())
       this->current_station_id(this->current_station_id() + 1);
    else
       this->current_station_id(0);


}
void
ConfigQml::refreshconfig(){
    emit ConfigQml::iconsetChanged();
    emit ConfigQml::iconspathChanged();
    emit ConfigQml::temperatureunitChanged();
    emit ConfigQml::fontcolorChanged();
    emit ConfigQml::stationnameChanged();
}


DataModel*
ConfigQml::getModel()
{
    DataModel *model = new DataModel(new DataItem);
    DataItem *forecast_data = NULL;

    int i = 0;
    Core::DataParser* dp = NULL;
    Core::Data *temp_data = NULL;
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
            &&  this->stationsList().at(this->current_station_id())){
        try{
            dp = new Core::DataParser(this->stationsList().at(this->current_station_id())->fileName(),
                                      Core::AbstractConfig::sharePath+Core::AbstractConfig::schemaPath+"data.xsd");
        }
        catch(const std::string &str){
            std::cerr<<"Error in DataParser class: "<< str <<std::endl;
            //return NULL;
        }
        catch(const char *str){
            std::cerr<<"Error in DataParser class: "<< str <<std::endl;
            //return NULL;
        }
    }

    while  (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL) + i))) {
        i = i + 3600*24;
        forecast_data = new DataItem(temp_data);
        forecast_data->Text(forecast_data->Text().c_str());
        model->appendRow(forecast_data);

    }
    return model;
}
