//
// Created by Елизавета on 17.09.2023.
//

#include <iostream>
#include <omp.h>

int main() {
    int a[10] = {5, 10, 2, 8, 7, 1, 3, 6, 4, 9};
    int b[10] = {15, 12, 20, 18, 17, 11, 13, 16, 14, 19};
    int minA = a[0];
    int maxB = b[0];

    #pragma omp parallel
    {
        int threadNum = omp_get_thread_num();
        if (threadNum == 0) {
            for (int i = 1; i < 10; i++) {
                if (a[i] < minA) {
                    minA = a[i];
                }
            }
        } else if (threadNum == 1) {
            for (int i = 1; i < 10; i++) {
                if (b[i] > maxB) {
                    maxB = b[i];
                }
            }
        }
    }

    std::cout << "Min a: " << minA << std::endl;
    std::cout << "Max b: " << maxB << std::endl;

    return 0;
}

