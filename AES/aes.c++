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

    //initialize keyschedule
    uint32_t keySchedule[60];
    initializeKeyschedule(keySchedule, keyArray, sbox);


    //initialize galois multiplication tables
    uint8_t galoisBy2 [256] = 
        {0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
        0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
        0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
        0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,	
        0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
        0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
        0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
        0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
        0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
        0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
        0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
        0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
        0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
        0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
        0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
        0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5};

    uint8_t galoisBy3 [256] = 
        {0x00,0x03,0x06,0x05,0x0c,0x0f,0x0a,0x09,0x18,0x1b,0x1e,0x1d,0x14,0x17,0x12,0x11,
        0x30,0x33,0x36,0x35,0x3c,0x3f,0x3a,0x39,0x28,0x2b,0x2e,0x2d,0x24,0x27,0x22,0x21,
        0x60,0x63,0x66,0x65,0x6c,0x6f,0x6a,0x69,0x78,0x7b,0x7e,0x7d,0x74,0x77,0x72,0x71,
        0x50,0x53,0x56,0x55,0x5c,0x5f,0x5a,0x59,0x48,0x4b,0x4e,0x4d,0x44,0x47,0x42,0x41,
        0xc0,0xc3,0xc6,0xc5,0xcc,0xcf,0xca,0xc9,0xd8,0xdb,0xde,0xdd,0xd4,0xd7,0xd2,0xd1,
        0xf0,0xf3,0xf6,0xf5,0xfc,0xff,0xfa,0xf9,0xe8,0xeb,0xee,0xed,0xe4,0xe7,0xe2,0xe1,
        0xa0,0xa3,0xa6,0xa5,0xac,0xaf,0xaa,0xa9,0xb8,0xbb,0xbe,0xbd,0xb4,0xb7,0xb2,0xb1,
        0x90,0x93,0x96,0x95,0x9c,0x9f,0x9a,0x99,0x88,0x8b,0x8e,0x8d,0x84,0x87,0x82,0x81,	
        0x9b,0x98,0x9d,0x9e,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8f,0x8c,0x89,0x8a,
        0xab,0xa8,0xad,0xae,0xa7,0xa4,0xa1,0xa2,0xb3,0xb0,0xb5,0xb6,0xbf,0xbc,0xb9,0xba,
        0xfb,0xf8,0xfd,0xfe,0xf7,0xf4,0xf1,0xf2,0xe3,0xe0,0xe5,0xe6,0xef,0xec,0xe9,0xea,	
        0xcb,0xc8,0xcd,0xce,0xc7,0xc4,0xc1,0xc2,0xd3,0xd0,0xd5,0xd6,0xdf,0xdc,0xd9,0xda,	
        0x5b,0x58,0x5d,0x5e,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4f,0x4c,0x49,0x4a,
        0x6b,0x68,0x6d,0x6e,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7f,0x7c,0x79,0x7a,	
        0x3b,0x38,0x3d,0x3e,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2f,0x2c,0x29,0x2a,
        0x0b,0x08,0x0d,0x0e,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1f,0x1c,0x19,0x1a};


    //xor messageArray with first key
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            messageArray[4*i + j] = messageArray[4*i + j] ^ ((keySchedule[i] >> 8*(3-j)) & 0xff);
        }
    }
    
    //14 rounds of encryption
    for (int i=0; i<14; i++){

        //substitute bytes
        for (int j = 0; j < 16; j++){
            messageArray[j] = sbox[messageArray[j]];
        }

        //shift rows
        shuffle(messageArray);

        //mix columns
        columnEncrypt(messageArray, galoisBy2, galoisBy3);
        
        //add round key
        for (int j=0; j<4; j++){
            for (int k=0; k<4; k++){
                messageArray[4*j + k] = messageArray[4*j + 1] ^ (keySchedule[4*i+j] >> 8*(3-k));//key
            }
        }

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
void initializeKeyschedule(uint32_t * keySchedule, uint32_t * key, uint8_t * sbox){

    uint32_t roundConstant[8] = {0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000};

    for (int i=0; i<8; i++){
        keySchedule[i] = key[i];
    }

    for (int i=8; i<60; i++){
        if (i%8 == 0){
            keySchedule[i] = keySchedule[i-8] ^ sub32Bit(rotateWord(keySchedule[i-1]),sbox) ^ roundConstant[i/8];
        } if (i%8 == 4){
            keySchedule[i] = keySchedule[i-8] ^ sub32Bit(keySchedule[i-1], sbox);
        } else{
            keySchedule[i] = keySchedule[i-8] ^ keySchedule[i-1];
        }
    }
}

//use sbox to change out 32-bit word
uint32_t sub32Bit(uint32_t subWord, uint8_t * sbox){
    uint8_t word[4];
    for (int i=0; i<4; i++){
        word[i] = (subWord >> 8*(3-i)) & 0xff;
        word[i] = sbox[word[i]];
    }

    uint32_t toReturn = 0;
    for (int i=0; i<4; i++){
        toReturn << 8;
        toReturn += word[i];
    }
    return toReturn;
}

//right rotate 32-bit word 1 byte
uint32_t rotateWord(uint32_t toRotate){
    return (toRotate << 8) + (toRotate >> 24);
}


//shift rows of message
void shuffle(uint8_t * message){
    int temp;
    //second row
    temp = message[1];
    message[1] = message[5];
    message[5] = message[9];
    message[9] = message[13];
    message[13] = temp;

    //third row
    temp = message[2];
    message[2] = message[10];
    message[10] = temp;

    temp = message[6];
    message[6] = message[14];
    message[14] = temp;

    //fourth row
    temp = message[3];
    message[3] = message[15];
    message[15] = message[11];
    message[11] = message[7];
    message[7] = temp;
}

void columnEncrypt(uint8_t * messageArray, uint8_t * galoisBy2, uint8_t * galoisBy3){
    
    uint8_t originalMessage [16];
    for (int i=0; i<16; i++){
        originalMessage[i] = messageArray[i];
    }


    for (int i=0; i<4; i++){
        messageArray[4*i] =   galoisBy2[originalMessage[4*i]]   ^ galoisBy3[originalMessage[4*i + 1]]   ^ originalMessage[4*i + 2]              ^ originalMessage[4*i + 3];
        messageArray[4*i+1] = originalMessage[4*i]              ^ galoisBy2[originalMessage[4*i + 1]]   ^ galoisBy3[originalMessage[4*i + 2]]   ^ originalMessage[4*i + 3];
        messageArray[4*i+2] = originalMessage[4*i]              ^ originalMessage[4*i + 1]              ^ galoisBy2[originalMessage[4*i + 2]]   ^ galoisBy3[originalMessage[4*i + 3]];
        messageArray[4*i+3] = galoisBy3[originalMessage[4*i]]   ^ originalMessage[4*i + 1]              ^ originalMessage[4*i + 2]              ^ galoisBy2[originalMessage[4*i + 3]];
        
    }


}