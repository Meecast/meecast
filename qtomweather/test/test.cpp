
#include <iostream>
#include "core.h"
#include <stdlib.h>
#include <limits.h>

//////////////////////////////////////////////////////////////////////////////
Core::Data *
create_and_fill_class_data_for_hours_forecast()
{
    Core::Data *wdata = new Core::Data;
    wdata->StartTime(time(NULL) - 3600);
    wdata->EndTime(time(NULL) + 3600);
    wdata->temperature_hi().value(23.0);
    wdata->temperature_low().value(15.0);
    wdata->Flike().value(18.0);
    wdata->WindSpeed(3.0);
    wdata->WindGust(4.0);
    wdata->WindDirection(std::string("NNW"));
    wdata->Icon(3);
    wdata->Text(std::string("Clear"));
    return wdata;
}
//////////////////////////////////////////////////////////////////////////////
Core::Data *
create_and_fill_class_data_for_day_forecast()
{
    time_t      current_time, begin_current_day, end_current_day;
    struct tm   *tm = NULL;
    int         year, current_month;

    Core::Data *wdata = new Core::Data;
    /* Create current day */
    current_time = time(NULL);
    begin_current_day = current_time;
    tm = localtime(&begin_current_day);
    year = 1900 + tm->tm_year;
    current_month = tm->tm_mon;
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    begin_current_day = mktime(tm);
    tm->tm_sec = 0; tm->tm_min = 59; tm->tm_hour = 23;
    end_current_day = mktime(tm);
    wdata->StartTime(begin_current_day);
    wdata->EndTime(end_current_day);
    wdata->temperature_low().value(20.0);
    wdata->temperature_hi().value(10.0);
    wdata->Flike().value(17.0);
    wdata->WindGust(2.8);
    wdata->WindSpeed(2.4);
    wdata->Humidity(845);
    wdata->Icon(4);
    wdata->Text(std::string("Cloudy"));
    return wdata;
}
//////////////////////////////////////////////////////////////////////////////
int
test_class_data()
{
    Core::Data *wdata = create_and_fill_class_data_for_day_forecast();
    delete wdata;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////
int
test_class_datalist(){

    Core::DataList *wdata_list = new Core::DataList;

    Core::Data *wdata = create_and_fill_class_data_for_hours_forecast();
    wdata_list->push_back(wdata);
    wdata =  create_and_fill_class_data_for_day_forecast();
    wdata_list->push_back(wdata);
    /* Check size of array */
    if (wdata_list->size() != 2)
        return -1;
    delete wdata_list;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////
int
test_class_source()
{
    Core::Source *wsource = new Core::Source() ;
    delete wsource;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////
int
test_class_dataparser()
{
    Core::DataParser *p = new Core::DataParser();
    if (!p->parse("data.xml")){
        delete p;
        Core::DataParser *p = new Core::DataParser();
        if (!p->parse("data.xml")){
            delete p;
            return -1;
        }
    }

    delete p;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////
Core::Config *
create_and_fill_config(){
    Core::Config *c = new Core::Config();
    c->Base_Icons_Path(std::string("../omweather/data/icons"));
    c->Iconset(std::string("Glance"));
    c->TemperatureUnit("F");
    return c;
}

//////////////////////////////////////////////////////////////////////////////
int
test_class_config(){
    Core::Config *c = create_and_fill_config();
    if (!(
         ((c->Base_Icons_Path().compare("../omweather/data/icons") == 0)&&
         (c->Iconset().compare("Glance")) == 0)&&
         (c->TemperatureUnit().compare("F")== 0)
        )){
        delete c;
        return -1;
    }

    delete c;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////
void
goto_from_program(int result){
    std::cout<<"Not Success Code: "<< result << std::endl;
    exit(result);
}

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){
  int i = 1; /* Number of test */
  int result = 0;

  std::cout<<"Testing..."<<std::endl;

  std::cout<<"Test "<< i << " Class Config   ";
  result = test_class_config();
  if ( result == 0)
      std::cout<<"Success"<<std::endl;
  else
      goto_from_program(result);
  i++;

  std::cout<<"Test " << i << " Class Data   ";
  result = test_class_data();
  if (result == 0)
      std::cout<<"Success"<<std::endl;
  else
      goto_from_program(result);

  i++;

  std::cout<<"Test "<< i << " Class DataList   ";
  result = test_class_datalist();
  if ( result == 0)
      std::cout<<"Success"<<std::endl;
  else
      goto_from_program(result);

  i++;

  std::cout<<"Test "<< i << " Class Source   ";
  result = test_class_source();
  if ( result == 0)
      std::cout<<"Success"<<std::endl;
  else
      goto_from_program(result);
  i++;

  std::cout<<"Test "<< i << " Class DataParser   ";
  result = test_class_dataparser();
  if ( result == 0)
      std::cout<<"Success"<<std::endl;
  else
      goto_from_program(result);


}
