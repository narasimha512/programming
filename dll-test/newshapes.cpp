#include <iostream> 
#include "circle.hh"
void circle::draw(){
   // simple ascii circle 
   cout << "\n"; 
   cout << "      ****\n";
   cout << "    *      *\n"; 
   cout << "   *        *\n"; 
   cout << "   *        *\n"; 
   cout << "   *        *\n"; 
   cout << "    *      *\n"; 
   cout << "      ****\n"; 
   cout << "\n";
}
extern "C" {
shape *maker(){
   return new circle;
}
}
