include(ExternalProject)

ExternalProject_Add(spdlog
    PREFIX ${CMAKE_BINARY_DIR}/spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.14.1.tar.gz
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(spdlog source_dir)
include_directories(${source_dir}/include)