
include_guard()

if(WIN32)
    set(CUACC_UNIX OFF)
    add_compile_definitions(OS_WINDOWS)
    set(CUACC_CXX_STANDARD 23)
elseif(APPLE)
    set(CUACC_UNIX ON)
    add_compile_definitions(OS_OSX)
    set(CUACC_CXX_STANDARD 23)
elseif(UNIX)
    set(CUACC_UNIX ON)

    add_compile_definitions(OS_LINUX)

    set(CUACC_CXX_STANDARD 26)
    set(CUACC_CXX_HOST_COMPILER_STANDARD 26)
    set(CUACC_CUDA_STANDARD 20)
    # https://arnon.dk/matching-sm-architectures-arch-and-gencode-for-various-nvidia-cards/
    # Pascal (60), Volta (70), Turing (75), Ampere 30xx (80)
    set(CUACC_CUDA_ARCHITECTURES 80)
endif()
