//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("I am process %d of %d\n", rank, size);

    MPI_Finalize();
    return 0;
}
