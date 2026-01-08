#!/bin/bash

#colors
readonly W="\033[0;00m"
readonly G="\033[0;92m"
readonly R="\033[0;91m"
readonly Y="\033[0;93m"

readonly num_re='^[0-9]+$'

readonly ROOT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")
readonly BUILD_DIR=$ROOT_DIR/.build
INSTALL_PREFIX=$ROOT_DIR/.out
RUN=true

CONFIG=RELEASE
ARCH=
TARGETS=install
JOBS=-j
CLEAN_BUILD=false
CLEAN_INSTALL=false
SHARED=false
#TODO: should be enabled from cli
BUILD_EXAMPLES=true

CXX_COMPILER=clang++
C_COMPILER=clang
CXX_HOST_COMPILER=g++
C_HOST_COMPILER=clang-14
CUDA_COMPILER=nvcc

#for dev requirements
CUACC_INSTALL=true
# CUDA_ROOT_DIR="/usr/local/cuda-12.1"

print_help() {
    printf "help:\n"
    printf "  --clean               Remove existing build folder './.build'  and installed files set by '--prefix or ./.out'.\n"
    printf "  --clean-build         Remove installed files.\n"
    printf "  --clean-install       Remove installed files.\n"
    printf "  --examples            Build examples.\n"
    printf "  --no-run              Build but don't run the app.\n"
    printf "  --shared              Build libcuacc as a shared lib.\n"
    printf "  --install             Install libcuacc.\n"
    printf "  --prefix              Set install prefix.\n"
    printf "  --config              Set build type. Options RELEASE, DEBUG ...\n"
    printf "  --cxx-compiler        Set c++ compiler. Default clang++.\n"
    printf "  --c-compiler          Set c compiler. Default clang.\n"
    printf "  --cuda-compiler       Set cuda compiler. Default system nvcc.\n"
    printf "  --arch                Pass arch to cmake.\n"
    printf "  --target              Pass target to cmake.\n"
    printf "  --cuda-path           Specify cuda toolkit path.\n"
    printf "  --package             Package this project.\n"
    printf "  -v | --verbose        cmake verbose output.\n"
    printf "  -j | --jobs           Set N jobs at once\n"
    printf "  -h | --help           this help.\n"
}

opts=("$@")
for ((i = 0; i < $#; i++)); do
    case "${opts[$i]}" in
    --clean)
        CLEAN_BUILD=true
        CLEAN_INSTALL=true
        ;;
    --clean-build)
        CLEAN_BUILD=true
        ;;
    --clean-install)
        CLEAN_INSTALL=true
        ;;
    --examples)
        BUILD_EXAMPLES=true
        ;;
    --no-run)
        RUN=false
        ;;
    --shared)
        SHARED=true
        ;;
    --install)
        CUACC_INSTALL=true
        ;;
    --prefix)
        INSTALL_PREFIX=${opts[$((i + 1))]}
        ((i++))
        ;;
    --config)
        CONFIG=${opts[$((i + 1))]}
        ((i++))
        ;;
    --cxx-compiler)
        CXX_COMPILER=${opts[$((i + 1))]}
        ((i++))
        ;;
    --c-compiler)
        C_COMPILER=${opts[$((i + 1))]}
        ((i++))
        ;;
    --cuda-compiler)
        CUDA_COMPILER=${opts[$((i + 1))]}
        ((i++))
        ;;
    --arch)
        ARCH=${opts[$((i + 1))]}
        ((i++))
        ;;
    --target)
        TARGETS="$TARGETS ${opts[$((i + 1))]}"
        ((i++))
        ;;
    --cuda-path)
        CUDA_ROOT_DIR=${opts[$((i + 1))]}
        ((i++))
        ;;
    --package)
        TARGETS="$TARGETS package"
        ;;
    -v | --verbose)
        CMAKE_VERBOSE="-v"
        ;;
    -j | --jobs)
        if [[ ${opts[$((i + 1))]} =~ $num_re ]]; then
            CMAKE_JOBS="-j ${opts[$((i + 1))]}"
            ((i++))
        else
            printf "-j | --jobs flag accepts only numbers."
            print_help
            exit 1
        fi
        ;;
    -h | --help)
        print_help
        exit 0
        ;;
    *)
        printf "\"${opts[$((i))]}\" is invalid option, use -h or --help for help\n" >&2
        print_help
        exit 1
        ;;
    esac
done

if [ $CLEAN_BUILD == true ]; then
    printf -- "-- Removing previous build.\n"
    if [[ -d $BUILD_DIR ]]; then
        rm -rf $BUILD_DIR
    else
        printf "\t${Y}Build Destination: ${BOLD}${Y}${BUILD_DIR}${W}${Y} not found\n${W}" >&2
    fi
fi

if [ $CLEAN_INSTALL == true ]; then
    printf -- "-- Removing previous installation.\n"
    if [[ -d $INSTALL_PREFIX ]]; then
        rm -rf $INSTALL_PREFIX
    else
        printf "\t${Y}Install Destination: ${BOLD}${Y}${INSTALL_PREFIX}${W}${Y} not found\n${W}" >&2
    fi
fi


cmake -S $ROOT_DIR -B $BUILD_DIR \
    -D CMAKE_CXX_COMPILER=$CXX_COMPILER \
    -D CMAKE_C_COMPILER=$C_COMPILER \
    -D CMAKE_CUDA_HOST_COMPILER=$CXX_HOST_COMPILER \
    -D CMAKE_CUDA_COMPILER=$CUDA_COMPILER \
    -D CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=true \
    -D CMAKE_BUILD_TYPE:STRING=$CONFIG \
    -D CMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
    -D CUACC_BUILD_EXAMPLES:BOOL=$BUILD_EXAMPLES \
    -D CUACC_FORCE_ASSERTS:BOOL=true \
    -D CUACC_SHARED_LIB:BOOL=$SHARED \
    -D CUACC_INSTALL:BOOL=$CUACC_INSTALL \
    -D CUACC_TCMALLOC:BOOL=TRUE \

if [[ $? -eq 1 ]]; then
    printf -- "-- ${R}CMake Configuration failed${W}\n" >&2
    exit 1
fi

cmake --build $BUILD_DIR --target $TARGETS $JOBS $CMAKE_VERBOSE
if [[ $? -eq 0 ]]; then
    printf "${G}Build successful.${W}\n\n"

    if [[ $RUN = true ]]; then
        printf "${G}-- Running Application.${W}\n\n"
        if [[ $CONFIG -eq "PROFILE" ]]; then
            cd ./.perf
            nsys profile --stats=true ../out/cuda-playground-vecadd
        else
            $INSTALL_PREFIX/bin/cuda-playground-vecadd

        fi
    fi
else
    printf "${R}Build failed.${W}\n"
fi
