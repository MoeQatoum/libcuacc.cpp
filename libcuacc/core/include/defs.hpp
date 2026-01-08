#ifndef CUDA_CXX_DEFS_H
#define CUDA_CXX_DEFS_H

#define LOG_COLOR_Y   "\033[93m"
#define LOG_COLOR_R   "\033[91m"
#define LOG_COLOR_W   "\033[0m"
#define LOG_BOLD_FONT "\033[1m"

#define __H_D_I__ __host__ __device__ __inline__
#define __H_I__   __host__ __inline__
#define __D_I__   __device__ __inline__
#define __H_D__   __host__ __device__
#define __H__     __host__
#define __D__     __device__
#define __G__     __global__
#define __I__     __inline__

#endif // CUDA_DEFINES_H
