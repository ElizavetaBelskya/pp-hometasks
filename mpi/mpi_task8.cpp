//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <mpi.h>

#define MATRIX_SIZE 10

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int matrixA[MATRIX_SIZE][MATRIX_SIZE];
    int matrixB[MATRIX_SIZE][MATRIX_SIZE];
    int result[MATRIX_SIZE][MATRIX_SIZE];
    MPI_Status status;
    int elements_per_process = 5;
    if (world_rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrixA[i][j] = i + j;
                matrixB[i][j] = j - i;
            }
        }
        printMatrix(matrixA);
        printMatrix(matrixB);
        int n = 1;
        for (int i = 0; i < MATRIX_SIZE; i += elements_per_process) {
            for (int j = 0; j < elements_per_process; j++) {
                MPI_Send(&matrixA[i+j][0], MATRIX_SIZE, MPI_INT, n, n*j+n, MPI_COMM_WORLD);
                MPI_Send(&matrixB[i+j][0], MATRIX_SIZE, MPI_INT, n, n*j+n, MPI_COMM_WORLD);
            }
            n++;
        }
        n = 1;
        for (int i = 0; i < MATRIX_SIZE; i += elements_per_process) {
            for (int j = 0; j < elements_per_process; j++) {
                MPI_Recv(&result[i+j][0], MATRIX_SIZE, MPI_INT, n, n*j + n, MPI_COMM_WORLD, &status);
            }
            n++;
        }

    } else if (world_rank <= MATRIX_SIZE / elements_per_process)  {
        int a[MATRIX_SIZE];
        int b[MATRIX_SIZE];
        for (int j = 0; j < elements_per_process; j++) {
            MPI_Recv(&a[0], MATRIX_SIZE, MPI_INT, 0, world_rank*j + world_rank, MPI_COMM_WORLD, &status);
            MPI_Recv(&b[0], MATRIX_SIZE, MPI_INT, 0, world_rank*j + world_rank, MPI_COMM_WORLD, &status);
            int res[MATRIX_SIZE];
            for (int k = 0; k < MATRIX_SIZE; k++) {
                res[k] = a[k] * b[k];
            }
            MPI_Send(&res, MATRIX_SIZE, MPI_INT, 0, world_rank*j + world_rank, MPI_COMM_WORLD);
        }

    }

    if (world_rank == 0) {
        printf("Result matrix:\n");
        printMatrix(result);
    }

    MPI_Finalize();
    return 0;
}
