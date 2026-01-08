#include "GPUMem.hpp"
#include "vecadd.hpp"

template<typename T>
__global__ void vec_add_kernel_v2(DevicePtr<T> dist, DevicePtr<T> a, DevicePtr<T> b) {
    int i   = threadIdx.x + blockIdx.x * blockDim.x;
    dist[i] = a[i] + b[i];
    // if (dist[i] == 0) {
    //   printf("from gpu: i=%d, a=%f, b=%f, d=%f\n", i, a[i], b[i], dist[i]);
    // }
}

template<typename T>
__host__ void add_vec_v2(ZeroCpyMem<T>& dist, ZeroCpyMem<T>& a, ZeroCpyMem<T>& b, std::size_t size_n) {
    vec_add_kernel_v2<T><<<size_n / (1 << 10), (1 << 10)>>>(dist, a.get_ptr(), b.get_ptr());
}

__host__ void gpu_vec_add_v2(ZeroCpyMem<int>& dist, ZeroCpyMem<int>& a, ZeroCpyMem<int>& b, std::size_t size_n) {
    add_vec_v2<int>(dist, a, b, size_n);
    cudaDeviceSynchronize();
}

__host__ void gpu_vec_add_v2f(ZeroCpyMem<double>& dist, ZeroCpyMem<double>& a, ZeroCpyMem<double>& b, std::size_t size_n) {
    add_vec_v2<double>(dist, a, b, size_n);
    cudaDeviceSynchronize();
}
