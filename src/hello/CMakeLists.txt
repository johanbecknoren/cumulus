if(NOT MAYA_FOUND)
    message(STATUS
        "Maya could not be found."
    )
    return()
endif()

set(PLATFORM_LIBRARIES
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
)

include_directories(
    ${MAYA_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIR}
)

set(SOURCE_FILES
	hello.cpp)

set(HEADER_FILES
)

if(UNIX)
    set(PLATFORM_COMPILE_FLAGS
        -D_BOOL
        -DREQUIRE_IOSTREAM
        -DLINUX
    )
    set(PLATFORM_PLUGIN_EXTENSION
        .so
    )
    set(PLATFORM_LINK_FLAGS
    )
endif(UNIX)

if(WIN32)
    set(PLATFORM_COMPILE_FLAGS
        /D_AFXDLL
        /DNT_PLUGIN
        /DREQUIRE_IOSTREAM
    )
    set(PLATFORM_PLUGIN_EXTENSION
        .mll
    )
    set(PLATFORM_LINK_FLAGS
        "/export:initializePlugin /export:uninitializePlugin"
    )
endif(WIN32)

MESSAGE("pcf:${PLATFORM_COMPILE_FLAGS}")

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

add_library(maya_plugin SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SHADER_FILES}
    ${INC_FILES}
)

set_target_properties(maya_plugin
    PROPERTIES
    OUTPUT_NAME "MayaViewer"
    PREFIX "osd"
    SUFFIX ${PLATFORM_PLUGIN_EXTENSION}
    LINK_FLAGS "${PLATFORM_LINK_FLAGS}"
)

target_link_libraries(maya_plugin
    ${MAYA_Foundation_LIBRARY}
    ${MAYA_OpenMaya_LIBRARY}
    ${MAYA_OpenMayaRender_LIBRARY}
    ${MAYA_OpenMayaUI_LIBRARY}
    ${PLATFORM_LIBRARIES}
)

install(TARGETS maya_plugin DESTINATION ${CMAKE_PLUGINDIR_BASE})