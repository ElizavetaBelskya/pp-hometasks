//
// Created by Елизавета on 03.12.2023.
//
#include <mpi.h>
#include <iostream>
#include <random>


const int group_amount = 5;
const int N = 10;

void printArray(const float* array, int length, int rank) {
    std::cout << "Process " << rank << ": ";
    for (int i = 0; i < length; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {

    int* ranks = new int[]{8, 3, 9, 1, 6};
    int rank, size;
    float array[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 10) {
        std::cerr << "Only 10 processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Group wgroup, group1, group2;
    MPI_Comm_group(MPI_COMM_WORLD, &wgroup);
    MPI_Group_incl(wgroup, group_amount, ranks, &group1);
    MPI_Group_excl(wgroup, group_amount, ranks, &group2);

    MPI_Comm comm1, comm2;
    MPI_Comm_create(MPI_COMM_WORLD, group1, &comm1);
    MPI_Comm_create(MPI_COMM_WORLD, group2, &comm2);

    if (comm1 != MPI_COMM_NULL) {
        MPI_Comm_rank(comm1, &rank);
    }
    if (comm2 != MPI_COMM_NULL) {
        MPI_Comm_rank(comm2, &rank);
    }

    if (comm1 != MPI_COMM_NULL) {
        if (rank == 0) {
            std::cout << "Initial array: ";
            for (int i = 0; i < N; i++) {
                array[i] = i / 10.0;
            }
            printArray(array, N, rank);
        }

        MPI_Bcast(&array[0], N, MPI_FLOAT, 0, comm1);
        printArray(array, N, rank);

    }

    if (comm1 != MPI_COMM_NULL && rank == group_amount - 1) {
        MPI_Send(&array[0], N, MPI_FLOAT, 0, 1, comm1);
    }

    if (comm1 != MPI_COMM_NULL && rank == 0) {
        float received[N];

        MPI_Recv(&received[0], N, MPI_FLOAT, group_amount - 1, 1, comm1, MPI_STATUSES_IGNORE);
        std::cout << "Final array: ";

        printArray(received, N, rank);
    }


    if (comm1 != MPI_COMM_NULL) {
        MPI_Comm_free(&comm1);
    }
    if (comm2 != MPI_COMM_NULL) {
        MPI_Comm_free(&comm2);
    }

    MPI_Group_free(&group1);
    MPI_Group_free(&group2);

    MPI_Finalize();
    delete[] ranks;

}

