cmake_minimum_required(VERSION 3.25.1)
project(main)

add_executable(main main.cpp)

add_dependencies(main spdlog rapidjson boost)

target_link_libraries(main BinanceExchange)
