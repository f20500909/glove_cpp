project(SimpleTinyServer)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread -lpthread")

find_package(Threads)

add_executable(test testLockGuard.cpp)

