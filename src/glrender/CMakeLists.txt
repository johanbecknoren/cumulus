# cumulusGL #

if(GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
  list(APPEND PLATFORM_LIBS 
	${GLEW_LIBRARY} 
	${GLFW_LIBRARY}
	cumulusCore)
endif()

file(GLOB HEADERS *.h*)
file(GLOB SRCS *.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
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

foreach(MLIB ${PLATFORM_LIBS})
	message("GL: ${MLIB}")
endforeach()

target_link_libraries(cumulusGL
	${PLATFORM_LIBS}
	)
