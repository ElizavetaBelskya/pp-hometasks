//
// Created by Елизавета on 18.10.2023.
//

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

    MPI_Status status;

    int elements_per_process = 5;

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
        int n = 1;
        int k = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE ; j++) {
                MPI_Send(&matrixA[i][0], MATRIX_SIZE, MPI_INT, n, n, MPI_COMM_WORLD);
                MPI_Send(&matrixB_trans[j][0], MATRIX_SIZE, MPI_INT, n, n, MPI_COMM_WORLD);
                if (k % 5 == 0) {
                    n++;
                }
                k++;
            }
        }

        n = 1;
        k = 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                MPI_Recv(&result[i][j], 1, MPI_INT, n, n, MPI_COMM_WORLD, &status);
                if (k % 5 == 0) {
                    n++;
                }
                k++;
            }
        }
        printMatrix(result);
    } else if (world_rank <= ((MATRIX_SIZE * MATRIX_SIZE) / 5)) {
        int vectorA[MATRIX_SIZE];
        int vectorB[MATRIX_SIZE];
        int c;
        for (int k = 0; k < elements_per_process; k++) {
            c = 0;
            MPI_Recv(&vectorA, MATRIX_SIZE, MPI_INT, 0,
                     world_rank, MPI_COMM_WORLD, &status);
            MPI_Recv(&vectorB, MATRIX_SIZE, MPI_INT, 0,
                     world_rank, MPI_COMM_WORLD, &status);
            for (int z = 0; z < elements_per_process; z++) {
                c += vectorA[z] * vectorB[z];
            }
            MPI_Send(&c, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}