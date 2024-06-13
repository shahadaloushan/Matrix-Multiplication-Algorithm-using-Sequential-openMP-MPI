#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate random value between 1 and 1000
int generateRandomValue() {
    return rand() % 99 + 1;
}

// Function to generate random vectors A and B of size n
void generateRandomVectors(int n, int *A, int *B) {
    for (int i = 0; i < n; i++) {
        A[i] = generateRandomValue();
        B[i] = generateRandomValue();
    }
}

// Function to compute dot product of vectors A and B and store it in X
int computeDotProduct(int n, int *A, int *B) {
    int X = 0;
    for (int i = 0; i < n; i++) {
        X += A[i] * B[i];
    }
    return X;
}

int main() {
    // Seed for random number generation
    srand(time(NULL));
    
    // Define the size of the matrix 100
    int size = 100; 

    // Allocate memory for arrays A, B
    int *A = (int *)malloc(size * sizeof(int));
    int *B = (int *)malloc(size * sizeof(int));

    // Generate random vectors A and B
    generateRandomVectors(size, A, B);

    printf("Size of matrix A: 1 x %d\n", size);
    printf("Size of matrix B: %d x 1\n", size);

    double total_time = 0;

    // Measure the computation time for 10 iterations
    for (int iter = 0; iter < 10; iter++) {
        clock_t start_time = clock(); // Record start time

        // Compute dot product of vectors A and B
        int result = computeDotProduct(size, A, B);

        clock_t end_time = clock(); // Record end time
        double iteration_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("\nTime of computation of iteration %d: %f seconds\n", iter, iteration_time);
        
        // Accumulate total time
        total_time += iteration_time;
    }

    // Calculate and print average computational time
    double avg_time = total_time / 10;
    printf("\nAverage computational time over 10 iterations: %f seconds\n", avg_time);

    // Free allocated memory
    free(A);
    free(B);

    return 0;
}
