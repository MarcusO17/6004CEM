#include <iostream>
#include <vector>
#include <omp.h>
#include <unistd.h>

void sequential_add(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c) {
    for (int i = 0; i < a.size(); i++) {
        /*if (i % 1000 == 0) {
            sleep(0.001); // Simulate uneven workload
        }*/
        c[i] = a[i] + b[i];
    }
}

void parallel_add_static(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c,int chunk_size = -1) {
    if (chunk_size < 0 ){
        chunk_size = a.size()/4;
    }// 4 threads as set in main

    #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(static, chunk_size)
        for (int i = 0; i < a.size(); i++) {
            /*if (i % 1000 == 0) {
                sleep(0.001); // Simulate uneven workload
            }*/
            c[i] = a[i] + b[i];
        }
    }
}

void parallel_add_dynamic(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c,int chunk_size = 1) {
    #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(dynamic, chunk_size)
        for (int i = 0; i < a.size(); i++) {
            /*if (i % 1000 == 0) {
                sleep(0.001); // Simulate uneven workload
            }*/
            c[i] = a[i] + b[i];
        }
    }
}

int main() 
{
    omp_set_num_threads(4); 

    long n = 10000000;

    std::vector<int> a(n,0); 
    std::vector<int> b(n,0);
    std::vector<int> c_static(n,0);
    std::vector<int> c_dynamic(n,0);
    std::vector<int> c_seq(n,0);

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < n; i++) {
            a[i] = i;
            b[i] = i;
        }
    }

    double start_time, end_time, elapsed_time;

    // Sequential Addition
    start_time = omp_get_wtime();
    sequential_add(a, b, c_seq);
    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;
    std::cout << "Time taken for sequential addition: " << elapsed_time << " seconds" << std::endl;

    // Parallel Addition with static scheduling
    start_time = omp_get_wtime();
    parallel_add_static(a, b, c_static);
    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;
    std::cout << "Time taken for parallel with static (chunk size = " << n / 4 << ") scheduling addition: " << elapsed_time << " seconds" << std::endl;

    // Parallel Addition with dynamic scheduling
    start_time = omp_get_wtime();
    parallel_add_dynamic(a, b, c_dynamic);
    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;
    std::cout << "Time taken for parallel with dynamic (chunk size = 1) schedulling addition: " << elapsed_time << " seconds" << std::endl;

    return 0;
}