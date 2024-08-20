cmake_minimum_required(VERSION 3.30.1)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake-modules/spdlogConfig.cmake)
include(cmake-modules/rapidjsonConfig.cmake)
# include(cmake-modules/beastConfig.cmake)
include(cmake-modules/boostConfig.cmake)

# file(GLOB cmakes "cmake-modules/*.cmake")
# foreach(file ${cmakes})
#     include(${file})
# endforeach()


add_executable(main app/main.cpp)


