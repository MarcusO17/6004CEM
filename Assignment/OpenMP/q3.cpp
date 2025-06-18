#include <iostream>
#include <omp.h>
#include <vector>

void printMatrix( std::vector<std::vector<int>>& matrix);

// function to populate matrices
std::vector<std::vector<int>> matrixPopulator(int r, int c);

void multiply_outer_loop_parallel(
     std::vector<std::vector<int>>& A,
     std::vector<std::vector<int>>& B,
    std::vector<std::vector<int>>& C,
    int N);

// Parallelizing only inner loop 
void multiply_inner_loop_parallel(
     std::vector<std::vector<int>>& A,
     std::vector<std::vector<int>>& B,
    std::vector<std::vector<int>>& C,
    int N);


// Parallel matrix multiplication
void multiply_parallel(
     std::vector<std::vector<int>>& A,
     std::vector<std::vector<int>>& B,
    std::vector<std::vector<int>>& C,
    int N);



int main() {

    srand(time(0));

    int matrix_sizes[] = {50, 500};      // N x N 
    int thread_counts[] = {1, 4, 8, 16};
    const int NUM_RUNS = 10;  


    // Populate matrices
    for (int N : matrix_sizes) {

        std::vector<std::vector<int>> A = matrixPopulator(N, N);
        std::vector<std::vector<int>> B = matrixPopulator(N, N);
        std::vector<std::vector<int>> C(N, std::vector<int>(N));

        std::cout << "Outer Loop Parallel Multiplication" << std::endl << std::endl;

        for (int num_threads : thread_counts) {
            omp_set_num_threads(num_threads); 
            double total_time_outer = 0.0;

            for (int run = 0; run < NUM_RUNS; ++run) {
                double start_time = omp_get_wtime();
                multiply_outer_loop_parallel(A, B, C, N);
                double end_time = omp_get_wtime();
                total_time_outer += (end_time - start_time);
            }
            double avg_time_outer = (total_time_outer / NUM_RUNS); 
            std::cout << N << "," << num_threads << ",Outer," << avg_time_outer << std::endl;
        }
        
        std::cout << "Inner Loop Parallel Multiplication" << std::endl << std::endl;

        for (int num_threads : thread_counts) {
            omp_set_num_threads(num_threads); 
            double total_time_outer = 0.0;

            for (int run = 0; run < NUM_RUNS; ++run) {
                double start_time = omp_get_wtime();
                multiply_inner_loop_parallel(A, B, C, N);
                double end_time = omp_get_wtime();
                total_time_outer += (end_time - start_time);
            }
            double avg_time_outer = (total_time_outer / NUM_RUNS); 
            std::cout << N << "," << num_threads << ",Outer," << avg_time_outer << std::endl;
        }

        std::cout << "Parallel Multiplication" << std::endl << std::endl;

        for (int num_threads : thread_counts) {
            omp_set_num_threads(num_threads); 
            double total_time_outer = 0.0;

            for (int run = 0; run < NUM_RUNS; ++run) {
                double start_time = omp_get_wtime();
                multiply_parallel(A, B, C, N);
                double end_time = omp_get_wtime();
                total_time_outer += (end_time - start_time);
            }
            double avg_time_outer = (total_time_outer / NUM_RUNS); 
            std::cout << N << "," << num_threads << ",Outer," << avg_time_outer << std::endl;
        }

        for (int num_threads : thread_counts) {
            omp_set_num_threads(num_threads); 
            double total_time_sequential = 0.0;

            for (int run = 0; run < NUM_RUNS; ++run) {
                double start_time = omp_get_wtime();
                multiply_sequential(A, B, C, N);
                double end_time = omp_get_wtime();
                total_time_sequential += (end_time - start_time);
            }
            double avg_time_sequential = (total_time_sequential / NUM_RUNS); 
            std::cout << N << "," << num_threads << ",sequential," << avg_time_sequential << std::endl;
        }

     }

    
    return 0;
}

void printMatrix(std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> matrixPopulator(int r, int c) {
    std::vector<std::vector<int>> matrix(r, std::vector<int>(c));
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            matrix[i][j] = rand() % 10; // Random values between 0 and 9
        }
    }
    return matrix;
}

void multiply_outer_loop_parallel(
     std::vector<std::vector<int>>& A,
     std::vector<std::vector<int>>& B,
    std::vector<std::vector<int>>& C,
    int N) {

    #pragma omp parallel for shared(A, B, C, N) schedule(static)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiply_inner_loop_parallel(
     std::vector<std::vector<int>>& A,
     std::vector<std::vector<int>>& B,
    std::vector<std::vector<int>>& C,
    int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int sum_val = 0;
            #pragma omp parallel for shared(A, B, N, i, j) reduction(+:sum_val) schedule(static)
            for (int k = 0; k < N; ++k) {
                sum_val += A[i][k] * B[k][j];
            }
            C[i][j] = sum_val;
        }
    }
}


void multiply_parallel(
     std::vector<std::vector<int>>& A,
     std::vector<std::vector<int>>& B,
    std::vector<std::vector<int>>& C,
    int N) {
    #pragma omp parallel shared(A, B, C, N)
    {
        #pragma omp for collapse(2) schedule(static)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                C[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
}


void multiply_sequential(
     std::vector<std::vector<int>>& A,
     std::vector<std::vector<int>>& B,
    std::vector<std::vector<int>>& C,
    int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}