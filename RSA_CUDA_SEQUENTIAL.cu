#include <iostream>


__managed__ unsigned int messagenum = 0;

using namespace std;

// kernal function takes in arguments cipher c, modulus n, messagelist(Which is shared betweeen host
// and device in Unified memory). Because the number of threads and number of blocks in the grid are
// 1 this task becomes similar to sequential.

__global__
void breakingrsa(unsigned long long ciphertext,unsigned long long int n,unsigned long long *messagelist, int ciphermessagescount){
    unsigned long long int thread_pos = blockIdx.x * blockDim.x +threadIdx.x;
    int total_threads = blockDim.x * gridDim.x;
    for (unsigned long long i = thread_pos; i < n; i+=total_threads) {
        unsigned long long val =1 ;
        for(int j =0;j<3;j++) {
            val = ((val * i) % n);
        }
        if(val==ciphertext && messagenum < ciphermessagescount){
            messagelist[atomicAdd(&messagenum, 1)] = i; // Here atomicAdd is used to perform the addition of the RSA messages which are decoded.
                                                        // This  is done by performing atomic addition i,e either it increases the values in the
                                                        // mentioned address or it doesnt and prevents the value from interleaving.
        }
    }
}

// This is is used to sort the messages received in our array messagelist.

void sort(unsigned long long int *messagelist){
    for(int i = 0; i< messagenum;i++){
        for(int j=i+1;j<messagenum;j++){
            if(messagelist[i] > messagelist[j]){
                unsigned long long int tmp = messagelist[i];
                messagelist[i] = messagelist[j];
                messagelist[j] = tmp;
            }
        }
    }

}

// This is the main function takes in aruments the ciphertext c and the modulus n.

int main(int argc, char **argv) {

    if (argc < 3) {
        // When the input arguments are wrong.
        std::cerr << "USAGE: " << argv[0] << " ciphertext" << " " <<argv[1]<< " modulus" << argv[2] << std::endl;
        return 1;
    }

    unsigned long long modulus = stoull(argv[2]);
    unsigned long long cipher = stoull(argv[1]);
    unsigned long long int *messagelist;

    int ciphermessagescount = 100;
    cudaMallocManaged(&messagelist,ciphermessagescount*sizeof(unsigned long long));

    breakingrsa<<< 1,1 >>>(cipher,modulus,messagelist,100);

    cudaDeviceSynchronize();

    if (messagenum == 0){
        cout << "No cube roots of " << cipher << " (mod " << argv[2] << ")";
    }else{
        sort(messagelist);
        for(int i = 0; i <messagenum;i++){
            cout << messagelist[i] <<"^3 = " << cipher << " (mod " << argv[2] << ")" << endl;
        }
    }

    cudaFree(messagelist);
    return 0;
}

