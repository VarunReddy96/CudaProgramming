#include <iostream>
#include <algorithm>

#define ciphermessagescount = 100;

__managed__ unsigned int messagenum;

using namespace std;

//kernal function

__global__
void breakingrsa(unsigned long long ciphertext,unsigned long long int n,unsigned long long *messagelist){
    int threadpos = blockIdx.x * blockDim.x +threadIdx.x;
    int totalthreads = blockDIm.x * gridDim.x;
    for (unsigned long long i = threadpos; i < n; i+=totalthreads) {
        unsigned long long val =1 ;
        for(int j =0;j<3;j++) {
            val = ((val * i) % modulus);
        }
        if((val==ciphertext && messagenum < ciphermessagescount){
            messagelist[atomicAdd(&messagenum, 1)] = val;
        }

    }
}

int main(int argc, char **argv) {

    if (argc < 3) {
        // Tell the user how to run the program
        std::cerr << "USAGE: " << argv[0] << " ciphertext" << " " <<argv[1]<< " modulus" << argv[2] << std::endl;
        return 1;
    }

    unsigned long long modulus = stoull(argv[2]);
    unsigned long long cipher = stoull(argv[1]);
    unsigned long long int *messagelist;

    //cout << "val of cipher is " <<cipher;
    //  cout << "Hello, World!" << endl;
// to read the values use cin
    //int age;
    //cin >> age;
    cudaMallocManaged(&messagelist,ciphermessagescount*sizeof(unsigned long long));
    messagenum = 0;

    int threadsperblock = 256;
    int totalblocks = (modulus+threadsperblock-1) / threadsperblock;
    breakingrsa<<< totalblocks,threadsperblock >>>(cipher,modulus,messagelist);

    cudaDeviceSynchronize();

    if (messagenum == 0){
        cout << "No cube roots of " << cipher << " (mod " << argv[2] << ")";
    }else{
        int num = sizeof(messagelist)/sizeof(messagelist[0]);
        sort(messagelist,messagelist+num);
        for(int i = 0; i <messagenum;i++){
            cout << messagelist[i] <<"^3 = " << cipher << " (mod " << argv[2] << ")" << endl;
        }
    }

    cudaFree(messagelist);
    return 0;
}

