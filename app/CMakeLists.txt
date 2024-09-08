cmake_minimum_required(VERSION 3.25.1)
project(main)

add_executable(${PROJECT_NAME} main.cpp)

add_dependencies(${PROJECT_NAME} spdlog rapidjson boost)

target_link_libraries(${PROJECT_NAME} BinanceExchange)
