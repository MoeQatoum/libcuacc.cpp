#include <cuda_runtime.h>

#include "CudaDevice.hpp"

#include <algorithm>
#include <iostream>

CudaDevice::CudaDevice(int device_index) : m_device(device_index), m_props(device_index) {
    int device_count;
    if ((device_count = CudaDevice::getDeviceCount()) == 0) { CU_CXX_ABORT("No CUDA devices found"); }

    if (device_index >= device_count) { CU_CXX_ABORT("requested device index \"%d\", but device count is \"%d\"", m_device, device_count); }
}

CUdevice CudaDevice::handle() const {
    CUdevice h;
    CUDA_CXX_CHECK_ERR(cuDeviceGet(&h, m_device), "Could not get device handle of device \"%d\"", m_device);
    return h;
}

CudaDevice CudaDevice::getDeviceByProperties(const CudaDeviceProperties& props) {
    int device;
    CUDA_CXX_CHECK_ERR(cudaChooseDevice(&device, &props.rawStruct()), "Failed to get CUDA device by properties");
    return CudaDevice{device};
}

int CudaDevice::getDeviceCount() {
    int numDevices = 0;
    CUDA_CXX_CHECK_ERR(cudaGetDeviceCount(&numDevices), "Failed to get number of CUDA devices");
    return numDevices;
}

CudaDevice CudaDevice::getDeviceByName(std::string dn) {
    int device_count;
    if ((device_count = CudaDevice::getDeviceCount()) == 0) { CU_CXX_ABORT("No CUDA devices found"); }
    std::transform(dn.begin(), dn.end(), dn.begin(), ::tolower);
    for (int i = 0; i < device_count; i++) {
        CudaDevice  d{i};
        std::string dn{d.m_props.name()};
        std::transform(dn.begin(), dn.end(), dn.begin(), ::tolower);
        if (std::string::npos != dn.find(dn)) { return d; }
    }
    CU_CXX_ABORT("Could not find CUDA device: \"%s\"", dn.c_str());
}

std::vector<CudaDevice> CudaDevice::EnumerateDevices() {
    std::vector<CudaDevice> res;
    int                     numDevices = getDeviceCount();
    for (int i = 0; i < numDevices; i++) {
        res.emplace_back(i);
    }
    return res;
}

CudaDevice CudaDevice::getCurrentDevice() {
    int device;
    CUDA_CXX_CHECK_ERR(cudaGetDevice(&device), "Can't get current device index");
    return CudaDevice{device};
}

void CudaDevice::setAsCurrentDevice() const {
    CUDA_CXX_CHECK_ERR(cudaSetDevice(m_device), "could't set device \"%d\" as current device.", m_device);
}

int CudaDevice::getDeviceAttribute(CUdevice_attribute attr) const {
    int ret;
    CUDA_CXX_CHECK_ERR(cuDeviceGetAttribute(&ret, attr, handle()), "could't get device attribute \"%d\"", attr);
    return ret;
}
