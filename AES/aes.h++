#include <iostream>
#include <bitset>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

string longToString(unsigned long message);
void stringToChar(string message, uint8_t * messageArray);
string encrypt(uint8_t * messageArray, uint32_t * keyArray);
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
void initializeSbox(uint8_t * sbox);
void initializeKeyschedule(uint32_t * keySchedule, uint32_t * key, uint8_t * sbox);
uint32_t sub32Bit(uint32_t subWord, uint8_t * sbox);
uint32_t rotateWord(uint32_t toRotate);
void shuffle(uint8_t * message);  
void columnEncrypt(uint8_t * messageArray, uint8_t * galoisBy2, uint8_t * galoisBy3);
string decrypt(uint8_t * messageArray, uint32_t * keyArray);
void initializeInvSbox(uint8_t * invsbox);
void reverseKey(uint32_t * keySchedule);
void columnDecrypt(uint8_t * messageArray, uint8_t * galoisBy9, uint8_t * galoisBy11, uint8_t * galoisBy13, uint8_t * galoisBy14);
void invShuffle(uint8_t * message);