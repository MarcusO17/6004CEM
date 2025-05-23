#include <iostream>
#include <omp.h>
#include <vector>

void printMatrix(std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

}

std::vector<std::vector<int>> matrixMultiplication(const std::vector<std::vector<int>>& A, 
                                                   const std::vector<std::vector<int>>& B) {
    int r1 = A.size();
    int c1 = A[0].size();
    int r2 = B.size();
    int c2 = B[0].size();


    std::vector<std::vector<int>> C(r1, std::vector<int>(c2, 0));

    for (int i = 0; i < r1; ++i) {         
        for (int j = 0; j < c2; ++j) {    
            for (int k = 0; k < c1; ++k) { 
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}                           

std::vector<std::vector<int>> matrixPopulator(r,c) {
    std::vector<std::vector<int>> matrix(r, std::vector<int>(c));
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            matrix[i][j] = rand() % 10;
        }
    }
    return matrix;
}



int main() {
    int r1, c1, r2, c2;

    std::cout << "Enter rows and columns for first matrix: ";
    std::cin >> r1 >> c1;
    std::cout << "Enter rows and columns for second matrix: ";
    std::cin >> r2 >> c2;

    if (c1 != r2) {
        std::cerr << "Matrix multiplication not possible!" << std::endl;
        return 1;
    }

    // Populate matrices
    std::vector<std::vector<int>> A = matrixPopulator(r1, c1);
    std::vector<std::vector<int>> B = matrixPopulator(r2, c2);

    // Print matrices
    std::cout << "Matrix A:" << std::endl;
    printMatrix(A);
    std::cout << "Matrix B:" << std::endl;
    printMatrix(B);

    // Perform multiplication
    auto C = matrixMultiplication(A, B);

    // Print result
    std::cout << "Resultant Matrix C:" << std::endl;
    printMatrix(C);

    return 0;
}