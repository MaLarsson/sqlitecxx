# Main CMakeLists.txt
cmake_minimum_required(VERSION 3.2)
project(sqlite_wapper)

# Set constants
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread -ldl")

# Make executable
add_executable(tests tests.cc sqlite3/sqlite3.c)

# Compiler warnings
if (MSVC)
  target_compile_options(tests PRIVATE "/W4")
else()
  target_compile_options(tests PRIVATE "-Wpedantic")
  target_compile_options(tests PRIVATE "-Wall")
  target_compile_options(tests PRIVATE "-Wextra")
endif()

# Includes
include_directories(include sqlite3)
