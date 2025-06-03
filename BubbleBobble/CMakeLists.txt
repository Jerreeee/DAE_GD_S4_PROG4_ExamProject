set(TARGET_NAME BubbleBobble)

set(SOURCE_DIR src)

set(SOURCE_FILES
    "Main.cpp"
    "GameManager/GameManagerComponent.h"
    "GameManager/GameManagerComponent.cpp"
    "GameManager/IGameState.h"
    "GameManager/MainMenuState.h"
    "GameManager/MainMenuState.cpp"
    "GameManager/LoadingScreenState.h"
    "GameManager/LoadingScreenState.cpp"
    "GameManager/InGameState.h"
    "GameManager/InGameState.cpp"

    "FileIO.h"
    "FileIO.cpp"
    "Utils.h"
    "Utils.cpp"

    "MenuBuilders.h"
    "MenuBuilders.cpp"

    "Events.h"

    "Components/PlayerUIComponent.h"
    "Components/PlayerUIComponent.cpp"
    "Components/ScoreComponent.h"
    "Components/ScoreComponent.cpp"
    "Components/HealthComponent.h"
    "Components/HealthComponent.cpp"
    "Components/FPSComponent.h"
    "Components/RotateParentComponent.h"
    "Components/FPSComponent.cpp"
    "Components/RotateParentComponent.cpp"

    "SceneBuilder.h"
    "SceneBuilder.cpp"

    "Level/TileMapComponent.h"
    "Level/TileMapComponent.cpp"
    "Level/TileMap.h"
    "Level/TileMap.cpp"
    "Level/TileMapImporter.h"
    "Level/TileMapImporter.cpp"

    #"Player/PlayerBuilder.h"
    #"Player/PlayerBuilder.cpp"
   #"Player/PlayerScriptComponent.h"
   #"Player/PlayerScriptComponent.cpp"
    #"Player/PlayerState.h"
    #"Player/PlayerState.cpp"
    #"Player/PlayerCommands.h"
    #"Player/PlayerCommands.cpp"
    "Player/PlayerUtils.h"
)
AddPrefix(SOURCE_FILES "${SOURCE_DIR}/")

find_package(VLD CONFIG)

# Create executable
add_executable(${TARGET_NAME} ${SOURCE_FILES})
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
