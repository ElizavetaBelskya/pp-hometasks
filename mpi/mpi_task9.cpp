//
// Created by Елизавета on 18.10.2023.
//
#define N 10
#include <stdio.h>
#include "mpi.h"

void print_results(int a[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf(" %d", a[i][j]);
        }
        printf ("\n");
    }
    printf ("\n\n");
}

int main(int argc, char *argv[]) {
    int rank, size, sum = 0;
    int a[N][N];
    int b[N][N];
    int c[N][N];
    int aa[N],cc[N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = i + j;
            b[i][j] = j - i;
        }
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Scatter(a, N*N/size, MPI_INT, aa, N*N/size, MPI_INT,0,MPI_COMM_WORLD);

    MPI_Bcast(b, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sum = sum + aa[j] * b[j][i];
        }
        cc[i] = sum;
        sum = 0;
    }

    MPI_Gather(cc, N*N/size, MPI_INT, c, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    if (rank == 0) {
        print_results(c);
    }
}
