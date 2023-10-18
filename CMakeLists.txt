cmake_minimum_required(VERSION 3.16)
project(OpenMPTest)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_executable(task1 mpi/mpi_task1.cpp)
add_executable(task2 mpi/mpi_task2.cpp)
add_executable(task3 mpi/mpi_task3.cpp)
add_executable(task4 mpi/mpi_task4.cpp)
add_executable(task5 mpi/mpi_task5.cpp)

# Связываем программу с MPI библиотекой
target_link_libraries(task1 ${MPI_C_LIBRARIES})
target_link_libraries(task2 ${MPI_C_LIBRARIES})
target_link_libraries(task3 ${MPI_C_LIBRARIES})
target_link_libraries(task4 ${MPI_C_LIBRARIES})
target_link_libraries(task5 ${MPI_C_LIBRARIES})