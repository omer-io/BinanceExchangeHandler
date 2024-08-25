cmake_minimum_required(VERSION 3.25.1)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

include(spdlogConfig)
include(rapidjsonConfig)
include(boostConfig)

link_libraries(${OPENSSL_LIBRARIES})

include_directories(include)

add_executable(main app/main.cpp src/getApiData.cpp)





