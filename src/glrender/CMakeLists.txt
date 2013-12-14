# cumulusGL #

if(GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
  list(APPEND PLATFORM_LIBS ${GLEW_LIBRARY} ${GLFW_LIBRARY})
endif()

file(GLOB HEADERS *.h*)
file(GLOB SRCS *.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  ${PROJECT_SOURCE_DIR}/mayarender
  ${CMAKE_SOURCE_DIR}/externals/include
  ${MAYA_INCLUDE_DIRS}
)

set(PLATFORM_COMPILE_FLAGS
    /DNT_PLUGIN
    /DREQUIRE_IOSTREAM
)

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

f_create_glfw_executable(cumulusGL
  ${SRCS}
  ${HEADERS})

target_link_libraries(cumulusGL
	${PLATFORM_LIBS}
	cumulusCore
	cumulusMaya)
