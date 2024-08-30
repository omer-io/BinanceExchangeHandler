include(ExternalProject)

ExternalProject_Add(boost
    PREFIX ${CMAKE_BINARY_DIR}/boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(boost source_dir)
include_directories(${source_dir}/libs/beast/include)
include_directories(${source_dir}/libs/config/include)
include_directories(${source_dir}/libs/core/include)
include_directories(${source_dir}/libs/static_assert/include)
include_directories(${source_dir}/libs/preprocessor/include)
include_directories(${source_dir}/libs/asio/include)
include_directories(${source_dir}/libs/throw_exception/include)
include_directories(${source_dir}/libs/assert/include)
include_directories(${source_dir}/libs/system/include)
include_directories(${source_dir}/libs/optional/include)
include_directories(${source_dir}/libs/type_traits/include)
include_directories(${source_dir}/libs/move/include)
include_directories(${source_dir}/libs/utility/include)
include_directories(${source_dir}/libs/date_time/include)
include_directories(${source_dir}/libs/smart_ptr/include)
include_directories(${source_dir}/libs/numeric/conversion/include)
include_directories(${source_dir}/libs/mpl/include)
include_directories(${source_dir}/libs/mp11/include)
include_directories(${source_dir}/libs/bind/include)
include_directories(${source_dir}/libs/intrusive/include)
include_directories(${source_dir}/libs/logic/include)
include_directories(${source_dir}/libs/static_string/include)
include_directories(${source_dir}/libs/container_hash/include)
include_directories(${source_dir}/libs/describe/include)
include_directories(${source_dir}/libs/io/include)
include_directories(${source_dir}/libs/endian/include)
include_directories(${source_dir}/libs/beast)
