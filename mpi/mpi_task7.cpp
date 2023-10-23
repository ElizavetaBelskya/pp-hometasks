//
// Created by Елизавета on 18.10.2023.
//


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 10
#define VECTOR_SIZE 10

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size < 2) {
        printf("This program should be run with at least 2 processes. Exiting...\n");
        MPI_Finalize();
        return 1;
    }

    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    int vector[MATRIX_SIZE];
    int result[MATRIX_SIZE];

    if (world_rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrix[i][j] = i + j;
            }
            vector[i] = i;
        }
    }


    MPI_Bcast(matrix, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[i] = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    MPI_Reduce(result, vector, MATRIX_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("Result vector: ");
        for (int i = 0; i < MATRIX_SIZE; i++) {
            printf("%d ", vector[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
