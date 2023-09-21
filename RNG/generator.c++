#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "rng.h++"
using namespace std;

class GeneratorState{
    public:
        unsigned long key;
        int counter;

        GeneratorState(){
            key = 0;
            counter = 0 ;
        }
        
        GeneratorState(unsigned long setKey, int setCounter){
            key = setKey;
            counter = setCounter;
        }
};


class Generator{
    public:
        GeneratorState state;

        //initialize generator
        Generator(){
            state.key = 0;
            state.counter = 0;
        }

        //generates new seed and increments by counter to indicate seeded
        void reseed(string seed){
            stringstream res1, res2;
            res1 << hex << uppercase << state.key;
            res2 << hex << uppercase << seed;

            stringstream processHash(sha256hash(res1.str() + res2.str()));
            processHash >> hex >> state.key;

            state.counter += 1;
        }


        //generate psuedorandom string of 16k bytes
        string generateBlock(Generator generator, unsigned int length){
            if (generator.state.counter == 0){
                return "ERROR - COUNTER IS ZERO";
            }

            string randString = "";

            for (int i=0; i<k; i++){
                r += encrypt(generator.state.key, generator.state.counter);
                generator.state.counter += 1;
            }

            return randString;

        }
        
        string generateData(Generator generator, unsigned int wantedLength){
            if (wantedLength > pow(2,20)){
                return "ERROR - TOO LONG";
            }

            string r = generateBlock(generator, ceil((double) wantedLength/16));
            while (r.length() > wantedLength){
                r = r.substr(0, r.length()-1);
            }


            //change keys
            stringstream processKey(generateBlock(generator, 2));
            processKey >> hex >> generator.state.key;
            
            return r;
        }

};

GeneratorState initialize(Generator generator){
    generator.state.key = 0;
    generator.state.counter = 0;
    return generator.state;
}

