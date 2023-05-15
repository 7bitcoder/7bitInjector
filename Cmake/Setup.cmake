include(Functions)

set(SEVEN_BIT_DI_PROJECT_NAME 7bitDI)

set(SEVEN_BIT_DI_VERSION_MAJOR 1)
set(SEVEN_BIT_DI_VERSION_MINOR 0)
set(SEVEN_BIT_DI_VERSION_PATCH 0)

set(SEVEN_BIT_DI_VERSION ${SEVEN_BIT_DI_VERSION_MAJOR}.${SEVEN_BIT_DI_VERSION_MINOR}.${SEVEN_BIT_DI_VERSION_PATCH})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPACK_PROJECT_NAME ${SEVEN_BIT_DI_PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${SEVEN_BIT_DI_VERSION})

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/publish)

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH}:\$ORIGIN/../bin:\$ORIGIN)

set(SEVEN_BIT_DI_HEADERS_DIR "${CMAKE_SOURCE_DIR}/Include")

file(GLOB SEVEN_BIT_DI_TOP_HEADERS "${SEVEN_BIT_DI_HEADERS_DIR}/SevenBit/DI/*.hpp")
file(GLOB SEVEN_BIT_DI_DETAILS_HEADERS "${SEVEN_BIT_DI_HEADERS_DIR}/SevenBit/DI/Details/*.hpp")
file(GLOB SEVEN_BIT_DI_IMPL_HEADERS "${SEVEN_BIT_DI_HEADERS_DIR}/SevenBit/DI/Impl/*.hpp")
set(SEVEN_BIT_DI_ALL_HEADERS ${SEVEN_BIT_DI_TOP_HEADERS} ${SEVEN_BIT_DI_DETAILS_HEADERS} ${SEVEN_BIT_DI_IMPL_HEADERS})

source_group("Header Files\\SevenBit" FILES ${SEVEN_BIT_DI_TOP_HEADERS})
source_group("Header Files\\SevenBit\\Details" FILES ${SEVEN_BIT_DI_DETAILS_HEADERS})
source_group("Header Files\\SevenBit\\Details\\Impl" FILES ${SEVEN_BIT_DI_IMPL_HEADERS})

# build shared option
option(SEVEN_BIT_DI_BUILD_SHARED "Build shared library" OFF)

# build position independent code
option(SEVEN_BIT_DI_BUILD_PIC "Build position independent code (-fPIC)" OFF)

# example options
option(SEVEN_BIT_DI_BUILD_EXAMPLES "Build example" OFF)
option(SEVEN_BIT_DI_BUILD_EXAMPLES_HO "Build header only example" OFF)

# testing options
option(SEVEN_BIT_DI_BUILD_TESTS "Build tests" OFF)
option(SEVEN_BIT_DI_BUILD_TESTS_HO "Build tests using the header only version" OFF)

# build documentation option (requires sphinx and doxygen installed)
option(SEVEN_BIT_DI_BUILD_DOCS "Turn on to build documentation" OFF)

if(SEVEN_BIT_DI_BUILD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(project_config_in ${CMAKE_SOURCE_DIR}/Cmake/7bitDIConfig.cmake.in)
set(project_config_out ${CMAKE_BINARY_DIR}/7bitDIConfig.cmake)
set(config_targets_file 7bitDIConfigTargets.cmake)
set(version_config_file ${CMAKE_BINARY_DIR}/7bitDIConfigVersion.cmake)
set(export_dest_dir ${CMAKE_INSTALL_LIBDIR}/cmake/7bitDI)
configure_file(${CMAKE_SOURCE_DIR}/Include/SevenBit/DI/Version.hpp.input ${CMAKE_SOURCE_DIR}/Include/SevenBit/DI/Version.hpp)

set(INFOS
    "${CMAKE_PROJECT_NAME} version: ${SEVEN_BIT_DI_VERSION}"
    "${CMAKE_PROJECT_NAME} build as shared library ${SEVEN_BIT_DI_BUILD_SHARED} "
    "=================================================="
    "Build tests: ${SEVEN_BIT_DI_BUILD_TESTS}"
    "Build tests header only: ${SEVEN_BIT_DI_BUILD_TESTS_HO}"
    "Build examples: ${SEVEN_BIT_DI_BUILD_EXAMPLES}"
    "Build examples header only: ${SEVEN_BIT_DI_BUILD_EXAMPLES_HO}"
    "Build documentation: ${SEVEN_BIT_DI_BUILD_DOCS}"
)
printInfo("${INFOS}" = 50 7 0)