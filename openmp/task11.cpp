//
// Created by Елизавета on 19.09.2023.
//


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>

int main() {
    int a[100];
    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        a[i] = rand() % 100;
    }

    int maxim = -1;

    #pragma omp parallel for
    for (int i = 0; i < 100; i++) {
        if (a[i] % 7 == 0 && a[i] > maxim) {
            #pragma omp critical
            maxim = a[i];
        }
    }
    if (maxim != -1) {
        std::cout << maxim;
    } else {
        std::cout << "No suitable elements in this array";
    }

    return 0;
}
