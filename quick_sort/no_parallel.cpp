#include <iostream>
#include <chrono>

constexpr size_t n = 100;

void quicksort(int *arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[i + 1], arr[high]);
        pivot = i + 1;

        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

int main() {
    srand(time(0));

    int *arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    // std::cout << "Array before sorting: ";
    // for (int i = 0; i < n; i++) {
    //     std::cout << arr[i] << " ";
    // }
    // std::cout << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    quicksort(arr, 0, n - 1);

    auto end_time = std::chrono::high_resolution_clock::now();

//     std::cout << "Array after sorting: ";
//     for (int i = 0; i < n; i++) {
//         std::cout << arr[i] << " ";
//     }
//     std::cout << std::endl;

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

    std::cout << "Time: " << duration.count() << std::endl;

    delete[] arr;

    return 0;
}
