//
// Created by Елизавета on 22.09.2023.
//

#include <iostream>
#include <omp.h>

std::string createMessage(int threadNum, int numThreads) {
    return "Hello world " + std::to_string(threadNum) + " of " + std::to_string(numThreads) + "\n";
}

//самый простой способ через сохранение в массив
int main() {
    int num_threads = 8;
    int i = 0;
    omp_set_num_threads(num_threads);

    int thread_ids[8];

    #pragma omp parallel
    {
        int threadNum = omp_get_thread_num();
        int numThreads = omp_get_num_threads();

        thread_ids[i] = threadNum;
        i++;

        #pragma omp critical
        {
            std::string message = createMessage(threadNum, numThreads);
            std::cout << message;
        }
    }


    for (int i = num_threads - 1; i >= 0; i--) {
        std::cout << thread_ids[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

