cmake_minimum_required(VERSION 3.30.1)
project(main)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# list(APPEND CMAKE_MODULE_PATH "cmake-modules/")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

include(cmake-modules/spdlogConfig.cmake)
include(cmake-modules/rapidjsonConfig.cmake)
include(cmake-modules/boostConfig.cmake)
# include(cmake-modules/opensslConfig.cmake)
# include(cmake-modules/beastConfig.cmake)
# file(GLOB cmakes "cmake-modules/*.cmake")
# foreach(file ${cmakes})
#     include(${file})
# endforeach()
link_libraries(${OPENSSL_LIBRARIES})

include_directories(include)

add_executable(main app/main.cpp)

# add_executable(https app/https.cpp)





