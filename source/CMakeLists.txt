cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.4.1")

CPMAddPackage(
  NAME fmt
  GIT_TAG 7.1.3
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

CPMAddPackage(
  NAME miniz
  GIT_TAG 2.1.0
  GITHUB_REPOSITORY richgel999/miniz
  # no cmake options available as of 2.1.0
)

CPMAddPackage(
  NAME miniz
  GIT_TAG 2.1.0
  GITHUB_REPOSITORY richgel999/miniz
  # no cmake options available as of 2.1.0
)

CPMAddPackage(
  NAME argparse
  GIT_TAG v2.0
  GITHUB_REPOSITORY p-ranav/argparse
  OPTIONS "ARGPARSE_BUILD_TESTS OFF"
)

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG release-1.10.0
  VERSION 1.10.0
  OPTIONS
      "INSTALL_GTEST OFF"
      "BUILD_GMOCK OFF"
      "gtest_force_shared_crt ON"
)

set(headers 
    "${PROJECT_SOURCE_DIR}/include/compressor.h")
set(sources
    "${CMAKE_CURRENT_SOURCE_DIR}/compressor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
set(test_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/compressor.cpp")

add_executable(wasmDemoNative 
    ${headers}
    ${sources})

add_executable(wasmDemoNativeTests 
    ${headers} 
    ${test_sources}
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/compression_tests.cpp")

# Link dependencies
target_link_libraries(wasmDemoNative PUBLIC
    PRIVATE
      project_options
      fmt::fmt
      miniz
      argparse::argparse)

target_link_libraries(wasmDemoNativeTests PUBLIC
    PRIVATE
      project_options
      fmt::fmt
      miniz
      argparse::argparse
      gtest_main)

target_include_directories(
  wasmDemoNative PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

target_include_directories(
  wasmDemoNativeTests PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

enable_testing()
add_test(DemoTests wasmDemoNativeTests)
