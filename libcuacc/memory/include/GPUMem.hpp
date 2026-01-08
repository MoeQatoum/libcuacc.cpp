#ifndef CUDA_DEVICE_MEMORY_H
#define CUDA_DEVICE_MEMORY_H

#include <cuda_runtime.h>

#include "CudaError.hpp"
#include "DevicePtr.hpp"
#include "defs.hpp"

template<typename T>
class GPUMem {
  protected:
    T*          m_ptr;
    std::size_t m_bytes = 0;

    GPUMem() {
    }

    ~GPUMem() {
    }

  public:
    GPUMem(GPUMem&& other) : m_ptr(other.m_ptr), m_bytes(other.m_bytes) {
        other.m_ptr   = nullptr;
        other.m_bytes = 0;
    }

    GPUMem& operator=(GPUMem&& other) {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_bytes, other.m_bytes);
        return *this;
    }

    GPUMem(const GPUMem&)            = delete;
    GPUMem& operator=(const GPUMem&) = delete;

    template<typename T1, typename = std::enable_if<std::is_convertible<T*, T1*>::value, bool>>
    operator DevicePtr<T1>() const {
        return DevicePtr<T1>::fromRawDevPtr(m_ptr);
    }

    // operator DevPtr<T>() const {
    //   return DevPtr<T>::fromRawDevPtr(m_ptr);
    // }

    DevicePtr<T> get_ptr() {
        return DevicePtr<T>::fromRawDevPtr(m_ptr);
    }

    operator bool() const {
        return m_ptr != nullptr;
    }

    std::size_t size() {
        return m_bytes;
    }

    std::size_t size_n() {
        return m_bytes / sizeof(T);
    }

    T* get() {
        return this->m_ptr;
    }
};

template<typename T>
class DevMem : public GPUMem<T> {
    DevMem(std::size_t bytes) {
        this->m_bytes = bytes;
        // CUDA_CXX_CHECK_ERR(cudaMalloc(&this->m_ptr, bytes), "could not execute device malloc.");
        cudaMalloc(&this->m_ptr, bytes);
    }

  public:
    ~DevMem() {
        if (this->m_ptr) { cudaFree(this->m_ptr); }
    }

    __H_I__ DevMem(DevMem&& other) : GPUMem<T>(std::move(other)) {
    }

    __H_I__ DevMem& operator=(DevMem&& other) {
        if (this != &other) { GPUMem<T>::operator=(std::move(other)); }
        return *this;
    }

    __H_I__ static DevMem mallocElemets(std::size_t n) {
        return {n * sizeof(T)};
    }

    __H_I__ static DevMem mallocBytes(std::size_t n_bytes) {
        return {n_bytes};
    }
};

template<typename T>
class ZeroCpyMem : public GPUMem<T> {
    ZeroCpyMem(std::size_t bytes) /* : GPUMem<T, true>() */ {
        this->m_bytes = bytes;
        // CUDA_CXX_CHECK_ERR(cudaMallocHost(&this->m_ptr, bytes), "could not execute pinned memory malloc.");
        cudaMallocHost(&this->m_ptr, bytes);
    }

  public:
    __H_I__ ~ZeroCpyMem() {
        if (this->m_ptr) { cudaFreeHost(this->m_ptr); }
    }

    __H_I__ ZeroCpyMem(ZeroCpyMem&& other) : GPUMem<T>(std::move(other)) {
    }

    __H_I__ ZeroCpyMem& operator=(ZeroCpyMem&& other) {
        if (this != &other) { GPUMem<T>::operator=(std::move(other)); }
        return *this;
    }

    __H_I__ static ZeroCpyMem mallocElements(std::size_t n) {
        return {n * sizeof(T)};
    }

    __H_I__ static ZeroCpyMem mallocBytes(std::size_t n_bytes) {
        return {n_bytes};
    }
};

#endif // CUDA_DEVICE_MEMORY_H
