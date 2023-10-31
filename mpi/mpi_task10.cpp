//
// Created by Елизавета on 31.10.2023.
//
#include <iostream>
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

int main(int argc, char** argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    if (rank == 0) {
        int a[MATRIX_SIZE][MATRIX_SIZE];
        int t_a[MATRIX_SIZE][MATRIX_SIZE];

        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                a[i][j] = 2*i + 3*j;
            }
        }
        printMatrix(a);
        int n = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                MPI_Send(&a[i][j], 1, MPI_INT, n, 10, MPI_COMM_WORLD);
                MPI_Send(&a[j][i], 1, MPI_INT, n, 11, MPI_COMM_WORLD);
                n++;
            }
        }

        n = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j ++) {
                MPI_Recv(&t_a[i][j], 1, MPI_INT, n, 12, MPI_COMM_WORLD, &status);
                MPI_Recv(&t_a[j][i], 1, MPI_INT, n, 13, MPI_COMM_WORLD, &status);
                n++;
            }
        }
        printMatrix(t_a);

    } else {
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        int a;
        int b;

        MPI_Recv(&a, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, 1, MPI_INT, 0, 11, MPI_COMM_WORLD, &status);

        MPI_Send(&b, 1, MPI_INT, 0, 12, MPI_COMM_WORLD);
        MPI_Send(&a, 1, MPI_INT, 0, 13, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}