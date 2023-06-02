cmake_minimum_required(VERSION 3.20)
project(feature_compare_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT
    "$<IF:$<CONFIG:Debug>,dwarf,dwarf-with-dsym>")

include(CheckCXXSourceRuns)
set(CHECK_SSE_CODE
    "
#include <immintrin.h>
int main(int argc, const char *argv[]) {
    float __attribute__((aligned(16))) target[4] = {0.0f,0.1f,0.001f,0.004f};
    __m128 a = _mm_load_ps(target);
    return 0;
}
")

set(CHECK_AVX_CODE
    "
#include <immintrin.h>
int main(int argc, const char *argv[]) {
    float __attribute__((aligned(16))) target[8] = {0.0f,0.1f,0.001f,0.004f,0.0f,0.1f,0.001f,0.004f};
    __m256 a = _mm256_load_ps(target);
    return 0;
}
")

check_cxx_source_runs("${CHECK_SSE_CODE}" CAN_COMPILE_SSE)
check_cxx_source_runs("${CHECK_AVX_CODE}" CAN_COMPILE_AVX)

if(CAN_COMPILE_SSE)
  set(SIMD_SUPPORT_SSE 1)
else()
  set(SIMD_SUPPORT_SSE 0)
endif()

if(CAN_COMPILE_SSE)
  set(SIMD_SUPPORT_AVX 1)
else()
  set(SIMD_SUPPORT_AVX 0)
endif()

message(STATUS "SIMD_SUPPORT_SSE: ${SIMD_SUPPORT_SSE}")
message(STATUS "SIMD_SUPPORT_AVX: ${SIMD_SUPPORT_AVX}")

if(APPLE)
  execute_process(
    COMMAND uname -m
    RESULT_VARIABLE result
    OUTPUT_VARIABLE OSX_NATIVE_ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  set(CHECK_SIMD8_CODE
      "
    #include <TargetConditionals.h>
    #if TARGET_OS_OSX
        #if TARGET_CPU_ARM64
        #include <simd/simd.h>
        #endif
    #endif
    int main(int argc, const char *argv[]) {
        simd_float8 result = simd_make_float8(0);
        return 0;
    }
    ")

  set(CHECK_SIMD16_CODE
      "
    #include <TargetConditionals.h>
    #if TARGET_OS_OSX
        #if TARGET_CPU_ARM64
        #include <simd/simd.h>
        #endif
    #endif
    int main(int argc, const char *argv[]) {
        simd_float16 result = simd_make_float16(0);
        return 0;
    }
    ")

  check_cxx_source_runs("${CHECK_SIMD8_CODE}" CAN_COMPILE_SIMD8)
  check_cxx_source_runs("${CHECK_SIMD16_CODE}" CAN_COMPILE_SIMD16)
  if(CAN_COMPILE_SIMD8)
    set(SIMD_SUPPORT_APPLE_ARM64_SIMD8 1)
  else()
    set(SIMD_SUPPORT_APPLE_ARM64_SIMD8 0)
  endif()

  if(CAN_COMPILE_SIMD16)
    set(SIMD_SUPPORT_APPLE_ARM64_SIMD16 1)
  else()
    set(SIMD_SUPPORT_APPLE_ARM64_SIMD16 0)
  endif()

  message(
    STATUS "SIMD_SUPPORT_APPLE_ARM64_SIMD8: ${SIMD_SUPPORT_APPLE_ARM64_SIMD8}")
  message(
    STATUS "SIMD_SUPPORT_APPLE_ARM64_SIMD16: ${SIMD_SUPPORT_APPLE_ARM64_SIMD16}"
  )

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

configure_file(simd_feature.hpp.in simd_feature.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJECT_NAME} main.cpp)
