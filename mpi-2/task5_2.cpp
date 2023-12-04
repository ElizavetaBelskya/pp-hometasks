//
// Created by Елизавета on 04.12.2023.
//

#include <mpi.h>
#include <iostream>

const int rows = 8;
const int cols = 8;

//четные и нечетные считаем с нуля ))

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int a[rows][cols];
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                a[i][j] = i * cols + j + 1;

        std::cout << "Initial matrix:" << std::endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << a[i][j] << " ";
            }
            std::cout << std::endl;
        }

        MPI_Datatype odd_row_type;
        MPI_Type_vector(rows / 2, cols, cols * 2, MPI_INT, &odd_row_type);
        MPI_Type_commit(&odd_row_type);


        MPI_Datatype even_row_type;
        MPI_Type_vector(rows / 2, cols, cols * 2, MPI_INT, &even_row_type);
        MPI_Type_commit(&even_row_type);

        MPI_Send(&a[1][0], 1, odd_row_type, 1, 0, MPI_COMM_WORLD);

        MPI_Send(&a[0][0], 1, even_row_type, 1, 1, MPI_COMM_WORLD);

        MPI_Type_free(&odd_row_type);
        MPI_Type_free(&even_row_type);
    } else if (rank == 1) {
        int b[rows / 2][cols];
        MPI_Recv(&b[0][0], rows / 2 * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int c[rows / 2][cols];
        MPI_Recv(&c[0][0], rows / 2 * cols, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (rank == 1) {
            std::cout << "Matrix b (odd rows):" << std::endl;
            for (int i = 0; i < rows / 2; ++i) {
                for (int j = 0; j < cols; ++j) {
                    std::cout << b[i][j] << " ";
                }
                std::cout << std::endl;
            }

            std::cout << "Matrix c (even rows):" << std::endl;
            for (int i = 0; i < rows / 2; ++i) {
                for (int j = 0; j < cols; ++j) {
                    std::cout << c[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}
