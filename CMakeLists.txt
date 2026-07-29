cmake_minimum_required(VERSION 3.20)

project(ac-telemetry-engine
    VERSION 0.1.0
    DESCRIPTION "Telemetry analytics engine"
    LANGUAGES CXX
)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cpp/include")
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cpp/src")
set(PROJECT_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cpp/tests")

file(GLOB_RECURSE TELEMETRY_ALL_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/*.cc"
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.cxx"
)

set(TELEMETRY_MAIN_SOURCE "${PROJECT_SOURCE_DIR}/main.cpp")
set(TELEMETRY_LIB_SOURCES ${TELEMETRY_ALL_SOURCES})
list(REMOVE_ITEM TELEMETRY_LIB_SOURCES "${TELEMETRY_MAIN_SOURCE}")

add_library(telemetry_lib STATIC ${TELEMETRY_LIB_SOURCES})
add_library(ac-telemetry-engine::telemetry_lib ALIAS telemetry_lib)

target_include_directories(telemetry_lib
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(telemetry_lib PUBLIC cxx_std_17)

target_compile_options(telemetry_lib
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

add_executable(telemetry_app "${TELEMETRY_MAIN_SOURCE}")

target_link_libraries(telemetry_app PRIVATE telemetry_lib)
target_compile_features(telemetry_app PRIVATE cxx_std_17)

target_compile_options(telemetry_app
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE TELEMETRY_TEST_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_TEST_DIR}/*.cc"
    "${PROJECT_TEST_DIR}/*.cpp"
    "${PROJECT_TEST_DIR}/*.cxx"
)

if(TELEMETRY_TEST_SOURCES)
    add_executable(telemetry_tests ${TELEMETRY_TEST_SOURCES})

    target_link_libraries(telemetry_tests
        PRIVATE
            telemetry_lib
            GTest::gtest_main
    )

    target_compile_features(telemetry_tests PRIVATE cxx_std_17)

    target_compile_options(telemetry_tests
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/W4>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
    )

    include(GoogleTest)
    gtest_discover_tests(telemetry_tests)
endif()

install(TARGETS telemetry_lib telemetry_app
    EXPORT ac-telemetry-engine-targets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(DIRECTORY "${PROJECT_INCLUDE_DIR}/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.hh"
        PATTERN "*.hxx"
)

install(EXPORT ac-telemetry-engine-targets
    FILE ac-telemetry-engine-targets.cmake
    NAMESPACE ac-telemetry-engine::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ac-telemetry-engine"
)
