#include <iostream>

#include "CudaDevice.hpp"

int main() {
    std::vector<CudaDevice> dev = CudaDevice::EnumerateDevices();
    for (const auto& d : dev) {
        std::cout << d.properties().name() << "\n";
    }
    CudaDevice cd(0);
    cd.handle();

    std::cout << "name: " << cd.properties().name() << "\n";
    std::cout << "compute version: " << cd.properties().major() << cd.properties().minor() << "\n";
    std::cout << "compute version: " << cd.properties().computeCapability() << "\n";
    std::cout << "Memory Bus Width: " << cd.getDeviceAttribute(CU_DEVICE_ATTRIBUTE_GLOBAL_MEMORY_BUS_WIDTH) << "\n";
    std::cout << "maxBlockDimX: " << cd.properties().maxBlockDimX() << "\n";
    std::cout << "maxBlockDimY: " << cd.properties().maxBlockDimY() << "\n";
    std::cout << "maxBlockDimZ: " << cd.properties().maxBlockDimZ() << "\n";
    std::cout << "maxGridDimX: " << cd.properties().maxGridDimX() << "\n";
    std::cout << "maxGridDimY: " << cd.properties().maxGridDimY() << "\n";
    std::cout << "maxGridDimZ: " << cd.properties().maxGridDimZ() << "\n";

    return 0;
}
