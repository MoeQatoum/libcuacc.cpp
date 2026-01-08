#include <iostream>
#include <numeric>

#include "cuda_cxx"

#define N     1 << 28
#define num_t double
// #define N 5

template<typename T, std::size_t n>
using array_t = std::remove_pointer_t<T>[n];

template<typename T, std::size_t n>
void fill_ints(array_t<T, n> arr, T val) {
    std::fill_n(arr, n, val);
}

template<typename T, std::size_t n>
void fill_ints(array_t<T, n> arr) {
    std::iota(arr, arr + n, 0);
}

template<typename T, std::size_t n>
void log(array_t<T, n> arr) {
    for (std::size_t i = 0; i < n; ++i) {
        if (i == n - 1) {
            printf("%d\n", arr[i]);
        } else {
            printf("%d, ", arr[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

#if 0
    int* a;
    int* b;
    int* dist;

    cudaMallocHost((void**)&a, sizeof(int) * N);
    cudaMallocHost((void**)&b, sizeof(int) * N);
    cudaMallocHost((void**)&dist, sizeof(int) * N);

    // std::cout << a << " " << b << " " << dist << "\n";
    // std::cout << a << " " << b << " " << dist << "\n";

    fill_ints<num_t, N>(a.get(), 5);
    fill_ints<num_t, N>(b.get(), 1);
    fill_ints<num_t, N>(dist.get(), 0);

    // log(a, N);
    // log(b, N);
    // log(dist, N);

    add_vec(dist, a, b, N);

    log(a, N);
    log(b, N);
    log(dist, N);
#else
    int* cpu_res = new int[N];
    auto a       = ZeroCpyMem<num_t>::mallocElements(N);
    auto b       = ZeroCpyMem<num_t>::mallocElements(N);
    auto dist    = ZeroCpyMem<num_t>::mallocElements(N);

    fill_ints<num_t, N>(a.get(), 1);
    fill_ints<num_t, N>(b.get(), 3);
    fill_ints<num_t, N>(dist.get(), 0);

    for (std::size_t i = 0; i < N; ++i) {
        cpu_res[i] = a.get()[i] + b.get()[i];
    }

    // std::cout << a.get() << " " << b.get() << " " << dist.get() << "\n";
    // std::cout << a.get() << " " << b.get() << " " << dist.get() << "\n";

    // log<int, N>(a.get());
    // log<int, N>(b.get());
    // log<int, N>(cpu_res);
    // log<int, N>(dist.get());

    gpu_vec_add_v2f(dist, a, b, N);

    // log<int, N>(a.get());
    // log<int, N>(b.get());
    // log<int, N>(dist.get());

    for (int i = 0; i < N; ++i) {
        if (cpu_res[i] != dist.get()[i]) {
            printf("error at %d %d != %f\n", i, cpu_res[i], dist.get()[i]);
            abort();
        }
        // printf("OK %d == %f\n", cpu_res[i], dist.get()[i]);
    }

#endif
    return 0;
}
