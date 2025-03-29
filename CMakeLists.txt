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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(FetchContent)
include(Utils)

# Add targets
add_subdirectory(JREngine)
add_subdirectory(BubbleBobble)

#temporarily define settings here until subcheck is fixed for multiple cmakelists files
# Enable C++20
target_compile_features(BubbleBobble PRIVATE cxx_std_20)

# Compile warnings as errors
target_compile_options(BubbleBobble PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# set default project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT BubbleBobble)