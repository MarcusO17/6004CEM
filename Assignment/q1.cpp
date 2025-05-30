#include <iostream>
#include <omp.h>

int main(){

    int thread_count = 0;
    std::cout << "How many threads do you want to use? " << std::endl;
    std::cin >> thread_count;


    #pragma omp parallel num_threads(thread_count)
    {
        #pragma omp critical // This ensures that only one thread prints at a time
        {
            std::cout << "Hello World from thread " << omp_get_thread_num() << std::endl;
        }
    }

    return 0;
}