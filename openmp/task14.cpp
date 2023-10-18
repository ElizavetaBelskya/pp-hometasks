//
// Created by Елизавета on 22.09.2023.
//

#include <iostream>

int main() {
    int num = 210;
    int square = 0;

    #pragma omp parallel for reduction(+:square)
    for (int i = 1; i <= num; i++) {
        square += (2 * i - 1);
    }

    std::cout << square << std::endl;

    return 0;
}
