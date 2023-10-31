//
// Created by Елизавета on 05.11.2023.
//

#include <stdio.h>
#include <mpi.h>

#define VECTOR_SIZE 100

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (VECTOR_SIZE % world_size != 0) {
        printf("Incorrect count of process\n");
        MPI_Finalize();
        return 1;
    }

    int local_size = VECTOR_SIZE / world_size;
    int local_vector_x[local_size];
    int local_vector_y[local_size];

    if (world_rank == 0) {
        int x[VECTOR_SIZE];
        int y[VECTOR_SIZE];

        for (int i = 0; i < VECTOR_SIZE; i++) {
            x[i] = i;
            y[i] = i;
        }

        MPI_Scatter(x, local_size, MPI_INT, local_vector_x, local_size,
                    MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(y, local_size, MPI_INT, local_vector_y, local_size,
                    MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatter(NULL, local_size, MPI_INT, local_vector_x, local_size,
                    MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, local_size, MPI_INT, local_vector_y, local_size,
                    MPI_INT, 0, MPI_COMM_WORLD);
    }

    int partial_scalar = 0;
    for (int i = 0; i < local_size; i++) {
        partial_scalar += local_vector_x[i] * local_vector_y[i];
    }

    int global_scalar = 0;

    MPI_Reduce(&partial_scalar, &global_scalar, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("Scalar Product: %d\n", global_scalar);
    }

    MPI_Finalize();
    return 0;
}
