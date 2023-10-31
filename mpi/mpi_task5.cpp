//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "This program should be run with at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    int number_to_send = rank * 10;
    int received_numbers[size];

    if (rank == 0) {
        for (int source_rank = 1; source_rank < size; source_rank++) {
            MPI_Recv(&received_numbers[source_rank], 1, MPI_INT, source_rank, source_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        for (int source_rank = 1; source_rank < size; source_rank++) {
            printf("Process %d sent: %d\n", source_rank, received_numbers[source_rank]);
        }
    } else {
        MPI_Send(&number_to_send, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

