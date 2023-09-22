cmake_minimum_required(VERSION 3.16)
project(OpenMPTest)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(task1 task1.cpp)

add_executable(task2 task2.cpp)

add_executable(task3 task3.cpp)

add_executable(task4 task4.cpp)

add_executable(task5 task5.cpp)

add_executable(task6 task6.cpp)

add_executable(task7 task7.cpp)

add_executable(task8 task8.cpp)

add_executable(task9 task9.cpp)

add_executable(task10 task10.cpp)

add_executable(task11 task11.cpp)

add_executable(task12 task12.cpp)

add_executable(task15 task15.cpp)