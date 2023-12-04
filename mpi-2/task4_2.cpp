//
// Created by Елизавета on 25.11.2023.
//

#include <mpi.h>
#include <iostream>
const int n = 12;

void printArray(const int* array, int length, int rank) {
    std::cout << "Process " << rank << ": ";
    for (int i = 0; i < length; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* array = new int[n];
    int step = 12 / size;

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            array[i] = i;
        }

        int start = step + 1;
        for (int i = 1; i < size; i++) {
            MPI_Send(array + start, step, MPI_INT, i, i, MPI_COMM_WORLD);
            start += step;
        }

        int* step_array = new int[step + 1];
        for (int i = 0; i < step + 1; i++) {
            step_array[i] = array[i];
        }
        printArray(step_array, step + 1, rank);
    } else {
        int* step_array = new int[step];
        MPI_Recv(step_array, step, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printArray(step_array, step, rank);
    }


    delete[] array;

    MPI_Finalize();
    return 0;
}
