#ifndef CUDA_DEVICE_PTR_H
#define CUDA_DEVICE_PTR_H

#include <type_traits>

template<typename T>
class DevicePtr {

    DevicePtr(T* ptr) : m_ptr(ptr) {
    }

  public:
    static DevicePtr fromRawDevPtr(T* p) {
        return {p};
    }

    template<typename T1, typename = std::enable_if<std::is_convertible<T*, T1*>::value, bool>>
    DevicePtr(const DevicePtr<T1>& other) : m_ptr((T1*)other) {
    }

    T* operator->() const {
        return m_ptr;
    }

    T& operator*() const {
        return *m_ptr;
    }

    operator T*() const {
        return m_ptr;
    }

    T& operator[](std::size_t s) {
        return m_ptr[s];
    }

    friend DevicePtr operator+(const DevicePtr& dev, std::size_t s) {
        return {dev.m_ptr + s};
    }

    DevicePtr& operator+=(std::size_t s) {
        m_ptr += s;
        return *this;
    }

    DevicePtr& operator++() {
        m_ptr++;
        return *this;
    }

    DevicePtr operator++(int) {
        return DevicePtr(m_ptr + 1);
    }

  private:
    T* m_ptr = nullptr;
};

#endif // CUDA_DEVICE_PTR_H
