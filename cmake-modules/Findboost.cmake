include(ExternalProject)

ExternalProject_Add(boost
    PREFIX ${CMAKE_BINARY_DIR}/boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
