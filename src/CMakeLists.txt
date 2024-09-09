cmake_minimum_required(VERSION 3.25.1)

find_package(spdlog REQUIRED)
find_package(rapidjson REQUIRED)
find_package(boost REQUIRED)
find_package(OpenSSL REQUIRED)

project(BinanceExchange)

file(GLOB LIB_SOURCES BinanceExchange.cpp getHttpsData.cpp)
add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})

add_dependencies(${PROJECT_NAME} spdlog rapidjson boost)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${RAPIDJSON_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SPDLOG_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/boost/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${BOOST_LIB_DIR}/beast)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})