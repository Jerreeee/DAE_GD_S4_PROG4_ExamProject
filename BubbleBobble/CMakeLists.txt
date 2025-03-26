set(TARGET_NAME BubbleBobble)

set(SOURCE_DIR src)

set(SOURCE_FILES
    "${SOURCE_DIR}/main.cpp"
    "${SOURCE_DIR}/Commands.h"
    "${SOURCE_DIR}/Commands.cpp"
    "${SOURCE_DIR}/Components.h"
    "${SOURCE_DIR}/Components.cpp"
    "${SOURCE_DIR}/Events.h"
    "${SOURCE_DIR}/FPSComponent.h"
    "${SOURCE_DIR}/RotateParentComponent.h"
    "${SOURCE_DIR}/FPSComponent.cpp"
    "${SOURCE_DIR}/RotateParentComponent.cpp"
)

find_package(VLD CONFIG)

# Create executable
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE
    ${VLD_LIBRARY}
    JREngine
)
target_include_directories(${TARGET_NAME} PRIVATE
    ${VLD_INCLUDE_DIR}
)


set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

# Enable C++20
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

# Compile warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# Copy the Data/Resource dir next to the exe
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
)

# Get DLLs from the JREngine target
# Copy them to the executable's folder
get_target_property(JRENGINE_DLLS JREngine RUNTIME_DLLS)
message(STATUS "JRENGINE_DLLS: ${JRENGINE_DLLS}")
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        ${JRENGINE_DLLS}
        "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)
