//
// Created by Елизавета on 18.10.2023.
//
#include <iostream>
#include <mpi.h>
#include <random>

#define MATRIX_SIZE 10

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size < MATRIX_SIZE*MATRIX_SIZE + 1) {
        printf("This program should be run with at least %d processes.\n", MATRIX_SIZE*MATRIX_SIZE + 1);
        MPI_Finalize();
        return 1;
    }

    MPI_Status status;

    if (world_rank == 0) {
        int matrixA[MATRIX_SIZE][MATRIX_SIZE];
        int matrixB[MATRIX_SIZE][MATRIX_SIZE];
        int matrixB_trans[MATRIX_SIZE][MATRIX_SIZE];
        int result[MATRIX_SIZE][MATRIX_SIZE];

        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrixA[i][j] = i + j;
                matrixB[i][j] = j - i;
                matrixB_trans[j][i] = j - i;
            }
        }

        printMatrix(matrixA);
        printMatrix(matrixB);
        int elements_per_process = MATRIX_SIZE;
        int n = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                MPI_Send(&matrixA[i][0], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD);
                MPI_Send(&matrixB_trans[j][0], elements_per_process, MPI_INT, n, n, MPI_COMM_WORLD);
                n++;
            }
        }

        n = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                MPI_Recv(&result[i][j], 1, MPI_INT, n, n, MPI_COMM_WORLD, &status);
                n++;
            }
        }
        printMatrix(result);

    } else {
        int count;
        MPI_Probe(0, world_rank, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int vectorA[MATRIX_SIZE];
        int vectorB[MATRIX_SIZE];
        int c = 0;

        MPI_Recv(&vectorA[0], count, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
        MPI_Recv(&vectorB[0], count, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; ++i) {
            c += vectorA[i] * vectorB[i];
        }

        MPI_Send(&c, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}