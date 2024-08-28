cmake_minimum_required(VERSION 3.25.1)
project(benchmarks)

include(ExternalProject)

ExternalProject_Add(benchmark
    PREFIX ${CMAKE_BINARY_DIR}/benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG v1.8.5
    CMAKE_ARGS -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(benchmark binary_dir)

add_executable(benchmarks benchmark.cpp ${CMAKE_SOURCE_DIR}/src/getApiData.cpp)

target_link_libraries(benchmarks ${binary_dir}/src/libbenchmark.a pthread)