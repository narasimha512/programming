#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

int main(int argc, char* argv[])
{
cout << "size of " << sizeof(unsigned long long) << endl;
int theMinActiveConnections = atoi(argv[1]);
int numOfRDSinPG = atoi(argv[2]);
float tmpConnections = (theMinActiveConnections * 125);
cout << "tmpConnections " << tmpConnections << endl;
tmpConnections = ceil(tmpConnections / 100);
cout << "tmpConnections " << tmpConnections << endl;
cout << "numOfRDSinPG " << numOfRDSinPG << endl;
int numOfParallel = ceil(tmpConnections / numOfRDSinPG);
cout << "numOfParallel " << numOfParallel << endl;
}
