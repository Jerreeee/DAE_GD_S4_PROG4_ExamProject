set(TARGET_NAME JREngine)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/JREngine)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
    "JREngine.cpp"

    "Animation/AnimationClip.cpp"

    "Core/UUID.cpp"
    "Core/Timer.cpp"
    "Core/Event.cpp"
    "Core/Observer.cpp"
    "Core/ServiceLocator.cpp"

    "Input/InputManager.cpp"
    "Input/SDLKeyboard.cpp"
    "Input/SDLKeyboard.h"
    "Input/XBoxController.cpp"
    "Input/XBoxController.h"

    "Rendering/Renderer.cpp"
    "Rendering/Texture2D.cpp"
    "Rendering/Sprite.cpp"
    "Rendering/RendererComponentBase.cpp"
    "Rendering/SpriteRendererComponent.cpp"
    "Rendering/TextRendererComponent.cpp"

    "Resources/Font.cpp"
    "Resources/RuntimeResourceManager.cpp"
    "Resources/EditorResourceManager.cpp"
    "Resources/AssetImporter.cpp"
    "Resources/Asset.cpp"
    "Resources/SpriteEditor.cpp"
    "Resources/TextureImporter.cpp"
    "Resources/FontImporter.cpp"
    "Resources/SoundClipImporter.cpp"
    "Resources/AssetRegistry.cpp"

    "Audio/SDLSoundSystem.cpp"
    "Audio/SDLSoundSystem.h"
    "Audio/SDLSoundClip.cpp"
    "Audio/SDLSoundClip.h"

    "Scene/GameObject.cpp"
    "Scene/Scene.cpp"
    "Scene/SceneManager.cpp"
    "Scene/Transform.cpp"
    "Scene/ComponentBase.cpp"
)
AddPrefix(SOURCE_FILES "${SOURCE_DIR}/")

set(INCLUDE_FILES
    "JREngine.h"

    "Animation/AnimationClip.h"

    "Math/Rect.h"

    "Core/UUID.h"
    "Core/Singleton.h"
    "Core/Command.h"
    "Core/Timer.h"
    "Core/Event.h"
    "Core/Observer.h"
    "Core/ServiceLocator.h"

    "Input/InputUtils.h"
    "Input/InputManager.h"
    "Input/IController.h"
    "Input/IKeyboard.h"

    "Rendering/Renderer.h"
    "Rendering/Texture2D.h"
    "Rendering/Sprite.h"
    "Rendering/RendererComponentBase.h"
    "Rendering/SpriteRendererComponent.h"
    "Rendering/TextRendererComponent.h"

    "Resources/Font.h"
    "Resources/IResourceManager.h"
    "Resources/RuntimeResourceManager.h"
    "Resources/EditorResourceManager.h"
    "Resources/ResourceManager.h"
    "Resources/IAssetImporter.h"
    "Resources/AssetImporter.h"
    "Resources/Asset.h"
    "Resources/SpriteEditor.h"
    "Resources/SoundClipImporter.h"
    "Resources/FontImporter.h"
    "Resources/TextureImporter.h"
    "Resources/AssetRegistry.h"
    "Resources/AssetMetadata.h"

    "Audio/ISoundSystem.h"
    "Audio/ISoundClip.h"

    "Scene/GameObject.h"
    "Scene/Scene.h"
    "Scene/SceneManager.h"
    "Scene/Transform.h"
    "Scene/ComponentBase.h"
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
target_include_directories(${TARGET_NAME}
    PRIVATE
        ${INTERNAL_INCLUDE_DIR}
        ${SOURCE_DIR} #for headers not exposed in include dir
    PUBLIC
        ${EXTERNAL_INCLUDE_DIR}
)

# Enable C++20
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

# Compile warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
