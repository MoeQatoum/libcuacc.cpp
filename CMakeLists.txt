cmake_minimum_required(VERSION 3.27)

include(cmake/checks.cmake)

set(LIB_CUACC cuacc CACHE INTERNAL "Project Name")

project(
    ${LIB_CUACC}
    VERSION 0.1.0
    DESCRIPTION "acceleration base library on top of cuda in C++."
    LANGUAGES CXX CUDA
)

include(GNUInstallDirs)
include(cmake/compileFlags.cmake)
include(cmake/project-is-top-level.cmake)

if(UNIX)
    if(${CMAKE_BUILD_TYPE} STREQUAL "")
        set(CMAKE_BUILD_TYPE RELEASE)
    else()
        string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
    endif()

    if(CUACC_SHARED_LIB)
        set(_type SHARED CACHE INTERNAL "Build type")
    else()
        set(_type STATIC CACHE INTERNAL "Build type")
    endif()
endif()

add_subdirectory(libcuacc)

if(CUACC_INSTALL)
    include(CMakePackageConfigHelpers)

    set(config_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_CUACC}-${PROJECT_VERSION})

    write_basic_package_version_file(${LIB_CUACC}ConfigVersion.cmake COMPATIBILITY ExactVersion)

    configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/${LIB_CUACC}Config.cmake.in
        ${PROJECT_BINARY_DIR}/${LIB_CUACC}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_CUACC}-${PROJECT_VERSION}
        INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
    )

    write_basic_package_version_file(
        ${LIB_CUACC}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(
        EXPORT ${LIB_CUACC}_Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_CUACC}-${PROJECT_VERSION}
        NAMESPACE ${LIB_CUACC}::
    )

    install(FILES
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_CUACC}-${PROJECT_VERSION}
    )

    if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
        set(CPACK_GENERATOR ZIP)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()

    set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
    set(CPACK_PACKAGE_CHECKSUM SHA256)
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
    include(CPack)
endif()

if(PROJECT_IS_TOP_LEVEL)
    if(CUACC_BUILD_EXAMPLES)
        add_subdirectory(examples)
    endif()
endif(PROJECT_IS_TOP_LEVEL)
