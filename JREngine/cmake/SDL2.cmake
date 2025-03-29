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

if(NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 SHARED IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        IMPORTED_LOCATION "${sdl2_SOURCE_DIR}/lib/${ARCH}/SDL2.dll"
        IMPORTED_IMPLIB "${sdl2_SOURCE_DIR}/lib/${ARCH}/SDL2.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${sdl2_SOURCE_DIR}/include"
    )
endif()

if(NOT TARGET SDL2::SDL2main)
    add_library(SDL2::SDL2main STATIC IMPORTED)
    set_target_properties(SDL2::SDL2main PROPERTIES
        IMPORTED_LOCATION "${sdl2_SOURCE_DIR}/lib/${ARCH}/SDL2main.lib"
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

if(NOT TARGET SDL2::Image)
    add_library(SDL2::Image SHARED IMPORTED)
    set_target_properties(SDL2::Image PROPERTIES
        IMPORTED_LOCATION "${sdl2_image_SOURCE_DIR}/lib/${ARCH}/SDL2_image.dll"
        IMPORTED_IMPLIB "${sdl2_image_SOURCE_DIR}/lib/${ARCH}/SDL2_image.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${sdl2_image_SOURCE_DIR}/include"
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

if(NOT TARGET SDL2::TTF)
    add_library(SDL2::TTF SHARED IMPORTED)
    set_target_properties(SDL2::TTF PROPERTIES
        IMPORTED_LOCATION "${sdl2_ttf_SOURCE_DIR}/lib/${ARCH}/SDL2_ttf.dll"
        IMPORTED_IMPLIB "${sdl2_ttf_SOURCE_DIR}/lib/${ARCH}/SDL2_ttf.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${sdl2_ttf_SOURCE_DIR}/include"
    )
endif()
