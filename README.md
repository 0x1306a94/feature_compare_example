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
SIMD(M1 float8) 1:10w 内积计算耗时: 50ms sim 0.0252284
SIMD(M1 float16) 1:10w 内积计算耗时: 27ms sim 0.0252284
```