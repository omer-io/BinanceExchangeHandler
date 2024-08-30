cmake_minimum_required(VERSION 3.25.1)
project(benchmarks)

include(ExternalProject)

ExternalProject_Add(benchmark
    PREFIX ${CMAKE_BINARY_DIR}/benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG v1.8.5
    CMAKE_ARGS -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(benchmark binary_dir)

add_executable(benchmarks benchmark.cpp ${CMAKE_SOURCE_DIR}/src/getApiData.cpp)


add_dependencies(benchmarks benchmark boost rapidjson spdlog)

target_link_libraries(benchmarks ${CMAKE_SOURCE_DIR}/build2/benchmark/src/benchmark-build/src/libbenchmark.a pthread)

