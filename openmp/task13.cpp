//
// Created by Елизавета on 22.09.2023.
//

#include <iostream>
#include <omp.h>

int main() {
    int n = 30;
    int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
    int p = 2;
    int result = 0;
    int power = 1;
    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < n; i++) {
        result += a[n - i - 1] * power;
        power *= p;
    }

    std::cout << "Result: " << result << std::endl;

    return 0;
}

//Result: 756893125