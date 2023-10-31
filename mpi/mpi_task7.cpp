//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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

    if (world_size < MATRIX_SIZE) {
        printf("This program should be run with at least MATRIX SIZE. Exiting...\n");
        MPI_Finalize();
        return 1;
    }

    MPI_Status status;
    int count;

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
        int elements_per_process = MATRIX_SIZE;
        int n = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            MPI_Send(&matrixA[i][0], elements_per_process, MPI_INT, n, 10, MPI_COMM_WORLD);
            MPI_Send(&vector, MATRIX_SIZE, MPI_INT, n, 10, MPI_COMM_WORLD);
            n++;
        }

        n = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            MPI_Recv(&result[i], 1, MPI_INT, n, 10, MPI_COMM_WORLD, &status);
            n++;
        }
        printVector(result);

    }
    else {
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int vectorA[MATRIX_SIZE];
        int vectorB[MATRIX_SIZE];
        int c = 0;

        MPI_Recv(&vectorA[0], count, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&vectorB[0], count, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; ++i) {
            c += vectorA[i] * vectorB[i];
        }

        MPI_Send(&c, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}