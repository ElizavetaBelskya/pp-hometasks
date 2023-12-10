//
// Created by Елизавета on 04.12.2023.
//
#include <mpi.h>
#include <iostream>

const int rows = 8;
const int cols = 8;

struct MatrixBlock {
    int data[2][cols];
};

void printArray(int rank, MatrixBlock local_block) {
    std::cout << "Process " << rank << " received data:" << std::endl;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << local_block.data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void createStructType(MPI_Datatype* type) {
    MPI_Aint displacements[2];
    int block_lengths[2];
    MPI_Datatype types[2];

    for (int i = 0; i < 2; ++i) {
        displacements[i] = i * cols * sizeof(int);
        block_lengths[i] = cols;
        types[i] = MPI_INT;
    }

    MPI_Type_create_struct(2, block_lengths, displacements, types, type);
    MPI_Type_commit(type);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        std::cerr << "This program is designed to run with 4 processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Datatype block_type;
    createStructType(&block_type);

    MatrixBlock local_block;
    auto send_buffer = new MatrixBlock[size];  // Allocate array based on the size

    if (rank == 0) {
        int a[rows][cols];
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                a[i][j] = i + j;
                std::cout << a[i][j] << " ";
            }
            std::cout << "\n";
        }

        for (int n = 0, i = 0, k = 4; i < rows / 2; i += 1, k += 1, n += 1) {
            for (int j = 0; j < cols; ++j) {
                send_buffer[n].data[0][j] = a[i][j];
                send_buffer[n].data[1][j] = a[k][j];
            }
        }
    }

    MPI_Scatter(send_buffer, 2 * cols, MPI_INT, &local_block, 2 * cols, MPI_INT, 0, MPI_COMM_WORLD);

    printArray(rank, local_block);

    delete[] send_buffer;
    MPI_Finalize();

    return 0;
}






