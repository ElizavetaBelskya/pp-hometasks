//
// Created by Елизавета on 17.09.2023.
//

#include <iostream>
#include <omp.h>


int makeThreadsIf(int numThreads, int area) {
    #pragma omp parallel num_threads(numThreads)
    if (numThreads > 1) {
        {
            int threadNum = omp_get_thread_num();
            int numThreads = omp_get_num_threads();
            #pragma omp critical
            {
                std::cout << "Parallel area " << area << " Thread " << threadNum << " of " << numThreads << std::endl;
            }
        }
    }
    return 0;
}

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
    makeThreads(3, 1); //тут 3 раза выполнится, как должно
    makeThreads(1, 2); //тут 1 раз выполнится, потому что 1 раз выполнится в любом случае
    makeThreadsIf(1, 3); //ну тут очевидно не выполниться
    return 0;
}

//если количество потоков меньше 2, то код выполнится 1 раз
// (даже если написать отрицательное число в numThreads)
// можно это обойти если просто написать условие внутри кода,
// который должен выполняться многопоточно
