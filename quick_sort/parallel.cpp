#include <iostream>
#include <mpi.h>
#include <random>
#include <ctime>
#include <chrono>

constexpr size_t n = 10000;

void quicksort(int* arr, int low, int high) {
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

void init_sendcounts(int* sendcounts, int size) {
    for (int i = 0; i < size; i++) {
        sendcounts[i] = n / size;
    }
   
    if (n % size != 0) {
        int mod = n % size;
        int i = 0;
        while (mod != 0) {
            sendcounts[i % size]++;
            i++;
            mod--;
        }
    }
}

void init_displs(int* displs, int* sendcounts, int size) {
    displs[0] = 0;
    for (int i = 1; i < size; i++) {
        displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
}

int* merge(int size, int *sendcounts, int *displs, int *arr) {
    int *merged = new int[n];
    int pos = 0;

    int *loc_pos = new int[size]{0};
    int *rest = new int[size];
    for (int i = 0; i < size; i++) {
        rest[i] = sendcounts[i];
    } 

    while (pos != n) {
        int min;
        int rank;
        for (int i = 0; i < size; i++) {
            if (rest[i] != 0) {
                min = arr[displs[i] + loc_pos[i]];
                rank = i;
                break;
            }
        }

        for (int i = rank + 1; i < size; i++) {
            if (rest[i] != 0 && min > arr[displs[i] + loc_pos[i]]) {
                min = arr[displs[i] + loc_pos[i]];
                rank = i;
            }
        }

        merged[pos] = min;
        pos++;

        loc_pos[rank]++;
        rest[rank]--;
    }

    return merged;
}

int main(int argc, char **argv) {
    srand(time(0));

    MPI_Init(&argc, &argv);
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* sendcounts = new int[size];
    int* displs = new int[size];

    init_sendcounts(sendcounts, size);
    init_displs(displs, sendcounts, size);

    int* arr;
    int* loc_arr = new int[sendcounts[rank]];

    if (rank == 0) {
        arr = new int[n];
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 1000;
        }

//        std::cout << "Array before sorting: ";
//        for (int i = 0; i < n; i++) {
//            std::cout << arr[i] << " ";
//        }
//        std::cout << std::endl;

    }

    auto start_time = std::chrono::high_resolution_clock::now();

    MPI_Scatterv((void*)arr, sendcounts, displs, MPI_INT, (void*)loc_arr, sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    quicksort(loc_arr, 0, sendcounts[rank] - 1);

    MPI_Gatherv((void*)loc_arr, sendcounts[rank], MPI_INT, (void*)arr, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int *merged_arr = merge(size, sendcounts, displs, arr);
        auto end_time = std::chrono::high_resolution_clock::now();

        delete[] arr;
        arr = merged_arr;
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        std::cout << "Time: " << duration.count() << std::endl;
//        std::cout << "Array after sorting: ";
//        for (int i = 0; i < n; i++) {
//            std::cout << arr[i] << " ";
//        }
        std::cout << std::endl;
    }
   
    MPI_Finalize();

    delete[] sendcounts;
    delete[] displs;
    delete[] loc_arr;
    if (rank == 0) {
        delete[] arr;
    }

    return 0;
}
