//
// Created by Елизавета on 18.10.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define ARRAY_SIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "This program should be run with at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 1) {
        int data[] = {1, 2, 3, 4, 5};
        int data_size = sizeof(data) / sizeof(data[0]);

        for (int dest_rank = 0; dest_rank < size; dest_rank++) {
            if (dest_rank != 1) {
                MPI_Send(data, data_size, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
                printf("Process 1 sent data to process %d\n", dest_rank);
            }
        }
    } else {
        int received_data_size;
        MPI_Status status;

        MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &received_data_size);

        int* received_data = (int*)malloc(received_data_size * sizeof(int));

        MPI_Recv(received_data, received_data_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d received data: ", rank);
        for (int i = 0; i < received_data_size; i++) {
            printf("%d ", received_data[i]);
        }
        printf("\n");

        free(received_data);
    }

    MPI_Finalize();
    return 0;
}

