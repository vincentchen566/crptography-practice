#include <iostream>
#include "sha.h++"
using namespace std;

int main(){
    
    //change to hash different message
    cout << sha256hash("hello");

    return 0;
}