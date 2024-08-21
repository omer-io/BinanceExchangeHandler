include(ExternalProject)

ExternalProject_Add(openssl
    PREFIX ${CMAKE_BINARY_DIR}/openssl
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(openssl source_dir)
include_directories(${source_dir}/include)