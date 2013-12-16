project(Cumulus)

cmake_minimum_required(VERSION 2.8.6)

# Set the directory where the libraries will be stored.
set(LIBRARY_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/lib
    CACHE PATH
    "Directory where all libraries will be stored"
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

if (UNIX)
	SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -Werror -Wall -Werror=sign-compare -Wno-error=unused-variable -Wno-error=unused-but-set-variable -g3")
	include_directories("/usr/X11/include")
else ()
    SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/Wall /WX- /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS")
endif (UNIX)

#set(CMAKE_SUPPRESS_REGENERATION TRUE)

find_package(Maya 201400)

find_package(GLFW)
if (GLFW_FOUND)
	include_directories(
		${GLFW_INCLUDE_DIR}
	)
	set(CUMULUS_LIBRARIES
        ${CUMULUS_LIBRARIES}
			${GLFW_LIBRARY})
	if(WIN32)
	  list(APPEND CUMULUS_LIBRARIES
		"")
	endif(WIN32)
endif (GLFW_FOUND)

add_definitions(-DGLFW_INCLUDE_GLU)

find_package(OpenGL)
if (OPENGL_FOUND)
	find_package(GLEW REQUIRED)
	list(APPEND
        CUMULUS_LIBRARIES
        ${OPENGL_gl_LIBRARY})
endif()

if( OPENGL_FOUND )
	include_directories(
		${OPENGL_INCLUDE_DIR}
	)
	if (GLEW_FOUND)
		include_directories(
			${GLEW_INCLUDE_DIR}
		)
		list(APPEND CUMULUS_LIBRARIES
			${GLEW_LIBRARY})
	endif()

endif()

macro(f_create_glfw_executable target)
  add_executable(${target} ${ARGN})

  if(UNIX)
    target_link_libraries(${target} ${CUMULUS_LIBRARIES} GL X11 m Xxf86vm Xrandr pthread Xi)
  else()
    target_link_libraries(${target} ${CUMULUS_LIBRARIES})
  endif(UNIX)

endmacro()

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} PATH)
add_definitions(-DCMAKE_PROJECT_ROOT_DIR=\"${PARENT_DIR}\")

add_subdirectory(externals/include)
add_subdirectory(core)
add_subdirectory(glrender)
add_subdirectory(mayarender)
# add_subdirectory(mayashader)
