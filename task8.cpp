//
// Created by Елизавета on 18.09.2023.
//

#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    const int rows = 1000;
    const int cols = 1000;

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vector(cols);
    std::vector<int> result(rows);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * 10 + j * 8;
        }
        vector[i] = i * 20;
    }

    // Вычисление времени выполнения последовательной версии
    double startTimeSeq = omp_get_wtime();
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    double endTimeSeq = omp_get_wtime();

    std::cout << "Time (sequential): " << endTimeSeq - startTimeSeq << " seconds" << std::endl;

    // Вычисление времени выполнения параллельной версии
    double startTimePar = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    double endTimePar = omp_get_wtime();

    std::cout << "Time (parallel): " << endTimePar - startTimePar << " seconds" << std::endl;

    return 0;
}

//параллельное выполнение в 1.5-2 раза быстрее