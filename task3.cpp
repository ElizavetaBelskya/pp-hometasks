//
// Created by Елизавета on 17.09.2023.
//
#include <iostream>
#include <omp.h>

int main() {
    int a = 0;
    int b = 0;

    std::cout << "Before: a = " << a << ", b = " << b << std::endl;
//private - каждый поток будет иметь свою собственную копию этой переменной,
// но ее начальное значение не предсказуемо
//firstprivate - каждый поток будет работать с копией переменной
#pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        int threadNum = omp_get_thread_num();
        a += threadNum;
        b += threadNum;
        #pragma omp critical
        {
            std::cout << "Inside 1 (Thread " << threadNum << "): a = " << a << ", b = " << b << std::endl;
        }
    }

    std::cout << "After: a = " << a << ", b = " << b << std::endl;
    std::cout << "Before: a = " << a << ", b = " << b << std::endl;

//    все потоки будут иметь доступ к одной и той же общей копии этой переменной,
//    и изменения, сделанные одним потоком, будут видны другим потокам
#pragma omp parallel num_threads(4) shared(a) private(b)
    {
        int threadNum = omp_get_thread_num();
        a -= threadNum;
        b -= threadNum;
        #pragma omp critical
        {
            std::cout << "Inside 2 (Thread " << threadNum << "): a = " << a << ", b = " << b << std::endl;
        }
    }

    std::cout << "After: a = " << a << ", b = " << b << std::endl;
    return 0;
}
