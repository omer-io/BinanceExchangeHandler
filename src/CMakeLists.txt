cmake_minimum_required(VERSION 3.25.1)
project(BinanceExchange)

set(LIB_SOURCES BinanceExchange.cpp getHttpsData.cpp)
add_library(BinanceExchange STATIC ${LIB_SOURCES})

add_dependencies(BinanceExchange spdlog rapidjson boost)

target_include_directories(BinanceExchange PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(BinanceExchange PUBLIC ${RAPIDJSON_INCLUDE_DIR})
target_include_directories(BinanceExchange PUBLIC ${SPDLOG_INCLUDE_DIR})

target_include_directories(BinanceExchange PUBLIC ${CMAKE_BINARY_DIR}/boost/include)
target_include_directories(BinanceExchange PUBLIC ${BOOST_LIB_DIR}/beast)