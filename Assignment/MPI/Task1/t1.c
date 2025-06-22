#include <mpi.h>   
#include <stdio.h> 

int main(int argc, char *argv[]) {
    int rank; // The rank (ID) of the current process
    int size; // The total number of processes in the MPI env


    MPI_Init(&argc, &argv); //init mpi environment

    MPI_Comm_size(MPI_COMM_WORLD, &size); // passes the number of processes into size

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // passes the rank of the current process into rank

    printf("Hello from process %d of %d!\n", rank, size);

    MPI_Finalize(); // Finalize the MPI environment

    return 0;
}