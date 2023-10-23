//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int my_number = world_rank * 10;
    int received_numbers[world_size];

    MPI_Gather(&my_number, 1, MPI_INT, received_numbers, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("Received numbers in ascending order of ranks:\n");
        for (int i = 0; i < world_size; i++) {
            printf("Process %d: %d\n", i, received_numbers[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
