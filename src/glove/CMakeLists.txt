cmake_minimum_required(VERSION 3.1.0)

project("glove")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../build)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../python/glove)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread -lpthread -Werror ")

include_directories(./)
aux_source_directory(./ SRC_LIST)



#导入pybind包
find_package(pybind11)
#链接pybind文件
pybind11_add_module(glove_pybind ./py_bind/glove_pybind.cpp ${SRC_LIST})


add_executable(${PROJECT_NAME} ${SRC_LIST})


