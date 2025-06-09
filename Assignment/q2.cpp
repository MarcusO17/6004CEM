#include <iostream>
#include <vector>
#include <omp.h>
#include <unistd.h>

void sequential_add(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c) {
    for (int i = 0; i < a.size(); i++) {
        c[i] = a[i] + b[i];
    }
}

void parallel_add_static(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c) {

    #pragma omp parallel default(shared) num_threads(4)
    {
        #pragma omp for schedule(static) 
        for (int i = 0; i < a.size(); i++) {
            /*if (i % 1000 == 0) {
                sleep(0.1); // Simulate uneven workload
            }*/
            c[i] = a[i] + b[i];
        }
    }
}

void parallel_add_dynamic(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c) {
    #pragma omp parallel default(shared) num_threads(4)
    {
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < a.size(); i++) {
            /*if (sleep_enabled && i % 1000 == 0) {
                sleep(0.1); // Simulate uneven workload
            }*/
            c[i] = a[i] + b[i];
        }
    }
}

int main() 
{
    omp_set_num_threads(4); 

    long n_sizes[7] = {10,100,1000,10000,100000, 1000000, 10000000}; // Different sizes for testing

    std::vector<int> a; 
    std::vector<int> b;
    std::vector<int> c_static;
    std::vector<int> c_dynamic;
    std::vector<int> c_seq;

    std::cout << "iteration,seq_time,static_time,dynamic_time" << std::endl;

    for (int size_idx = 0; size_idx < sizeof(n_sizes)/sizeof(n_sizes[0]); ++size_idx) {

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

        //for (int chunk_idx = 0; chunk_idx < 5; ++chunk_idx) {
            //int chunk_size = n/chunk_sizes[chunk_idx];

            double start_time, end_time, seq_time, static_time, dynamic_time;

            // Sequential Addition
            start_time = omp_get_wtime();
            sequential_add(a, b, c_seq);
            end_time = omp_get_wtime();
            seq_time = end_time - start_time;

            // Parallel Addition with static scheduling
            start_time = omp_get_wtime();
            parallel_add_static(a, b, c_static);
            end_time = omp_get_wtime();
            static_time = end_time - start_time;

            // Parallel Addition with dynamic scheduling
            start_time = omp_get_wtime();
            parallel_add_dynamic(a, b, c_dynamic);
            end_time = omp_get_wtime();
            dynamic_time = end_time - start_time;

            std::cout << n << "," << seq_time << "," << static_time << "," << dynamic_time << "," << std::endl;
        //}
    }

    return 0;
}