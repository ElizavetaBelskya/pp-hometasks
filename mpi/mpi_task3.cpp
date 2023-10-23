//
// Created by Елизавета on 18.10.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 2) {
        printf("This program should be run with 2 processes. Exiting...\n");
        MPI_Finalize();
        return 1;
    }

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0) {
        srand(time(NULL));
        int a[10];
        for (int i = 0; i < 10; i++) {
            a[i] = rand() % 100;
        }

        MPI_Send(a, 10, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) {
        int received_a[10];
        MPI_Recv(received_a, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Received array on process 1: ");
        for (int i = 0; i < 10; i++) {
            printf("%d ", received_a[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
