cmake_minimum_required(VERSION 3.25.1)
project(BINANCEINFOHANDLER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_UNITTEST "Enable compilation of unittest" ON)
option(ENABLE_BENCHMARK "Enable compilation of benchmark" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")

add_subdirectory(src)
add_subdirectory(app)

if(ENABLE_UNITTEST)
    add_subdirectory(unittest)
endif()

if(ENABLE_BENCHMARK)
    add_subdirectory(benchmark)
endif()








