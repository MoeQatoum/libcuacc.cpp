#ifndef CUDA_KER_VEC_ADD_V2_H
#define CUDA_KER_VEC_ADD_V2_H

template<typename T>
class ZeroCpyMem;

void gpu_vec_add_v2(ZeroCpyMem<int>& dist, ZeroCpyMem<int>& a, ZeroCpyMem<int>& b, std::size_t size_n);
void gpu_vec_add_v2f(ZeroCpyMem<double>& dist, ZeroCpyMem<double>& a, ZeroCpyMem<double>& b, std::size_t size_n);

#endif // CUDA_KER_VEC_ADD_H
