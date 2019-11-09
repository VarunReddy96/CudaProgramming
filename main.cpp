#include <iostream>

#define ciphermessagescount = 100;

__managed__ unsigned int messagenum;

using namespace std;

int main(int argc, char **argv) {

    if (argc < 3) {
        // Tell the user how to run the program
        std::cerr << "Cipher: " << argv[0] << " modulus" << std::endl;
        return 1;
    }



    unsigned long long modulus = stoull(argv[2]);
    int cipher = stoi(argv[1]);


    //cout << "val of cipher is " <<cipher;
    //  cout << "Hello, World!" << endl;
// to read the values use cin
    //int age;
    //cin >> age;


    int count = 0;
    for (unsigned long long i = 0; i < modulus; i++) {
        unsigned long long val =1 ;
        for(int j =0;j<3;j++) {
            val = ((val * i) % modulus);
        }
        if((int)val==cipher){
            cout << i <<"^3 = " << cipher << " (mod " << argv[2] << ")" << endl;
            count++;
        }


    }
    if(count==0){
        cout << "No cube roots of " << cipher << " (mod " << argv[2] << ")";
    }


    return 0;
}