set(TARGET_NAME JRCore)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/JREngine)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
    "Core/Timer.cpp"
    "Core/UUID.cpp"
    "Core/Event.cpp"
    "Core/Observer.cpp"
    "Core/State.cpp"
    "Core/FiniteStateMachine.cpp"
    "Core/HierarchicalStateMachine.cpp"
    "Core/ServiceLocator.cpp"

    "Scene/Transform.cpp"
    "Scene/ComponentBase.cpp"
    "Scene/GameObject.cpp"
    "Scene/Scene.cpp"
    "Scene/SceneManager.cpp"

    "Rendering/RendererComponentBase.cpp"

    "Asset/Asset.cpp"
    "Asset/AssetRegistry.cpp"

    "Animation/SpriteAnimationClip.cpp"
)
AddPrefix(SOURCE_FILES "${SOURCE_DIR}/")

set(INCLUDE_FILES
    "Animation/SpriteAnimationClip.h"

    "Math/Rect.h"

    "Core/UUID.h"
    "Core/Singleton.h"
    "Core/Command.h"
    "Core/Timer.h"
    "Core/Hash.h"
    "Core/Event.h"
    "Core/Observer.h"
    "Core/ServiceLocator.h"
    "Core/State.h"
    "Core/FiniteStateMachine.h"
    "Core/HierarchicalStateMachine.h"

    "Input/InputUtils.h"
    "Input/IController.h"
    "Input/IKeyboard.h"
    "Input/ActionMap.h"

    "Rendering/RendererComponentBase.h"

    "Asset/Asset.h"
    "Asset/AssetMetadata.h"
    "Asset/IResourceManager.h"
    "Asset/IAssetImporter.h"
    "Asset/AssetRegistry.h"
    "Asset/ResourceManager.h"
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
    "Scene/Prefab.h"

    "Physics/IPhysicsSystem.h"
    "Physics/ICollider.h"
)
AddPrefix(INCLUDE_FILES "${INTERNAL_INCLUDE_DIR}/")

include(glm)

add_library(${TARGET_NAME} STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)
target_include_directories(${TARGET_NAME}
    PRIVATE
        ${INTERNAL_INCLUDE_DIR}
        ${SOURCE_DIR}
    PUBLIC
        ${EXTERNAL_INCLUDE_DIR}
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
