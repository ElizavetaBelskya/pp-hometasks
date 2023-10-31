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

    if (world_size - 1 < MATRIX_SIZE) {
        printf("This program should be run with at least %d processes. Exiting...\n", MATRIX_SIZE);
        MPI_Finalize();
        return 1;
    }

    int matrixA[MATRIX_SIZE][MATRIX_SIZE];
    int matrixB[MATRIX_SIZE][MATRIX_SIZE];
    int result[MATRIX_SIZE][MATRIX_SIZE];
    MPI_Status status;
    int count;

    if (world_rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrixA[i][j] = i + j;
                matrixB[i][j] = j - i;
            }
        }
        printMatrix(matrixA);
        printMatrix(matrixB);
        int elements_per_process = MATRIX_SIZE;
        int n = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            MPI_Send(&matrixA[i][0], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD);
            MPI_Send(&matrixB[i][0], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD);
            n++;
        }

        n = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            MPI_Recv(&result[i][0], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD, &status);
            n++;
        }

    } else {
        MPI_Probe(0, world_rank, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int a[MATRIX_SIZE];
        int b[MATRIX_SIZE];
        int res[MATRIX_SIZE];

        MPI_Recv(&a[0], count, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
        MPI_Recv(&b[0], count, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; ++i) {
            res[i] += a[i] * b[i];
        }
        MPI_Send(&res, count, MPI_INT, 0, world_rank, MPI_COMM_WORLD);

    }


    if (world_rank == 0) {
        printf("Result matrix:\n");
        printMatrix(result);
    }

    MPI_Finalize();
    return 0;
}
