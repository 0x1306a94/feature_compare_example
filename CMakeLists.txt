cmake_minimum_required(VERSION 3.20)
project(feature_compare_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT
    "$<IF:$<CONFIG:Debug>,dwarf,dwarf-with-dsym>")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(check_simd)

if(APPLE)
  execute_process(
    COMMAND uname -m
    RESULT_VARIABLE result
    OUTPUT_VARIABLE OSX_NATIVE_ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(${OSX_NATIVE_ARCHITECTURE} STREQUAL "x86_64" AND SIMD_SUPPORT_AVX)
    set(CMAKE_CXX_FLAGS "-mavx ${CMAKE_CXX_FLAGS}")
  elseif(${OSX_NATIVE_ARCHITECTURE} STREQUAL "x86_64" AND SIMD_SUPPORT_SSE)
    set(CMAKE_CXX_FLAGS "-msse4.1 ${CMAKE_CXX_FLAGS}")
  endif()

elseif(UNIX)

  if(SIMD_SUPPORT_AVX)
    set(CMAKE_CXX_FLAGS "-mavx ${CMAKE_CXX_FLAGS}")
  elseif(SIMD_SUPPORT_SSE)
    set(CMAKE_CXX_FLAGS "-msse4.1 ${CMAKE_CXX_FLAGS}")
  endif()

endif()

configure_file(simd_feature.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/simd_feature.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJECT_NAME} main.cpp ${CMAKE_CURRENT_BINARY_DIR}/simd_feature.hpp)
