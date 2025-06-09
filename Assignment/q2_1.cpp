#include <iostream>
#include <vector>
#include <omp.h>
#include <unistd.h>
#include <iomanip>
#include <thread>
#include <chrono>

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

void sequential_add(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c,bool sleep_enabled = false) {
    for (int i = 0; i < a.size(); i++) {
        if (sleep_enabled && is_prime(i)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Simulate uneven workload
            }
        c[i] = a[i] + b[i];
    }
}

void parallel_add_static(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c,int verbose = 0,bool sleep_enabled = false) {

    #pragma omp parallel default(shared) num_threads(4)
    {
        #pragma omp for schedule(static) 
        for (int i = 0; i < a.size(); i++) {
            if(verbose==1){
                #pragma omp critical
                {
                std::cout << std::setprecision(15) << "Thread " << omp_get_thread_num() << " processing index " << i << " at " << omp_get_wtime()<< std::endl;
            }
        }   
            if (sleep_enabled && is_prime(i)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Simulate uneven workload
            }
            c[i] = a[i] + b[i];
        }
    }
}

void parallel_add_dynamic(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c,int verbose = 0, bool sleep_enabled = false) {
    #pragma omp parallel default(shared) num_threads(4)
    {
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < a.size(); i++) {
             if(verbose==1){
                #pragma omp critical
                {
                std::cout << std::setprecision(15) << "Thread " << omp_get_thread_num() << " processing index " << i << " at " << omp_get_wtime() << std::endl;
            }
        }   
            if (sleep_enabled && is_prime(i)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Simulate uneven workload
            }
            c[i] = a[i] + b[i];
        }
    }
}

int main() 
{
    omp_set_num_threads(4); 

    long n_sizes[4] = {100,1000,10000,100000}; // Different sizes for testing
    //int chunk_sizes[6] = {2500000,1000000,5000,100,10,1}; // Different chunk sizes for testing

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

        //for (int chunk_idx = 0; chunk_idx < 6 ;++chunk_idx) {
            //int chunk_size = chunk_sizes[chunk_idx];
        

            double start_time, end_time, seq_time, static_time, dynamic_time;

            // Sequential Addition
            start_time = omp_get_wtime();
            sequential_add(a, b, c_seq,true);
            end_time = omp_get_wtime();
            seq_time = end_time - start_time;

            //std::cout << "Starting static parallel addition..." << std::endl;
            // Parallel Addition with static scheduling
            start_time = omp_get_wtime();
            parallel_add_static(a, b, c_static,0,true);
            end_time = omp_get_wtime();
            static_time = end_time - start_time;

            //std::cout << "Starting dynamic parallel addition..." << std::endl;
            // Parallel Addition with dynamic scheduling
            start_time = omp_get_wtime();
            parallel_add_dynamic(a, b, c_dynamic,0,true);
            end_time = omp_get_wtime();
            dynamic_time = end_time - start_time;

            std::cout << n << "," << seq_time << "," << static_time << "," << dynamic_time << "," << std::endl;
        //}
        }

    return 0;
}