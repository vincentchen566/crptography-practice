#include <iostream>
#include <bitset>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

string longToString(unsigned long message);
void stringToChar(string message, uint8_t * messageArray);
void encrypt(uint8_t * messageArray, uint32_t * keyArray);