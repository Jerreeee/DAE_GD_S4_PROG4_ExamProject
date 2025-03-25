# add SDL2
FetchContent_Declare(
    SDL2
    URL https://www.libsdl.org/release/SDL2-devel-2.30.10-VC.zip 
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads 
)
FetchContent_GetProperties(SDL2)
if(NOT SDL2_POPULATED)
    FetchContent_Populate(SDL2)
    set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
    set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/${ARCH}/SDL2.lib;")
    set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/${ARCH}/SDL2.dll")
endif()
# DONT ADD: ${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib

if(NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib"
    )
endif()

if(NOT TARGET SDL2::SDL2main)
    add_library(SDL2::SDL2main INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2main PROPERTIES
        INTERFACE_LINK_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib"
    )
endif()

# add SDL2_image
FetchContent_Declare(
    SDL2_image
    URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.3-VC.zip 
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_image)
if(NOT SDL2_image_POPULATED)
    FetchContent_Populate(SDL2_image)
    set(SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include)
    set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/${ARCH}/SDL2_image.lib")
    set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/${ARCH}/SDL2_image.dll")
endif()

# add SDL2_ttf
FetchContent_Declare(
    SDL2_ttf
    URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-VC.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_ttf)
if(NOT sdl2_ttf_POPULATED)
FetchContent_Populate(SDL2_ttf)
    set(SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include)
    set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/${ARCH}/SDL2_ttf.lib)
    set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/${ARCH}/SDL2_ttf.dll")
endif()