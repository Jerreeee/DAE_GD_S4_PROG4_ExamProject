set(TARGET_NAME BubbleBobble)

set(SOURCE_DIR src)

set(SOURCE_FILES
    "Main.cpp"

    "Events.h"
    "Utils.h"
    "Utils.cpp"
    "EngineSetup.h"

    "GameManager/GameManagerComponent.h"
    "GameManager/GameManagerComponent.cpp"
    "GameManager/IGameState.h"
    "GameManager/MainMenuState.h"
    "GameManager/MainMenuState.cpp"
    "GameManager/LoadingScreenState.h"
    "GameManager/LoadingScreenState.cpp"
    "GameManager/InGameState.h"
    "GameManager/InGameState.cpp"
    "GameManager/GameOverScreenState.h"
    "GameManager/GameOverScreenState.cpp"

    "SceneBuilders/BuilderHelpers.h"
    "SceneBuilders/BuilderHelpers.cpp"
    "SceneBuilders/MainMenuBuilder.h"
    "SceneBuilders/MainMenuBuilder.cpp"
    "SceneBuilders/LoadingScreenBuilder.h"
    "SceneBuilders/LoadingScreenBuilder.cpp"
    "SceneBuilders/LevelBuilder.h"
    "SceneBuilders/LevelBuilder.cpp"
    "SceneBuilders/UIBuilder.h"
    "SceneBuilders/UIBuilder.cpp"
    "SceneBuilders/GameOverScreenBuilder.h"
    "SceneBuilders/GameOverScreenBuilder.cpp"

    "TileMap/TileMapComponent.h"
    "TileMap/TileMapComponent.cpp"
    "TileMap/TileMap.h"
    "TileMap/TileMap.cpp"
    "TileMap/TileMapImporter.h"
    "TileMap/TileMapImporter.cpp"

    "Assets/AnimsData.h"
    "Assets/AnimsDataImporter.h"
    "Assets/AnimsDataImporter.cpp"
    "Assets/LevelData.h"
    "Assets/LevelDataImporter.h"
    "Assets/LevelDataImporter.cpp"
    "Assets/LevelDataComponent.h"

    "Player/PlayerBuilder.h"
    "Player/PlayerBuilder.cpp"
    "Player/PlayerScriptComponent.h"
    "Player/PlayerScriptComponent.cpp"
    "Player/PlayerUtils.h"
    "Player/PlayerController.h"
    "Player/PlayerController.cpp"
    "Player/PlayerStates.h"
    "Player/PlayerStates.cpp"

    "Bubble/BubbleBuilder.h"
    "Bubble/BubbleBuilder.cpp"
    "Bubble/BubbleScript.h"
    "Bubble/BubbleScript.cpp"

    "Enemies/Zenchan/ZenchanBuilder.h"
    "Enemies/Zenchan/ZenchanBuilder.cpp"
    "Enemies/Zenchan/ZenchanScriptComponent.h"
    "Enemies/Zenchan/ZenchanScriptComponent.cpp"
    "Enemies/Zenchan/ZenchanAIController.h"
    "Enemies/Zenchan/ZenchanAIController.cpp"

    "Components/HealthComponent.h"
    "Components/HealthComponent.cpp"
    "Components/ScoreComponent.h"
    "Components/ScoreComponent.cpp"
    "Components/PortalScriptComponent.h"
    "Components/PortalScriptComponent.cpp"

    #"Components/FPSComponent.h"
    #"Components/FPSComponent.cpp"
    #"Components/RotateParentComponent.h"
    #"Components/RotateParentComponent.cpp"
)
AddPrefix(SOURCE_FILES "${SOURCE_DIR}/")

find_package(VLD CONFIG)

# Create executable
add_executable(${TARGET_NAME} ${SOURCE_FILES} "src/Assets/AnimsDataImporter.cpp")
target_link_libraries(${TARGET_NAME} PRIVATE
    ${VLD_LIBRARY}
    JREngine
)
target_include_directories(${TARGET_NAME} PRIVATE
    ${SOURCE_DIR}
    ${VLD_INCLUDE_DIR}
)

set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

# Enable C++20
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

# Compile warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# Copy the Data/Resource dir next to the exe
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
)

# Copy all required ddl's next to the exe
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${TARGET_NAME}> $<TARGET_RUNTIME_DLLS:${TARGET_NAME}>
    COMMAND_EXPAND_LISTS
)
