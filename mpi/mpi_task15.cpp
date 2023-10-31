//
// Created by Елизавета on 05.11.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define VECTOR_SIZE 100

void printVector(int v[VECTOR_SIZE]) {
    for (int i = 0; i < VECTOR_SIZE; i++) {
        printf("%d ", v[i]);
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (VECTOR_SIZE % world_size != 0) {
        printf("Incorrect count of processes\n");
        MPI_Finalize();
        return 1;
    }

    int local_size = VECTOR_SIZE / world_size;
    int x[VECTOR_SIZE];
    int local_max;
    int local_max_index;

    if (world_rank == 0) {
        for (int i = 0; i < VECTOR_SIZE; i++) {
            x[i] = rand() % 100;
        }
    }

    int local_x[local_size];
    MPI_Scatter(x, local_size, MPI_INT, local_x, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    local_max = local_x[0];
    local_max_index = 0;

    for (int i = 0; i < local_size; i++) {
        if (local_x[i] > local_max) {
            local_max = local_x[i];
        }
    }

    int* all_local_max = (int*)malloc(world_size * sizeof(int));

    MPI_Gather(&local_max, 1, MPI_INT, all_local_max, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (world_rank == 0) {
        printVector(x);
        for (int i = 0; i < world_size; i++) {
            printf("Local max %d on a process %d\n", all_local_max[i],  i);
        }
    }

    free(all_local_max);

    MPI_Finalize();
    return 0;
}
