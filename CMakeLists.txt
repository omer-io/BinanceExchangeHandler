cmake_minimum_required(VERSION 3.25.1)
project(main)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

include(spdlogConfig)
include(rapidjsonConfig)
include(boostConfig)

link_libraries(${OPENSSL_LIBRARIES})

include_directories(include)

add_subdirectory(googletest)
link_libraries(gtest)

add_subdirectory(benchmark)
add_subdirectory(unittest)

add_executable(main app/main.cpp src/getApiData.cpp)





