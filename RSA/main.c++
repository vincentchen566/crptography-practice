#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <iomanip>
#include "rsa.h++"


using namespace std;

int main(){

    //only for encrypting integers


    //large primes (wow so big)
    int p = 41;
    int q = 47;
    
    int n = p*q;
    int t = (p-1) * (q-1);

    //public exponent
    int e = 7;

    //private exponent
    int gcdReturn [3];
    extendedGCD(e, t, gcdReturn);
    int d = gcdReturn[1];
    cout << "Private key: " << d << endl;

    int message = 20;
    unsigned long encryptedMessage = encrypt(message, e, n);
    cout << "Encrypted message: " << encryptedMessage << endl;

    int decryptedMessage = decrypt(encryptedMessage, d, n);
    cout << "Decrypted message: " << decryptedMessage;

    return 0;
}

unsigned long encrypt(int message, int e, int n){
    int initialMessage = message;
    for (int i=1; i<e; i++){
        message *= initialMessage;
        message = message%n;
    }

    return message;
}

int decrypt(unsigned long encryptedMessage, int d, int n){
    unsigned long originalEncrypt = encryptedMessage;
    for (int i=1; i<d; i++){
        encryptedMessage *= originalEncrypt;
        encryptedMessage = encryptedMessage % n;
    }
    return (int) encryptedMessage;
}

//euclidean alg
void extendedGCD(int a, int b, int * toReturn){
    int c = a;
    int d = b;

    int u_c = 1;
    int v_c = 0;
    int u_d = 0;
    int v_d = 1;

    while (c!=0){
        int q = floor((double) d/c);
        int tempC = c;
        c = d-q*c;
        d = tempC;

        int tempu_c = u_c;
        int tempv_c = v_c;
        int tempu_d = u_d;
        int tempv_d = v_d;

        u_c = tempu_d - q*tempu_c;
        v_c = tempv_d - q*tempv_c;
        u_d = tempu_c;
        v_d = tempv_c;

    }

        toReturn[0] = d;
        toReturn[1] = u_d;
        toReturn[2] = v_d;
        return;




}