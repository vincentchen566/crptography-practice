#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

string sha256hash(string message);
unsigned long rightRotate(unsigned long toRotate, int n);
unsigned int rightRotate(unsigned int toRotate, int n);
int main();
uint8_t stringToChar(string binaryString);

string encrypt(uint8_t * messageArray, uint32_t * keyArray);


class GeneratorState{
    public:
        uint32_t key [8];
        int counter;

        GeneratorState(){
            for (int i=0; i<8; i++){
                key[i] = 0;
            }
            counter = 0 ;
        }
        
        GeneratorState(uint32_t * setKey, int setCounter){
            for (int i=0; i<8; i++){
                key[i] = setKey[i];
            }
            counter = setCounter;
        }
};


class Generator{
    public:
        GeneratorState state;
 //initialize generator
        Generator(){
            state = GeneratorState();
        }
    
        void reseed(string seed);
        string generateBlock(unsigned int length);
        string generateData(unsigned int wantedLength);
};
