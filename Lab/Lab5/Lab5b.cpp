#include <iostream>
#include <omp.h>
int main(void){
    int sum = 0;
    int sa = 0;
    int sb = 0;
    int m = 10;
    int N = 100;

    int A[N];
    for (int i = 0; i < N; i++) {
        A[i] = i + 1;
    }

    int B[N];
    for (int i = 0; i < N; i++) {
        B[i] = i + 1;
    }

    #pragma omp parallel for reduction(+:sa,sb) reduction(max:m)
    for (int i = 0; i < N; i++) {
        sa += A[i];
        sb += B[i];
        if (A[i] > m) m = A[i];
        m = B[i]>m?B[i]:m;

    }

    std::cout << "Sum of A (sa): " << sa << std::endl;
    std::cout << "Sum of B (sb): " << sb << std::endl;
    std::cout << "Maximum value (m): " << m << std::endl;
}