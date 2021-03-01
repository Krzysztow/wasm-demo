cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

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

set(headers 
    "${PROJECT_SOURCE_DIR}/include/shared/compressor.h")
set(sources
    "${CMAKE_CURRENT_SOURCE_DIR}/compressor.cpp")

add_library(wasmDemoLib ${headers} ${sources})

target_link_libraries(wasmDemoLib PUBLIC
    PRIVATE
      project_options
      fmt::fmt
      miniz)

target_include_directories(
  wasmDemoLib PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
