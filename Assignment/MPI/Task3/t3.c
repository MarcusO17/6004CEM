#include <mpi.h>  
#include <stdio.h> 
#include <string.h> 

#define MAX_MESSAGE_LENGTH 100 
int main(int argc, char *argv[]) {
    int rank; // The rank (ID) of the current process
    int size; // The total number of processes in the communicator

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Master process

        for (int i = 1; i < size; i++) {
            char sent_message[MAX_MESSAGE_LENGTH];
            sprintf(sent_message, "Hello process %d, from Master.", i);
            //Use tag 100 for receiving messages
            MPI_Send(sent_message, MAX_MESSAGE_LENGTH, MPI_CHAR, i, 100, MPI_COMM_WORLD);
        }
        printf("Master: All messages sent.\n");
    } else {
        // Slave processes
        char recieve_message[MAX_MESSAGE_LENGTH];
        MPI_Status status;


        // Send using Tag 100
        // The count is strlen(message_to_send) + 1 to include the null terminator
        MPI_Recv(recieve_message, MAX_MESSAGE_LENGTH, MPI_CHAR, 0, 101, MPI_COMM_WORLD, &status);
        printf("Slave %d: Received message: %s\n", rank, recieve_message);
    }

    MPI_Finalize();
    return 0;
}
