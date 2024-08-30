cmake_minimum_required(VERSION 3.25.1)
project(main)

add_executable(main main.cpp ${CMAKE_SOURCE_DIR}/src/BinanceExchange.cpp ${CMAKE_SOURCE_DIR}/src/https.cpp)
add_dependencies(main spdlog rapidjson boost)
