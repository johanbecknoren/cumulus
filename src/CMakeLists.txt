project(Cumulus)

cmake_minimum_required(VERSION 2.8.6)

# Set the directory where the executables will be stored.
set(EXECUTABLE_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Directory where executables will be stored"
	)

# Specify the list of directories to search for cmake modules.
set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
	)

FILE(GLOB HEADERS ./glrender/*.h*)
FILE(GLOB SRCS  ./glrender/*.cpp)

if (NOT DEFINED CMAKE_PLUGINDIR_BASE)
    set( CMAKE_PLUGINDIR_BASE plugin )
endif()

if (WIN32)
	#list(APPEND COMPILER_FLAGS /Wall)
	#list(APPEND COMPILER_FLAGS
	#	/W3
	#	/WX #Treat all warnings as errors
	#	/DNOMINMAX
	#	/D_USE_MATH_DEFINES
	#	)
else()
	if (UNIX)
		list(APPEND COMPILER_FLAGS "-std=c++11")
	endif (UNIX)
	list(APPEND COMPILER_FLAGS -Wall)
endif()

#set(CMAKE_SUPPRESS_REGENERATION TRUE)

find_package(GLFW)
if (GLFW_FOUND)
#	include_directories(
#		${GLFW_INCLUDE_DIR}
#	)
	list(APPEND CUMULUS_LIBRARIES
			${GLFW_LIBRARY})
endif (GLFW_FOUND)

#find_package(Maya 201400)

#if(MAYA_FOUND)
#    add_definitions(
#        -DCUMULUS_FOUND_MAYA
#    )
#else()
#    message(WARNING
#        "Maya was not found "
#        "please add your Maya path to FindMaya.cmake in "
#        "${PROJECT_SOURCE_DIR}/cmake or set it through the MAYA_LOCATION "
#        "cmake command line argument or environment variable."
#    )
#endif()

#add_definitions(-DGLFW_INCLUDE_GLU)

find_package(OpenGL)
if (OPENGL_FOUND)
	find_package(GLEW REQUIRED)
	list(APPEND CUMULUS_LIBRARIES
			${OPENGL_gl_LIBRARY})
endif()

set(PLATFORM_LIBRARIES
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
)

if( OPENGL_FOUND )
	include_directories(
		${OPENGL_INCLUDE_DIR}
	)
	if (GLEW_FOUND)
		include_directories(
			${GLEW_INCLUDE_DIR}
		)
	endif()

endif()

MESSAGE("${COMPILER_FLAGS} .. ${CUMULUS_LIBRARIES}")

set(CMAKE_CXX_FLAGS ${COMPILER_FLAGS})

#add_subdirectory(glrender)
#add_subdirectory(hello)

add_executable(cumulus ${HEADERS} ${SRCS} )

target_link_libraries(cumulus ${CUMULUS_LIBRARIES})