#include <stdio.h>
#include <omp.h>

int main(void){
    int sum = 100;

    printf("Before parallel region, sum's address is %p \n", &sum);

    #pragma omp parallel for reduction(+:sum)
    for(int i = 1; i <= 4; i++){
        printf("Thread %d, sum's address is %p, and value is %d \n", omp_get_thread_num(), &sum, sum);
        sum += i;
    }


    printf("After parallel region, sum's address is %p and values is %d \n", &sum, sum);
    return 0;
}