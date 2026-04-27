cmake_minimum_required(VERSION 3.14)
project(utility VERSION 1.0.0 LANGUAGES CXX)

# Require C++11 as requested
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the header-only library
add_library(utilitycxx INTERFACE)
add_library(utilitycxx::utilitycxx ALIAS utilitycxx)

# Include directories
target_include_directories(utilitycxx INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optional components
option(UTILITY_BUILD_TESTS "Build the utilitycxx tests" ON)
option(UTILITY_BUILD_EXAMPLES "Build the utilitycxx examples" ON)

if(UTILITY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(UTILITY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
