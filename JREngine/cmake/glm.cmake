include(FetchContent)
# add glm
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()