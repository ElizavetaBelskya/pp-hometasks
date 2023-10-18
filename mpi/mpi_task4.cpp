//
// Created by Елизавета on 18.10.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define ARRAY_SIZE 10

#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int number = world_rank * 10;

    if (world_rank == 0) {
        int received_numbers[world_size];
        received_numbers[0] = number;

        for (int source_rank = 1; source_rank < world_size; source_rank++) {
            MPI_Recv(&received_numbers[source_rank], 1, MPI_INT, source_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        printf("Received numbers in ascending order of process ranks: ");
        for (int i = 0; i < world_size; i++) {
            printf("%d ", received_numbers[i]);
        }
        printf("\n");
    } else {
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}


