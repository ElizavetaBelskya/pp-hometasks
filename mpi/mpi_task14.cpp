//
// Created by Елизавета on 05.11.2023.
//

#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define MATRIX_SIZE 100

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

    if (MATRIX_SIZE != world_size) {
        printf("Incorrect count of group_amount\n");
        MPI_Finalize();
        return 1;
    }

    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    int temp_vector[MATRIX_SIZE];

    if (world_rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrix[i][j] = i + j;
            }
        }
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        MPI_Scatter(&matrix[i][world_rank], 1, MPI_INT,
                    &temp_vector[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    int partial_norm = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        partial_norm += abs(temp_vector[i]);
    }

    int global_norm;
    MPI_Reduce(&partial_norm, &global_norm, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (world_rank == 0) {
        printMatrix(matrix);
        printf("Norm of the vector: %d\n", global_norm);
    }

    MPI_Finalize();
    return 0;
}
