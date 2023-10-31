//
// Created by Елизавета on 04.11.2023.
//

#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define VECTOR_SIZE 100

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (VECTOR_SIZE % world_size != 0) {
        printf("Incorrect count of process\n");
        MPI_Finalize();
        return 1;
    }

    int local_size = VECTOR_SIZE / world_size;
    int vector[VECTOR_SIZE];

    int local_vector[local_size];

    if (world_rank == 0) {
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] = i;
        }
        MPI_Scatter(vector, local_size, MPI_INT, local_vector,
                    local_size, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatter(NULL, local_size, MPI_INT, local_vector,
                    local_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int partial_norm = 0;

    for (int i = 0; i < local_size; i++) {
        partial_norm += abs(local_vector[i]);
    }

    int global_norm;
    MPI_Reduce(&partial_norm, &global_norm, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (world_rank == 0) {
        printf("Norm of the vector: %d\n", global_norm);
    }

    MPI_Finalize();
    return 0;
}
