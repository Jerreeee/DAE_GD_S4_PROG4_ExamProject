set(TARGET_NAME JREngine)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/JREngine)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
    "${SOURCE_DIR}/Font.cpp"
    "${SOURCE_DIR}/GameObject.cpp"
    "${SOURCE_DIR}/InputManager.cpp"
    "${SOURCE_DIR}/JREngine.cpp"
    "${SOURCE_DIR}/Renderer.cpp"
    "${SOURCE_DIR}/ResourceManager.cpp"
    "${SOURCE_DIR}/Scene.cpp"
    "${SOURCE_DIR}/SceneManager.cpp"
    "${SOURCE_DIR}/Texture2D.cpp"
    "${SOURCE_DIR}/Transform.cpp"
    "${SOURCE_DIR}/Timer.cpp"
    "${SOURCE_DIR}/ComponentBase.cpp"
    "${SOURCE_DIR}/SpriteRendererComponent.cpp"
    "${SOURCE_DIR}/TextRendererComponent.cpp"
    "${SOURCE_DIR}/RendererComponentBase.cpp"
    "${SOURCE_DIR}/Controller.h"
    "${SOURCE_DIR}/XBoxController.h"
    "${SOURCE_DIR}/XBoxController.cpp"
    "${SOURCE_DIR}/Keyboard.h"
    "${SOURCE_DIR}/SDLKeyboard.h"
    "${SOURCE_DIR}/SDLKeyboard.cpp"
    "${SOURCE_DIR}/Observer.cpp"
    "${SOURCE_DIR}/Event.cpp"
)

set(INCLUDE_FILES
    "${INTERNAL_INCLUDE_DIR}/Singleton.h"
    "${INTERNAL_INCLUDE_DIR}/Font.h"
    "${INTERNAL_INCLUDE_DIR}/GameObject.h"
    "${INTERNAL_INCLUDE_DIR}/InputUtils.h"
    "${INTERNAL_INCLUDE_DIR}/InputManager.h"
    "${INTERNAL_INCLUDE_DIR}/JREngine.h"
    "${INTERNAL_INCLUDE_DIR}/Renderer.h"
    "${INTERNAL_INCLUDE_DIR}/ResourceManager.h"
    "${INTERNAL_INCLUDE_DIR}/Scene.h"
    "${INTERNAL_INCLUDE_DIR}/SceneManager.h"
    "${INTERNAL_INCLUDE_DIR}/Texture2D.h"
    "${INTERNAL_INCLUDE_DIR}/Transform.h"
    "${INTERNAL_INCLUDE_DIR}/Timer.h"
    "${INTERNAL_INCLUDE_DIR}/ComponentBase.h"
    "${INTERNAL_INCLUDE_DIR}/SpriteRendererComponent.h"
    "${INTERNAL_INCLUDE_DIR}/TextRendererComponent.h"
    "${INTERNAL_INCLUDE_DIR}/RendererComponentBase.h"
    "${INTERNAL_INCLUDE_DIR}/Command.h"
    "${INTERNAL_INCLUDE_DIR}/Observer.h"
    "${INTERNAL_INCLUDE_DIR}/Event.h"
)

include(FetchContent)
# add glm
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8-docking
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(ImGui)

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

# Fetch ImGui-Plot
FetchContent_Declare(
    ImGuiPlot
    GIT_REPOSITORY https://github.com/soulthreads/imgui-plot.git
    GIT_TAG master  # Use master branch, v0.1.0 contains outdated ImGui code
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_Populate(ImGuiPlot)

set(IMGUI_FILES
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui.h

    # backends
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp

    #imgui plot files
    ${imguiplot_SOURCE_DIR}/include/imgui_plot.h
    ${imguiplot_SOURCE_DIR}/src/imgui_plot.cpp
)

add_library(ImGui STATIC ${IMGUI_FILES})
target_include_directories(ImGui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imguiplot_SOURCE_DIR}/include
)
target_link_libraries(ImGui PRIVATE SDL2::SDL2)

find_library(XINPUT_LIBRARY XInput)
if(XINPUT_LIBRARY)
    message(STATUS "XInput library found: ${XINPUT_LIBRARY}")
else()
    message(FATAL_ERROR "XInput library not found")
endif()

# Create static library
add_library(${TARGET_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
# Enable c++20 features
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
# set libraries to link with
target_link_libraries(${TARGET_NAME} PRIVATE
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES} 
    ImGui
    ${XINPUT_LIBRARY}
)

# set all include directories
target_include_directories(${TARGET_NAME} PRIVATE
    ${SOURCE_DIR} #for headers not exposed in include dir
    ${INTERNAL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR} 
    ${SDL2_IMAGE_INCLUDE_DIR} 
    ${SDL2_TTF_INCLUDE_DIR}
    ${imgui_SOURCE_DIR}
    ${imguiplot_SOURCE_DIR}/include
)
target_include_directories(${TARGET_NAME} PUBLIC
    ${EXTERNAL_INCLUDE_DIR}
    ${glm_SOURCE_DIR}
)

message(STATUS "SOURCE_DIR resolved to: ${SOURCE_DIR}")
get_target_property(JRE_INCLUDE_DIRS ${TARGET_NAME} INCLUDE_DIRECTORIES)
message(STATUS "JREngine include dirs: ${JRE_INCLUDE_DIRS}")

set(DLL_DEPENDENCIES
    ${SDL2_LIBRARY_DLL}
    ${SDL2_IMAGE_LIBRARY_DLL}
    ${SDL2_TTF_LIBRARY_DLL}
)

# Attach these paths as a custom property to the engine target
set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_DLLS "${DLL_DEPENDENCIES}"
)

# compile at max warning level + treat warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
