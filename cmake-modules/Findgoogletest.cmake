cmake_minimum_required(VERSION 3.25.1)
project(googletest)

include(ExternalProject)

ExternalProject_Add(${PROJECT_NAME}
    PREFIX ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(${PROJECT_NAME} source_dir)
set(GOOGLETEST_INCLUDE_DIR ${source_dir}/googletest/include)

ExternalProject_Get_Property(${PROJECT_NAME} binary_dir)
set(GOOGLETEST_BINARY_DIR ${binary_dir})