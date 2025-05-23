#include <iostream>
#include <omp.h>

int main(){

    #pragma omp parallel for schedule(guided,1)
    for(int i = 0; i < 20; i++){
        std::cout << "Thread " << omp_get_thread_num() << " is running number " << i << std::endl;
    }



    return 0;
}