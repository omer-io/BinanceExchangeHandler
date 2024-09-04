include(ExternalProject)

ExternalProject_Add(boost
    PREFIX ${CMAKE_BINARY_DIR}/boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

# ExternalProject_Add(boost
#     URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
#     BUILD_IN_SOURCE 1
#     CONFIGURE_COMMAND ./bootstrap.sh --prefix=${CMAKE_INSTALL_PREFIX}
#     BUILD_COMMAND ./b2 --with-program_options --with-system --with-filesystem
#     INSTALL_COMMAND ./b2 install
# )
