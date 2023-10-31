//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <mpi.h>

#define VECTOR_SIZE 30
#define A 2.0
#define B 3.0

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    MPI_Status status;
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (VECTOR_SIZE % (world_size - 1) != 0) {
        fprintf(stderr, "This program should be run with processes divisible by %d.", VECTOR_SIZE);
        MPI_Finalize();
        return 1;
    } else {
        int elements_per_process = VECTOR_SIZE / (world_size - 1);

        if (world_rank == 0) {
            int x[VECTOR_SIZE];
            int y[VECTOR_SIZE];
            int res[VECTOR_SIZE];

            for (int i = 0; i < VECTOR_SIZE; i++) {
                x[i] = i - 1;
                y[i] = i + 1;
            }

            int pointer = 0;
            for (int n = 1; n < world_size; n++) {
                MPI_Send(&x[pointer], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD);
                MPI_Send(&y[pointer], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD);
                pointer += elements_per_process;
            }

            pointer = 0;
            for (int n = 1; n < world_size; n++) {
                MPI_Recv(&res[pointer], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD, &status);
                pointer += elements_per_process;
            }

            printf("Resulting vector z: ");
            for (int i = 0; i < VECTOR_SIZE; i++) {
                printf("%.0d ", res[i]);
            }
            printf("\n");

            MPI_Finalize();
        } else {
            int local_x[elements_per_process];
            int local_y[elements_per_process];
            int local_res[elements_per_process];
            MPI_Recv(&local_x,  elements_per_process, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
            MPI_Recv(&local_y, elements_per_process, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);

            for (int i = 0; i < elements_per_process; i++) {
                local_res[i] = A * local_x[i] + B * local_y[i];
            }
            MPI_Send(&local_res, elements_per_process, MPI_INT, 0, world_rank, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
