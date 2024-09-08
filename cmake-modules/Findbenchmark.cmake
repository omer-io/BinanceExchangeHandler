cmake_minimum_required(VERSION 3.25.1)
project(benchmark)

include(ExternalProject)

ExternalProject_Add(${PROJECT_NAME}
    PREFIX ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.0.tar.gz
    CMAKE_ARGS -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(${PROJECT_NAME} binary_dir)
set(BENCHMARK_BINARY_DIR ${binary_dir})
