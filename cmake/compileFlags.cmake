if(CUACC_UNIX)
    set(CUACC_TCMALLOC_FLAGS -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
    set(CUACC_TCMALLOC_LINKER_FLAGS $<$<CONFIG:DEBUG>: -ltcmalloc > $<$<CONFIG:RELEASE>: -ltcmalloc_minimal>)

    set(CUACC_STD_LIB_CXX_FLAGS -stdlib=libc++)
    set(CUACC_STD_LIB_CXX_LINKER_FLAGS -lc++ -nostdlib++) # or -stdlib=libc++)

    set(CUACC_REL_BUILD_FLAGS -O3 -DNDEBUG)
    set(CUACC_REL_WARN_FLAGS -Wall -Wextra -Wreorder-ctor -Wpedantic -Wdouble-promotion)

    set(CUACC_DBG_BUILD_FLAGS -O0 -ggdb3)
    set(CUACC_DBG_WARN_FLAGS -Wall -Wextra -Wreorder-ctor -Wconversion -Wpedantic -Wdouble-promotion -Wswitch-enum)
endif()
