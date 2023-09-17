//
// Created by Елизавета on 17.09.2023.
//

#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

int main() {
    omp_lock_t mutex;
    omp_init_lock(&mutex);

    const int rows = 6;
    const int cols = 8;
    int d[rows][cols];

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            d[i][j] = std::rand() % 100;
            std::cout << d[i][j] << std::endl;
        }
    }

    float average = 0;
    int min = d[0][0];
    int max = d[0][0];
    int count = 0;

#pragma omp parallel sections
    {
#pragma omp section
        {
            int threadNum = omp_get_thread_num();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    average += d[i][j];
                }
            }
            average /= (rows * cols);
            omp_set_lock(&mutex);
            std::cout << "Section 1 (Thread " << threadNum << "): Average = " << average << std::endl;
            omp_unset_lock(&mutex);
        }

#pragma omp section
        {
            int threadNum = omp_get_thread_num();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (d[i][j] < min) {
                        min = d[i][j];
                    }
                    if (d[i][j] > max) {
                        max = d[i][j];
                    }
                }
            }
            omp_set_lock(&mutex);
            std::cout << "Section 2 (Thread " << threadNum << "): Min = " << min << ", Max = " << max << std::endl;
            omp_unset_lock(&mutex);
        }

#pragma omp section
        {
            int threadNum = omp_get_thread_num();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (d[i][j] % 3 == 0) {
                        count++;
                    }
                }
            }
            omp_set_lock(&mutex);
            std::cout << "Section 3 (Thread " << threadNum << "): Count = " << count << std::endl;
            omp_unset_lock(&mutex);
        }
    }

    omp_destroy_lock(&mutex);
    return 0;
}



