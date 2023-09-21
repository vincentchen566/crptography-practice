//compile with sha.c++ in sha-256 folder


#include <iostream>
#include "rng.h++"
using namespace std;

int main(){
    
    //change to hash different message
    cout << sha256hash("hello");

    return 0;
}