#ifndef CUDA_CXX_HELPERS_H
#define CUDA_CXX_HELPERS_H

#include <inttypes.h>
#include <math.h>
#include <type_traits>

template<typename T, std::enable_if_t<std::is_unsigned_v<T> && !std::is_floating_point_v<T>, bool> = true>
T fuseInts(T int1, T int2) {
    uint64_t times = (int2 == 0) ? 10 : pow(10, ceil(log10(int2)));
    return (int1 * times) + int2;
}

#endif // CUDA_CXX_HELPERS_H
