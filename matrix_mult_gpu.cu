/*
    Matrix Multiplication on GPU
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <math.h>

#include "common.h"

#define SIZE 4000

////////// FUNCTION DECLARATIONS //////////

void printMatrix(int * M);
void fillMatrix(int * M);
//bool checkResult(int * m_host, int * m_gpu);
//void checkResult2(int *hostRef, int *gpuRef);
//void matrixMultHost(int * C, int * A, int * B);
__global__ void matrixMultGPU(int * A, int * B, int * C);

////////// FUNCTION DEFINITIONS //////////

//Print the matrix
void printMatrix(int * M){
    int size = SIZE * SIZE;
    for (int i = 0; i < size; i++) {
        std::cout << M[i] << " ";
    }
    std::cout << "\n";
}

// Function to fill the matrix
void fillMatrix(int * M){
    int size = SIZE * SIZE;
    // Fill the matrices with random numbers 1-10
    for (int i = 0; i < size; i++) {
        M[i] = rand() % 10 + 1;
    }
}

// // Check that the two matrices match
// bool checkResult(int * m_host, int * m_gpu){
//     int size = SIZE * SIZE;
//     for(int i = 0; i< size; i++){
//         if(m_host[i]!= m_gpu[i]){
//             return false;
//         }
//     }
//     return true;
// }

// // Function to multiply matrices cpu (Host)
// void matrixMultHost(int * C, int * A, int * B){
//     for(int i = 0; i < SIZE; i++){
//         for(int j = 0; j < SIZE; j++){
//             for(int k = 0; k < SIZE; k++){
//                 C[i * SIZE + j] += A[k + i * SIZE] * B[k * SIZE + j];
//             }
//         }
//     }
// }

// Function to multiply matrices gpu
__global__ void matrixMultGPU(int * A, int * B, int * C){

    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int iy = threadIdx.y + blockIdx.y * blockDim.y;
    unsigned int idx = ix * SIZE + iy;

    int temp = 0;

    if (ix < SIZE && iy < SIZE){
        for(int i = 0; i < SIZE ; i++){
            temp += A[ix * SIZE + i] * B[i * SIZE + iy];
        }
        C[idx] = temp;
    }
}

// Main function
int main(int argc, char **argv){

    srand(time(NULL));

    std::cout << "\n";
    std::cout << "---------- MATRIX MULTIPLICATION CPU (NO THREADS) ----------" << "\n";
    std::cout << "\n";

    std::cout << "initializing..." << "\n\n";

    // Set up device
    int dev = 0;
    cudaDeviceProp deviceProp;
    SAFE_CALL(cudaGetDeviceProperties(&deviceProp, dev), "Error device prop");
    printf("Using Device %d: %s\n", dev, deviceProp.name);
    SAFE_CALL(cudaSetDevice(dev), "Error setting device");

    // Set up data size of matrix
    int nx = SIZE;
    int ny = SIZE;

    int nxy = nx * ny;
    int nBytes = nxy * sizeof(int);
    std::cout << "Matrix size: nx " << nx << " ny " << ny << "\n";

    // Malloc host memory
    int *A, *B, *gpuRef;
    A = (int *)malloc(nBytes);
    B = (int *)malloc(nBytes);
    //hostRef = (int *)malloc(nBytes);
    gpuRef = (int *)malloc(nBytes);

    // Initialize data at host side

    fillMatrix(A);
    fillMatrix(B);

    //printMatrix(A);
    //printMatrix(B);

    // Fill with 0
    //memset(hostRef, 0, nBytes);
    memset(gpuRef, 0, nBytes);

    // Multiply matrix at host side for result SAFE_CALLs
    // auto start_cpu = std::chrono::high_resolution_clock::now();
    // matrixMultHost(hostRef, A, B);
    // auto end_cpu = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<float, std::milli> duration_ms = end_cpu - start_cpu;
    //
    // std::cout << "matrixMultHost elapsed " << duration_ms.count() << " ms" << "\n";

    // malloc device global memory
    int *d_MatA, *d_MatB, *d_MatC;
    SAFE_CALL(cudaMalloc((void **)&d_MatA, nBytes), "Error allocating d_MatA");
    SAFE_CALL(cudaMalloc((void **)&d_MatB, nBytes), "Error allocating d_MatB");
    SAFE_CALL(cudaMalloc((void **)&d_MatC, nBytes), "Error allocating d_MatC");

    // transfer data from host to device
    SAFE_CALL(cudaMemcpy(d_MatA, A, nBytes, cudaMemcpyHostToDevice), "Error copying d_MatA");
    SAFE_CALL(cudaMemcpy(d_MatB, B, nBytes, cudaMemcpyHostToDevice), "Error copying d_MatB");

    // invoke kernel at host side
    int dimx = 1;
    int dimy = 128;
    dim3 block(dimx, dimy);
    dim3 grid((nx + block.x - 1) / block.x, (ny + block.y - 1) / block.y);

    auto start_cpu =  std::chrono::high_resolution_clock::now();
    matrixMultGPU<<<grid, block>>>(d_MatA, d_MatB, d_MatC);
    SAFE_CALL(cudaDeviceSynchronize(), "Error executing kernel");
    auto end_cpu =  std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration_ms = end_cpu - start_cpu;

    printf("matrixMultGPU <<<(%d,%d), (%d,%d)>>> elapsed %f ms\n", grid.x, grid.y, block.x, block.y, duration_ms.count());

    // SAFE_CALL kernel error
    SAFE_CALL(cudaGetLastError(), "Error with last error");

    // copy kernel result back to host side
    SAFE_CALL(cudaMemcpy(gpuRef, d_MatC, nBytes, cudaMemcpyDeviceToHost), "Error copying d_MatC");

    // Compare results
    //checkResult(hostRef, gpuRef);

    // free device global memory
    SAFE_CALL(cudaFree(d_MatA), "Error freeing memory");
    SAFE_CALL(cudaFree(d_MatB), "Error freeing memory");
    SAFE_CALL(cudaFree(d_MatC), "Error freeing memory");

    // free host memory
    free(A);
    free(B);
    //free(hostRef);
    free(gpuRef);

    // reset device
    SAFE_CALL(cudaDeviceReset(), "Error reseting");

    return (0);
}
