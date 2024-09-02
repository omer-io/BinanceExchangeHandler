cmake_minimum_required(VERSION 3.25.1)
project(test)

add_executable(test unittest.cpp)

add_dependencies(test gtest boost rapidjson spdlog)

target_link_libraries(test gtest BinanceExchange)