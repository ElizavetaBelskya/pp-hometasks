//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <mpi.h>

#define VECTOR_SIZE 10
#define A 2.0
#define B 3.0

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    MPI_Status status;
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    //колво процессов -1 должно быть кратно VECTOR_SIZE
    int elements_per_process = VECTOR_SIZE / (world_size - 1);

    if (world_rank == 0) {
        int x[VECTOR_SIZE];
        int y[VECTOR_SIZE];
        int res[VECTOR_SIZE];

        for (int i = 0; i < VECTOR_SIZE; i++) {
            x[i] = i;
            y[i] = i;
            printf("%.0d ", x[i]);
            printf("%.0d ", y[i]);
            printf("\n");
        }


        int n = 1;
        for (int i = 0; i < VECTOR_SIZE && n < world_size; i += elements_per_process) {
            MPI_Send(&x[i], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD);
            MPI_Send(&y[i], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD);
            n++;
        }

        n = 1;
        for (int i = 0; i < VECTOR_SIZE && n < world_size; i += elements_per_process) {
            MPI_Recv(&res[i], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD, &status);
            n++;
        }

        printf("Resulting vector z: ");

        for (int re : res) {
            printf("%.0d ", re);
            printf("\n");
        }

    } else {
        int count;
        int local_x[elements_per_process];
        int local_y[elements_per_process];
        int local_res[elements_per_process];

        MPI_Probe(0, world_rank, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        MPI_Recv(&local_x[0], count, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_y[0], count, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; i++) {
            local_res[i] = A * local_x[i] + B * local_y[i];
        }

        MPI_Send(&local_res[0], count, MPI_INT, 0, world_rank, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}