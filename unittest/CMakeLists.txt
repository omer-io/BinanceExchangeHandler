cmake_minimum_required(VERSION 3.25.1)
project(test)

add_executable(test unittest.cpp ${CMAKE_SOURCE_DIR}/src/getApiData.cpp)
add_dependencies(test gtest boost rapidjson spdlog)

