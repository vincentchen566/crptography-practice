#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

int main();
unsigned long encrypt(int message, int e, int n);
int decrypt(unsigned long encryptedMessage, int d, int n);
void extendedGCD(int a, int b, int * toReturn);