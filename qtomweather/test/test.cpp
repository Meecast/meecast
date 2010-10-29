#include <iostream>
#include "core.h"
//////////////////////////////////////////////////////////////////////////////
Core::Data *
create_and_fill_class_data()
{
    Core::Data *wdata = new Core::Data;
    wdata->SetStartTime(time(NULL) - 3600);
    wdata->SetEndTime(time(NULL) + 3600);
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
test_class_datalist()
{
    Core::DataList *wdata_list = new Core::DataList;
    Core::Data *wdata =  create_and_fill_class_data();
    wdata_list->AddData(wdata);
    /* Check size of array */
    if (wdata_list->Size() != 1)
        return -1;
    delete wdata_list;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  int i = 1; /* Number of test */

  std::cout<<"Testing..."<<std::endl;
  std::cout<<"Test " << i << " Class Data   ";
  if (test_class_data() == 0)
      std::cout<<"Success"<<std::endl;
  else
      std::cout<<"Not Success"<<std::endl;

  i++;

  std::cout<<"Test "<< i << " Class DataList   ";
  if (test_class_datalist() == 0)
      std::cout<<"Success"<<std::endl;
  else
      std::cout<<"Not Success"<<std::endl;
}
