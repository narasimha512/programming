#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(int i)
{
    cout << "i is " << i << endl;
}

int main()
{
vector<int> v={1,2,3,4,5};

int uni_init{10};

cout << "uniform initialized uni_init is " << uni_init << endl;

for(int i:v)
{
    cout << "i is " << i << endl;
}

for_each(v.begin(), v.end(), print);
for_each(begin(v), end(v), print);

for_each(begin(v), end(v), [](int i){ cout << "in Lamda: i is " << i << endl; });
 
}
