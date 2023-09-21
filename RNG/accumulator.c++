#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "rng.h++"
using namespace std;

//generates seed for generator initialization
class Accumulator{
    public:
        string getSeed(){
            return "Totally random I promise"; //implement later
        }
};