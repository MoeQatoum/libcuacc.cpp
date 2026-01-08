
# set(CUACC_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "cuacc main dir")
# set(CUACC_SRC_DIR ${CUACC_DIR}/src CACHE PATH "cuacc src dir")
# set(CUACC_INCLUDE_DIR ${CUACC_DIR}/include CACHE PATH "cuacc include dir")
# set(CUACC_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE PATH "cuacc build directory")

# if(UNIX)
#     if("${CMAKE_BUILD_TYPE}" STREQUAL "")
#         set(CMAKE_BUILD_TYPE RELEASE)
#     else()
#         string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
#     endif()
# endif()

# file(GLOB_RECURSE CUACC_CXX_SRC
#     ${CUACC_SRC_DIR}/*.cpp
# )
# file(GLOB_RECURSE CUACC_CXX_HEADERS
#     ${CUACC_SRC_DIR}/*.hpp
#     ${CUACC_INCLUDE_DIR}/*.hpp
# )

# add_library(${LIB_CUACC} STATIC ${CUACC_CXX_SRC})
# add_library(${LIB_CUACC}::${LIB_CUACC} ALIAS ${LIB_CUACC})

# target_link_libraries(${LIB_CUACC} PRIVATE
#     # cuacc::cuacc-kernels
# )

# target_include_directories(${LIB_CUACC} PUBLIC
#     $<BUILD_INTERFACE:${CUACC_INCLUDE_DIR}>
#     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/>
# )

# target_compile_options(${LIB_CUACC} PRIVATE
#     $<$<CONFIG:RELEASE>: ${CUACC_REL_BUILD_FLAGS} ${CUACC_REL_WARN_FLAGS} $<$<BOOL:${CUACC_TCMALLOC}>:${CUACC_TCMALLOC_FLAGS}>>
#     $<$<CONFIG:DEBUG>: ${CUACC_DBG_BUILD_FLAGS} ${CUACC_DBG_WARN_FLAGS} $<$<BOOL:${CUACC_TCMALLOC}>:${CUACC_TCMALLOC_FLAGS}>>
#     $<$<CONFIG:PERF>: $<$<COMPILE_LANGUAGE:CXX>: -O3 -pg -Wall -Wextra> $<$<COMPILE_LANGUAGE:CUDA>: -lineinfo> $<$<BOOL:${CUACC_TCMALLOC}>:${CUACC_TCMALLOC_FLAGS}>>
# )

# target_link_options(${LIB_CUACC} PUBLIC
#     $<$<CONFIG:RELEASE>: -s $<$<BOOL:${CUACC_TCMALLOC}>:${CUACC_TCMALLOC_LINKER_FLAGS}>>
#     $<$<CONFIG:DEBUG>: $<$<BOOL:${CUACC_TCMALLOC}>:${CUACC_TCMALLOC_LINKER_FLAGS}>>
# )

# set_target_properties(${LIB_CUACC} PROPERTIES
#     INTERPROCEDURAL_OPTIMIZATION ON
#     CXX_STANDARD ${CUACC_CXX_STANDARD}
#     CXX_STANDARD_REQUIRED ON
#     CXX_EXTENSIONS OFF
#     OUTPUT_NAME ${LIB_CUACC}
#     # VERSION ${PROJECT_VERSION}
#     SOVERSION ${PROJECT_VERSION}
#     # SOVERSION ${PROJECT_VERSION_MAJOR}
# )

# if(CUACC_INSTALL)
#     install(
#         TARGETS ${LIB_CUACC}
#         EXPORT ${LIB_CUACC}_Targets
#         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         NAMELINK_SKIP
#         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # for windows
#         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#     )

#     install(TARGETS ${LIB_CUACC}
#         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         NAMELINK_ONLY
#         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # for windows
#         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     )

#     install(
#         DIRECTORY ${CUACC_INCLUDE_DIR}/
#         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
#         FILES_MATCHING
#         PATTERN "*.hpp"
#         REGEX "/[^.]+$"
#     )
# endif(CUACC_INSTALL)

find_package(CUDAToolkit REQUIRED)

add_subdirectory(core)
add_subdirectory(kernels)
add_subdirectory(memory)

set(LIB_CEX_LIBRARIES_BUILD_DIR
    ${CUACC_CORE_BUILD_DIR}
    ${CUACC_KERNELS_BUILD_DIR}
    ${CUACC_MEMORY_BUILD_DIR}
    CACHE PATH "list holds cuacc targets build directories."
)
