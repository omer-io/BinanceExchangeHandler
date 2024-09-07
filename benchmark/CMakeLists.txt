cmake_minimum_required(VERSION 3.25.1)
project(benchmarks)

add_executable(benchmarks benchmark.cpp)

add_dependencies(benchmarks benchmark boost rapidjson spdlog)

find_package(benchmark)

target_link_libraries(benchmarks ${BENCHMARK_BINARY_DIR}/src/libbenchmark.a pthread BinanceExchange)
