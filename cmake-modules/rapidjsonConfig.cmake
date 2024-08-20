include(ExternalProject)

ExternalProject_Add(rapidjson
    PREFIX ${CMAKE_BINARY_DIR}/rapidjson
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG v1.1.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(rapidjson source_dir)
include_directories(${source_dir}/include)