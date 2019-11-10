//#include <iostream>
//#include <algorithm>
//
//__managed__ unsigned int messagenum = 0;
//
//using namespace std;
//
//// kernal function takes in arguments cipher c, modulus n, messagelist(Which is shared betweeen host
//// and device in Unified memory).
//
//__global__
//void breakingrsa(unsigned long long ciphertext,unsigned long long int n,unsigned long long *messagelist, int ciphermessagescount){
//    unsigned long long int thread_pos = blockIdx.x * blockDim.x +threadIdx.x;
//    int total_threads = blockDim.x * gridDim.x;
//    for (unsigned long long i = thread_pos; i < n; i+=total_threads) {
//        unsigned long long val =1 ;
//        for(int j =0;j<3;j++) {
//            val = ((val * i) % n);
//        }
//        if(val==ciphertext && messagenum < ciphermessagescount){
//            messagelist[atomicAdd(&messagenum, 1)] = i; // Here atomicAdd is used to perform the addition of the RSA messages which are decoded.
//            // This  is done by performing atomic addition i,e either it increases the values in the
//            // mentioned address or it doesnt and prevents the value from interleaving.
//        }
//    }
//}
//
//
//// This is the main function takes in aruments the ciphertext c and the modulus n.
//
//int main(int argc, char **argv) {
//
//    if (argc < 3) {
//        // When the input arguments are wrong.
//        std::cerr << "USAGE: " << argv[0] << " ciphertext" << " " <<argv[1]<< " modulus" << argv[2] << std::endl;
//        return 1;
//    }
//
//    unsigned long long modulus = stoull(argv[2]);
//    unsigned long long cipher = stoull(argv[1]);
//    unsigned long long int *messagelist;
//
//    int ciphermessagescount = 100;
//    cudaMallocManaged(&messagelist,ciphermessagescount*sizeof(unsigned long long));
//
//    int threadsperblock = 256;
//    int totalblocks = (modulus+threadsperblock-1) / threadsperblock;
//    breakingrsa<<< totalblocks,threadsperblock >>>(cipher,modulus,messagelist,100);
//
//    cudaDeviceSynchronize();
//
//    if (messagenum == 0){
//        cout << "No cube roots of " << cipher << " (mod " << argv[2] << ")";
//    }else{
//        sort(messagelist,messagelist+messagenum);
//        for(int i = 0; i <messagenum;i++){
//            cout << messagelist[i] <<"^3 = " << cipher << " (mod " << argv[2] << ")" << endl;
//        }
//    }
//
//    cudaFree(messagelist);
//    return 0;
//}
//
