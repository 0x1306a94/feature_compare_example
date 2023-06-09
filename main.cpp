//
//  main.cpp
//  feature_compare
//
//  Created by king on 2023/5/31.
//

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <stdlib.h>

#include "simd_feature.hpp"

#if SIMD_SUPPORT_SSE || SIMD_SUPPORT_AVX
#include <immintrin.h>
#endif

#if SIMD_SUPPORT_APPLE_SIMD8 || SIMD_SUPPORT_APPLE_SIMD16
#include <simd/simd.h>
#endif

constexpr size_t FEATURE_LENGTH = 1024;
constexpr size_t FEATURE_COUNT = 100000;

float gen_rand_float(void) {
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.01;
    return r;
}

float compare(const float *target, const float *source) {
    float sum = 0;
    for (size_t idx = 0; idx < FEATURE_LENGTH; idx++) {
        sum += target[idx] * source[idx];
    }
    //    sum = std::max(sum, 0.0f);
    return sum;
}

#if SIMD_SUPPORT_SSE
float compare_sse(const float *target, const float *source) {
    float sum = 0;
    for (uint32_t i = 0; i < FEATURE_LENGTH; i += 4) {
        __m128 C = _mm_dp_ps(_mm_loadu_ps(target + i), _mm_loadu_ps(source + i), 0xf1);
        sum += C[0];
    }
    //    sum = std::max(sum, 0.0f);
    return sum;
}
#endif

#if SIMD_SUPPORT_AVX
float compare_avx(const float *target, const float *source) {
    float sum = 0;
    for (uint32_t i = 0; i < FEATURE_LENGTH; i += 8) {
        __m256 tmp = _mm256_dp_ps(_mm256_loadu_ps(target + i), _mm256_loadu_ps(source + i), 0xf1);
        sum += tmp[0] + tmp[4];
    }
    //    sum = std::max(sum, 0.0f);
    return sum;
}
#endif

#if SIMD_SUPPORT_APPLE_SIMD8
float compare_simd8(const float *target, const float *source) {
    simd_float8 result = simd_make_float8(0);
    for (uint32_t i = 0; i < FEATURE_LENGTH; i += 8) {
        simd_float8 *l = (simd_float8 *)(target + i);
        simd_float8 *r = (simd_float8 *)(source + i);
        result += *l * *r;
    }
    float sum = result[0] + result[1] + result[2] + result[3] + result[4] + result[5] + result[6] + result[7];
    //    sum = std::max(sum, 0.0f);
    return sum;
}
#endif

#if SIMD_SUPPORT_APPLE_SIMD16
float compare_simd16(const float *target, const float *source) {
    simd_float16 result = simd_make_float16(0);
    for (uint32_t i = 0; i < FEATURE_LENGTH; i += 16) {
        simd_float16 *l = (simd_float16 *)(target + i);
        simd_float16 *r = (simd_float16 *)(source + i);
        result += *l * *r;
    }
    float sum = result[0] + result[1] + result[2] + result[3] + result[4] + result[5] + result[6] + result[7] + result[8] + result[9] + result[10] + result[11] + result[12] + result[13] + result[14] + result[15];
    //    sum = std::max(sum, 0.0f);
    return sum;
}
#endif

void test(void) {
#if 0
    std::unique_ptr<float[]> all_features = std::make_unique<float[]>(FEATURE_COUNT * FEATURE_LENGTH);
    std::unique_ptr<float[]> target_feature = std::make_unique<float[]>(FEATURE_LENGTH);
#else
    //    auto buffer = new float[FEATURE_COUNT * FEATURE_LENGTH];
    //    float *buffer = (float *)malloc(sizeof(float) * FEATURE_COUNT * FEATURE_LENGTH);
    //    std::shared_ptr<float[]> all_features(buffer, std::default_delete<float[]>());

    std::shared_ptr<float[]> all_features(new float[FEATURE_COUNT * FEATURE_LENGTH], std::default_delete<float[]>());
    std::cout << "10w memory size: " << sizeof(float) * FEATURE_COUNT * FEATURE_LENGTH << std::endl;
    std::shared_ptr<float[]> target_feature(new float[FEATURE_LENGTH], std::default_delete<float[]>());
#endif
    //    getchar();
    //    return;
    auto len = FEATURE_COUNT * FEATURE_LENGTH;
    auto start = std::chrono::steady_clock::now();
    for (size_t idx = 0; idx < len; idx++) {
        all_features[idx] = gen_rand_float();
    }

    for (size_t idx = 0; idx < FEATURE_LENGTH; idx++) {
        target_feature[idx] = gen_rand_float();
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "初始化10w条向量耗时: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms\n";
    float *ptr = all_features.get();
    start = std::chrono::steady_clock::now();

    float sim = 0;
    for (size_t idx = 0; idx < FEATURE_COUNT; idx++) {
        float v = compare(target_feature.get(), ptr + (idx * FEATURE_LENGTH));
        //        std::max(sim, v);
        sim = v;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "常规 1:10w 内积计算耗时: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms"
              << " sim " << sim << std::endl;

#if SIMD_SUPPORT_APPLE_SIMD8
    start = std::chrono::steady_clock::now();
    sim = 0;
    for (size_t idx = 0; idx < FEATURE_COUNT; idx++) {
        float v = compare_simd8(target_feature.get(), ptr + (idx * FEATURE_LENGTH));
        //        std::max(sim, v);
        sim = v;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "SIMD(simd float8) 1:10w 内积计算耗时: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms"
              << " sim " << sim << std::endl;
#endif

#if SIMD_SUPPORT_APPLE_SIMD16
    start = std::chrono::steady_clock::now();
    sim = 0;
    for (size_t idx = 0; idx < FEATURE_COUNT; idx++) {
        float v = compare_simd16(target_feature.get(), ptr + (idx * FEATURE_LENGTH));
        //        std::max(sim, v);
        sim = v;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "SIMD(simd float16) 1:10w 内积计算耗时: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms"
              << " sim " << sim << std::endl;
#endif

#if SIMD_SUPPORT_SSE
    start = std::chrono::steady_clock::now();
    sim = 0;
    for (size_t idx = 0; idx < FEATURE_COUNT; idx++) {
        float v = compare_sse(target_feature.get(), ptr + (idx * FEATURE_LENGTH));
        //        std::max(sim, v);
        sim = v;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "SIMD(SSE4) 1:10w 内积计算耗时: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms"
              << " sim " << sim << std::endl;
#endif

#if SIMD_SUPPORT_AVX
    start = std::chrono::steady_clock::now();
    sim = 0;
    for (size_t idx = 0; idx < FEATURE_COUNT; idx++) {
        float v = compare_avx(target_feature.get(), ptr + (idx * FEATURE_LENGTH));
        //        std::max(sim, v);
        sim = v;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "SIMD(AVX) 1:10w 内积计算耗时: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms"
              << " sim " << sim << std::endl;
#endif
}

int main(int argc, const char *argv[]) {
    srand(static_cast<unsigned>(time(0)));
    test();
    getchar();
    return 0;
}
