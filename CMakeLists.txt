# Main CMakeLists.txt
cmake_minimum_required(VERSION 3.2)
project(sqlite_wapper)

# Set constants
set(CMAKE_CXX_STANDARD 11)

# Make executable
add_executable(tests tests.cc sqlite3/sqlite3.c)
target_link_libraries(tests dl pthread)

# Add tests
enable_testing()
add_test(Tests tests)

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
