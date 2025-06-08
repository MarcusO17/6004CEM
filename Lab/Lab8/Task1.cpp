#include <stdio.h>
#include <omp.h>

int main() {
    int i, sum = 0;
    int array[100];

    // Initialize array
    for (i = 0; i < 100; i++) array[i] = i + 1;

    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < 100; i++) {
        sum += array[i];
    }

    printf("Total sum (OpenMP): %d\n", sum);
    return 0;
}
