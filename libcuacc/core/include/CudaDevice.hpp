#ifndef CUDA_DEVICE_H
#define CUDA_DEVICE_H

#include <cuda.h>
#include <driver_types.h>

#include <string>
#include <vector>

#include "CudaError.hpp"
#include "assert.hpp"
#include "helpers.hpp"

class CudaDeviceProperties {

    explicit CudaDeviceProperties(const cudaDeviceProp& props) : m_props(props) {
    }

  public:
    CudaDeviceProperties(int device) {
        cudaGetDeviceProperties(&m_props, device);
    }

    static CudaDeviceProperties FromExistingProperties(const cudaDeviceProp& props) {
        return CudaDeviceProperties{props};
    }

    const cudaDeviceProp& rawStruct() const {
        return m_props;
    }

    int computeCapability() const {
        return fuseInts(static_cast<unsigned int>(major()), static_cast<unsigned int>(minor()));
    }

    std::string name() const {
        return m_props.name;
    }
    int major() const {
        return m_props.major;
    }
    int minor() const {
        return m_props.minor;
    }
    int maxThreadsPerBlock() {
        return m_props.maxThreadsPerBlock;
    }
    int maxBlockDimX() const {
        return m_props.maxThreadsDim[0];
    }
    int maxBlockDimY() const {
        return m_props.maxThreadsDim[1];
    }
    int maxBlockDimZ() const {
        return m_props.maxThreadsDim[2];
    }
    int maxGridDimX() const {
        return m_props.maxGridSize[0];
    }
    int maxGridDimY() const {
        return m_props.maxGridSize[1];
    }
    int maxGridDimZ() const {
        return m_props.maxGridSize[2];
    }

  private:
    cudaDeviceProp m_props;
};

class CudaDevice {
  public:
    explicit CudaDevice(int device);

    CUdevice handle() const;
    void     setAsCurrentDevice() const;

    int getDeviceAttribute(CUdevice_attribute attr) const;

    const CudaDeviceProperties& properties() const {
        return m_props;
    }

    static std::vector<CudaDevice> EnumerateDevices();

    static CudaDevice getDeviceByProperties(const CudaDeviceProperties& props);
    static CudaDevice getDeviceByName(std::string name);
    static CudaDevice getCurrentDevice();
    static int        getDeviceCount();

  private:
    int                  m_device;
    CudaDeviceProperties m_props;
};

#endif // CUDA_DEVICE_H
