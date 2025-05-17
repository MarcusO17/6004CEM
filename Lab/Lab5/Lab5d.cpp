#include <stdio.h>
#include <omp.h>

int main(){

    int sum = 0;
    int x[10000];
    for (int i = 0; i < 10000; i++) {
        x[i] = i; // Initialize array with some values
    }

    #pragma omp parallel 
    {
        #pragma for shared(x) private(i) reduction(+:sum)
        for (int i = 0; i < 10000; i++) {
            sum += x[i];
        }
    }

    printf("Sum: %d\n", sum);
    return 0;
}