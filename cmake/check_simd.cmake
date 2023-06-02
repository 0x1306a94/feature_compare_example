include(CheckCXXSourceRuns)

set(CHECK_SSE_CODE
    "
#include <immintrin.h>
int main(int argc, const char *argv[]) {
    float __attribute__((aligned(16))) target[4] = {
        0.0f,0.1f,0.001f,0.004f,
    };
    __m128 a = _mm_load_ps(target);
    return 0;
}
")

set(CHECK_AVX_CODE
    "
#include <immintrin.h>
int main(int argc, const char *argv[]) {
    float __attribute__((aligned(32))) target[8] = {
        0.0f,0.1f,0.001f,0.004f,
        0.0f,0.1f,0.001f,0.004f,
    };
    __m256 a = _mm256_load_ps(target);
    return 0;
}
")

check_cxx_source_runs("${CHECK_SSE_CODE}" CAN_COMPILE_SSE)
message(STATUS "CMAKE_REQUIRED_FLAGS: ${CMAKE_REQUIRED_FLAGS}")

set(CMAKE_REQUIRED_FLAGS_BACK ${CMAKE_REQUIRED_FLAGS})
set(CMAKE_REQUIRED_FLAGS "-mavx ${CMAKE_REQUIRED_FLAGS_BACK}")
check_cxx_source_runs("${CHECK_AVX_CODE}" CAN_COMPILE_AVX)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_BACK})

if(CAN_COMPILE_SSE)
  set(SIMD_SUPPORT_SSE 1)
else()
  set(SIMD_SUPPORT_SSE 0)
endif()

if(CAN_COMPILE_AVX)
  set(SIMD_SUPPORT_AVX 1)
else()
  set(SIMD_SUPPORT_AVX 0)
endif()

message(STATUS "SIMD_SUPPORT_SSE: ${SIMD_SUPPORT_SSE}")
message(STATUS "SIMD_SUPPORT_AVX: ${SIMD_SUPPORT_AVX}")

if(APPLE)
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

endif(APPLE)
