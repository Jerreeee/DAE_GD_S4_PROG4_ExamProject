# --- SDL2 ---
FetchContent_Declare(
    SDL2
    URL https://www.libsdl.org/release/SDL2-devel-2.30.10-VC.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2)
if(NOT SDL2_POPULATED)
    FetchContent_Populate(SDL2)
endif()

set(SDL2_INCLUDE_DIR "${sdl2_SOURCE_DIR}/include")
set(SDL2_LIBRARY      "${sdl2_SOURCE_DIR}/lib/${ARCH}/SDL2.lib") # DONT ADD: ${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib
set(SDL2_DLL          "${sdl2_SOURCE_DIR}/lib/${ARCH}/SDL2.dll")

message(STATUS "SDL2 DLL: ${SDL2_DLL}")

if(NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES      "${SDL2_LIBRARY}"
    )
endif()

if(NOT TARGET SDL2::SDL2main)
    add_library(SDL2::SDL2main INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2main PROPERTIES
        INTERFACE_LINK_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib"
    )
endif()

# --- SDL2_image ---
FetchContent_Declare(
    SDL2_image
    URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.3-VC.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_image)
if(NOT SDL2_image_POPULATED)
    FetchContent_Populate(SDL2_image)
endif()

set(SDL2_IMAGE_INCLUDE_DIR "${sdl2_image_SOURCE_DIR}/include")
set(SDL2_IMAGE_LIBRARY     "${sdl2_image_SOURCE_DIR}/lib/${ARCH}/SDL2_image.lib")
set(SDL2_IMAGE_DLL         "${sdl2_image_SOURCE_DIR}/lib/${ARCH}/SDL2_image.dll")

if(NOT TARGET SDL2::Image)
    add_library(SDL2::Image INTERFACE IMPORTED)
    set_target_properties(SDL2::Image PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES      "${SDL2_IMAGE_LIBRARY}"
    )
endif()

# --- SDL2_ttf ---
FetchContent_Declare(
    SDL2_ttf
    URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-VC.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_ttf)
if(NOT SDL2_ttf_POPULATED)
    FetchContent_Populate(SDL2_ttf)
endif()

set(SDL2_TTF_INCLUDE_DIR "${sdl2_ttf_SOURCE_DIR}/include")
set(SDL2_TTF_LIBRARY     "${sdl2_ttf_SOURCE_DIR}/lib/${ARCH}/SDL2_ttf.lib")
set(SDL2_TTF_DLL         "${sdl2_ttf_SOURCE_DIR}/lib/${ARCH}/SDL2_ttf.dll")

if(NOT TARGET SDL2::TTF)
    add_library(SDL2::TTF INTERFACE IMPORTED)
    set_target_properties(SDL2::TTF PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_TTF_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES      "${SDL2_TTF_LIBRARY}"
    )
endif()
