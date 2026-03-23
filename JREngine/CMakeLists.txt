set(TARGET_NAME JREngine)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/JREngine)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
    "JREngine.cpp"

    "Animation/SpriteAnimationClip.cpp"
    "Animation/SpriteAnimatorComponent.cpp"

    "Core/UUID.cpp"
    "Core/Timer.cpp"
    "Core/Event.cpp"
    "Core/Observer.cpp"
    "Core/ServiceLocator.cpp"
    "Core/State.cpp"
    "Core/FiniteStateMachine.cpp"
    "Core/HierarchicalStateMachine.cpp"

    "Input/InputManager.cpp"
    "Input/SDLKeyboard.cpp"
    "Input/SDLKeyboard.h"
    "Input/XBoxController.cpp"
    "Input/XBoxController.h"

    "Rendering/SDLRenderer.cpp"
    "Rendering/SpriteRendererComponent.cpp"
    "Rendering/TextRendererComponent.cpp"
    "Rendering/RendererComponentBase.cpp"

    "Asset/Texture2D.cpp"
    "Asset/Sprite.cpp"
    "Asset/Font.cpp"
    "Asset/RuntimeResourceManager.cpp"
    "Asset/EditorResourceManager.cpp"
    "Asset/AssetDatabase.cpp"
    "Asset/AssetLoaderRegistry.cpp"
    "Asset/Asset.cpp"
    "Asset/SpriteEditor.cpp"
    "Asset/TextureImporter.cpp"
    "Asset/FontImporter.cpp"
    "Asset/SoundClipImporter.cpp"
    "Asset/SoundMusicImporter.cpp"
    "Asset/AssetRegistry.cpp"

    "Audio/SDLSoundSystem.cpp"
    "Audio/SDLSoundSystem.h"
    "Audio/SDLSoundClip.cpp"
    "Audio/SDLSoundClip.h"
    "Audio/SDLSoundMusic.cpp"
    "Audio/SDLSoundMusic.h"

    "Scene/GameObject.cpp"
    "Scene/Scene.cpp"
    "Scene/SceneManager.cpp"
    "Scene/Transform.cpp"
    "Scene/ComponentBase.cpp"

    "Physics/BoxPhysicsSystem.cpp"
    "Physics/RigidBody2DComponent.cpp"
    "Physics/Box2DColliderComponent.cpp"
)
AddPrefix(SOURCE_FILES "${SOURCE_DIR}/")

set(INCLUDE_FILES
    "JREngine.h"

    "Animation/SpriteAnimationClip.h"
    "Animation/SpriteAnimatorComponent.h"

    "Math/Rect.h"

    "Core/UUID.h"
    "Core/Singleton.h"
    "Core/Command.h"
    "Core/Timer.h"
    "Core/Event.h"
    "Core/Observer.h"
    "Core/ServiceLocator.h"
    "Core/State.h"
    "Core/FiniteStateMachine.h"
    "Core/HierarchicalStateMachine.h"

    "Input/InputUtils.h"
    "Input/InputManager.h"
    "Input/IController.h"
    "Input/IKeyboard.h"

    "Rendering/SDLRenderer.h"
    "Rendering/RendererComponentBase.h"
    "Rendering/SpriteRendererComponent.h"
    "Rendering/TextRendererComponent.h"

    "Asset/Texture2D.h"
    "Asset/Sprite.h"
    "Asset/Font.h"
    "Asset/IResourceManager.h"
    "Asset/RuntimeResourceManager.h"
    "Asset/EditorResourceManager.h"
    "Asset/ResourceManager.h"
    "Asset/IAssetImporter.h"
    "Asset/AssetDatabase.h"
    "Asset/AssetLoaderRegistry.h"
    "Asset/Asset.h"
    "Asset/SpriteEditor.h"
    "Asset/SoundClipImporter.h"
    "Asset/SoundMusicImporter.h"
    "Asset/FontImporter.h"
    "Asset/TextureImporter.h"
    "Asset/AssetRegistry.h"
    "Asset/AssetMetadata.h"
    "Asset/SoftAssetRef.h"

    "Audio/ISoundSystem.h"
    "Audio/ISoundClip.h"
    "Audio/ISoundMusic.h"

    "Scene/GameObject.h"
    "Scene/Scene.h"
    "Scene/SceneManager.h"
    "Scene/Transform.h"
    "Scene/ComponentBase.h"
    "Scene/PersistenceLayer.h"

    "Physics/IPhysicsSystem.h"
    "Physics/ICollider.h"
    "Physics/BoxPhysicsSystem.h"
    "Physics/RigidBody2DComponent.h"
    "Physics/Box2DColliderComponent.h"
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
        XInput::XInput
        SDL2::Image
        SDL2::TTF
        SDL2::Mixer
        ImGui
    PUBLIC
        SDL2::SDL2
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
