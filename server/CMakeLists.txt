cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SERVER_DIR)

include_directories(src)
aux_source_directory(src SERVER_DIR)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread -lpthread -Werror ")

find_package(Threads)

add_executable(server ${SERVER_DIR})

