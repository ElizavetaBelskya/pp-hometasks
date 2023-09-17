//
// Created by Елизавета on 17.09.2023.
//
#include <iostream>
#include <omp.h>

int main() {
    int a = 0;
    int b = 0;
    omp_lock_t mutex;
    omp_init_lock(&mutex);

    std::cout << "Before: a = " << a << ", b = " << b << std::endl;

#pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        int threadNum = omp_get_thread_num();
        a += threadNum;
        b += threadNum;
        omp_set_lock(&mutex);
        std::cout << "Inside 1 (Thread " << threadNum << "): a = " << a << ", b = " << b << std::endl;
        omp_unset_lock(&mutex);
    }

    std::cout << "After: a = " << a << ", b = " << b << std::endl;
    std::cout << "Before: a = " << a << ", b = " << b << std::endl;


#pragma omp parallel num_threads(4) shared(a) private(b)
    {
        int threadNum = omp_get_thread_num();
        a -= threadNum;
        b -= threadNum;
        omp_set_lock(&mutex);
        std::cout << "Inside 2 (Thread " << threadNum << "): a = " << a << ", b = " << b << std::endl;
        omp_unset_lock(&mutex);
    }

    std::cout << "After: a = " << a << ", b = " << b << std::endl;

    omp_destroy_lock(&mutex);

    return 0;
}
