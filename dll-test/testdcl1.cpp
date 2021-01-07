#include <iostream>
#include "shape.hh"
#include <map> 
#include <list> 
#include <vector>
#include <string> 
#include <dlfcn.h> 
#include <stdio.h> 
#include <unistd.h> 

using namespace std;

int main()
{

void *hndl = dlopen("libnewshapes.so", RTLD_NOW);
if(hndl == NULL){
   cerr << dlerror() << endl;
}
void *mkr = dlsym(hndl, "maker");
shape *my_shape = reinterpret_cast<maker_t*>(mkr)();
my_shape->draw();

}
