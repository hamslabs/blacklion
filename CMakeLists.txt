# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_C_STANDARD 99)

set(GSG_BASE_DIR ${CMAKE_SOURCE_DIR})

# use the repo version of ninja on Windows as there is no Ninja installer
#if(WIN32)
#    set(CMAKE_MAKE_PROGRAM ${GSG_BASE_DIR}/cmake/ninja CACHE STRING "Ninja location")
#endif()

# Set the toolchain if not defined
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${GSG_BASE_DIR}/cmake/arm-gcc-cortex-m0.cmake")
endif()

include(${GSG_BASE_DIR}/cmake/utilities.cmake)

# Define the Project
# project(atsame54_azure_iot C ASM)
project(app C ASM)

# add_subdirectory(${SHARED_SRC_DIR} shared_src)
#add_subdirectory(app)
add_subdirectory(shared/lib/threadx)
add_subdirectory(shared/lib/filex)
add_subdirectory(shared/lib/netxduo)
add_subdirectory(shared/lib/tiva)
add_subdirectory(example)
