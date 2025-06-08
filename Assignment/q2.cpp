#include <iostream>
#include <vector>
#include <omp.h>
#include <unistd.h>

void sequential_add(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c,bool sleep_enabled = true) {
    for (int i = 0; i < a.size(); i++) {
        if (sleep_enabled && i % 1000 == 0) {
            sleep(0.01); // Simulate uneven workload
        }
        c[i] = a[i] + b[i];
    }
}

void parallel_add_static(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c,int chunk_size = -1,bool sleep_enabled = true) {
    if (chunk_size < 0 ){
        chunk_size = a.size()/4;
    }// 4 threads as set in main

    #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(static)
        for (int i = 0; i < a.size(); i++) {
            if (sleep_enabled && i % 1000 == 0) {
                sleep(0.01); // Simulate uneven workload
            }
            c[i] = a[i] + b[i];
        }
    }
}

void parallel_add_dynamic(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c,int chunk_size = 1,bool sleep_enabled = true) {
    if (chunk_size < 1 ){
        chunk_size = 1; // Minimum chunk size for dynamic scheduling
    }
   
    #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < a.size(); i++) {
            if (sleep_enabled && i % 1000 == 0) {
                sleep(0.01); // Simulate uneven workload
            }
            c[i] = a[i] + b[i];
        }
    }
}

int main() 
{
    omp_set_num_threads(4); 

    long n_sizes[4] = {10000, 100000, 1000000, 10000000}; // Different sizes for testing
    int chunk_sizes[5] = {1,2,4,8,16};

    std::vector<int> a; 
    std::vector<int> b;
    std::vector<int> c_static;
    std::vector<int> c_dynamic;
    std::vector<int> c_seq;

    std::cout << "iteration,seq_time,static_time,dynamic_time,chunk_size" << std::endl;

    for (int size_idx = 0; size_idx < 4; ++size_idx) {

        long n = n_sizes[size_idx];
        a.assign(n, 0);
        b.assign(n, 0);
        c_seq.assign(n, 0);
        c_static.assign(n, 0);
        c_dynamic.assign(n, 0);

        // Initialize vectors   
        #pragma omp parallel for
        for (long i = 0; i < n; i++) {
            a[i] = i;
            b[i] = i;
        }

        for (int chunk_idx = 0; chunk_idx < 5; ++chunk_idx) {
            int chunk_size = n/chunk_sizes[chunk_idx];

            double start_time, end_time, seq_time, static_time, dynamic_time;

            // Sequential Addition
            start_time = omp_get_wtime();
            sequential_add(a, b, c_seq);
            end_time = omp_get_wtime();
            seq_time = end_time - start_time;

            // Parallel Addition with static scheduling
            start_time = omp_get_wtime();
            parallel_add_static(a, b, c_static, chunk_size);
            end_time = omp_get_wtime();
            static_time = end_time - start_time;

            // Parallel Addition with dynamic scheduling
            start_time = omp_get_wtime();
            parallel_add_dynamic(a, b, c_dynamic, chunk_size);
            end_time = omp_get_wtime();
            dynamic_time = end_time - start_time;

            std::cout << n << "," << seq_time << "," << static_time << "," << dynamic_time << "," << chunk_size << std::endl;
        }
    }

    return 0;
}