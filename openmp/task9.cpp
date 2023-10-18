//
// Created by Елизавета on 19.09.2023.
//

#include <iostream>
#include <cstdlib>

double random(double min, double max) {
    return min + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (max - min)));
}

int main() {
    const int rows = 6;
    const int cols = 8;
    double d[rows][cols];

    #pragma omp parallel for
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            d[i][j] = random(1.0, 100.0);
        }
    }

    double minVal = d[0][0];
    double maxVal = d[0][0];

    #pragma omp parallel for
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double value = d[i][j];

            #pragma omp critical
            {
                if (value < minVal) {
                    minVal = value;
                }
                if (value > maxVal) {
                    maxVal = value;
                }
            }
        }
    }
    
    std::cout << "Min value: " << minVal << std::endl;
    std::cout << "Max value: " << maxVal << std::endl;

    return 0;
}
