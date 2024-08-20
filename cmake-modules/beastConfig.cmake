include(ExternalProject)

ExternalProject_Add(beast
    PREFIX ${CMAKE_BINARY_DIR}/beast
    GIT_REPOSITORY https://github.com/boostorg/beast.git
    GIT_TAG v124
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(beast source_dir)
include_directories(${source_dir}/include)