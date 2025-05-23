#include <stdio.h>
#include <omp.h>

int fib(int n) {
    if (n < 2) return n;
    return fib(n - 1) + fib(n - 2);
}

int main() {
    int n = 10;
    int x = 0, y = 0;
    
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            x = fib(n - 1);
        }
        #pragma omp section
        {
            y = fib(n - 2);
        }
    }

    int result = x + y;
    printf("Fibonacci(%d) = %d\n", n, result);
    return 0;
}

