add_executable(test unittest.cpp ${CMAKE_SOURCE_DIR}/src/getApiData.cpp)


# include(ExternalProject)

# ExternalProject_Add(googletest
#     PREFIX ${CMAKE_BINARY_DIR}/googletest
#     GIT_REPOSITORY https://github.com/google/googletest
#     GIT_TAG main
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND ""
#     INSTALL_COMMAND ""
# )

# ExternalProject_Get_Property(googletest binary_dir)
# ExternalProject_Get_Property(googletest source_dir)

# link_libraries(gtest)
# add_executable(test unittest.cpp ${CMAKE_SOURCE_DIR}/src/getApiData.cpp)
# # message(${source_dir})
# include_directories(${source_dir}/googletest/include)
# #target_link_libraries(test ${binary_dir}/src/libgtest.a pthread)