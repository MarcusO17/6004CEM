#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_data[4];     // data each process contributes
    int reduce_result[4]; // result of Reduce (at root)
    int allreduce_result[4]; // result of Allreduce (on all processes)
    int reduce_scatter_result; // each process gets one part

    int recvcounts[4] = {1, 1, 1, 1}; // 1 value per process for Reduce_scatter

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("Please run with 4 processes!\n");
        MPI_Finalize();
        return 0;
    }

    // Each process fills in 4 elements: [rank+1, rank+2, rank+3, rank+4]
    for (int i = 0; i < 4; i++) {
        send_data[i] = rank + i + 1;
    }

    // --- MPI_Reduce ---
    MPI_Reduce(send_data, reduce_result, 4, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("MPI_Reduce result at root: ");
        for (int i = 0; i < 4; i++) printf("%d ", reduce_result[i]);
        printf("\n");
    }

    // --- MPI_Allreduce ---
    MPI_Allreduce(send_data, allreduce_result, 4, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d - MPI_Allreduce result: ", rank);
    for (int i = 0; i < 4; i++) printf("%d ", allreduce_result[i]);
    printf("\n");

    // --- MPI_Reduce_scatter ---
    MPI_Reduce_scatter(send_data, &reduce_scatter_result, recvcounts, MPI_INT,MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d - MPI_Allreduce result: ", rank);
    for (int i = 0; i < 4; i++) printf("%d ", allreduce_result[i]);
    printf("\n");