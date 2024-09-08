cmake_minimum_required(VERSION 3.25.1)

find_package(benchmark)
project(benchmarks)

add_executable(${PROJECT_NAME} benchmark.cpp)

add_dependencies(${PROJECT_NAME} benchmark boost rapidjson spdlog)

target_link_libraries(${PROJECT_NAME} ${BENCHMARK_BINARY_DIR}/src/libbenchmark.a pthread BinanceExchange)
