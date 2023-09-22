#include <iostream>
#include "sha.h++"
using namespace std;

int main(){
    
    
    //change to hash different message
    cout << sha256hash("a really good seed");

    return 0;
}