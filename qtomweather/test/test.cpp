#include <iostream>
#include "core.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
Core::Data *
create_and_fill_class_data()
{
    Core::Data *wdata = new Core::Data;
    wdata->SetStartTime(time(NULL) - 3600);
    wdata->SetEndTime(time(NULL) + 3600);
    wdata->SetTemperature(23.0);
    return wdata;
}
//////////////////////////////////////////////////////////////////////////////
int
test_class_data()
{
    Core::Data *wdata = create_and_fill_class_data();
    delete wdata;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////
int
test_class_datalist(){
    Core::DataList *wdata_list = new Core::DataList;
    Core::Data *wdata =  create_and_fill_class_data();
    wdata_list->AddData(wdata);
    /* Check size of array */
    if (wdata_list->Size() != 1)
        return -1;
    if ( wdata_list->GetDataForTime(time(NULL))->GetTemperature() != 23.0)
        return -2;
    delete wdata_list;
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
}
