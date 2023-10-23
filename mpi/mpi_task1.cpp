#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4) {
        printf("This program should be run with 4 processes. Exiting...\n");
        MPI_Finalize();
        return 1;
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello, World!\n", rank, size);

    MPI_Finalize();
    return 0;
}

