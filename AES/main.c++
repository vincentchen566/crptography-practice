#include <iostream>
#include <bitset>
#include "aes.h++"
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

int main(){
    //convert message to array of 16 bytes
    uint8_t messageArray [16];
    stringToChar("hello", messageArray);
    
    //set initial 256-bit key
    uint32_t keyArray [4] = {0x1111, 0x2222, 0x3333, 0x4444}; //really good key

    cout << encrypt(messageArray, keyArray) <<endl;
    cout << decrypt(messageArray, keyArray) << endl;


    

   

}



