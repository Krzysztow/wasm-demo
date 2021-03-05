cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

set(headers 
    "${PROJECT_SOURCE_DIR}/include/shared/compressor.h")
set(sources
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_executable(wasmDemo ${headers} ${sources})
# NOTE: make sure we put no spaces in the -sEXPORTED_FUNCTIONS=['_main'...]
target_link_options(wasmDemo PRIVATE -sEXPORTED_FUNCTIONS=['_main','_demo_compress','_demo_decompress','_demo_maxCompressSize']) 
target_link_options(wasmDemo PRIVATE -sEXPORTED_RUNTIME_METHODS=['malloc','free']) 

# Link dependencies
target_link_libraries(wasmDemo PUBLIC
    PRIVATE
      project_options
      wasmDemoLib
    PUBLIC
      )

target_include_directories(
  wasmDemo PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
