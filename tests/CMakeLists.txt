include(CTest)
enable_testing()

include(../cmake/DownloadProject.cmake)

download_project(
  PROJ googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG master
  UPDATE_DISCONNECTED 1)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

# Make executable
add_executable(sqlitecxx_tests tests.cpp)
target_link_libraries(sqlitecxx_tests PUBLIC sqlitecxx gtest)

# Add tests
add_test(all_tests sqlitecxx_tests)
add_custom_target(tests
  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
  DEPENDS sqlitecxx_tests)
