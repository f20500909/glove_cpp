cmake_minimum_required(VERSION 3.1.0)

project("glove")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread -lpthread -Werror ")


include_directories(./)
aux_source_directory(./ SRC_LIST)


link_libraries(pthread)
add_executable(${PROJECT_NAME} ${SRC_LIST})
