#include <iostream>
#include <omp.h>
int main(void){
    int sum = 0;
    int N = 100;

    int A[N];
    for (int i = 0; i < N; i++) {
        A[i] = i + 1;
    }

    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < N; i++){
        sum += A[i];
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
