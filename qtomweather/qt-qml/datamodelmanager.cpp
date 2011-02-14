#include "datamodelmanager.h"

DataModelManager::DataModelManager(ConfigQml *c)
{
    config = c;
    DataItem *forecast_data = NULL;

    Core::DataParser* dp = NULL;
    Core::Data *temp_data = NULL;
    int i;

    try{
       if (config->current_station_id() != INT_MAX && config->stationsList().at(config->current_station_id()))
           dp = new Core::DataParser(config->stationsList().at(config->current_station_id())->fileName(),
                                     Core::AbstractConfig::sharePath+Core::AbstractConfig::schemaPath+"data.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        dp = new Core::DataParser();
        //return -1;
    }
    catch(const char *str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        dp = new Core::DataParser();
        //return -1;
    }

    model = new DataModel(new DataItem);
    i = 0;
    while  (temp_data = dp->data().GetDataForTime(time(NULL) + i)) {
        i = i + 3600*24;
        forecast_data = new DataItem(temp_data);
        forecast_data->Text(forecast_data->Text().c_str());
        model->appendRow(forecast_data);

    }
}
