#include <iostream>
#include <bitset>
#include "aes.h++"
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;


//convert long to string
string longToString(unsigned long message){
    stringstream stream;
    stream << hex << uppercase << message;
    return stream.str();
}

//convert string into byte array
void stringToChar(string message, uint8_t * messageArray){
    if (message.length() > 16){
        cout << "ERROR - message block must be 16 bytes or less";
        return;
    }
    
    for (int i=0; i<message.length(); i++){
        messageArray[i] = (uint8_t) message[i];
    }

    return;
}


//aes encryption
void encrypt(uint8_t * messageArray, uint32_t * keyArray){
    //initialize sbox
    uint8_t sbox [256];
    initializeSbox(sbox);

    uint32_t keySchedule[120];
    initializeKeyschedule(keySchedule, keyArray);

    //xor messageArray with first key
    


    //14 rounds of encryption
    for (int i=0; i<14; i++){
        for (int j = 0; j < 16; j++){
            messageArray[i] = sbox[messageArray[i]];
        }
        //substitute bytes

        //shift rows

        //mix columns

        //add round key
    }
}


//initialize sbox - copied from wikipedia
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

void initializeSbox(uint8_t * sbox){
    uint8_t p = 1, q = 1;

    /* loop invariant: p * q == 1 in the Galois field */
	do {
		/* multiply p by 3 */
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		/* divide q by 3 (equals multiplication by 0xf6) */
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		/* compute the affine transformation */
		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	/* 0 is a special case since it has no inverse */
	sbox[0] = 0x63;
}

//create key schedule
void initializeKeyschedule(uint32_t * keySchedule, uint32_t * key){
    for (int i=0; i<8; i++){
        keySchedule[i] = key[i];
    }

    for (int i=8; i<60; i++){
        if (i%8 == 0){
            keySchedule[i] = keySchedule[i-8] ^ ;
        } if (i%8 == 4){
            keySchedule[i] = keySchedule[i-8] ^ 
        } else{
            keySchedule[i] = keySchedule[i-8] ^ keySchedule[i-1];
        }
    }
}

uint32_t sub32Bit(uint32_t subWord){
    
}