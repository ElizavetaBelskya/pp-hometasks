//
// Created by Елизавета on 25.11.2023.
//

#include <mpi.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int n = std::atoi(argv[1]);
    const int iterations = 100;

    if (size != 2) {
        std::cerr << "Only 2 processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    std::srand(std::time(0));
    char* message = new char[n];
    for (int i = 0; i < n; ++i) {
        message[i] = static_cast<char>(rand() % 256);
    }



    std::vector<double> ping_pong_times(iterations);

    for (int i = 0; i < iterations; ++i) {
        if (rank == 0) {
            auto start_time = std::chrono::high_resolution_clock::now();
            MPI_Send(message, n, MPI_CHAR, 1, i, MPI_COMM_WORLD);
            MPI_Recv(message, n, MPI_CHAR, 1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            auto end_time = std::chrono::high_resolution_clock::now();
            ping_pong_times[i] = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        } else if (rank == 1) {
            MPI_Recv(message, n, MPI_CHAR, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, n, MPI_CHAR, 0, i, MPI_COMM_WORLD);
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    double average_time = std::accumulate(ping_pong_times.begin(), ping_pong_times.end(), 0.0) / ping_pong_times.size();

    if (rank == 0) {
        std::cout << "Average PingPong Time: " << average_time << " microseconds" << std::endl;
    }

    std::vector<double> pingping_times(iterations);
    char* send_message = new char[n];
    for (int i = 0; i < n; ++i) {
        send_message[i] = static_cast<char>(rand() % 256);
    }
    char* received_message = new char[n];


    for (int i = 0; i < iterations; ++i) {
        auto start_time = std::chrono::high_resolution_clock::now();

        if (rank == 0) {
            MPI_Send(send_message, n, MPI_CHAR, 1, i, MPI_COMM_WORLD);
        } else if (rank == 1) {
            MPI_Send(received_message, n, MPI_CHAR, 0, i, MPI_COMM_WORLD);
        }

        if (rank == 0) {
            MPI_Recv(received_message, n, MPI_CHAR, 1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (rank == 1) {
            MPI_Recv(send_message, n, MPI_CHAR, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        pingping_times[i] = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    }


    MPI_Barrier(MPI_COMM_WORLD);
    double average_pingping_time = std::accumulate(pingping_times.begin(), pingping_times.end(), 0.0) / pingping_times.size();


    if (rank == 0) {
        std::cout << "Average PingPing Time: " << average_pingping_time << " microseconds" << std::endl;
    }

    delete[] message;

    delete[] send_message;
    delete[] received_message;

    MPI_Finalize();

    return 0;
}



