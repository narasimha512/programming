#include<iostream>
#include<memory>
using namespace std;

class SmartPointer
{
int a;
public:
SmartPointer(int x)
{
a=x;
}
void print()
{
 cout << "a is " << a << endl;
}
};

int main()
{

unique_ptr<int*> p = new int;

auto p1 = std::unique_ptr<SmartPointer>(new SmartPointer(4));
p1->print();

//auto p2 =p1; //unique_ptr(const unique_ptr&) = delete; error: use of deleted function
	
}
