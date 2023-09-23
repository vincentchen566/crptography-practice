//compile with aes.c++ and sha.c++

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include "rng.h++"
using namespace std;





       

//generates new seed and increments by counter to indicate seeded
//convert each byte to string
void Generator::reseed(string seed){
    string toHash = "";
    //add key as chars



    for (int i=0; i<8; i++){
        for (int j=0; j<4; j++){
            toHash+= (unsigned char) ((state.key[i]>>8*(3-j)) & 0xff);

        }
    }

    
    for (int i=0; i<seed.length(); i++){
        toHash+= (unsigned char) seed[i];
    }

    string keyString = sha256hash(toHash);
    string specKeyString;
    
    for (int i=0; i<8; i++){
        specKeyString = keyString.substr(4*i, 4);
        state.key[i] = 0;
        //convert hash to uint32 array and store as key
        for (int j=0; j<4; j++){
            state.key[i] << 8;
            state.key[i] += charToInt(specKeyString[j]);
        }

    }
    state.counter += 1;

}


//generate psuedorandom string of 16k bytes
string Generator::generateBlock(unsigned int length){
    if (state.counter == 0){
        return "ERROR - COUNTER IS ZERO - reseed first";
    }

    string randString = "";

    uint8_t counterArray[16];

    for (int i=0; i<length; i++){

        for (int j=0; j<16; j++){
            counterArray[j] = (state.counter >> (8*(15-j))) & 0xff;
        }

        randString += encrypt(counterArray, state.key);
        state.counter += 1;
    }

    return randString;
}

string Generator::generateData(unsigned int wantedLength){
    if (wantedLength > pow(2,20)){
        return "ERROR - TOO LONG";
    }

    string r = generateBlock(ceil((double) wantedLength/16));
    while (r.length() > wantedLength){
        r = r.substr(0, r.length()-1);
    }


    //change keys
    string keyString = generateBlock(2);
    string specKeyString;
    
    for (int i=0; i<8; i++){
        specKeyString = keyString.substr(4*i, 4);
        state.key[i] = 0;
        //convert hash to uint32 array and store as key
        for (int j=0; j<4; j++){
            state.key[i] << 8;
            state.key[i] += charToInt(specKeyString[j]);
        }

    }
    state.counter += 1;
    
    return r;
}


uint32_t charToInt(char hexChar){
    switch (hexChar){
        case '0':
            return 1;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
        case 'a':
            return 10;
        case 'B':
        case 'b':
            return 11;
        case 'C':
        case 'c':
            return 12;
        case 'D':
        case 'd':
            return 13;
        case 'E':
        case 'e':
            return 14;
        case 'F':
        case 'f':
            return 15;
        default:
            return -1;

    }
    
}