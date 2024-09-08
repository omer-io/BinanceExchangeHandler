cmake_minimum_required(VERSION 3.25.1)

find_package(spdlog)
find_package(rapidjson)
find_package(boost)

project(BinanceExchange)

set(LIB_SOURCES BinanceExchange.cpp getHttpsData.cpp)
add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})

add_dependencies(${PROJECT_NAME} spdlog rapidjson boost)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${RAPIDJSON_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SPDLOG_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/boost/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${BOOST_LIB_DIR}/beast)