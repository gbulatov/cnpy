cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(cnpy LANGUAGES CXX)

option(CNPY_BUILD_SHARED "Build shared library" OFF)
option(CNPY_BUILD_EXAMPLES "Build example programs" OFF)
option(CNPY_BUILD_TESTS "Build unit tests" ON)

find_package(ZLIB REQUIRED)
include(CTest)

# Static library (always built)
add_library(cnpy STATIC cnpy.cpp)
target_include_directories(cnpy
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(cnpy PUBLIC ZLIB::ZLIB)
set_target_properties(cnpy PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

# Optional shared library
if(CNPY_BUILD_SHARED)
    add_library(cnpy-shared SHARED cnpy.cpp)
    target_include_directories(cnpy-shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(cnpy-shared PUBLIC ZLIB::ZLIB)
    set_target_properties(cnpy-shared PROPERTIES
        OUTPUT_NAME cnpy
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
    )
    install(TARGETS cnpy-shared
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

# Optional example
if(CNPY_BUILD_EXAMPLES)
    add_executable(example1 example1.cpp)
    target_link_libraries(example1 cnpy)
endif()

if(CNPY_BUILD_TESTS)
    add_executable(cnpy-tests tests/test_npz_writer.cpp)
    target_link_libraries(cnpy-tests PRIVATE cnpy)
    add_test(NAME cnpy-tests COMMAND cnpy-tests)
endif()

# Install
install(TARGETS cnpy
    EXPORT cnpyTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(FILES cnpy.h DESTINATION include)

# CMake package config for find_package(cnpy)
install(EXPORT cnpyTargets
    FILE cnpyTargets.cmake
    NAMESPACE cnpy::
    DESTINATION lib/cmake/cnpy
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cnpyConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cnpyConfig.cmake"
    INSTALL_DESTINATION lib/cmake/cnpy
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cnpyConfig.cmake"
    DESTINATION lib/cmake/cnpy
)
