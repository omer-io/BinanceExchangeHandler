cmake_minimum_required(VERSION 3.25.1)
project(rapidjson)

include(ExternalProject)

ExternalProject_Add(${PROJECT_NAME}
    PREFIX ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    URL https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.tar.gz
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(${PROJECT_NAME} source_dir)
set(RAPIDJSON_INCLUDE_DIR ${source_dir}/include)
