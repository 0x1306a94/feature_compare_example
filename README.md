# feature_compare_example

#### Linux
```bash
CPU: Intel(R) Xeon(R) CPU E5-2620 v3 @ 2.40GHz
Memory: 16G
OS: Ubuntu 18.04.6 LTS

10w memory size: 409600000
初始化10w条向量耗时: 1691ms
常规 1:10w 内积计算耗时: 293ms sim 0.0262037
SIMD(SSE4) 1:10w 内积计算耗时: 102ms sim 0.0262037
SIMD(AVX) 1:10w 内积计算耗时: 63ms sim 0.0262037
```

#### macOS
```bash
CPU: Apple M1
Memory: 16G
OS: Ventura 13.2.1

10w memory size: 409600000
初始化10w条向量耗时: 824ms
常规 1:10w 内积计算耗时: 413ms sim 0.0252284
SIMD(simd float8) 1:10w 内积计算耗时: 50ms sim 0.0252284
SIMD(simd float16) 1:10w 内积计算耗时: 27ms sim 0.0252284
```

#### macOS
```bash
CPU: 2.2 GHz 四核Intel Core i7
Memory: 16G
OS: Monterey 12.6.6

10w memory size: 409600000
初始化10w条向量耗时: 1345ms
常规 1:10w 内积计算耗时: 299ms sim 0.0270767
SIMD(simd float8) 1:10w 内积计算耗时: 59ms sim 0.0270767
SIMD(simd float16) 1:10w 内积计算耗时: 44ms sim 0.0270767
SIMD(SSE4) 1:10w 内积计算耗时: 106ms sim 0.0270767
SIMD(AVX) 1:10w 内积计算耗时: 67ms sim 0.0270767
```