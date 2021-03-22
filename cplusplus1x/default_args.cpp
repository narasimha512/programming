int sum(char* x=nullptr, int x, int y, int z=0, int w=0) 
{ 
    return (x + y + z + w); 
} 
  
/* Driver program to test above function*/
int main() 
{ 
    cout << sum(10, 15) << endl; 
    cout << sum(10, 15, 25) << endl; 
    cout << sum(10, 15, 25, 30) << endl; 
    return 0; 
}
