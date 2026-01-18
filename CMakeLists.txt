cmake_minimum_required(VERSION 3.10)
project(cli_parser)

# Header-only library
add_library(cli_parser INTERFACE)
target_include_directories(cli_parser INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(cli_parser INTERFACE cxx_std_17)

# Tests
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests tests/test_main.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain cli_parser)

# Examples
add_executable(basic_example examples/basic.cpp)
target_link_libraries(basic_example PRIVATE cli_parser)

add_executable(comprehensive_example examples/comprehensive.cpp)
target_link_libraries(comprehensive_example PRIVATE cli_parser)