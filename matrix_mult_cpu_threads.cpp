/*
    Matrix Multiplication on CPU with Threads
*/

#include <iostream>
#include <pthread.h>
#include <chrono>

#define SIZE 4000 // size matrix
#define THREADS 6 // number of threads

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE]; // Matrices
int iterator = 0; // Iterator for each thread

////////////////// FUNCTION DECLARATIONS //////////////////
void printMatrix(int M[SIZE][SIZE]);
void * multiplyMatrix(void * argc);
void createThreads();
////////////////// FUNCTION DEFINITIONS //////////////////

void * multiplyMatrix(void * argc){
    int matrixIterator = iterator++;
    // 1 thread calculates 1/THREADth of the multiplication
    for (int i = matrixIterator * SIZE / THREADS; i < (matrixIterator + 1) * SIZE / THREADS; i++){
        for (int j = 0; j < SIZE; j++){
            for (int k = 0; k < SIZE; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    //std::cout << "Thread Working" << "\n";
    pthread_exit(NULL); // To exit "non void" function and thread
}

// Function to print each matrix
void printMatrix(int M[SIZE][SIZE]){
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++){
            std::cout << M[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// Function that creates and manages threads
void createThreads(){
    pthread_t threads[THREADS]; // Declare threads

    // thread creation
    for (int i = 0; i < THREADS; i++) {
        int * p;
        pthread_create(&threads[i], NULL, multiplyMatrix, (void*)(p));
    }

    // completion from threads
    for (int i = 0; i < THREADS; i++){
        pthread_join(threads[i], NULL);
    }
}

// Main function
int main(int argc, char const *argv[]){
    // Menu
    std::cout << "\n";
    std::cout << "---------- MATRIX MULTIPLICATION CPU (THREADS) ----------" << "\n";
    std::cout << "\n";
    std::cout << "initializing..." << "\n\n";
    srand(time(NULL));

    // Clean up the matrices
    memset(A, 0, sizeof A);
    memset(B, 0, sizeof B);
    memset(C, 0, sizeof C);

    std::cout << "Multiplication with: " << SIZE << " X " << SIZE << " matrix " <<  "and " << THREADS << " threads" << "\n\n";

    // Fill the matrices
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 10 + 1;
            B[i][j] = rand() % 10 + 1;
        }
    }

    // // Displaying A
    // std::cout << "Matrix A: " << SIZE << " X "<< SIZE << "\n";
    // //printMatrix(A);
    // // Displaying B
    // std::cout << "Matrix B: " << SIZE << " X "<< SIZE << "\n";
    // //printMatrix(B);

    //std::cout << "\n";

    auto start = std::chrono::high_resolution_clock::now();
    createThreads(); // Function called for threads
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration_ms = end - start;

    // Displaying the result matrix
    std::cout << "RESUlT:" << "\n";
    //printMatrix(C);
    std::cout << duration_ms.count() << " miliseconds" << "\n";
    std::cout << "\n";
    return 0;
}
