cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(
  WasmDemo
  VERSION 1.0
  LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME fmt
  GIT_TAG 7.1.3
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

option(FOR_NATIVE "Build code for native" OFF)
option(FOR_WASM "Build code for wasm" OFF)

if((NOT FOR_NATIVE AND NOT FOR_WASM) OR (FOR_NATIVE AND FOR_WASM))
  message(FATAL_ERROR "Need to have one and only option set: [FOR_NATIVE, FOR_WASM]")
endif()

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

add_subdirectory("source/shared")

if(FOR_NATIVE)
  add_subdirectory("source/native")
endif()

if (FOR_WASM)
  add_subdirectory("source/wasm")
endif()

