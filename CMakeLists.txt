cmake_minimum_required(VERSION 3.22)

project(
    CppProject
    VERSION 1.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VENV_DIR
    ""
    CACHE PATH "Path to Python virtual environment")

set(BACKENDS_LIB_NAME "fink-backends")
set(INSTRUMENTS_LIB_NAME "fink-instruments")
set(MATH_LIB_NAME "fink-math")
set(MC_LIB_NAME "fink-mc")
set(MODELS_LIB_NAME "fink-models")
set(PRICERS_LIB_NAME "fink-pricers")
set(RNG_LIB_NAME "fink-rng")

option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable compiler warnings as errors" ON)

option(ENABLE_LTO "Enable to add Link Time Optimization." OFF)

option(ENABLE_SANITIZE_ADDR "Enable address sanitize." OFF)
option(ENABLE_SANITIZE_UNDEF "Enable undefined sanitize." OFF)
option(ENABLE_SANITIZE_LEAK "Enable leak sanitize (Gcc/Clang only)." OFF)
option(ENABLE_SANITIZE_THREAD "Enable thread sanitize (Gcc/Clang only)." OFF)

option(ENABLE_TESTING "Enable testing." ON)

option(ENABLE_CLANG_TIDY "Enable to add clang tidy." ON)
option(ENABLE_CLANG_FORMAT "Enable to add clang-format." ON)
option(ENABLE_CMAKE_FORMAT "Enable to add cmake-format." ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

include(Warnings)
include(LTO)
include(Sanitizer)

include(ClangTidy)
include(ClangFormat)
include(CMakeFormat)
add_clang_format_target()
add_cmake_format_target()

include(Docs)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

if(ENABLE_TESTING)
    find_package(GTest REQUIRED)
    include(CTest)
    enable_testing()
endif()

add_subdirectory(libs)
add_subdirectory(apps)
