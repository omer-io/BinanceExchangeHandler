include(ExternalProject)

ExternalProject_Add(boost
    PREFIX ${CMAKE_BINARY_DIR}/boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.86.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(boost source_dir)
include_directories(${source_dir}/include)