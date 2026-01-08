#ifndef CUDA_DEVICE_PTR_H
#define CUDA_DEVICE_PTR_H

#include <cuda_runtime.h>

#include "defs.hpp"

template<typename T>
class DevicePtr {

    __host__ __device__ __inline__ DevicePtr(T* ptr) : m_ptr(ptr) {
    }

  public:
    __host__ static DevicePtr fromRawDevPtr(T* p) {
        return {p};
    }

    // template<typename T1, typename = std::enable_if<std::is_convertible<T*, T1*>::value, bool>>
    template<typename T1>
    __H_D_I__ DevicePtr(const DevicePtr<T1>& other) : m_ptr((T1*)other) {
    }

    __D_I__ T* operator->() const {
        return m_ptr;
    }

    __D_I__ T& operator*() const {
        return *m_ptr;
    }

    __H_D_I__ operator T*() const {
        return m_ptr;
    }

    __D_I__ T& operator[](std::size_t s) {
        return m_ptr[s];
    }

    friend __D_I__ DevicePtr operator+(const DevicePtr& dev, std::size_t s) {
        return {dev.m_ptr + s};
    }

    __D_I__ DevicePtr& operator+=(std::size_t s) {
        m_ptr += s;
        return *this;
    }

    __D_I__ DevicePtr& operator++() {
        m_ptr++;
        return *this;
    }

    __D_I__ DevicePtr operator++(int) {
        return DevicePtr(m_ptr + 1);
    }

  private:
    T* m_ptr = nullptr;
};

#endif // CUDA_DEVICE_PTR_H
