cmake_minimum_required(VERSION 3.1.0)

project("glove")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../build)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/python/glove)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread -lpthread -Werror ")


include_directories(./)
aux_source_directory(./ SRC_LIST)

include_directories(./base)
aux_source_directory(./base SRC_LIST)


link_libraries(pthread)
add_executable(${PROJECT_NAME} ${SRC_LIST})

find_package(pybind11 REQUIRED)
#pybind11_add_module(glove_pybind python/glove/pybind/glove_pybind.cpp)