set(TARGET_NAME JREngine)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/JREngine)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
    "Font.cpp"
    "GameObject.cpp"
    "InputManager.cpp"
    "JREngine.cpp"
    "Renderer.cpp"
    "ResourceManager.cpp"
    "Scene.cpp"
    "SceneManager.cpp"
    "Texture2D.cpp"
    "Transform.cpp"
    "Timer.cpp"
    "ComponentBase.cpp"
    "SpriteRendererComponent.cpp"
    "TextRendererComponent.cpp"
    "RendererComponentBase.cpp"
    "Controller.h"
    "XBoxController.h"
    "XBoxController.cpp"
    "Keyboard.h"
    "SDLKeyboard.h"
    "SDLKeyboard.cpp"
    "Observer.cpp"
    "Event.cpp"
    "ServiceLocator.cpp"
    "SDLSoundSystem.cpp"
    "SDLSoundClip.cpp"
    "ResourceHandle.cpp"
)
AddPrefix(SOURCE_FILES "${SOURCE_DIR}/")

set(INCLUDE_FILES
    "Singleton.h"
    "Font.h"
    "GameObject.h"
    "InputUtils.h"
    "InputManager.h"
    "JREngine.h"
    "Renderer.h"
    "IResourceManager.h"
    "ResourceManager.h"
    "Scene.h"
    "SceneManager.h"
    "Texture2D.h"
    "Transform.h"
    "Timer.h"
    "ComponentBase.h"
    "SpriteRendererComponent.h"
    "TextRendererComponent.h"
    "RendererComponentBase.h"
    "Command.h"
    "Observer.h"
    "Event.h"
    "ISoundSystem.h"
    "ServiceLocator.h"
    "SDLSoundSystem.h"
    "ISoundClip.h"
    "SDLSoundClip.h"
    "ResourceHandle.h"
)
AddPrefix(INCLUDE_FILES "${INTERNAL_INCLUDE_DIR}/")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(glm)
include(SDL2)
include(ImGui)
include(XInput)

# Create static library
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(${TARGET_NAME}
    PRIVATE
        SDL2::SDL2
        SDL2::Image
        SDL2::TTF
        SDL2::Mixer
        ImGui
        XInput::XInput
    PUBLIC
        glm::glm
)
target_include_directories(${TARGET_NAME} PRIVATE
    ${INTERNAL_INCLUDE_DIR}
    ${SOURCE_DIR} #for headers not exposed in include dir
)
target_include_directories(${TARGET_NAME} PUBLIC
    ${EXTERNAL_INCLUDE_DIR}
)

# Enable C++20
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

# Compile warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
