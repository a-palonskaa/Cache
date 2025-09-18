cmake_minimum_required(VERSION 3.12)
project(cache_implementation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ./include)
set(SRC_DIR ./src)

add_executable(belady ${SRC_DIR}/belady.cpp)
add_executable(2q  ${SRC_DIR}/two_q.cpp)

target_include_directories(belady PRIVATE ${INCLUDE_DIR})
target_include_directories(2q PRIVATE ${INCLUDE_DIR})
