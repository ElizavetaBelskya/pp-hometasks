//
// Created by Елизавета on 17.09.2023.
//

#include <iostream>
#include <omp.h>


int makeThreads(int numThreads, int area) {
#pragma omp parallel if (numThreads > 1) num_threads(numThreads)
    {
        int threadNum = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
#pragma omp critical
        {
            std::cout << "Parallel area " << area << " Thread " << threadNum << " of " << numThreads << std::endl;
        }
    }
    return 0;
}

int main() {
    makeThreads(3, 1);
    makeThreads(1, 1);
    return 0;
}

//если количество потоков меньше 2, то код выполнится 1 раз (даже если написать отрицательное число)