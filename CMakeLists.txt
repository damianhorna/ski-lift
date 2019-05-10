cmake_minimum_required(VERSION 2.8.9)
project (ski-lift)

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

add_executable(ski-lift main.cpp QueueElement.cpp QueueElement.h ThreadManagerBase.cpp ThreadManagerBase.h easylogging++.h MessageType.h ThreadManager.cpp ThreadManager.h)
target_link_libraries(ski-lift ${MPI_C_LIBRARIES} -lpthread)