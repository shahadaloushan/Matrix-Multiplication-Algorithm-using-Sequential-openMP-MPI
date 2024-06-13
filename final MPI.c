#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_SIZE 100
#define NUM_ITERATIONS 10
int main(int argc, char* argv[]) 
{
int *A = NULL, *B = NULL; // A[1,n] and B[n,1]
int *local_A = NULL, *local_B = NULL,*sendcounts,*displs ; //for all processes 
int local_sum = 0, total_sum = 0 ,  base_elements, remaining_elements;
              
              //mpi
              
int my_ID, num_process, elements_per_proc;
double start_time, end_time, total_time,iteration_time,avg;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &my_ID);
MPI_Comm_size(MPI_COMM_WORLD, &num_process);//


if (my_ID == 0) {
    A = malloc(MAX_SIZE * sizeof(int)); // matrix A[1,100]
    B = malloc(MAX_SIZE * sizeof(int)); // matrix B[100,1]
  
if (!A || !B) {
    fprintf(stderr, "Failed to allocate memory for matrices on root process.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
}

// initialize matrices A and B on the root process p0 =0 
       
        for (int i = 0; i < MAX_SIZE; i++) {
            A[i] = rand() % 100;
            B[i] = rand() % 100;
            }
       
    }//end  if p0 
    
    sendcounts = malloc(num_process * sizeof(int));
    displs = malloc(num_process * sizeof(int)); //index in array 

int start = 0;
// Calculate base number of elements for each processor
base_elements = MAX_SIZE / num_process;
remaining_elements = MAX_SIZE% num_process; // Calculate the remaining elements

// distributes the remaining element evenly on the processes
for (int i = 0; i < num_process; i++) {
    sendcounts[i] = base_elements + (remaining_elements > 0 ? 1 : 0);
    remaining_elements--; // Decrease the remaining elements count
    displs[i] = start; // index 
    start+= sendcounts[i];
}

local_A = malloc(sendcounts[my_ID] * sizeof(int));
local_B = malloc(sendcounts[my_ID] * sizeof(int));

if (!local_A || !local_B) {
    fprintf(stderr, "Memory allocation failed for local arrays on process %d.\n", my_ID);
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
}

    // Scatter the elements of A and B to all processes
MPI_Scatterv(A, sendcounts, displs, MPI_INT, local_A, sendcounts[my_ID], MPI_INT, 0, MPI_COMM_WORLD);
MPI_Scatterv(B, sendcounts, displs, MPI_INT, local_B, sendcounts[my_ID], MPI_INT, 0, MPI_COMM_WORLD); 
   
if (my_ID == 0) {

        for (int i = 0; i < num_process; i++) {
            printf("Process %d gets %d elements, starts at index %d\n", i, sendcounts[i], displs[i]);
        }
    }
    //******************************
     
     //printf("Performing multiplication p %d \n", my_ID);
    for (int iter = 0; iter <  NUM_ITERATIONS; iter++) {
    local_sum = 0;
    start_time = MPI_Wtime();
    for (int i = 0; i < sendcounts[my_ID]; i++) {
        local_sum += local_A[i] * local_B[i];
    }//end for loop
    MPI_Reduce(&local_sum, &total_sum , 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();
    iteration_time = end_time - start_time;
    if (my_ID == 0) {
      printf("Final result = %d\n",total_sum);
            total_time += iteration_time;
        }
    }//end iteration
    
   if (my_ID == 0) {
   avg = total_time /NUM_ITERATIONS;
   printf("Sum dot product result for matrix A[1,100], B[100,1] is:\n X= %d\n",total_sum);
   printf("Average computation time: %.9f seconds\n", avg);
   }

    

    // clean 
    if (my_ID == 0) {
        free(A);
        free(B);

    }
            free(sendcounts);
        free(displs);
    free(local_A);
    free(local_B);

    MPI_Finalize();
            
    return 0;







 }//end main 