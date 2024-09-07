cmake_minimum_required(VERSION 3.25.1)
project(BINANCEINFOHANDLER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(spdlog)
find_package(rapidjson)
find_package(boost)

link_libraries(${OPENSSL_LIBRARIES})

# add_subdirectory(googletest)

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(benchmark)
add_subdirectory(unittest)








