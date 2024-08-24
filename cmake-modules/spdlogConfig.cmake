include(ExternalProject)

ExternalProject_Add(spdlog
    PREFIX ${CMAKE_BINARY_DIR}/spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.x
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(spdlog source_dir)
include_directories(${source_dir}/include)