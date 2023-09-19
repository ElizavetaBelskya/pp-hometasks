#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>


int main() {
    int a[30];
    int count = 0;

    srand(time(NULL));
    for (int i = 0; i < 30; i++) {
        a[i] = rand() % 100;
    }

    #pragma omp parallel for num_threads(5)
    for (int i = 0; i < 30; i++) {
        if (a[i] % 9 == 0) {
            #pragma omp atomic
            count++;
        }
    }

    std::cout << count;

    return 0;
}
