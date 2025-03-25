FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8-docking
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(ImGui)

# Fetch ImGui-Plot
FetchContent_Declare(
    ImGuiPlot
    GIT_REPOSITORY https://github.com/soulthreads/imgui-plot.git
    GIT_TAG master  # Use master branch, v0.1.0 contains outdated ImGui code
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_Populate(ImGuiPlot)

set(IMGUI_FILES
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui.h

    # backends
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp

    #imgui plot files
    ${imguiplot_SOURCE_DIR}/include/imgui_plot.h
    ${imguiplot_SOURCE_DIR}/src/imgui_plot.cpp
)

add_library(ImGui STATIC ${IMGUI_FILES})
target_include_directories(ImGui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imguiplot_SOURCE_DIR}/include
)
target_link_libraries(ImGui PRIVATE SDL2::SDL2)