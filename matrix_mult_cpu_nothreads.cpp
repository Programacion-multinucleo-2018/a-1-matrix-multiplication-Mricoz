/*
    Matrix Multiplication on CPU with No Threads
*/

#include <iostream>
#include <chrono>
#include <cstdlib>

#define SIZE 4000 // Defines size of squared matrix

////////////////// FUNCTION DECLARATIONS //////////////////

int * fillMatrix(int * M, int size);
void printMatrix(int * M, int size);
int * multiplyMatrix(int * A, int * B, int * C);

////////////////// FUNCTION DEFINITIONS //////////////////

// Function to fill matrix
int * fillMatrix(int * M, int size){
    // Fill the matrices with random numbers 1-10
    for (int i = 0; i < size; i++) {
        M[i] = rand() % 10 + 1;
    }
    return M;
}

// Function to print each matrix
void printMatrix(int * M, int size){
    for (int i = 0; i < size; i++) {
        std::cout << M[i] << " ";
    }
    std::cout << "\n";
}

// Function that actually multiplies the matrices
int * multiplyMatrix(int * A, int * B, int * C){
    // Actual for loop to multiply
    for(int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int sum = 0;
            for (int k = 0; k < SIZE; k++){
                sum += A[i * SIZE + k] * B[k * SIZE + j];
            }
            C[i * SIZE + j] = sum;
        }
    }
    return C;
}

// Main function
int main(int argc, char const *argv[]) {

    srand(time(NULL));

    std::cout << "\n";
    std::cout << "---------- MATRIX MULTIPLICATION CPU (NO THREADS) ----------" << "\n";
    std::cout << "\n";

    int totalSize = SIZE * SIZE;

    int *A, *B, *C;// Declare matrix
    A = new int[totalSize];
    B = new int[totalSize];
    C = new int[totalSize];

    std::cout << "initializing..." << "\n\n";
    std::cout << "Multiplication with: " << SIZE << " X " << SIZE << " matrix" << "\n\n";

    A = fillMatrix(A, totalSize); // fill matrix a randomly
    B = fillMatrix(B, totalSize); // fill matrix b randomly

    //printMatrix(A, totalSize); // Prints A
    //printMatrix(B, totalSize); // Prints B

    // Calculates time
    auto start = std::chrono::high_resolution_clock::now();

    C = multiplyMatrix(A, B, C); // Multiplication

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration_ms = end - start; // Ends time

    // Displaying the result matrix
    std::cout << "RESUlT:" << "\n";
    //printMatrix(C, totalSize);
    std::cout << duration_ms.count() << " miliseconds" << "\n";
    std::cout << "\n";

    // Delete allocated memory
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
