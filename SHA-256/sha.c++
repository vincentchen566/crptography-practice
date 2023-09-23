//implemented following tutorial at https://blog.boot.dev/cryptography/how-sha-2-works-step-by-step-sha-256/


#include <iostream>
#include <bitset>
#include "sha.h++"
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

string sha256hash(string message){


    //find length of message and padding
    int messageBytes = message.length()+1;
    int paddingBytes = 0;

    while ((paddingBytes + messageBytes) % 64 != 56){
        paddingBytes += 1;
    }

    int totalBytes = messageBytes + paddingBytes + 8;


    //create array with message
    uint8_t * messageArray = new uint8_t[totalBytes];

    for (int i=0; i<message.length(); i++){
        messageArray[i] = message[i];
    }
    //add padding
    messageArray[message.length()] = (uint8_t) (0b10000000);
    for (int i=1; i<=paddingBytes; i++){
        messageArray[message.length()+i] = (uint8_t) (0b00000000);
    }
    

    

    //add length of message (bits) to end 64 bits big-endian
    string messageLengthBinary = bitset<64>(8*message.length()).to_string();
   

    for (int i=0; i<8; i++){
        uint8_t lengthToAdd = stringToChar(messageLengthBinary.substr(8*i, 8));
        messageArray[totalBytes-8+i] = lengthToAdd;
    }


    //assigning constants for hashing
    uint32_t hashValues [8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0x3c6ef372, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    uint32_t roundConstants [64] =
        {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};


    //sorting into 512-bit chunks
    uint32_t chunkNum = totalBytes/64;

    //hashing per chunk
    for (int i=0; i<chunkNum; i++){

        //turn each 512-bit chunk into sets of 16 32-bit words with 48 words of padding at end
        uint32_t chunkToHash[64];
        for (int j = 0; j<16; j++){

            uint32_t toAdd = 0;
            toAdd += messageArray[64*i + j*4];
            toAdd << 8;
            toAdd += messageArray[64*i + j*4 + 1];
            toAdd << 8;
            toAdd += messageArray[64*i + j*4 + 2];
            toAdd << 8;
            toAdd += messageArray[64*i + j*4 + 3];
            chunkToHash[j] = toAdd;

        }


        //fill out rest of chunk
        for (int j = 16; j<64; j++){
            uint32_t s0 = rightRotate(chunkToHash[j-15], 7) ^ rightRotate(chunkToHash[j-15], 18) ^ (chunkToHash[j-15]>>3);
            uint32_t s1 = rightRotate(chunkToHash[j-2], 17) ^ rightRotate(chunkToHash[j-19], 18) ^ (chunkToHash[j-2]>>10);
            chunkToHash[j] = (uint32_t) (chunkToHash[j-16] + s0 + chunkToHash[j-7] + s1); //ignore overflow, calculate mod 2^32
        }

        //COMPRESSION - changing hash values

        //initializing variables
        uint32_t a = hashValues[0];
        uint32_t b = hashValues[1];
        uint32_t c = hashValues[2];
        uint32_t d = hashValues[3];
        uint32_t e = hashValues[4];
        uint32_t f = hashValues[5];
        uint32_t g = hashValues[6];
        uint32_t h = hashValues[7];

        for (int j=0; j<64; j++){
            uint32_t r1 = rightRotate(e,6) ^ rightRotate(e, 11) ^ rightRotate(e,25);
            uint32_t ch = (e & f) ^ ((~e) ^ g);
            uint32_t temp1 = h + r1 + ch + roundConstants[j] + chunkToHash[j];
            uint32_t r0 = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a,22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = r0 + maj;
            h=g;
            g=f;
            f=e;
            e=d+temp1;
            d=c;
            c=b;
            b=a;
            a=temp1 + temp2;

        }

        hashValues[0] = hashValues[0] + a;
        hashValues[1] = hashValues[1] + b;
        hashValues[2] = hashValues[2] + c;
        hashValues[3] = hashValues[3] + d;
        hashValues[4] = hashValues[4] + e;
        hashValues[5] = hashValues[5] + f;
        hashValues[6] = hashValues[6] + g;
        hashValues[7] = hashValues[7] + h;

    }

    //concatenate to get final hash
    string finalHash;

    for (int i=0; i<8; i++){
        string convertHash = bitset<32> (hashValues[i]).to_string();
        for (int j=0; j<4; j++){
            bitset<8> set(convertHash.substr(8*j, 8));
            stringstream res;
            res << hex << uppercase << set.to_ulong();
            string addToHash = res.str();
            while (addToHash.length() < 2){
                addToHash = "0" + addToHash;
            }
            finalHash += addToHash;
        }
        
      
    }

    return finalHash;
}

uint32_t rightRotate(uint32_t toRotate, int n) {
    return (toRotate >> n)|(toRotate << (32 - n));
}

uint8_t stringToChar(string binaryString){
    uint8_t value = 0x00;
    for (int i=0; i<binaryString.length(); i++){
        if (binaryString.substr(i,1)=="0"){
            value*=2;
        } else if (binaryString.substr(i,1)=="1"){
            value*=2;
            value+=1;
        }
    }
    return value;
}