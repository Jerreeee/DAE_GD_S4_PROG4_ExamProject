cmake_minimum_required(VERSION 3.26)
project(Prog4)

# Set the output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)
## Ensure vendor directory exists
#if (NOT EXISTS "${VENDOR_DIR}")
#    file(MAKE_DIRECTORY ${VENDOR_DIR})
#endif()

# Detect architecture (x86 or x64)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH "x64")
else()
    set(ARCH "x86")
endif()

include(FetchContent)

# Add targets
add_subdirectory(JREngine)
add_subdirectory(BubbleBobble)

# set default project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT BubbleBobble)