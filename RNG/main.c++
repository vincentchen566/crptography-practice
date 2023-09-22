//compile with sha.c++ in sha-256 folder


#include <iostream>
#include "rng.h++"
using namespace std;

int main(){

    //change to hash different message
    string seed = "a really good seed";
    Generator generator;
    generator.reseed(seed);
    //cout << generator.generateBlock(2);


    return 0;
}