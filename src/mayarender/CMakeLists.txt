# cumulusMaya #

find_package(Maya 201200)

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

		opengl32.lib
		glu32.lib)
endif(WIN32)

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

file(GLOB HEADERS *.h*)
file(GLOB SRCS *.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  ${CMAKE_SOURCE_DIR}/externals/include
  ${MAYA_INCLUDE_DIRS}
)

message("${CMAKE_LIBRARY_PATH}")
add_library(cumulusMaya
	${SRCS}
	${HEADERS})

set_target_properties(cumulusMaya
	PROPERTIES
	OUTPUT_NAME "CumulusMaya"
	SUFFIX ${PLATFORM_PLUGIN_EXTENSION}
	LINK_FLAGS "${PLATFORM_LINK_FLAGS}")

target_link_libraries(cumulusMaya
  ${MAYA_PLATFORM_LIBS}
  ${GLEW_LIBRARY}
  ${GLFW_LIBRARY}
  tinyobj
  cumulusCore)

install(TARGETS cumulusMaya DESTINATION ${CMAKE_PLUGINDIR_BASE})
