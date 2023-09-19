//
// Created by Елизавета on 18.09.2023.
//
#include <iostream>
#include <omp.h>

int main() {
    int a[12], b[12], c[12];
    #pragma omp parallel for schedule(static, 4) num_threads(3)
    for (int i = 0; i < 12; i++) {
        a[i] = i;
        b[i] = i;
    #pragma omp critical
        {
            int threadNum = omp_get_thread_num();
            int numThreads = omp_get_num_threads();
            std::cout << "Thread " << threadNum << " of " << numThreads << std::endl;
        }
    }

    #pragma omp parallel for schedule(dynamic, 3) num_threads(4)
    for (int i = 0; i < 12; i++) {
        c[i] = a[i] + b[i];
        #pragma omp critical
        {
            int threadNum = omp_get_thread_num();
            int numThreads = omp_get_num_threads();
            std::cout << "Thread " << threadNum << " of " << numThreads << std::endl;
        }
    }


    std::cout << "Array a: ";
    for (int i = 0; i < 12; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Array b: ";
    for (int i = 0; i < 12; i++) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Array c: ";
    for (int i = 0; i < 12; i++) {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

//разница между статическим и динамическим распределением
//При статическом распределении итерации цикла равномерно разделяются между потоками на
// этапе начала выполнения цикла
//При динамическом распределении итерации цикла динамически назначаются потокам по мере их доступности.