//
// Created by Елизавета on 19.09.2023.
//

#include <iostream>
#include <cstdlib>

double random(double min, double max) {
    return min + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (max - min)));
}

int main() {
    const int numRows = 6;
    const int numCols = 8;
    double d[numRows][numCols];

    #pragma omp parallel for
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            d[i][j] = random(1.0, 100.0);
        }
    }

    double minVal = d[0][0];
    double maxVal = d[0][0];

    #pragma omp parallel for
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
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
