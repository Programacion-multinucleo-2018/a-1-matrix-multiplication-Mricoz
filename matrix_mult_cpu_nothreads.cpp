/*
    Matrix multiplication in CPU (No threads)
*/

#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <time.h>

#define SIZE 800 // SIZE TO CHANGE

////////// Function declarations //////////
void printMatrix(int M[SIZE][SIZE]);
void multiply(int A[SIZE][SIZE], int B[SIZE][SIZE]);

////////// Function definitions //////////

// Matrix multiplication function
void multiply(int A[SIZE][SIZE], int B[SIZE][SIZE]) {

    int C[SIZE][SIZE]; // New matrix

    // For loop to iterate through the matrix and perform operation
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    //printMatrix(C);
}

void printMatrix(int M[SIZE][SIZE]){
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            std::cout << M[i][j] << "  ";
        }
        std::cout << "\n";
    }
}

int main(int argc, char const *argv[]) {

    std::cout << "\n";
    std::cout << "---------- MATRIX MULTIPLICATION CPU (NO THREADS) ----------" << "\n";
    std::cout << "\n";

    // LLenar las matrices con tempo_numeros random

    int A[SIZE][SIZE]; // Declare matrix A
    int B[SIZE][SIZE]; // Declare matrix B
    srand(time(NULL)); // Rand

    // Fill the matrices with random natural numbers
    for (int i = 0; i < SIZE ; i++) {
        for (int j = 0; j < SIZE; j++){
            A[i][j] = rand() % 10 + 1;
            B[i][j] = rand() % 10 + 1;
        }
    }
    std::cout << "MATRIX A: " << SIZE << " X " << SIZE << "\n";
    //printMatrix(A);
    std::cout << "MATRIX B: " << SIZE << " X " << SIZE << "\n";
    //printMatrix(B);
    std::cout << "\n";
    std::cout << "RESULTS:" << "\n";

    // Timing
    clock_t t;
	t = clock();
    multiply(A, B); // llama la funcion de multiplicacion
    t = clock() - t;

    std::cout << "Time Elapsed: " << t << " miliseconds" << "\n";
	std::cout << "Cps: " << CLOCKS_PER_SEC << " clocks per second" << "\n";

    std::cout << "\n";
    return 0;
}
