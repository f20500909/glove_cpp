cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../build)

aux_source_directory(. SERVER_DIR)

include_directories(./)
aux_source_directory(./ SERVER_DIR)


include_directories(data)
aux_source_directory(data SERVER_DIR)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread -lpthread  ")

find_package(Threads)
add_executable(server_pop ${SERVER_DIR})

