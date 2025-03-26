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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(glm)
include(SDL2)
include(ImGui)
include(XInput)

# Create static library
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE
    SDL2::SDL2
    SDL2::Image
    SDL2::TTF
    #${SDL2_LIBRARIES}
    #${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES} 
    ImGui
    XInput::XInput
    PUBLIC
        glm::glm
)
target_include_directories(${TARGET_NAME} PRIVATE
    ${SOURCE_DIR} #for headers not exposed in include dir
    ${INTERNAL_INCLUDE_DIR}
    #${SDL2_INCLUDE_DIR} 
    #${SDL2_IMAGE_INCLUDE_DIR} 
    #${SDL2_TTF_INCLUDE_DIR}
    ${imgui_SOURCE_DIR}
    ${imguiplot_SOURCE_DIR}/include
)
target_include_directories(${TARGET_NAME} PUBLIC
    ${EXTERNAL_INCLUDE_DIR}
)

# Enable C++20
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

# Compile warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

set(DLL_DEPENDENCIES
    ${SDL2_DLL}
    ${SDL2_IMAGE_DLL}
    ${SDL2_TTF_DLL}
)
set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_DLLS "${DLL_DEPENDENCIES}"
)

