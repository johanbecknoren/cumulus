# cumulusMayaShader #

if(WIN32)

	set(MAYA_LIB_PATH
		${MAYA_LIBRARY_DIR})
    set(PLATFORM_COMPILE_FLAGS
        /DNT_PLUGIN
        /DREQUIRE_IOSTREAM
        /D_AFXDLL
    )
    set(PLATFORM_PLUGIN_EXTENSION
        .mll
    )
    set(PLATFORM_LINK_FLAGS
        "/export:initializePlugin /export:uninitializePlugin"
    )

    set(MAYA_PLATFORM_LIBS
    	${MAYA_LIB_PATH}/Foundation.lib
    	${MAYA_LIB_PATH}/OpenMaya.lib
    	${MAYA_LIB_PATH}/OpenMayaUI.lib
    	${MAYA_LIB_PATH}/OpenMayaAnim.lib
    	${MAYA_LIB_PATH}/OpenMayaFX.lib
	    ${MAYA_LIB_PATH}/OpenMayaRender.lib
		${MAYA_LIB_PATH}/Image.lib
		cumulusCore
		  )
else()
    
    set(PLATFORM_COMPILE_FLAGS
        -D_BOOL
        -DREQUIRE_IOSTREAM
        -DLINUX
    )
    
    set(PLATFORM_PLUGIN_EXTENSION
        .so
    )

	set(MAYA_PLATFORM_LIBS
		${MAYA_OpenMaya_LIBRARY}
		${MAYA_Foundation_LIBRARY}
		${MAYA_OpenMayaUI_LIBRARY}
		${MAYA_OpenMayaAnim_LIBRARY}
		${MAYA_OpenMayaFX_LIBRARY}
		${MAYA_OpenMayaRenderer_LIBRARY}
		${MAYA_Image_LIBRARY}
		cumulusCore
	)
endif(WIN32)

foreach(MLIB ${MAYA_PLATFORM_LIBS})
	message("Maya: ${MLIB}")
endforeach()

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

file(GLOB HEADERS *.h*)
file(GLOB SRCS *.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  ${CMAKE_SOURCE_DIR}/externals/include
  ${MAYA_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
)

add_library(mayaShader SHARED
	${SRCS}
	${HEADERS})

set_target_properties(mayaShader
	PROPERTIES
	OUTPUT_NAME "CumulusMayaShader"
	SUFFIX ${PLATFORM_PLUGIN_EXTENSION}
	LINK_FLAGS "${PLATFORM_LINK_FLAGS}")

target_link_libraries(mayaShader
  ${MAYA_PLATFORM_LIBS}
)

install(TARGETS mayaShader DESTINATION ${CMAKE_PLUGINDIR_BASE})
