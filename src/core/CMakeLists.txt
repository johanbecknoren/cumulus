# cumulusCore #

file(GLOB CORE_HEADERS [a-z]*.h*)
file(GLOB CORE_SRCS [a-z]*.cpp)

include_directories(
  ${CMAKE_SOURCE_DIR}/externals/include
  )

add_library(cumulusCore ${CORE_HEADERS} ${CORE_SRCS})

set_target_properties(cumulusCore PROPERTIES OUTPUT_NAME core CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(
  cumulusCore
  ${PLATFORM_LIBRARIES}
  ${GLEW_LIBRARY}
  ${GLFW_LIBRARY}
  opengl32.lib
	glu32.lib
  tinyobj)

install(FILES ${CORE_HEADERS}
  DESTINATION ${CMAKE_INCDIR_BASE}/core
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
