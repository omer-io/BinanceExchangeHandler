cmake_minimum_required(VERSION 3.25.1)
project(test)

add_executable(test unittest.cpp)

add_dependencies(test googletest boost rapidjson spdlog)

find_package(googletest)

target_link_libraries(test ${GOOGLETEST_BINARY_DIR}/lib/libgtest.a pthread BinanceExchange)