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
    cout << toHash << endl;
    string keyString = sha256hash(toHash);
    cout << keyString << endl;
    string specKeyString;
    
    for (int i=0; i<8; i++){
        specKeyString = keyString.substr(4*i, 4);
        state.key[i] = 0;
        //convert hash to uint32 array and store as key
        for (int j=0; j<4; j++){
            state.key[i] << 8;
            state.key[i] += (uint32_t) specKeyString[j];
        }

    }
    state.counter += 1;

    for (int i=0; i<8; i++){
        cout << state.key[0];
    }
    //for (int i=0; i<8; i++){
    //    cout << state.key[i] + " ";
    //}
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
            state.key[i] += (uint32_t) specKeyString[j];
        }

    }
    state.counter += 1;
    
    return r;
}

