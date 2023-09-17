//
// Created by Елизавета on 17.09.2023.
//

#include <iostream>
#include <string>
#include <omp.h>

std::string createMessage(int threadNum, int numThreads) {
    return "Hello world " + std::to_string(threadNum) + " of " + std::to_string(numThreads);
}

int main() {
#pragma omp parallel num_threads(8)
    {
        int threadNum = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
        std::string message = createMessage(threadNum, numThreads);
        std::cout << message << std::endl;
    }

    return 0;
}


//Всегда ли вывод идентичен? Почему?
//Порядок, в котором потоки выполняются и выводят эту информацию, зависит от операционной системы,
// планировщика потоков и других факторов.