#include <iostream>
#include <bitset>
#include "aes.h++"
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

int main(){
    //convert message to array of 16 bytes
    uint8_t messageArray [16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    stringToChar("hello           ", messageArray);
    
    //set initial 256-bit key
    uint32_t keyArray [8] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555,0x66666666,0x77777777,0x88888888}; //really good key
    
    encrypt(messageArray, keyArray);
    printMessage(messageArray);
    decrypt(messageArray, keyArray);
    printMessage(messageArray);


    return 0;
}



