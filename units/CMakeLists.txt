cmake_minimum_required(VERSION 3.1.0)

project("glove")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)

add_executable(glove vocabulary.cpp vocabulary.h utils.cpp utils.h)

