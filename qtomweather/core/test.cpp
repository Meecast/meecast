#include <iostream>

#include "data.h"
using namespace std;

int
test_class_data()
{
    Core::Data wdata;
    delete wdata;
    return 0;
}

int main(int argc, char *argv[])
{
  cout<<"Testing..."<<endl;
  cout<<"First test 1. Class Data   ";
  if (test_class_data() == 0)
      cout<<"Success"<<endl;
  else
      cout<<"Not Success"<<endl;
}
