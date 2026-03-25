set(TARGET_NAME JREditor)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BB_SRC_DIR  ${CMAKE_SOURCE_DIR}/BubbleBobble/src)

set(SOURCE_FILES
    # Editor core
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AssetDatabase.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/EditorResourceManager.cpp"

    # BubbleBobble asset registration — compiled into the editor until BubbleBobbleLib exists
    "${BB_SRC_DIR}/Assets/AssetManifest.cpp"
    "${BB_SRC_DIR}/Assets/AnimsDataImporter.cpp"
    "${BB_SRC_DIR}/Assets/LevelDataImporter.cpp"
    "${BB_SRC_DIR}/TileMap/TileMap.cpp"
    "${BB_SRC_DIR}/TileMap/TileMapImporter.cpp"
    "${BB_SRC_DIR}/Assets/SceneDescriptorImporter.cpp"
)

set(INCLUDE_FILES
    "${INCLUDE_DIR}/AssetDatabase.h"
    "${INCLUDE_DIR}/EditorResourceManager.h"
)

add_executable(${TARGET_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE JRRuntime)
target_include_directories(${TARGET_NAME}
    PRIVATE
        ${INCLUDE_DIR}
        ${BB_SRC_DIR}
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
