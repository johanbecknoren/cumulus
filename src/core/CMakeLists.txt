# cumulusCore #

file(GLOB HEADERS *.h*)
file(GLOB SRCS *.cpp)

add_library(cumulusCore ${HEADERS} ${SRCS})

set_target_properties(cumulusCore PROPERTIES OUTPUT_NAME core CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(
  cumulusCore
  ${PLATFORM_LIBRARIES})

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INCDIR_BASE}/core
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)