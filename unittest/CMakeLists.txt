cmake_minimum_required(VERSION 3.25.1)
find_package(googletest REQUIRED)
project(test)

add_executable(${PROJECT_NAME} unittest.cpp)

add_dependencies(${PROJECT_NAME} googletest boost rapidjson spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC ${GOOGLETEST_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${GOOGLETEST_BINARY_DIR}/lib/libgtest.a pthread BinanceExchange)