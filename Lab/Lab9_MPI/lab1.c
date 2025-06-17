#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // total data in root
    int recv_buffer[10]; // buffer large enough to hold all data at root
    int recv_count;
    int sendbuf[10]; // local buffer (may be more than needed)
    
    // Scatterv/Gatherv parameters
    int sendcounts[4] = {2, 3, 2, 3};   // how many elements each process receives/sends
    int displs[4]     = {0, 2, 5, 7};   // starting index for each process in data array

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("Please run with 4 processes!\n");
        MPI_Finalize();
        return 0;
    }

    printf("=== Process %d ===\n", rank);

    // ---- Scatter vs Scatterv ----
    // With Scatter: each gets 2 elements
    int scatter_recv[2];
    MPI_Scatter(data, 2, MPI_INT, scatter_recv, 2, MPI_INT, 0, MPI_COMM_WORLD);

    printf("MPI_Scatter: received [");
    for (int i = 0; i < 2; i++) printf(" %d", scatter_recv[i]);
    printf(" ]\n");

    // With Scatterv: variable elements per process
    recv_count = sendcounts[rank];
    int scatterv_recv[3]; // max 3
    MPI_Scatterv(data, sendcounts, displs, MPI_INT, scatterv_recv, recv_count, MPI_INT, 0, MPI_COMM_WORLD);

    printf("MPI_Scatterv: received [");
    for (int i = 0; i < recv_count; i++) printf(" %d", scatterv_recv[i]);
    printf(" ]\n");

    // Modify received data to simulate processing
    for (int i = 0; i < recv_count; i++) {
        scatterv_recv[i] *= 10;
    }

    // ---- Gather vs Gatherv ----
    // With Gather: send 2 elements back
    int gather_send[2] = {rank*10, rank*10 + 1};
    MPI_Gather(gather_send, 2, MPI_INT, recv_buffer, 2, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("MPI_Gather: gathered [");
        for (int i = 0; i < 8; i++) printf(" %d", recv_buffer[i]);
        printf(" ]\n");
    }

    // With Gatherv: send variable elements back
    MPI_Gatherv(scatterv_recv, recv_count, MPI_INT,
                recv_buffer, sendcounts, displs, MPI_INT,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("MPI_Gatherv: gathered [");
        for (int i = 0; i < 10; i++) printf(" %d", recv_buffer[i]);
        printf(" ]\n");
    }

    MPI_Finalize();
    return 0;
}
