#include <iostream>
#include <omp.h>

//  int num_threads = 0;
//std::cout << "How many threads do you want to use? " << std::endl;
//std::cin >> num_threads;

int main(){

    #pragma omp parallel
    {
        #pragma omp critical 
        {
            std::cout << "Hello World from thread " << omp_get_thread_num() << std::endl;
        }
    }

    return 0;
}