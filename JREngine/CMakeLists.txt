set(TARGET_NAME JREngine)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/JREngine)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
    "JREngine.cpp"

    "Animation/SpriteAnimatorComponent.cpp"

    "Input/InputManager.cpp"
    "Input/SDLKeyboard.cpp"
    "Input/SDLKeyboard.h"
    "Input/XBoxController.cpp"
    "Input/XBoxController.h"

    "Rendering/SDLRenderer.cpp"
    "Rendering/SpriteRendererComponent.cpp"
    "Rendering/TextRendererComponent.cpp"

    "Asset/Texture2D.cpp"
    "Asset/Sprite.cpp"
    "Asset/Font.cpp"
    "Asset/RuntimeResourceManager.cpp"
    "Asset/EditorResourceManager.cpp"
    "Asset/AssetDatabase.cpp"
    "Asset/AssetLoaderRegistry.cpp"
    "Asset/SpriteEditor.cpp"
    "Asset/TextureImporter.cpp"
    "Asset/FontImporter.cpp"
    "Asset/SoundClipImporter.cpp"
    "Asset/SoundMusicImporter.cpp"

    "Audio/SDLSoundSystem.cpp"
    "Audio/SDLSoundSystem.h"
    "Audio/SDLSoundClip.cpp"
    "Audio/SDLSoundClip.h"
    "Audio/SDLSoundMusic.cpp"
    "Audio/SDLSoundMusic.h"

    "Physics/BoxPhysicsSystem.cpp"
    "Physics/RigidBody2DComponent.cpp"
    "Physics/Box2DColliderComponent.cpp"
)
AddPrefix(SOURCE_FILES "${SOURCE_DIR}/")

set(INCLUDE_FILES
    "JREngine.h"

    "Animation/SpriteAnimatorComponent.h"

    "Input/InputManager.h"

    "Rendering/SDLRenderer.h"
    "Rendering/SpriteRendererComponent.h"
    "Rendering/TextRendererComponent.h"

    "Asset/Texture2D.h"
    "Asset/Sprite.h"
    "Asset/Font.h"
    "Asset/RuntimeResourceManager.h"
    "Asset/EditorResourceManager.h"
    "Asset/AssetDatabase.h"
    "Asset/AssetLoaderRegistry.h"
    "Asset/SpriteEditor.h"
    "Asset/SoundClipImporter.h"
    "Asset/SoundMusicImporter.h"
    "Asset/FontImporter.h"
    "Asset/TextureImporter.h"

    "Physics/BoxPhysicsSystem.h"
    "Physics/RigidBody2DComponent.h"
    "Physics/Box2DColliderComponent.h"
)
AddPrefix(INCLUDE_FILES "${INTERNAL_INCLUDE_DIR}/")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(SDL2)
include(ImGui)
include(XInput)

# Create static library
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(${TARGET_NAME}
    PRIVATE
        XInput::XInput
        SDL2::Image
        SDL2::TTF
        SDL2::Mixer
        ImGui
    PUBLIC
        JRCore
        SDL2::SDL2
)
target_include_directories(${TARGET_NAME}
    PRIVATE
        ${INTERNAL_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/JRCore/include/JREngine  # for short-form includes from JRCore headers
        ${SOURCE_DIR}
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
