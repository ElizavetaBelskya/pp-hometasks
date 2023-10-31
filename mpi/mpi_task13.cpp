//
// Created by Елизавета on 05.11.2023.
//


#include <stdio.h>
#include <mpi.h>

#define MATRIX_SIZE 100

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

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != MATRIX_SIZE) {
        printf("This program should be run with %d processes. Exiting...\n", MATRIX_SIZE);
        MPI_Finalize();
        return 1;
    }

    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    int vector[MATRIX_SIZE];
    int result[MATRIX_SIZE];
    int temp_vector[MATRIX_SIZE];

    if (rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrix[i][j] = i + j;
            }
        }

        for (int i = 0; i < MATRIX_SIZE; i++) {
            vector[i] = i;
        }
    }

    MPI_Scatter(&matrix[0], MATRIX_SIZE, MPI_INT, temp_vector, MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    int local_result = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        local_result += temp_vector[i] * vector[i];
    }

    MPI_Gather(&local_result, 1, MPI_INT, result, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Matrix:\n");
        printMatrix(matrix);

        printf("Vector:\n");
        printVector(vector);

        printf("Result:\n");
        printVector(result);
    }

    MPI_Finalize();
    return 0;
}
