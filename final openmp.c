#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Function to generate random value between 1 and 99
int generateRandomValue() {
    return rand() % 99 + 1;
}

// Function to generate random vectors A and B 
void generateRandomVectors(int n, int *A, int *B) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        A[i] = generateRandomValue();
        B[i] = generateRandomValue();
    }
}

// Function to compute dot product of vectors A and B
int computeDotProduct(int n, int *A, int *B) {
    int X = 0;
    #pragma omp parallel for reduction(+:X)
    for (int i = 0; i < n; i++) {
        X += A[i] * B[i];
    }
    return X;
}

int main() {
    // Seed for random number generation
    srand(time(NULL));

    // Define the size of the matrix
    int n = 100;

    // Loop for different thread numbers
    for (int num_threads = 2; num_threads <= 16; num_threads *= 2) {
        // Set the number of threads
        omp_set_num_threads(num_threads);

        double computation_time = 0.0; // Variable to store the total computation time

        // Loop for 10 iterations
        for (int iteration = 0; iteration < 10; iteration++) {
            // Allocate memory for arrays A and B
            int *A = (int *)malloc(n * sizeof(int));
            int *B = (int *)malloc(n * sizeof(int));

            // Start measuring the time for parallel sections
            double start_time = omp_get_wtime();

            // Generate random vectors A and B
            generateRandomVectors(n, A, B);

            // Compute dot product of vectors A and B
            int result = computeDotProduct(n, A, B);

            // End measuring the time for parallel sections
            double end_time = omp_get_wtime();

            // Calculate the time of computation
            computation_time += end_time - start_time;

            // Print the time of computation for each iteration
            printf("Time of computation for %d iteration/s: %.6f seconds\n", iteration + 1, end_time - start_time);

            // Free allocated memory
            free(A);
            free(B);
        }

        // Calculate and print the average time of computation
        printf("The average time of computation: %.6f seconds\n", computation_time / 10.0);

        // Print the run time of the algorithm with the current number of threads
        printf("The Run Time of The Algorithm With %d Threads\n\n", num_threads);
    }

    return 0;
}
