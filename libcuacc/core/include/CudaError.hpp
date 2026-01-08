#ifndef CUDA_ERROR_H
#define CUDA_ERROR_H

#include <cuda.h>
#include <cuda_runtime.h>

#include <concepts>
#include <string>

#include "defs.hpp"

template<typename T>
concept cuda_err_t = std::is_same_v<CUresult, T> || std::is_same_v<cudaError_t, T>;

template<typename err_t>
class CudaError {
  public:
    CudaError(err_t cuda_errno) : m_cuda_err_no(cuda_errno) {
    }
    CudaError(CudaError& other) : m_cuda_err_no(other.m_cuda_err_no) {
    }

    template<typename T = err_t, typename = std::enable_if_t<std::is_same_v<T, cudaError_t>, bool>>
    std::string errorName() {
        const char* err_name = cudaGetErrorName(m_cuda_err_no);
        return err_name ? err_name : "UNKNOWN_ERROR";
    }

    template<typename T = err_t, std::enable_if_t<std::is_same_v<T, CUresult>, bool> = true>
    std::string errorName() {
        const char* err_name = NULL;
        cuGetErrorName(m_cuda_err_no, &err_name);
        return err_name ? err_name : "UNKNOWN_ERROR";
    }

    bool isSuccess() {
        return static_cast<int>(m_cuda_err_no) == 0;
    }

    operator bool() {
        return static_cast<int>(m_cuda_err_no) != 0;
    }

    int errNo() {
        return m_cuda_err_no;
    }

  private:
    err_t m_cuda_err_no;
};

#define CUDA_CXX_CHECK_ERR(cuda_errno, msg, ...)                                                                          \
    do {                                                                                                                  \
        CudaError err{(cuda_errno)};                                                                                      \
        if (err) {                                                                                                        \
            fprintf(stderr,                                                                                               \
                    LOG_COLOR_R "[CUDA_ERROR] %s:%d code=%d(%s) \"%s\": " LOG_COLOR_W LOG_BOLD_FONT msg LOG_COLOR_W "\n", \
                    __FILE_NAME__,                                                                                        \
                    __LINE__,                                                                                             \
                    err.errNo(),                                                                                          \
                    err.errorName().c_str(),                                                                              \
                    #cuda_errno __VA_OPT__(, ) __VA_ARGS__);                                                              \
            abort();                                                                                                      \
        }                                                                                                                 \
    } while (0)

#endif // CUDA_ERROR_H
