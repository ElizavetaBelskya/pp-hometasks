//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <mpi.h>

#define VECTOR_SIZE 100
#define A 2.0
#define B 3.0

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int elements_per_process = VECTOR_SIZE / world_size;

    double x[VECTOR_SIZE];
    double y[VECTOR_SIZE];
    double local_x[elements_per_process];
    double local_y[elements_per_process];
    double local_z[elements_per_process];

    if (world_rank == 0) {
        for (int i = 0; i < VECTOR_SIZE; i++) {
            x[i] = i + 1;
            y[i] = i + 1;
            printf("%.2f ", x[i]);
            printf("%.2f ", y[i]);
            printf("\n");

        }
    }

    MPI_Scatter(x, elements_per_process, MPI_DOUBLE, local_x, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, elements_per_process, MPI_DOUBLE, local_y, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < elements_per_process; i++) {
        local_z[i] = A * local_x[i] + B * local_y[i];
    }

    MPI_Gather(local_z, elements_per_process, MPI_DOUBLE, x, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("Resulting vector z: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            printf("%.2f ", local_z[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
