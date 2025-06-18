#include <mpi.h>   // MPI_Init, MPI_Comm_size, MPI_Comm_rank, MPI_Finalize, MPI_COMM_WORLD
#include <stdio.h> // printf

int main(int argc, char *argv[]) {
    int rank; // The rank (ID) of the current process
    int size; // The total number of processes in the communicator


    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello from process %d of %d!\n", rank, size);

    MPI_Finalize();

    return 0;
}