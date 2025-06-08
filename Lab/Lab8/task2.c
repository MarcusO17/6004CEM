#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, i;
    int array[100];
    int local_sum = 0, total_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get current process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get total number of processes

    // Only root process initializes the array
    if (rank == 0) {
        for (i = 0; i < 100; i++) array[i] = i + 1;
    }

    // Scatter 100/size elements to each process
    int chunk = 100 / size;
    int local_array[chunk];
    MPI_Scatter(array, chunk, MPI_INT, local_array, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes partial sum
    for (i = 0; i < chunk; i++) {
        local_sum += local_array[i];
    }

    // Reduce all partial sums into total sum at root
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum (MPI): %d\n", total_sum);
    }

    MPI_Finalize();
    return 0;
}
