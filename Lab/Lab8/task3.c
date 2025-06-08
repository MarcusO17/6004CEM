#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, i;
    int array[100];
    int chunk, *local_array;
    int local_sum = 0, total_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunk = 100 / size;
    local_array = (int*)malloc(chunk * sizeof(int));

    // Initialize full array in root process
    if (rank == 0) {
        for (i = 0; i < 100; i++) array[i] = i + 1;
    }

    // Distribute data to all processes
    MPI_Scatter(array, chunk, MPI_INT, local_array, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    // Parallel sum using OpenMP within each process
    #pragma omp parallel for reduction(+:local_sum)
    for (i = 0; i < chunk; i++) {
        local_sum += local_array[i];
    }

    // Combine all local sums into total sum at root process
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum (Hybrid MPI + OpenMP): %d\n", total_sum);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
