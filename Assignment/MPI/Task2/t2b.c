#include <mpi.h>   
#include <stdio.h> 
#include <string.h> 

#define MAX_MESSAGE_LENGTH 100 // Define a maximum length for messages

int main(int argc, char *argv[]) {
    int rank; // The rank (ID) of the current process
    int size; // The total number of processes in the communicator

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Master process
        printf("Master: Hello slaves give me your messages\n");

        for (int i = 1; i < size; i++) {
            char received_message[MAX_MESSAGE_LENGTH];
            MPI_Status status;

            MPI_Recv(received_message, MAX_MESSAGE_LENGTH, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            printf("Master: Message received from process %d : %s\n", i, received_message);
        }
        
        printf("Master: All messages received.\n");
    } else {
        // Slave processes
        char message_to_send[MAX_MESSAGE_LENGTH];

        // Construct a unique message based on the slave's rank
        if (rank == 1) {
            sprintf(message_to_send, "Hello, I am John");
        } else if (rank == 2) {
            sprintf(message_to_send, "Hello, I am Mary");
        } else if (rank == 3) {
            sprintf(message_to_send, "Hello, I am Susan");
        } else {
            // Generic message for other slaves
            sprintf(message_to_send, "Hello, I am slave %d", rank);
        }

        // Send the string message to the master (rank 0)
        // The count is strlen(message_to_send) + 1 to include the null terminator
        MPI_Send(message_to_send, strlen(message_to_send) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}