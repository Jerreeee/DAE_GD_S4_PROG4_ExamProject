cmake_minimum_required(VERSION 3.26)
project(Minigin)

if(EMSCRIPTEN)
    message("Using Emscripten!")
endif()

if(EMSCRIPTEN)
    set(TARGET_NAME minigin_web)
    set(TARGET_PARAM )
else()
    set(TARGET_NAME minigin)
    set(TARGET_PARAM WIN32)
endif()

# list source files here
add_executable(${TARGET_NAME} ${TARGET_PARAM}
    "Minigin/Font.cpp"
    "Minigin/Font.h"
    "Minigin/GameObject.cpp"
    "Minigin/GameObject.h"
    "Minigin/InputManager.cpp"
    "Minigin/InputManager.h"
    "Minigin/Minigin.cpp"
    "Minigin/Minigin.h"
    "Minigin/Renderer.cpp"
    "Minigin/Renderer.h"
    "Minigin/ResourceManager.cpp"
    "Minigin/ResourceManager.h"
    "Minigin/Scene.cpp"
    "Minigin/Scene.h"
    "Minigin/SceneManager.cpp"
    "Minigin/SceneManager.h"
    "Minigin/Texture2D.cpp"
    "Minigin/Texture2D.h"
    "Minigin/Transform.cpp"
    "Minigin/Transform.h"
    "Minigin/Timer.cpp"
    "Minigin/Timer.h"
    "Minigin/ComponentBase.cpp"
    "Minigin/ComponentBase.h"
    "Minigin/FPSComponent.cpp"
    "Minigin/FPSComponent.h"
    "Minigin/SpriteRendererComponent.cpp"
    "Minigin/SpriteRendererComponent.h"
    "Minigin/TextRendererComponent.cpp"
    "Minigin/TextRendererComponent.h"
    "Minigin/RotateParentComponent.cpp"
    "Minigin/RotateParentComponent.h"
    "Minigin/RendererComponentBase.h"
    "Minigin/RendererComponentBase.cpp"
    "Minigin/Command.h"
    "Minigin/Controller.h"
    "Minigin/XBoxController.h"
    "Minigin/XBoxController.cpp"
    "Minigin/InputUtils.h"
    "Minigin/Keyboard.h"
    "Minigin/SDLKeyboard.h"
    "Minigin/SDLKeyboard.cpp"
    "Minigin/Observer.h"
    "Minigin/Observer.cpp"
    "Minigin/Event.h"
    "Minigin/Event.cpp"

    "Game/Main.cpp"
    "Game/GameCommands.h"
    "Game/GameCommands.cpp"
    "Game/GameComponents.h"
    "Game/GameComponents.cpp"
    "Game/GameEvents.h"
)

# enable c++20 features
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
# set minigin as the default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})

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
if (WIN32)
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
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
            set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
        else()
            set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
            set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
        endif()
    endif()

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
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib")
            set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll")
        else()
            set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib")
            set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll")
        endif()
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
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib)
            set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll")
        else()
            set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib)
            set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll")
        endif()
    endif()

    find_package(VLD CONFIG)

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
    target_link_libraries(ImGui PRIVATE SDL2::SDL2 SDL2::SDL2main)

    find_library(XINPUT_LIBRARY XInput)
    if(XINPUT_LIBRARY)
        message(STATUS "XInput library found: ${XINPUT_LIBRARY}")
    else()
        message(FATAL_ERROR "XInput library not found")
    endif()

    # set all include directories
    target_include_directories(${TARGET_NAME} PRIVATE
        ${VLD_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR} 
        ${SDL2_IMAGE_INCLUDE_DIR} 
        ${SDL2_TTF_INCLUDE_DIR}
        ${imgui_SOURCE_DIR}
        ${imguiplot_SOURCE_DIR}/include
    )
  
    # set libraries to link with
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${SDL2_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES} 
        ${SDL2_TTF_LIBRARIES} 
        ${VLD_LIBRARY}
        ImGui
        ${XINPUT_LIBRARY}
    )

elseif(NOT EMSCRIPTEN)

    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.10
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL2)
  
    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    set(SDL2IMAGE_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_image)

    FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    set(SDL2TTF_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_ttf)

    target_include_directories("${TARGET_NAME}" PUBLIC 
        ${sdl2_SOURCE_DIR}/include
        ${sdl2_image_SOURCE_DIR}/include
        ${sdl2_ttf_SOURCE_DIR}
    )
    target_link_libraries("${TARGET_NAME}" PUBLIC 
        SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
    )
endif()

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Minigin/
    ${glm_SOURCE_DIR}
)

# compile at max warning level + treat warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

if(EMSCRIPTEN)
    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g")
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\" -sSDL2_IMAGE_FORMATS=['tga','png']")

    # work around https://github.com/emscripten-core/emscripten/issues/11513
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -fno-stack-protector")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/EmscriptenToolchain.cmake)

  # Additional settings for Emscripten build
    set_target_properties(${TARGET_NAME} PROPERTIES
        COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
        LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
  
elseif(WIN32)
    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

    # add post build commands
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
    )
endif()
