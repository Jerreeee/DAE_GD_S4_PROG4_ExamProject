find_library(XINPUT_LIBRARY XInput)
if(XINPUT_LIBRARY)
    message(STATUS "XInput library found: ${XINPUT_LIBRARY}")

    if(NOT TARGET XInput::XInput)
        add_library(XInput::XInput INTERFACE IMPORTED)
        set_target_properties(XInput::XInput PROPERTIES
            INTERFACE_LINK_LIBRARIES "${XINPUT_LIBRARY}"
        )
    endif()
else()
    message(FATAL_ERROR "XInput library not found")
endif()