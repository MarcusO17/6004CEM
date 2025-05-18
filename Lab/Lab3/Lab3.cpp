#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main()  
{
    int shared = 0;

    printf("Thread %d says A\n", omp_get_thread_num());

    #pragma omp parallel
    {

        #pragma omp critical
        {
            shared++;
            printf("Thread %d says B ---- %d\n", omp_get_thread_num(), shared);
        }
    

        #pragma omp for 
        for (int i = 0; i < 10; i++)
        {
            printf("Thread %d says C %d \n", omp_get_thread_num(), i);
        }

        shared++;
        printf("Thread %d says D ----- %d \n", omp_get_thread_num(), shared);
    
    
    }

    printf("Thread %d says F\n", omp_get_thread_num());
    return 0;
}
