//
// Created by Елизавета on 22.09.2023.
//
#include <iostream>
#include <omp.h>
#include <limits>

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)  {
            return false;
        }
    }

    return true;
}

int main() {
    int start, end;
    do {
        std::cin >> start;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input error." << std::endl;
        } else {
            break;
        }
    } while (true);

    do {
        std::cin >> end;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input error." << std::endl;
        } else {
            break;
        }
    } while (true);

    if (start > end) {
        std::swap(start, end);
    }

    int max_threads = omp_get_max_threads();
    int chunk_size = (end - start + 1) / max_threads;
    omp_set_num_threads(max_threads);

    #pragma omp parallel for schedule(static, chunk_size)
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            #pragma omp critical
            {
                std::cout << i << " ";
            }
        }
    }

    std::cout << std::endl;

    return 0;
}