// is_heap example
#include <iostream>     // std::cout
#include <algorithm>    // std::is_heap, std::make_heap, std::pop_heap
#include <vector>       // std::vector

int main () {
  std::vector<int> foo {9,8,7,6,5,4,3,2,1};

  if (!std::is_heap(foo.begin(),foo.end()))
  {
  std::cout << "make heap" << std::endl;
    std::make_heap(foo.begin(),foo.end());
  }

  std::cout << "Popping out elements:";
  while (!foo.empty()) {
    std::pop_heap(foo.begin(),foo.end());   // moves largest element to back
    std::cout << ' ' << foo.back();         // prints back
    foo.pop_back();                         // pops element out of container
  }
  std::cout << '\n';

  return 0;
}

