cmake_minimum_required(VERSION 3.25.1)
project(BINANCEINFOHANDLER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Enable compilation of unittest" ON)
option(BUILD_BENCHMARKS "Enable compilation of benchmark" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")

add_subdirectory(src)
add_subdirectory(app)

if(BUILD_TESTS)
    add_subdirectory(unittest)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()








