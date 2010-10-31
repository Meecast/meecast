#include "datalist.h"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    DataList::DataList(){
    }
////////////////////////////////////////////////////////////////////////////////
    DataList::~DataList(){
    }
////////////////////////////////////////////////////////////////////////////////
    void
    DataList::AddData(Data *data){
        data_array.push_back(data);
    }
///////////////////////////////////////////////////////////////////////////////
     int
     DataList::Size(){
        return data_array.size();
     }
///////////////////////////////////////////////////////////////////////////////
     Data*
     DataList::GetDataForTime(time_t _time){
         std::vector<Data*>::const_iterator next_it, it;
         time_t temp_time = -1, result_time;
         Data* temp_data;
         Data* temp_data_result = NULL;
         for(it=this->data_array.begin(); it!=this->data_array.end(); ++it) {
            ++(next_it = it);
            temp_data = *it;

            result_time = temp_data->GetTimeDistance(_time);
            if (temp_time == -1)
                temp_time = result_time;
            if ((result_time >0) && result_time <= temp_time){
                temp_time = result_time;
                temp_data_result = temp_data;
            }
         }
         return temp_data_result;
     }

////////////////////////////////////////////////////////////////////////////////
} // namespace Core
