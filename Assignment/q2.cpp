#include <iostream>
#include <vector>
#include <omp.h>


int main() {
    int n = 10;
    std::vector<int> a(n, 0);
    std::vector<int> b(n, 0);
    std::vector<int> c(n, 0);

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < n; i++) {
            a[i] = i;
            b[i] = i;
        }

        #pragma omp for
        for (int i = 0; i < n; i++) {
            c[i] = a[i] + b[i];
        }
    }

    for (int i = 0; i < n; i++) {
        std::cout << "c[" << i << "] = " << c[i] << std::endl;
    }

    return 0;
}