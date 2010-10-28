#include <iostream>
#include "core.h"

int
test_class_data()
{
    Core::Data *wdata = new Core::Data;
    delete wdata;
    return 0;
}

int main(int argc, char *argv[])
{
  std::cout<<"Testing..."<<std::endl;
  std::cout<<"First test 1. Class Data   ";
  if (test_class_data() == 0)
      std::cout<<"Success"<<std::endl;
  else
      std::cout<<"Not Success"<<std::endl;
}
