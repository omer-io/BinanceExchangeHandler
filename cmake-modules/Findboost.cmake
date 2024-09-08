cmake_minimum_required(VERSION 3.25.1)
project(boost)

include(ExternalProject)

ExternalProject_Add(${PROJECT_NAME}
    URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}/boost
    BUILD_COMMAND ./b2 --with-program_options --with-system --with-filesystem
    INSTALL_COMMAND ./b2 install
)

ExternalProject_Get_Property(${PROJECT_NAME} source_dir)
set(BOOST_LIB_DIR ${source_dir}/libs)