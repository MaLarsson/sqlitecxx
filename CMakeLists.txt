# Main CMakeLists.txt
cmake_minimum_required(VERSION 3.2)
project(sqlitecxx)

# Set constants
set(CMAKE_CXX_STANDARD 14)

# Compiler warnings
if (MSVC)
  add_compile_options("/W4")
else()
  add_compile_options("-Wpedantic")
  add_compile_options("-Wall")
  add_compile_options("-Wextra")
endif()

# Add sqlite3 c library
include(cmake/DownloadProject.cmake)

download_project(
  PROJ sqlite3
  GIT_REPOSITORY https://github.com/MaLarsson/sqlite3
  GIT_TAG master
  UPDATE_DISCONNECTED 1)

add_subdirectory(${sqlite3_SOURCE_DIR} ${sqlite3_BINARY_DIR})

# Add sqlitecxx library
add_library(sqlitecxx INTERFACE)

target_include_directories(sqlitecxx INTERFACE include)
target_link_libraries(sqlitecxx INTERFACE sqlite3)

# Add tests
add_subdirectory(tests)
