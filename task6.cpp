//
// Created by Елизавета on 18.09.2023.
//

#include <iostream>
#include <omp.h>

int main() {
    const int N = 10;
    int a[N], b[N];

    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i ;
    }

    int sum_a = 0;
    int sum_b = 0;

    #pragma omp parallel for reduction(+:sum_a, sum_b)
    for (int i = 0; i < N; i++) {
        sum_a += a[i];
        sum_b += b[i];
    }

    double avg_a = static_cast<double>(sum_a) / N;
    double avg_b = static_cast<double>(sum_b) / N;

    std::cout << "Average value for a: " << avg_a << std::endl;
    std::cout << "Average value for b: " << avg_b << std::endl;
    return 0;
}
