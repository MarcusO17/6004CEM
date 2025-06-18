#include <mpi.h>   // MPI_Init, MPI_Comm_size, MPI_Comm_rank, MPI_Finalize, MPI_COMM_WORLD
#include <stdio.h> // printf

#define HELLO_TAG 0


int main(int argc, char *argv[]) {
    int rank; // The rank (ID) of the current process
    int size; // The total number of processes in the communicator


    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

     if (rank == 0) {
        printf("Master: Hello slaves give me your messages\n");

        for (int i = 1; i < size; i++) {
            int received_rank;
            MPI_Status status; 

            MPI_Recv(&received_rank, 1, MPI_INT, i, HELLO_TAG, MPI_COMM_WORLD, &status);
            printf("Message received from process %d : Hello back\n", received_rank);
        
        }
        printf("Master: All messages received.\n");

    } else {
        MPI_Send(&rank, 1, MPI_INT, 0, HELLO_TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}