//
// Created by Елизавета on 17.09.2023.
//

#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <cstdlib>

double random(double min, double max) {
    return min + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (max - min)));
}

int main() {

    const int rows = 6;
    const int cols = 8;
    int d[rows][cols];

    int minValue = d[0][0];
    int maxValue = d[0][0];
    int count = 0;
    int sum = 0;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            d[i][j] = random(0, 99);
            if (d[i][j] < minValue) {
                minValue = d[i][j];
            }
            if (d[i][j] > maxValue) {
                maxValue = d[i][j];
            }
            if (d[i][j] % 3 == 0) {
                count++;
            }
            sum += d[i][j];
        }
    }

    std::cout << "Min Value: " << minValue << std::endl;
    std::cout << "Max Value: " << maxValue << std::endl;
    std::cout << "Count of Multiples of 3: " << count << std::endl;
    if (rows > 0 && cols > 0) {
        double average = static_cast<double>(sum) / (rows * cols);
        std::cout << "Average: " << average << std::endl;
    }

    minValue = d[0][0];
    maxValue = d[0][0];
    count = 0;
    sum = 0;

#pragma omp parallel sections
    {
        #pragma omp section
        {
            int threadNum = omp_get_thread_num();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    sum += d[i][j];
                }
            }
            double average = static_cast<double>(sum) / (rows * cols);
            #pragma omp critical
            {
                std::cout << "Section 1 (Thread " << threadNum << "): Average = " << average << std::endl;
            }
        }

        #pragma omp section
        {
            int threadNum = omp_get_thread_num();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (d[i][j] < minValue) {
                        minValue = d[i][j];
                    }
                    if (d[i][j] > maxValue) {
                        maxValue = d[i][j];
                    }
                }
            }
            #pragma omp critical
            {
                std::cout << "Section 2 (Thread " << threadNum << "): Min = " << minValue << ", Max = " << maxValue << std::endl;
            }
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
            #pragma omp critical
            {
                std::cout << "Section 3 (Thread " << threadNum << "): Count = " << count << std::endl;
            }
        }
    }


    return 0;
}



