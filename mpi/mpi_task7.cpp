//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include <mpi.h>
#include <random>

#define MATRIX_SIZE 20

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printVector(int v[MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("%d ", v[i]);
        printf("\n");
    }
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Status status;
    int elements_per_process = 5;

    if (world_rank == 0) {
        int matrixA[MATRIX_SIZE][MATRIX_SIZE];
        int vector[MATRIX_SIZE];
        int result[MATRIX_SIZE];
        for (int i = 0; i < MATRIX_SIZE; i++) {
            vector[i] = i;
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrixA[i][j] = i + j;
            }
        }
        printMatrix(matrixA);
        printVector(vector);
        int n = 1;
        for (int i = 0; i < MATRIX_SIZE; i += elements_per_process) {
            MPI_Send(&vector, MATRIX_SIZE, MPI_INT, n, n, MPI_COMM_WORLD);
            for (int j = 0; j < elements_per_process; j++) {
                MPI_Send(&matrixA[i + j][0], MATRIX_SIZE, MPI_INT, n, n*j+n, MPI_COMM_WORLD);
            }
            n++;
        }

        n = 1;
        for (int i = 0; i < MATRIX_SIZE; i += elements_per_process) {
            for (int j = 0; j < elements_per_process; j++) {
                MPI_Recv(&result[i + j], 1, MPI_INT, n, n*j + n, MPI_COMM_WORLD, &status);
            }
            n++;
        }
        printVector(result);

    } else if (world_rank <= MATRIX_SIZE / elements_per_process) {
        int vectorA[MATRIX_SIZE];
        int vectorB[MATRIX_SIZE];
        MPI_Recv(&vectorA[0], MATRIX_SIZE, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
        for (int j = 0; j < elements_per_process; j++) {
            int c = 0;
            MPI_Recv(&vectorB[0], MATRIX_SIZE, MPI_INT,
                     0, world_rank*j + world_rank, MPI_COMM_WORLD, &status);
            for (int i = 0; i < MATRIX_SIZE; i++) {
                c += vectorA[i] * vectorB[i];
            }
            MPI_Send(&c, 1, MPI_INT, 0, world_rank*j + world_rank, MPI_COMM_WORLD);
        }

    }

    MPI_Finalize();
    return 0;

}