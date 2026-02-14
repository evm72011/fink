#pragma once

#if defined(CUDA_ENABLED) && defined(__CUDACC__)
    #define CUDA_HD __host__ __device__ 
    #define CUDA_D __device__ 
    #define CUDA_H __host__ 
    #define CUDA_F __forceinline__ 
#else 
    #define CUDA_HD 
    #define CUDA_D 
    #define CUDA_H 
    #define CUDA_F inline 
#endif
