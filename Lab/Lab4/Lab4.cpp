#include <iostream>
#include <omp.h>
int main(){
    #pragma omp parallel
    {
        int sum_local = 0;
        #pragma omp for nowait
        for (int i = 0; i < 10; i++) {
            #pragma omp single
            {
                std::cout << "Execution started" << std::endl;
            }  
            sum_local += i;
        }
    }
    return 0;
}