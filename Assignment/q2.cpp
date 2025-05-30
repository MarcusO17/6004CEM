#include <iostream>
#include <vector>
#include <omp.h>
#include <unistd.h>
#include <chrono> 


void verify_vectors(const std::vector<int>& v1, const std::vector<int>& v2, const std::string& name) {
    if (v1.size() != v2.size()) {
        std::cout << "ERROR "<< name <<" : Vector sizes differ! "
                  << v1.size() << " vs " << v2.size() << std::endl;
    }
    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) {
            std::cout << "ERROR "<< name <<" : Mismatch at index " << i << ". "
                      << "Expected: " << v2[i] 
                      << ", Got: " << v1[i] << std::endl;
        }
    }
    std::cout << "Verification for " << name << ": SUCCESS! Results match." << std::endl;
}


int main() 
{

    omp_set_num_threads(4); // Set the number of threads to 4

    long n = 10000000;
    std::vector<int> a(n,0); 
    std::vector<int> b(n,0);
    std::vector<int> c_static(n,0);
    std::vector<int> c_dynamic(n,0);
    std::vector<int> c_seq(n,0);

    #pragma omp parallel  // Separate parallel region for vector initialization
    {
        // Initialize vectors in parallel
        #pragma omp for
        for (int i = 0; i < n; i++) {
            a[i] = i;
            b[i] = i;
        }
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
         /*if (i % 1000 == 0) {
                sleep(0.001); // Simulate uneven workload
            }*/
            c_seq[i] = a[i] + b[i];
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "Time taken for sequential addition: " << elapsed_time.count() << " seconds" << std::endl;


    start_time = std::chrono::high_resolution_clock::now();
    #pragma omp parallel  //Seperate parallel region for vector addition
    {
        //Vector Addition
        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++) {
             /*if (i % 1000 == 0) {
                sleep(0.001); // Simulate uneven workload
            }*/
            c_static[i] = a[i] + b[i];
        }
    }
    end_time = std::chrono::high_resolution_clock::now();

    elapsed_time = end_time - start_time;
    std::cout << "Time taken for parallel with static (chunk size = " << n / 4 << ") scheduling addition: " << elapsed_time.count() << " seconds" << std::endl;


    start_time = std::chrono::high_resolution_clock::now();
    #pragma omp parallel  //Seperate parallel region for vector addition
    {
        //Vector Addition
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < n; i++) {
            /*if (i % 1000 == 0) {
                sleep(0.001); // Simulate uneven workload
            }*/
            c_dynamic[i] = a[i] + b[i];
        }
    }
    end_time = std::chrono::high_resolution_clock::now();

    elapsed_time = end_time - start_time;
    std::cout << "Time taken for parallel with dynamic " << "(chunk size = 1)" << " schedulling addition: " << elapsed_time.count() << " seconds" << std::endl;

    // Verify the last element of vector c
    verify_vectors(c_dynamic, c_seq, "Dynamic");
    verify_vectors(c_static, c_seq, "Static");
    

    return 0;
}