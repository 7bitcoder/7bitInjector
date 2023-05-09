include(Functions)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/publish)

SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH}:\$ORIGIN/../bin:\$ORIGIN)

set(LIBRARY_TYPE "HeaderOnly" CACHE STRING
    "Library build type: Shared;Static;HeaderOnly")

set(LIBRARY_TYPE_VALUES "Shared;Static;HeaderOnly" CACHE STRING
    "List of possible BUILD_LIBRARY_Type values")

set_property(CACHE LIBRARY_TYPE PROPERTY STRINGS ${LIBRARY_TYPE_VALUES})

set(BUILD_DOCS false CACHE BOOL "Turn on to build documentation")
set(BUILD_TESTS false CACHE BOOL "Turn on to build tests")
set(BUILD_EXAMPLES false CACHE BOOL "Turn on to build examples")

set(SEVEN_BIT_INJECTOR_VERSION ${CMAKE_PROJECT_VERSION})

if(LIBRARY_TYPE STREQUAL "Shared")
    set(BUILD_LIBRARY_TYPE "Shared")
    set(SEVEN_BIT_INJECTOR_SHARED_LIB true)
elseif(LIBRARY_TYPE STREQUAL "Static")
    set(BUILD_LIBRARY_TYPE "Static")
    set(SEVEN_BIT_INJECTOR_STATIC_LIB true)
else() # headerOnly
    set(BUILD_LIBRARY_TYPE "HeaderOnly")
    set(SEVEN_BIT_INJECTOR_HEADER_ONLY_LIB true)
endif()

configure_file(Include/SevenBit/DI/CmakeDef.hpp.input ${PROJECT_SOURCE_DIR}/Include/SevenBit/DI/CmakeDef.hpp)

set(INFOS
    "7BitInjector version: ${SEVEN_BIT_INJECTOR_VERSION}"
    "7BitInjector build as ${BUILD_LIBRARY_TYPE} library"
    "=================================================="
    "Build tests: ${BUILD_TESTS}"
    "Build examples: ${BUILD_EXAMPLES}"
    "Build documentation: ${BUILD_DOCS}"
)
printInfo("${INFOS}" = 50 7 0)